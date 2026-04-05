"""
Script 2: Examine the non-pointer words in reloc files to understand
what data types they contain.

For each non-pointer u32 word, classifies it as:
- "gfx_opcode": high byte matches a known F3DEX2 GBI opcode
- "float": looks like an IEEE 754 float (reasonable magnitude)
- "small_int": value fits in a small range (likely s32/u32 enum, count, ID)
- "packed_u16": both halves are small non-zero values (likely two u16 fields)
- "address_like": looks like a segment address (0x0N______)
- "raw_data": everything else (texture pixels, bytecode, etc.)

Also identifies likely Gfx display list regions (consecutive opcode words)
and likely vertex data regions (patterns of s16 coordinate values).

Usage: python tools/examine_word_patterns.py [path-to-baserom.us.z64]
"""

import os
import struct
import sys
import ctypes
import subprocess
import tempfile
import shutil
from collections import Counter

# ROM constants
RELOC_TABLE_ROM_ADDR   = 0x001AC870
RELOC_FILE_COUNT       = 2132
RELOC_TABLE_ENTRY_SIZE = 12
RELOC_TABLE_SIZE       = (RELOC_FILE_COUNT + 1) * RELOC_TABLE_ENTRY_SIZE
RELOC_DATA_START       = RELOC_TABLE_ROM_ADDR + RELOC_TABLE_SIZE

# F3DEX2 GBI opcodes (high byte of Gfx.w0)
# See gbi.h — these are the most common ones
F3DEX2_OPCODES = {
    0x01,  # G_VTX
    0x03,  # G_MODIFYVTX (F3DEX2)
    0x04,  # G_TRI_FILL (F3DEX2 = 0x04?)
    0x05,  # G_TRI1
    0x06,  # G_TRI2
    0x07,  # G_QUAD
    0xBF,  # G_TRI1 (legacy)
    0xD7,  # G_TEXTURE
    0xD9,  # G_SETOTHERMODE_H
    0xDA,  # G_SETOTHERMODE_L
    0xDB,  # G_RDPHALF_1
    0xDC,  # G_MOVEWORD
    0xDD,  # G_SPNOOP / G_RDPHALF_2 (F3DEX2)
    0xDE,  # G_DL
    0xDF,  # G_ENDDL
    0xE1,  # G_RDPHALF_2
    0xE2,  # G_SETCONVERT
    0xE3,  # G_SETSCISSOR
    0xE4,  # G_TEXRECT
    0xE5,  # G_TEXRECTFLIP
    0xE6,  # G_RDPLOADSYNC
    0xE7,  # G_RDPPIPESYNC
    0xE8,  # G_RDPTILESYNC
    0xE9,  # G_RDPFULLSYNC
    0xED,  # G_SETSCISSOR
    0xEE,  # G_SETZIMG
    0xEF,  # G_SETCIMG
    0xF0,  # G_LOADTLUT
    0xF2,  # G_SETTILESIZE
    0xF3,  # G_LOADBLOCK
    0xF4,  # G_LOADTILE
    0xF5,  # G_SETTILE
    0xF6,  # G_FILLRECT
    0xF7,  # G_SETFILLCOLOR
    0xF8,  # G_SETFOGCOLOR
    0xF9,  # G_SETBLENDCOLOR
    0xFA,  # G_SETPRIMCOLOR
    0xFB,  # G_SETENVCOLOR
    0xFC,  # G_SETCOMBINE
    0xFD,  # G_SETTIMG
    0xFE,  # G_SETZIMG
    0xFF,  # G_SETCIMG
    0xBB,  # G_GEOMETRYMODE (F3DEX2)
    0xB6,  # G_CLEARGEOMETRYMODE (F3DEX2)
    0xB7,  # G_SETGEOMETRYMODE (F3DEX2)
    0xB8,  # G_POPMTX
    0xBC,  # G_MOVEWORD (F3DEX2)
    0xBD,  # G_MOVEMEM
    0xC0,  # G_NOOP
    0xCB,  # G_RDPHALF_2 (F3DEX2)
    0xD8,  # G_SPECIAL_1
}


def is_gfx_opcode(word):
    """Check if a big-endian u32 word looks like a Gfx w0 (has valid opcode in high byte)."""
    opcode = (word >> 24) & 0xFF
    return opcode in F3DEX2_OPCODES


def is_float(word):
    """Check if a big-endian u32 looks like a reasonable IEEE 754 float."""
    if word == 0:
        return True  # 0.0f
    # Unpack as big-endian float
    try:
        f = struct.unpack(">f", struct.pack(">I", word))[0]
    except Exception:
        return False
    # Check for reasonable magnitude (not NaN, not denormalized, reasonable range)
    if f != f:  # NaN
        return False
    exponent = (word >> 23) & 0xFF
    if exponent == 0 or exponent == 255:
        return False  # denorm or inf/nan
    # Reasonable range: 1e-10 to 1e10, or exactly 0
    absf = abs(f)
    return 1e-10 < absf < 1e10


