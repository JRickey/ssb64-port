#!/usr/bin/env python3
"""
phase2_msvc_corpus.py — sweep the source-compile pipeline across the
eligible-set with both clang and MSVC backends; bucket failures.

Drives build_reloc_resource.py once per file per backend, captures
stderr and the resulting .relo SHA-256, and prints a summary plus a
JSON report at <build>/phase2_results.json.

Usage (from a VS x64 Developer Shell with LLVM/clang on PATH):

    python tools/phase2_msvc_corpus.py \
        --build-dir   build \
        --clang       clang \
        --msvc-wrapper build/_relocdata_msvc_env.bat \
        --jobs        8 \
        [--limit N]                  # quick shake-out: only first N files

Reads the eligible-set from build/reloc_data_targets.cmake (produced
by gen_reloc_cmake.py at configure time). The .inc.c extract dir
(build/inc_c_extracts) is required for any file using #include
<*/*.inc.c>; without it those files fail to compile on both backends.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import os
import re
import subprocess
import sys
import time
from concurrent.futures import ProcessPoolExecutor, as_completed
from pathlib import Path
from collections import defaultdict, Counter

# Match: add_reloc_resource(<id> ${CMAKE_SOURCE_DIR}/decomp/src/relocData/<file>.c "<res_path>")
_TARGET_RE = re.compile(
    r'^add_reloc_resource\((\d+)\s+'
    r'\$\{CMAKE_SOURCE_DIR\}/decomp/src/relocData/(\S+\.c)\s+'
    r'"([^"]+)"\)'
)


def parse_eligible_set(targets_cmake: Path) -> list[tuple[int, str, str]]:
    """Returns list of (file_id, source_basename, resource_path)."""
    out = []
    for line in targets_cmake.read_text(encoding="utf-8").splitlines():
        m = _TARGET_RE.match(line.strip())
        if m:
            out.append((int(m.group(1)), m.group(2), m.group(3)))
    return out


def build_one(task: dict) -> dict:
    """Worker entry. Builds one file with one backend; returns result dict."""
    repo         = Path(task["repo"])
    build_dir    = Path(task["build_dir"])
    file_id      = task["file_id"]
    src_basename = task["src"]
    kind         = task["kind"]
    cc           = task["cc"]
    wrapper      = task.get("wrapper")

    src_path   = repo / "decomp" / "src" / "relocData" / src_basename
    reloc_path = src_path.with_suffix(".reloc")
    out_path   = build_dir / "phase2_outputs" / f"{kind}_{file_id}.relo"
    out_path.parent.mkdir(parents=True, exist_ok=True)

    cmd = [
        sys.executable,
        str(repo / "tools" / "build_reloc_resource.py"),
        "--src",      str(src_path),
        "--reloc",    str(reloc_path),
        "--file-id",  str(file_id),
        "--cc",       cc,
        "--cc-kind",  kind,
        "--include-dir", str(repo / "decomp" / "include"),
        "--include-dir", str(repo / "decomp" / "src"),
        "--include-dir", str(repo / "decomp" / "src" / "relocData"),
        "--include-dir", str(build_dir / "inc_c_extracts"),
        "--output",   str(out_path),
    ]

    # MSVC needs the configure-time env wrapper. Prefix the command with
    # it on Windows; on non-Windows, never set wrapper (clang-only host).
    if kind == "msvc":
        if not wrapper:
            return {
                "file_id": file_id, "src": src_basename, "kind": kind,
                "exit": -1, "ok": False, "stderr": "no wrapper provided",
                "elapsed": 0.0,
            }
        cmd = [str(wrapper)] + cmd

    t0 = time.monotonic()
    try:
        res = subprocess.run(
            cmd, capture_output=True, text=True, timeout=120,
            errors="replace")
    except subprocess.TimeoutExpired:
        return {
            "file_id": file_id, "src": src_basename, "kind": kind,
            "exit": -1, "ok": False, "stderr": "TIMEOUT (>120s)",
            "elapsed": time.monotonic() - t0,
        }

    elapsed = time.monotonic() - t0
    ok = (res.returncode == 0 and out_path.exists())
    info = {
        "file_id": file_id, "src": src_basename, "kind": kind,
        "exit": res.returncode, "ok": ok,
        # stdout includes the python script's own emit message; useful
        # mostly for debugging. Keep stderr in full when failing.
        "stderr": (res.stderr or "")[:4000] if not ok else "",
        "elapsed": elapsed,
    }
    if ok:
        blob = out_path.read_bytes()
        info["sha256"] = hashlib.sha256(blob).hexdigest()
        info["size"] = len(blob)
    return info


# ───────────────────────────── failure bucketing ───────────────────────


def first_error_signature(stderr: str) -> str:
    """Reduce a multi-line stderr to a single short pattern for bucketing.

    Heuristic: take the first MSVC error code (e.g. "C2143") + short
    surrounding context, or the first 'fatal error' line, or fall back
    to the first non-empty line.
    """
    if not stderr:
        return "<no stderr>"
    text = stderr.replace("\r\n", "\n")
    # MSVC C-family error code with the short message after it.
    m = re.search(r"(error C\d{4}):\s*([^\n]{0,120})", text)
    if m:
        return f"{m.group(1)}: {m.group(2).strip()}"
    # Header-not-found before any C2xxx fires.
    m = re.search(r"(fatal error C1083):\s*([^\n]{0,120})", text)
    if m:
        return f"{m.group(1)}: {m.group(2).strip()}"
    # clang-style errors.
    m = re.search(r"error:\s*([^\n]{0,120})", text)
    if m:
        return f"clang error: {m.group(1).strip()}"
    # python traceback (usually means our pipeline crashed, not the cc).
    m = re.search(r"^([A-Za-z_]\w*Error):\s*([^\n]{0,120})", text, re.MULTILINE)
    if m:
        return f"{m.group(1)}: {m.group(2).strip()}"
    # Fallback: first non-empty line.
    for line in text.split("\n"):
        line = line.strip()
        if line:
            return line[:160]
    return "<empty stderr>"


# ───────────────────────────── main ───────────────────────────────────


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--build-dir", type=Path, required=True,
        help="path to the cmake build dir (must contain reloc_data_targets.cmake)")
    ap.add_argument("--repo", type=Path, default=None,
        help="project root (default: parent of build dir)")
    ap.add_argument("--clang", default="clang",
        help="path to clang (default: 'clang' on PATH)")
    ap.add_argument("--msvc-wrapper", type=Path, default=None,
        help="path to the cmake-generated MSVC env wrapper batch")
    ap.add_argument("--msvc-cc", default="cl",
        help="cl.exe to invoke through the wrapper (default: 'cl' on the "
             "wrapper's captured PATH)")
    ap.add_argument("--jobs", type=int, default=os.cpu_count() or 4,
        help="parallel worker count")
    ap.add_argument("--limit", type=int, default=0,
        help="if >0, only run on the first N eligible files (shake-out)")
    ap.add_argument("--skip-clang", action="store_true",
        help="don't run the clang backend (msvc-only sweep)")
    ap.add_argument("--skip-msvc", action="store_true",
        help="don't run the MSVC backend (clang-only sweep)")
    ap.add_argument("--report", type=Path, default=None,
        help="JSON report path (default: <build-dir>/phase2_results.json)")
    args = ap.parse_args()

    repo = args.repo or args.build_dir.parent
    targets_file = args.build_dir / "reloc_data_targets.cmake"
    if not targets_file.exists():
        print(f"error: {targets_file} not found — run cmake configure first",
              file=sys.stderr)
        return 2

    if not args.skip_msvc and not args.msvc_wrapper:
        # Try the standard path the cmake module emits.
        cand = args.build_dir / "_relocdata_msvc_env.bat"
        if cand.exists():
            args.msvc_wrapper = cand
        else:
            print("error: --msvc-wrapper not given and "
                  f"{cand} doesn't exist (run cmake configure with "
                  "RELOCDATA_COMPILER=msvc or auto first)", file=sys.stderr)
            return 2

    eligible = parse_eligible_set(targets_file)
    if args.limit > 0:
        eligible = eligible[:args.limit]

    print(f"Phase 2 corpus sweep: {len(eligible)} files, "
          f"{args.jobs} parallel workers", flush=True)

    backends = []
    if not args.skip_clang:
        backends.append(("clang", args.clang, None))
    if not args.skip_msvc:
        backends.append(("msvc",  args.msvc_cc, str(args.msvc_wrapper)))

    tasks = []
    for file_id, src, _res in eligible:
        for kind, cc, wrapper in backends:
            tasks.append({
                "repo": str(repo),
                "build_dir": str(args.build_dir),
                "file_id": file_id,
                "src": src,
                "kind": kind,
                "cc": cc,
                "wrapper": wrapper,
            })

    # Run in a process pool. ProcessPoolExecutor on Windows uses spawn,
    # so the worker function must be importable at module top-level
    # (build_one is — see above).
    results: list[dict] = []
    t0 = time.monotonic()
    completed = 0
    with ProcessPoolExecutor(max_workers=args.jobs) as ex:
        for fut in as_completed(ex.submit(build_one, t) for t in tasks):
            results.append(fut.result())
            completed += 1
            if completed % 200 == 0 or completed == len(tasks):
                elapsed = time.monotonic() - t0
                rate = completed / elapsed if elapsed else 0
                print(f"  {completed:>5}/{len(tasks)}  "
                      f"({rate:.1f} files/s, "
                      f"{elapsed:.0f}s elapsed)", flush=True)

    elapsed_total = time.monotonic() - t0

    # ── Index by (file_id, kind) ────────────────────────────────────
    by_kind: dict[str, dict[int, dict]] = defaultdict(dict)
    for r in results:
        by_kind[r["kind"]][r["file_id"]] = r

    # ── Report ──────────────────────────────────────────────────────
    print(f"\n=== Phase 2 results ({elapsed_total:.0f}s wall) ===\n")

    summary: dict = {"total_files": len(eligible), "elapsed_s": elapsed_total}

    for kind in sorted(by_kind):
        kind_results = by_kind[kind]
        ok    = sum(1 for r in kind_results.values() if r["ok"])
        fail  = len(kind_results) - ok
        print(f"{kind} backend:  {ok}/{len(kind_results)} ok  ({fail} failed)")
        summary[f"{kind}_ok"] = ok
        summary[f"{kind}_fail"] = fail

    # Backend-vs-backend byte equivalence (only when both ran).
    if "clang" in by_kind and "msvc" in by_kind:
        identical, differ, only_one = 0, 0, 0
        diff_files: list[tuple[int, str]] = []
        for fid, c_res in by_kind["clang"].items():
            m_res = by_kind["msvc"].get(fid)
            if not m_res:
                continue
            if c_res["ok"] and m_res["ok"]:
                if c_res["sha256"] == m_res["sha256"]:
                    identical += 1
                else:
                    differ += 1
                    if len(diff_files) < 30:
                        diff_files.append((fid, c_res["src"]))
            else:
                only_one += 1
        print(f"\nbyte-identity: {identical} identical, {differ} differ, "
              f"{only_one} only-one-backend-built")
        summary["byte_identical"] = identical
        summary["byte_differ"] = differ
        summary["byte_only_one"] = only_one
        if diff_files:
            print(f"\nfirst {len(diff_files)} byte-different files:")
            for fid, name in diff_files[:30]:
                print(f"  {fid:>4}  {name}")

    # ── Failure buckets per backend ─────────────────────────────────
    for kind in sorted(by_kind):
        fails = [r for r in by_kind[kind].values() if not r["ok"]]
        if not fails:
            continue
        bucket: Counter[str] = Counter()
        bucket_examples: dict[str, list[tuple[int, str]]] = defaultdict(list)
        for r in fails:
            sig = first_error_signature(r["stderr"])
            bucket[sig] += 1
            if len(bucket_examples[sig]) < 3:
                bucket_examples[sig].append((r["file_id"], r["src"]))

        print(f"\n=== {kind} failure buckets ({len(fails)} fails, "
              f"{len(bucket)} distinct signatures) ===")
        for sig, n in bucket.most_common(20):
            print(f"  {n:>4}  {sig}")
            for fid, name in bucket_examples[sig]:
                print(f"        e.g. {fid:>4} {name}")
        summary[f"{kind}_buckets"] = [
            {"sig": sig, "count": n,
             "examples": bucket_examples[sig]}
            for sig, n in bucket.most_common()
        ]

    # ── JSON report ─────────────────────────────────────────────────
    report_path = args.report or (args.build_dir / "phase2_results.json")
    report_path.write_text(json.dumps({
        "summary": summary,
        "results": results,
    }, indent=2), encoding="utf-8")
    print(f"\nJSON report: {report_path}")

    # Exit non-zero only if the clang regression check itself broke —
    # MSVC failures are expected to exist before triage, not a test fail.
    if "clang" in by_kind and summary.get("clang_fail", 0) > 0:
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
