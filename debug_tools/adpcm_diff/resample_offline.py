#!/usr/bin/env python3
"""
resample_offline.py — Reference 4-tap resampler for SSB64 port debugging.

Reads /tmp/resample_dump.bin (captured by aResampleImpl with
SSB64_RESAMPLE_DUMP=1) and decodes it independently using a from-spec
implementation matching the N64 RSP resampler.

Compares to /tmp/resample_dump_output.bin (the port's actual output).

Dump format:
    [0..7]   magic "RSAMPDP1"
    [8..11]  flags    (u32 LE)
    [12..15] pitch    (u32 LE; only low 16 bits used)
    [16..19] nbytes   (s32 LE) — output bytes to produce (rounded up to 16)
    [20..21] in_off   (u16 LE) — DMEM addr for input
    [22..23] out_off  (u16 LE) — DMEM addr for output
    [24..33] reserved (10 bytes)
    [34..65] prior state = 16 s16 LE  (32 bytes)
    [66..)   full DMEM (4096 bytes)

Output dump format:
    [0..3]   n_samples (s32 LE)
    [4..)    n_samples × s16 LE (output samples)
    [..]     final state = 16 s16 LE
"""
import os
import struct
import sys


# Resample table copied verbatim from port/audio/mixer.c (64×4 s16, hex).
RESAMPLE_TABLE = [
    [0x0c39, 0x66ad, 0x0d46, 0xffdf], [0x0b39, 0x6696, 0x0e5f, 0xffd8],
    [0x0a44, 0x6669, 0x0f83, 0xffd0], [0x095a, 0x6626, 0x10b4, 0xffc8],
    [0x087d, 0x65cd, 0x11f0, 0xffbf], [0x07ab, 0x655e, 0x1338, 0xffb6],
    [0x06e4, 0x64d9, 0x148c, 0xffac], [0x0628, 0x643f, 0x15eb, 0xffa1],
    [0x0577, 0x638f, 0x1756, 0xff96], [0x04d1, 0x62cb, 0x18cb, 0xff8a],
    [0x0435, 0x61f3, 0x1a4c, 0xff7e], [0x03a4, 0x6106, 0x1bd7, 0xff71],
    [0x031c, 0x6007, 0x1d6c, 0xff64], [0x029f, 0x5ef5, 0x1f0b, 0xff56],
    [0x022a, 0x5dd0, 0x20b3, 0xff48], [0x01be, 0x5c9a, 0x2264, 0xff3a],
    [0x015b, 0x5b53, 0x241e, 0xff2c], [0x0101, 0x59fc, 0x25e0, 0xff1e],
    [0x00ae, 0x5896, 0x27a9, 0xff10], [0x0063, 0x5720, 0x297a, 0xff02],
    [0x001f, 0x559d, 0x2b50, 0xfef4], [0xffe2, 0x540d, 0x2d2c, 0xfee8],
    [0xffac, 0x5270, 0x2f0d, 0xfedb], [0xff7c, 0x50c7, 0x30f3, 0xfed0],
    [0xff53, 0x4f14, 0x32dc, 0xfec6], [0xff2e, 0x4d57, 0x34c8, 0xfebd],
    [0xff0f, 0x4b91, 0x36b6, 0xfeb6], [0xfef5, 0x49c2, 0x38a5, 0xfeb0],
    [0xfedf, 0x47ed, 0x3a95, 0xfeac], [0xfece, 0x4611, 0x3c85, 0xfeab],
    [0xfec0, 0x4430, 0x3e74, 0xfeac], [0xfeb6, 0x424a, 0x4060, 0xfeaf],
    [0xfeaf, 0x4060, 0x424a, 0xfeb6], [0xfeac, 0x3e74, 0x4430, 0xfec0],
    [0xfeab, 0x3c85, 0x4611, 0xfece], [0xfeac, 0x3a95, 0x47ed, 0xfedf],
    [0xfeb0, 0x38a5, 0x49c2, 0xfef5], [0xfeb6, 0x36b6, 0x4b91, 0xff0f],
    [0xfebd, 0x34c8, 0x4d57, 0xff2e], [0xfec6, 0x32dc, 0x4f14, 0xff53],
    [0xfed0, 0x30f3, 0x50c7, 0xff7c], [0xfedb, 0x2f0d, 0x5270, 0xffac],
    [0xfee8, 0x2d2c, 0x540d, 0xffe2], [0xfef4, 0x2b50, 0x559d, 0x001f],
    [0xff02, 0x297a, 0x5720, 0x0063], [0xff10, 0x27a9, 0x5896, 0x00ae],
    [0xff1e, 0x25e0, 0x59fc, 0x0101], [0xff2c, 0x241e, 0x5b53, 0x015b],
    [0xff3a, 0x2264, 0x5c9a, 0x01be], [0xff48, 0x20b3, 0x5dd0, 0x022a],
    [0xff56, 0x1f0b, 0x5ef5, 0x029f], [0xff64, 0x1d6c, 0x6007, 0x031c],
    [0xff71, 0x1bd7, 0x6106, 0x03a4], [0xff7e, 0x1a4c, 0x61f3, 0x0435],
    [0xff8a, 0x18cb, 0x62cb, 0x04d1], [0xff96, 0x1756, 0x638f, 0x0577],
    [0xffa1, 0x15eb, 0x643f, 0x0628], [0xffac, 0x148c, 0x64d9, 0x06e4],
    [0xffb6, 0x1338, 0x655e, 0x07ab], [0xffbf, 0x11f0, 0x65cd, 0x087d],
    [0xffc8, 0x10b4, 0x6626, 0x095a], [0xffd0, 0x0f83, 0x6669, 0x0a44],
    [0xffd8, 0x0e5f, 0x6696, 0x0b39], [0xffdf, 0x0d46, 0x66ad, 0x0c39],
]
# Convert to signed 16-bit
def _s16(x):
    return x - 0x10000 if x >= 0x8000 else x
