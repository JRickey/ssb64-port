# M3.P15 — Source-Compile Bitfield-Init RelocData Files (Follow-up)

**Status:** Open. Currently covered by passthrough (Torch bytes verbatim) — game runs, but no source-edit path for these files. Modders editing fighter / weapon / ground attributes don't see changes via the source-compile pipeline yet.

**Scope:** 76 files in `decomp/src/relocData/` whose top-level initializers use IDO-style positional bitfield syntax against structs the port has rewritten under `#ifdef PORT` to use explicit `u32` words:

| Struct | Files | Modder use case |
|---|---:|---|
| `FTAttributes` | 27 | Fighter tuning — speed, jump heights, hit knockback |
| `MPGroundData` | 41 | Stage collision / surface properties |
| `WPAttributes` | 8 | Projectile attributes (Mario fireball, Samus missile, etc.) |
| `ITAttributes` | 0 | (rewritten in 2026-04-24, already passthrough-clean) |

## Why passthrough is the wrong long-term answer

Currently each of these files ships in `BattleShip.fromsource.o2r` via `add_passthrough_resource()` — bytes copied verbatim from `BattleShip.o2r`. That works for *running* the game but doesn't enable the modder loop:

```
edit decomp/src/relocData/203_MarioMain.c → cmake --build → see change in-game
```

Edits to a passthrough file are ignored — the source-compile pipeline never touches it. A modder tweaking Mario's `walk_initial_speed` would have no effect; the runtime serves Torch's ROM-extracted bytes.

## The core problem

IDO 7.1 (MIPS BE) packs small bitfields into struct pad gaps **MSB-first**. clang i686 (LE) packs **LSB-first** into the same containing word. The PORT-guarded structs flatten bitfields to a single `u32` and code reads them with explicit shift/mask at the IDO bit positions. So:

| Struct definition | Initializer | Packed u32 layout the runtime expects |
|---|---|---|
| Upstream (IDO bitfield) | `s32 angle:10; s32 init_y:10; ...` | MSB-first: `(angle << 22) \| (init_y << 12) \| ...` |
| PORT (explicit u32) | `u32 packed_word_0x4;` | Code reads `(packed_word_0x4 >> 22) & 0x3FF` for angle |

If we naively compile the upstream `.c` with clang i686, the bitfields pack LSB-first. The runtime extracts at MSB-first positions → wrong values.

## Solution sketch — source-compile pre-processor

Add `tools/pack_bitfield_init.py` that:

1. **Knows each struct's bit layout.** Hand-derived from the upstream struct definition, verified against the audit procedure in `docs/debug_ido_bitfield_layout.md` (compile the upstream `.c` with IDO + rabbitizer-disassemble the resulting `.data` section to confirm exact bit positions per field). Encode as a Python table:
   ```python
   FT_ATTRIBUTES_LAYOUT = {
     "walk_initial_speed":      Field(off=0x0, size=4, kind="f32"),
     "angle":                   Field(off=0x4, bit=22, width=10, kind="s32"),
     "init_jump_y_velocity":    Field(off=0x4, bit=12, width=10, kind="s32"),
     ...
   }
   ```

2. **Parses `.c` bitfield initializers.** Extracts `<TYPE> <name> = { <field>=<value>, ... };` from the file. Tolerates positional inits too — the upstream `.c`s mix designated and positional.

3. **Emits a packed-u32 initializer.** Pre-processor writes a sibling `<name>.packed.c` that contains the same data laid out as the PORT struct expects:
   ```c
   FTAttributes dMarioMainAttributes = {
       0.5f,           /* walk_initial_speed */
       0xb6464000,     /* packed word at +0x04 — MSB-first bit-packed */
       ...
   };
   ```

4. **CMake hooks it before `build_reloc_resource.py`.** New eligibility branch in `tools/gen_reloc_cmake.py`: files in `uses_bitfield_init` route to `add_reloc_resource()` with `--src` pointing at the pre-processor output instead of the original `.c`.

## Implementation order (per struct, biggest leverage first)

1. **FTAttributes (27 files)** — fighter tuning has the highest modder value. Field count ~80 per file; multiple bitfield-packed words.
2. **MPGroundData (41 files)** — large file count but per-file field set is smaller (~10 fields).
3. **WPAttributes (8 files)** — smallest count; projectile attributes.

## Deliverables for P15

- `tools/pack_bitfield_init.py` — pre-processor (~300 LOC).
- `tools/bitfield_layouts/{FTAttributes,WPAttributes,MPGroundData}.py` — per-struct layout tables.
- One-page audit doc per struct in `docs/audit_bitfield_<struct>_2026-MM-DD.md` documenting how each bit position was verified (rabbitizer disasm output snippet).
- `tools/gen_reloc_cmake.py` change: eligibility filter routes bitfield-init files through the pre-processor instead of skipping them.
- Validation: each transitioned file passes `tools/validate_reloc_archives.py` byte-for-byte against Torch's bytes (or documented exception list).

## Verification gate for P15-done

1. All 76 files transition from `add_passthrough_resource` to `add_reloc_resource` in the generated cmake.
2. `tools/validate_reloc_archives.py` reports byte-identical or layout-shifted-but-runtime-equivalent for each.
3. Smoke test: pick one fighter (Mario), edit `walk_initial_speed` from 0.5f to 1.5f in `203_MarioMain.c`, rebuild, confirm in-game Mario walks visibly faster.
4. Smoke test: same for one stage's MPGroundData and one weapon's WPAttributes.

## Why not just port upstream's bitfield-aware extractor?

Upstream doesn't have one — their build is IDO MIPS BE and bitfield positions are whatever IDO emits. The port's PORT-guarded struct rewrites are what created this divergence in the first place. The fix has to live on our side.

## Tracking

Reference memory: `[Prefer struct rewrite over game-logic override](feedback_struct_rewrite_over_overrides.md)` — established that struct-layout rewrites under PORT are the correct answer for cross-LE-target compatibility. P15 is the build-side complement: keep upstream's bitfield-init `.c` files compilable into the PORT layout via a pre-processor, rather than rewriting every initializer by hand.
