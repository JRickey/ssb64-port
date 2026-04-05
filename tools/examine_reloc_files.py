"""
Examine all 2132 reloc files to understand their internal structure.

For each file, this script:
1. Decompresses if needed (using vpk0)
2. Walks the internal + external reloc chains (counting pointer slots)
3. Computes the fraction of the file that is pointer slots vs data
4. Identifies what word sizes appear in non-pointer regions

The output is a CSV + summary statistics to help understand what
byte-swapping strategy is needed for the PC port.

Usage: python tools/examine_reloc_files.py [path-to-baserom.us.z64]
"""

import os
import struct
import sys
import ctypes
import subprocess
import tempfile
import shutil
from collections import Counter

# ROM constants (shared with other tools)
RELOC_TABLE_ROM_ADDR   = 0x001AC870
RELOC_FILE_COUNT       = 2132
RELOC_TABLE_ENTRY_SIZE = 12
RELOC_TABLE_SIZE       = (RELOC_FILE_COUNT + 1) * RELOC_TABLE_ENTRY_SIZE
RELOC_DATA_START       = RELOC_TABLE_ROM_ADDR + RELOC_TABLE_SIZE


def parse_reloc_table(rom):
    """Parse the full reloc table including sentinel entry."""
    entries = []
    for i in range(RELOC_FILE_COUNT + 1):
        offset = RELOC_TABLE_ROM_ADDR + i * RELOC_TABLE_ENTRY_SIZE
        data = rom[offset:offset + RELOC_TABLE_ENTRY_SIZE]
        first_word, reloc_intern, compressed_size, reloc_extern, decompressed_size = \
            struct.unpack(">IHHHH", data)
        entries.append({
            "id": i,
            "is_compressed": bool(first_word & 0x80000000),
            "data_offset": first_word & 0x7FFFFFFF,
            "reloc_intern": reloc_intern,       # in u32 words, 0xFFFF = none
            "compressed_size": compressed_size,  # in u32 words
            "reloc_extern": reloc_extern,        # in u32 words, 0xFFFF = none
            "decompressed_size": decompressed_size,  # in u32 words
        })
    return entries


def get_file_raw_bytes(rom, entries, file_id):
    """Get the raw (possibly compressed) bytes for a file from ROM."""
    e = entries[file_id]
    data_addr = RELOC_DATA_START + e["data_offset"]
    compressed_bytes = e["compressed_size"] * 4
    return rom[data_addr:data_addr + compressed_bytes]


def build_vpk0_decompressor():
    """Build the VPK0 decompressor as a shared library."""
    repo_root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    vpk0_c = os.path.join(repo_root, "torch", "lib", "libvpk0", "vpk0.c")

    if not os.path.exists(vpk0_c):
        return None

    tmp_dir = tempfile.mkdtemp(prefix="vpk0_exam_")

    if sys.platform == "win32":
        dll_path = os.path.join(tmp_dir, "vpk0.dll")
        bat_path = os.path.join(tmp_dir, "build.bat")
        # Write a .def file to export the symbols
        def_path = os.path.join(tmp_dir, "vpk0.def")
        with open(def_path, "w") as df:
            df.write("EXPORTS\n")
            df.write("    vpk0_decode\n")
            df.write("    vpk0_decoded_size\n")
        with open(bat_path, "w") as bf:
            bf.write('@echo off\n')
            bf.write('call "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Auxiliary\\Build\\vcvars64.bat" >nul 2>&1\n')
            bf.write(f'cl /O2 /LD /Fe:"{dll_path}" "{vpk0_c}" /link /DEF:"{def_path}" >nul 2>&1\n')
        subprocess.run(["cmd", "/c", bat_path], capture_output=True, text=True, cwd=tmp_dir)
    else:
        dll_path = os.path.join(tmp_dir, "vpk0.so")
        for compiler in ["gcc", "clang", "cc"]:
            try:
                subprocess.run(
                    [compiler, "-O2", "-shared", "-fPIC", "-o", dll_path, vpk0_c],
                    check=True, capture_output=True
                )
                break
            except (FileNotFoundError, subprocess.CalledProcessError):
                continue

    if not os.path.exists(dll_path):
        return None

    return dll_path, tmp_dir


def decompress_file(vpk0_lib, raw_bytes, expected_size):
    """Decompress a VPK0-compressed file."""
    if raw_bytes[:4] != b"vpk0":
        return None

    src = ctypes.create_string_buffer(bytes(raw_bytes), len(raw_bytes))
    dst = ctypes.create_string_buffer(expected_size)

    # uint32_t vpk0_decode(const uint8_t *src, size_t src_size, uint8_t *dst, size_t dst_size)
    vpk0_lib.vpk0_decode.restype = ctypes.c_uint32
    vpk0_lib.vpk0_decode.argtypes = [ctypes.c_void_p, ctypes.c_size_t,
                                      ctypes.c_void_p, ctypes.c_size_t]

    result = vpk0_lib.vpk0_decode(src, len(raw_bytes), dst, expected_size)
    if result == 0:
        return None

    return dst.raw[:expected_size]


