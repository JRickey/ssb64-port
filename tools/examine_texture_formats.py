"""
Script 5: Analyze texture/palette pixel data formats and the opaque binary regions.

Parses gDPSetTextureImage commands from display lists to determine:
- What texture formats (CI4, CI8, RGBA16, IA8, I4, etc.) are used
- How much data each format accounts for
- What byte-swap strategy each format needs

Also examines the "opaque binary" pointer targets from script 4 by
looking at nearby gDPSetTextureImage commands that reference them.

Finally, produces a complete byte-swap budget: for every byte in
every file, what swap width is needed (none/u16/u32).

Usage: python tools/examine_texture_formats.py [path-to-baserom.us.z64]
"""

import os
import struct
import sys
import ctypes
import subprocess
import tempfile
import shutil
from collections import Counter, defaultdict

# ROM constants
RELOC_TABLE_ROM_ADDR   = 0x001AC870
RELOC_FILE_COUNT       = 2132
RELOC_TABLE_ENTRY_SIZE = 12
RELOC_TABLE_SIZE       = (RELOC_FILE_COUNT + 1) * RELOC_TABLE_ENTRY_SIZE
RELOC_DATA_START       = RELOC_TABLE_ROM_ADDR + RELOC_TABLE_SIZE

# F3DEX2 opcodes
OP_VTX       = 0x01
OP_DL        = 0xDE
OP_ENDDL     = 0xDF
OP_SETTIMG   = 0xFD
OP_LOADBLOCK = 0xF3
OP_LOADTILE  = 0xF4
OP_SETTILE   = 0xF5
OP_SETTILESIZE = 0xF2
OP_LOADTLUT  = 0xF0

GFX_OPCODES = {
    0x01, 0x03, 0x05, 0x06, 0x07, 0xBF, 0xD7, 0xD9, 0xDA, 0xDB, 0xDC,
    0xDD, 0xDE, 0xDF, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8,
    0xE9, 0xED, 0xEE, 0xEF, 0xF0, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7,
    0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF, 0xBB, 0xB6, 0xB7,
    0xB8, 0xBC, 0xBD, 0xC0, 0xCB, 0xD8,
}

# N64 texture formats
FMT_NAMES = {0: "RGBA", 1: "YUV", 2: "CI", 3: "IA", 4: "I"}
SIZ_NAMES = {0: "4b", 1: "8b", 2: "16b", 3: "32b"}
BPP = {0: 4, 1: 8, 2: 16, 3: 32}

# Swap strategy per format+size combo
# Key insight: the swap needed depends on the PIXEL SIZE, not the format
# 4bpp: pixels are nibbles packed into bytes -> NO swap (byte-granular)
# 8bpp: pixels are bytes -> NO swap
# 16bpp: pixels are u16 -> u16 swap
# 32bpp: pixels are u32 -> u32 swap
# Palettes: always u16 (RGBA5551) -> u16 swap
SWAP_FOR_SIZ = {0: "none", 1: "none", 2: "u16", 3: "u32"}


def parse_reloc_table(rom):
    entries = []
    for i in range(RELOC_FILE_COUNT + 1):
        offset = RELOC_TABLE_ROM_ADDR + i * RELOC_TABLE_ENTRY_SIZE
        data = rom[offset:offset + RELOC_TABLE_ENTRY_SIZE]
        first_word, reloc_intern, compressed_size, reloc_extern, decompressed_size = \
            struct.unpack(">IHHHH", data)
        entries.append({
            "is_compressed": bool(first_word & 0x80000000),
            "data_offset": first_word & 0x7FFFFFFF,
            "reloc_intern": reloc_intern,
            "compressed_size": compressed_size,
            "reloc_extern": reloc_extern,
            "decompressed_size": decompressed_size,
        })
    return entries


