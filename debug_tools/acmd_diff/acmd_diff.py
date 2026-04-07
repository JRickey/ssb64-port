#!/usr/bin/env python3
"""
acmd_diff.py — Compare Acmd audio command traces between the port and emulator.

Parses .acmd trace files produced by:
  - Port-side acmd_trace system (debug_traces/port_acmd_trace.acmd)
  - Mupen64Plus RSP trace plugin (emu_acmd_trace.acmd)

Aligns commands by audio task, produces a structured diff highlighting divergences.

Usage:
    python acmd_diff.py <port_trace.acmd> <emu_trace.acmd> [options]

Options:
    --task N             Only diff task N (default: all tasks)
    --task-range A-B     Diff tasks A through B inclusive
    --context N          Show N matching commands around each divergence (default: 3)
    --summary            Only show per-task summary, not individual diffs
    --ignore-addresses   Don't flag address differences in A_LOADBUFF/A_SAVEBUFF etc.
    --output FILE        Write diff to file instead of stdout
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
class AcmdCommand:
    """A single parsed audio command."""
    index: int          # [NNNN] command index within task
    opcode: str         # A_ADPCM, A_RESAMPLE, etc.
    w0: str             # 8-hex-digit w0
    w1: str             # 8-hex-digit w1
    params: str         # Decoded parameter string
    raw: str = ""       # Full original line

    @property
    def opcode_and_params(self) -> str:
        return f"{self.opcode} {self.params}".strip()


@dataclass
class AudioTask:
    """A single audio task's worth of commands."""
    number: int
    commands: list  # list[AcmdCommand]
    total_cmds: int  # reported count from END AUDIO TASK line


# ============================================================================
#  Parser
# ============================================================================

# Matches: [0042] A_ADPCM          w0=01010000 w1=80140800  flags=INIT gain=0 state=80140800
CMD_RE = re.compile(
    r'\[(\d+)\]\s+'                  # index
    r'(\S+)\s+'                      # opcode
    r'w0=([0-9A-Fa-f]+)\s+'          # w0
    r'w1=([0-9A-Fa-f]+)'             # w1
    r'(?:\s+(.*))?'                  # optional decoded params
)

TASK_START_RE = re.compile(r'^=== AUDIO TASK (\d+) ===$')
TASK_END_RE = re.compile(r'^=== END AUDIO TASK (\d+)\s*(?:—|-)\s*(\d+) commands ===$')


def parse_trace(filepath: str) -> dict[int, AudioTask]:
    """Parse an .acmd trace file into a dict of task_number -> AudioTask."""
    tasks: dict[int, AudioTask] = {}
    current_task: Optional[int] = None
    current_cmds: list[AcmdCommand] = []

    with open(filepath, 'r', encoding='utf-8') as f:
        for line in f:
            line = line.rstrip('\n')

            # Skip comments and blank lines
            if not line or line.startswith('#'):
                continue

            # Task start
            m = TASK_START_RE.match(line)
            if m:
                current_task = int(m.group(1))
                current_cmds = []
                continue

            # Task end
            m = TASK_END_RE.match(line)
            if m:
                task_num = int(m.group(1))
                total = int(m.group(2))
                tasks[task_num] = AudioTask(
                    number=task_num,
                    commands=current_cmds,
                    total_cmds=total,
                )
                current_task = None
                continue

            # Command line
            if current_task is not None:
                m = CMD_RE.match(line)
                if m:
                    cmd = AcmdCommand(
                        index=int(m.group(1)),
                        opcode=m.group(2),
                        w0=m.group(3).upper(),
                        w1=m.group(4).upper(),
                        params=m.group(5).strip() if m.group(5) else "",
                        raw=line,
                    )
                    current_cmds.append(cmd)

    return tasks


# ============================================================================
#  Comparison logic
# ============================================================================

# Opcodes where w1 is a DRAM address (not comparable between port and emu)
ADDRESS_OPCODES = {
    'A_ADPCM', 'A_LOADBUFF', 'A_SAVEBUFF', 'A_LOADADPCM',
    'A_SETLOOP', 'A_ENVMIXER', 'A_POLEF', 'A_RESAMPLE', 'A_SEGMENT',
}


@dataclass
class Divergence:
    """A single command divergence between port and emu."""
    cmd_index: int
    port_cmd: Optional[AcmdCommand]
    emu_cmd: Optional[AcmdCommand]
    reason: str  # What diverged: opcode, w0, w1, missing


