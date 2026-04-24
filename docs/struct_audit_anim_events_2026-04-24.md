# Animation/Event Bytecode Struct Audit — 2026-04-24

Audit of bitfield-bearing structs in `src/sys/objtypes.h`, `src/gm/gmscript.h`, and
`src/gm/gmtypes.h` against IDO-emitted bit layouts. Methodology follows
`docs/struct_rom_layout_audit_handoff_2026-04-24.md`: disassemble the reader
function in `baserom.us.z64`, match MIPS `lw`/`sll`/`srl`/`andi` patterns to
physical bit positions, compare to the port's `#else` (LE) declaration order.

**Headline:** No bit-layout mismatches detected in any of the three files.
Every bitfield group I checked reads with IDO emitting shifts that match the
port's LSB-first LE declaration. A handful of structs are flagged "not
audited — needs follow-up" at the end because they live on the rumble/Rumble
path (no reader VRAM located within the time budget) or are trivially
non-bitfield.

Known halfswap/fixup interaction is preserved: nothing in this audit
contradicts `project_aobjevent32_halfswap.md`. If animations are still
decoding wrong, the cause is load-time data corruption (halfswap, BSWAP, pad
handling in `portFixupStructU*`), not struct layout.

---

## `src/sys/objtypes.h`

Two `IS_BIG_ENDIAN` branches: `AObjEvent16`, `AObjEvent32`. `MObjSub` has no
bitfields but is included because the prompt called it out.

### `AObjEvent16` — OK

Used for fighter figatree animation streams (half-size AObj events). Layout
is a 16-bit storage unit with three packed fields.

Reader: `ftAnimParseDObjFigatree` at **VRAM 0x800EC238** (from
`references/ssb-decomp-re/symbols/symbols_us.txt`). Confirmed by source at
`src/ft/ftanim.c:107–125` dispatching on `event16->command.opcode`.

IDO extraction (disasm of 0x800EC3CC onward, after `lhu $a0, 0($s2)`):

| Asm | Field | Physical bits | Width | Signed |
|---|---|---|---|---|
| `lhu; srl 11` | opcode | [15:11] | 5 | u |
| `lhu; sll 21; srl 22` | flags | [10:1] | 10 | u |
| `lhu; andi 0x1` | toggle | [0] | 1 | u |

Port LE declaration (LSB-first): `toggle:1, flags:10, opcode:5` → toggle[0],
flags[10:1], opcode[15:11]. **MATCH.**

### `AObjEvent32` — OK

Standard AObj event stream, 32-bit storage. Used by all objects (DObj, MObj,
CObj) except fighter joint anims.

Reader: `gcParseDObjAnimJoint` at **VRAM 0x8000BFE8**. Source at
`src/sys/objanim.c:268–328` (first opcode dispatch `command_kind = dobj->anim_joint.event32->command.opcode`).
Additional readers with identical extraction pattern: `gcParseMObjMatAnimJoint`
(0x8000CF6C), `gcParseCObjCamAnimJoint` (0x8000FA74), `gcGetAnimTotalLength`
(`objanim.c:2923`).

IDO extraction (disasm of 0x8000C180 onward, after `lw $s0, 0($v1)`):

| Asm | Field | Physical bits | Width | Signed |
|---|---|---|---|---|
| `lw; srl 25` | opcode | [31:25] | 7 | u |
| `lw; sll 7; srl 22` | flags | [24:15] | 10 | u |
| `lw; andi 0x7FFF` | payload | [14:0] | 15 | u |

Port LE declaration (LSB-first): `payload:15, flags:10, opcode:7` →
payload[14:0], flags[24:15], opcode[31:25]. **MATCH.**

**Halfswap interaction note.** `port/port_aobj_fixup.cpp` un-halfswaps the
stream at load time (walks by command, advancing by opcode-specific payload
width). The walker's advance logic depends on these exact bit widths; since
the layout matches IDO, the walker's arithmetic is sound. If a new animation
bug surfaces with zeroed or garbage fields, check the halfswap walker first
(wrong advance = misaligned word = every subsequent opcode reads garbage),
not this struct.

### `MObjSub` — OK (not a bitfield struct)

