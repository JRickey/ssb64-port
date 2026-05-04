# Issue #128 — Random crashes after extended campaign play (handoff)

Sibling investigation to issue #103 (PR #119, commit `f714a0d`). User reports
random crashes on RetroPie/Pi 4 with v0.7.3-beta after a full campaign run.
Crash logs show the libultraship G_VTX guard firing repeatedly with stale
display-list pointers, then escalating to a hard fault.

Background: `docs/bugs/g_vtx_unresolved_addr_guard_2026-05-03.md`.

## Log fingerprint

```
G_VTX(f3dex2) skipped: unresolved addr=0x1000010 n=0 dest_end=8 (raw w1=0x1000010)
Unhandled OP code: 0x70/0x71/0x88, for loaded ucode: 4 (f3dex2)
```

- Ucode 4 = f3dex2 → mainline 3D rendering (not 2D menus).
- Address `0x01000010` = N64 segment 0x01 + offset 0x10.
- 20 unique `cmd=` host addresses across the entire 176-line log → the SAME
  stale DL is being walked frame-after-frame, not progressive corruption.
- `cmd=0x5588a3f000 w0=0x88A63C00 w1=0x0000005588A6BC00` proves the memory
  was once a real port-fixed-up DL (host pointer in w1) before being
  repurposed.
- 37 minutes later, a second burst at fresh host addresses — same bug class,
  different stale region.

## Investigation summary (4 parallel Opus agents, 2026-05-03)

Four converging leads. Listed in order of confidence.

### Lead 1 — `sPortPackedDisplayListCache` missing per-load eviction (HIGH)

**Location:** `libultraship/src/fast/interpreter.cpp:91`, populated by
`portNormalizeDisplayListPointer` at line 229.

**Key:** raw heap `const void*` of the source packed DL.

**Stored value:** `shared_ptr<vector<F3DGfx>>` of widened commands plus the
`fileBase`/`fileSize` of the originating reloc file *at widening time*.

**Bug:** evicted only by `portResetPackedDisplayListCache()`, called from a
single site — `lbRelocInitSetup` (`port/bridge/lbreloc_bridge.cpp:885`,
full scene reset). NOT evicted by `lbRelocLoadAndRelocFile`
(`lbreloc_bridge.cpp:335-389`), even though peer caches at the same call
site (`portEvictStructFixupsInRange`, `portTextureCacheDeleteRange`,
`portRelocEvictFileRangesInRange`, lines 378-388) are all evicted there.

**Failure shape:** when a bump-reset heap reuses the same source-DL address
inside a single scene (stage-select wallpaper rewind via
`lbRelocGetForceExternHeapFile`, classic-mode round transitions that
recycle the round-data extern heap, CSS hover reloads), the cache returns
a vector with stale `fileBase`/`fileSize`. Segment-0E sub-DL references
then resolve to the *prior* file's address window. A `gsSPDisplayList(0x0E000000+X)`
jumps into stale bytes; G_VTX in that stale stream carries an N64 segment
address from the prior file's reloc layout, which `Interpreter::SegAddr`
cannot resolve → exact issue #103/#128 fault shape.