def build_vpk0():
    repo_root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    vpk0_c = os.path.join(repo_root, "torch", "lib", "libvpk0", "vpk0.c")
    if not os.path.exists(vpk0_c):
        return None
    tmp_dir = tempfile.mkdtemp(prefix="vpk0_exam_")
    if sys.platform == "win32":
        dll_path = os.path.join(tmp_dir, "vpk0.dll")
        def_path = os.path.join(tmp_dir, "vpk0.def")
        bat_path = os.path.join(tmp_dir, "build.bat")
        with open(def_path, "w") as df:
            df.write("EXPORTS\n    vpk0_decode\n    vpk0_decoded_size\n")
        with open(bat_path, "w") as bf:
            bf.write('@echo off\n')
            bf.write('call "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Auxiliary\\Build\\vcvars64.bat" >nul 2>&1\n')
            bf.write(f'cl /O2 /LD /Fe:"{dll_path}" "{vpk0_c}" /link /DEF:"{def_path}" >nul 2>&1\n')
        subprocess.run(["cmd", "/c", bat_path], capture_output=True, text=True, cwd=tmp_dir)
    else:
        dll_path = os.path.join(tmp_dir, "vpk0.so")
        for cc in ["gcc", "clang", "cc"]:
            try:
                subprocess.run([cc, "-O2", "-shared", "-fPIC", "-o", dll_path, vpk0_c],
                               check=True, capture_output=True)
                break
            except (FileNotFoundError, subprocess.CalledProcessError):
                continue
    if not os.path.exists(dll_path):
        return None
    return ctypes.CDLL(dll_path), tmp_dir


def decompress_file(vpk0_lib, raw_bytes, expected_size):
    if raw_bytes[:4] != b"vpk0":
        return None
    src = ctypes.create_string_buffer(bytes(raw_bytes), len(raw_bytes))
    dst = ctypes.create_string_buffer(expected_size)
    vpk0_lib.vpk0_decode.restype = ctypes.c_uint32
    vpk0_lib.vpk0_decode.argtypes = [ctypes.c_void_p, ctypes.c_size_t,
                                      ctypes.c_void_p, ctypes.c_size_t]
    result = vpk0_lib.vpk0_decode(src, len(raw_bytes), dst, expected_size)
    return dst.raw[:expected_size] if result else None


def get_data(rom, entries, vpk0_lib, fid):
    e = entries[fid]
    dec_bytes = e["decompressed_size"] * 4
    if dec_bytes == 0:
        return None
    data_addr = RELOC_DATA_START + e["data_offset"]
    raw = rom[data_addr:data_addr + e["compressed_size"] * 4]
    if e["is_compressed"]:
        return decompress_file(vpk0_lib, raw, dec_bytes) if vpk0_lib else None
    return raw[:dec_bytes]


def walk_reloc_chain(words, start):
    slots = set()
    cur = start
    while cur != 0xFFFF and cur < len(words):
        w = words[cur]
        nxt = (w >> 16) & 0xFFFF
        slots.add(cur)
        cur = nxt
    return slots


def walk_reloc_chain_targets(words, start):
    results = []
    cur = start
    while cur != 0xFFFF and cur < len(words):
        w = words[cur]
        nxt = (w >> 16) & 0xFFFF
        target = w & 0xFFFF
        results.append((cur, target))
        cur = nxt
    return results


def is_float(word):
    if word == 0:
        return True
    exp = (word >> 23) & 0xFF
    if exp == 0 or exp == 255:
        return False
    try:
        f = struct.unpack(">f", struct.pack(">I", word))[0]
        return f == f and 1e-10 < abs(f) < 1e10
    except:
        return False


