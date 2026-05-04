#!/usr/bin/env python3
"""
build_reloc_resource.py — compile a single decomp/src/relocData/*.c file and
emit a LUS RelocFile resource (the same byte format Torch's RelocFactory writes
into BattleShip.o2r), so that the resulting `.relo` can be packed into
BattleShip.fromsource.o2r and shadow Torch's ROM-extracted equivalent at
runtime.

Pipeline (mirrors decomp/tools/fixRelocChain.py with format adjustments):

    .c source ──clang──► ELF .o ──parse──► (.data bytes, symtab)
                                              │
                                              ├─byte-swap LE→BE (entire .data)
                                              │
                                              ├─parse .reloc file
                                              │   intern <ptr_label> <target_label>
                                              │   extern <ptr_label> <target_label>
                                              │
                                              ├─build chains (overwrite
                                              │   placeholder bytes at each
                                              │   ptr offset with
                                              │   (next_word << 16) | target_word
                                              │   in BE)
                                              │
                                              └─emit RelocFile resource
                                                  LUS header + fields + data

The runtime's lbreloc_byteswap then swaps every u32 BE→LE on load, leaving
the chain slots in the format the chain-walk in lbreloc_bridge expects.

Usage:
    build_reloc_resource.py
        --src         decomp/src/relocData/<id>_<Name>.c
        --reloc       decomp/src/relocData/<id>_<Name>.reloc
        --file-id     <N>
        --symbol-index <build>/reloc_objects/symbol_index.json (optional;
                      required if .reloc has any extern entries)
        --clang       /path/to/clang        (optional; defaults to `clang`)
        --include-dir <decomp/include>      (repeatable)
        --include-dir <decomp/src>          (repeatable)
        --output      <build>/reloc_resources/<id>.relo
        [--obj-out    <build>/reloc_objects/<id>.o]   (keep .o on disk)
        [--header     <ref-resource>]                  (LUS header bytes; if
                      omitted, write the canonical 0x40-byte header observed
                      in BattleShip.o2r)
"""

from __future__ import annotations

import argparse
import json
import os
import re
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

ELF_MAGIC = b"\x7fELF"

# ELFCLASS32 + EI_DATA = 1 (LSB) + EV_CURRENT + EM_386 expected.
# Section types we care about.
SHT_NULL = 0
SHT_PROGBITS = 1
SHT_SYMTAB = 2
SHT_STRTAB = 3
SHT_RELA = 4
SHT_REL = 9


# ───────────────────────────── ELF parser ─────────────────────────────

