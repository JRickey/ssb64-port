#!/usr/bin/env python3
"""
pointer_audit.py - Audit reloc struct pointer fields for PORT tokenization.

Parses all C headers under src/, extracts struct definitions, and reports
pointer-typed fields that lack a '#ifdef PORT u32 ...' guard. Outputs both
a console summary and a persistent Markdown report.

Usage:
    python tools/pointer_audit.py
    python tools/pointer_audit.py --output port/docs/pointer_audit.md
"""

import re
import sys
import argparse
from pathlib import Path
from collections import defaultdict

# ---------------------------------------------------------------------------
# Configuration
# ---------------------------------------------------------------------------

SRC_ROOT = Path(__file__).parent.parent / "src"

# Structs we know are loaded directly from reloc files (graph roots).
# The linter expands this set transitively via pointer-field types.
RELOC_ROOTS = {
    # Fighter
    "FTAttributes", "FTAccessPart", "FTModelPart", "FTCommonPart",
    "FTModelPartContainer", "FTModelPartDesc", "FTSkeleton", "FTSprites",
    "FTCommonPartContainer",
    # Items / weapons
    "ITAttributes", "WPAttributes",
    # Object / display
    "DObjDesc", "DObjTraDesc", "DObjMultiList", "DObjDLLink",
    "DObjDistDL", "DObjDistDLLink", "AObjEvent32", "MObjSub",
    # Library
    "LBScriptDesc", "LBTextureDesc", "LBTexture",
    # System
    "SYInterpDesc",
}

# Types that are known-good non-struct pointer targets (primitives, opaque
# handles, etc.) — following their pointer chain is not meaningful.
PRIMITIVE_OR_OPAQUE = {
    "void", "char", "u8", "u16", "u32", "u64",
    "s8", "s16", "s32", "s64", "f32", "f64",
    "Gfx", "Vtx", "Mtx", "Lights1",
    "GObj", "DObj", "MObj", "AObj",
    "OSThread", "OSMesgQueue", "OSMesg",
    "u8", "u16", "u32",
}

# ---------------------------------------------------------------------------
# Struct extraction
# ---------------------------------------------------------------------------

def strip_comments(text):
    """Remove // and /* */ comments."""
    # Block comments
    text = re.sub(r'/\*.*?\*/', ' ', text, flags=re.DOTALL)
    # Line comments
    text = re.sub(r'//[^\n]*', '', text)
    return text


def extract_structs(text):
    """
    Return dict of {struct_name: raw_body_text} for all 'struct Name { ... };'
    definitions in text. Handles nested braces.
    """
    structs = {}
    # Find "struct Name" followed (possibly after whitespace/newlines) by '{'
    pattern = re.compile(r'\bstruct\s+(\w+)\s*\{')
    pos = 0
    while True:
        m = pattern.search(text, pos)
        if not m:
            break
        name = m.group(1)
        body_start = m.end()
        # Walk forward counting brace depth
        depth = 1
        i = body_start
        while i < len(text) and depth > 0:
            if text[i] == '{':
                depth += 1
            elif text[i] == '}':
                depth -= 1
            i += 1
        body = text[body_start:i - 1]
        structs[name] = body
        pos = m.start() + 1
    return structs


# ---------------------------------------------------------------------------
# Field analysis
# ---------------------------------------------------------------------------

POINTER_FIELD_RE = re.compile(
    r'^'
    r'(?P<type>[\w\s]+?\*+)'    # type with stars, e.g. "Gfx *" or "MObjSub **"
    r'\s*'
    r'(?P<name>\w+)'             # field name
    r'\s*(?:\[\s*\w+\s*\])*'    # optional array dimensions
    r'\s*;'
)

