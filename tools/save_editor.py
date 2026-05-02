#!/usr/bin/env python3
"""SSB64 PC port save game editor.

Reads / writes the binary save file the port persists to disk. The on-disk
format mirrors the in-memory `LBBackupData` struct from `src/lb/lbtypes.h`
exactly (little-endian, native LP64 alignment, two redundant slots).

Default save file location (when --file is omitted):
    macOS   ~/Library/Application Support/BattleShip/ssb64_save.bin
    Linux   ${XDG_DATA_HOME:-~/.local/share}/BattleShip/ssb64_save.bin
    Windows %APPDATA%\\BattleShip\\ssb64_save.bin

The same path is used by the running game (port_save.cpp resolves it via
SDL_GetPrefPath, with override env var `SSB64_SAVE_PATH`).

Usage examples:
    # Inspect the current save
    python tools/save_editor.py dump

    # Generate a fully-completed save (every char, stage, option)
    python tools/save_editor.py write --all

    # Just unlock the four hidden characters and item switch
    python tools/save_editor.py write --unlock-chars --item-switch

    # Reset to a fresh-cart save (defaults the game writes on first boot)
    python tools/save_editor.py write --reset

Run `python tools/save_editor.py write --help` for the full flag list.
"""

from __future__ import annotations

import argparse
import os
import struct
import sys
from dataclasses import dataclass, field
from pathlib import Path
from typing import List, Optional


# ---------------------------------------------------------------------------
# Game constants — kept in sync with the C headers.
# ---------------------------------------------------------------------------

NUM_FIGHTERS = 12  # GMCOMMON_FIGHTERS_PLAYABLE_NUM

# FTKind enum — index into vs_records / spgame_records.
FIGHTERS = [
    "Mario", "Fox", "DonkeyKong", "Samus", "Luigi", "Link",
    "Yoshi", "CaptainFalcon", "Kirby", "Pikachu", "Jigglypuff", "Ness",
]

# Bit positions in `unlock_mask`.
UNLOCK_BITS = {
    "luigi":      0,
    "ness":       1,
    "captain":    2,
    "purin":      3,
    "inishie":    4,  # Mushroom Kingdom stage
    "soundtest":  5,
    "itemswitch": 6,
}
UNLOCK_MASK_NEWCOMERS = (
    (1 << UNLOCK_BITS["luigi"]) | (1 << UNLOCK_BITS["ness"]) |
    (1 << UNLOCK_BITS["captain"]) | (1 << UNLOCK_BITS["purin"])
)
UNLOCK_MASK_PRIZE = (
    (1 << UNLOCK_BITS["inishie"]) | (1 << UNLOCK_BITS["soundtest"]) |
    (1 << UNLOCK_BITS["itemswitch"])
)
UNLOCK_MASK_ALL = UNLOCK_MASK_NEWCOMERS | UNLOCK_MASK_PRIZE

# Bits in `fighter_mask` — only the unlockable four matter (the eight starters
# are gated by LBBACKUP_CHARACTER_MASK_STARTER OR'd in at access sites).
FIGHTER_MASK_UNLOCK = (
    (1 << FIGHTERS.index("Luigi")) |
    (1 << FIGHTERS.index("CaptainFalcon")) |
    (1 << FIGHTERS.index("Jigglypuff")) |
    (1 << FIGHTERS.index("Ness"))
)
FIGHTER_MASK_ALL = (1 << NUM_FIGHTERS) - 1

# Stage bits in `ground_mask` — eight starter VS stages.
STAGES = [
    "Castle",   # Peach's Castle
    "Sector",   # Sector Z
    "Jungle",   # Kongo Jungle
    "Zebes",    # Planet Zebes
    "Hyrule",   # Hyrule Castle
    "Yoster",   # Yoshi's Island
    "Pupupu",   # Dream Land
    "Yamabuki", # Saffron City
]
GROUND_MASK_ALL = (1 << len(STAGES)) - 1

# 1P difficulty values.
DIFFICULTY_VERY_EASY  = 0
DIFFICULTY_EASY       = 1
DIFFICULTY_NORMAL     = 2
DIFFICULTY_HARD       = 3
DIFFICULTY_VERY_HARD  = 4

# Bonus stage targets/platforms count for a perfect run.
BONUS_TASK_MAX = 10  # SCBATTLE_BONUSGAME_TASK_MAX

# Default "no record" bonus time (sentinel = 60 minutes in ticks @ 60Hz).
TIME_SEC = 60
DEFAULT_BONUS_TIME = 60 * 60 * TIME_SEC  # 216000

