#!/usr/bin/env python3
"""
extract_inc_c.py — port-side equivalent of upstream's tools/extractRelocInc.py.

Reads each decomp/src/relocData/<id>_<Name>.c file, finds every
`#include "<File>/<sym>.<type>.inc.c"` line, looks up the matching
declaration's symbol offset within the file's data, slices those bytes
from the corresponding Torch-extracted resource in BattleShip.o2r,
and emits a properly-formatted .inc.c.

Format conventions (matching upstream's emitters):
  .vtx.inc.c     each Vtx (16B) → `{{{x,y,z}, 0xFLAG, {s,t}, {0xR,0xG,0xB,0xA}}}` (BE-decoded)
  .dl.inc.c      each Gfx (8B)  → `{{0xWWXXYYZZ, 0xAABBCCDD}}` BE u32 word pair
  .palette.inc.c 16x u16 (32B)  → `0xHHHH, 0xHHHH, ...` (BE u16)
  .data.inc.c    raw u8         → `0xHH, 0xHH, ...`
  .tex.inc.c     raw u8         → `0xHH, 0xHH, ...`

Symbol offset resolution (no IDO/nm):
  1. Suffix `_0x<HEX>_sub_0x<HEX>` → primary + sub
  2. Suffix `_0x<HEX>` → primary
  3. Else: lookup in decomp/tools/relocFileDescriptions.us.txt FILE CONTENTS section
     (the [N] block lists named symbols + offsets per file_id)

Symbol size is computed from declaration's array length × type size:
  Vtx: 16, Gfx: 8, u8: 1, u16: 2, u32: 4

Usage:
    extract_inc_c.py
        --battleship-o2r build/BattleShip.o2r
        --reloc-table    port/resource/RelocFileTable.cpp
        --reloc-dir      decomp/src/relocData
        --descriptions   decomp/tools/relocFileDescriptions.us.txt
        --output-dir     build/inc_c_extracts

Output structure: <output-dir>/<FileName>/<sym>.<type>.inc.c
This matches upstream's `<file>/<sym>.<type>.inc.c` include path.
"""

from __future__ import annotations

import argparse
import re
import struct
import sys
import zipfile
from collections import defaultdict
from dataclasses import dataclass
from pathlib import Path

HEADER_SIZE = 0x40  # LUS BinaryResource header preceding RelocFile fields

TABLE_RE = re.compile(r'^\s*"([^"]+)",\s*/\*\s*(\d+)\s*\*/')
RELOCDATA_FILE_RE = re.compile(r"^(\d+)_([A-Za-z0-9_]+)\.c$")

# Matches `<TypeName> dXxx_<Name>[<count>] = {`
# Type covers raw-byte u8/u16/u32 wrappers and typed Vtx / Gfx / DObjDesc /
# MObjSub / etc. Count may be empty (auto-sized) — caller must derive size.
DECL_RE = re.compile(
    r"^\s*(u8|u16|u32|Vtx|Gfx|DObjDesc|MObjSub|MatAnimJoint|AnimJoint|"
    r"CamAnimJoint|MapHead|FTSpecialColl|GRAttackColl|HitDesc|HitParties|"
    r"BloatScales|MPGeometryData|SpriteArray|Sprite|Bitmap)\s+"
    r"(d\w+)\s*\[(\d*)\]\s*=\s*\{",
    re.M,
)

INCLUDE_RE = re.compile(
    r'#\s*include\s+[<"]([^>"]*)\.(vtx|dl|palette|data|tex)\.inc\.c[>"]'
)

# Symbol offset patterns
SYM_OFF_PRIMARY = re.compile(r"_0x([0-9a-fA-F]+)$")
SYM_OFF_SUB = re.compile(r"_0x([0-9a-fA-F]+)_sub_0x([0-9a-fA-F]+)$")


@dataclass
class Decl:
    type_name: str       # u8/u16/u32/Vtx/Gfx
    sym: str             # full symbol e.g. dStageExplainFile2_gap_0x0040
    count: int           # array length (in elements)
    inc_dir: str         # filename inside the include path (e.g. StageExplainFile2)
    inc_sym: str         # the part before the .<type>.inc.c (e.g. gap_0x0040)
    inc_type: str        # vtx/dl/palette/data/tex


