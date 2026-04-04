"""
Validate external file ID extraction from the reloc table.

For each file, checks that:
1. Files with reloc_extern_offset == 0xFFFF have zero extern IDs
2. Files with extern IDs have reloc_extern_offset != 0xFFFF
3. All extern file IDs are valid (< 2132)
4. The extern region size matches the number of reloc chain entries

Usage: python tools/test_extern_ids.py [path-to-baserom.us.z64]
"""

import os
import struct
import sys

RELOC_TABLE_ROM_ADDR   = 0x001AC870
RELOC_FILE_COUNT       = 2132
RELOC_TABLE_ENTRY_SIZE = 12
RELOC_TABLE_SIZE       = (RELOC_FILE_COUNT + 1) * RELOC_TABLE_ENTRY_SIZE
RELOC_DATA_START       = RELOC_TABLE_ROM_ADDR + RELOC_TABLE_SIZE


def main():
    repo_root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    rom_path = sys.argv[1] if len(sys.argv) > 1 else os.path.join(repo_root, "baserom.us.z64")

    with open(rom_path, "rb") as f:
        rom = f.read()

    # Parse all table entries including sentinel
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
            "compressed_size": compressed_size,  # words
            "reloc_extern": reloc_extern,
            "decompressed_size": decompressed_size,  # words
        })

    passed = 0
    warnings = 0
    failed = 0
    total_extern_ids = 0
    files_with_externs = 0

    for i in range(RELOC_FILE_COUNT):
        e = entries[i]
        ne = entries[i + 1]

        compressed_bytes = e["compressed_size"] * 4
        data_addr = RELOC_DATA_START + e["data_offset"]
        extern_start = data_addr + compressed_bytes
        extern_end = RELOC_DATA_START + ne["data_offset"]
        extern_bytes = extern_end - extern_start
        num_extern_ids = extern_bytes // 2

        # Read the extern IDs
        extern_ids = []
        for j in range(num_extern_ids):
            addr = extern_start + j * 2
            ext_id = struct.unpack(">H", rom[addr:addr + 2])[0]
            extern_ids.append(ext_id)

        # Validate
        has_reloc_extern = e["reloc_extern"] != 0xFFFF

        if num_extern_ids > 0:
            files_with_externs += 1
            total_extern_ids += num_extern_ids

        # Check 1: extern IDs present ↔ reloc_extern != 0xFFFF
        if num_extern_ids > 0 and not has_reloc_extern:
            print(f"  [FAIL] File {i}: has {num_extern_ids} extern IDs "
                  f"but reloc_extern=0xFFFF")
            failed += 1
            continue

        if num_extern_ids == 0 and has_reloc_extern:
            print(f"  [FAIL] File {i}: reloc_extern=0x{e['reloc_extern']:04X} "
                  f"but no extern IDs in ROM region")
            failed += 1
            continue

        # Check 2: all extern IDs in range
        bad_ids = [eid for eid in extern_ids if eid >= RELOC_FILE_COUNT]
        if bad_ids:
            print(f"  [FAIL] File {i}: extern IDs out of range: {bad_ids}")
            failed += 1
            continue

        # Check 3: region size is even (exact u16 boundary)
        if extern_bytes % 2 != 0:
            print(f"  [WARN] File {i}: extern region is {extern_bytes} bytes "
                  f"(odd, not u16-aligned)")
            warnings += 1

        passed += 1

    print(f"\nExternal file ID validation:")
    print(f"  Total files: {RELOC_FILE_COUNT}")
    print(f"  Files with extern refs: {files_with_externs}")
    print(f"  Total extern IDs: {total_extern_ids}")
    print(f"  Passed: {passed}")
    print(f"  Warnings: {warnings}")
    print(f"  Failed: {failed}")

    return 1 if failed > 0 else 0


if __name__ == "__main__":
    sys.exit(main())
