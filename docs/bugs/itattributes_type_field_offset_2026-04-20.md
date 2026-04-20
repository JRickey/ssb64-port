# ITAttributes.type decoded from wrong bitfield word — RESOLVED (2026-04-20)

## Symptom

Using non-shootable items caused the game to behave incorrectly or crash:

- **Pokeball (nITKindMBall)**: pressing A with it held → SIGBUS `fault_addr=0x6400000064` at `ftMainProcPhysicsMap+196`.
- **Capsule ("pill bottle")**: same crash signature.
- **Hammer**: disappeared after pickup instead of being held while Mario hops around.
- **Star Rod**: Mario appeared to drop it instead of swinging it.
- **Heart**: behaved like Maxim Tomato (both are Consume — this one is actually per-kind handling, not a type issue).

The crash fingerprint was distinctive: `0x6400000064` where both 32-bit halves equal `0x64 = 100`, matching the `motion_id=100` (`nFTCommonMotionLightThrowAirF`) that appeared in the log line immediately before the fault. The immediate cause was `ftCommonItemShootSetStatus` being called with `ip->kind` neither `nITKindLGun` nor `nITKindFFlower`, hitting a switch with no default case and passing an uninitialized `status_id` (stack garbage `0xFFFFFFFF = -1`) to `ftMainSetStatus`, which then read garbage out of `dFTCommonNullStatusDescs[-1]` and corrupted state that crashed the next frame.

## Root Cause

`ftCommonItemShootSetStatus` is only supposed to be called when `ip->type == nITTypeShoot` (`= 2`). The caller at `ftcommonattack1.c:271` gates on this. But items like Pokeball/Capsule/MSBomb (all `nITTypeThrow` in gameplay) were decoding `ip->type = 2 = Shoot` because **the `type` field in `ITAttributes` is declared in the wrong bitfield word**.

The original decomp declaration put `type:4` at bits 28-31 of what it called Word B4 (u32 at struct offset `0x44`):

```c
// Wrong — what the port inherited from the decomp.
u32 knockback_weight : 10;
s32 shield_damage   : 8;
u32 attack_count    : 2;    // <-- fake fields pinned to bits 13-10 of Word B2
u32 can_setoff      : 1;    //     (really where `type` lives)
u32 hit_sfx         : 10;
u32 : 1;
...
u32 type            : 4;    // <-- at bits 28-31 of Word B4 (reads unrelated bits)
u32 hitstatus       : 4;
...
```

The ROM byte evidence (extracted from `baserom.us.z64`, reloc file 251) shows that the real field at bits 13-10 of Word B2 (offset `0x3C`) is `type:4`, and it decodes cleanly across every item in the file:

| Item          | `w3C`        | bits 13-10 | Expected   | Matches? |
| ------------- | ------------ | ---------- | ---------- | -------- |
| Box, Taru     | `0x230a0040` | `0`        | Damage (0) | ✓        |
| Sword, Bat, StarRod, Harisen | `0x270f0400` | `1`        | Swing (1)  | ✓        |
| LGun, FFlower | `0x27028800` | `2`        | Shoot (2)  | ✓        |
| Pokeball, Egg, MSBomb, GShell, RShell, Capsule | `0x270f0c00` | `3`        | Throw (3)  | ✓        |
| Star          | `0x30001000` | `4`        | Touch (4)  | ✓        |
| Heart, Tomato, Hammer | `0x37001400` | `5`        | Consume (5) | ✓     |

The actual layout of Word B2 is:

```
bits 31-22: knockback_weight (10)
bits 21-14: shield_damage    (8)
bits 13-10: type             (4)  ← here
bits 9-0:   <other, mostly 0 in ROM>
```