**Same family as commit `b50602f`** ("Fix scene-45 wallpaper: evict Fast3D
cache on heap reuse"), but for the packed-DL widening cache instead of the
texture cache. The texture cache fix landed; this one was missed.

**Fix:** add `portPackedDisplayListCacheDeleteRange(ram_dst, copySize)` to
`lbRelocLoadAndRelocFile` next to the existing range-evict calls. Walk
`sPortPackedDisplayListCache` and erase entries whose `info.source` falls
in the new range. Requires libultraship submodule bump.

### Lead 2 — Segment 1 unmapped during stale `lbtransition` DL walk (HIGH)

**The decisive fingerprint:** `addr=0x01000010` is N64 segment 1 + offset
0x10. There is **exactly one** `gSPSegment(..., 0x1, ...)` call in the
entire decomp tree — `decomp/src/lb/lbtransition.c:221`, binding segment 1
to `sLBTransitionPhotoHeap`.

**Only consumer of `lbTransitionMakeTransition`:** `mnvsresults.c:3388`
(VS-results screen). Classic-mode campaign reaches VS-results between
matches; the cycle re-enters this scene repeatedly.

**Bug:** `sLBTransitionFileHeap` / `sLBTransitionPhotoHeap` BSS globals
(`lbtransition.c:182, 185`) are written *only* by
`lbTransitionSetupTransition`, which is gated on
`sMNVSResultsKind != nMNVSResultsKindNoContest` (`mnvsresults.c:3386-3390`).
On a NoContest path, setup is skipped — but if a previous results screen
left transition-DL data alive in the DObj tree (e.g., a sibling camera's
`COBJ_MASK_DLLINK(32)` mask still includes the transition gobj's bit), the
walk dereferences transition-file Gfx whose intra-file w1 resolves through
seg-1 — which was never re-bound this frame.

**Compounding factor:** `Interpreter::SpReset`
(`libultraship/src/fast/interpreter.cpp:5755-5770`) does **not** clear
`mSegmentPointers[]` between frames. A segment pointer written in frame N
survives unless overwritten — and segment 1 is only ever written by
`lbtransition.c:221`, so once the transition stops running, segment 1 is
either stale (host pointer to freed memory) or has never been written
this session.

**Same family** as `btt_result_screen_state_leak_2026-04-30.md` and
`intro_jungle_state_leak_2026-05-02.md` — overlay BSS state that on N64 is
implicitly cleared by `syDmaLoadOverlay`'s BSS re-DMA, but which on PC
(stubbed `syDmaLoadOverlay`, statically-linked overlays) leaks across
re-entries.

**Confirmation instrumentation:** add `port_log` at `lbtransition.c:221`
and `:271`, plus log every `Interpreter::SegAddr` call where
`(w1 >> 24) == 0x01` and `mSegmentPointers[1]` is null/stale. If the next
repro shows a G_VTX-skip burst with no preceding `lbTransitionProcDisplay`
log line in the same frame, this is confirmed.

### Lead 3 — `mnPlayers1PGame/Bonus/TrainingInitPlayer` slot-field gap (HIGH)

**The 1P twin of PR #119's fix.** PR #119 added explicit NULL clears to
`mnPlayersVSInitPlayer` (`mnplayersvs.c:4644-4660`) — the slot's
`cursor`, `puck`, `name_emblem_gobj`, `panel_doors`, `panel`,
`figatree_heap`. The 1P-mode siblings have the same struct layout and
identical lifecycle but were not fixed:

- `decomp/src/mn/mnplayers/mnplayers1pgame.c:3436-3473`
  (`mnPlayers1PGameInitPlayer`) — clears only `flash`, `p_sfx`, `player`.
- `decomp/src/mn/mnplayers/mnplayers1ptraining.c:3034-3071`
  (`mnPlayers1PTrainingInitPlayer`) — same gap.
- `decomp/src/mn/mnplayers/mnplayers1pbonus.c:2787-2800`
  (`mnPlayers1PBonusInitPlayer`) — same gap. **The reporter's repro hit
  bonus stages.**

**Why it crashes:** the slot's `name_emblem_gobj` reaches `SObj` reads
later in the same compilation unit; `figatree_heap` flows into
`desc.figatree_heap` which seeds `Vtx*` resolution. After
`taskman.c:1336-1359` frees the prior 16 MiB scene arena per
`syTaskmanStartTask`, the still-stored slot pointers dereference into
freed memory. Matches the G_VTX fault signature.

### Lead 4 — `sc1pmanager.c` bypasses `scManagerRunLoop` GObj-clear (CONTEXT)

**Architectural finding** that explains why the existing `scmanager.c:954-958`
clear is insufficient. `sc1PManagerUpdateScene`
(`decomp/src/sc/sc1pmode/sc1pmanager.c:372,384,391,414,477,489,496,504,522,529`)
calls `XxxStartScene()` functions **directly**, never returning to
`scManagerRunLoop` between transitions. This means
`gSCManager1PGameBattleState.players[*].fighter_gobj` is **not nulled**
across the chain CSS → 1Pintro → 1Pgame → StageClear → CSS for the entire
campaign. The arena-free in `taskman.c` runs correctly, but the per-state
GObj scrub at `scmanager.c:954` is bypassed.

This makes Leads 1-3 worse: stale pointers that would have been nulled
under the standard `scManagerRunLoop` path stay live throughout the
campaign cycle.

## Other findings (lower priority)

- **`mn1pcontinue.c`**: 10 sibling GObj statics adjacent to the existing
  `Issue #103` fix (lines 73-142) are not nulled. Same lifecycle as the
  fixed pair (`sMN1PContinueFighterGObj`, `sMN1PContinueFigatreeHeap`).
  Existing fix at line 1199-1200 is incomplete.
- **`mvOpeningRoom*.c`**: 23 GObj + 3 figatree heap statics; only ticcount
  reset in InitVars. Hit on attract loop — relevant for users who idle.
- **`mvEnding.c`**: 11 GObj + figatree heap; matters once per campaign
  loop.
- **`scStaffroll.c`**: GObj statics not cleared; matters once per Ending.
- **`port_aobj_event32_unhalfswap_reset`**: exposed but never wired into
  `lbRelocInitSetup`. Masked by per-range lockstep eviction; wire it for
  parity.

## Recommended fix order

1. **Lead 1** (libultraship `sPortPackedDisplayListCache` per-load
   eviction). Single-file patch + submodule bump. Most targeted fix; same
   pattern as the texture-cache predecessor.
2. **Lead 3** (1P-mode slot-field NULLs in `InitPlayer` × 3 files). Same
   diff shape as PR #119 — paste-mirror the VS fix into the three 1P
   siblings. Low risk, covers exact reproducer profile.
3. **Lead 2** (lbtransition / segment-1 BSS leak). Add
   `Interpreter::SpReset` segment clear + `lbtransition` setup PORT-guard
   for stale-heap pointers. Higher risk, but uniquely matches the address
   fingerprint.
4. **Lead 4 (cleanup)** — extend the `scmanager.c:954` scrub pattern into
   `sc1pmanager.c`'s direct dispatch sites, OR make all per-scene InitVars
   self-sufficient.
5. Lower-priority statics (mn1pcontinue siblings, mvOpening, mvEnding,
   scstaffroll) as a follow-up sweep.

## Data preserved

User's logs at `/tmp/issue128_battleship.log` and
`/tmp/issue128_vertex.log`. Cmd-address clusters (`0x5588a3ef..f1` and
`0x5580797100..190`) are the fingerprint to confirm the fix worked — both
should disappear after Lead 1 lands.