Plain 0x78-byte sprite material descriptor — `_Static_assert(sizeof(MObjSub)
== 0x78)` holds. Field offsets hand-verified against consumers at
`src/sys/objdisplay.c:1237–1340` (reads `mobj->sub.flags`, `scau/scav`,
`trau/trav`, `primcolor.s.r`, etc. — all at the expected byte offsets) and
the copy-ctor `gcAddMObjForDObj` at `src/sys/objman.c:1302`. No bitfields, no
packing pitfalls. Pass1 BSWAP32 + appropriate `portFixupStructU16` over the
u16 fields (pad00, unk08/0A/0C/0E, flags, block_dxt, unk36/38/3A) is
sufficient — no audit concern.

---

## `src/gm/gmscript.h`

11 `IS_BIG_ENDIAN` branches across the `GMColEvent*` family. Note:
**`gmscript.h` and `gmtypes.h` redeclare the same structs** (color-animation
bytecode) and differ only at the trivia level (`GMColEventDefault.value1` vs
`.value`, union tag order). The reader resolves both to the same raw u32. I
audited the layouts once (via `gmtypes.h`'s reader) and the analysis applies
to both headers.

Reader for every `GMColEvent*` variant: `ftMainUpdateColAnim` at **VRAM
0x800E0880** (source at `src/ft/ftmain.c:960–1200`). Dispatches on
`gmColEventCast(p_script, GMColEventDefault)->opcode`; jump-table branches
to a case per opcode that extracts the command-specific fields from the same
raw u32.

### `GMColEventDefault` / `LoopBegin` / `BlendRGBA1` / `PlayFGM` — OK

All four share the same layout: `opcode:6 + value/loop_count/blend_frames/sfx_id/value1:26`.

IDO extraction (from the dispatcher at 0x800E0908):

| Asm | Field | Physical bits | Width |
|---|---|---|---|
| `lw; srl 26` | opcode | [31:26] | 6 |
| `lw; and 0x03FFFFFF` | value/payload | [25:0] | 26 |

Port LE declaration (LSB-first): `value:26, opcode:6` → value[25:0],
opcode[31:26]. **MATCH** for all four variants.

### `GMColEventSetRGBA2` / `BlendRGBA2` — OK

Four plain u8 fields packed into a u32. IDO emits four `lbu` reads at byte
offsets 0, 1, 2, 3 of the struct — trivial byte-level layout, no bitfield
packing involved. Port LE declares `a:8, b:8, g:8, r:8` which LSB-first
places `a` at [7:0] (byte 3 on BE), `b` at [15:8] (byte 2), `g` at [23:16]
(byte 1), `r` at [31:24] (byte 0). BE declaration has `r,g,b,a` in
MSB-first order which lands `r` at bytes 0, `g` at 1, etc. **MATCH.**

### `GMColEventMakeEffect1` — OK

Most densely packed struct in the file: `opcode:6, joint_id:7 (signed),
effect_id:9, flag:10`.

Reader extraction (0x800E0EA0 onward, for the MakeEffect opcode case):

| Asm | Field | Physical bits | Width | Signed |
|---|---|---|---|---|
| `lw; srl 26` (via dispatcher) | opcode | [31:26] | 6 | u |
| `lw; sll 6; sra 25` | joint_id | [25:19] | 7 | **s** |
| `lw; sll 13; srl 23` | effect_id | [18:10] | 9 | u |
| `lw; andi 0x3FF` | flag | [9:0] | 10 | u |

Port LE declaration (LSB-first): `flag:10, effect_id:9, joint_id:7,
opcode:6` → flag[9:0], effect_id[18:10], joint_id[25:19], opcode[31:26].
**MATCH.**

Note the port declares `u32 joint_id:7` (not `s32`) with a code-site
`BITFIELD_SEXT(x, 7)` at reads — IDO does the sign-extension via `sra 25`,
so the semantics match. Verify consumers use `BITFIELD_SEXT(.joint_id, 7)`
at every read site.

### `GMColEventMakeEffect2` / `3` / `4` — OK

Three u32 structs each holding two `s16 : 16` fields (`off_x/off_y`,
`off_z/rng_x`, `rng_y/rng_z`). IDO emits plain `lh $t, 0($p)` and `lh $t,
2($p)` — standard s16 loads, sign-extended. No bitfield packing; layout is
just two aligned signed halfwords. **MATCH** (the LE branch is really just
for consistency — these could be plain `s16` members).

### `GMColEventSetLight` — OK

`opcode:6, light1:13 (signed), light2:13 (signed)`. Extracted at
0x800E0FD0:

| Asm | Field | Physical bits | Width | Signed |
|---|---|---|---|---|
| `lw; sll 6; sra 19` | light1 | [25:13] | 13 | **s** |
| `lw; sll 19; sra 19` | light2 | [12:0] | 13 | **s** |

Port LE declaration: `u32 light2:13, u32 light1:13, u32 opcode:6` →
light2[12:0], light1[25:13], opcode[31:26]. **MATCH.** Port stores unsigned
with manual `BITFIELD_SEXT13()` at read sites; IDO does it inline via
`sra 19`. Semantically equivalent.

### `GMColEventGoto1/2`, `Subroutine1/2`, `Parallel1/2` — not bitfield-critical

Each is an `opcode:6` dispatcher struct followed by a raw
pointer/PORT-token struct. Opcode extraction is the same top-6-bit pattern
as `Default`. Pointer struct is a plain `void*` / `u32 p_*` token — no bitfields.
**OK** by trivial inspection.

### `GMRumbleEventDefault` (gmscript.h) — not audited

`u16 opcode:3, u16 param:13`. No `IS_BIG_ENDIAN` branch in the gmscript.h
version (only the gmtypes.h copy has a branch). The port declaration in
gmscript.h matches the BE layout by default since it's positional, so any
clang compile on LE will pack LSB-first: param[12:0], opcode[15:13]. IDO
MSB-first would put opcode[15:13], param[12:0]. **Reader VRAM not located
within time budget**; flagging as needs-follow-up. Impact limited to rumble
pak, which many PC-port users don't exercise.

---

## `src/gm/gmtypes.h`

11 `IS_BIG_ENDIAN` branches. Ten of these are `GMColEvent*` duplicates of
`gmscript.h` structs (same layouts, same reader `ftMainUpdateColAnim` —
covered above). The unique one is **GMStatFlags**.

### `GMStatFlags` — OK

Attack/status flag union, 16-bit storage embedded in `FTStruct.stat_flags`
(also in `FTStruct.damage_stat_flags`, `ITAttackColl.stat_flags`,
`WPAttackColl.stat_flags`, `ITStruct.reflect_stat_flags`, `WPStruct.reflect_stat_flags`).

Reader: `ftParamUpdate1PGameAttackStats` at **VRAM 0x800EA7B0** (source at
`src/ft/ftparam.c:1687`). Picked this because it reads all four relevant
fields in sequence against the same `fp->stat_flags` (offset 0x28C in
FTStruct on BE).

IDO loads the u16 `stat_flags` and the adjacent u16 `stat_count` with a
single `lw $v0, 0x28C($a0)`. Because `stat_flags` is at byte offset 0x28E
(two bytes after `motion_count`), on BE it ends up in the **low 16 bits**
of the loaded u32. All extractions therefore work on bits [15:0] of that
u32 = bits [15:0] of the u16 stat_flags.

Extraction sequence (0x800EA804 onward, following the C source
`attack_id, is_smash_attack, ga, is_projectile`):

| Asm | Field | Physical bits (in u16) | Width |
|---|---|---|---|
| `lw; andi 0x3FF` | attack_id | [9:0] | 10 |
| `lw; sll 19; srl 31` | is_smash_attack | [12] | 1 |
| `lw; sll 20; srl 31` | ga | [11] | 1 |
| `lw; sll 21; srl 31` | is_projectile | [10] | 1 |

(In the u32 load those shifts land on bits [12], [11], [10] of the u32 —
but since stat_flags occupies low 16 of the u32, the bits inside the u16
are identical: [12], [11], [10].)

Port LE declaration (LSB-first):
```
u16 attack_id:10, ub16 is_projectile:1, ub16 ga:1, ub16 is_smash_attack:1, u16 unused:3
```
→ attack_id[9:0], is_projectile[10], ga[11], is_smash_attack[12],
unused[15:13]. **MATCH.**

The decomp BE declaration (MSB-first) reads
`unused:3, is_smash_attack:1, ga:1, is_projectile:1, attack_id:10` which
places `unused[15:13], is_smash[12], ga[11], is_projectile[10],
attack_id[9:0]` — also consistent with IDO.

**Suspicious u32 words:** none. The union halfword overlay
(`u16 halfword`) is used in
`ftMainSetFighterRelease` and similar to pass the full packed value by
value via `*(GMStatFlags*)&flags`. This is a u16 ABI plumbing detail, not a
bit-layout concern. The `lbu` at offset 0x4AE3 and similar in the disasm
are reads of `SCManager` scene state, not stat_flags.

### `GMRumbleEventDefault` (gmtypes.h) — not audited

Same caveat as the gmscript.h copy. Reader not located. `u16 opcode:3, u16
param:13`; port LE declares `param:13, opcode:3` which LSB-first gives
param[12:0], opcode[15:13] — would match IDO MSB-first. **Needs
follow-up** to disassemble an actual rumble script consumer (likely in
`src/gm/` rumble manager or `gmrumble.c` — not obvious from the symbol
map).

---

## Not audited — needs follow-up

- **`GMRumbleEventDefault`** (both copies). Reader VRAM not identified.
  Low impact (rumble-only), but worth checking with a half-hour of
  disassembly against whatever function walks `gmRumbleEvent.p_script`.
- **MObjSub BE-specific pad/flag placement near offset 0x30–0x3B.** Several
  `u16 unk36/unk38/unk3A/block_dxt` fields are adjacent u16s; if pass1
  BSWAP32 doesn't cover them correctly, individual fields will swap
  pairwise. No mismatch found here in the bitfield sense but if sprite
  animation tearing resurfaces, confirm the fixup list in
  `port/port_mobjsub_fixup.*` (if it exists) covers these offsets.

---

## Interaction with known halfswap fixups

`AObjEvent32` is the only struct in this audit touched by
`project_aobjevent32_halfswap.md`. The halfswap walker at
`port/port_aobj_fixup.cpp` advances opcode-by-opcode, relying on the
payload/flags widths verified above. Since those widths match IDO, the
walker is correct **for the opcodes it knows about**. The `default:` case
in `objanim.c` is a belt-and-suspenders fallback that assumes 32-bit
alignment — still valid.

`AObjEvent16` (figatree) had separate halfswap fixes (see MEMORY
`project_fighter_intro_anim.md`, `project_aobjevent32_halfswap.md`). The
layout is confirmed correct; any residual figatree corruption is a walker
or loader issue, not a struct layout issue.

`GMStatFlags` is a plain u16 with no halfswap interaction. Its
`portFixupStructU16` coverage depends on the enclosing struct
(FTStruct/ITAttackColl/WPAttackColl) — out of scope here.

## Summary table

| Struct | File | Bits? | Reader VRAM | Status |
|---|---|---|---|---|
| AObjEvent16 | objtypes.h | yes | 0x800EC238 | OK |
| AObjEvent32 | objtypes.h | yes | 0x8000BFE8 | OK |
| MObjSub | objtypes.h | no | n/a | OK (plain struct) |
| GMColEventDefault | gmscript.h + gmtypes.h | yes | 0x800E0880 | OK |
| GMColEventLoopBegin | gmscript.h | yes | 0x800E0880 | OK |
| GMColEventSetRGBA2 | both | yes | 0x800E0880 | OK |
| GMColEventBlendRGBA1 | both | yes | 0x800E0880 | OK |
| GMColEventBlendRGBA2 | both | yes | 0x800E0880 | OK |
| GMColEventMakeEffect1 | both | yes | 0x800E0880 | OK |
| GMColEventMakeEffect2/3/4 | both | yes | 0x800E0880 | OK |
| GMColEventSetLight | both | yes | 0x800E0880 | OK |
| GMColEventPlayFGM | gmscript.h | yes | 0x800E0880 | OK |
| GMStatFlags | gmtypes.h | yes | 0x800EA7B0 | OK |
| GMRumbleEventDefault | gmscript.h + gmtypes.h | yes | not located | **not audited** |
