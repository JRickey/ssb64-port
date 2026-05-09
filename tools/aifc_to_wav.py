#!/usr/bin/env python3
"""
aifc_to_wav.py — Decode N64 VADPCM .aifc files to plain S16 PCM .wav.

The N64 SDK's AIFC variant uses Sony VADPCM compression (codec tag
'VAPC') stored in IFF/AIFC framing with the predictor codebook in an
APPL/stoc/VADPCMCODES chunk and audio samples in SSND. ffmpeg cannot
decode this codec; this tool implements the bruteforcing decoder
algorithm matching torch/src/factories/naudio/v0/AIFCDecode.cpp
(itself derived from the standard sm64 / nlibgcn aifc_decode tool).

Output is mono S16 LE PCM at the source's sample rate, written as a
canonical RIFF/WAV. To turn that into the project's interleaved-stereo
32 kHz S16 .pcm.inc.c expected by the voice mixer, pipe through
tools/wav_to_pcm_inc_c.py:

    python tools/aifc_to_wav.py input.aifc /tmp/out.wav
    python tools/wav_to_pcm_inc_c.py /tmp/out.wav out.pcm.inc.c

Usage:
    aifc_to_wav.py <input.aifc> <output.wav>
"""

from __future__ import annotations

import struct
import sys
import wave
from pathlib import Path


def parse_extended_float(b: bytes) -> float:
    """Parse an 80-bit IEEE extended-precision float (sample-rate field
    in AIFF COMM chunks). Big-endian. Returns Python float."""
    if len(b) != 10:
        raise ValueError("extended float must be 10 bytes")
    sign = (b[0] >> 7) & 1
    exponent = ((b[0] & 0x7f) << 8) | b[1]
    mantissa = int.from_bytes(b[2:10], 'big')
    if exponent == 0 and mantissa == 0:
        return 0.0
    if exponent == 0x7fff:
        return float('inf') if sign == 0 else float('-inf')
    bias = 16383
    value = mantissa * (2.0 ** (exponent - bias - 63))
    return -value if sign else value


def clamp_s16(x: int) -> int:
    if x < -0x8000:
        return -0x8000
    if x > 0x7fff:
        return 0x7fff
    return x


def inner_product(length: int, v1: list[int], v2: list[int]) -> int:
    """Mirror of AIFCDecode.cpp::inner_product — sum of products,
    divided by 2^11 with floor-rounding adjustment."""
    out = 0
    for i in range(length):
        out += v1[i] * v2[i]
    dout = out // (1 << 11) if out >= 0 else -((-out) // (1 << 11))
    fiout = dout * (1 << 11)
    if (out - fiout) < 0:
        dout -= 1
    return dout


def expand_codebook(raw_book: list[list[int]], order: int, npredictors: int) -> list[list[list[int]]]:
    """raw_book[predictor][order][8] of s16 → coefTable[predictor][8][order+8]."""
    table: list[list[list[int]]] = []
    for i in range(npredictors):
        # Initialize coefTable[i] as 8 rows of (order + 8) zeros.
        entry: list[list[int]] = [[0] * (order + 8) for _ in range(8)]
        # Fill from raw_book: raw_book[i][j][k] (s16) → entry[k][j].
        for j in range(order):
            for k in range(8):
                entry[k][j] = raw_book[i][j][k]
        # Set the order-th column from the diagonal of previous rows.
        for k in range(1, 8):
            entry[k][order] = entry[k - 1][order - 1]
        entry[0][order] = 1 << 11
        # Triangular extension into columns order+1 .. order+7.
        for k in range(1, 8):
            for j in range(0, k):
                entry[j][k + order] = 0
            for j in range(k, 8):
                entry[j][k + order] = entry[j - k][order]
        table.append(entry)
    return table


