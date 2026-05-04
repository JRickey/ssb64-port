#!/usr/bin/env python3
"""
upstream_reconcile.py — Mechanically PORT-guard port modifications against upstream decomp.

For each modified file in the working tree of a git checkout:
  - Mode-only diffs: revert the file mode to HEAD's mode (keep port's content).
  - Whitespace-/line-ending-only diffs: revert content to HEAD's (kills CRLF/whitespace noise).
  - Substantive diffs: walk the difflib opcode stream and wrap each non-equal block in
        #ifdef PORT
        <port lines>
        #else            (only if there are upstream lines to delete)
        <upstream lines>
        #endif
    For pure additions, emit just `#ifdef PORT / <port> / #endif`.
    For pure deletions, emit `#ifndef PORT / <upstream> / #endif`.

Files that already contain ANY `#if(def)? (!? )?PORT` or `#if (!? )?defined(PORT)` are left
untouched — assumed to be hand-curated.

Usage:
    python3 upstream_reconcile.py <git-checkout>
"""

from __future__ import annotations
import argparse
import os
import re
import subprocess
import sys
from difflib import SequenceMatcher
from pathlib import Path

PORT_GUARD_RE = re.compile(
    r"#\s*if(?:def|ndef)?\s+(?:!\s*)?PORT\b"
    r"|#\s*if\s+(?:!\s*)?defined\s*\(\s*PORT\s*\)"
    r"|#\s*if\s+!\s*PORT\b"
)

# File suffixes the wrapping logic is safe to apply to. Header/source only.
WRAPPABLE_SUFFIXES = {".c", ".h", ".cpp", ".hpp", ".inc"}

# Files that are auto-generated and should NOT be committed to the submodule fork —
# the submodule is for source, not codegen artifacts.
AUTO_GEN_FILES = {
    "include/reloc_data.h",
}
AUTO_GEN_GLOBS = (
    re.compile(r"^src/credits/.+\.encoded$"),
    re.compile(r"^src/credits/.+\.metadata$"),
)


def is_auto_generated(rel_path: str) -> bool:
    if rel_path in AUTO_GEN_FILES:
        return True
    return any(rx.match(rel_path) for rx in AUTO_GEN_GLOBS)


def run(cmd: list[str], cwd: Path, check=True, capture=True, text=True):
    return subprocess.run(cmd, cwd=cwd, check=check, capture_output=capture, text=text)


def list_modified(repo: Path) -> list[str]:
    out = run(["git", "status", "--short"], repo).stdout
    files = []
    for line in out.splitlines():
        if line.startswith(" M") or line.startswith("M "):
            files.append(line[3:].strip())
    return files


def get_head_blob(repo: Path, path: str) -> str | None:
    r = subprocess.run(
        ["git", "show", f"HEAD:{path}"],
        cwd=repo, capture_output=True, text=True,
    )
    if r.returncode != 0:
        return None
    return r.stdout


def head_file_mode(repo: Path, path: str) -> str | None:
    """Return the file mode for path at HEAD, e.g. '100644', or None if not tracked."""
    r = subprocess.run(
        ["git", "ls-tree", "HEAD", path],
        cwd=repo, capture_output=True, text=True,
    )
    if r.returncode != 0 or not r.stdout.strip():
        return None
    return r.stdout.split()[0]


def is_mode_only_diff(repo: Path, path: str) -> bool:
    """Returns True if the only diff is a file-mode change (content identical)."""
    full_path = repo / path
    head_blob = get_head_blob(repo, path)
    if head_blob is None:
        return False
    try:
        port_content = full_path.read_text()
    except Exception:
        return False
    return head_blob == port_content


def is_whitespace_only_diff(repo: Path, path: str) -> bool:
    head_blob = get_head_blob(repo, path)
    if head_blob is None:
        return False
    try:
        port_content = (repo / path).read_text()
    except Exception:
        return False
    norm = lambda s: re.sub(r"\s+", " ", s).strip()
    return norm(head_blob) == norm(port_content)


def has_existing_port_guards(content: str) -> bool:
    return bool(PORT_GUARD_RE.search(content))


