#!/usr/bin/env python3
"""
verify_struct_byteswap.py — Read raw reloc file data from the .o2r archive
and verify struct field layouts against the blanket u32 byte-swap.

Shows exactly what each field looks like in:
  1. Original big-endian (as stored in archive)
  2. After blanket BSWAP32 (current broken state)
  3. After proposed fixup (what we want)

Usage:
    python tools/verify_struct_byteswap.py
"""

import struct
import zipfile
import sys
import os

O2R_PATH = os.path.join(os.path.dirname(__file__), "..", "build", "Debug", "ssb64.o2r")

# ── OTR/O2R resource header ──────────────────────────────────────────
# LUS binary resource format: 12-byte header then resource-specific data
# endianness = u8, type = u32, version = u32, id = u64, ...
# We skip past the LUS header to get to our custom fields.

def read_reloc_resource(zf, entry_name):
    """Read a RelocFile from the archive, returning the decompressed data blob."""
    with zf.open(entry_name) as f:
        raw = f.read()

    # LUS binary resource format (observed from actual data):
    #   0x00-0x3F: Fixed 64-byte LUS ResourceInitData header
    #     0x00: u32 endianness (0 = LE)
    #     0x04: u32 resourceType (0x52454C4F = "RELO")
    #     0x08: u32 resourceVersion
    #     0x0C: u64 id
    #     0x14-0x3F: padding/reserved
    #   0x40+: Our RelocFile custom fields (all little-endian):
    #     u32 file_id
    #     u16 reloc_intern_offset
    #     u16 reloc_extern_offset
    #     u32 num_extern_ids
    #     u16[num_extern_ids] extern_file_ids
    #     u32 decompressed_data_size
    #     u8[decompressed_data_size] data (big-endian ROM data)

    LUS_HEADER_SIZE = 0x40
    off = LUS_HEADER_SIZE

    file_id = struct.unpack_from("<I", raw, off)[0]; off += 4
    reloc_intern = struct.unpack_from("<H", raw, off)[0]; off += 2
    reloc_extern = struct.unpack_from("<H", raw, off)[0]; off += 2
    num_extern = struct.unpack_from("<I", raw, off)[0]; off += 4
    extern_ids = []
    for i in range(num_extern):
        extern_ids.append(struct.unpack_from("<H", raw, off)[0]); off += 2
    data_size = struct.unpack_from("<I", raw, off)[0]; off += 4
    data = raw[off:off+data_size]

    return {
        "file_id": file_id,
        "reloc_intern": reloc_intern,
        "reloc_extern": reloc_extern,
        "extern_ids": extern_ids,
        "data_size": data_size,
        "data": data,  # big-endian, as extracted from ROM
    }


def bswap32(val):
    """Simulate C BSWAP32: reverse 4 bytes of a u32."""
    return (((val >> 24) & 0xFF) |
            (((val >> 16) & 0xFF) << 8) |
            (((val >> 8) & 0xFF) << 16) |
            ((val & 0xFF) << 24))

def rotate16_word(val):
    return ((val << 16) | (val >> 16)) & 0xFFFFFFFF

def read_be_u32(data, offset):
    return struct.unpack_from(">I", data, offset)[0]

def read_be_s16(data, offset):
    return struct.unpack_from(">h", data, offset)[0]

def read_be_u16(data, offset):
    return struct.unpack_from(">H", data, offset)[0]

def read_be_u8(data, offset):
    return data[offset]

def read_be_f32(data, offset):
    return struct.unpack_from(">f", data, offset)[0]

def sim_u32swap_read_u16_pair(data, offset):
    """Simulate what happens to a u16 pair after the C code's blanket u32 swap.
    Returns (lo_u16, hi_u16) as they'd be read on little-endian after the swap."""
    raw_le = struct.unpack_from("<I", data, offset)[0]  # C reads raw bytes as LE
    swapped = bswap32(raw_le)                            # C does BSWAP32
    result = struct.pack("<I", swapped)                  # C writes back as LE
    lo = struct.unpack_from("<h", result, 0)[0]          # read s16 at +0
    hi = struct.unpack_from("<h", result, 2)[0]          # read s16 at +2
    return lo, hi

def sim_u32swap_read_u8_quad(data, offset):
    """Simulate what happens to 4 u8 bytes after blanket u32 swap."""
    raw_le = struct.unpack_from("<I", data, offset)[0]
    swapped = bswap32(raw_le)
    result = struct.pack("<I", swapped)
    return result[0], result[1], result[2], result[3]


