#!/usr/bin/env python3
"""
gbi_diff.py — Compare GBI display list traces between the port and emulator.

Parses .gbi trace files produced by:
  - Port-side gbi_trace system (debug_traces/port_trace.gbi)
  - Mupen64Plus trace plugin   (emu_trace.gbi)

Aligns commands by frame, produces a structured diff highlighting divergences.

Usage:
    python gbi_diff.py <port_trace.gbi> <emu_trace.gbi> [options]

Options:
    --frame N          Only diff frame N (default: all frames)
    --frame-range A-B  Diff frames A through B inclusive
    --context N        Show N matching commands around each divergence (default: 3)
    --summary          Only show per-frame summary, not individual diffs
    --ignore-addresses Don't flag address differences in G_DL/G_VTX/G_MTX etc.
    --output FILE      Write diff to file instead of stdout
"""
import argparse
import re
import sys
from dataclasses import dataclass
from typing import Optional


# ============================================================================
#  Data structures
# ============================================================================

@dataclass
class GbiCommand:
    """A single parsed GBI command."""
    index: int          # [NNNN] command index within frame
    depth: int          # d=N display list call depth
    opcode: str         # G_VTX, G_TRI1, etc.
    w0: str             # 8-hex-digit w0
    w1: str             # 8-hex-digit w1
    params: str         # Decoded parameter string
    w1_64: str = ""     # Optional 64-bit w1 from port (for info only)
    raw: str = ""       # Full original line

    @property
    def opcode_and_params(self) -> str:
        return f"{self.opcode} {self.params}".strip()


@dataclass
class Frame:
    """A single frame's worth of commands."""
    number: int
    commands: list  # list[GbiCommand]
    total_cmds: int  # reported count from END FRAME line


# ============================================================================
#  Parser
# ============================================================================

# Matches: [0042] d=1 G_VTX              w0=01020040 w1=06001234  n=2 v0=0 addr=06001234
CMD_RE = re.compile(
    r'\[(\d+)\]\s+d=(\d+)\s+'       # index, depth
    r'(\S+)\s+'                       # opcode
    r'w0=([0-9A-Fa-f]+)\s+'          # w0
    r'w1=([0-9A-Fa-f]+)'             # w1
    r'(?:\s+(.*))?'                   # optional decoded params
)

# Matches the optional (w1_64=...) suffix on port traces
W1_64_RE = re.compile(r'\(w1_64=([0-9A-Fa-f]+)\)')

FRAME_START_RE = re.compile(r'^=== FRAME (\d+) ===$')
FRAME_END_RE = re.compile(r'^=== END FRAME (\d+)\s*(?:—|-)\s*(\d+) commands ===$')


def parse_trace(filepath: str) -> dict[int, Frame]:
    """Parse a .gbi trace file into a dict of frame_number -> Frame."""
    frames: dict[int, Frame] = {}
    current_frame: Optional[int] = None
    current_cmds: list[GbiCommand] = []

    with open(filepath, 'r') as f:
        for line in f:
            line = line.rstrip('\n')

            # Skip comments and blank lines
            if not line or line.startswith('#'):
                continue

            # Frame start
            m = FRAME_START_RE.match(line)
            if m:
                current_frame = int(m.group(1))
                current_cmds = []
                continue

            # Frame end
            m = FRAME_END_RE.match(line)
            if m:
                frame_num = int(m.group(1))
                total = int(m.group(2))
                frames[frame_num] = Frame(
                    number=frame_num,
                    commands=current_cmds,
                    total_cmds=total,
                )
                current_frame = None
                continue

            # Command line
            if current_frame is not None:
                m = CMD_RE.match(line)
                if m:
                    w1_64 = ""
                    m2 = W1_64_RE.search(line)
                    if m2:
                        w1_64 = m2.group(1)

                    cmd = GbiCommand(
                        index=int(m.group(1)),
                        depth=int(m.group(2)),
                        opcode=m.group(3),
                        w0=m.group(4).upper(),
                        w1=m.group(5).upper(),
                        params=m.group(6).strip() if m.group(6) else "",
                        w1_64=w1_64,
                        raw=line,
                    )
                    current_cmds.append(cmd)

    return frames


# ============================================================================
#  Comparison logic
# ============================================================================