The field the decomp labeled `attr->type` at bits 28-31 of Word B4 sat in what is actually the trailing part of the struct (or past the struct's real 72-byte end — see "Struct size mismatch" below) and produced effectively random 4-bit values (observed: `2` or `3` across all items regardless of actual behavior).

## Why the Decomp Got This Wrong

The ssb-decomp-re ITAttributes layout was almost certainly derived by copy-paste from WPAttributes. The two structs share the first bitfield word (angle/kb_scale/damage/element) and a very similar second word (kb_weight/shield_damage/...). **But WPAttributes has no `type` field** — weapons are identified by per-weapon code, not by a runtime-read type category. ITAttributes does need `type` because the item-use code branches on it, and the real hardware put `type:4` into the bits that WPAttributes uses for `attack_count:2 + can_setoff:1 + top-bit-of-sfx:1`.

On the original N64 (BE), the MIPS disassembly for code reading `attr->type` almost certainly looks like:

```
lw   $t0, 0x3C($a0)    # load Word B2
srl  $t0, $t0, 10      # shift to bring type bits down
andi $t0, $t0, 0xF     # mask to 4 bits
```

An RE reviewer looking at WPAttributes first would see:
```
lw   $t0, 0x2C($a0)    # load Word 2 of WPAttributes
srl  $t0, $t0, 12      # attack_count at bits 13-12
andi $t0, $t0, 3
```

and model the struct with `attack_count:2 + can_setoff:1 + hit_sfx:10`. Applying that same template to ITAttributes without noticing the shift is off by 2 — and without noticing there's no `type` opcode matching the inferred position — gives the incorrect declaration we inherited.

Because the original BE SGI IDO compile laid the struct out following the (wrong) declaration, the BE build compiles code that reads `attr->type` from bits 28-31 of Word B4 — which, in the BE binary, doesn't match what the **item code actually does**. The actual shipped ROM data uses the layout described above. So the decomp's declaration has always been inaccurate; the BE build was never exercised against non-LGun/FFlower items through `ftCommonItemShootSetStatus` in a way that would have caught the garbage return, because:

1. Pressing A with a Throw item hits the `ip->type == nITTypeThrow` branch first at `ftcommonattack1.c:253` and returns before the problematic switch.
2. Pressing A with a Swing item hits `case nITTypeSwing` and dispatches to `ftCommonItemSwingSetStatus`.
3. Only the `nITTypeShoot` case reaches `ftCommonItemShootSetStatus`, and that function *does* have a full switch on `ip->kind` for LGun/FFlower — but in practice on BE, the upstream `ip->type == nITTypeThrow` check probably *happens to succeed* for most Throw items by luck (bits 28-31 reading 3 = `nITTypeThrow`), routing them to the Throw path before `ItemShoot` ever gets called. On our LP64 PC build, the garbage bits give 2 (Shoot) for exactly the items that should be Throw, exposing the bug.

Effectively: the struct has been wrong since the initial decomp import, the BE build accidentally worked around it, and the port finally tripped the landmine.

## Struct size mismatch (also observed, deferred)

The port's `_Static_assert(sizeof(ITAttributes) == 0x50)` claims the struct is 80 bytes, but the ROM item-offset table in `tools/reloc_data_symbols.us.txt` shows 72-byte (0x48) strides between most items in file 251 (`llITCommonDataHeartItemAttributes = 0x100`, `llITCommonDataStarItemAttributes = 0x148`, diff = `0x48`). The extra 8 bytes in our declaration (Word B5 + `spin_speed` + pad) read into the next item's pointer fields at runtime. This didn't cause visible breakage in the items we tested (most items have 0 or reasonable values at those offsets, and `smash_sfx` / `vel_scale` / `spin_speed` reads land in the next item's reloc tokens which happen to decode to plausible-enough numbers), but fields past offset `0x48` should be considered unreliable. **Deferred: confirm the real sizeof and move `smash_sfx`/`vel_scale`/`spin_speed` either to their real location or out of the ROM-backed struct.**

## Fix

Applied in `src/it/itmanager.c:280-299`. On the PC (PORT) build, bypass the broken bitfield declaration and extract `type` from the known-correct word position:

```c
#ifdef PORT
    {
        const u32 *word_b2 = (const u32 *)((const char *)attr + 0x3C);
        ip->type = (*word_b2 >> 10) & 0xF;
    }
#else
    ip->type = attr->type;
#endif
```

Kept the original `attr->type` path on BE (it has been living with the bug all along; changing the decomp source-of-truth struct layout without reverifying every other `attr->X` read is out of scope).

### Safety net

`ftCommonItemShootSetStatus` in `src/ft/ftcommon/ftcommonitemshoot.c:301` previously had no `default:` case in its `switch (ip->kind)`. Any unexpected kind would fall through with `status_id` and `proc_accessory` left uninitialized. Added a PORT-only default that logs and returns cleanly — defense in depth for any remaining decode bugs that route non-LGun/FFlower items here.

### Diagnostic plumbing

Added `port_dump_backtrace()` to `port/port_watchdog.cpp` + `.h` as an extern "C" wrapper around the pre-existing `DumpBacktraceBoth` (which was in an anonymous namespace). Called from `ftMainSetStatus` when `status_id < 0` to surface the caller if any similar miscoded path ever reappears.

## Verification

Pokeball + Capsule previously produced:

```
SSB64: !!! ftMainSetStatus ENTRY status_id=0xffffffff (negative) caller_ra=0x1041aa1dc
SSB64: ---- main-thread backtrace ----
0   ssb64  ftMainSetStatus + 112
1   ssb64  ftCommonItemShootSetStatus + 164
2   ssb64  ftCommonAttack1CheckInterruptCommon + 268
...
SSB64: !!!! CRASH SIGBUS fault_addr=0x6400000064
3   ssb64  ftMainProcPhysicsMap + 196
```

Post-fix: Pokeball/Capsule/Egg/MSBomb route through `ftCommonItemThrowSetStatus` and launch normally; LGun/FFlower correctly reach `ftCommonItemShootSetStatus` with LGun/FFlower kinds. Sword/Bat/StarRod/Harisen hit the Swing path. Hammer stays held. Star grants invincibility on touch. The `!!! ftMainSetStatus ENTRY status_id=<negative>` line no longer appears in the log under item interaction.

## Files touched

- `src/it/itmanager.c` — surgical fix for `ip->type` decode (PORT guard).
- `src/ft/ftcommon/ftcommonitemshoot.c` — added `default:` case safety net.
- `src/ft/ftmain.c` — `status_id < 0` diagnostic guard at `ftMainSetStatus` entry.
- `port/port_watchdog.cpp`, `port/port_watchdog.h` — `port_dump_backtrace()` C wrapper.

## Related

- `docs/bugs/samus_charge_shot_hit_detection_2026-04-13.md` — suspected same class of bitfield miscoding may still affect WPAttributes' `attack_count` decode on the intro-bypass path. Worth re-examining in light of this finding.
- `src/wp/wptypes.h` — WPAttributes and ITAttributes share Word B2 declaration shape; WPAttributes has no `type` field so the bits that ITAttributes uses for `type:4` really are `attack_count:2 + can_setoff:1 + top-bit-of-sfx:1` in WPAttributes (confirmed by weapon-decoded counts in `samus_charge_shot_hit_detection_2026-04-13.md`: FoxBlaster=1, SamusBomb=2, MarioFireball=2, SamusChargeShot=0). So the WP layout is fine as-is; only IT is misdeclared.
