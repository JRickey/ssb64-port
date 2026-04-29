# Per-Character Bonus Stage Audit — 2026-04-29

## Trigger

Sweep kicked off after a user-reported "freeze in classic mode, possibly related to Link" was traced to `sc1pbonusstage.c:673` `platform_count--` (a `u8` decrement with no underflow guard, identical bug class to the `target_count` underflow patched in commit `4dbf5d4`). Fix shipped as commit `9a0a834` ("Guard platform_count underflow in sc1PBonusStageUpdatePlatformCount").

To make sure no sibling freezes were lurking, four parallel sub-agents swept the per-character fighter / weapon code (12 fighters in three groups) and the shared bonus-stage scaffolding for the same bug-class fingerprints:

1. Unsigned counter underflow → loop wrap (`u8`/`u16` `_count--` with no guard, used as a `for/while` bound or `==0` sentinel)
2. NULL deref on fighter / weapon GObj pointers
3. `AVOID_UB` / `DAIRANTOU_OPT0` sites — verify the port-active branch is the safe one
4. Static `GObj*` slot staleness across scene re-entry
5. LP64 implicit-int truncation
6. MObjSub fixup coverage on `gcAddMObjForDObj` callsites (shared scaffolding only)

The port unconditionally defines `AVOID_UB=1` (`CMakeLists.txt:112`) and does not define `DAIRANTOU_OPT0`, so every `#elif !defined(DAIRANTOU_OPT0)` branch downstream of an `AVOID_UB` guard is the one that runs.

## Verdict

**Bonus stages will not freeze from any of the audited fighter / weapon code.** The `platform_count` fix is the only one that ties to a user-reported repro; everything else surfaced is latent (would fire only under conditions we cannot currently reach in normal play). No further code changes shipped from this audit — findings are documented here for future reference.

If a user reports a freeze or crash that lands in one of the call paths called out below, this doc is the starting point.

---

## Per-Fighter Status