# Type sizes (in bytes per element). Drawn from src/sys/objtypes.h /
# include/PR/sp.h / include/PR/gbi.h / etc — the IDO/PORT struct sizes
# match per static_assert. For typed structs that vary by version we use
# the PORT size; for raw integer types it's universal.
TYPE_SIZE = {
    "u8": 1,
    "u16": 2,
    "u32": 4,
    "Vtx": 16,           # Vtx_t
    "Gfx": 8,            # Gwords (w0, w1)
    "DObjDesc": 44,      # _Static_assert(sizeof(DObjDesc) == 44)
    "MObjSub": 120,      # 0x78
    "MatAnimJoint": 16,
    "AnimJoint": 16,
    "CamAnimJoint": 16,
    "MapHead": 24,
    "FTSpecialColl": 12,
    "GRAttackColl": 24,
    "HitDesc": 32,
    "HitParties": 8,
    "BloatScales": 16,
    "MPGeometryData": 16,
    "SpriteArray": 4,    # array of pointers (4-byte token slots)
    "Sprite": 56,
    "Bitmap": 32,
}


def parse_reloc_table(path: Path) -> dict[int, str]:
    out: dict[int, str] = {}
    for line in path.read_text().splitlines():
        m = TABLE_RE.match(line)
        if m:
            out[int(m.group(2))] = m.group(1)
    return out


def parse_descriptions(path: Path) -> tuple[dict[int, str], dict[int, dict[str, int]]]:
    """Parse decomp/tools/relocFileDescriptions.us.txt into:
       file_names[fid] = "FileName"  (from FILE NAMES section)
       file_contents[fid] = {symbol: offset}  (from FILE CONTENTS section,
           where symbol = "<Name>_<Type>" matching upstream's d<FileName>_<Name>_<Type>
           symbol naming convention).
    """
    names: dict[int, str] = {}
    contents: dict[int, dict[str, int]] = defaultdict(dict)
    section: str = "header"
    cur_fid: int | None = None

    name_re = re.compile(r"^-(\d+):\s*(\S+)\s*$")
    block_re = re.compile(r"^\[(\d+)\]\s*$")
    entry_re = re.compile(r"^(\S+)\s+(\S+)\s+0x([0-9a-fA-F]+)\s*$")

    for line in path.read_text().splitlines():
        if "FILE NAMES" in line:
            section = "names"; continue
        if "FILE CONTENTS" in line:
            section = "contents"; continue
        if section == "names":
            m = name_re.match(line)
            if m:
                names[int(m.group(1))] = m.group(2)
        elif section == "contents":
            m = block_re.match(line)
            if m:
                cur_fid = int(m.group(1))
                continue
            m = entry_re.match(line)
            if m and cur_fid is not None:
                type_, sym_name, off_hex = m.group(1), m.group(2), m.group(3)
                # Build symbol key matching upstream's d<FileName>_<Name>_<Type> convention
                # — but stored per-fid, so we just need <Name>_<Type> here.
                key = f"{sym_name}_{type_}"
                contents[cur_fid][key] = int(off_hex, 16)
    return names, dict(contents)


def torch_data(zf: zipfile.ZipFile, table: dict[int, str], file_id: int
               ) -> bytes | None:
    path = table.get(file_id)
    if path is None:
        return None
    try:
        blob = zf.read(path)
    except KeyError:
        return None
    off = HEADER_SIZE
    if len(blob) < off + 12:
        return None
    fid, intern_off, extern_off, n_ext = struct.unpack_from("<IHHI", blob, off)
    off += 4 + 2 + 2 + 4
    off += 2 * n_ext
    data_size, = struct.unpack_from("<I", blob, off)
    off += 4
    return blob[off:off + data_size]