# Anti-piracy flag bits in `error_flags` — keep clear unless debugging.
ERROR_BIT_RANDOMKB    = 0
ERROR_BIT_HALFSTICK   = 1
ERROR_BIT_1PMARIO     = 2
ERROR_BIT_VSCASTLE    = 3

# Required signature, checked by lbBackupIsChecksumValid.
SIGNATURE = 666  # 0x29A

# Trailing 4 SRAM bytes hold checksum; checksum sums everything before.
BACKUP_SIZE      = 1516
CHECKSUM_OFFSET  = 1512
SLOT_A_OFFSET    = 0
SLOT_B_OFFSET    = 0x5F0     # ALIGN(1516, 0x10) == 1520
SRAM_FILE_SIZE   = 0x8000    # PORT_SAVE_SIZE in port_save.h


# ---------------------------------------------------------------------------
# Struct layout (verified empirically against the LP64/clang build).
# ---------------------------------------------------------------------------
#
# LBBackupVSRecord  — 92 bytes
#   u16 ko_count[12]                offset  0   (24)
#   u32 time_used                   offset 24   ( 4)
#   u32 damage_given                offset 28   ( 4)
#   u32 damage_taken                offset 32   ( 4)
#   u16 unk                         offset 36   ( 2)
#   u16 selfdestructs               offset 38   ( 2)
#   u16 games_played                offset 40   ( 2)
#   u16 player_count_tally          offset 42   ( 2)
#   u16 player_count_tallies[12]    offset 44   (24)
#   u16 played_against[12]          offset 68   (24)
#
# LBBackup1PRecord  — 32 bytes (incl. trailing pad to 4-byte alignment)
#   u32 spgame_hiscore              offset  0   ( 4)
#   u32 spgame_continues            offset  4   ( 4)
#   u32 spgame_total_bonuses        offset  8   ( 4)
#   u8  spgame_best_difficulty      offset 12   ( 1)
#   u32 bonus1_time                 offset 16   ( 4)
#   u8  bonus1_task_count           offset 20   ( 1)
#   u32 bonus2_time                 offset 24   ( 4)
#   u8  bonus2_task_count           offset 28   ( 1)
#   ub8 is_spgame_complete          offset 29   ( 1)
#
# LBBackupData  — 1516 bytes
#   LBBackupVSRecord vs_records[12] offset    0  (1104)
#   ub8  is_allow_screenflash       offset 1104  ( 1)
#   ub8  sound_mono_or_stereo       offset 1105  ( 1)
#   s16  screen_adjust_h            offset 1106  ( 2)
#   s16  screen_adjust_v            offset 1108  ( 2)
#   u8   characters_fkind           offset 1110  ( 1)
#   u8   unlock_mask                offset 1111  ( 1)
#   u16  fighter_mask               offset 1112  ( 2)
#   u8   spgame_difficulty          offset 1114  ( 1)
#   u8   spgame_stock_count         offset 1115  ( 1)
#   LBBackup1PRecord spgame_records[12] offset 1116 (384)
#   u16  ground_mask                offset 1500  ( 2)
#   u8   vs_itemswitch_battles      offset 1502  ( 1)
#   u16  vs_total_battles           offset 1504  ( 2)
#   u8   error_flags                offset 1506  ( 1)
#   u8   boot                       offset 1507  ( 1)
#   u16  signature                  offset 1508  ( 2)
#   s32  checksum                   offset 1512  ( 4)


# All multi-byte integers are little-endian (PC port writes native LE bytes).
VS_FMT = "<12HIIIHHHH12H12H"
assert struct.calcsize(VS_FMT) == 92, struct.calcsize(VS_FMT)

SP_FMT = "<IIIBxxxIBxxxIBB"  # trailing 'B' for is_spgame_complete then pad
# Actually compute size with explicit padding.


@dataclass
class VSRecord:
    ko_count: List[int] = field(default_factory=lambda: [0] * 12)
    time_used: int = 0
    damage_given: int = 0
    damage_taken: int = 0
    unk: int = 0
    selfdestructs: int = 0
    games_played: int = 0
    player_count_tally: int = 0
    player_count_tallies: List[int] = field(default_factory=lambda: [0] * 12)
    played_against: List[int] = field(default_factory=lambda: [0] * 12)

    def pack(self) -> bytes:
        return struct.pack(
            VS_FMT,
            *self.ko_count,
            self.time_used,
            self.damage_given,
            self.damage_taken,
            self.unk,
            self.selfdestructs,
            self.games_played,
            self.player_count_tally,
            *self.player_count_tallies,
            *self.played_against,
        )

    @classmethod
    def unpack(cls, buf: bytes) -> "VSRecord":
        v = struct.unpack(VS_FMT, buf)
        return cls(
            ko_count=list(v[0:12]),
            time_used=v[12],
            damage_given=v[13],
            damage_taken=v[14],
            unk=v[15],
            selfdestructs=v[16],
            games_played=v[17],
            player_count_tally=v[18],
            player_count_tallies=list(v[19:31]),
            played_against=list(v[31:43]),
        )