def compare_commands(port_cmd: AcmdCommand, emu_cmd: AcmdCommand,
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


def diff_task(port_task: AudioTask, emu_task: AudioTask,
              ignore_addresses: bool) -> list[Divergence]:
    """Diff two audio tasks command-by-command. Returns list of divergences."""
    divergences: list[Divergence] = []

    max_len = max(len(port_task.commands), len(emu_task.commands))

    for i in range(max_len):
        port_cmd = port_task.commands[i] if i < len(port_task.commands) else None
        emu_cmd = emu_task.commands[i] if i < len(emu_task.commands) else None

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

def format_cmd(label: str, cmd: Optional[AcmdCommand]) -> str:
    """Format a command for diff output."""
    if cmd is None:
        return f"  {label}: (absent)"
    return f"  {label}: [{cmd.index:04d}] {cmd.opcode:<16s} w0={cmd.w0} w1={cmd.w1}  {cmd.params}"


def print_diff(port_tasks: dict[int, AudioTask], emu_tasks: dict[int, AudioTask],
               args, out):
    """Produce the full diff report."""

    # Determine task range
    all_tasks = sorted(set(port_tasks.keys()) | set(emu_tasks.keys()))

    if args.task is not None:
        all_tasks = [t for t in all_tasks if t == args.task]
    elif args.task_range:
        a, b = map(int, args.task_range.split('-'))
        all_tasks = [t for t in all_tasks if a <= t <= b]

    total_divs = 0
    total_tasks_diffed = 0
    tasks_with_divs = 0

    for tnum in all_tasks:
        port_task = port_tasks.get(tnum)
        emu_task = emu_tasks.get(tnum)

        if port_task is None:
            out.write(f"\n--- AUDIO TASK {tnum}: present in emu only ({emu_task.total_cmds} cmds) ---\n")
            total_tasks_diffed += 1
            tasks_with_divs += 1
            continue

        if emu_task is None:
            out.write(f"\n--- AUDIO TASK {tnum}: present in port only ({port_task.total_cmds} cmds) ---\n")
            total_tasks_diffed += 1
            tasks_with_divs += 1
            continue

        divergences = diff_task(port_task, emu_task, args.ignore_addresses)
        total_tasks_diffed += 1

        if not divergences:
            continue

        tasks_with_divs += 1
        total_divs += len(divergences)

        port_count = len(port_task.commands)
        emu_count = len(emu_task.commands)
        matching = max(port_count, emu_count) - len(divergences)

        out.write(f"\n{'='*72}\n")
        out.write(f"AUDIO TASK {tnum}: {len(divergences)} divergences "
                  f"(port={port_count} cmds, emu={emu_count} cmds, "
                  f"{matching} matching)\n")
        out.write(f"{'='*72}\n")

        if args.summary:
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
                    pc = port_task.commands[ci] if ci < port_count else None
                    ec = emu_task.commands[ci] if ci < emu_count else None
                    if pc and ec and pc.opcode == ec.opcode:
                        if not has_context:
                            out.write("  context:\n")
                            has_context = True
                        out.write(f"    [{ci:04d}] {pc.opcode:<16s} "
                                  f"w0={pc.w0} w1={pc.w1}  {pc.params}\n")

    # Summary
    out.write(f"\n{'='*72}\n")
    out.write(f"SUMMARY: {total_tasks_diffed} audio tasks compared, "
              f"{tasks_with_divs} with divergences, "
              f"{total_divs} total divergences\n")
    out.write(f"{'='*72}\n")


# ============================================================================
#  Main
# ============================================================================

def main():
    parser = argparse.ArgumentParser(
        description="Compare Acmd audio command traces between port and emulator")
    parser.add_argument("port_trace", help="Port-side trace file (.acmd)")
    parser.add_argument("emu_trace", help="Emulator-side trace file (.acmd)")
    parser.add_argument("--task", type=int, default=None,
                        help="Only diff this specific task number")
    parser.add_argument("--task-range", type=str, default=None,
                        help="Diff task range (e.g. 10-20)")
    parser.add_argument("--context", type=int, default=3,
                        help="Lines of context around divergences (default: 3)")
    parser.add_argument("--summary", action="store_true",
                        help="Only show per-task summary counts")
    parser.add_argument("--ignore-addresses", action="store_true",
                        help="Don't flag w1 differences for address-bearing opcodes")
    parser.add_argument("--output", type=str, default=None,
                        help="Output file (default: stdout)")

    args = parser.parse_args()

    print(f"Parsing port trace: {args.port_trace}")
    port_tasks = parse_trace(args.port_trace)
    print(f"  -> {len(port_tasks)} audio tasks parsed")

    print(f"Parsing emu trace: {args.emu_trace}")
    emu_tasks = parse_trace(args.emu_trace)
    print(f"  -> {len(emu_tasks)} audio tasks parsed")

    out = open(args.output, 'w') if args.output else sys.stdout

    try:
        print_diff(port_tasks, emu_tasks, args, out)
    finally:
        if args.output and out is not sys.stdout:
            out.close()
            print(f"Diff written to {args.output}")


if __name__ == "__main__":
    main()