def analyze_file(words, num_words, ptr_indices, file_size):
    """
    Full analysis: parse DL commands, find texture refs, compute swap budget.
    Returns per-word swap requirement array and texture stats.
    """
    # swap_map: for each word, what swap width is needed
    # "u32" = swap as u32, "u16" = swap as u16, "none" = no swap, "ptr" = reloc handles it
    swap_map = ["unknown"] * num_words

    # Mark pointer slots
    for idx in ptr_indices:
        if idx < num_words:
            swap_map[idx] = "ptr"

    # Parse DL commands and collect texture references
    tex_refs = []  # (byte_offset, fmt, siz)
    vtx_refs = []  # (byte_offset, num_vtx)
    dl_words = set()

    i = 0
    while i < num_words - 1:
        if i in ptr_indices or (i + 1) in ptr_indices:
            i += 1
            continue

        w0 = words[i]
        w1 = words[i + 1]
        opcode = (w0 >> 24) & 0xFF

        if opcode not in GFX_OPCODES:
            i += 1
            continue

        dl_words.add(i)
        dl_words.add(i + 1)

        if opcode == OP_VTX:
            num_vtx = (w0 >> 12) & 0xFF
            seg = (w1 >> 24) & 0xFF
            off = w1 & 0x00FFFFFF
            if seg == 0 and off < file_size:
                vtx_refs.append((off, num_vtx))
            # Even if segment != 0, the pointer was relocated
            # Check if w1 position is a pointer slot
            # (reloc chain would have patched the segment address)

        elif opcode == OP_SETTIMG:
            fmt = (w0 >> 21) & 0x07
            siz = (w0 >> 19) & 0x03
            seg = (w1 >> 24) & 0xFF
            off = w1 & 0x00FFFFFF
            tex_refs.append((off if seg == 0 else None, fmt, siz))

        i += 2

    # Mark DL commands as u32 swap
    for idx in dl_words:
        if idx < num_words and swap_map[idx] == "unknown":
            swap_map[idx] = "u32"

    # Mark vertex regions as u16 swap
    for (byte_off, num_vtx) in vtx_refs:
        word_off = byte_off // 4
        vtx_words = num_vtx * 4  # 16 bytes = 4 words per Vtx
        for k in range(word_off, min(word_off + vtx_words, num_words)):
            if swap_map[k] == "unknown":
                swap_map[k] = "u16"

    # For remaining unknowns, classify
    for idx in range(num_words):
        if swap_map[idx] != "unknown":
            continue
        w = words[idx]
        if w == 0:
            swap_map[idx] = "none"  # zero, no swap needed
        elif is_float(w):
            swap_map[idx] = "u32"
        elif w <= 0xFFFF:
            swap_map[idx] = "u32"  # small int, u32 swap is safe
        elif w >= 0xFFFF0000:
            swap_map[idx] = "u32"  # negative int
        else:
            # This is the ambiguous category.
            # Could be texture pixels, packed u16 struct fields, etc.
            # For now mark as "ambiguous"
            swap_map[idx] = "ambiguous"

    # Texture format stats
    fmt_siz_counts = Counter()
    for (off, fmt, siz) in tex_refs:
        key = f"{FMT_NAMES.get(fmt, '?')}{SIZ_NAMES.get(siz, '?')}"
        fmt_siz_counts[key] += 1

    return swap_map, fmt_siz_counts, tex_refs, vtx_refs