@dataclass
class SPRecord:
    spgame_hiscore: int = 0
    spgame_continues: int = 0
    spgame_total_bonuses: int = 0
    spgame_best_difficulty: int = 0
    bonus1_time: int = DEFAULT_BONUS_TIME
    bonus1_task_count: int = 0
    bonus2_time: int = DEFAULT_BONUS_TIME
    bonus2_task_count: int = 0
    is_spgame_complete: int = 0

    def pack(self) -> bytes:
        # 32 bytes: laid out by hand to match LP64 padding.
        return struct.pack(
            "<III B 3x I B 3x I B B 2x",
            self.spgame_hiscore,
            self.spgame_continues,
            self.spgame_total_bonuses,
            self.spgame_best_difficulty,
            self.bonus1_time,
            self.bonus1_task_count,
            self.bonus2_time,
            self.bonus2_task_count,
            self.is_spgame_complete,
        )

    @classmethod
    def unpack(cls, buf: bytes) -> "SPRecord":
        assert len(buf) == 32, len(buf)
        v = struct.unpack("<III B 3x I B 3x I B B 2x", buf)
        return cls(
            spgame_hiscore=v[0],
            spgame_continues=v[1],
            spgame_total_bonuses=v[2],
            spgame_best_difficulty=v[3],
            bonus1_time=v[4],
            bonus1_task_count=v[5],
            bonus2_time=v[6],
            bonus2_task_count=v[7],
            is_spgame_complete=v[8],
        )


assert struct.calcsize("<III B 3x I B 3x I B B 2x") == 32


@dataclass
class BackupData:
    vs_records: List[VSRecord] = field(default_factory=lambda: [VSRecord() for _ in range(NUM_FIGHTERS)])
    is_allow_screenflash: int = 1
    sound_mono_or_stereo: int = 1   # 0 = mono, 1 = stereo
    screen_adjust_h: int = 0
    screen_adjust_v: int = 0
    characters_fkind: int = 0       # nFTKindMario
    unlock_mask: int = 0
    fighter_mask: int = 0
    spgame_difficulty: int = DIFFICULTY_EASY
    spgame_stock_count: int = 2
    spgame_records: List[SPRecord] = field(default_factory=lambda: [SPRecord() for _ in range(NUM_FIGHTERS)])
    ground_mask: int = 0
    vs_itemswitch_battles: int = 0
    vs_total_battles: int = 0
    error_flags: int = 0
    boot: int = 0
    signature: int = SIGNATURE
    checksum: int = 0  # filled in by pack()

    # ----- pack / unpack -----------------------------------------------

    def pack(self) -> bytes:
        body = bytearray()
        for r in self.vs_records:
            body += r.pack()
        assert len(body) == 1104, len(body)

        body += struct.pack(
            "<BB hh BB H BB",
            self.is_allow_screenflash & 0xFF,
            self.sound_mono_or_stereo & 0xFF,
            self.screen_adjust_h,
            self.screen_adjust_v,
            self.characters_fkind & 0xFF,
            self.unlock_mask & 0xFF,
            self.fighter_mask & 0xFFFF,
            self.spgame_difficulty & 0xFF,
            self.spgame_stock_count & 0xFF,
        )
        assert len(body) == 1116, len(body)

        for r in self.spgame_records:
            body += r.pack()
        assert len(body) == 1500, len(body)

        # ground_mask, vs_itemswitch_battles, [pad], vs_total_battles,
        # error_flags, boot, signature, [pad to align s32 checksum]
        body += struct.pack(
            "<H B x H B B H xx",
            self.ground_mask & 0xFFFF,
            self.vs_itemswitch_battles & 0xFF,
            self.vs_total_battles & 0xFFFF,
            self.error_flags & 0xFF,
            self.boot & 0xFF,
            self.signature & 0xFFFF,
        )
        assert len(body) == CHECKSUM_OFFSET, len(body)

        cksum = compute_checksum(bytes(body))
        body += struct.pack("<i", cksum)
        assert len(body) == BACKUP_SIZE, len(body)
        return bytes(body)

    @classmethod
    def unpack(cls, buf: bytes) -> "BackupData":
        if len(buf) < BACKUP_SIZE:
            raise ValueError(f"backup blob too short: {len(buf)} < {BACKUP_SIZE}")
        buf = buf[:BACKUP_SIZE]

        out = cls()
        off = 0
        out.vs_records = [VSRecord.unpack(buf[off + i * 92: off + (i + 1) * 92])
                          for i in range(NUM_FIGHTERS)]
        off += 1104

        (
            out.is_allow_screenflash,
            out.sound_mono_or_stereo,
            out.screen_adjust_h,
            out.screen_adjust_v,
            out.characters_fkind,
            out.unlock_mask,
            out.fighter_mask,
            out.spgame_difficulty,
            out.spgame_stock_count,
        ) = struct.unpack("<BB hh BB H BB", buf[off:off + 12])
        off += 12
        assert off == 1116

        out.spgame_records = [SPRecord.unpack(buf[off + i * 32: off + (i + 1) * 32])
                              for i in range(NUM_FIGHTERS)]
        off += 384
        assert off == 1500

        (
            out.ground_mask,
            out.vs_itemswitch_battles,
            out.vs_total_battles,
            out.error_flags,
            out.boot,
            out.signature,
        ) = struct.unpack("<H B x H B B H xx", buf[off:off + 12])
        off += 12
        out.checksum = struct.unpack("<i", buf[CHECKSUM_OFFSET:CHECKSUM_OFFSET + 4])[0]
        return out


