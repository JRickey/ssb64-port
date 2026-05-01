# Boss event32 walker cache invalidation on figatree heap reuse

**Resolved 2026-05-01**

## Symptom

Master Hand's okutsubushi (palm slam), okupunch (rocket punch), and drill
(spinning finger) animations all rendered wrong in attract / 1P-mode boss
fights:

- okutsubushi: `translate->z` set to `-15000.0F` by `ftBossOkutsubushiSetStatus`
  was never integrated back to 0 — the boss parked behind the stage for the
  duration of the attack and never returned.
- okupunch: forward swing never landed; the boss retreated and stayed back.
- drill: model rendered upside-down through the move.

Reproduced reliably by booting the 1P-mode boss stage and waiting for the
CPU AI to cycle through attacks (PORT_START_BOSS dev shortcut in
`scmanager.c` skips menu nav for repro).

## Root cause

`port_aobj_fixup.cpp` runs three pointer-keyed caches:

- `sUnhalfswappedVisited`: per-block one-shot fixup tracker for spline data
  (used by `interp.c`).
- `sUnswappedHeads`: event32 stream entry points the walker successfully
  un-halfswapped.
- `sRejectedHeads`: stream entry points the walker decided not to touch
  (didn't look like halfswap-corrupted event32).

`port_aobj_register_halfswapped_range` is called every time a fighter
figatree file is loaded into the figatree heap (`reloc_animations/FT*` and
`reloc_submotions/FT*` matches in `lbreloc_bridge.cpp`).  The figatree
heap is bump-reset between loads — different animation files end up at the
same heap addresses across status changes.  The register call evicted
**only the first cache** when a new range was registered.  The other two
were never evicted.

Concrete chain for okutsubushi:

1. Boss enters okutsubushi for the first time.  File 2127
   (`FTMasterHandAnim029`) loads at heap address H.
2. Walker is called on each of the file's per-joint event32 streams.  For
   joints whose stream entry happened to validate, `H+offset` was added to
   `sUnswappedHeads` and the bytes were rewritten in place.  For streams
   the walker couldn't classify, `H+offset` went into `sRejectedHeads` and
   the bytes were left as-is.
3. Boss exits okutsubushi.  At some point a different animation file
   (different joint count, different per-joint stream offsets) loads at
   address H, overwriting the bytes there.
4. Boss re-enters okutsubushi.  File 2127 loads at H again — fresh
   halfswap-corrupted bytes.
5. The first parser entry into a stream at `H+offset` calls the walker.
   `sUnswappedHeads` / `sRejectedHeads` still contain `H+offset` from
   step 2.  The walker short-circuits at the top of `walk()`, leaving the
   freshly-loaded halfswap-corrupted bytes un-fixed.
6. `gcParseDObjAnimJoint` reads the corrupted command word.  Opcode field
   reads as 64 (the high bit of an originally-low opcode value, lifted
   into the opcode bit range by halfswap), which isn't in the parser's
   case list.  The default fires "UNHANDLED opcode=64 — ending anim",
   sets `dobj->anim_wait = AOBJ_ANIM_END`.
7. Animation never runs.  TransN.translate stays (0,0,0) every frame for
   the rest of the attack.  `ftMainPlayAnim` reads no delta from TransN,
   `vel_air.z` stays 0, `ftBossOkutsubushiProcPhysics` only updates
   `vel_air.x` (its explicit X-chase).  TopN.translate.z is pinned at
   the `-15000.0F` from SetStatus and never integrates forward.

The 1st-time reproduction worked occasionally only because the first walker
call landed before the cache had any entries to short-circuit on.  Every
subsequent boss encounter in the same play session hit the cache and broke.

## Fix

Three lines added to `port_aobj_register_halfswapped_range`:
the same per-range eviction loop that already cleared
`sUnhalfswappedVisited` is now applied to `sUnswappedHeads` and
`sRejectedHeads` too.  Implementation factored into a small lambda for
readability.

`port/port_aobj_fixup.cpp`, around the existing eviction site.

## Why this was hard to spot

- **Heuristic-driven walker.**  The walker tries to distinguish
  halfswap-corrupted event32 from native event32 by simulating a parse and
  measuring stream length.  This works on the first encounter but
  intentionally caches the verdict to avoid re-scanning every frame.  The
  caching is correct; only the eviction was incomplete.

- **Decomp parser truncates on unknown opcode.**  When the un-fixed-up
  bytes hit the parser, "UNHANDLED opcode=64" is logged and the animation
  ends silently.  No crash, no wrong-but-noisy output — just a quiet
  flatline for that joint's transform.  That made the symptom look like a
  joint-hierarchy or bind-pose issue rather than a stream-decode issue.

- **Boss is a thin test surface.**  Fighter character animations have
  enough redundancy across motions that a single broken stream gets noticed
  fast.  Master Hand's per-attack streams are essentially independent —
  one breaking just makes that one attack look weird, with no obvious
  cross-pollution.  None of the boss attacks were tested on a *second*
  encounter in the same session before today.

- **Initial diagnostic detour.**  The first hypothesis chased a wrong
  systemic answer ("`fp->joints[TransN]` is NULL for the boss") that fit a
  static reading of the data tables but turned out to be false in the
  runtime telemetry — TransN gets created via `ftMainUpdateHiddenPartID`
  on each okutsubushi entry.  A 16-line resolver function and three
  caller reroutes were written and discarded once the per-frame log
  showed TransN.translate stuck at (0,0,0) despite the joint existing.

## Telemetry that broke the case open

A targeted per-frame dump of all four boss-relevant joints
(`fp->joints[TopN/TransN/XRotN/YRotN/CommonStart]` translate / rotate /
scale, plus `anim_vel`, `vel_air`, `status_id`) plus the pre/post bytes
each walker call observed.  Showed within 30 frames:

- TransN existed (joint=0x...) — the static analysis was wrong
- TransN.translate stayed (0,0,0) every frame — animation wasn't applying
- Walker rejected the relevant stream with `before == after == <halfswap-corrupted form>`
- Saved log: `logs/boss-okutsubushi-FIXED-2026-05-01.log`

## Systemic audit

Inventoried every pointer-keyed cache in `port/`:

- `port_aobj_fixup.cpp`: 3 caches (above).  All now evicted on figatree
  heap reload via `port_aobj_register_halfswapped_range`.
- `port/bridge/lbreloc_byteswap.cpp`: 4 caches (`sStructU16Fixups`,
  `sTexFixupExtent`, `sTexFixupWords`, `sDeswizzle4cFixups`).  All
  evicted via `portEvictStructFixupsInRange` called from
  `lbRelocLoadAndRelocFile` before bytes are copied.

No other at-risk caches found.  The architecture is sound now — one
eviction call site per cache bucket, every cache wired through it.  The
only design rule going forward: **a new pointer-keyed cache in either
file must be added to the existing eviction site in the same change.**
