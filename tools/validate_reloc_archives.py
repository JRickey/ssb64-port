#!/usr/bin/env python3
"""
validate_reloc_archives.py — diff per-file RelocFile resource bytes between
two sources: the Torch-extracted BattleShip.o2r and the source-compiled
output (either an .o2r archive or a directory of .relo files).

Usage:
    validate_reloc_archives.py
        --torch       <path-to-BattleShip.o2r>
        --source      <BattleShip.fromsource.o2r OR dir-of-.relo-files>
        --reloc-table <port/resource/RelocFileTable.cpp>
        [--file-id N [--file-id M ...]]   (default: all files in source)
        [--out         report.md]

For each file present in --source, finds the corresponding entry in --torch
via the path mapping in --reloc-table, parses both as RelocFile resources,
and reports a field-level diff: file_id, intern_off, extern_off,
extern_file_ids, data_size, data bytes (first/last differing offset,
total bytes differing).
"""

from __future__ import annotations

import argparse
import re
import struct
import sys
import zipfile
from dataclasses import dataclass, field
from pathlib import Path


HEADER_SIZE = 0x40  # LUS BinaryResource header — opaque, compared as bytes.


@dataclass
class RelocResource:
    header: bytes
    file_id: int
    intern_off: int
    extern_off: int
    extern_file_ids: list[int]
    data: bytes

    @classmethod
    def parse(cls, blob: bytes) -> "RelocResource":
        if len(blob) < HEADER_SIZE + 12:
            raise ValueError(f"resource too short ({len(blob)} bytes)")
        header = blob[:HEADER_SIZE]
        off = HEADER_SIZE
        (file_id,) = struct.unpack_from("<I", blob, off); off += 4
        (intern_off,) = struct.unpack_from("<H", blob, off); off += 2
        (extern_off,) = struct.unpack_from("<H", blob, off); off += 2
        (n_ext,) = struct.unpack_from("<I", blob, off); off += 4
        externs = list(struct.unpack_from(f"<{n_ext}H", blob, off))
        off += 2 * n_ext
        (data_size,) = struct.unpack_from("<I", blob, off); off += 4
        data = blob[off:off + data_size]
        if len(data) != data_size:
            raise ValueError(
                f"data size {data_size} but only {len(data)} trailing bytes")
        return cls(
            header=header, file_id=file_id, intern_off=intern_off,
            extern_off=extern_off, extern_file_ids=externs, data=data,
        )


@dataclass
class FieldDiff:
    file_id_match: bool
    header_match: bool
    intern_off_match: bool
    extern_off_match: bool
    extern_file_ids_match: bool
    data_size_match: bool
    data_first_diff_byte: int  # -1 if match
    data_byte_diff_count: int

    @classmethod
    def of(cls, a: RelocResource, b: RelocResource) -> "FieldDiff":
        first_diff = -1
        diff_count = 0
        if a.data == b.data:
            return cls(
                file_id_match=a.file_id == b.file_id,
                header_match=a.header == b.header,
                intern_off_match=a.intern_off == b.intern_off,
                extern_off_match=a.extern_off == b.extern_off,
                extern_file_ids_match=a.extern_file_ids == b.extern_file_ids,
                data_size_match=len(a.data) == len(b.data),
                data_first_diff_byte=-1,
                data_byte_diff_count=0,
            )
        n = min(len(a.data), len(b.data))
        for i in range(n):
            if a.data[i] != b.data[i]:
                if first_diff < 0:
                    first_diff = i
                diff_count += 1
        diff_count += abs(len(a.data) - len(b.data))
        return cls(
            file_id_match=a.file_id == b.file_id,
            header_match=a.header == b.header,
            intern_off_match=a.intern_off == b.intern_off,
            extern_off_match=a.extern_off == b.extern_off,
            extern_file_ids_match=a.extern_file_ids == b.extern_file_ids,
            data_size_match=len(a.data) == len(b.data),
            data_first_diff_byte=first_diff,
            data_byte_diff_count=diff_count,
        )

    @property
    def all_match(self) -> bool:
        return (self.file_id_match and self.header_match
                and self.intern_off_match and self.extern_off_match
                and self.extern_file_ids_match and self.data_size_match
                and self.data_byte_diff_count == 0)


# RelocFileTable.cpp lines look like: `	"reloc_animations/FTMarioAnimWait", /* 0 */`
_TABLE_RE = re.compile(r'^\s*"([^"]+)",\s*/\*\s*(\d+)\s*\*/')


def parse_reloc_table(path: Path) -> dict[int, str]:
    out: dict[int, str] = {}
    for line in path.read_text().splitlines():
        m = _TABLE_RE.match(line)
        if m:
            res_path, fid = m.group(1), int(m.group(2))
            out[fid] = res_path
    return out


def chain_walk(data: bytes, start_word: int, max_steps: int = 4096
               ) -> tuple[int, list[int]]:
    """
    Walk a relocation chain from `start_word` (in BE u32 slots), returning
    (steps_taken, list_of_target_words). Stops at 0xFFFF or if max_steps
    exceeded. Used as an integrity sanity check — every chain must
    terminate at 0xFFFF.
    """
    if start_word == 0xFFFF:
        return 0, []
    targets: list[int] = []
    word = start_word
    for _ in range(max_steps):
        byte_off = word * 4
        if byte_off + 4 > len(data):
            raise RuntimeError(
                f"chain slot at word 0x{word:04x} (byte 0x{byte_off:x}) "
                f"out of range (data {len(data)} bytes)")
        slot = struct.unpack_from(">I", data, byte_off)[0]
        next_word = (slot >> 16) & 0xFFFF
        target_word = slot & 0xFFFF
        targets.append(target_word)
        if next_word == 0xFFFF:
            return len(targets), targets
        word = next_word
    raise RuntimeError(f"chain did not terminate after {max_steps} steps")