def walk_reloc_chain(data_words, start_offset):
    """
    Walk a reloc chain in decompressed file data (as big-endian u32 array).
    Returns list of (word_index, target_word_index) for each pointer slot.
    """
    slots = []
    current = start_offset  # in u32 words

    while current != 0xFFFF and current < len(data_words):
        word = data_words[current]
        next_reloc = (word >> 16) & 0xFFFF
        target_words = word & 0xFFFF
        slots.append({
            "slot_word": current,
            "target_word": target_words,
        })
        current = next_reloc

    return slots


def analyze_file(data_bytes, reloc_intern, reloc_extern):
    """Analyze a decompressed file's structure."""
    if len(data_bytes) == 0:
        return None

    num_words = len(data_bytes) // 4
    # Parse as big-endian u32 words
    data_words = struct.unpack(f">{num_words}I", data_bytes[:num_words * 4])

    # Walk reloc chains
    intern_slots = walk_reloc_chain(data_words, reloc_intern) if reloc_intern != 0xFFFF else []
    extern_slots = walk_reloc_chain(data_words, reloc_extern) if reloc_extern != 0xFFFF else []

    # Mark which words are pointer slots
    pointer_word_indices = set()
    for s in intern_slots:
        pointer_word_indices.add(s["slot_word"])
    for s in extern_slots:
        pointer_word_indices.add(s["slot_word"])

    # Analyze non-pointer words
    # Check for patterns that suggest u16 data (values where high or low 16 bits are zero/small)
    non_ptr_words = []
    for i, w in enumerate(data_words):
        if i not in pointer_word_indices:
            non_ptr_words.append(w)

    return {
        "size_bytes": len(data_bytes),
        "num_words": num_words,
        "intern_ptr_count": len(intern_slots),
        "extern_ptr_count": len(extern_slots),
        "total_ptr_count": len(intern_slots) + len(extern_slots),
        "pointer_word_indices": pointer_word_indices,
        "non_ptr_words": non_ptr_words,
    }