def decode_frame(frame: bytes, state: list[int], order: int, coef_table: list[list[list[int]]]) -> None:
    """Decode one 9-byte VADPCM frame into 16 samples, updating state."""
    header = frame[0]
    scale = 1 << (header >> 4)
    optimalp = header & 0xf

    ix = [0] * 16
    for i in range(0, 16, 2):
        c = frame[1 + i // 2]
        ix[i] = c >> 4
        ix[i + 1] = c & 0xf
    for i in range(16):
        if ix[i] >= 8:
            ix[i] -= 16
        ix[i] *= scale

    for j in range(2):
        in_vec = [0] * 16
        if j == 0:
            for i in range(order):
                in_vec[i] = state[16 - order + i]
        else:
            for i in range(order):
                in_vec[i] = state[8 - order + i]
        for i in range(8):
            ind = j * 8 + i
            in_vec[order + i] = ix[ind]
            state[ind] = inner_product(order + i, coef_table[optimalp][i], in_vec) + ix[ind]


def decode_aifc(path: Path) -> tuple[int, list[int]]:
    """Returns (sample_rate, [s16 samples])."""
    data = path.read_bytes()
    pos = 0

    if data[:4] != b'FORM':
        raise ValueError(f"not an IFF/AIFC file (bad FORM magic): {path}")
    form_size = struct.unpack('>I', data[4:8])[0]
    if data[8:12] != b'AIFC':
        raise ValueError(f"not AIFC form type: {data[8:12]!r}")

    pos = 12
    sample_rate = 0
    num_samples = 0
    coef_table: list[list[list[int]]] | None = None
    order = 0
    sound_pointer = -1

    while pos + 8 <= len(data):
        ck_id = data[pos:pos + 4]
        ck_size = struct.unpack('>I', data[pos + 4:pos + 8])[0]
        ck_size_aligned = (ck_size + 1) & ~1
        body_start = pos + 8

        if ck_id == b'COMM':
            num_channels = struct.unpack('>h', data[body_start:body_start + 2])[0]
            num_frames = struct.unpack('>I', data[body_start + 2:body_start + 6])[0]
            sample_size = struct.unpack('>h', data[body_start + 6:body_start + 8])[0]
            sample_rate = int(parse_extended_float(data[body_start + 8:body_start + 18]))
            ctype = data[body_start + 18:body_start + 22]
            if ctype != b'VAPC':
                raise ValueError(f"unsupported codec {ctype!r} (only VAPC / VADPCM supported)")
            if num_channels != 1:
                raise ValueError(f"{path}: {num_channels} channels (only mono supported)")
            if sample_size != 16:
                raise ValueError(f"{path}: {sample_size}-bit samples (only 16-bit supported)")
            num_samples = num_frames - (num_frames % 16)
        elif ck_id == b'APPL':
            ts = data[body_start:body_start + 4]
            if ts == b'stoc':
                name_len = data[body_start + 4]
                if name_len == 11:
                    chunk_name = bytes(data[body_start + 5:body_start + 5 + 11]).rstrip(b'\x00')
                    sub_pos = body_start + 5 + 11
                    if chunk_name == b'VADPCMCODES':
                        version = struct.unpack('>h', data[sub_pos:sub_pos + 2])[0]
                        if version != 1:
                            raise ValueError(f"unknown VADPCMCODES version {version}")
                        sub_pos += 2
                        order = struct.unpack('>h', data[sub_pos:sub_pos + 2])[0]
                        sub_pos += 2
                        npredictors = struct.unpack('>h', data[sub_pos:sub_pos + 2])[0]
                        sub_pos += 2
                        raw_book: list[list[list[int]]] = []
                        for _ in range(npredictors):
                            pred: list[list[int]] = []
                            for _j in range(order):
                                row = list(struct.unpack(
                                    '>8h', data[sub_pos:sub_pos + 16]))
                                pred.append(row)
                                sub_pos += 16
                            raw_book.append(pred)
                        coef_table = expand_codebook(raw_book, order, npredictors)
                    # VADPCMLOOPS chunks ignored — one-shot SFX don't loop.
        elif ck_id == b'SSND':
            offset = struct.unpack('>I', data[body_start:body_start + 4])[0]
            block_size = struct.unpack('>I', data[body_start + 4:body_start + 8])[0]
            assert offset == 0 and block_size == 0
            sound_pointer = body_start + 8

        pos = body_start + ck_size_aligned

    if coef_table is None:
        raise ValueError(f"{path}: codebook (VADPCMCODES) missing")
    if sound_pointer < 0:
        raise ValueError(f"{path}: SSND chunk missing")

    # Decode all frames.
    state = [0] * 16
    samples: list[int] = []
    cur_pos = 0
    sp = sound_pointer
    while cur_pos < num_samples:
        frame = data[sp:sp + 9]
        if len(frame) < 9:
            raise ValueError(f"{path}: truncated SSND data at sample {cur_pos}")
        decode_frame(frame, state, order, coef_table)
        for s in state:
            samples.append(clamp_s16(s))
        cur_pos += 16
        sp += 9

    return sample_rate, samples


def write_wav(path: Path, sample_rate: int, samples: list[int]) -> None:
    with wave.open(str(path), 'wb') as wf:
        wf.setnchannels(1)
        wf.setsampwidth(2)
        wf.setframerate(sample_rate)
        wf.writeframes(b''.join(struct.pack('<h', s) for s in samples))


def main(argv: list[str]) -> int:
    if len(argv) != 3:
        print("usage: aifc_to_wav.py <input.aifc> <output.wav>", file=sys.stderr)
        return 2
    in_path = Path(argv[1])
    out_path = Path(argv[2])
    sample_rate, samples = decode_aifc(in_path)
    write_wav(out_path, sample_rate, samples)
    print(f"{in_path} -> {out_path}: {len(samples)} samples @ {sample_rate} Hz "
          f"({len(samples) / sample_rate:.3f}s)")
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))
