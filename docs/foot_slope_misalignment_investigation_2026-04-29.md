# Character Foot Misalignment on Slopes — Investigation Handoff (2026-04-29)

**Issue:** [#5 Character foot misalignment on sloped surfaces](https://github.com/JRickey/BattleShip/issues/5) —
Kirby/Samus/DK (and likely other characters) don't rotate their bodies
to follow slope normals. Result: the character renders as if standing
flat, with one foot floating above the slope and the other clipping
into it.

**Status:** Fixed later on 2026-04-29. See
`docs/bugs/fighter_slope_contour_lp64_alias_2026-04-29.md`.

The root cause was not bad floor normals. It was a port-only LP64 alias
bug in the fighter foot IK path: `func_ovl2_800EBC0C` read a cached
`FTParts` transform vector through an N64-only `FTParts*`-as-`DObj*`
layout pun. On 64-bit hosts, `DObj::rotate.vec.f` moved away from the
intended `FTParts::unk_dobjtrans_0x10[2]` row, so the slope-contour
solver used the wrong orientation.

## Reproduction signal

Reporter close-ups show Kirby on Peach's Castle's central pyramid roof
(a clear slope). On N64 / GLideN64 reference, Kirby's body axis
matches the slope normal; on the port the body axis stays vertical and
the feet plant on a horizontal Y plane below the slope contour.

The same bug is mentioned for Samus and DK — i.e. it's not character-
specific, it's the slope-attachment math. Whichever character
*doesn't* show it (the reporter calls out Link as the BTT-arrow
counterexample on a different issue, not this one) probably has a
different ECB / hurtbox shape that masks the rendering offset.

## Where to look

This is a **physics / fighter-positioning** bug, not a rendering one.
Likely candidates:

1. `src/ft/ftmain.c` — fighter ground attachment / hurtbox / ground
   contact math. Anywhere a Vec3f normal feeds a rotation matrix
   that tilts the model.
2. `src/mp/mpcommon.c` — map-collision side. `MPObjectColl` and
   slope-line intersection math (computes the `ground_angle` /
   `surface_normal` consumed by the fighter side).
3. `src/ft/ftparam.c` — fighter parameter init for slopes. Some
   fighters disable slope tilt via attribute flags.

## Why I'm not patching blindly

Slope math is the kind of code where a one-byte sign flip or a wrong
coordinate-space conversion produces *exactly* the symptom in the
screenshots — but the same symptom is also produced by a wrong
`MPVertex` byte-swap, a missed `MPGroundData` field, or a
fighter-attribute bitfield read off the wrong offset.

We've shipped fixes for several of those classes already
(`mpvertex_byte_swap_2026-04-11`, `ground_geometry_collision_2026-04-08`,
`mpgrounddata_layer_mask_u8_byteswap_2026-04-22`,
`itattributes_rom_layout_rewrite_2026-04-24`). This is plausibly a
sibling of those — an `FTAttributes` slope-related field at a wrong
offset, an `MPGroundData` slope-normal field that pass1 BSWAP corrupts,
or a slope-line `Vec3f` whose halfswap state isn't handled.

To validate: dump the per-frame `surface_normal` / `ground_angle`
values written into the fighter struct against an N64 trace. If the
port is reading 0/identity, the source data is the problem. If it's
reading sane values but the model tilt isn't applied, the bug is in
the rotation-matrix pipeline (ftDisplay or one of the matrix list
functions).

## What the reporter has

- Kirby-only `ssb64.log`. No GBI trace.

## Caveat

This is potentially a multi-day investigation. Treating it as out of
scope for the current "sweep through reported issues" pass.