class Elf32:
    """Minimal ELF32-LSB reader. Just enough to get .data + symtab."""

    def __init__(self, blob: bytes):
        if blob[:4] != ELF_MAGIC:
            raise ValueError(f"not an ELF: magic {blob[:4]!r}")
        if blob[4] != 1:
            raise ValueError("not ELFCLASS32")
        if blob[5] != 1:
            raise ValueError("not ELFDATA2LSB (little-endian)")

        self.blob = blob

        # ELF32 header (52 bytes): e_ident[16], e_type(2), e_machine(2),
        # e_version(4), e_entry(4), e_phoff(4), e_shoff(4), e_flags(4),
        # e_ehsize(2), e_phentsize(2), e_phnum(2), e_shentsize(2),
        # e_shnum(2), e_shstrndx(2)
        (
            self.e_type, self.e_machine, self.e_version, self.e_entry,
            self.e_phoff, self.e_shoff, self.e_flags, self.e_ehsize,
            self.e_phentsize, self.e_phnum, self.e_shentsize,
            self.e_shnum, self.e_shstrndx,
        ) = struct.unpack_from("<HHIIIIIHHHHHH", blob, 16)

        # Read all section headers
        self.sections: list[dict] = []
        for i in range(self.e_shnum):
            off = self.e_shoff + i * self.e_shentsize
            (sh_name, sh_type, sh_flags, sh_addr, sh_offset, sh_size,
             sh_link, sh_info, sh_addralign, sh_entsize) = struct.unpack_from(
                "<IIIIIIIIII", blob, off)
            self.sections.append(dict(
                idx=i, name_off=sh_name, type=sh_type, flags=sh_flags,
                offset=sh_offset, size=sh_size, link=sh_link, info=sh_info,
                entsize=sh_entsize,
            ))

        # Resolve section names via shstrtab
        shstrtab = self.sections[self.e_shstrndx]
        sh_strs = blob[shstrtab["offset"]:shstrtab["offset"] + shstrtab["size"]]
        for s in self.sections:
            s["name"] = self._cstr(sh_strs, s["name_off"])

    @staticmethod
    def _cstr(buf: bytes, off: int) -> str:
        end = buf.find(b"\x00", off)
        return buf[off:end].decode("utf-8", errors="replace")

    def section(self, name: str) -> dict | None:
        for s in self.sections:
            if s["name"] == name:
                return s
        return None

    def section_bytes(self, sec: dict) -> bytes:
        return self.blob[sec["offset"]:sec["offset"] + sec["size"]]

    def symtab(self) -> dict[str, dict]:
        """Returns {symbol_name: {offset, size, section_idx, bind, type}}."""
        sym_sec = next((s for s in self.sections if s["type"] == SHT_SYMTAB), None)
        if sym_sec is None:
            return {}
        strtab_idx = sym_sec["link"]
        strtab = self.sections[strtab_idx]
        str_buf = self.section_bytes(strtab)

        sym_buf = self.section_bytes(sym_sec)
        out: dict[str, dict] = {}
        # ELF32 Sym: st_name(4), st_value(4), st_size(4), st_info(1),
        # st_other(1), st_shndx(2) = 16 bytes
        for i in range(0, len(sym_buf), 16):
            (st_name, st_value, st_size, st_info, st_other, st_shndx) = \
                struct.unpack_from("<IIIBBH", sym_buf, i)
            if st_name == 0:
                continue
            name = self._cstr(str_buf, st_name)
            if not name:
                continue
            out[name] = dict(
                offset=st_value, size=st_size,
                section_idx=st_shndx, info=st_info, other=st_other,
            )
        return out


# ───────────────────────────── label resolution ─────────────────────────────

def resolve_label(label: str, symbols: dict[str, dict]) -> int:
    """Resolve `varname` or `varname+0xOFF` or raw `0xOFF` to a byte offset."""
    s = label.strip()
    if s.startswith("0x") or s.startswith("0X"):
        return int(s, 16)
    if "+" in s:
        name, _, off_s = s.partition("+")
        name = name.strip()
        rel = int(off_s.strip(), 16) if off_s.strip().lower().startswith("0x") \
            else int(off_s.strip(), 0)
        if name not in symbols:
            raise KeyError(f"symbol not in object: {name!r}")
        return symbols[name]["offset"] + rel
    if s not in symbols:
        raise KeyError(f"symbol not in object: {s!r}")
    return symbols[s]["offset"]


_ANNOT_RE = re.compile(r"#\s*->\s*file\s*(\d+)")


def parse_reloc(reloc_path: Path, symbols: dict[str, dict]
                ) -> tuple[list[tuple[int, int, str]],
                           list[tuple[int, int, int, str]]]:
    """
    Returns (intern_entries, extern_entries) where:
      intern entry  = (ptr_byte_offset, target_byte_offset, target_label)
      extern entry  = (ptr_byte_offset, target_byte_offset_in_dep,
                       dep_file_id_or_-1, target_label)

    For extern entries, the dep_file_id is parsed from the trailing
    `# -> file N (Name)` annotation written by
    tools/annotate_externs_from_torch.py. -1 means missing — the caller
    can decide how to handle it (skip, error, manual override).
    """
    intern: list[tuple[int, int, str]] = []
    extern: list[tuple[int, int, int, str]] = []

    for raw in reloc_path.read_text().splitlines():
        # Capture the annotation BEFORE stripping the comment.
        annot_match = _ANNOT_RE.search(raw)
        dep_fid = int(annot_match.group(1)) if annot_match else -1

        body = raw.split("#", 1)[0] if "#" in raw else raw
        parts = body.split()
        if len(parts) != 3:
            continue
        kind, ptr_label, target_label = parts
        ptr_off = resolve_label(ptr_label, symbols)
        if kind == "intern":
            try:
                tgt_off = resolve_label(target_label, symbols)
            except KeyError:
                tgt_off = 0
            intern.append((ptr_off, tgt_off, target_label))
        elif kind == "extern":
            # For externs, target_label is a raw hex offset INTO the dep
            # file (not a symbol in this .o). dep_fid comes from the
            # annotation comment (or -1 if missing).
            try:
                tgt_off = int(target_label, 0)
            except ValueError:
                # Unusual case: extern target is a symbolic name. Resolve
                # to 0 (chain still encodes; runtime will use the wrong
                # offset — flag this case loudly).
                print(f"warning: extern target {target_label!r} in "
                      f"{reloc_path.name} is not a hex offset", file=sys.stderr)
                tgt_off = 0
            extern.append((ptr_off, tgt_off, dep_fid, target_label))
        else:
            print(f"warning: unknown reloc kind {kind!r} in {reloc_path}",
                  file=sys.stderr)
    return intern, extern


