"""
Script 3: Parse GFX display list commands to map complete blob layouts.

For each file, parses actual F3DEX2 display list commands to find:
- gSPVertex references → identifies vertex buffer regions
- gDPSetTextureImage references → identifies texture pixel regions
- gDPLoadTLUT references → identifies palette regions

Produces a full layout map: what percentage of each blob is DL commands,
vertices, texture pixels, palettes, struct data, and unknown.

Also examines the "other" words from script 2 to determine what they are.

Usage: python tools/examine_blob_layout.py [path-to-baserom.us.z64]
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

# F3DEX2 opcodes we care about for data reference tracking
OP_VTX         = 0x01
OP_DL          = 0xDE
OP_ENDDL       = 0xDF
OP_SETTIMG     = 0xFD  # gDPSetTextureImage
OP_LOADTLUT    = 0xF0  # gDPLoadTLUT
OP_LOADBLOCK   = 0xF3  # gDPLoadBlock
OP_LOADTILE    = 0xF4
OP_SETTILESIZE = 0xF2
OP_SETTILE     = 0xF5

# Texture format sizes (bits per pixel)
G_IM_SIZ_4b  = 0
G_IM_SIZ_8b  = 1
G_IM_SIZ_16b = 2
G_IM_SIZ_32b = 3

BPP = {0: 4, 1: 8, 2: 16, 3: 32}


def parse_reloc_table(rom):
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
            "reloc_intern": reloc_intern,
            "compressed_size": compressed_size,
            "reloc_extern": reloc_extern,
            "decompressed_size": decompressed_size,
        })
    return entries


def get_file_raw_bytes(rom, entries, file_id):
    e = entries[file_id]
    data_addr = RELOC_DATA_START + e["data_offset"]
    compressed_bytes = e["compressed_size"] * 4
    return rom[data_addr:data_addr + compressed_bytes]


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
    if result == 0:
        return None
    return dst.raw[:expected_size]


def get_data(rom, entries, vpk0_lib, fid):
    e = entries[fid]
    dec_bytes = e["decompressed_size"] * 4
    if dec_bytes == 0:
        return None
    raw = get_file_raw_bytes(rom, entries, fid)
    if e["is_compressed"]:
        if vpk0_lib is None:
            return None
        return decompress_file(vpk0_lib, raw, dec_bytes)
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


def parse_display_lists(words, ptr_indices, file_size_bytes):
    """
    Find and parse display list commands. Return:
    - dl_word_set: set of word indices that are DL commands (w0+w1 pairs)
    - vtx_refs: list of (byte_offset_in_file, num_vertices) from gSPVertex
    - tex_refs: list of (byte_offset_in_file, fmt, siz) from gDPSetTextureImage
    - sub_dl_refs: list of byte_offset from gSPDisplayList calls (intra-file)
    """
    dl_words = set()
    vtx_refs = []
    tex_refs = []
    sub_dl_refs = []
    loadblock_info = []  # (num_texels_upper_bound,) from most recent LoadBlock

    n = len(words)
    i = 0
    while i < n - 1:
        w0 = words[i]
        w1 = words[i + 1]
        opcode = (w0 >> 24) & 0xFF

        # Only consider command if it looks like a valid GFX opcode
        # and neither word is a known pointer slot
        if i in ptr_indices or (i + 1) in ptr_indices:
            i += 1
            continue

        if opcode == OP_VTX:
            # gSPVertex: w0 = 01 NN NNNN (num_vtx in bits 12-15, length in bytes)
            # F3DEX2: w0[23:12] = num_vertices, w0[11:1] = length/2 - 1 (?)
            # Actually in F3DEX2: w0 = 0x01 | (n << 12) | ((v0 + n) << 1)
            # The vertex count is (w0 >> 12) & 0xFF
            num_vtx = (w0 >> 12) & 0xFF
            # w1 = segment address of vertex buffer
            seg = (w1 >> 24) & 0xFF
            offset_in_seg = w1 & 0x00FFFFFF

            if seg == 0 and offset_in_seg < file_size_bytes:
                # Segment 0 = direct offset within this file's data
                vtx_refs.append((offset_in_seg, num_vtx))
                dl_words.add(i)
                dl_words.add(i + 1)
                i += 2
                continue
            elif 1 <= seg <= 15:
                # Segment address — points to data that was relocated
                # The reloc chain should have patched w1 to a pointer
                # If w1 is in ptr_indices, it was already handled
                dl_words.add(i)
                dl_words.add(i + 1)
                i += 2
                continue

        elif opcode == OP_SETTIMG:
            # gDPSetTextureImage: w0 = FD FF F_SS (fmt, siz in bits)
            # w0[23:21] = fmt, w0[20:19] = siz
            fmt = (w0 >> 21) & 0x07
            siz = (w0 >> 19) & 0x03
            seg = (w1 >> 24) & 0xFF
            offset_in_seg = w1 & 0x00FFFFFF

            if seg == 0 and offset_in_seg < file_size_bytes:
                tex_refs.append((offset_in_seg, fmt, siz))
            dl_words.add(i)
            dl_words.add(i + 1)
            i += 2
            continue

        elif opcode == OP_DL:
            # gSPDisplayList: w1 = segment address of sub-DL
            seg = (w1 >> 24) & 0xFF
            offset_in_seg = w1 & 0x00FFFFFF
            if seg == 0 and offset_in_seg < file_size_bytes:
                sub_dl_refs.append(offset_in_seg)
            dl_words.add(i)
            dl_words.add(i + 1)
            i += 2
            continue

        elif opcode == OP_ENDDL:
            dl_words.add(i)
            dl_words.add(i + 1)
            i += 2
            continue

        elif opcode == OP_LOADBLOCK:
            # gDPLoadBlock: w1 bits [11:0] = dxt, w1 bits [23:12] = texels-1
            num_texels_m1 = (w1 >> 12) & 0xFFF
            loadblock_info.append(num_texels_m1 + 1)
            dl_words.add(i)
            dl_words.add(i + 1)
            i += 2
            continue

        # Other known opcodes — mark as DL but don't extract refs
        elif opcode in {0x03, 0x05, 0x06, 0x07, 0xBF, 0xD7, 0xD9, 0xDA,
                        0xDB, 0xDC, 0xDD, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5,
                        0xE6, 0xE7, 0xE8, 0xE9, 0xED, 0xEE, 0xEF,
                        0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB,
                        0xFC, 0xFE, 0xFF, 0xBB, 0xB6, 0xB7, 0xB8, 0xBC,
                        0xBD, 0xC0, 0xCB, 0xD8, 0xF0, 0xF2}:
            dl_words.add(i)
            dl_words.add(i + 1)
            i += 2
            continue

        i += 1

    return dl_words, vtx_refs, tex_refs, sub_dl_refs


def map_blob_layout(words, data_bytes, ptr_indices, dl_words, vtx_refs, tex_refs):
    """
    Given parsed DL refs, mark each word in the blob with a region type.
    Returns an array of region labels per word.
    """
    n = len(words)
    labels = ["unknown"] * n
    file_size = len(data_bytes)

    # Mark pointer slots
    for idx in ptr_indices:
        if idx < n:
            labels[idx] = "pointer"

    # Mark DL command words
    for idx in dl_words:
        if idx < n and labels[idx] == "unknown":
            labels[idx] = "dl_cmd"

    # Mark vertex regions (16 bytes = 4 words per vertex)
    for (byte_off, num_vtx) in vtx_refs:
        word_off = byte_off // 4
        vtx_words = num_vtx * 4  # 4 words per Vtx
        for k in range(word_off, min(word_off + vtx_words, n)):
            if labels[k] == "unknown":
                labels[k] = "vtx_data"

    # Mark texture regions
    # We don't know exact sizes from SETTIMG alone, but we can mark the
    # start and estimate based on the next known region boundary
    tex_starts = sorted(set(ref[0] for ref in tex_refs))
    for byte_off in tex_starts:
        word_off = byte_off // 4
        if word_off < n and labels[word_off] == "unknown":
            # Mark forward until we hit a non-unknown label or end
            k = word_off
            while k < n and labels[k] == "unknown":
                labels[k] = "tex_data"
                k += 1

    return labels


def is_float(word):
    if word == 0:
        return True
    try:
        f = struct.unpack(">f", struct.pack(">I", word))[0]
    except Exception:
        return False
    if f != f:
        return False
    exp = (word >> 23) & 0xFF
    if exp == 0 or exp == 255:
        return False
    return 1e-10 < abs(f) < 1e10


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

    # Global word counters per region type
    region_totals = Counter()
    total_words = 0

    # Track how the "unknown" words break down further
    unknown_sub = Counter()

    # Per-file: what % is each region type
    file_profiles = []

    print(f"Parsing display lists in {RELOC_FILE_COUNT} files...\n")

    for fid in range(RELOC_FILE_COUNT):
        e = entries[fid]
        data = get_data(rom, entries, vpk0_lib, fid)
        if data is None:
            continue

        num_words = len(data) // 4
        if num_words == 0:
            continue

        words = struct.unpack(f">{num_words}I", data[:num_words * 4])

        # Reloc chains
        ptr_indices = set()
        if e["reloc_intern"] != 0xFFFF:
            ptr_indices |= walk_reloc_chain(words, e["reloc_intern"])
        if e["reloc_extern"] != 0xFFFF:
            ptr_indices |= walk_reloc_chain(words, e["reloc_extern"])

        # Parse display lists
        dl_words, vtx_refs, tex_refs, sub_dl_refs = \
            parse_display_lists(words, ptr_indices, len(data))

        # Map layout
        labels = map_blob_layout(words, data, ptr_indices, dl_words, vtx_refs, tex_refs)

        # Count per region
        file_counts = Counter(labels)
        for region, count in file_counts.items():
            region_totals[region] += count
        total_words += num_words

        # Sub-classify unknown words
        for i, label in enumerate(labels):
            if label == "unknown":
                w = words[i]
                if w == 0:
                    unknown_sub["zero"] += 1
                elif is_float(w):
                    unknown_sub["float"] += 1
                elif w <= 0xFFFF:
                    unknown_sub["small_int"] += 1
                elif w >= 0xFFFF0000:
                    unknown_sub["neg_int"] += 1
                else:
                    # Check if it looks like packed s16 (vertex-like but not in vtx region)
                    hi = (w >> 16) & 0xFFFF
                    lo = w & 0xFFFF
                    s_hi = hi if hi < 0x8000 else hi - 0x10000
                    s_lo = lo if lo < 0x8000 else lo - 0x10000
                    if abs(s_hi) < 5000 and abs(s_lo) < 5000:
                        unknown_sub["packed_s16"] += 1
                    else:
                        unknown_sub["opaque_data"] += 1

        # File profile
        profile = {}
        for region in ["pointer", "dl_cmd", "vtx_data", "tex_data", "unknown"]:
            profile[region] = file_counts.get(region, 0) / num_words * 100 if num_words > 0 else 0
        profile["id"] = fid
        profile["size"] = len(data)
        profile["num_words"] = num_words
        file_profiles.append(profile)

    # Results
    print("=" * 65)
    print("BLOB LAYOUT ANALYSIS (via GFX command parsing)")
    print("=" * 65)
    print(f"Total words: {total_words:,} ({total_words * 4:,} bytes)\n")

    print("Region breakdown:")
    for region in ["dl_cmd", "vtx_data", "tex_data", "pointer", "unknown"]:
        count = region_totals.get(region, 0)
        pct = count / total_words * 100 if total_words > 0 else 0
        bytes_val = count * 4
        print(f"  {region:>12s}: {count:>10,} words ({bytes_val:>12,} bytes)  {pct:5.1f}%")

    print(f"\nUnknown word sub-classification:")
    unk_total = sum(unknown_sub.values())
    for sub, count in unknown_sub.most_common():
        pct = count / total_words * 100
        print(f"  {sub:>15s}: {count:>10,} words  ({pct:5.1f}% of total)")

    # Byte-swap strategy summary
    print(f"\n{'=' * 65}")
    print("BYTE-SWAP STRATEGY IMPLICATIONS")
    print(f"{'=' * 65}")

    dl = region_totals.get("dl_cmd", 0)
    vtx = region_totals.get("vtx_data", 0)
    tex = region_totals.get("tex_data", 0)
    ptr = region_totals.get("pointer", 0)
    unk = region_totals.get("unknown", 0)

    print(f"\n  DL commands ({dl/total_words*100:.1f}%):")
    print(f"    Each is a u32 pair (w0, w1). u32 swap is correct.")
    print(f"\n  Vertex data ({vtx/total_words*100:.1f}%):")
    print(f"    Vtx has s16 fields. Needs u16 swap, NOT u32 swap.")
    print(f"\n  Texture data ({tex/total_words*100:.1f}%):")
    print(f"    Format-dependent. 4bpp/8bpp = byte data (no swap).")
    print(f"    16bpp = u16 swap. 32bpp = u32 swap.")
    print(f"\n  Pointer slots ({ptr/total_words*100:.1f}%):")
    print(f"    Overwritten by reloc chain. Swap before reading chain,")
    print(f"    then overwrite with native-endian token.")
    print(f"\n  Unknown/struct ({unk/total_words*100:.1f}%):")
    print(f"    Floats: {unknown_sub.get('float', 0):,} (u32 swap)")
    print(f"    Zeros:  {unknown_sub.get('zero', 0):,} (no swap needed)")
    print(f"    Small ints: {unknown_sub.get('small_int', 0):,} (u32 swap)")
    print(f"    Neg ints: {unknown_sub.get('neg_int', 0):,} (u32 swap)")
    print(f"    Packed s16: {unknown_sub.get('packed_s16', 0):,} (u16 swap)")
    print(f"    Opaque data: {unknown_sub.get('opaque_data', 0):,} (need further analysis)")

    # Files that are purely struct data (no DL, no VTX, no TEX)
    struct_only = [p for p in file_profiles
                   if p["dl_cmd"] == 0 and p["vtx_data"] == 0 and p["tex_data"] == 0]
    print(f"\n  Files with NO display lists, vertices, or textures: {len(struct_only)}")
    if struct_only:
        total_bytes = sum(p["size"] for p in struct_only)
        print(f"    Total size: {total_bytes:,} bytes ({total_bytes/1024:.1f} KB)")

    # Write per-file CSV
    csv_path = os.path.join(repo_root, "tools", "blob_layout_analysis.csv")
    with open(csv_path, "w") as f:
        f.write("file_id,size_bytes,num_words,pct_dl,pct_vtx,pct_tex,pct_ptr,pct_unknown\n")
        for p in file_profiles:
            f.write(f"{p['id']},{p['size']},{p['num_words']},"
                    f"{p['dl_cmd']:.1f},{p['vtx_data']:.1f},{p['tex_data']:.1f},"
                    f"{p['pointer']:.1f},{p['unknown']:.1f}\n")
    print(f"\nPer-file CSV: {csv_path}")

    if tmp_dir:
        shutil.rmtree(tmp_dir, ignore_errors=True)
    return 0


if __name__ == "__main__":
    sys.exit(main())