def wrap_hunks(upstream: str, port: str) -> str:
    """
    Walk a SequenceMatcher opcode stream over (upstream_lines, port_lines).
    Emit upstream lines unchanged where they match; emit PORT-guarded blocks
    around non-matching regions.
    """
    up_lines = upstream.splitlines(keepends=True)
    pt_lines = port.splitlines(keepends=True)

    matcher = SequenceMatcher(a=up_lines, b=pt_lines, autojunk=False)
    out: list[str] = []

    def emit(lines):
        out.extend(lines)

    def emit_line(s: str):
        # Always end with a newline for the directive lines we add.
        if not s.endswith("\n"):
            s += "\n"
        out.append(s)

    for op, i1, i2, j1, j2 in matcher.get_opcodes():
        if op == "equal":
            emit(up_lines[i1:i2])
        elif op == "insert":
            emit_line("#ifdef PORT")
            emit(pt_lines[j1:j2])
            # ensure trailing newline before #endif
            if pt_lines[j1:j2] and not pt_lines[j2-1].endswith("\n"):
                emit_line("")
            emit_line("#endif")
        elif op == "delete":
            emit_line("#ifndef PORT")
            emit(up_lines[i1:i2])
            if up_lines[i1:i2] and not up_lines[i2-1].endswith("\n"):
                emit_line("")
            emit_line("#endif")
        elif op == "replace":
            emit_line("#ifdef PORT")
            emit(pt_lines[j1:j2])
            if pt_lines[j1:j2] and not pt_lines[j2-1].endswith("\n"):
                emit_line("")
            emit_line("#else")
            emit(up_lines[i1:i2])
            if up_lines[i1:i2] and not up_lines[i2-1].endswith("\n"):
                emit_line("")
            emit_line("#endif")
        else:
            raise RuntimeError(f"Unknown opcode: {op}")
    return "".join(out)


def reconcile_file(repo: Path, rel_path: str) -> tuple[str, dict]:
    """
    Returns (status, info_dict).
    status: one of {'auto_gen_dropped', 'mode_revert', 'whitespace_revert',
                    'already_guarded', 'wrapped', 'wrapped_skipped_unsafe', 'untouched'}
    """
    full = repo / rel_path
    info = {}

    if is_auto_generated(rel_path):
        # Restore upstream's version (drop port's auto-generated artifact)
        head_blob = get_head_blob(repo, rel_path)
        if head_blob is not None:
            full.write_text(head_blob)
        return "auto_gen_dropped", info

    # Mode-only?
    if is_mode_only_diff(repo, rel_path):
        # Just `git checkout HEAD -- <path>` to restore mode (content is identical).
        run(["git", "checkout", "HEAD", "--", rel_path], repo)
        return "mode_revert", info

    head_blob = get_head_blob(repo, rel_path)
    if head_blob is None:
        # Port-only file (added by port). Keep as-is.
        return "untouched", info

    # Whitespace-only? Use upstream content (mode stays from working tree).
    if is_whitespace_only_diff(repo, rel_path):
        full.write_text(head_blob)
        return "whitespace_revert", info

    try:
        port_text = full.read_text()
    except Exception as e:
        info["error"] = str(e)
        return "untouched", info

    # Already has PORT guards somewhere — leave it alone.
    if has_existing_port_guards(port_text):
        return "already_guarded", info

    # Skip wrapping for non-source files (text data, etc.)
    if Path(rel_path).suffix not in WRAPPABLE_SUFFIXES:
        return "wrapped_skipped_unsafe", info

    wrapped = wrap_hunks(head_blob, port_text)
    full.write_text(wrapped)
    return "wrapped", info


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("repo", type=Path, help="git checkout to reconcile (e.g. /tmp/decomp-prep)")
    ap.add_argument("--dry-run", action="store_true")
    args = ap.parse_args()

    repo: Path = args.repo.resolve()
    if not (repo / ".git").exists():
        sys.exit(f"Not a git checkout: {repo}")

    files = list_modified(repo)
    print(f"Found {len(files)} modified files in {repo}\n")

    counters: dict[str, int] = {}
    examples: dict[str, list[str]] = {}
    for f in files:
        if args.dry_run:
            # Read-only categorization
            if is_auto_generated(f):
                status = "auto_gen_dropped"
            elif is_mode_only_diff(repo, f):
                status = "mode_revert"
            elif (head := get_head_blob(repo, f)) is None:
                status = "untouched"
            elif is_whitespace_only_diff(repo, f):
                status = "whitespace_revert"
            else:
                try:
                    port_text = (repo / f).read_text()
                except Exception:
                    status = "untouched"; counters[status] = counters.get(status, 0)+1; continue
                if has_existing_port_guards(port_text):
                    status = "already_guarded"
                elif Path(f).suffix not in WRAPPABLE_SUFFIXES:
                    status = "wrapped_skipped_unsafe"
                else:
                    status = "wrapped"
        else:
            status, _ = reconcile_file(repo, f)
        counters[status] = counters.get(status, 0) + 1
        examples.setdefault(status, []).append(f)

    print("=== Reconciliation summary ===")
    for k in sorted(counters):
        print(f"  {k:<28s}  {counters[k]}")
    print()
    for k, ex in examples.items():
        print(f"--- {k} (showing up to 5) ---")
        for f in ex[:5]:
            print(f"    {f}")


if __name__ == "__main__":
    main()