# ---------------------------------------------------------------------------
# Checksum — port of lbBackupCreateChecksum from src/lb/lbbackup.c.
# ---------------------------------------------------------------------------

def compute_checksum(body_bytes: bytes) -> int:
    """Sum each byte multiplied by (1-based index), exactly as the game does.

    The game stores the result in a signed 32-bit slot. We mask to 32 bits
    and sign-correct so the value matches what the game wrote on disk.
    """
    assert len(body_bytes) == CHECKSUM_OFFSET, len(body_bytes)
    cksum = 0
    for i, b in enumerate(body_bytes):
        cksum = (cksum + b * (i + 1)) & 0xFFFFFFFF
    if cksum & 0x80000000:
        cksum -= 0x100000000
    return cksum


# ---------------------------------------------------------------------------
# Default save-file path — mirrors port_save.cpp's resolveSavePath().
# ---------------------------------------------------------------------------

_PROJECT_ROOT = Path(__file__).resolve().parent.parent


def _is_portable_build() -> bool:
    """Return True if the local CMake build was configured with
    NON_PORTABLE=OFF — in which case the running binary writes
    `ssb64_save.bin` to its cwd (the project root, when launched as
    `build/BattleShip` from the repo) instead of the OS app-data dir.

    This is the default for dev/in-tree CMake configures and is what
    catches people out: the python script wrote to the macOS app-data
    dir while the binary read from the repo root.
    """
    cache = _PROJECT_ROOT / "build" / "CMakeCache.txt"
    if not cache.exists():
        return False
    try:
        for line in cache.read_text(errors="replace").splitlines():
            line = line.strip()
            if line.startswith("NON_PORTABLE:") and "=" in line:
                return line.split("=", 1)[1].strip().upper() == "OFF"
    except OSError:
        return False
    return False


def default_save_path() -> Path:
    """Resolve the save file path with the same precedence the running
    game uses (port_save.cpp::resolveSavePath):

      1. $SSB64_SAVE_PATH explicit override
      2. $SHIP_HOME (macOS / Linux only — matches LUS Context behavior)
      3. Project root if the local CMake build is NON_PORTABLE=OFF —
         the dev-checkout default. The binary writes `./ssb64_save.bin`
         relative to cwd, which is the repo root when launched as
         `build/BattleShip` from there.
      4. NON_PORTABLE app-data dir (the default for release bundles).
    """
    env = os.environ.get("SSB64_SAVE_PATH")
    if env:
        return Path(env).expanduser()

    if sys.platform != "win32":
        ship_home = os.environ.get("SHIP_HOME")
        if ship_home:
            return Path(ship_home).expanduser() / "ssb64_save.bin"

    if _is_portable_build():
        return _PROJECT_ROOT / "ssb64_save.bin"

    if sys.platform == "darwin":
        base = Path.home() / "Library" / "Application Support" / "BattleShip"
    elif sys.platform.startswith("win"):
        appdata = os.environ.get("APPDATA")
        base = Path(appdata) / "BattleShip" if appdata else Path.cwd()
    else:
        xdg = os.environ.get("XDG_DATA_HOME")
        base = Path(xdg) / "BattleShip" if xdg else Path.home() / ".local" / "share" / "BattleShip"
    return base / "ssb64_save.bin"