def load_archive_index(path: Path) -> tuple[zipfile.ZipFile, set[str]]:
    zf = zipfile.ZipFile(path, "r")
    return zf, set(zf.namelist())


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--torch", type=Path, required=True)
    ap.add_argument("--source", type=Path, required=True,
                    help=".o2r archive OR directory of <file_id>.relo")
    ap.add_argument("--reloc-table", type=Path, required=True)
    ap.add_argument("--file-id", action="append", type=int, default=None)
    ap.add_argument("--out", type=Path, default=None)
    ap.add_argument("--verbose", action="store_true")
    args = ap.parse_args()

    table = parse_reloc_table(args.reloc_table)
    if not table:
        print(f"error: no entries parsed from {args.reloc_table}", file=sys.stderr)
        return 2

    torch_zf, torch_names = load_archive_index(args.torch)

    # Source-side: enumerate file_ids. From .o2r if zip, otherwise from
    # directory of <id>.relo.
    source_resources: dict[int, bytes] = {}
    if args.source.is_dir():
        for entry in sorted(args.source.glob("*.relo")):
            try:
                fid = int(entry.stem)
            except ValueError:
                continue
            source_resources[fid] = entry.read_bytes()
    else:
        sf = zipfile.ZipFile(args.source, "r")
        # The source archive may store entries by RelocFileTable path or
        # by <file_id>.relo. Handle either.
        path_to_fid = {p: fid for fid, p in table.items()}
        for name in sf.namelist():
            if name in path_to_fid:
                source_resources[path_to_fid[name]] = sf.read(name)
            elif name.endswith(".relo"):
                try:
                    fid = int(Path(name).stem)
                    source_resources[fid] = sf.read(name)
                except ValueError:
                    pass

    if args.file_id is not None:
        source_resources = {fid: blob for fid, blob in source_resources.items()
                            if fid in set(args.file_id)}

    if not source_resources:
        print("error: no source resources found / matched", file=sys.stderr)
        return 2

    rows: list[tuple[int, str, FieldDiff, str]] = []  # (fid, name, diff, note)

    for fid in sorted(source_resources):
        res_path = table.get(fid)
        if res_path is None:
            rows.append((fid, "<no path>", None, "no entry in reloc table"))
            continue
        if res_path not in torch_names:
            rows.append((fid, res_path, None, "not in torch archive"))
            continue
        try:
            torch_blob = torch_zf.read(res_path)
            torch_res = RelocResource.parse(torch_blob)
            src_res = RelocResource.parse(source_resources[fid])
        except Exception as e:
            rows.append((fid, res_path, None, f"parse error: {e}"))
            continue

        # Chain integrity sanity-check on source side.
        chain_note = ""
        try:
            steps, _ = chain_walk(src_res.data, src_res.intern_off)
            if src_res.intern_off != 0xFFFF and steps == 0:
                chain_note = "INTERN chain empty?"
        except RuntimeError as e:
            chain_note = f"BAD CHAIN: {e}"

        diff = FieldDiff.of(torch_res, src_res)
        rows.append((fid, res_path, diff, chain_note))

    # Print summary
    n_match = sum(1 for _, _, d, _ in rows if d is not None and d.all_match)
    n_diff = sum(1 for _, _, d, _ in rows if d is not None and not d.all_match)
    n_err = sum(1 for _, _, d, _ in rows if d is None)
    print(f"Compared {len(rows)} files: "
          f"{n_match} identical, {n_diff} differing, {n_err} errors")

    if args.verbose or args.out is None or n_diff <= 50:
        for fid, name, diff, note in rows:
            if diff is None:
                print(f"  [{fid:4d}] {name}: {note}")
                continue
            if diff.all_match and not args.verbose:
                continue
            tags = []
            if not diff.file_id_match: tags.append("file_id")
            if not diff.header_match: tags.append("header")
            if not diff.intern_off_match: tags.append("intern_off")
            if not diff.extern_off_match: tags.append("extern_off")
            if not diff.extern_file_ids_match: tags.append("extern_ids")
            if not diff.data_size_match: tags.append("data_size")
            if diff.data_byte_diff_count > 0:
                tags.append(f"data({diff.data_byte_diff_count}B"
                            f" first@0x{diff.data_first_diff_byte:x})")
            t = ",".join(tags) or "ok"
            extra = f" [{note}]" if note else ""
            print(f"  [{fid:4d}] {name}: {t}{extra}")

    if args.out is not None:
        with args.out.open("w") as f:
            f.write(f"# RelocData validation report\n\n")
            f.write(f"- Torch source : `{args.torch}`\n")
            f.write(f"- From source  : `{args.source}`\n")
            f.write(f"- Identical    : {n_match}\n")
            f.write(f"- Differing    : {n_diff}\n")
            f.write(f"- Errors       : {n_err}\n\n")
            f.write("| file_id | path | status | notes |\n")
            f.write("|---|---|---|---|\n")
            for fid, name, diff, note in rows:
                if diff is None:
                    f.write(f"| {fid} | `{name}` | error | {note} |\n")
                    continue
                if diff.all_match:
                    continue
                tags = []
                if not diff.intern_off_match: tags.append("intern_off")
                if not diff.extern_off_match: tags.append("extern_off")
                if not diff.extern_file_ids_match: tags.append("extern_ids")
                if not diff.data_size_match: tags.append("data_size")
                if diff.data_byte_diff_count > 0:
                    tags.append(f"data{diff.data_byte_diff_count}B")
                f.write(f"| {fid} | `{name}` | {','.join(tags)} | {note} |\n")
        print(f"wrote {args.out}")

    return 0 if n_err == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
