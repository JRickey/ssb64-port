#!/usr/bin/env python3
"""
m64p_screenshot.py — Capture pixel-perfect Mupen64Plus screenshots of SSB64 at
specific game frames, for use as "ground truth" reference images in the
SSB64 PC port's rendering validation workflow.

Context
-------
The SSB64 PC port (libultraship-based) renders many scenes incorrectly compared
to the original N64. To debug rendering bugs we diff display list streams
(see debug_tools/gbi_diff, debug_tools/m64p_trace_plugin) — but sometimes we
need a pixel reference of what the scene SHOULD look like. Mupen64Plus with a
high-accuracy video plugin (glide64mk2 / GLideN64) is our reference renderer.

This script drives Mupen64Plus non-interactively via its `--testshots` CLI
option, which auto-captures screenshots at the listed frames and then quits.
After m64p exits we locate the produced PNGs in the platform-specific
screenshot directory (e.g. %APPDATA%/Mupen64Plus/screenshot/) and copy them
to the user's chosen output location with stable filenames.

Usage examples
--------------
    # Capture frame 30, write to default dir (debug_traces/m64p_screenshots/)
    python debug_tools/m64p_screenshot/m64p_screenshot.py --frames 30

    # Multiple frames to a custom directory
    python debug_tools/m64p_screenshot/m64p_screenshot.py \\
        --frames 10,30,55 --out-dir ref_screenshots/

    # Single frame to a specific file path
    python debug_tools/m64p_screenshot/m64p_screenshot.py \\
        --frames 80 --out ref/dog_frame.png

    # Override ROM, m64p binary, or gfx plugin
    python debug_tools/m64p_screenshot/m64p_screenshot.py \\
        --frames 80 --rom path/to/baserom.us.z64 \\
        --m64p-bin "C:/Program Files/Mupen64Plus/mupen64plus.exe" \\
        --gfx-plugin mupen64plus-video-glide64mk2.dll

Design notes for the agent operator
-----------------------------------
* Runs fully non-interactive: no prompts, no GUI clicks, subprocess has a
  timeout, and m64p is invoked with --testshots which auto-quits after the
  last listed frame is reached.
* Stdlib only — no Pillow, no Pygame. Mupen64Plus writes PNGs directly.
* If mupen64plus is not installed, the script prints the search paths it
  tried and exits with code 2 (a clean "setup error"), not a crash.
* --testshots writes filenames like "<rom-good-name>-<NNN>.png" where NNN
  is a sequential index per testshot (NOT the N64 frame number). We map
  captured files back to requested frames by sorting new files by mtime
  and zipping against the requested frames list (which is sorted ascending
  before being passed to m64p, since m64p reaches frames in order).

Exit codes
----------
    0  success — every requested frame produced an output file
    1  partial / runtime failure (m64p ran but fewer screenshots than asked,
       or m64p exited non-zero, or m64p hung past the timeout)
    2  setup / input error (ROM missing, m64p binary not findable, bad args)
"""

from __future__ import annotations

import argparse
import os
import shutil
import subprocess
import sys
import time
from pathlib import Path
from typing import Iterable, List, Optional, Sequence

# --------------------------------------------------------------------------
# Constants
# --------------------------------------------------------------------------

# Repo-root-relative default for the ROM. CLAUDE.md documents this filename
# and the expected SHA1 (e2929e10fccc0aa84e5776227e798abc07cedabf).
DEFAULT_ROM_RELPATH = "baserom.us.z64"

# Where to dump screenshots by default (mirrors debug_traces/ convention).
DEFAULT_OUT_DIR_RELPATH = "debug_traces/m64p_screenshots"

# Binary name to look up on PATH.
M64P_BINARY_NAMES = ("mupen64plus.exe", "mupen64plus")