# ---------------------------------------------------------------------------
# I/O — read both slots, write both slots, pad the whole 32 KiB.
# ---------------------------------------------------------------------------

def read_save(path: Path) -> Optional[BackupData]:
    if not path.exists():
        return None
    raw = path.read_bytes()
    # Try slot A first, then slot B. Mirror lbBackupIsSramValid logic.
    for slot in (SLOT_A_OFFSET, SLOT_B_OFFSET):
        if len(raw) < slot + BACKUP_SIZE:
            continue
        try:
            data = BackupData.unpack(raw[slot:slot + BACKUP_SIZE])
        except Exception:
            continue
        body = raw[slot:slot + CHECKSUM_OFFSET]
        if compute_checksum(body) == data.checksum and data.signature == SIGNATURE:
            return data
    # Fall back to slot A even if checksum is bad — useful for forensics.
    if len(raw) >= BACKUP_SIZE:
        return BackupData.unpack(raw[:BACKUP_SIZE])
    return None


def write_save(path: Path, data: BackupData) -> None:
    blob = data.pack()
    # Build the full 32 KiB SRAM image with both slots populated.
    out = bytearray(SRAM_FILE_SIZE)
    out[SLOT_A_OFFSET:SLOT_A_OFFSET + BACKUP_SIZE] = blob
    out[SLOT_B_OFFSET:SLOT_B_OFFSET + BACKUP_SIZE] = blob

    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(bytes(out))


# ---------------------------------------------------------------------------
# Mutators driven by CLI flags.
# ---------------------------------------------------------------------------

def apply_unlock_chars(d: BackupData) -> None:
    d.unlock_mask |= UNLOCK_MASK_NEWCOMERS
    d.fighter_mask |= FIGHTER_MASK_UNLOCK


def apply_unlock_inishie(d: BackupData) -> None:
    d.unlock_mask |= 1 << UNLOCK_BITS["inishie"]


def apply_unlock_soundtest(d: BackupData) -> None:
    d.unlock_mask |= 1 << UNLOCK_BITS["soundtest"]


def apply_unlock_itemswitch(d: BackupData) -> None:
    d.unlock_mask |= 1 << UNLOCK_BITS["itemswitch"]


def apply_unlock_all_chars_and_stages(d: BackupData) -> None:
    d.unlock_mask |= UNLOCK_MASK_ALL
    d.fighter_mask |= FIGHTER_MASK_UNLOCK
    d.ground_mask = GROUND_MASK_ALL


def apply_complete_1p(d: BackupData, difficulty: int = DIFFICULTY_VERY_HARD,
                      hiscore: int = 999_999) -> None:
    for r in d.spgame_records:
        r.is_spgame_complete = 1
        r.spgame_best_difficulty = max(r.spgame_best_difficulty, difficulty)
        r.spgame_hiscore = max(r.spgame_hiscore, hiscore)


def apply_perfect_btt(d: BackupData, time_ticks: int = 30 * TIME_SEC) -> None:
    for r in d.spgame_records:
        r.bonus1_task_count = BONUS_TASK_MAX
        if r.bonus1_time == DEFAULT_BONUS_TIME or r.bonus1_time > time_ticks:
            r.bonus1_time = time_ticks


def apply_perfect_btp(d: BackupData, time_ticks: int = 30 * TIME_SEC) -> None:
    for r in d.spgame_records:
        r.bonus2_task_count = BONUS_TASK_MAX
        if r.bonus2_time == DEFAULT_BONUS_TIME or r.bonus2_time > time_ticks:
            r.bonus2_time = time_ticks


def apply_vs_fill(
    d: BackupData,
    ko_each: int = 25,
    time_seconds: int = 3600,
    damage_given: int = 50_000,
    damage_taken: int = 40_000,
    selfdestructs: int = 5,
    games_played: int = 50,
    avg_player_count: int = 2,
    total_battles: Optional[int] = None,
) -> None:
    """Populate every fighter's VS record with non-zero stats so the
    Character Data screen shows actual numbers (a fresh save displays
    all zeros which looks identical to "never played")."""
    for i, r in enumerate(d.vs_records):
        # Pairwise KOs — fighter i has KO'd every other fighter ko_each times.
        # ko_count[i] (self) stays 0 — you can't KO yourself, that's an SD.
        r.ko_count = [ko_each if k != i else 0 for k in range(NUM_FIGHTERS)]
        r.time_used     = time_seconds
        r.damage_given  = damage_given
        r.damage_taken  = damage_taken
        r.selfdestructs = selfdestructs
        r.games_played  = games_played
        r.player_count_tally = games_played * avg_player_count
        r.player_count_tallies = [games_played * avg_player_count] * NUM_FIGHTERS
        r.played_against       = [games_played] * NUM_FIGHTERS
    if total_battles is not None:
        d.vs_total_battles = total_battles & 0xFFFF
    elif d.vs_total_battles == 0:
        # Game-wide total = sum of per-fighter games_played, but that double-
        # counts (each match increments every participant's slot). Pick a
        # reasonable scalar so the menu's "Total VS battles" line is non-zero.
        d.vs_total_battles = min(games_played * NUM_FIGHTERS // avg_player_count,
                                 0xFFFF)