def analyze_sprite(data, offset, label="Sprite"):
    """Parse a Sprite struct from big-endian data and show all field states."""
    print(f"\n{'='*70}")
    print(f"  {label} at blob offset 0x{offset:04X}")
    print(f"{'='*70}")
    print(f"  {'Field':<20} {'BE (correct)':<16} {'After u32swap':<16} {'Fixup'}")
    print(f"  {'-'*20} {'-'*16} {'-'*16} {'-'*10}")

    d = data; o = offset
    S = sim_u32swap_read_u16_pair
    U = sim_u32swap_read_u8_quad

    # Word 0: s16 x, s16 y
    x, y = read_be_s16(d,o), read_be_s16(d,o+2)
    ax, ay = S(d, o)
    print(f"  {'x':<20} {x:<16} {ax:<16} rotate16")
    print(f"  {'y':<20} {y:<16} {ay:<16} rotate16")

    # Word 1: s16 width, s16 height
    width, height = read_be_s16(d,o+4), read_be_s16(d,o+6)
    aw, ah = S(d, o+4)
    print(f"  {'width':<20} {width:<16} {aw:<16} rotate16")
    print(f"  {'height':<20} {height:<16} {ah:<16} rotate16")

    # Words 2-3: f32 scalex, scaley
    print(f"  {'scalex':<20} {read_be_f32(d,o+8):<16.4f} {'(ok)':<16} -")
    print(f"  {'scaley':<20} {read_be_f32(d,o+12):<16.4f} {'(ok)':<16} -")

    # Word 4: s16 expx, expy
    ex, ey = read_be_s16(d,o+16), read_be_s16(d,o+18)
    aex, aey = S(d, o+16)
    print(f"  {'expx':<20} {ex:<16} {aex:<16} rotate16")
    print(f"  {'expy':<20} {ey:<16} {aey:<16} rotate16")

    # Word 5: u16 attr, s16 zdepth
    attr, zdepth = read_be_u16(d,o+20), read_be_s16(d,o+22)
    aa, az = S(d, o+20)
    print(f"  {'attr':<20} 0x{attr:04X}{'':<10} 0x{aa&0xFFFF:04X}{'':<10} rotate16")
    print(f"  {'zdepth':<20} {zdepth:<16} {az:<16} rotate16")

    # Word 6: u8 r,g,b,a
    r,g,b,a = read_be_u8(d,o+24), read_be_u8(d,o+25), read_be_u8(d,o+26), read_be_u8(d,o+27)
    ar,ag,ab,aa2 = U(d, o+24)
    print(f"  {'rgba':<20} ({r},{g},{b},{a}){'':<5} ({ar},{ag},{ab},{aa2}){'':<5} bswap32")

    # Word 7: s16 startTLUT, nTLUT
    st, nt = read_be_s16(d,o+28), read_be_s16(d,o+30)
    ast, ant = S(d, o+28)
    print(f"  {'startTLUT':<20} {st:<16} {ast:<16} rotate16")
    print(f"  {'nTLUT':<20} {nt:<16} {ant:<16} rotate16")

    # Word 8: u32 LUT
    print(f"  {'LUT (tok)':<20} 0x{read_be_u32(d,o+32):08X}{'':<6} {'(ok)':<16} -")

    # Word 9: s16 istart, istep
    ist, isp = read_be_s16(d,o+36), read_be_s16(d,o+38)
    aist, aisp = S(d, o+36)
    print(f"  {'istart':<20} {ist:<16} {aist:<16} rotate16")
    print(f"  {'istep':<20} {isp:<16} {aisp:<16} rotate16")

    # Word 10: s16 nbitmaps, ndisplist
    nbm, ndl = read_be_s16(d,o+40), read_be_s16(d,o+42)
    anbm, andl = S(d, o+40)
    print(f"  {'nbitmaps':<20} {nbm:<16} {anbm:<16} rotate16")
    print(f"  {'ndisplist':<20} {ndl:<16} {andl:<16} rotate16")

    # Word 11: s16 bmheight, bmHreal
    bmh, bmhr = read_be_s16(d,o+44), read_be_s16(d,o+46)
    abmh, abmhr = S(d, o+44)
    print(f"  {'bmheight':<20} {bmh:<16} {abmh:<16} rotate16")
    print(f"  {'bmHreal':<20} {bmhr:<16} {abmhr:<16} rotate16")

    # Word 12: u8 bmfmt, bmsiz, pad, pad
    fmt,siz = read_be_u8(d,o+48), read_be_u8(d,o+49)
    afmt, asiz, _, _ = U(d, o+48)
    print(f"  {'bmfmt':<20} {fmt:<16} {afmt:<16} bswap32")
    print(f"  {'bmsiz':<20} {siz:<16} {asiz:<16} bswap32")

    # Words 13-15: u32 tokens
    print(f"  {'bitmap (tok)':<20} 0x{read_be_u32(d,o+52):08X}{'':<6} {'(ok)':<16} -")
    print(f"  {'rsp_dl (tok)':<20} 0x{read_be_u32(d,o+56):08X}{'':<6} {'(ok)':<16} -")
    print(f"  {'rsp_dl_next':<20} 0x{read_be_u32(d,o+60):08X}{'':<6} {'(ok)':<16} -")

    # Word 16: s16 frac_s, frac_t
    fs, ft = read_be_s16(d,o+64), read_be_s16(d,o+66)
    afs, aft = S(d, o+64)
    print(f"  {'frac_s':<20} {fs:<16} {afs:<16} rotate16")
    print(f"  {'frac_t':<20} {ft:<16} {aft:<16} rotate16")

    print(f"\n  Fixup summary (17 words):")
    print(f"    rotate16: words 0,1,4,5,7,9,10,11,16  (9 words)")
    print(f"    bswap32:  words 6,12                   (2 words — u8 data)")
    print(f"    skip:     words 2,3,8,13,14,15         (6 words — f32/u32)")

    return {"width": width, "height": height, "nbitmaps": nbm,
            "bmfmt": fmt, "bmsiz": siz, "bmheight": bmh}