def is_segment_address(word):
    """Check if word looks like an N64 segment address (0x0N______)."""
    seg = (word >> 24) & 0xFF
    return 1 <= seg <= 15 and (word & 0x00FFFFFF) < 0x400000


def classify_word(word):
    """Classify a single big-endian u32 word."""
    if word == 0:
        return "zero"
    if is_gfx_opcode(word):
        return "gfx_opcode"
    if is_segment_address(word):
        return "segment_addr"
    if is_float(word):
        return "float"
    # Small integer (fits in 16 bits or less)
    if word <= 0xFFFF:
        return "small_int"
    # Check for packed u16 pattern: both halves non-zero and relatively small
    hi = (word >> 16) & 0xFFFF
    lo = word & 0xFFFF
    if hi > 0 and lo > 0 and hi < 0x1000 and lo < 0x1000:
        return "packed_u16"
    # Large negative-ish values (common for s32 = -1, -2, etc.)
    if word >= 0xFFFF0000:
        return "neg_int"

    return "other"


def detect_gfx_regions(words, pointer_indices):
    """Detect contiguous regions that look like display lists (pairs of opcode + data words)."""
    regions = []
    i = 0
    n = len(words)

    while i < n - 1:
        if i not in pointer_indices and is_gfx_opcode(words[i]):
            # Start of potential DL region
            start = i
            # DL commands are 8 bytes (2 words): w0 (opcode) + w1 (data)
            j = i
            while j < n - 1 and j not in pointer_indices and is_gfx_opcode(words[j]):
                j += 2  # skip w0 + w1
            if j - start >= 4:  # at least 2 commands
                regions.append((start, j))
            i = j
        else:
            i += 1

    return regions


def detect_vtx_regions(words, pointer_indices, gfx_regions):
    """
    Detect regions that look like vertex data.
    Vtx = 16 bytes = 4 words:
      w0: s16 ob[0] | s16 ob[1]
      w1: s16 ob[2] | u16 flag
      w2: s16 tc[0] | s16 tc[1]
      w3: u8 cn[0] | u8 cn[1] | u8 cn[2] | u8 cn[3]

    Look for groups of 4 words where first 3 words look like packed s16 values.
    """
    # Build set of words in GFX regions
    gfx_words = set()
    for start, end in gfx_regions:
        for k in range(start, end):
            gfx_words.add(k)

    regions = []
    i = 0
    n = len(words)

    while i < n - 3:
        if i in pointer_indices or i in gfx_words:
            i += 1
            continue

        # Check if this looks like a Vtx (4 words)
        # w0: two s16 coords (both halves should be in reasonable range for 3D coords)
        w0 = words[i]
        hi0 = (w0 >> 16) & 0xFFFF
        lo0 = w0 & 0xFFFF
        # s16 range check: interpret as signed
        s_hi0 = hi0 if hi0 < 0x8000 else hi0 - 0x10000
        s_lo0 = lo0 if lo0 < 0x8000 else lo0 - 0x10000

        if abs(s_hi0) < 10000 and abs(s_lo0) < 10000:
            # Could be vertex coords — check if we have a run of these
            start = i
            j = i
            while j < n - 3:
                if j in pointer_indices or j in gfx_words:
                    break
                w = words[j]
                hi = (w >> 16) & 0xFFFF
                lo = w & 0xFFFF
                s_hi = hi if hi < 0x8000 else hi - 0x10000
                s_lo = lo if lo < 0x8000 else lo - 0x10000
                if abs(s_hi) > 10000 or abs(s_lo) > 10000:
                    break
                j += 4  # skip one Vtx (4 words)

            vtx_count = (j - start) // 4
            if vtx_count >= 4:  # at least 4 vertices
                regions.append((start, j, vtx_count))
            i = j if j > i else i + 1
        else:
            i += 1

    return regions


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


def build_vpk0_decompressor():
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


def walk_reloc_chain(data_words, start_offset):
    slots = set()
    current = start_offset
    while current != 0xFFFF and current < len(data_words):
        word = data_words[current]
        next_reloc = (word >> 16) & 0xFFFF
        slots.add(current)
        current = next_reloc
    return slots