def main():
    repo_root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    rom_path = sys.argv[1] if len(sys.argv) > 1 else os.path.join(repo_root, "baserom.us.z64")

    if not os.path.exists(rom_path):
        print(f"Error: ROM not found at {rom_path}")
        return 1

    with open(rom_path, "rb") as f:
        rom = f.read()

    entries = parse_reloc_table(rom)

    print("Building VPK0 decompressor...")
    vpk0_result = build_vpk0()
    vpk0_lib = vpk0_result[0] if vpk0_result else None
    tmp_dir = vpk0_result[1] if vpk0_result else None

    # Global swap budget
    swap_budget = Counter()  # "u32" / "u16" / "none" / "ptr" / "ambiguous" -> word count
    total_words = 0

    # Texture format stats
    global_tex_formats = Counter()
    total_tex_refs = 0

    # Files with ambiguous data
    ambiguous_files = []

    print(f"Full swap analysis of {RELOC_FILE_COUNT} files...\n")

    for fid in range(RELOC_FILE_COUNT):
        e = entries[fid]
        data = get_data(rom, entries, vpk0_lib, fid)
        if data is None:
            continue

        num_words = len(data) // 4
        if num_words == 0:
            continue

        words = struct.unpack(f">{num_words}I", data[:num_words * 4])

        ptr_indices = set()
        if e["reloc_intern"] != 0xFFFF:
            ptr_indices |= walk_reloc_chain(words, e["reloc_intern"])
        if e["reloc_extern"] != 0xFFFF:
            ptr_indices |= walk_reloc_chain(words, e["reloc_extern"])

        swap_map, fmt_counts, tex_refs, vtx_refs = \
            analyze_file(words, num_words, ptr_indices, len(data))

        # Accumulate
        for label in swap_map:
            swap_budget[label] += 1
        total_words += num_words

        for fmt_key, count in fmt_counts.items():
            global_tex_formats[fmt_key] += count
        total_tex_refs += len(tex_refs)

        # Track ambiguous
        amb_count = swap_map.count("ambiguous")
        if amb_count > 0:
            ambiguous_files.append((fid, amb_count, num_words, amb_count / num_words * 100))

    # Results
    print("=" * 65)
    print("COMPLETE BYTE-SWAP BUDGET")
    print("=" * 65)
    print(f"Total: {total_words:,} words ({total_words * 4:,} bytes)\n")

    print("Per-word swap requirement:")
    for label in ["u32", "u16", "none", "ptr", "ambiguous"]:
        count = swap_budget.get(label, 0)
        pct = count / total_words * 100 if total_words > 0 else 0
        bytes_val = count * 4
        swap_desc = {
            "u32": "byte-reverse each u32",
            "u16": "byte-reverse each u16 half",
            "none": "no swap needed (zeros, byte data)",
            "ptr": "handled by reloc chain",
            "ambiguous": "needs format-dependent handling",
        }
        print(f"  {label:>10s}: {count:>10,} words ({bytes_val:>12,} bytes)  {pct:5.1f}%  -- {swap_desc[label]}")

    # Texture format distribution
    print(f"\nTexture format distribution ({total_tex_refs:,} gDPSetTextureImage calls):")
    for fmt_key, count in global_tex_formats.most_common():
        pct = count / total_tex_refs * 100 if total_tex_refs > 0 else 0
        # Determine swap need from size suffix
        if "4b" in fmt_key or "8b" in fmt_key:
            swap = "no swap (byte-granular pixels)"
        elif "16b" in fmt_key:
            swap = "u16 swap"
        elif "32b" in fmt_key:
            swap = "u32 swap"
        else:
            swap = "unknown"
        print(f"  {fmt_key:>10s}: {count:>5,} refs  ({pct:5.1f}%)  -- {swap}")

    # Palette analysis
    ci_refs = sum(c for k, c in global_tex_formats.items() if k.startswith("CI"))
    non_ci = total_tex_refs - ci_refs
    print(f"\n  CI (indexed) textures: {ci_refs} ({ci_refs/total_tex_refs*100:.1f}%) -- palettes need u16 swap")
    print(f"  Non-CI textures: {non_ci} ({non_ci/total_tex_refs*100:.1f}%) -- no palettes")

    # Ambiguous data summary
    print(f"\nAmbiguous data (format-dependent, {swap_budget.get('ambiguous', 0):,} words):")
    amb_total = swap_budget.get("ambiguous", 0)
    if amb_total > 0:
        print(f"  This is {amb_total/total_words*100:.1f}% of all data")
        print(f"  Found in {len(ambiguous_files)} files")

        # Top files by ambiguous %
        ambiguous_files.sort(key=lambda x: x[1], reverse=True)
        print(f"\n  Top 15 files by ambiguous word count:")
        print(f"  {'ID':>6s}  {'AmbWords':>8s}  {'Total':>8s}  {'Pct':>6s}")
        for fid, amb, total, pct in ambiguous_files[:15]:
            print(f"  {fid:>6d}  {amb:>8d}  {total:>8d}  {pct:>5.1f}%")

    # Final strategy summary
    u32_words = swap_budget.get("u32", 0)
    u16_words = swap_budget.get("u16", 0)
    none_words = swap_budget.get("none", 0)
    ptr_words = swap_budget.get("ptr", 0)
    amb_words = swap_budget.get("ambiguous", 0)

    determined = u32_words + u16_words + none_words + ptr_words
    determined_pct = determined / total_words * 100

    print(f"\n{'=' * 65}")
    print("STRATEGY SUMMARY")
    print(f"{'=' * 65}")
    print(f"""
  Deterministic swap:     {determined:>10,} words  ({determined_pct:.1f}%)
    u32 swap:             {u32_words:>10,} words  (DL commands, floats, ints)
    u16 swap:             {u16_words:>10,} words  (vertex data)
    no swap:              {none_words:>10,} words  (zeros)
    reloc-handled:        {ptr_words:>10,} words  (pointer slots)

  Ambiguous:              {amb_words:>10,} words  ({amb_words/total_words*100:.1f}%)
    Most likely texture pixel data + palette data.
    Swap strategy depends on format:
      4bpp/8bpp pixels: no swap (byte data)
      16bpp pixels/palettes: u16 swap
      32bpp pixels: u32 swap

  The ambiguous data is {amb_words/total_words*100:.1f}% of total.
  If we default ambiguous to u32 swap, the WORST case for byte-data
  textures (4bpp/8bpp) is corrupted pixels that need runtime fixup.
  If we default to NO swap, 16bpp/32bpp textures break.

  Recommended: classify ambiguous regions using gDPSetTextureImage
  format info to determine per-region swap width at extraction time.
""")

    if tmp_dir:
        shutil.rmtree(tmp_dir, ignore_errors=True)
    return 0


if __name__ == "__main__":
    sys.exit(main())
