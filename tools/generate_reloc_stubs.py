#!/usr/bin/env python3
"""
generate_reloc_stubs.py — Emit include/reloc_data.h as a complete
#define-only mirror of tools/reloc_data_symbols.us.txt.

The decomp references ~3900 `ll*` linker symbols across src/. On N64 the
MIPS linker turns each one into an absolute address constant via
`symbols/reloc_data_symbols.us.txt` (vendored here as
tools/reloc_data_symbols.us.txt). On PC we cannot use a MIPS linker, but
the decomp uses many of these symbols as file-scope struct-literal
initialisers — which C11 only accepts if they are **constant
expressions**, not `extern` variables. So we emit a `#define`-only
header whose entries are the real numeric values from the symbols table.

The header includes **every** symbol in the vendored table, not just the
subset that src/ currently references. That matters because downstream
tools (tools/generate_yamls.py) parse reloc_data.h to turn `ll*FileID`
symbols into human-readable archive paths — `DkIcon`, `MasterHandIcon`,
etc. — and the game looks those files up by file_id at runtime even
though the `ll*` symbol names never appear in src/. Filtering to
src/-referenced symbols silently drops entries from the name table and
causes Torch to extract the files under their fallback `file_NNNN`
names, which then don't match `port/resource/RelocFileTable.cpp`.

Any `ll*` identifier that src/ references but that is missing from the
symbols table (e.g. a brand-new symbol added to the decomp sources) is
still emitted, as an `((intptr_t)0)` fallback with a `/* STUBBED */`
comment so it can be grepped and triaged later.

Generates:
    include/reloc_data.h  — `#define <name> ((intptr_t)<value>)` per
                            symbol in the table, plus stubs for any
                            extra `ll*` names referenced by src/.

Usage: run from the repo root.
    python3 tools/generate_reloc_stubs.py
"""

import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
# Decomp source lives in the `decomp/` submodule (port-patches branch). The
# pre-submodule path was ROOT/src; it's still on disk during the submodule
# migration but slated for deletion. Prefer the submodule path when present.
SRC_DIR = (ROOT / "decomp" / "src") if (ROOT / "decomp" / "src").is_dir() else (ROOT / "src")
HEADER_OUT = ROOT / "include" / "reloc_data.h"
SYMBOLS_TXT = ROOT / "tools" / "reloc_data_symbols.us.txt"

SYMBOL_RE = re.compile(r"\bll[A-Z_][A-Za-z0-9_]*\b")
ASSIGN_RE = re.compile(r"^\s*(ll[A-Za-z_][A-Za-z0-9_]*)\s*=\s*([^;]+?)\s*;\s*$")


def collect_src_symbols() -> set[str]:
    """Scan src/ for every `ll*` identifier referenced in the decomp.

    Used only to detect `ll*` names that are not in the symbols table —
    the actual header contents come from parse_symbol_values().
    """
    symbols: set[str] = set()
    for path in SRC_DIR.rglob("*"):
        if path.suffix not in (".c", ".h"):
            continue
        try:
            text = path.read_text(encoding="utf-8", errors="ignore")
        except Exception:
            continue
        for match in SYMBOL_RE.findall(text):
            symbols.add(match)
    return symbols


def parse_symbol_values() -> dict[str, str]:
    """Parse tools/reloc_data_symbols.us.txt into {name: value_literal}.

    Blank lines and `#`-comment lines are ignored. Values are kept as
    their original textual form (e.g. `0x73c0`, `2132`) so the generated
    header preserves the same base as upstream — it reads better when
    you grep for a specific file ID.
    """
    if not SYMBOLS_TXT.is_file():
        print(
            f"error: {SYMBOLS_TXT.relative_to(ROOT)} not found — "
            "the generated header will only contain stubs.",
            file=sys.stderr,
        )
        return {}
    values: dict[str, str] = {}
    for lineno, raw in enumerate(SYMBOLS_TXT.read_text(encoding="utf-8").splitlines(), 1):
        stripped = raw.strip()
        if not stripped or stripped.startswith("#"):
            continue
        m = ASSIGN_RE.match(raw)
        if not m:
            print(
                f"warning: {SYMBOLS_TXT.name}:{lineno}: unrecognised line: {raw!r}",
                file=sys.stderr,
            )
            continue
        name, literal = m.group(1), m.group(2)
        values[name] = literal
    return values


