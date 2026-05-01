# ITAttackEvent / ITMonsterEvent `size` double-fixup

**Date resolved:** 2026-05-01
**Symptom:** Bob-omb explosion hitbox is "tiny / misses" even though the
walking detection hitbox does its 1% on contact. Players' clip on RMG showed
a 2-hit reading where only the detection hit registered; the explosion
itself never connected. Same class affects every exploding common item
(Capsule, Egg, Mr. Saturn / Motion-Sensor Bomb, Box, Barrel, Marumine
landmine, Link-Bomb) and the ITMonsterEvent-driven explosions (Porygon
tackle, Venusaur SolarBeam).

## Root cause

`ITAttackEvent` is 8 bytes:

```
0x00..0x03  u32  (timer:8 | angle:10 | damage:8 | :6 pad)   bitfield
0x04..0x05  u16  size                                       <-- ground truth
0x06..0x07  2 bytes pad (always 0 in ROM)
```

The original `87c4fe8` (2026-04-20) fixed a sizeof=12 bug where `u8 timer`
got its own storage with 3 bytes of alignment padding. The new struct
sizeof=8 is correct: the LE PORT layout puts `u16 size` at struct offset
**0x06**, not 0x04. That's the right place: after pass1 BSWAP32 of the u32
word at offset 0x04, the original BE bytes `[size_hi, size_lo, 0, 0]` end
up at LE memory `[0, 0, size_lo, size_hi]`, so `u16` read at offset 0x06
yields the original BE numeric size value. **Pass1 BSWAP32 alone is enough.**

The bug: `87c4fe8` carried over a `portFixupStructU16(&ev[i], 0x04, 1)`
call from the prior sizeof=12 commit (where the call had been at offset
0x08, sized for the wrong struct). On struct-fix, the offset was
mechanically retargeted to 0x04 instead of being deleted. The retargeted
call rotates the half-words at offset 0x04..0x07 from
`[0, 0, size_lo, size_hi]` to `[size_lo, size_hi, 0, 0]`, after which the
struct's `size` field at offset 0x06 reads `[0, 0]` = **0**.

Net effect: `ip->attack_coll.size = ev[i].size` always assigned 0 on PC.
Detection hitboxes were unaffected because they take their size from
`attr->size * 0.5F` set during `itManagerMakeItem` (a separate path).
Only the explosion-progression events were broken.

## Verification

`debug_tools/reloc_extract/reloc_extract.py extract baserom.us.z64 251`
gives the bomb's attack events at offset 0x46C, 4×8 bytes:

```
ev[0]: timer=0 angle=361 damage=30  size@0x04=350  size@0x06=0
ev[1]: timer=2 angle=361 damage=30  size@0x04=250  size@0x06=0
ev[2]: timer=4 angle=361 damage=20  size@0x04=150  size@0x06=0
ev[3]: timer=6 angle=361 damage=1   size@0x04=  0  size@0x06=0
```

Same pattern across capsule / box / msbomb / marumine / linkbomb attack
events. Porygon `ITMonsterEvent` (file 264 offset 0x1B4) similarly has
size=300 at offset 0x04 and `fgm_id` at offset 0x20 (low half of its u32
word, so `fgm_id` does need its own `portFixupStructU16(0x20, 1)` rotation
— that one is kept).

IDO probe (`docs/debug_ido_bitfield_layout.md` method) of the BE struct
emits `lhu 0x6($a0)` for `ev->size`, confirming the LE-PORT layout's choice
of struct-offset 0x06 matches IDO. So the struct decl is correct and the
fixup was the problem.

## Fix

Removed `portFixupStructU16(&ev[ip->event_id], 0x04, 1)` from all 7 call
sites:

- `src/it/itmain.c::itMainUpdateAttackEvent`
- `src/it/itcommon/itbombhei.c::itBombHeiCommonUpdateAttackEvent`
- `src/it/itcommon/itmsbomb.c::itMSBombExplodeUpdateAttackEvent`
- `src/it/itground/itmarumine.c::itMarumineExplodeUpdateAttackEvent`
- `src/it/itground/itporygon.c::itPorygonCommonUpdateMonsterEvent`
- `src/it/itground/itfushigibana.c::itFushigibanaCommonUpdateMonsterEvent`
- `src/it/itfighter/itlinkbomb.c::itLinkBombExplodeUpdateAttackEvent`

Kept the legitimate `portFixupStructU16(&ev[i], 0x20, 1)` calls in the two
ITMonsterEvent callers — `fgm_id` is at offset 0x20 (low half of its word)
so it genuinely needs the rotation.

Updated `ITAttackEvent` and `ITMonsterEvent` struct comments in
`src/it/ittypes.h` with a do-not-fixup-size warning so a future "looks like
a missed u16 fixup" sweep doesn't reintroduce the bug.

## Class

This is a "fixup left over after struct rewrite" trap. The 87c4fe8 commit
correctly fixed the layout but didn't audit whether the existing fixup was
still needed under the new layout. The blast radius (every explosion in
the game reading 0 size) is invisible during normal play because explosions
do happen visually via separate effect particles — only the *collision*
side of the hitbox was broken.

Lesson: when a struct layout fix changes where a field lives in memory,
delete every `portFixupStructU16` / `portFixupStructU32` keyed on that
field's old offset; don't just retarget them. After-fix invariant: the
fixup should only exist if a field's struct offset cannot be read directly
after pass1 BSWAP32. For fields at the *high half* of a u32 word, BSWAP32
alone is sufficient; for fields at the *low half*, the rotation is
required.

## Validation tools used

- `debug_tools/reloc_extract/reloc_extract.py` to dump file 251 / 264.
- IDO probe (`/tmp/probe_itattackevent.c` + rabbitizer) to confirm the BE
  struct's compiled access offsets, per `docs/debug_ido_bitfield_layout.md`.
- Hitbox-view enhancement (`gEnhancements.HitboxView`) shipped in the prior
  commit — toggle to "Filled" in the Port Menu to see explosion hitboxes
  expanding through the 350→250→150 progression after the fix.