RESAMPLE_TABLE = [[_s16(c) for c in row] for row in RESAMPLE_TABLE]


def clamp16(v: int) -> int:
    if v < -0x8000: return -0x8000
    if v >  0x7fff: return  0x7fff
    return v


def resample(flags: int, pitch: int, nbytes: int, in_off: int, out_off: int,
             prior_state, dmem_bytes):
    """Reference resampler matching aResampleImpl semantics.

    Returns (output_samples, final_state, debug_info).
    DMEM is mutable here too because the impl does memcpy(in, tmp, 4*s16)
    overwriting samples in-place.
    """
    # Treat dmem as a mutable bytearray we can both read s16 from and modify.
    dmem = bytearray(dmem_bytes)

    def s16_at(byte_off):
        v = struct.unpack_from('<h', dmem, byte_off)[0]
        return v

    def write_s16_at(byte_off, v):
        struct.pack_into('<h', dmem, byte_off, v & 0xffff)

    in_byte_off  = in_off
    out_byte_off = out_off
    in_initial_byte = in_byte_off

    tmp = [0] * 16
    if flags & 0x01:    # A_INIT
        # only first 5 elements zeroed (rest left undefined, mirroring memset(tmp,0,5*s16))
        # In the C code, 'tmp' is uninitialized stack; we approximate as zeros.
        pass
    else:
        for i in range(16):
            tmp[i] = prior_state[i]

    if flags & 0x02:    # A_LOOP
        for k in range(8):
            write_s16_at(in_byte_off - 16 + k * 2, tmp[8 + k])
        in_byte_off -= tmp[5]   # tmp[5] is in bytes per C; not divided by sizeof
        # The C code does `in -= tmp[5] / sizeof(int16_t)` where `in` is int16_t* —
        # so /2 in BYTES it cancels out. We track byte offsets, so subtract tmp[5]
        # bytes (since /2 in element-space == subtract tmp[5] bytes).

    in_byte_off -= 8   # in -= 4 (4 s16 = 8 bytes)
    pitch_acc = tmp[4] & 0xffff
    # memcpy(in, tmp, 4 * sizeof(int16_t)) — overwrite 4 s16 at in
    for k in range(4):
        write_s16_at(in_byte_off + k * 2, tmp[k])

    out = []
    nbytes_remain = nbytes  # output bytes to produce
    while nbytes_remain > 0:
        for i in range(8):
            tbl = RESAMPLE_TABLE[(pitch_acc * 64) >> 16]
            in0 = s16_at(in_byte_off + 0)
            in1 = s16_at(in_byte_off + 2)
            in2 = s16_at(in_byte_off + 4)
            in3 = s16_at(in_byte_off + 6)
            sample = (
                ((in0 * tbl[0] + 0x4000) >> 15) +
                ((in1 * tbl[1] + 0x4000) >> 15) +
                ((in2 * tbl[2] + 0x4000) >> 15) +
                ((in3 * tbl[3] + 0x4000) >> 15)
            )
            out.append(clamp16(sample))

            pitch_acc = (pitch_acc + (pitch << 1)) & 0xffffffff
            in_byte_off += (pitch_acc >> 16) * 2   # *2 since byte stride for s16
            pitch_acc %= 0x10000
        nbytes_remain -= 8 * 2   # 8 samples × 2 bytes

    final_state = [0] * 16
    final_state[4] = pitch_acc & 0xffff
    if final_state[4] >= 0x8000:
        final_state[4] -= 0x10000
    for k in range(4):
        final_state[k] = s16_at(in_byte_off + k * 2)
    in_elem_off = (in_byte_off - in_initial_byte) // 2
    i_align = (in_elem_off + 4) & 7
    in_byte_off -= i_align * 2
    if i_align != 0:
        i_align = -8 - i_align
    final_state[5] = i_align
    if final_state[5] < 0:
        final_state[5] += 0x10000
        final_state[5] -= 0x10000
    for k in range(8):
        final_state[8 + k] = s16_at(in_byte_off + k * 2)

    return out, final_state, {"in_initial_byte": in_initial_byte,
                              "in_final_byte": in_byte_off}


