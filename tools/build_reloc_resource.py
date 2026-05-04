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


def parse_reloc(reloc_path: Path, symbols: dict[str, dict]
                ) -> tuple[list[tuple[int, int, str]], list[tuple[int, int, str]]]:
    """
    Returns (intern_entries, extern_entries) where each entry is
    (ptr_byte_offset, target_byte_offset, target_label).

    target_label is preserved on extern entries so the caller can map the
    target symbol → file_id for the resource's extern_file_ids[] array.
    """
    intern: list[tuple[int, int, str]] = []
    extern: list[tuple[int, int, str]] = []

    for raw in reloc_path.read_text().splitlines():
        # Strip inline comments like `# -> file 208 (FoxMainMotion)`.
        if "#" in raw:
            raw = raw.split("#", 1)[0]
        parts = raw.split()
        if len(parts) != 3:
            continue
        kind, ptr_label, target_label = parts
        ptr_off = resolve_label(ptr_label, symbols)
        # For extern, target may be a symbol that doesn't exist in THIS .o
        # (it's defined in another file). We use 0 as the within-file
        # offset placeholder — it's stored in target_label for cross-file
        # lookup at the chain-emit step.
        try:
            tgt_off = resolve_label(target_label, symbols)
        except KeyError:
            tgt_off = 0
        if kind == "intern":
            intern.append((ptr_off, tgt_off, target_label))
        elif kind == "extern":
            # extern target offset is offset INTO the dep file, not this one.
            # The current placeholder won't be in this .o's symtab — caller
            # will compute via the dep file's symtab during pack. For the
            # POC where externs are out of scope, we still record the entry.
            extern.append((ptr_off, 0, target_label))
        else:
            print(f"warning: unknown reloc kind {kind!r} in {reloc_path}",
                  file=sys.stderr)
    return intern, extern


# ───────────────────────────── chain encoding ─────────────────────────────

def byteswap_u32(buf: bytearray) -> bytearray:
    """Swap every aligned u32 in-place (LE↔BE). Length must be /4."""
    n = len(buf) - (len(buf) % 4)
    for i in range(0, n, 4):
        buf[i:i+4] = bytes(reversed(buf[i:i+4]))
    return buf


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
        "-target", "i686-pc-linux-gnu",
        "-ffreestanding", "-nostdlib",
        "-Wno-pointer-to-int-cast",
        "-Wno-incompatible-pointer-types",
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

    # Compile .c → .o.
    # NamedTemporaryFile on Windows holds an exclusive lock until close, so
    # we use mkstemp + explicit close — gives clang an unlocked path to
    # write to on every platform.
    if args.obj_out is not None:
        args.obj_out.parent.mkdir(parents=True, exist_ok=True)
        obj_path = args.obj_out
        compile_to_elf(args.src, obj_path, args.clang, args.include_dir)
        cleanup_obj = False
    else:
        fd, tmp_name = tempfile.mkstemp(suffix=".o")
        os.close(fd)
        obj_path = Path(tmp_name)
        cleanup_obj = True
        try:
            compile_to_elf(args.src, obj_path, args.clang, args.include_dir)
        except Exception:
            obj_path.unlink(missing_ok=True)
            raise

    elf = Elf32(obj_path.read_bytes())
    sym_table = elf.symtab()

    data_sec = elf.section(".data")
    if data_sec is None:
        raise RuntimeError(f"{args.src.name}: no .data section in compiled .o")
    data = bytearray(elf.section_bytes(data_sec))

    # Byte-swap LE→BE before chain encoding. Runtime byteswap will reverse
    # this on load.
    byteswap_u32(data)

    intern_entries, extern_entries = parse_reloc(args.reloc, sym_table)

    intern_off = write_chain(data, intern_entries)

    # Externs need cross-file lookup. For the POC, accept files with no
    # externs and skip extern handling. M2.P2 / M3 will populate via
    # symbol_index.
    extern_file_ids: list[int] = []
    if extern_entries:
        if args.symbol_index is None:
            print(f"warning: {args.src.name} has {len(extern_entries)} extern "
                  "entries but no --symbol-index; emitting chain with target=0",
                  file=sys.stderr)
            extern_off = write_chain(data, extern_entries)
        else:
            sym_index = json.loads(args.symbol_index.read_text())
            # For each extern entry, look up target symbol's file_id AND
            # offset within the dep file. The .reloc target_label points
            # at a symbol defined in the dep file's compiled .o; the
            # symbol_index records {symbol → {file_id, offset}}.
            resolved: list[tuple[int, int, str]] = []
            ids: list[int] = []
            for ptr_off, _placeholder, label in sorted(extern_entries,
                                                       key=lambda e: e[0]):
                # Strip +0xN suffix on the target label
                base, _, off_s = label.partition("+")
                base = base.strip()
                rel = int(off_s.strip(), 16) if off_s.strip().lower().startswith("0x") \
                    else (int(off_s.strip(), 0) if off_s.strip() else 0)
                if base not in sym_index:
                    raise KeyError(f"extern target {base!r} not in symbol index")
                entry = sym_index[base]
                ids.append(entry["file_id"])
                resolved.append((ptr_off, entry["offset"] + rel, label))
            extern_off = write_chain(data, resolved)
            extern_file_ids = ids
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