# Opcodes where w1 is an address (not comparable between port and emu)
ADDRESS_OPCODES = {
    'G_VTX', 'G_DL', 'G_MTX', 'G_SETTIMG', 'G_SETCIMG', 'G_SETZIMG',
    'G_MOVEMEM', 'G_LOAD_UCODE', 'G_BRANCH_Z', 'G_DMA_IO',
}


@dataclass
class Divergence:
    """A single command divergence between port and emu."""
    cmd_index: int
    port_cmd: Optional[GbiCommand]
    emu_cmd: Optional[GbiCommand]
    reason: str  # What diverged: opcode, w0, w1, params, missing


def compare_commands(port_cmd: GbiCommand, emu_cmd: GbiCommand,
                     ignore_addresses: bool) -> Optional[Divergence]:
    """Compare two commands. Returns a Divergence if they differ, else None."""

    # Different opcode = definite divergence
    if port_cmd.opcode != emu_cmd.opcode:
        return Divergence(
            cmd_index=port_cmd.index,
            port_cmd=port_cmd,
            emu_cmd=emu_cmd,
            reason=f"opcode: {port_cmd.opcode} vs {emu_cmd.opcode}",
        )

    # Same opcode — compare w0 (always comparable)
    if port_cmd.w0 != emu_cmd.w0:
        return Divergence(
            cmd_index=port_cmd.index,
            port_cmd=port_cmd,
            emu_cmd=emu_cmd,
            reason=f"w0: {port_cmd.w0} vs {emu_cmd.w0}",
        )

    # Compare w1 — skip for address opcodes if requested
    if not (ignore_addresses and port_cmd.opcode in ADDRESS_OPCODES):
        if port_cmd.w1 != emu_cmd.w1:
            return Divergence(
                cmd_index=port_cmd.index,
                port_cmd=port_cmd,
                emu_cmd=emu_cmd,
                reason=f"w1: {port_cmd.w1} vs {emu_cmd.w1}",
            )

    return None


def diff_frame(port_frame: Frame, emu_frame: Frame,
               ignore_addresses: bool) -> list[Divergence]:
    """Diff two frames command-by-command. Returns list of divergences."""
    divergences: list[Divergence] = []

    max_len = max(len(port_frame.commands), len(emu_frame.commands))

    for i in range(max_len):
        port_cmd = port_frame.commands[i] if i < len(port_frame.commands) else None
        emu_cmd = emu_frame.commands[i] if i < len(emu_frame.commands) else None

        if port_cmd is None:
            divergences.append(Divergence(
                cmd_index=i,
                port_cmd=None,
                emu_cmd=emu_cmd,
                reason="missing in port trace",
            ))
            continue

        if emu_cmd is None:
            divergences.append(Divergence(
                cmd_index=i,
                port_cmd=port_cmd,
                emu_cmd=None,
                reason="missing in emu trace",
            ))
            continue

        div = compare_commands(port_cmd, emu_cmd, ignore_addresses)
        if div:
            divergences.append(div)

    return divergences


# ============================================================================
#  Output formatting
# ============================================================================

def format_cmd(label: str, cmd: Optional[GbiCommand]) -> str:
    """Format a command for diff output."""
    if cmd is None:
        return f"  {label}: (absent)"
    return f"  {label}: [{cmd.index:04d}] d={cmd.depth} {cmd.opcode:<18s} w0={cmd.w0} w1={cmd.w1}  {cmd.params}"