def analyze_bitmap(data, offset, label="Bitmap"):
    """Parse a Bitmap struct from big-endian data."""
    print(f"\n  {label} at blob offset 0x{offset:04X}")

    d = data; o = offset
    S = sim_u32swap_read_u16_pair

    w, wi = read_be_s16(d,o), read_be_s16(d,o+2)
    aw, awi = S(d, o)
    s, t = read_be_s16(d,o+4), read_be_s16(d,o+6)
    as_, at = S(d, o+4)
    buf = read_be_u32(d, o+8)
    ah_, alut = read_be_s16(d,o+12), read_be_s16(d,o+14)
    aah, aalut = S(d, o+12)

    print(f"    width={w} width_img={wi} s={s} t={t} buf=0x{buf:08X} actualH={ah_} LUTofs={alut}")
    print(f"    After u32swap: width={aw} wimg={awi} s={as_} t={at} actualH={aah} LUTofs={aalut}")
    print(f"    Fixup: rotate16 words 0,1,3  (u16 pairs); word 2 = u32 ok")


def analyze_mobjsub(data, offset, label="MObjSub"):
    """Parse first few fields of MObjSub from big-endian data."""
    print(f"\n  {label} at blob offset 0x{offset:04X}")
    print(f"  {'-'*60}")

    d = data
    o = offset

    # Word 0: u16 pad00, u8 fmt, u8 siz
    pad00 = read_be_u16(d, o+0)
    fmt = read_be_u8(d, o+2)
    siz = read_be_u8(d, o+3)
    print(f"    {'pad00':<16} 0x{pad00:04X}     {'fmt':<8} {fmt}  {'siz':<8} {siz}")
    print(f"    Word 0 needs bswap32_undo (mixed u16+u8)")

    # Word 1: u32 sprites (token)
    sprites = read_be_u32(d, o+4)
    print(f"    {'sprites (tok)':<16} 0x{sprites:08X} — u32 ok")

    # Words 2-3: u16 unk08/unk0A, u16 unk0C/unk0E
    unk08 = read_be_u16(d, o+8)
    unk0A = read_be_u16(d, o+10)
    unk0C = read_be_u16(d, o+12)
    unk0E = read_be_u16(d, o+14)
    print(f"    {'unk08':<8} {unk08:<6} {'unk0A':<8} {unk0A:<6} {'unk0C':<8} {unk0C:<6} {'unk0E':<8} {unk0E}")
    print(f"    Words 2-3 need rotate16 (u16 pairs)")

    # Word 4: s32 unk10
    unk10 = struct.unpack_from(">i", d, o+16)[0]
    print(f"    {'unk10':<16} {unk10} — s32 ok")

    # Words 5-10: f32 trau, trav, scau, scav, unk24, unk28
    trau = read_be_f32(d, o+20)
    trav = read_be_f32(d, o+24)
    scau = read_be_f32(d, o+28)
    scav = read_be_f32(d, o+32)
    print(f"    trau={trau:.2f} trav={trav:.2f} scau={scau:.2f} scav={scav:.2f} — f32 ok")

    # Word 11: u32 palettes (token) at offset 0x2C
    palettes = read_be_u32(d, o+0x2C)
    print(f"    {'palettes (tok)':<16} 0x{palettes:08X} — u32 ok")

    # Word 12: u16 flags, u8 block_fmt, u8 block_siz at offset 0x30
    flags = read_be_u16(d, o+0x30)
    block_fmt = read_be_u8(d, o+0x32)
    block_siz = read_be_u8(d, o+0x33)
    print(f"    {'flags':<8} 0x{flags:04X}   {'block_fmt':<12} {block_fmt}  {'block_siz':<12} {block_siz}")
    print(f"    Word 12 needs special handling (u16 + 2xu8)")