def resolve_sym_offset(sym: str, file_name: str,
                       file_contents: dict[str, int]) -> int | None:
    """Resolve d<FileName>_<rest> symbol to a byte offset within the file.

    Strategy:
      1. Strip leading `d<FileName>_` prefix. Yields `<rest>`.
      2. If `<rest>` matches `<base>_0x<HEX>_sub_0x<HEX>` → offset = primary+sub.
      3. If `<rest>` matches `<base>_0x<HEX>` → offset = HEX.
      4. Else look up `<rest>` in file_contents (key format: `<Name>_<Type>`).
    """
    prefix = f"d{file_name}_"
    if not sym.startswith(prefix):
        return None
    rest = sym[len(prefix):]

    m = SYM_OFF_SUB.search(rest)
    if m:
        return int(m.group(1), 16) + int(m.group(2), 16)
    m = SYM_OFF_PRIMARY.search(rest)
    if m:
        return int(m.group(1), 16)

    # Named symbol — look up in descriptions
    if rest in file_contents:
        return file_contents[rest]
    return None


def find_all_declarations(c_text: str) -> list[tuple[int, str, str, int, str | None, str | None, str | None]]:
    """Walk the .c file and return EVERY top-level declaration in source
    order, with: (file_pos, type_name, sym, count, inc_dir, inc_sym, inc_type).
    inc_* are None for declarations without an .inc.c include — we still
    track them because their byte size contributes to the running offset
    walk.
    """
    out: list = []
    for m in DECL_RE.finditer(c_text):
        type_name = m.group(1)
        sym = m.group(2)
        count_str = m.group(3)
        count = int(count_str) if count_str else 0

        body_start = m.end()
        body_end = c_text.find("};", body_start)
        if body_end < 0:
            continue
        body = c_text[body_start:body_end]

        inc_match = INCLUDE_RE.search(body)
        if inc_match is None:
            inc_dir = inc_sym = inc_type = None
        else:
            inc_path = inc_match.group(1)
            inc_type = inc_match.group(2)
            if "/" in inc_path:
                inc_dir, inc_sym = inc_path.split("/", 1)
            else:
                # Rare: include without subdirectory
                inc_dir, inc_sym = "", inc_path

        out.append((m.start(), type_name, sym, count,
                    inc_dir, inc_sym, inc_type))
    return out


def derive_offsets(c_text: str, file_name: str,
                   file_contents: dict[str, int]) -> dict[str, int]:
    """Walk every top-level declaration in source order. Resolve each
    symbol's byte offset using (in priority):
      1. `_0xHEX_sub_0xHEX` suffix — primary + sub
      2. `_0xHEX` suffix — primary
      3. relocFileDescriptions.us.txt entry (key: `<Name>_<Type>`)
      4. Same as (3) but with common subtype suffix stripped
         (`_data`, `_block`, `_aliases`)
      5. Sequential fall-through: previous decl's offset + previous size
      6. Last resort: `_0xHEX` substring anywhere in symbol name
    Returns {sym: offset}. Symbols that can't be resolved are absent.
    """
    decls = find_all_declarations(c_text)
    out: dict[str, int] = {}
    running = 0  # byte offset cursor for sequential walk

    # Strippable subtype suffixes — used in (4) when the symbol has an
    # extra trailing token like `_data` past the type name.
    STRIPPABLE = ("_data", "_block", "_aliases", "_array", "_inc")

    for _pos, type_name, sym, count, _id, _is, _it in decls:
        # 1+2+6: any `_0xHEX` pattern in symbol
        m_sub = SYM_OFF_SUB.search(sym)
        if m_sub:
            off = int(m_sub.group(1), 16) + int(m_sub.group(2), 16)
            out[sym] = off
            running = off + count * TYPE_SIZE.get(type_name, 1)
            continue
        m_pri = SYM_OFF_PRIMARY.search(sym)
        if m_pri:
            off = int(m_pri.group(1), 16)
            out[sym] = off
            running = off + count * TYPE_SIZE.get(type_name, 1)
            continue

        # 3+4: descriptions match
        prefix = f"d{file_name}_"
        rest = sym[len(prefix):] if sym.startswith(prefix) else sym
        if rest in file_contents:
            off = file_contents[rest]
            out[sym] = off
            running = off + count * TYPE_SIZE.get(type_name, 1)
            continue
        for suf in STRIPPABLE:
            if rest.endswith(suf):
                trimmed = rest[: -len(suf)]
                if trimmed in file_contents:
                    # The data block follows the typed parent header. Best
                    # estimate without IDO nm: use the parent's offset +
                    # sizeof(parent header) — but we don't know which exact
                    # type the parent is. As a conservative fallback, use
                    # the parent's offset itself and let sequential-walk
                    # correct downstream entries. We only emit AFTER we
                    # confirm the running cursor matches.
                    parent_off = file_contents[trimmed]
                    # If we just walked past the parent, the data block
                    # starts right after — use the running cursor.
                    if running > parent_off:
                        out[sym] = running
                        running = running + count * TYPE_SIZE.get(type_name, 1)
                        break
                    out[sym] = parent_off
                    running = parent_off + count * TYPE_SIZE.get(type_name, 1)
                    break
        else:
            # 5: sequential fall-through
            out[sym] = running
            running = running + count * TYPE_SIZE.get(type_name, 1)
            continue

    return out