def fresh_default() -> BackupData:
    """Return a BackupData identical to the in-game default (`dSCManagerDefaultBackupData`)."""
    return BackupData()  # all dataclass defaults already match


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------

def cmd_dump(args: argparse.Namespace) -> int:
    path = Path(args.file) if args.file else default_save_path()
    data = read_save(path)
    if data is None:
        print(f"No save file at: {path}")
        return 1
    print(f"Save file: {path}")
    print(f"  signature        : 0x{data.signature:X}  (expected 0x{SIGNATURE:X})")
    print(f"  stored checksum  : {data.checksum}")
    body = data.pack()[:CHECKSUM_OFFSET]
    expected = compute_checksum(body)
    print(f"  computed checksum: {expected}  ({'OK' if expected == data.checksum else 'BAD'})")
    print(f"  boot count       : {data.boot}")
    print(f"  unlock_mask      : 0x{data.unlock_mask:02X}")
    for name, bit in UNLOCK_BITS.items():
        on = "✓" if data.unlock_mask & (1 << bit) else " "
        print(f"      [{on}] {name}")
    print(f"  fighter_mask     : 0x{data.fighter_mask:04X}")
    for i, name in enumerate(FIGHTERS):
        on = "✓" if data.fighter_mask & (1 << i) else (
            "·" if name in {"Mario", "Fox", "DonkeyKong", "Samus", "Link",
                            "Yoshi", "Kirby", "Pikachu"} else " ")
        print(f"      [{on}] {name}")
    print(f"  ground_mask      : 0x{data.ground_mask:04X}")
    for i, name in enumerate(STAGES):
        on = "✓" if data.ground_mask & (1 << i) else " "
        print(f"      [{on}] {name}")
    print(f"  vs_total_battles : {data.vs_total_battles}")
    print(f"  vs_itemswitch    : {data.vs_itemswitch_battles}")
    print(f"  spgame_difficulty: {data.spgame_difficulty}")
    print(f"  spgame_stocks    : {data.spgame_stock_count}")
    print(f"  sound mode       : {'stereo' if data.sound_mono_or_stereo else 'mono'}")
    print(f"  screen flash on  : {bool(data.is_allow_screenflash)}")
    print(f"  screen offset    : ({data.screen_adjust_h}, {data.screen_adjust_v})")
    print(f"  error_flags      : 0x{data.error_flags:02X}")
    print(f"  characters_fkind : {data.characters_fkind} ({FIGHTERS[data.characters_fkind] if data.characters_fkind < NUM_FIGHTERS else '?'})")
    print()
    print("  1P records:")
    print(f"  {'fighter':<14} {'cleared':>7} {'best-diff':>9} {'hiscore':>8} "
          f"{'BTT(tics)':>9} {'BTT-tasks':>9} {'BTP(tics)':>9} {'BTP-tasks':>9}")
    for i, r in enumerate(data.spgame_records):
        print(f"  {FIGHTERS[i]:<14} {r.is_spgame_complete:>7} "
              f"{r.spgame_best_difficulty:>9} {r.spgame_hiscore:>8} "
              f"{r.bonus1_time:>9} {r.bonus1_task_count:>9} "
              f"{r.bonus2_time:>9} {r.bonus2_task_count:>9}")
    return 0