def main():
    if not os.path.exists(O2R_PATH):
        print(f"ERROR: {O2R_PATH} not found. Build the project first.")
        sys.exit(1)

    print("=" * 70)
    print("  Struct Byte-Swap Verification Tool")
    print("  Reading actual data from ssb64.o2r archive")
    print("=" * 70)

    zf = zipfile.ZipFile(O2R_PATH, "r")

    # List all entries to find our test files
    entries = zf.namelist()

    # ── Test 1: N64 Logo (Sprite + Bitmaps) ──
    n64_entry = "reloc_misc_named/N64Logo"
    if n64_entry not in entries:
        print(f"\nWARN: '{n64_entry}' not in archive. Available reloc entries:")
        for e in sorted(entries):
            if "reloc_" in e:
                print(f"  {e}")
        return

    print(f"\nLoading: {n64_entry}")
    res = read_reloc_resource(zf, n64_entry)
    print(f"  file_id: {res['file_id']} (0x{res['file_id']:X})")
    print(f"  data size: {res['data_size']} bytes (0x{res['data_size']:X})")
    print(f"  reloc_intern: 0x{res['reloc_intern']:04X}")
    print(f"  reloc_extern: 0x{res['reloc_extern']:04X}")
    print(f"  extern deps: {res['extern_ids']}")

    sprite_offset = 0x73C0
    if sprite_offset + 68 > res["data_size"]:
        print(f"  ERROR: Sprite offset 0x{sprite_offset:X} beyond data size 0x{res['data_size']:X}")
        return

    sprite_info = analyze_sprite(res["data"], sprite_offset, "N64Logo Sprite")

    nbm = sprite_info["nbitmaps"]
    print(f"\n  Sprite has {nbm} bitmaps")

    # The bitmap field is a reloc descriptor: upper 16 = next_reloc, lower 16 = word offset
    bitmap_raw = read_be_u32(res["data"], sprite_offset + 52)
    target_words = bitmap_raw & 0xFFFF
    bm_offset = target_words * 4
    print(f"  Bitmap reloc raw: 0x{bitmap_raw:08X} -> word offset 0x{target_words:04X} -> byte offset 0x{bm_offset:04X}")

    if bm_offset + nbm * 16 <= res["data_size"]:
        for i in range(min(nbm, 4)):
            analyze_bitmap(res["data"], bm_offset + i * 16, f"Bitmap[{i}]")
        if nbm > 4:
            print(f"  ... ({nbm - 4} more bitmaps)")

    # ── Test 2: MNCommon (has MObjSub data) ──
    mn_entry = "reloc_menus/MNCommon"
    if mn_entry in entries:
        print(f"\n\n{'='*70}")
        print(f"  Loading: {mn_entry}")
        res2 = read_reloc_resource(zf, mn_entry)
        print(f"  file_id: {res2['file_id']} data size: 0x{res2['data_size']:X}")

        # Look for MObjSub-like data by scanning for known patterns
        # MObjSub has fmt/siz bytes that should be valid GBI format values (0-4)
        # and sprite/palette tokens that would be reloc offsets
        # For now just dump the first 0x78 bytes at a few offsets
        print(f"\n  Raw hex dump of first 128 bytes:")
        for row in range(8):
            off = row * 16
            hexes = " ".join(f"{res2['data'][off+i]:02X}" for i in range(16))
            print(f"    0x{off:04X}: {hexes}")

    zf.close()
    print(f"\n{'='*70}")
    print("  Done. Review the above to verify struct layouts match expectations.")
    print("=" * 70)


if __name__ == "__main__":
    main()