# ───────────────────────────── emitters ─────────────────────────────

def emit_vtx(data: bytes, off: int, count: int) -> str | None:
    if off + 16*count > len(data):
        return None
    lines = []
    for i in range(count):
        v = data[off + i*16 : off + (i+1)*16]
        x, y, z = struct.unpack(">3h", v[0:6])
        flag = struct.unpack(">H", v[6:8])[0]
        s, t = struct.unpack(">2h", v[8:12])
        r, g, b, a = v[12], v[13], v[14], v[15]
        lines.append(
            f"\t{{ {{ {{ {x}, {y}, {z} }}, 0x{flag:04X}, "
            f"{{ {s}, {t} }}, {{ 0x{r:02X}, 0x{g:02X}, 0x{b:02X}, 0x{a:02X} }} }} }},"
        )
    return "\n".join(lines) + "\n"


def emit_dl(data: bytes, off: int, count: int) -> str | None:
    """Each Gfx is 8 bytes = 2 BE u32. Emit `{{ 0xWWXXYYZZ, 0xAABBCCDD }}`
    for each. Returns None if out-of-range."""
    if off + 8*count > len(data):
        return None
    lines = []
    for i in range(count):
        g = data[off + i*8 : off + (i+1)*8]
        w0, w1 = struct.unpack(">II", g)
        lines.append(f"\t{{ {{ 0x{w0:08X}, 0x{w1:08X} }} }},")
    return "\n".join(lines) + "\n"


def emit_palette(data: bytes, off: int, count: int) -> str | None:
    """count is in u16 elements. Emit BE-decoded as `0xHHHH, ...`.
    Returns None if out-of-range (offset+size exceeds the file's data)."""
    if count == 0:
        return ""
    if off + 2*count > len(data):
        return None
    pairs = struct.unpack(f">{count}H", data[off:off + 2*count])
    lines = []
    for i in range(0, count, 8):
        chunk = pairs[i:i+8]
        lines.append("\t" + ", ".join(f"0x{c:04X}" for c in chunk) + ",")
    return "\n".join(lines) + "\n"


def emit_bytes(data: bytes, off: int, count: int) -> str | None:
    """Raw u8 hex. Returns None if out-of-range."""
    if off + count > len(data):
        return None
    chunk = data[off:off + count]
    lines = []
    for i in range(0, len(chunk), 16):
        row = chunk[i:i+16]
        lines.append("\t" + ", ".join(f"0x{b:02X}" for b in row) + ",")
    return "\n".join(lines) + "\n"


EMITTERS = {
    "vtx": emit_vtx,
    "dl": emit_dl,
    "palette": emit_palette,
    "data": emit_bytes,
    "tex": emit_bytes,
}


# ───────────────────────────── driver ─────────────────────────────