# Common Windows install locations, expanded at runtime.
WINDOWS_INSTALL_CANDIDATES = (
    r"C:\Program Files\Mupen64Plus\mupen64plus.exe",
    r"C:\Program Files (x86)\Mupen64Plus\mupen64plus.exe",
    r"%LOCALAPPDATA%\Mupen64Plus\mupen64plus.exe",
    r"%LOCALAPPDATA%\Programs\Mupen64Plus\mupen64plus.exe",
    r"%APPDATA%\Mupen64Plus\mupen64plus.exe",
)

# Common POSIX install locations.
POSIX_INSTALL_CANDIDATES = (
    "/usr/bin/mupen64plus",
    "/usr/local/bin/mupen64plus",
    "/opt/mupen64plus/bin/mupen64plus",
    "~/.local/bin/mupen64plus",
)

# Minimum and per-frame timeout slack.
MIN_TIMEOUT_SECONDS = 30.0
PER_FRAME_TIMEOUT_SECONDS = 0.5
DEFAULT_TIMEOUT_SECONDS = 60.0


# --------------------------------------------------------------------------
# Path / binary discovery
# --------------------------------------------------------------------------

def _repo_root() -> Path:
    """Return the ssb64-port repo root (debug_tools/m64p_screenshot/../..)."""
    return Path(__file__).resolve().parent.parent.parent


def find_m64p_binary(user_supplied: Optional[str]) -> Optional[Path]:
    """Locate the mupen64plus executable.

    Resolution order:
      1. --m64p-bin if provided and it exists.
      2. ``shutil.which("mupen64plus")`` / ``mupen64plus.exe``.
      3. Platform-specific hard-coded install candidates.

    Returns the resolved absolute Path, or None if nothing was found.
    """
    if user_supplied:
        p = Path(os.path.expandvars(os.path.expanduser(user_supplied)))
        return p if p.is_file() else None

    for name in M64P_BINARY_NAMES:
        found = shutil.which(name)
        if found:
            return Path(found)

    candidates: Iterable[str]
    if os.name == "nt":
        candidates = WINDOWS_INSTALL_CANDIDATES
    else:
        candidates = POSIX_INSTALL_CANDIDATES

    for raw in candidates:
        expanded = Path(os.path.expandvars(os.path.expanduser(raw)))
        if expanded.is_file():
            return expanded

    return None


def _searched_m64p_paths() -> List[str]:
    """Return the list of paths we'd report on a "binary not found" error."""
    paths: List[str] = ["PATH lookup (shutil.which)"]
    if os.name == "nt":
        for raw in WINDOWS_INSTALL_CANDIDATES:
            paths.append(os.path.expandvars(os.path.expanduser(raw)))
    else:
        for raw in POSIX_INSTALL_CANDIDATES:
            paths.append(os.path.expandvars(os.path.expanduser(raw)))
    return paths


def get_screenshot_dir() -> Path:
    """Return the directory where Mupen64Plus writes screenshots.

    m64p's default is the user data dir:
      * Windows: %APPDATA%/Mupen64Plus/screenshot/
      * Linux:   ~/.local/share/mupen64plus/screenshot/
      * macOS:   ~/Library/Application Support/Mupen64Plus/screenshot/

    Note: users can override this via mupen64plus.cfg "ScreenshotPath".
    We don't parse the cfg — we assume the default, which matches a fresh
    install. If a user has customized it, they can pass --screenshot-dir.
    """
    if os.name == "nt":
        appdata = os.environ.get("APPDATA")
        if not appdata:
            # Fall back to the usual location if APPDATA is unset for some reason.
            appdata = str(Path.home() / "AppData" / "Roaming")
        return Path(appdata) / "Mupen64Plus" / "screenshot"

    if sys.platform == "darwin":
        return Path.home() / "Library" / "Application Support" / "Mupen64Plus" / "screenshot"

    # Linux / other POSIX — XDG_DATA_HOME or ~/.local/share
    xdg = os.environ.get("XDG_DATA_HOME")
    base = Path(xdg) if xdg else (Path.home() / ".local" / "share")
    return base / "mupen64plus" / "screenshot"


# --------------------------------------------------------------------------
# Mupen64Plus invocation
# --------------------------------------------------------------------------