def main():
    repo_root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    rom_path = sys.argv[1] if len(sys.argv) > 1 else os.path.join(repo_root, "baserom.us.z64")

    if not os.path.exists(rom_path):
        print(f"Error: ROM not found at {rom_path}")
        return 1

    print(f"Loading ROM: {rom_path}")
    with open(rom_path, "rb") as f:
        rom = f.read()

    entries = parse_reloc_table(rom)
    print(f"Parsed {RELOC_FILE_COUNT} reloc table entries")

    # Build VPK0 decompressor
    print("Building VPK0 decompressor...")
    result = build_vpk0_decompressor()
    if result is None:
        print("WARNING: Could not build VPK0 decompressor. Compressed files will be skipped.")
        vpk0_lib = None
        tmp_dir = None
    else:
        dll_path, tmp_dir = result
        vpk0_lib = ctypes.CDLL(dll_path)
        print(f"  Built: {dll_path}")

    # Analyze all files
    print(f"\nAnalyzing {RELOC_FILE_COUNT} files...\n")

    stats = {
        "total": 0,
        "compressed": 0,
        "uncompressed": 0,
        "skipped": 0,
        "empty": 0,
        "has_intern_reloc": 0,
        "has_extern_reloc": 0,
        "has_any_reloc": 0,
        "no_reloc": 0,
    }

    # Per-file results
    file_results = []

    # Size distribution
    size_buckets = Counter()  # bucket → count
    ptr_density_buckets = Counter()  # "X%" → count

    for fid in range(RELOC_FILE_COUNT):
        e = entries[fid]
        decompressed_bytes = e["decompressed_size"] * 4

        if decompressed_bytes == 0:
            stats["empty"] += 1
            file_results.append({"id": fid, "status": "empty"})
            continue

        stats["total"] += 1

        # Get decompressed data
        raw = get_file_raw_bytes(rom, entries, fid)

        if e["is_compressed"]:
            stats["compressed"] += 1
            if vpk0_lib is None:
                stats["skipped"] += 1
                file_results.append({"id": fid, "status": "skipped_no_decompressor"})
                continue
            data = decompress_file(vpk0_lib, raw, decompressed_bytes)
            if data is None:
                stats["skipped"] += 1
                file_results.append({"id": fid, "status": "decompress_failed"})
                continue
        else:
            stats["uncompressed"] += 1
            data = raw[:decompressed_bytes]

        analysis = analyze_file(data, e["reloc_intern"], e["reloc_extern"])
        if analysis is None:
            file_results.append({"id": fid, "status": "analyze_failed"})
            continue

        has_intern = e["reloc_intern"] != 0xFFFF
        has_extern = e["reloc_extern"] != 0xFFFF

        if has_intern:
            stats["has_intern_reloc"] += 1
        if has_extern:
            stats["has_extern_reloc"] += 1
        if has_intern or has_extern:
            stats["has_any_reloc"] += 1
        else:
            stats["no_reloc"] += 1

        # Size bucket
        if decompressed_bytes < 64:
            bucket = "<64B"
        elif decompressed_bytes < 256:
            bucket = "64-255B"
        elif decompressed_bytes < 1024:
            bucket = "256B-1K"
        elif decompressed_bytes < 4096:
            bucket = "1K-4K"
        elif decompressed_bytes < 16384:
            bucket = "4K-16K"
        elif decompressed_bytes < 65536:
            bucket = "16K-64K"
        else:
            bucket = ">64K"
        size_buckets[bucket] += 1

        # Pointer density
        if analysis["num_words"] > 0:
            density_pct = (analysis["total_ptr_count"] / analysis["num_words"]) * 100
        else:
            density_pct = 0
        density_bucket = f"{int(density_pct // 5) * 5}-{int(density_pct // 5) * 5 + 5}%"
        ptr_density_buckets[density_bucket] += 1

        file_results.append({
            "id": fid,
            "status": "ok",
            "size": decompressed_bytes,
            "compressed": e["is_compressed"],
            "intern_ptrs": analysis["intern_ptr_count"],
            "extern_ptrs": analysis["extern_ptr_count"],
            "total_ptrs": analysis["total_ptr_count"],
            "num_words": analysis["num_words"],
            "density_pct": density_pct,
        })

    # Print summary
    print("=" * 60)
    print("RELOC FILE ANALYSIS SUMMARY")
    print("=" * 60)
    print(f"  Total files:      {RELOC_FILE_COUNT}")
    print(f"  Empty files:      {stats['empty']}")
    print(f"  Compressed:       {stats['compressed']}")
    print(f"  Uncompressed:     {stats['uncompressed']}")
    print(f"  Skipped:          {stats['skipped']}")
    print(f"  Has intern reloc: {stats['has_intern_reloc']}")
    print(f"  Has extern reloc: {stats['has_extern_reloc']}")
    print(f"  Has any reloc:    {stats['has_any_reloc']}")
    print(f"  No reloc at all:  {stats['no_reloc']}")

    print(f"\nFile size distribution:")
    for bucket in ["<64B", "64-255B", "256B-1K", "1K-4K", "4K-16K", "16K-64K", ">64K"]:
        count = size_buckets.get(bucket, 0)
        print(f"  {bucket:>10s}: {count:4d} files")

    print(f"\nPointer density distribution (% of words that are pointer slots):")
    for pct in range(0, 105, 5):
        bucket = f"{pct}-{pct+5}%"
        count = ptr_density_buckets.get(bucket, 0)
        if count > 0:
            print(f"  {bucket:>8s}: {count:4d} files")

    # Top 20 largest files
    ok_files = [r for r in file_results if r["status"] == "ok"]
    ok_files_sorted = sorted(ok_files, key=lambda x: x["size"], reverse=True)
    print(f"\nTop 20 largest files:")
    print(f"  {'ID':>6s}  {'Size':>8s}  {'Comp':>4s}  {'IntPtrs':>7s}  {'ExtPtrs':>7s}  {'Density':>7s}")
    for r in ok_files_sorted[:20]:
        comp = "Y" if r["compressed"] else "N"
        print(f"  {r['id']:>6d}  {r['size']:>8d}  {comp:>4s}  {r['intern_ptrs']:>7d}  {r['extern_ptrs']:>7d}  {r['density_pct']:>6.1f}%")

    # Files with NO reloc (pure data, no pointers)
    no_reloc_files = [r for r in ok_files if r["total_ptrs"] == 0]
    print(f"\nFiles with zero pointer slots: {len(no_reloc_files)}")
    if no_reloc_files:
        total_bytes = sum(r["size"] for r in no_reloc_files)
        print(f"  Total bytes in zero-pointer files: {total_bytes} ({total_bytes/1024:.1f} KB)")

    # Write CSV for further analysis
    csv_path = os.path.join(repo_root, "tools", "reloc_file_analysis.csv")
    with open(csv_path, "w") as f:
        f.write("file_id,status,size_bytes,compressed,intern_ptrs,extern_ptrs,total_ptrs,num_words,density_pct\n")
        for r in file_results:
            if r["status"] == "ok":
                f.write(f"{r['id']},{r['status']},{r['size']},{r['compressed']},"
                        f"{r['intern_ptrs']},{r['extern_ptrs']},{r['total_ptrs']},"
                        f"{r['num_words']},{r['density_pct']:.2f}\n")
            else:
                f.write(f"{r['id']},{r['status']},,,,,,\n")
    print(f"\nCSV written to: {csv_path}")

    # Cleanup
    if tmp_dir:
        try:
            shutil.rmtree(tmp_dir, ignore_errors=True)
        except Exception:
            pass

    return 0


if __name__ == "__main__":
    sys.exit(main())