def cmd_write(args: argparse.Namespace) -> int:
    path = Path(args.file) if args.file else default_save_path()

    if args.from_existing and path.exists():
        data = read_save(path) or fresh_default()
    else:
        data = fresh_default()

    if args.reset:
        data = fresh_default()

    if args.all:
        args.unlock_chars = True
        args.unlock_stages = True
        args.item_switch = True
        args.sound_test = True
        args.complete_1p = True
        args.perfect_btt = True
        args.perfect_btp = True
        args.vs_fill = True
        # ground_mask records unique VS stages played; max it so the save
        # also reflects "every stage has been played at least once."
        if args.ground_mask is None:
            data.ground_mask = GROUND_MASK_ALL

    if args.unlock_chars:
        apply_unlock_chars(data)
    if args.unlock_stages:
        apply_unlock_inishie(data)
    if args.sound_test:
        apply_unlock_soundtest(data)
    if args.item_switch:
        apply_unlock_itemswitch(data)
    if args.complete_1p:
        apply_complete_1p(data, difficulty=args.completion_difficulty,
                          hiscore=args.completion_hiscore)
    if args.perfect_btt:
        apply_perfect_btt(data, time_ticks=args.perfect_time_ticks)
    if args.perfect_btp:
        apply_perfect_btp(data, time_ticks=args.perfect_time_ticks)
    if args.vs_fill:
        apply_vs_fill(
            data,
            ko_each=args.vs_ko_each,
            time_seconds=args.vs_time_seconds,
            damage_given=args.vs_damage_given,
            damage_taken=args.vs_damage_taken,
            selfdestructs=args.vs_selfdestructs,
            games_played=args.vs_games_played,
            avg_player_count=args.vs_avg_players,
        )

    if args.unlock_mask is not None:
        data.unlock_mask = parse_int(args.unlock_mask) & 0xFF
    if args.fighter_mask is not None:
        data.fighter_mask = parse_int(args.fighter_mask) & 0xFFFF
    if args.ground_mask is not None:
        data.ground_mask = parse_int(args.ground_mask) & 0xFFFF

    if args.boot is not None:
        data.boot = args.boot & 0xFF
    if args.error_flags is not None:
        data.error_flags = parse_int(args.error_flags) & 0xFF

    if args.sound_mode is not None:
        data.sound_mono_or_stereo = 1 if args.sound_mode == "stereo" else 0
    if args.screen_flash is not None:
        data.is_allow_screenflash = 1 if args.screen_flash else 0
    if args.screen_h is not None:
        data.screen_adjust_h = args.screen_h
    if args.screen_v is not None:
        data.screen_adjust_v = args.screen_v

    if args.last_difficulty is not None:
        data.spgame_difficulty = args.last_difficulty & 0xFF
    if args.last_stocks is not None:
        data.spgame_stock_count = args.last_stocks & 0xFF
    if args.last_character is not None:
        data.characters_fkind = resolve_fighter(args.last_character) & 0xFF

    if args.vs_total_battles is not None:
        data.vs_total_battles = args.vs_total_battles & 0xFFFF
    if args.vs_itemswitch_battles is not None:
        data.vs_itemswitch_battles = args.vs_itemswitch_battles & 0xFF

    write_save(path, data)
    print(f"Wrote save to: {path}")
    print(f"  unlock_mask  = 0x{data.unlock_mask:02X}")
    print(f"  fighter_mask = 0x{data.fighter_mask:04X}")
    print(f"  ground_mask  = 0x{data.ground_mask:04X}")
    print(f"  signature    = 0x{data.signature:X}")
    print(f"  checksum     = {compute_checksum(data.pack()[:CHECKSUM_OFFSET])}")
    return 0


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

def parse_int(s) -> int:
    if isinstance(s, int):
        return s
    s = str(s).strip()
    base = 16 if s.lower().startswith("0x") else 10
    return int(s, base)


def resolve_fighter(name) -> int:
    if isinstance(name, int):
        return name
    s = str(name).strip().lower()
    aliases = {
        "mario": 0, "fox": 1, "donkey": 2, "donkeykong": 2, "dk": 2,
        "samus": 3, "luigi": 4, "link": 5, "yoshi": 6,
        "captain": 7, "captainfalcon": 7, "falcon": 7,
        "kirby": 8, "pikachu": 9, "purin": 10, "jigglypuff": 10, "jiggly": 10,
        "ness": 11,
    }
    if s in aliases:
        return aliases[s]
    return parse_int(s)