| Group | Fighter | Verdict | Notes |
|-------|---------|---------|-------|
| A | Mario   | CLEAN   | One AVOID_UB site verified safe at `ftmariospecialn.c:42-46` (port returns, doesn't fall through to uninit spawn). |
| A | Fox     | CLEAN   | Three `s32--` counters in Fire Fox (`ftfoxspecialhi.c:73,151`, `ftfoxspeciallw.c:179`) are signed with proper `==0` exits. Reflector NULL-guarded. |
| A | Donkey  | CLEAN   | Giant Punch only increments `charge_level`; resets to 0 on damage. `catch_gobj` NULL-guarded. |
| A | Samus   | LATENT  | See Finding A1. |
| B | Luigi   | CLEAN   | No fighter-specific specials code; dispatches to Mario handlers via `ftluigistatus.h`. |
| B | Link    | CLEAN   | All four AVOID_UB branches in `ftlinkspecialhi.c:67/78/89/100` run the corrected `wp->weapon_gobj` path. Boomerang `homing_delay--` and `flyforward_timer--` both `> 0` guarded. Bomb `item_gobj` NULL-guarded. |
| B | Yoshi   | CLEAN   | Egg Lay flag-based, no decrement counters. `wpYoshiStarMakeWeapon` AVOID_UB site at `wpyoshistar.c:190` runs the corrected return-path branch. |
| B | Captain | CLEAN   | Falcon Dive `flag2--` at `ftcaptainspecialhi.c:115` is guarded by `flag2 == 0` early-out at line 108. |
| C | Kirby   | LATENT  | See Findings C1, C2. |
| C | Pikachu | CLEAN   | All decremented timers are `s32`; `thunder_gobj` always NULL-guarded; `is_thunder_destroy` flag set whenever wp ejects. |
| C | Purin   | CLEAN   | Zero counters, no projectile, no GObj-slot state. `FTPurinPassiveVars` is a single `u32 unk_0x0`. |
| C | Ness    | LATENT  | See Finding C3. |

Shared scaffolding (group D): see Findings D1 and D2.

---

## Latent findings — not shipped

Each entry is **latent** — would fire only under specific conditions that have not been observed in playtesting. They are documented here so that if a user crash lands on one of these call paths, we have a head start.

### A1. Samus weapon `wp->owner_gobj` NULL deref class

- `src/wp/wpsamus/wpsamuschargeshot.c:261`
- `src/wp/wpsamus/wpsamusbomb.c:194`

Both reflector callbacks call `ftGetStruct(wp->owner_gobj)` and then `wpMainReflectorSetLR(wp, fp)` (which derefs `fp->lr` at `wpmain.c:105`) with no NULL guard.

`wp->owner_gobj` is reassigned to the *reflecting* fighter at deflect time (`wpprocess.c:526`). If that fighter dies / is ejected on the same frame as deflect impact resolution, `owner_gobj` is stale.

**Same pattern exists in:** `wpmariofireball.c:150`, `wpkirbycutter.c:123`, `wpyoshistar.c:140`. Original-game design (HAL); not a port regression.

Fix shape would be a `if (wp->owner_gobj == NULL || ftGetStruct(wp->owner_gobj) == NULL) return TRUE;` early-return at the top of every reflector callback. Multi-file, defensive — defer until a real owner-death-on-deflect crash is reported.

### C1. Kirby specialhi `default: while (TRUE)` panic

- `src/ft/ftchar/ftkirby/ftkirbyspecialhi.c:30-34` (flag1 default)
- `src/ft/ftchar/ftkirby/ftkirbyspecialhi.c:82-87` (flag2 default)

```c
default:
    while (TRUE)
    {
        syDebugPrintf("gcFighterSpecialHiEffectKirby : Error  Unknown value %d \n", fp->motion_vars.flags.flag1);
        scManagerRunPrintGObjStatus();
    }
```

HAL's "should never happen" debug stub. `motion_vars.flags.flag1` and `flag2` are motion-script-driven; if a script ever yields an unexpected value (corruption, byteswap miss, copy-ability-state desync), the game enters an infinite `syDebugPrintf` loop with no recovery path.

Fix shape would be `#ifdef PORT break; #else while (TRUE) {...} #endif`. Defer until a user crash log shows entry into one of these defaults.

### C2. Kirby Mario-copy fireball — broken AVOID_UB safety net

- `src/ft/ftchar/ftkirby/ftkirbycopymariospecialn.c:41-60`

The `#if defined(AVOID_UB) return; #else break; #endif` block sits between two case labels with no preceding `default:`. Currently dead code — unreachable by switch design. The bug is the *safety net* is non-functional, not that the switch falls through today.

If `copy_id != Mario && copy_id != Luigi-family`, the switch would fall through with `fireball_kind` uninitialized, then `wpMarioFireballMakeWeapon(... fireball_kind)` would index `dWPMarioFireballWeaponAttributes[fireball_kind]` (OOB read → garbage `lifetime`/`angle`/`vel`). Currently masked by upstream invariant (Kirby copy assignment never produces other values).

Defer — document the fragility; no need to ship a rewrite of the case dispatch.

### C3. Ness PK Thunder double-reflect — documented in-source by decomp

- `src/wp/wpness/wpnesspkthunder.c:613-617` (the deref site)
- `src/wp/wpness/wpnesspkthunder.c:546-547` (the root cause + suggested fix)

```c
// Line 546-547:
wp->lifetime = WPPKTHUNDER_LIFETIME; // This line is indirectly responsible for the PK Thunder double reflect crash; omitting it fixes the oversight
                                     // Solution: wpNessPKReflectHeadSetDestroyTrails(weapon_gobj, nWPNessPKThunderStatusDestroy);

// Line 613-617:
// Game hangs on the following line when PK Thunder crash occurs (DObjGetStruct returns NULL)
// This happens because the program loses the reference to the old PK Thunder trail objects which are still accessing the head's DObj even after it's ejected
DObjGetStruct(weapon_gobj)->translate.vec.f.x =
(DObjGetStruct(wp->weapon_vars.pkthunder_trail.head_gobj)->translate.vec.f.x - ...);
```

This is an **original-game bug** — observable on a real N64 with the right reflector chain. The decomp authors documented it but did not patch it.

The decomp's own suggested fix is at the root cause (line 547): destroy trails when the head is reflected, instead of resetting the head's lifetime. That is the *correct* fix; a NULL guard at line 616 would mask the symptom but leave orphaned trails alive.

Defer — if a user crash log shows entry into `wpNessPKReflectTrailProcUpdate` after a Ness-vs-Fox-reflector scenario, apply the line-547 solution (one-line replacement; preserves N64 buggy behavior under `#ifndef PORT`).

### D1. `sc1pbonusstage.c:714` unguarded `dobj->child` deref

```c
if (dobj->child->user_data.s != nMPYakumonoStatusNone)
```

`gcEjectDObj(dobj->child)` at line 660 nulls `dobj->child` (eject splices `parent->child = sib_next`). `lbCommonSetupTreeDObjs` at 662 normally repopulates it. If the boarded-platform reloc lookup ever yields zero entries (data corruption / asset extraction edge case), `dobj->child == NULL` survives and the next-frame walker faults at line 714.

Latent: depends on a data-corruption invariant break we have not seen.

### D2. `gcEjectGObj(slot)` not paired with `slot = NULL` — parity gaps

Multiple sites in `sc1pstageclear.c` (1798, 1832, 1837, 1853, 1858, 1884, 1917, 1922, 1938, 1943, 1969, 2048, 2054, 2066) and `mnplayers1pbonus.c:91, 2103` eject a static slot without setting it to NULL afterward. Same-scene re-fire is prevented by tic gating today; cross-scene staleness is masked by `InitVars` resets where they exist (the four added in commit `ee7453f` close most of these).

`sMNPlayers1PBonusGameModeGObj` is also missing from `mnPlayers1PBonusInitVars` (line 2774-2796), unlike its siblings `sMNPlayers1PBonusHiScoreGObj` (the `ded7063` fix) and `sMNPlayers1PBonusTotalTimeGObj`. Today the slot is unconditionally re-assigned by `mnPlayers1PBonusMakeLabels` during scene Start, masking the staleness.

Pure parity hardening; not behavior-affecting today. Defer until the broader staleness sweep next runs.

---

## Coverage summary

Bug-class fingerprints confirmed clean across all 12 fighter / weapon directories and the shared bonus-stage scaffolding:

- `u8` / `u16` counter underflow → loop wrap (the freeze fingerprint). Only `target_count` and `platform_count` had the pattern; both now guarded.
- `AVOID_UB` / `DAIRANTOU_OPT0` sites — every reachable site runs the safe branch under the port's macro definitions.
- LP64 implicit-int truncation — `-Werror=implicit-function-declaration` is enforced; no fighter / weapon files surfaced new sites.
- LP64 stride bugs in shared bonus code — `grBonus3MakeBumpers` (commit `e9af95b`), `sc1PBonusStageMakeTargets`, `sc1PBonusStageMakeBumpers` all use the correct PORT pattern (`u32*` token walker + `PORT_RESOLVE`).
- Static `GObj*` slot staleness — every `if (slot != NULL) gcEjectGObj(slot)` site in 1P / bonus code is either reset in `InitVars` or guaranteed safe by unconditional re-assignment before any read.
- MObjSub fixup coverage in shared bonus scaffolding — every `gcAddMObjForDObj` reaches the path that already calls `portFixupMObjSub`.

## Methodology notes

The four agents ran in parallel against `/Users/jackrickey/Dev/ssb64-port`. Each had ~300-word output budget and worked from a self-contained prompt that included the bug-class fingerprints, the worked example for AVOID_UB (Link's specialhi), and per-fighter focus hints. Per-character coverage:

- Agent A: Mario, Fox, Donkey, Samus
- Agent B: Luigi, Link, Yoshi, Captain Falcon
- Agent C: Kirby, Pikachu, Purin (Jigglypuff), Ness
- Agent D: shared scaffolding (sc1pbonusstage / sc1pstageclear / sc1pgame / sc1pmanager / mnplayers1pbonus / mn1pcontinue / grbonus3 / grcommon / port glue)

The prompts told them not to fix anything — investigation only. Group A turnaround was 5 min, B was 4 min, C was 5.5 min, D was 9 min.

Followup investigations that surface a confirmed user repro should re-read this doc before opening a fresh sweep — most plausible candidates already have the call site, the trigger mechanism, and the suggested fix shape captured above.
