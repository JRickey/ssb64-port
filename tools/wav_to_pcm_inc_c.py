#!/usr/bin/env python3
"""
wav_to_pcm_inc_c.py — convert a WAV file to 32 kHz interleaved-stereo S16
PCM, emitted as int16 literals in a .pcm.inc.c file.

The PC port's audio output stream feeds libultraship via
AudioPlayerPlayFrame(buf, len) at 32 kHz, interleaved stereo S16
(see port/audio/audio_playback.cpp). PokemonEngine's voice mixer hooks
that function and mixes registered PCM blobs into the buffer; the format
must match exactly or playback comes out the wrong rate / aliased.

Input: any WAV that scipy.io.wavfile can read (PCM only — compressed
WAVs unsupported). Resampled to 32000 Hz with scipy.signal.resample_poly,
mono inputs duplicated to both channels, S16 saturating-converted.

Output: a .pcm.inc.c snippet meant to be #included between
`static const int16_t kFooPCM[] = {` and `};`. Eight values per line.

Usage:
    wav_to_pcm_inc_c.py <input.wav> <output.pcm.inc.c>
"""

from __future__ import annotations

import argparse
import sys
from pathlib import Path

import numpy as np
from scipy.io import wavfile
from scipy.signal import resample_poly


TARGET_RATE = 32000


def load_wav_as_float_stereo(wav_path: Path) -> tuple[np.ndarray, int]:
    """Read a WAV, return (float32 stereo samples in [-1, 1], src_rate)."""
    src_rate, data = wavfile.read(str(wav_path))

    if data.dtype == np.int16:
        norm = data.astype(np.float32) / 32768.0
    elif data.dtype == np.int32:
        norm = data.astype(np.float32) / 2147483648.0
    elif data.dtype == np.uint8:
        norm = (data.astype(np.float32) - 128.0) / 128.0
    elif data.dtype == np.float32:
        norm = data
    else:
        raise SystemExit(f"unsupported WAV sample dtype: {data.dtype}")

    if norm.ndim == 1:
        norm = np.stack([norm, norm], axis=1)
    elif norm.shape[1] == 1:
        norm = np.repeat(norm, 2, axis=1)
    elif norm.shape[1] >= 2:
        norm = norm[:, :2]

    return norm, src_rate


def resample_to_target(samples: np.ndarray, src_rate: int) -> np.ndarray:
    """Resample float32 stereo to TARGET_RATE via polyphase filter."""
    if src_rate == TARGET_RATE:
        return samples
    g = np.gcd(src_rate, TARGET_RATE)
    up = TARGET_RATE // g
    down = src_rate // g
    return resample_poly(samples, up, down, axis=0).astype(np.float32)


def to_s16(samples: np.ndarray) -> np.ndarray:
    """Saturating float32 [-1,1] -> int16 [-32768, 32767]."""
    clipped = np.clip(samples, -1.0, 1.0)
    return (clipped * 32767.0).astype(np.int16)


def write_inc_c(s16_stereo: np.ndarray, output_path: Path,
                src_rate: int, frame_count: int) -> None:
    """Emit interleaved S16 values, 8 per line, signed decimal literals."""
    flat = s16_stereo.reshape(-1)
    output_path.parent.mkdir(parents=True, exist_ok=True)
    with open(output_path, "w", encoding="utf-8") as f:
        f.write(f"/* Auto-generated from WAV via wav_to_pcm_inc_c.py — do not edit. */\n")
        f.write(f"/* Format: 32 kHz stereo S16 PCM, {frame_count} frames "
                f"({frame_count / TARGET_RATE:.3f}s, resampled from {src_rate} Hz) */\n")
        for i in range(0, len(flat), 8):
            chunk = flat[i:i + 8]
            f.write("    " + ", ".join(f"{int(v):6d}" for v in chunk) + ",\n")


def convert_one(wav_path: Path, out_path: Path) -> None:
    floats, src_rate = load_wav_as_float_stereo(wav_path)
    resampled = resample_to_target(floats, src_rate)
    s16 = to_s16(resampled)
    frame_count = s16.shape[0]
    write_inc_c(s16, out_path, src_rate, frame_count)
    print(f"  {wav_path.name}  ({src_rate} Hz, {floats.shape[0]} frames) "
          f"-> {out_path.name}  ({TARGET_RATE} Hz, {frame_count} frames, "
          f"{frame_count * 4} bytes)")


def main(argv: list[str]) -> int:
    p = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("input", help="input WAV file")
    p.add_argument("output", help="output .pcm.inc.c path")
    args = p.parse_args(argv)

    wav = Path(args.input)
    out = Path(args.output)
    if not wav.is_file():
        print(f"error: {wav} is not a file", file=sys.stderr)
        return 1
    convert_one(wav, out)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
