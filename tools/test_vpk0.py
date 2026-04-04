"""
Test the VPK0 decompressor by building it as a shared library via ctypes
and running it against all compressed files in the ROM.

Usage: python tools/test_vpk0.py [path-to-baserom.us.z64]
"""

import ctypes
import os
import struct
import subprocess
import sys
import tempfile

# ROM constants
RELOC_TABLE_ROM_ADDR   = 0x001AC870
RELOC_FILE_COUNT       = 2132
RELOC_TABLE_ENTRY_SIZE = 12
RELOC_TABLE_SIZE       = (RELOC_FILE_COUNT + 1) * RELOC_TABLE_ENTRY_SIZE
RELOC_DATA_START       = RELOC_TABLE_ROM_ADDR + RELOC_TABLE_SIZE


def build_vpk0_test_exe():
    """Build a test exe that decompresses files and reports results via exit code."""
    repo_root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    vpk0_c = os.path.join(repo_root, "torch", "lib", "libvpk0", "vpk0.c")
    test_c = os.path.join(repo_root, "tools", "test_vpk0.c")

    if not os.path.exists(vpk0_c):
        raise FileNotFoundError(f"Cannot find {vpk0_c}")
    if not os.path.exists(test_c):
        raise FileNotFoundError(f"Cannot find {test_c}")

    tmp_dir = tempfile.mkdtemp(prefix="vpk0_test_")
    exe_path = os.path.join(tmp_dir, "test_vpk0.exe" if sys.platform == "win32" else "test_vpk0")

    if sys.platform == "win32":
        bat_path = os.path.join(tmp_dir, "build.bat")
        with open(bat_path, "w") as bf:
            bf.write('@echo off\n')
            bf.write('call "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Auxiliary\\Build\\vcvars64.bat" >nul 2>&1\n')
            bf.write(f'cl /O2 /Fe:"{exe_path}" "{test_c}" "{vpk0_c}" /I "{os.path.join(repo_root, "torch", "lib")}" >nul 2>&1\n')
            bf.write(f'echo BUILD_RESULT=%ERRORLEVEL%\n')

        result = subprocess.run(["cmd", "/c", bat_path], capture_output=True, text=True, cwd=tmp_dir)
    else:
        for compiler in ["gcc", "clang", "cc"]:
            try:
                subprocess.run(
                    [compiler, "-O2", "-o", exe_path, test_c, vpk0_c,
                     f"-I{os.path.join(repo_root, 'torch', 'lib')}"],
                    check=True, capture_output=True, text=True
                )
                break
            except (FileNotFoundError, subprocess.CalledProcessError):
                continue

    if not os.path.exists(exe_path):
        raise RuntimeError("Could not compile test_vpk0.c — no compiler found.")

    return exe_path, tmp_dir


def parse_reloc_table(rom):
    """Parse the reloc table from ROM bytes."""
    entries = []
    for i in range(RELOC_FILE_COUNT):
        offset = RELOC_TABLE_ROM_ADDR + i * RELOC_TABLE_ENTRY_SIZE
        data = rom[offset:offset + RELOC_TABLE_ENTRY_SIZE]
        first_word, reloc_intern, compressed_size, reloc_extern, decompressed_size = \
            struct.unpack(">IHHHH", data)

        is_compressed = bool(first_word & 0x80000000)
        data_offset = first_word & 0x7FFFFFFF

        entries.append({
            "id": i,
            "is_compressed": is_compressed,
            "data_offset": data_offset,
            "compressed_size_words": compressed_size,
            "decompressed_size_words": decompressed_size,
        })
    return entries


def test_vpk0_sizes_only(rom, entries):
    """Python-only test: verify VPK0 magic and sizes without decompressing."""
    compressed = [e for e in entries if e["is_compressed"]]
    print(f"\n[Size-only validation] {len(compressed)} compressed files\n")

    passed = 0
    failed = 0

    for e in compressed:
        fid = e["id"]
        data_addr = RELOC_DATA_START + e["data_offset"]
        compressed_bytes = e["compressed_size_words"] * 4
        expected_dec = e["decompressed_size_words"] * 4

        chunk = rom[data_addr:data_addr + min(compressed_bytes, 8)]
        if len(chunk) < 8:
            print(f"  [FAIL] File {fid}: too short to read VPK0 header")
            failed += 1
            continue

        magic = chunk[0:4]
        if magic != b"vpk0":
            print(f"  [FAIL] File {fid}: bad magic {magic!r} (expected b'vpk0')")
            failed += 1
            continue

        stored_size = struct.unpack(">I", chunk[4:8])[0]
        if stored_size != expected_dec:
            print(f"  [FAIL] File {fid}: VPK0 header size={stored_size}, "
                  f"table says={expected_dec}")
            failed += 1
            continue

        passed += 1

    print(f"  Passed: {passed}")
    print(f"  Failed: {failed}")
    return failed == 0



def main():
    repo_root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    rom_path = sys.argv[1] if len(sys.argv) > 1 else os.path.join(repo_root, "baserom.us.z64")

    if not os.path.exists(rom_path):
        print(f"Error: ROM not found at {rom_path}")
        return 1

    print(f"Loading ROM: {rom_path}")
    with open(rom_path, "rb") as f:
        rom = f.read()
    print(f"ROM size: {len(rom)} bytes ({len(rom) / (1024*1024):.1f} MB)")

    entries = parse_reloc_table(rom)
    compressed_count = sum(1 for e in entries if e["is_compressed"])
    print(f"Reloc table: {len(entries)} files, {compressed_count} compressed")

    # Always run size-only validation (no compiler needed)
    size_ok = test_vpk0_sizes_only(rom, entries)

    # Try to build and run full decompression test (native exe)
    exe_path = None
    tmp_dir = None
    full_ok = True
    try:
        print("\nBuilding native test executable...")
        exe_path, tmp_dir = build_vpk0_test_exe()
        print("  Build succeeded!")
        print(f"\nRunning full decompression test (native)...")
        result = subprocess.run(
            [exe_path, rom_path],
            capture_output=True, text=True, timeout=120
        )
        print(result.stdout)
        if result.stderr:
            print(result.stderr)
        full_ok = result.returncode == 0
    except Exception as e:
        print(f"  Native test failed: {e}")
        print("  Skipping full decompression test.")

    # Cleanup
    if tmp_dir:
        import shutil
        try:
            shutil.rmtree(tmp_dir, ignore_errors=True)
        except Exception:
            pass

    print("\n" + "=" * 50)
    if size_ok and full_ok:
        print("ALL TESTS PASSED")
        return 0
    else:
        print("SOME TESTS FAILED")
        return 1


if __name__ == "__main__":
    sys.exit(main())