def get_decompressed_data(rom, entries, vpk0_lib, file_id):
    """Get decompressed bytes for a file."""
    e = entries[file_id]
    decompressed_bytes = e["decompressed_size"] * 4
    if decompressed_bytes == 0:
        return None

    raw = get_file_raw_bytes(rom, entries, file_id)
    if e["is_compressed"]:
        if vpk0_lib is None:
            return None
        return decompress_file(vpk0_lib, raw, decompressed_bytes)
    else:
        return raw[:decompressed_bytes]


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
    result = build_vpk0_decompressor()
    vpk0_lib = ctypes.CDLL(result[0]) if result else None
    tmp_dir = result[1] if result else None

    # Global counters
    global_word_classes = Counter()
    global_gfx_words = 0
    global_vtx_words = 0
    global_ptr_words = 0
    global_total_words = 0
    global_total_bytes = 0

    # Per-file type classification
    # A file is "gfx_heavy" if >20% of words are in GFX regions
    # "vtx_heavy" if >10% are in VTX regions
    # "struct_heavy" if mostly floats + small ints + pointers
    # "data_only" if no relocs
    file_types = Counter()

    print(f"Analyzing word patterns in {RELOC_FILE_COUNT} files...\n")

    for fid in range(RELOC_FILE_COUNT):
        e = entries[fid]
        data = get_decompressed_data(rom, entries, vpk0_lib, fid)
        if data is None:
            continue

        num_words = len(data) // 4
        if num_words == 0:
            continue

        words = struct.unpack(f">{num_words}I", data[:num_words * 4])

        # Walk reloc chains
        ptr_indices = set()
        if e["reloc_intern"] != 0xFFFF:
            ptr_indices |= walk_reloc_chain(words, e["reloc_intern"])
        if e["reloc_extern"] != 0xFFFF:
            ptr_indices |= walk_reloc_chain(words, e["reloc_extern"])

        # Detect GFX display list regions
        gfx_regions = detect_gfx_regions(words, ptr_indices)
        gfx_word_set = set()
        for start, end in gfx_regions:
            for k in range(start, end):
                gfx_word_set.add(k)

        # Detect vertex regions
        vtx_regions = detect_vtx_regions(words, ptr_indices, gfx_regions)
        vtx_word_set = set()
        for start, end, count in vtx_regions:
            for k in range(start, end):
                vtx_word_set.add(k)

        # Classify remaining words
        file_classes = Counter()
        for i, w in enumerate(words):
            if i in ptr_indices:
                file_classes["pointer"] += 1
            elif i in gfx_word_set:
                file_classes["gfx"] += 1
            elif i in vtx_word_set:
                file_classes["vtx"] += 1
            else:
                cls = classify_word(w)
                file_classes[cls] += 1

        # Accumulate globals
        for cls, count in file_classes.items():
            global_word_classes[cls] += count
        global_total_words += num_words
        global_total_bytes += len(data)
        global_gfx_words += file_classes.get("gfx", 0)
        global_vtx_words += file_classes.get("vtx", 0)
        global_ptr_words += file_classes.get("pointer", 0)

        # Classify file type
        gfx_pct = file_classes.get("gfx", 0) / num_words * 100
        vtx_pct = file_classes.get("vtx", 0) / num_words * 100
        ptr_pct = file_classes.get("pointer", 0) / num_words * 100
        float_pct = file_classes.get("float", 0) / num_words * 100

        if gfx_pct > 20:
            file_types["gfx_heavy (>20% DL)"] += 1
        elif vtx_pct > 10:
            file_types["vtx_heavy (>10% vertex)"] += 1
        elif float_pct > 30:
            file_types["float_heavy (>30% float)"] += 1
        elif ptr_pct == 0 and gfx_pct == 0:
            file_types["no_reloc_no_gfx (pure data)"] += 1
        else:
            file_types["mixed"] += 1

    # Print results
    print("=" * 60)
    print("WORD PATTERN ANALYSIS")
    print("=" * 60)
    print(f"Total words analyzed: {global_total_words:,} ({global_total_bytes:,} bytes)")
    print()

    print("Word classification (all files combined):")
    total = global_total_words
    for cls in ["pointer", "gfx", "vtx", "float", "zero", "small_int",
                "packed_u16", "segment_addr", "neg_int", "gfx_opcode", "other"]:
        count = global_word_classes.get(cls, 0)
        pct = count / total * 100 if total > 0 else 0
        if count > 0:
            print(f"  {cls:>15s}: {count:>10,} words  ({pct:5.1f}%)")

    print()
    print("Byte-swap implications:")
    u32_safe = global_word_classes.get("pointer", 0) + \
               global_word_classes.get("gfx", 0) + \
               global_word_classes.get("float", 0) + \
               global_word_classes.get("zero", 0) + \
               global_word_classes.get("small_int", 0) + \
               global_word_classes.get("neg_int", 0) + \
               global_word_classes.get("segment_addr", 0)
    u16_needs = global_word_classes.get("packed_u16", 0) + \
                global_word_classes.get("vtx", 0)
    unknown = global_word_classes.get("other", 0) + \
              global_word_classes.get("gfx_opcode", 0)

    print(f"  u32-swap safe:   {u32_safe:>10,} words  ({u32_safe/total*100:5.1f}%)")
    print(f"  Needs u16 swap:  {u16_needs:>10,} words  ({u16_needs/total*100:5.1f}%)")
    print(f"  Unknown/other:   {unknown:>10,} words  ({unknown/total*100:5.1f}%)")

    print()
    print("File type distribution:")
    for ftype, count in file_types.most_common():
        print(f"  {ftype:>35s}: {count:4d} files")

    # Cleanup
    if tmp_dir:
        shutil.rmtree(tmp_dir, ignore_errors=True)

    return 0


if __name__ == "__main__":
    sys.exit(main())