# ───────────────────────────── chain encoding ─────────────────────────────

def byteswap_u32(buf: bytearray, skip: set[int] | None = None) -> bytearray:
    """Swap every aligned u32 in-place (LE↔BE). Length must be /4.

    `skip`, if given, is a set of byte offsets — words starting at those
    offsets are left untouched (used by the per-struct field-byteswap pass
    to exclude already-handled u16/u8 fields from the global LE→BE swap).
    """
    if skip is None:
        skip = set()
    n = len(buf) - (len(buf) % 4)
    for i in range(0, n, 4):
        if i in skip:
            continue
        buf[i:i+4] = bytes(reversed(buf[i:i+4]))
    return buf


_STRUCT_DECL_RE_CACHE: dict[str, re.Pattern[str]] = {}


def _struct_decl_re(type_name: str) -> re.Pattern[str]:
    pat = _STRUCT_DECL_RE_CACHE.get(type_name)
    if pat is None:
        # Match `<TYPE> <name>...= {`. Tolerates whitespace, attribute-like
        # qualifiers, and array decls (e.g. `T name[]`). The type must be a
        # whole-word match.
        pat = re.compile(
            rf"\b{re.escape(type_name)}\s+([A-Za-z_]\w*)\s*(?:\[\s*\w*\s*\])?\s*="
        )
        _STRUCT_DECL_RE_CACHE[type_name] = pat
    return pat


def find_struct_regions(c_text: str, sym_table: dict[str, dict],
                        fixup_tables: dict[str, list[tuple[str, int]]],
                        size_table: dict[str, int]
                        ) -> list[tuple[str, str, int, int]]:
    """Returns a list of (struct_type, sym_name, element_base_off, struct_size)
    — ONE entry per array element (so per-field rules apply to every element
    of an `FTSkeleton arr[33]` declaration, not just the first).

    Walks the .c source for declarations of any struct type whose fixups
    are in `fixup_tables`. Each declaration's symbol is looked up in the
    ELF symbol table; the symbol's total byte size must be a multiple of
    the struct's declared single-element size (catches struct-layout drift
    between port headers and these tables).
    """
    from struct_byteswap_tables import SYMBOL_NAME_TYPE_OVERRIDES

    regions: list[tuple[str, str, int, int]] = []
    matched_names: set[str] = set()
    for type_name in fixup_tables:
        pat = _struct_decl_re(type_name)
        for m in pat.finditer(c_text):
            name = m.group(1)
            sym = sym_table.get(name)
            if sym is None:
                continue
            matched_names.add(name)
            _emit_region(regions, type_name, name, sym, size_table)

    # Generic u8[] arrays — every such declaration in the .c source holds
    # raw bytes (commonly from a `#include <...inc.c>` of ROM-extracted
    # data, but also explicit u8 literals like color/SFX byte arrays).
    # Torch ships ROM bytes verbatim; clang emits the same raw bytes. So
    # the global LE→BE bswap32 must NOT touch these — same treatment as
    # MPItemWeights, just by source-text type rather than type-name.
    for m in _U8_ARRAY_DECL_RE.finditer(c_text):
        name = m.group(1)
        if name in matched_names:
            continue
        sym = sym_table.get(name)
        if sym is None:
            continue
        matched_names.add(name)
        regions.append(("__u8_array__", name, sym["offset"], sym["size"]))

    # Symbol-name-based overrides: apply when the .c declares the symbol with
    # a primitive type (e.g. `u8 dXXX_item_weights[20]`) rather than the
    # struct type. Skip symbols already matched by a type-based regex above.
    for name, sym in sym_table.items():
        if name in matched_names:
            continue
        for name_pat, type_name in SYMBOL_NAME_TYPE_OVERRIDES:
            if name_pat.search(name) and type_name in fixup_tables:
                _emit_region(regions, type_name, name, sym, size_table)
                break
    return regions