def analyze_body(body):
    """
    Walk the struct body line by line tracking #ifdef PORT / #else / #endif.
    Returns:
        unguarded  - list of (field_name, raw_type) for pointer fields with no PORT guard
        guarded    - list of (field_name, raw_type) that are correctly guarded
        port_types - set of field names declared as u32 in the PORT branch
    """
    lines = body.splitlines()

    # Stack-based preprocessor state.
    # Each frame: ('port' | 'else' | 'other')
    pp_stack = []

    port_u32_fields = set()   # u32 names seen in the PORT branch
    unguarded = []
    guarded = []

    def in_port_branch():
        return pp_stack and pp_stack[-1] == 'port'

    def in_else_branch():
        return pp_stack and pp_stack[-1] == 'else'

    def visible():
        """Field is compiled on the non-PORT path (N64 / else branch)."""
        # Visible if not inside any ifdef, OR inside the else branch of PORT
        for frame in pp_stack:
            if frame == 'port':
                return False
        return True

    for line in lines:
        stripped = line.strip()

        # ---- preprocessor directives ----
        if stripped.startswith('#ifdef') or stripped.startswith('#if '):
            if re.match(r'#ifdef\s+PORT\b', stripped):
                pp_stack.append('port')
            else:
                pp_stack.append('other')
            continue

        if stripped.startswith('#ifndef'):
            pp_stack.append('other')
            continue

        if stripped == '#else':
            if pp_stack:
                top = pp_stack[-1]
                if top == 'port':
                    pp_stack[-1] = 'else'
                elif top == 'else':
                    pp_stack[-1] = 'port'
                # 'other' else: flip to other-else (still not visible to us)
                else:
                    pp_stack[-1] = 'other-else'
            continue

        if stripped == '#endif':
            if pp_stack:
                pp_stack.pop()
            continue

        if stripped.startswith('#'):
            continue  # other directives

        # ---- u32 declarations in PORT branch ----
        if in_port_branch():
            u32m = re.match(r'^u32\s+(\w+)\s*(?:\[.*?\])?\s*;', stripped)
            if u32m:
                port_u32_fields.add(u32m.group(1))
            continue

        # ---- pointer field detection on visible / else path ----
        if not visible() and not in_else_branch():
            continue

        pm = POINTER_FIELD_RE.match(stripped)
        if not pm:
            continue

        raw_type = pm.group('type').strip()
        name = pm.group('name')

        # Skip function pointers (contains '(')
        if '(' in raw_type:
            continue

        # Guarded: this field appears in the #else branch of a PORT guard
        # and has a corresponding u32 in the PORT branch
        if in_else_branch() and name in port_u32_fields:
            guarded.append((name, raw_type))
        else:
            unguarded.append((name, raw_type))

    return unguarded, guarded, port_u32_fields


def base_type(raw_type):
    """
    Extract the base struct name from a pointer type string.
    "MObjSub **" -> "MObjSub", "Gfx *" -> "Gfx"
    """
    return re.sub(r'[\s\*]+', '', raw_type).rstrip()


# ---------------------------------------------------------------------------
# Graph expansion
# ---------------------------------------------------------------------------

def build_struct_graph(all_structs):
    """
    For each struct, collect the set of struct names it points to.
    Returns dict {struct_name: {referenced_struct_names}}.
    """
    graph = {}
    for name, body in all_structs.items():
        refs = set()
        unguarded, guarded, _ = analyze_body(body)
        for (_, raw_type) in unguarded + guarded:
            bt = base_type(raw_type)
            if bt and bt != name and bt not in PRIMITIVE_OR_OPAQUE:
                refs.add(bt)
        graph[name] = refs
    return graph


def expand_reachable(roots, graph):
    """BFS from roots through pointer graph."""
    visited = set(roots)
    queue = list(roots)
    while queue:
        node = queue.pop()
        for neighbor in graph.get(node, set()):
            if neighbor not in visited and neighbor in graph:
                visited.add(neighbor)
                queue.append(neighbor)
    return visited


# ---------------------------------------------------------------------------
# Report generation
# ---------------------------------------------------------------------------

def run_audit(src_root):
    """
    Parse all headers, return audit results.
    Returns list of dicts with keys:
        header, struct_name, unguarded, guarded, reachable
    """
    all_structs = {}   # name -> body
    struct_file = {}   # name -> header path

    for header in sorted(src_root.rglob("*.h")):
        text = strip_comments(header.read_text(encoding='utf-8', errors='replace'))
        structs = extract_structs(text)
        for name, body in structs.items():
            if name not in all_structs:
                all_structs[name] = body
                struct_file[name] = header.relative_to(src_root.parent)

    graph = build_struct_graph(all_structs)
    reachable = expand_reachable(RELOC_ROOTS, graph)

    results = []
    for name in sorted(reachable):
        if name not in all_structs:
            continue
        body = all_structs[name]
        unguarded, guarded, _ = analyze_body(body)
        results.append({
            "struct": name,
            "header": str(struct_file.get(name, "?")),
            "unguarded": unguarded,
            "guarded": guarded,
            "in_roots": name in RELOC_ROOTS,
        })

    return results, reachable, all_structs, struct_file