def parse_dump(path):
    with open(path, "rb") as f:
        data = f.read()
    if data[:8] != b"RSAMPDP1":
        raise SystemExit(f"bad magic: {data[:8]!r}")
    flags  = struct.unpack_from("<I", data, 8)[0]
    pitch  = struct.unpack_from("<I", data, 12)[0] & 0xffff
    nbytes = struct.unpack_from("<i", data, 16)[0]
    in_off  = struct.unpack_from("<H", data, 20)[0]
    out_off = struct.unpack_from("<H", data, 22)[0]
    pos = 34
    prior_state = list(struct.unpack_from("<16h", data, pos)); pos += 32
    dmem = data[pos : pos + 4096]
    if len(dmem) < 4096:
        raise SystemExit(f"truncated dmem: {len(dmem)}")
    return {"flags": flags, "pitch": pitch, "nbytes": nbytes,
            "in_off": in_off, "out_off": out_off,
            "prior_state": prior_state, "dmem": dmem}


def main():
    dump_path = sys.argv[1] if len(sys.argv) > 1 else "/tmp/resample_dump.bin"
    output_path = sys.argv[2] if len(sys.argv) > 2 else "/tmp/resample_dump_output.bin"

    d = parse_dump(dump_path)
    print(f"== Dump {dump_path}")
    print(f"   flags=0x{d['flags']:02x}  pitch=0x{d['pitch']:04x} ({d['pitch']/0x8000:.4f}x)")
    print(f"   nbytes={d['nbytes']}  in=0x{d['in_off']:04x}  out=0x{d['out_off']:04x}")
    print(f"   prior_state={d['prior_state']}")
    # Print input samples around in_off
    in_samples = list(struct.unpack_from(f"<32h", d['dmem'], d['in_off']))
    print(f"   input samples [{d['in_off']:#06x}..+64]: {in_samples}")

    ref_out, ref_state, _ = resample(
        d["flags"], d["pitch"], d["nbytes"],
        d["in_off"], d["out_off"], d["prior_state"], d["dmem"])
    print(f"\n== Reference produced {len(ref_out)} samples")
    print(f"   first 16: {ref_out[:16]}")
    print(f"   final state: {ref_state}")

    if os.path.exists(output_path):
        with open(output_path, "rb") as f:
            head = f.read(4)
            n_samples = struct.unpack("<i", head)[0]
            port_out = list(struct.unpack(f"<{n_samples}h", f.read(n_samples * 2)))
            port_state = list(struct.unpack(f"<16h", f.read(32)))

        print(f"\n== Port output {output_path}: {n_samples} samples")
        print(f"   first 16: {port_out[:16]}")
        print(f"   final state: {port_state}")

        n = min(len(ref_out), len(port_out))
        diffs = [(i, ref_out[i], port_out[i]) for i in range(n) if ref_out[i] != port_out[i]]
        if not diffs:
            print(f"\n[OK] First {n} samples match exactly. Resampler is bit-correct.")
        else:
            print(f"\n[DIFF] {len(diffs)} divergences in first {n} samples. First 8:")
            for i, r, p in diffs[:8]:
                print(f"   sample {i:5d}: ref={r:+6d}  port={p:+6d}  delta={p-r:+6d}")
            print(f"\n   Side-by-side first 16 samples:")
            for i in range(min(16, n)):
                marker = "" if ref_out[i] == port_out[i] else "  ←"
                print(f"     [{i:3d}] ref={ref_out[i]:+6d}  port={port_out[i]:+6d}{marker}")

        if ref_state != port_state:
            print(f"\n[DIFF] final state mismatch:")
            for i, (r, p) in enumerate(zip(ref_state, port_state)):
                if r != p:
                    print(f"     state[{i}]: ref={r}  port={p}")
    else:
        print(f"\n[no port output file at {output_path}; running with dump only]")


if __name__ == "__main__":
    main()