def _compute_timeout(frames: Sequence[int]) -> float:
    """Pick a subprocess timeout that scales with the highest requested frame.

    For short captures we use DEFAULT_TIMEOUT_SECONDS. For long captures we
    budget PER_FRAME_TIMEOUT_SECONDS per frame reached, clamped below by
    MIN_TIMEOUT_SECONDS.
    """
    if not frames:
        return DEFAULT_TIMEOUT_SECONDS
    max_frame = max(frames)
    scaled = max(MIN_TIMEOUT_SECONDS, max_frame * PER_FRAME_TIMEOUT_SECONDS)
    return max(DEFAULT_TIMEOUT_SECONDS, scaled)


def run_m64p_testshots(
    m64p_bin: Path,
    rom_path: Path,
    frames: Sequence[int],
    gfx_plugin: Optional[str],
    timeout: float,
) -> subprocess.CompletedProcess:
    """Invoke mupen64plus with --testshots and return the CompletedProcess.

    Does NOT raise on non-zero exit — caller inspects ``returncode``.
    Raises subprocess.TimeoutExpired on timeout (caller handles).
    """
    frames_arg = ",".join(str(f) for f in frames)

    # Build command. --testshots is the key: m64p captures each listed frame
    # and exits after the last one. --nosaveoptions prevents touching cfg.
    cmd: List[str] = [
        str(m64p_bin),
        "--testshots", frames_arg,
        "--nosaveoptions",
    ]

    # Optional gfx plugin override. If the user doesn't pass one, we let
    # m64p use whatever mupen64plus.cfg specifies (typically glide64mk2 or
    # GLideN64, both of which are fine reference renderers).
    if gfx_plugin:
        cmd.extend(["--gfx", gfx_plugin])

    cmd.append(str(rom_path))

    print(f"[m64p_screenshot] running: {' '.join(cmd)}", flush=True)
    print(f"[m64p_screenshot] timeout: {timeout:.0f}s", flush=True)

    # capture_output=True so stderr/stdout come back for diagnostics.
    return subprocess.run(
        cmd,
        capture_output=True,
        text=True,
        timeout=timeout,
        # Don't inherit any TTY — m64p must never prompt.
        stdin=subprocess.DEVNULL,
    )


# --------------------------------------------------------------------------
# Screenshot collection
# --------------------------------------------------------------------------

def _snapshot_pngs(directory: Path) -> set:
    """Return the set of .png filenames currently in ``directory``."""
    if not directory.is_dir():
        return set()
    return {p.name for p in directory.iterdir() if p.suffix.lower() == ".png"}


def collect_new_screenshots(
    screenshot_dir: Path,
    before: set,
) -> List[Path]:
    """Return newly produced PNGs (those not present in ``before``), mtime-sorted."""
    if not screenshot_dir.is_dir():
        return []
    new_files: List[Path] = []
    for p in screenshot_dir.iterdir():
        if p.suffix.lower() != ".png":
            continue
        if p.name in before:
            continue
        new_files.append(p)
    new_files.sort(key=lambda p: p.stat().st_mtime)
    return new_files


# --------------------------------------------------------------------------
# Argument parsing
# --------------------------------------------------------------------------

def _parse_frames(raw: str) -> List[int]:
    """Parse "10,30,55" into [10, 30, 55]. Rejects empty / non-numeric."""
    parts = [p.strip() for p in raw.split(",") if p.strip()]
    if not parts:
        raise argparse.ArgumentTypeError("--frames must list at least one integer")
    out: List[int] = []
    for p in parts:
        try:
            n = int(p)
        except ValueError as e:
            raise argparse.ArgumentTypeError(f"invalid frame {p!r}: {e}") from e
        if n < 0:
            raise argparse.ArgumentTypeError(f"frame must be >= 0 (got {n})")
        out.append(n)
    return out