def build_arg_parser() -> argparse.ArgumentParser:
    p = argparse.ArgumentParser(
        description="SSB64 PC port save game editor.",
        formatter_class=argparse.RawDescriptionHelpFormatter,
    )
    p.add_argument("--file", help="Override path to ssb64_save.bin (default: SDL pref dir).")
    sub = p.add_subparsers(dest="cmd", required=True)

    pdump = sub.add_parser("dump", help="Print the contents of an existing save.")
    pdump.set_defaults(func=cmd_dump)

    pw = sub.add_parser("write", help="Generate / mutate a save.")
    pw.set_defaults(func=cmd_write)

    pw.add_argument("--reset", action="store_true",
                    help="Start from a fresh-default save (default if no existing file).")
    pw.add_argument("--from-existing", action="store_true",
                    help="Start from the existing save on disk if present.")
    pw.add_argument("--all", action="store_true",
                    help="Set every unlock + completion flag (every char, stage, "
                         "every option, every 1P record perfect at Very Hard).")

    g = pw.add_argument_group("unlock toggles")
    g.add_argument("--unlock-chars", action="store_true",
                   help="Unlock Luigi, Ness, Captain Falcon, Jigglypuff.")
    g.add_argument("--unlock-stages", action="store_true",
                   help="Unlock Mushroom Kingdom (Inishie).")
    g.add_argument("--sound-test", action="store_true",
                   help="Unlock the Sound Test menu.")
    g.add_argument("--item-switch", action="store_true",
                   help="Unlock the Item Switch menu.")

    g = pw.add_argument_group("1P / bonus records")
    g.add_argument("--complete-1p", action="store_true",
                   help="Mark every fighter's 1P Game as cleared.")
    g.add_argument("--completion-difficulty", type=int, default=DIFFICULTY_VERY_HARD,
                   help="Best difficulty stamped on each 1P record (0..4). "
                        "Default 4 (Very Hard).")
    g.add_argument("--completion-hiscore", type=int, default=999_999,
                   help="High score stamped on each 1P record. Default 999999.")
    g.add_argument("--perfect-btt", action="store_true",
                   help="Set Break the Targets to 10 targets broken with --perfect-time-ticks.")
    g.add_argument("--perfect-btp", action="store_true",
                   help="Set Board the Platforms to 10 platforms with --perfect-time-ticks.")
    g.add_argument("--perfect-time-ticks", type=int, default=30 * TIME_SEC,
                   help=f"Time used by --perfect-btt/-btp. 60 ticks/sec. "
                        f"Default {30 * TIME_SEC} (= 30s).")

    g = pw.add_argument_group("raw mask overrides (advanced)")
    g.add_argument("--unlock-mask", help="Force unlock_mask to this value (hex or dec).")
    g.add_argument("--fighter-mask", help="Force fighter_mask to this value.")
    g.add_argument("--ground-mask", help="Force ground_mask to this value.")
    g.add_argument("--error-flags", help="Force error_flags to this value (anti-piracy bits).")
    g.add_argument("--boot", type=int, help="Set boot count.")

    g = pw.add_argument_group("display / audio options")
    g.add_argument("--sound-mode", choices=["mono", "stereo"], help="Set sound output mode.")
    g.add_argument("--screen-flash", type=int, choices=[0, 1],
                   help="0 disables damage screen flash, 1 enables it.")
    g.add_argument("--screen-h", type=int, help="Horizontal screen-center offset (signed).")
    g.add_argument("--screen-v", type=int, help="Vertical screen-center offset (signed).")

    g = pw.add_argument_group("menu state")
    g.add_argument("--last-character", help=f"FTKind index OR name {FIGHTERS}.")
    g.add_argument("--last-difficulty", type=int,
                   help="0=VeryEasy 1=Easy 2=Normal 3=Hard 4=VeryHard.")
    g.add_argument("--last-stocks", type=int, help="Last 1P stock count selected.")

    g = pw.add_argument_group("VS counters")
    g.add_argument("--vs-total-battles", type=int, help="Total VS games played.")
    g.add_argument("--vs-itemswitch-battles", type=int,
                   help="VS games counted toward Item Switch unlock.")

    g = pw.add_argument_group("VS records (per-fighter Character Data stats)")
    g.add_argument("--vs-fill", action="store_true",
                   help="Populate every fighter's VS record with non-zero stats "
                        "so the Character Data screen shows actual numbers. "
                        "Implied by --all.")
    g.add_argument("--vs-ko-each", type=int, default=25,
                   help="KOs landed by each fighter on each opponent (default 25).")
    g.add_argument("--vs-time-seconds", type=int, default=3600,
                   help="Per-fighter time used (seconds; default 3600 = 1h).")
    g.add_argument("--vs-damage-given", type=int, default=50_000,
                   help="Per-fighter damage dealt (default 50000).")
    g.add_argument("--vs-damage-taken", type=int, default=40_000,
                   help="Per-fighter damage taken (default 40000).")
    g.add_argument("--vs-selfdestructs", type=int, default=5,
                   help="Per-fighter self-destruct count (default 5).")
    g.add_argument("--vs-games-played", type=int, default=50,
                   help="Per-fighter games-played count (default 50).")
    g.add_argument("--vs-avg-players", type=int, default=2,
                   help="Avg player count per match, used to fill the "
                        "running tallies (default 2).")

    return p


def main(argv: Optional[List[str]] = None) -> int:
    p = build_arg_parser()
    args = p.parse_args(argv)
    return args.func(args)


if __name__ == "__main__":
    sys.exit(main())