# Match `u8 <name>[<size>] = ` at the start of a line (any whitespace before).
# Captures the symbol name. Tolerates multiple lines after the `=`.
_U8_ARRAY_DECL_RE = re.compile(
    r"^\s*u8\s+([A-Za-z_]\w*)\s*\[[^\]]*\]\s*=", re.MULTILINE)


def _emit_region(regions: list[tuple[str, str, int, int]],
                 type_name: str, name: str, sym: dict,
                 size_table: dict[str, int]) -> None:
    base_off = sym["offset"]
    total_size = sym["size"]
    expected = size_table.get(type_name)
    if expected is None:
        regions.append((type_name, name, base_off, total_size))
        return
    if total_size % expected != 0:
        raise RuntimeError(
            f"{name}: ELF symbol size {total_size} is not a multiple "
            f"of sizeof({type_name})={expected}. Struct layout drift "
            f"between port headers and tools/struct_byteswap_tables.py "
            f"— cross-check decomp/src/.../*types.h _Static_assert.")
    n_elements = total_size // expected
    for i in range(n_elements):
        regions.append((type_name, f"{name}[{i}]",
                        base_off + i * expected, expected))


def apply_struct_aware_bswap(data: bytearray, regions: list[tuple[str, str, int, int]],
                             fixup_tables: dict[str, list[tuple[str, int]]]
                             ) -> set[int]:
    """Apply per-struct field byteswaps to the data section. Returns the
    set of byte offsets that were touched here and should be excluded from
    the subsequent global LE→BE bswap32 pass.

    Rules (from struct_byteswap_tables.py docstring):
      "rotate16"    — u16-pair word: bswap16 each u16 half [a b c d] → [b a d c],
                      then exclude this slot from the global LE→BE pass. Result
                      on disk matches Torch's BE-stored u16 pair; runtime pass1
                      + fixup_rotate16 produce correct LE pair in memory.
      "raw_u8"      — u8[4] (RGBA, raw filler): leave bytes as clang emitted
                      AND exclude from the global LE→BE pass. u8 has no
                      endianness so Torch and clang both emit the same bytes;
                      our job is just to keep them out of the global swap.
      "raw_u8_all"  — apply raw_u8 to every aligned word in the symbol's
                      footprint (rounded up to 4-byte boundary). Used for
                      flex-array u8 structs like MPItemWeights where the
                      size varies per-symbol.
    """
    skip: set[int] = set()
    for type_name, _name, base_off, size in regions:
        if type_name == "__u8_array__":
            # Synthesised "type" for any `u8 NAME[]` declaration discovered
            # by find_struct_regions's source scan. Every word in the
            # symbol's footprint is raw u8.
            rules: list[tuple[str, int]] = [
                ("raw_u8", i) for i in range((size + 3) // 4)
            ]
        else:
            # Expand "raw_u8_all" rules into per-word raw_u8 entries based
            # on this symbol's footprint. Footprint is rounded up to the
            # next 4-byte boundary because clang aligns the next symbol
            # there and trailing padding bytes belong to this symbol's slot.
            rules = []
            for kind, word_off in fixup_tables[type_name]:
                if kind == "raw_u8_all":
                    num_words = (size + 3) // 4
                    rules.extend(("raw_u8", word_off + i)
                                 for i in range(num_words))
                else:
                    rules.append((kind, word_off))

        for kind, word_off in rules:
            byte_off = base_off + word_off * 4
            if byte_off + 4 > len(data):
                raise RuntimeError(
                    f"struct {type_name} word offset 0x{word_off:x} "
                    f"at byte 0x{byte_off:x} extends past data section size "
                    f"0x{len(data):x}; check tools/struct_byteswap_tables.py.")
            slot = data[byte_off:byte_off + 4]
            if kind == "rotate16":
                data[byte_off:byte_off + 4] = bytes(
                    [slot[1], slot[0], slot[3], slot[2]])
            elif kind == "raw_u8":
                pass  # leave bytes as clang emitted; just skip global pass
            else:
                raise RuntimeError(
                    f"unknown struct fixup kind {kind!r} for {type_name}")
            skip.add(byte_off)
    return skip


def write_chain(data: bytearray, entries: list[tuple[int, int, str]]
                ) -> int:
    """
    Overwrite slot at each ptr_offset with (next_word << 16) | target_word,
    big-endian. Returns the chain start word offset, or 0xFFFF if no entries.
    """
    if not entries:
        return 0xFFFF

    sorted_entries = sorted(entries, key=lambda e: e[0])
    for i, (ptr_off, tgt_off, _label) in enumerate(sorted_entries):
        target_word = tgt_off // 4
        next_word = (sorted_entries[i + 1][0] // 4
                     if i + 1 < len(sorted_entries) else 0xFFFF)
        packed = ((next_word & 0xFFFF) << 16) | (target_word & 0xFFFF)
        struct.pack_into(">I", data, ptr_off, packed)

    return sorted_entries[0][0] // 4


# ───────────────────────────── compile + emit ─────────────────────────────

# Reverses the 22 `is_have_*` positional bitfield init lines so that LE
# clang fills the LE-declared field order (which is BE order reversed) with
# the values intended for BE order. Without this, e.g. the value for
# `is_have_attack11` lands on `is_have_voice` instead. The runtime reads bits
# at fixed positions, so getting the values into the right *fields* is what
# matters — bit positions handled by the LE branch of the struct definition.
_BITFIELD_REVERSE_RE = re.compile(
    r'((?:[ \t]*[01],[ \t]*/\* is_have_\w+ \*/[ \t]*\n){22})',
    re.MULTILINE)


def preprocess_le_bitfield_inits(c_text: str) -> str:
    def reverse_block(m: re.Match[str]) -> str:
        block = m.group(1)
        # Last line may not end in newline; capture trailing newlines to put
        # back after reversing the lines.
        lines = block.split('\n')
        if lines[-1] == '':
            lines = lines[:-1]
            trailer = '\n'
        else:
            trailer = ''
        return '\n'.join(reversed(lines)) + trailer
    return _BITFIELD_REVERSE_RE.sub(reverse_block, c_text)


def compile_to_elf(src: Path, output: Path, clang: str,
                   include_dirs: list[Path]) -> None:
    """
    Compile a single relocData .c to a 32-bit i686 ELF object.

    The 32-bit target is critical: relocData files cast (u32)&symbol_addr
    in static initialisers, which is only a constant expression when
    sizeof(void*) == 4. -ffreestanding -nostdlib avoids pulling host
    system headers — the .c only depends on decomp/include and
    decomp/src/relocData/relocdata_types.h.
    """
    cmd = [
        clang,
        "-DPORT=1",
        "-D_LANGUAGE_C",
        "-DF3DEX_GBI_2=1",
        "-DREGION_US=1",
        "-target", "i686-pc-linux-gnu",
        "-ffreestanding", "-nostdlib",
        # Force all-zero arrays into .data instead of .bss. The pipeline
        # only reads .data + applies the runtime byteswap layout to it; an
        # all-NULL array in .bss has no bytes, so its file offset gets
        # silently dropped from the emitted resource (e.g. fighter
        # modelparts_container[25] = {NULL,...}). Torch ships the zero
        # bytes from ROM, so byte-equivalence requires us to too.
        "-fno-zero-initialized-in-bss",
        "-Wno-pointer-to-int-cast",
        "-Wno-incompatible-pointer-types",
        "-Wno-int-conversion",
    ]
    for inc in include_dirs:
        cmd += ["-I", str(inc)]
    cmd += ["-c", str(src), "-o", str(output)]
    subprocess.run(cmd, check=True)


# Canonical 0x40-byte LUS BinaryResource header observed in Torch-emitted
# RelocFile resources (matches the exact bytes inside BattleShip.o2r so the
# RelocFileFactory parser accepts it without modification).
LUS_HEADER_PREFIX = (
    bytes.fromhex("00000000")            # 0x00 endianness/version (zero)
    + bytes.fromhex("4f4c4552")          # 0x04 magic "OLER" (LE "RELO")
    + bytes.fromhex("00000000")          # 0x08 version
    + bytes.fromhex("efbeadde")          # 0x0C placeholder
    + bytes.fromhex("efbeadde")          # 0x10 placeholder
    + b"\x00" * 0x2C                     # 0x14–0x3F zero padding
)
assert len(LUS_HEADER_PREFIX) == 0x40, len(LUS_HEADER_PREFIX)


def emit_resource(*, file_id: int, intern_off: int, extern_off: int,
                  extern_file_ids: list[int], data: bytes,
                  header_prefix: bytes = LUS_HEADER_PREFIX) -> bytes:
    """Build the on-disk RelocFile resource bytes."""
    out = bytearray()
    out += header_prefix
    out += struct.pack("<I", file_id)
    out += struct.pack("<H", intern_off & 0xFFFF)
    out += struct.pack("<H", extern_off & 0xFFFF)
    out += struct.pack("<I", len(extern_file_ids))
    for fid in extern_file_ids:
        out += struct.pack("<H", fid)
    out += struct.pack("<I", len(data))
    out += data
    return bytes(out)


# ───────────────────────────── main ─────────────────────────────

def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--src", type=Path, required=True)
    ap.add_argument("--reloc", type=Path, required=True)
    ap.add_argument("--file-id", type=int, required=True)
    ap.add_argument("--symbol-index", type=Path, default=None,
                    help="JSON {symbol→file_id} for extern resolution")
    ap.add_argument("--clang", default="clang")
    ap.add_argument("--include-dir", type=Path, action="append", default=[])
    ap.add_argument("--obj-out", type=Path, default=None,
                    help="keep the compiled .o at this path")
    ap.add_argument("--output", type=Path, required=True)
    args = ap.parse_args()

    # Pre-process the .c source for LE-clang quirks (FTAttributes bitfield
    # init order). Skip the preprocess step when the file doesn't contain
    # the bitfield region — preprocess_le_bitfield_inits is a no-op for
    # those files anyway, but we still write a temp .c file. Cheap enough
    # to do unconditionally.
    src_text = args.src.read_text()
    src_text_pp = preprocess_le_bitfield_inits(src_text)
    if src_text_pp != src_text:
        # Need a temp .c file with the same #include search path as the
        # original. Place next to the obj-out (or in /tmp) and pass to clang
        # with -I including the original .c's parent (so any local includes
        # like relocdata_types.h resolve).
        if args.obj_out is not None:
            args.obj_out.parent.mkdir(parents=True, exist_ok=True)
            pp_src = args.obj_out.with_suffix(".pp.c")
        else:
            fd, pp_name = tempfile.mkstemp(suffix=".pp.c")
            os.close(fd)
            pp_src = Path(pp_name)
        pp_src.write_text(src_text_pp)
        compile_src = pp_src
        cleanup_pp = (args.obj_out is None)
    else:
        compile_src = args.src
        cleanup_pp = False
        pp_src = None

    # Compile .c → .o.
    # NamedTemporaryFile on Windows holds an exclusive lock until close, so
    # we use mkstemp + explicit close — gives clang an unlocked path to
    # write to on every platform.
    if args.obj_out is not None:
        args.obj_out.parent.mkdir(parents=True, exist_ok=True)
        obj_path = args.obj_out
        # If we're compiling a pre-processed temp file, it doesn't sit in
        # the original include path. Add the original .c's parent directory
        # to the include path so relative includes (e.g. relocdata_types.h)
        # resolve.
        include_dirs = list(args.include_dir)
        if compile_src != args.src:
            include_dirs.insert(0, args.src.parent)
        compile_to_elf(compile_src, obj_path, args.clang, include_dirs)
        cleanup_obj = False
    else:
        fd, tmp_name = tempfile.mkstemp(suffix=".o")
        os.close(fd)
        obj_path = Path(tmp_name)
        cleanup_obj = True
        include_dirs = list(args.include_dir)
        if compile_src != args.src:
            include_dirs.insert(0, args.src.parent)
        try:
            compile_to_elf(compile_src, obj_path, args.clang, include_dirs)
        except Exception:
            obj_path.unlink(missing_ok=True)
            if cleanup_pp and pp_src:
                pp_src.unlink(missing_ok=True)
            raise

    if cleanup_pp and pp_src:
        pp_src.unlink(missing_ok=True)

    elf = Elf32(obj_path.read_bytes())
    sym_table = elf.symtab()

    data_sec = elf.section(".data")
    if data_sec is None:
        raise RuntimeError(f"{args.src.name}: no .data section in compiled .o")
    data = bytearray(elf.section_bytes(data_sec))

    # Per-struct field-aware byteswap for typed structs (FTAttributes,
    # WPAttributes, MPGroundData) whose runtime fixup helpers in
    # port/bridge/lbreloc_byteswap.cpp rotate u16-pair words and bswap
    # u8[4] color quads. The global LE→BE pass below skips slots already
    # handled here. See tools/struct_byteswap_tables.py for the rationale.
    from struct_byteswap_tables import STRUCT_FIELD_FIXUPS, STRUCT_SIZE
    c_text = args.src.read_text()
    regions = find_struct_regions(c_text, sym_table,
                                  STRUCT_FIELD_FIXUPS, STRUCT_SIZE)
    pre_skip = apply_struct_aware_bswap(data, regions, STRUCT_FIELD_FIXUPS)

    # Byte-swap LE→BE before chain encoding. Runtime byteswap will reverse
    # this on load. Slots already handled by the per-struct pass are skipped.
    byteswap_u32(data, skip=pre_skip)

    intern_entries, extern_entries = parse_reloc(args.reloc, sym_table)

    intern_off = write_chain(data, intern_entries)

    # Externs: dep_file_id comes from the trailing `# -> file N (Name)`
    # annotation written by tools/annotate_externs_from_torch.py. Source-
    # order matches Torch's chain-walk order (sorted by ptr_offset within
    # this file's data layout) for >99% of files (verified empirically).
    extern_file_ids: list[int] = []
    if extern_entries:
        # Sort by ptr_offset to match the chain encoding order.
        sorted_externs = sorted(extern_entries, key=lambda e: e[0])
        missing_annotations = [(p, t, l) for p, t, fid, l in sorted_externs
                               if fid < 0]
        if missing_annotations:
            print(f"error: {args.src.name} has {len(missing_annotations)} "
                  f"extern entries without `# -> file N` annotation. "
                  f"Run tools/annotate_externs_from_torch.py to populate.",
                  file=sys.stderr)
            return 2

        # write_chain expects 3-tuples (ptr, tgt, label) — adapt.
        chain_input = [(p, t, l) for p, t, _fid, l in sorted_externs]
        extern_off = write_chain(data, chain_input)
        extern_file_ids = [fid for _p, _t, fid, _l in sorted_externs]
    else:
        extern_off = 0xFFFF

    # Emit the RelocFile resource bytes.
    blob = emit_resource(
        file_id=args.file_id,
        intern_off=intern_off,
        extern_off=extern_off,
        extern_file_ids=extern_file_ids,
        data=bytes(data),
    )

    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_bytes(blob)

    if cleanup_obj:
        obj_path.unlink(missing_ok=True)

    print(f"emitted {args.output} "
          f"(file_id={args.file_id}, "
          f"intern_off=0x{intern_off:04x}, "
          f"extern_off=0x{extern_off:04x}, "
          f"extern_count={len(extern_file_ids)}, "
          f"data_size={len(data)})")
    return 0


if __name__ == "__main__":
    sys.exit(main())