def _build_argparser() -> argparse.ArgumentParser:
    ap = argparse.ArgumentParser(
        prog="m64p_screenshot",
        description=(
            "Capture pixel-perfect Mupen64Plus screenshots of SSB64 at specific "
            "frames. Drives m64p via --testshots and collects the resulting PNGs."
        ),
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=(
            "Exit codes: 0 = all frames captured, 1 = partial / m64p failure, "
            "2 = setup/input error."
        ),
    )
    ap.add_argument(
        "--frames",
        required=True,
        type=_parse_frames,
        help="Comma-separated list of N64 frame numbers to capture, e.g. '10,30,55'",
    )
    ap.add_argument(
        "--rom",
        default=None,
        help=f"Path to the SSB64 ROM (default: <repo>/{DEFAULT_ROM_RELPATH})",
    )
    ap.add_argument(
        "--m64p-bin",
        default=None,
        help="Path to mupen64plus binary (default: PATH lookup + common install dirs)",
    )
    ap.add_argument(
        "--gfx-plugin",
        default=None,
        help=(
            "Override video plugin path (default: whatever mupen64plus.cfg says; "
            "typical good choice: mupen64plus-video-glide64mk2 for accuracy)"
        ),
    )
    ap.add_argument(
        "--screenshot-dir",
        default=None,
        help=(
            "Override m64p's screenshot output directory "
            "(default: platform user data dir, e.g. %%APPDATA%%/Mupen64Plus/screenshot)"
        ),
    )
    out_group = ap.add_mutually_exclusive_group()
    out_group.add_argument(
        "--out",
        default=None,
        help="Single-frame output PNG path (only valid if --frames has exactly one frame)",
    )
    out_group.add_argument(
        "--out-dir",
        default=None,
        help=f"Output directory; files named frame_<N>.png (default: <repo>/{DEFAULT_OUT_DIR_RELPATH})",
    )
    ap.add_argument(
        "--timeout",
        type=float,
        default=None,
        help="Subprocess timeout in seconds (default: scaled by max frame)",
    )
    return ap


# --------------------------------------------------------------------------
# Main
# --------------------------------------------------------------------------