def write_header(values: dict[str, str], extra_stubs: set[str]) -> None:
    # Preserve upstream order inside the table, then append any extra
    # src/-only stubs sorted for determinism.
    table_names = list(values.keys())
    stub_names = sorted(extra_stubs)

    lines = [
        "#ifndef _RELOC_DATA_H_",
        "#define _RELOC_DATA_H_",
        "",
        "/*",
        " * reloc_data.h — AUTO-GENERATED. Run tools/generate_reloc_stubs.py to",
        " * regenerate after updating tools/reloc_data_symbols.us.txt or after",
        " * adding new decomp sources that reference `ll*` linker symbols.",
        " *",
        " * Every symbol in tools/reloc_data_symbols.us.txt (vendored from",
        " * ssb-decomp-re) is mirrored here as a #define holding the real",
        " * numeric value, so downstream tools (generate_yamls.py,",
        " * generate_reloc_table.py) always see the full name table even if",
        " * the individual symbol isn't referenced from src/ directly.",
        " *",
        " * Any `ll*` name referenced by src/ but missing from the table is",
        " * emitted as `((intptr_t)0)` with a trailing STUBBED marker so it",
        " * can be grep'd and triaged later — search the file for 'STUBBED'.",
        " *",
        " * Each symbol is emitted as a `#define` (rather than `extern intptr_t`)",
        " * so it works as a compile-time integer constant inside file-scope",
        " * struct initialisers, which C11 does not allow for external variables.",
        " */",
        "",
        "#include <stdint.h>",
        "",
        f"/* {len(table_names)} symbols from the vendored table, "
        f"{len(stub_names)} extra stubs from src/. */",
        "",
    ]

    # Use `((intptr_t)<literal>)` for every entry so downstream tools
    # (tools/generate_yamls.py) see a consistent regex-friendly format.
    for name in table_names:
        lines.append(f"#define {name} ((intptr_t){values[name]})")

    if stub_names:
        lines.append("")
        lines.append("/* ll* symbols referenced by src/ but absent from the symbols table. */")
        for name in stub_names:
            lines.append(f"#define {name} ((intptr_t)0) /* STUBBED */")

    lines += ["", "#endif /* _RELOC_DATA_H_ */", ""]
    new_content = "\n".join(lines)
    # Skip the write when the existing file already has identical contents.
    # The output is ~340 KB and regenerated on every build trigger; a stale
    # editor / reader (pylance, antivirus) holding even a brief shared lock
    # would otherwise crash the build with PermissionError.
    if HEADER_OUT.exists():
        try:
            if HEADER_OUT.read_text(encoding="utf-8") == new_content:
                return
        except OSError:
            pass
    HEADER_OUT.write_text(new_content, encoding="utf-8")


def main() -> None:
    values = parse_symbol_values()
    src_symbols = collect_src_symbols()
    extra_stubs = src_symbols - set(values.keys())

    print(f"Loaded {len(values)} symbols from {SYMBOLS_TXT.relative_to(ROOT)}")
    print(f"Scanned src/ and found {len(src_symbols)} ll* references")
    print(f"  {len(src_symbols & set(values.keys()))} referenced symbols resolve from the table")
    print(f"  {len(extra_stubs)} referenced symbols stub to ((intptr_t)0)")

    write_header(values, extra_stubs)
    print(f"Wrote {HEADER_OUT.relative_to(ROOT)} "
          f"({len(values) + len(extra_stubs)} total #defines)")


if __name__ == "__main__":
    main()