def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--battleship-o2r", type=Path, required=True)
    ap.add_argument("--reloc-table", type=Path, required=True)
    ap.add_argument("--reloc-dir", type=Path, required=True)
    ap.add_argument("--descriptions", type=Path, required=True)
    ap.add_argument("--output-dir", type=Path, required=True)
    ap.add_argument("--verbose", action="store_true")
    args = ap.parse_args()

    table = parse_reloc_table(args.reloc_table)
    file_names, file_contents_all = parse_descriptions(args.descriptions)
    zf = zipfile.ZipFile(args.battleship_o2r, "r")

    args.output_dir.mkdir(parents=True, exist_ok=True)

    files_processed = 0
    files_emitted = 0
    inc_c_emitted = 0
    skipped: list[tuple[int, str]] = []

    for c_path in sorted(args.reloc_dir.glob("*.c")):
        m = RELOCDATA_FILE_RE.match(c_path.name)
        if not m:
            continue
        fid = int(m.group(1))

        c_text = c_path.read_text()
        if not INCLUDE_RE.search(c_text):
            continue   # no .inc.c includes — no work to do

        files_processed += 1
        file_name = file_names.get(fid)
        if file_name is None:
            skipped.append((fid, "no name in descriptions"))
            continue

        data = torch_data(zf, table, fid)
        if data is None:
            skipped.append((fid, "no Torch resource"))
            continue

        all_decls = find_all_declarations(c_text)
        if not all_decls:
            skipped.append((fid, "no parseable declarations"))
            continue

        # Resolve all symbol offsets in source order (sequential walk handles
        # named sub-blocks like `<sym>_data` whose offset isn't in descriptions).
        offsets = derive_offsets(c_text, file_name,
                                 file_contents_all.get(fid, {}))

        per_file_emitted = 0
        for _pos, type_name, sym, count, inc_dir, inc_sym, inc_type in all_decls:
            if inc_type is None:
                continue  # declaration without .inc.c
            if count == 0:
                continue  # auto-sized — out of scope
            sym_off = offsets.get(sym)
            if sym_off is None:
                if args.verbose:
                    print(f"  [{fid}] cannot resolve offset for {sym}",
                          file=sys.stderr)
                continue

            elem_size = TYPE_SIZE.get(type_name, 1)
            byte_count = count * elem_size

            if inc_type == "palette":
                content = emit_palette(data, sym_off, count)
            elif inc_type == "vtx":
                content = emit_vtx(data, sym_off, count)
            elif inc_type == "dl":
                content = emit_dl(data, sym_off, count)
            else:  # data / tex
                content = emit_bytes(data, sym_off, byte_count)

            if content is None:
                # Offset+size exceeds the file's data — sequential walk
                # produced a wrong offset for this symbol. Skip rather
                # than emit a bogus block; gen_reloc_cmake will exclude
                # this file from the build.
                if args.verbose:
                    print(f"  [{fid}] {sym}: out-of-range "
                          f"(off=0x{sym_off:x}, need={byte_count}, "
                          f"have={len(data)})", file=sys.stderr)
                continue

            out_path = args.output_dir / inc_dir / f"{inc_sym}.{inc_type}.inc.c"
            out_path.parent.mkdir(parents=True, exist_ok=True)
            out_path.write_text(content)
            inc_c_emitted += 1
            per_file_emitted += 1

        if per_file_emitted > 0:
            files_emitted += 1
            if args.verbose:
                print(f"  [{fid:4d}] {file_name}: {per_file_emitted} .inc.c files emitted")

    print(f"=== extract_inc_c.py summary ===")
    print(f"  files containing .inc.c includes : {files_processed}")
    print(f"  files with at least one emit     : {files_emitted}")
    print(f"  total .inc.c files emitted       : {inc_c_emitted}")
    print(f"  files skipped                    : {len(skipped)}")
    for fid, reason in skipped[:10]:
        print(f"    {fid}: {reason}")
    if len(skipped) > 10:
        print(f"    (+ {len(skipped) - 10} more)")

    return 0


if __name__ == "__main__":
    sys.exit(main())