def main(argv: Optional[Sequence[str]] = None) -> int:
    args = _build_argparser().parse_args(argv)

    repo = _repo_root()
    frames: List[int] = sorted(set(args.frames))  # dedupe + ascending order
    if not frames:
        print("[m64p_screenshot] ERROR: no frames to capture", file=sys.stderr)
        return 2

    # --- Validate --out vs --out-dir semantics (pure arg check, do first) ---
    if args.out is not None and len(frames) != 1:
        print(
            f"[m64p_screenshot] ERROR: --out requires exactly one frame, got {len(frames)}",
            file=sys.stderr,
        )
        return 2

    # --- Validate ROM path ---
    rom_path = Path(args.rom).expanduser() if args.rom else (repo / DEFAULT_ROM_RELPATH)
    if not rom_path.is_file():
        print(f"[m64p_screenshot] ERROR: ROM not found: {rom_path}", file=sys.stderr)
        print(
            "[m64p_screenshot] Provide --rom or place baserom.us.z64 at the repo root.",
            file=sys.stderr,
        )
        return 2

    # --- Locate m64p binary ---
    m64p_bin = find_m64p_binary(args.m64p_bin)
    if m64p_bin is None:
        print("[m64p_screenshot] ERROR: mupen64plus binary not found.", file=sys.stderr)
        print("[m64p_screenshot] Searched:", file=sys.stderr)
        for p in _searched_m64p_paths():
            print(f"    {p}", file=sys.stderr)
        print(
            "[m64p_screenshot] Install Mupen64Plus or pass --m64p-bin <path>.",
            file=sys.stderr,
        )
        return 2

    # --- Resolve screenshot dir ---
    screenshot_dir = (
        Path(args.screenshot_dir).expanduser()
        if args.screenshot_dir
        else get_screenshot_dir()
    )
    screenshot_dir.mkdir(parents=True, exist_ok=True)

    # --- Resolve user output destination ---
    out_dir: Optional[Path] = None
    out_file: Optional[Path] = None
    if args.out is not None:
        out_file = Path(args.out).expanduser()
        out_file.parent.mkdir(parents=True, exist_ok=True)
    else:
        out_dir = (
            Path(args.out_dir).expanduser()
            if args.out_dir
            else (repo / DEFAULT_OUT_DIR_RELPATH)
        )
        out_dir.mkdir(parents=True, exist_ok=True)

    # --- Snapshot existing screenshots so we can identify new ones ---
    before = _snapshot_pngs(screenshot_dir)
    run_started_at = time.time()

    # --- Run m64p ---
    timeout = args.timeout if args.timeout is not None else _compute_timeout(frames)
    try:
        result = run_m64p_testshots(
            m64p_bin=m64p_bin,
            rom_path=rom_path,
            frames=frames,
            gfx_plugin=args.gfx_plugin,
            timeout=timeout,
        )
    except subprocess.TimeoutExpired as e:
        print(
            f"[m64p_screenshot] ERROR: mupen64plus timed out after {timeout:.0f}s",
            file=sys.stderr,
        )
        if e.stdout:
            print("[m64p_screenshot] --- last stdout ---", file=sys.stderr)
            print(e.stdout[-2000:] if isinstance(e.stdout, str) else "", file=sys.stderr)
        if e.stderr:
            print("[m64p_screenshot] --- last stderr ---", file=sys.stderr)
            print(e.stderr[-2000:] if isinstance(e.stderr, str) else "", file=sys.stderr)
        return 1
    except FileNotFoundError:
        print(
            f"[m64p_screenshot] ERROR: failed to launch {m64p_bin} (FileNotFoundError)",
            file=sys.stderr,
        )
        return 2

    # --- Inspect m64p result ---
    if result.returncode != 0:
        print(
            f"[m64p_screenshot] WARNING: mupen64plus exited with code {result.returncode}",
            file=sys.stderr,
        )
        tail_lines = 30
        if result.stdout:
            print("[m64p_screenshot] --- stdout tail ---", file=sys.stderr)
            for line in result.stdout.splitlines()[-tail_lines:]:
                print(line, file=sys.stderr)
        if result.stderr:
            print("[m64p_screenshot] --- stderr tail ---", file=sys.stderr)
            for line in result.stderr.splitlines()[-tail_lines:]:
                print(line, file=sys.stderr)
        # Still try to collect any screenshots that were produced.

    # --- Collect new screenshots ---
    new_files = collect_new_screenshots(screenshot_dir, before)

    # Defensive filter: only keep files that were actually created during this run.
    # (Avoids latching onto any file mysteriously appearing unrelated to our call.)
    new_files = [p for p in new_files if p.stat().st_mtime + 1.0 >= run_started_at]

    if not new_files:
        print(
            "[m64p_screenshot] ERROR: no new screenshots were produced in "
            f"{screenshot_dir}",
            file=sys.stderr,
        )
        print(
            "[m64p_screenshot] Check that mupen64plus actually executed frames "
            f"up to {max(frames)}, and that --gfx-plugin (if any) is valid.",
            file=sys.stderr,
        )
        return 1

    # --- Map captured PNGs to requested frames ---
    # m64p --testshots reaches frames in ascending order, so files sorted by
    # mtime correspond 1:1 to the sorted ``frames`` list.
    pairs = list(zip(frames, new_files))
    captured_count = len(pairs)

    copied: List[Path] = []
    for frame_no, src in pairs:
        if out_file is not None:
            dest = out_file
        else:
            assert out_dir is not None
            dest = out_dir / f"frame_{frame_no:05d}.png"
        shutil.copy2(src, dest)
        copied.append(dest)
        print(f"frame {frame_no} -> {dest}")

    # --- Report ---
    total_requested = len(frames)
    if captured_count < total_requested:
        missing = frames[captured_count:]
        print(
            f"[m64p_screenshot] WARNING: only {captured_count}/{total_requested} "
            f"frames captured; missing: {missing}",
            file=sys.stderr,
        )
        return 1

    print(f"[m64p_screenshot] ok — captured {captured_count} frame(s)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