def print_diff(port_frames: dict[int, Frame], emu_frames: dict[int, Frame],
               args, out):
    """Produce the full diff report."""

    # Determine frame range
    all_frames = sorted(set(port_frames.keys()) | set(emu_frames.keys()))

    if args.frame is not None:
        all_frames = [f for f in all_frames if f == args.frame]
    elif args.frame_range:
        a, b = map(int, args.frame_range.split('-'))
        all_frames = [f for f in all_frames if a <= f <= b]

    total_divs = 0
    total_frames_diffed = 0
    frames_with_divs = 0

    for fnum in all_frames:
        port_frame = port_frames.get(fnum)
        emu_frame = emu_frames.get(fnum)

        if port_frame is None:
            out.write(f"\n--- FRAME {fnum}: present in emu only ({emu_frame.total_cmds} cmds) ---\n")
            total_frames_diffed += 1
            frames_with_divs += 1
            continue

        if emu_frame is None:
            out.write(f"\n--- FRAME {fnum}: present in port only ({port_frame.total_cmds} cmds) ---\n")
            total_frames_diffed += 1
            frames_with_divs += 1
            continue

        divergences = diff_frame(port_frame, emu_frame, args.ignore_addresses)
        total_frames_diffed += 1

        if not divergences:
            if not args.summary:
                # Only note matching frames in verbose mode
                pass
            continue

        frames_with_divs += 1
        total_divs += len(divergences)

        port_count = len(port_frame.commands)
        emu_count = len(emu_frame.commands)
        matching = max(port_count, emu_count) - len(divergences)

        out.write(f"\n{'='*72}\n")
        out.write(f"FRAME {fnum}: {len(divergences)} divergences "
                  f"(port={port_count} cmds, emu={emu_count} cmds, "
                  f"{matching} matching)\n")
        out.write(f"{'='*72}\n")

        if args.summary:
            # Just list divergence types
            opcode_divs = sum(1 for d in divergences if d.reason.startswith("opcode"))
            w0_divs = sum(1 for d in divergences if d.reason.startswith("w0"))
            w1_divs = sum(1 for d in divergences if d.reason.startswith("w1"))
            missing = sum(1 for d in divergences if "missing" in d.reason)
            out.write(f"  opcode mismatches: {opcode_divs}\n")
            out.write(f"  w0 mismatches:     {w0_divs}\n")
            out.write(f"  w1 mismatches:     {w1_divs}\n")
            out.write(f"  missing commands:  {missing}\n")
            continue

        # Detailed divergence output with context
        for div in divergences:
            out.write(f"\n  DIVERGENCE at cmd #{div.cmd_index} — {div.reason}\n")
            out.write(format_cmd("PORT", div.port_cmd) + "\n")
            out.write(format_cmd("EMU ", div.emu_cmd) + "\n")

            # Show context (surrounding matching commands)
            if args.context > 0:
                ctx_start = max(0, div.cmd_index - args.context)
                ctx_end = min(max(port_count, emu_count),
                              div.cmd_index + args.context + 1)

                has_context = False
                for ci in range(ctx_start, ctx_end):
                    if ci == div.cmd_index:
                        continue
                    pc = port_frame.commands[ci] if ci < port_count else None
                    ec = emu_frame.commands[ci] if ci < emu_count else None
                    if pc and ec and pc.opcode == ec.opcode:
                        if not has_context:
                            out.write("  context:\n")
                            has_context = True
                        out.write(f"    [{ci:04d}] {pc.opcode:<18s} "
                                  f"w0={pc.w0} w1={pc.w1}  {pc.params}\n")

    # Summary
    out.write(f"\n{'='*72}\n")
    out.write(f"SUMMARY: {total_frames_diffed} frames compared, "
              f"{frames_with_divs} with divergences, "
              f"{total_divs} total divergences\n")
    out.write(f"{'='*72}\n")


# ============================================================================
#  Main
# ============================================================================

def main():
    parser = argparse.ArgumentParser(
        description="Compare GBI display list traces between port and emulator")
    parser.add_argument("port_trace", help="Port-side trace file (.gbi)")
    parser.add_argument("emu_trace", help="Emulator-side trace file (.gbi)")
    parser.add_argument("--frame", type=int, default=None,
                        help="Only diff this specific frame number")
    parser.add_argument("--frame-range", type=str, default=None,
                        help="Diff frame range (e.g. 10-20)")
    parser.add_argument("--context", type=int, default=3,
                        help="Lines of context around divergences (default: 3)")
    parser.add_argument("--summary", action="store_true",
                        help="Only show per-frame summary counts")
    parser.add_argument("--ignore-addresses", action="store_true",
                        help="Don't flag w1 differences for address-bearing opcodes")
    parser.add_argument("--output", type=str, default=None,
                        help="Output file (default: stdout)")

    args = parser.parse_args()

    print(f"Parsing port trace: {args.port_trace}")
    port_frames = parse_trace(args.port_trace)
    print(f"  -> {len(port_frames)} frames parsed")

    print(f"Parsing emu trace: {args.emu_trace}")
    emu_frames = parse_trace(args.emu_trace)
    print(f"  -> {len(emu_frames)} frames parsed")

    out = open(args.output, 'w') if args.output else sys.stdout

    try:
        print_diff(port_frames, emu_frames, args, out)
    finally:
        if args.output and out is not sys.stdout:
            out.close()
            print(f"Diff written to {args.output}")


if __name__ == "__main__":
    main()