# ---------------------------------------------------------------------------
# Output
# ---------------------------------------------------------------------------

def render_markdown(results, reachable, all_structs, struct_file):
    lines = []
    lines.append("# Reloc Struct Pointer Field Audit")
    lines.append("")
    lines.append("Generated by `tools/pointer_audit.py`. "
                 "Each struct reachable from a known reloc root is listed.")
    lines.append("Fields marked **UNGUARDED** need a `#ifdef PORT u32 ... #else type* ... #endif` guard.")
    lines.append("")

    clean = [r for r in results if not r["unguarded"]]
    dirty = [r for r in results if r["unguarded"]]

    lines.append(f"**{len(dirty)} structs with unguarded pointer fields** / "
                 f"{len(clean)} structs fully guarded / "
                 f"{len(results)} total reachable")
    lines.append("")

    if dirty:
        lines.append("## Structs Needing Fixes")
        lines.append("")
        for r in dirty:
            lines.append(f"### `{r['struct']}` — `{r['header']}`")
            if r["in_roots"]:
                lines.append("**(reloc root)**")
            lines.append("")
            lines.append("| Field | Type | Status |")
            lines.append("|---|---|---|")
            for name, raw_type in r["unguarded"]:
                lines.append(f"| `{name}` | `{raw_type}` | UNGUARDED |")
            for name, raw_type in r["guarded"]:
                lines.append(f"| `{name}` | `{raw_type}` | guarded ✓ |")
            lines.append("")

    if clean:
        lines.append("## Fully Guarded Structs")
        lines.append("")
        for r in clean:
            badge = " (root)" if r["in_roots"] else ""
            guarded_summary = ", ".join(f"`{n}`" for n, _ in r["guarded"]) or "—"
            lines.append(f"- `{r['struct']}`{badge} — {r['header']} — guarded: {guarded_summary}")
        lines.append("")

    # Structs reachable but not found in headers (forward-declared elsewhere, etc.)
    missing = sorted(reachable - {r["struct"] for r in results})
    if missing:
        lines.append("## Reachable But Not Found in Headers")
        lines.append("")
        lines.append("These are in the pointer graph but were not parsed "
                     "(may be defined in .c files or external headers):")
        lines.append("")
        for name in missing:
            lines.append(f"- `{name}`")
        lines.append("")

    return "\n".join(lines)


def render_console(results):
    dirty = [r for r in results if r["unguarded"]]
    clean_count = len(results) - len(dirty)

    print(f"\n=== Pointer Audit: {len(dirty)} structs need fixes, "
          f"{clean_count} clean ===\n")

    if not dirty:
        print("All reachable structs are fully guarded.")
        return

    for r in dirty:
        print(f"  {r['struct']} ({r['header']})")
        for name, raw_type in r["unguarded"]:
            print(f"    UNGUARDED: {raw_type} {name}")
        for name, raw_type in r["guarded"]:
            print(f"    guarded:   {raw_type} {name}")
        print()


# ---------------------------------------------------------------------------
# Entry point
# ---------------------------------------------------------------------------

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--output", "-o",
        help="Write Markdown report to this path (default: stdout only)",
    )
    parser.add_argument(
        "--src", default=str(SRC_ROOT),
        help="Path to src/ directory (default: auto-detected from script location)",
    )
    args = parser.parse_args()

    src_root = Path(args.src)
    if not src_root.is_dir():
        print(f"ERROR: src directory not found: {src_root}", file=sys.stderr)
        sys.exit(1)

    results, reachable, all_structs, struct_file = run_audit(src_root)
    render_console(results)

    md = render_markdown(results, reachable, all_structs, struct_file)

    if args.output:
        out = Path(args.output)
        out.parent.mkdir(parents=True, exist_ok=True)
        out.write_text(md, encoding='utf-8')
        print(f"Report written to {out}")
    else:
        print("\n--- Markdown report (pass --output to save) ---\n")
        print(md)


if __name__ == "__main__":
    main()
