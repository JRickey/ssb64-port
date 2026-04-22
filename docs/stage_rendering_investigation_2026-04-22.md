# Stage Rendering Investigation — Handoff (2026-04-22)

**RESOLVED 2026-04-22.** Root cause: `MPGroundData.layer_mask` (u8 at struct
offset 68) was read as 0 after pass-1 blanket BSWAP32, which pushed the real
byte to offset 71. `grDisplayMakeGeometryLayer` then always picked
`pri_proc_display` (`gcDrawDObjTreeForGObj`) instead of the DL-Links variant,
dispatching DObjDLLink chains as raw `Gfx*` — the (0, token, 4, 0) NOOP runs
in the GBI trace. Fix: new `portFixupStructU32` helper + one call in
`mpCollisionInitAll`. See
`docs/bugs/mpgrounddata_layer_mask_u8_byteswap_2026-04-22.md`. Post-fix GBI
trace across 5,470 frames shows zero `G_NOOP`s (pre-fix Sector Z alone had 276).

Infrastructure added during this investigation (demo-cycle loop + per-file
audit counters) remains in place and should stay — future stage bugs will use
the same counters.

---

## Original user report

User reports the following stages visibly broken during VS play:

| Stage                          | Reported symptom                                            |
|--------------------------------|-------------------------------------------------------------|
| Hyrule Castle                  | Central tower missing                                       |
| Sector Z (Fox)                 | Great Fox spaceship missing entirely                        |
| Zebes (Samus)                  | "just empty" — map does not render                          |
| Dream Land (Kirby)             | Trees' middle section broken                                |
| Yoshi's Island (Yoster)        | Flower shows horizontal stripe corruption                   |
| VS mode in aggregate           | Crashes on stage select; scrolling corrupts prior maps      |

This investigation is partially underway and partially stale — several
items resolve correctly under the demo-cycle probe (see Visual Evidence
below), so the report may predate recent commits. Sector Z reproduces.

---

## Infrastructure added this session

### 1. `SSB64_STAGE_CYCLE_DEMO` CMake option

Default **OFF**. Build with `-DSSB64_STAGE_CYCLE_DEMO=ON` to boot
directly into auto-demo mode and loop forever through all 8 VS stages
in order:

```
Pupupu → Zebes → Castle → Jungle → Sector → Yoster → Yamabuki → Hyrule
```

Implementation:

- **`CMakeLists.txt`** — adds the option + propagates
  `-DPORT_STAGE_CYCLE_DEMO=1` when enabled.
- **`src/sc/scmanager.c:~875`** — demo-mode scene override. When the
  option is set, scManager skips title/menu and jumps straight into
  auto-demo with a hand-seeded roster.
- **`src/sc/sccommon/scautodemo.c`** — under `#ifdef PORT_STAGE_CYCLE_DEMO`:
  - `scAutoDemoExit` is patched to loop back into the next stage in
    sequence instead of returning to the attract screen.
  - `demo_fkind[]` is seeded with **Mario** (P1) and **Fox** (P2) so
    every stage has a valid 2-fighter match.
  - `scAutoDemoDetectExit` is no-op'd so the demo never exits early on
    input or KO.

This lets the port be visually inspected across all stages without
controller input — sufficient for screenshot capture and GBI-trace
capture.

### 2. `SSB64_STAGE_AUDIT=1` env var

Runtime instrumentation gate. When the env var is set, per-file audit
counters in `port/bridge/lbreloc_byteswap.cpp` collect:

- **Pass 2 scan counters** — `vtx_scan_count`, `settimg_scan_count` per
  file (how many vertex / texture headers the pass 2 walker encountered).
- **Chain-walk fixup counts** — how many u16-halfswaps actually applied
  to vertex data and texture data *during load* (pass 2 chain walk).
- **Runtime dispatch counters** — how many times a GBI vertex/settimg
  lookup hit a file-resident address vs. a heap address after load is
  done (`runtime.skip` vs `runtime.heap` vs `runtime.fix`).
- **Per-file dispatch-count map** — keyed by originating file name;
  tells you which file actually owns the data a given frame is reading.
- **Opcode census** — G_* command histogram.
- **Byte histogram** — raw byte distribution for pre/post pass1 audit
  of arbitrary file regions.

Summary emitted via **`portStageAuditEmitLoadSummary`** in
`port/bridge/lbreloc_bridge.cpp` after **each file load completes**.

**Counter-reset gotcha (important).** Because the per-file counters
reset on every file-load entry and bank loads are recursive (main file
A loads dependency B, which loads C…), the per-file chain-walk numbers
printed in summaries **only reflect the topmost file of each nested
load, not the leaf where the fixups actually landed.** In the Sector Z
vs Jungle comparison below this made the raw per-file chain-fixup
counts look falsely low for the map files — the real work happened in
`ExternDataBank104`, `ExternDataBank109`, `MiscData086`, etc., which
load recursively and print their summary first. If you see `chain:0`
on a stage file, scroll back up and look at the dependency banks.

No-op behavior when `SSB64_STAGE_AUDIT` is unset — zero runtime cost on
normal builds.

### 3. Screencapture strategy

With `SSB64_STAGE_CYCLE_DEMO=ON`, the port cycles stages on a known
cadence (≈15s per stage after transitions). Timed screenshot captures
during specific seconds of a run land reliably on the target stage.
Four captures at t≈100/105/110/115/120s land in the 2nd cycle's Sector
Z window.

---

## Ruled-out hypotheses (with evidence)

### H1. Struct layout drift on `MPGroundDesc` / `MPGroundData` / `MPGeometryData`

Hypothesis: IDO BE and clang LP64 LE produce different field offsets
for the ground-description chain, and a mismatched offset causes the
DL/collision pointers to resolve wrong.

**Rejected.** Side-by-side probes compiled with both toolchains
(`/tmp/probe_mpground_ido.c` and `/tmp/probe_mpground_port.c`) showed
**identical** field offsets:

| Struct            | sizeof | Field offsets                                                 |
|-------------------|--------|---------------------------------------------------------------|
| `MPGroundDesc`    | 0x10   | 0x00 / 0x04 / 0x08 / 0x0C                                     |
| `MPGroundData`    | 0xA8   | 0x00 / 0x10 / 0x20 / 0x30 / 0x40 / 0x44 / 0x48 / 0x4C / …     |
| `MPGeometryData`  | 0x1C   | (layout matches IDO)                                          |

No padding divergence. This class of bug (the 2026-04-08
`ground_geometry_collision` fix) is already covered.

### H2. `map_head = PORT_RESOLVE(map_nodes) − intra_file_offset` arithmetic

Hypothesis: The subtraction in the stage-load path produces a
pre-origin pointer that breaks subsequent relative addressing inside
the file blob.

**Rejected.** `portRelocRegisterPointer` registers targets as
`ram_dst + in-file_offset`, meaning the reloc token system already
preserves contiguous-file-blob semantics across load. Walking backwards
from the registered pointer by the intra-file offset returns the same
byte the N64 would see at the file's base — contiguous addressing is
preserved. The subtraction pattern is semantically faithful and not
the source of any rendering artifact we've observed.

### H3. Vertex-byteswap pipeline for stage-adjacent files

Hypothesis: The u16-halfswap that applies to vertex data during load
never lands on stage files (skipped because the file doesn't match a
known pattern / walker bails out).

**Rejected for at least the heavy stage-geometry file.** Direct trace
of `chain_fixup_vertex` showed **30+ successful calls** fixing
`ExternDataBank104`'s vertex data (that file carries a large fraction
of stage geometry). Byte-dumps pre-pass1, post-pass1, and at runtime
confirmed `rotate16` actually applied to the target bytes — the
halfswap walker is reaching stage vertex data correctly.

### H4. Texture-byteswap skipped for Sector Z files

Hypothesis: Sector Z's textures fail to byte-swap on load, causing the
DL to draw transparent / garbage tiles → hull appears missing.

**Rejected.** `tex_fixup.log` shows:

| Stage      | runtime.skip | runtime.heap | runtime.fix |
|------------|-------------:|-------------:|------------:|
| Sector Z   | 83,180       | 0            | 0           |
| Jungle     | 50,275       | 0            | 0           |

Per-file breakdown of the Sector Z `runtime.skip` (file owning each
dispatch):

- `StageSector`: 99
- `GRSectorMap`: 262
- `FoxSpecial3`: 161
- (others)

Per-file Jungle:

- `StageJungle`: 92
- `GRJungleMap`: 261
- `DonkeyMain` family: (balance)

Both stages' textures get fixed via the **chain-walk path during
dependency-bank loads** — the work happens inside `MiscData086`
(chain:623), `ExternDataBank109` (chain:273), `FoxSpecial3`
(chain:238), and sibling banks, not during the top-level
`StageSector` / `StageJungle` load (which is why those direct counts
look low — see "Counter-reset gotcha" above).

**Conclusion for H4.** Textures do get fixed, and the DL dispatcher
does read them at frame time (57K+ runtime texture hits for Sector Z
alone). A texture-side bug would show up as **wrong-looking Sector Z
surfaces**, not **missing Sector Z hull**. The runtime data path for
textures is live and working.

---

## Visual evidence

Auto-demo screenshot survey across one full cycle:

| Stage                | Renders? | Notes                                              |
|----------------------|----------|----------------------------------------------------|
| Peach's Castle       | OK       | Tower visible — user report may be stale           |
| Kongo Jungle         | OK       | Floor, ropes, background all render                |
| Yoshi's Island       | OK       | Flower renders, no horizontal stripes seen          |
| Saffron City         | OK       | Buildings and rooftop render                       |
| Dream Land           | Largely OK | Trees render; user's "middle broken" not reproduced in this pass |
| **Sector Z**         | **BROKEN** | See below — 4 targeted captures                 |
| Zebes                | TBD      | Need longer capture window in next pass            |
| Hyrule Castle        | OK       | Tower present                                      |

### Sector Z targeted captures

At t≈100s, 105s, 110s, 115s, 120s (2nd demo cycle, inside the Sector Z
window), the frame shows:

- **Black background where Great Fox hull should be**
- **Orange engine-flame billboard renders** (floats in empty space)
- **Fighters rendered in empty space** (no ship deck visible)
- **No hull geometry** at any point of the capture window

The flame rendering correctly while the hull is absent is the critical
data point for the current hypothesis.

**User's report appears partially stale.** Several items
("Hyrule Castle tower missing", "Zebes just empty", "Yoshi flower
horizontal lines") do not reproduce cleanly under demo-cycle probe.
Sector Z reproduces definitively. Dream Land trees' middle requires a
longer capture window to confirm.

---

## Current leading hypothesis — Sector Z Great Fox hull DL

Evidence pulled together:

1. **DObj tree builds correctly** — Sector Z L1 produces 11 DObjs, most
   at origin with scale=1 and valid non-null DL tokens.
2. **DL dispatch IS running** — 57K+ `runtime.skip` texture dispatches
   for Sector Z files (99 StageSector + 262 GRSectorMap + 161
   FoxSpecial3 + rest) prove the DL walker is reaching Sector Z's DL
   chain and reading its tiles.
3. **Vertex-only DLs work** — the engine flame billboard (a simple
   vertex-color-only DL on a DObj in the same tree) renders fine. So
   the DL walker, vertex pipe, and primitive emit path are all alive
   in this frame.
4. **Hull DLs execute but emit no visible tris.**

### Candidate root causes (not yet tested)

- **Geometry / render mode mismatch.** `G_SETGEOMETRYMODE` /
  `G_CLEARGEOMETRYMODE` flipped bits (e.g. CULL_BACK vs CULL_FRONT) →
  every tri back-facing → zero coverage. Matches symptom: DL runs,
  textures load, no visible tris.
- **Matrix chain produces degenerate transform.** Pushing the wrong
  Mtx from segmented memory → determinant near zero → all tris clip
  out. Matches: hull geometry happens to live deeper in the Mtx chain
  than the flame, so only hull tris degenerate.
- **Sub-DL chain that resolves to an immediate `G_ENDDL`.** A
  `G_DL(branch)` into a segment whose first byte is already `G_ENDDL`
  would pop out of the hull draw without emitting anything but leave
  the DL walker happily continuing to the next DObj.
- **Color combiner / blend producing transparent output.** CC
  configured so every texel reads `(alpha=0)` regardless of texture
  content. Matches: textures still get touched (runtime.skip counts
  stay high) but nothing blends onto the framebuffer.

All four are consistent with the observed symptom set. Differentiating
them needs a GBI-level trace.

---

## Recommended next step

**GBI trace of one Sector Z frame vs one Jungle frame**, then diff.
Infrastructure already exists: `docs/debug_gbi_trace.md` documents both
the port-side capture and the M64P-plugin-side capture, plus
`gbi_diff.py` for structured comparison.

Expected signals in the diff:

- If CC/blend is the bug — Sector Z SetCombine / SetOtherMode differs
  from Jungle's in the alpha-output source channel.
- If geometry mode is the bug — SETGEOMETRYMODE argument differs
  (CULL_BACK vs CULL_FRONT bit).
- If matrix is the bug — the Mtx pushed around the hull draw has
  different-valued floats than expected, or zeroed columns.
- If sub-DL is the bug — a `G_DL` with a target that immediately
  dispatches `G_ENDDL` shows up in the trace.

**This investigation is currently underway in parallel to this
handoff.** The next session can pick up the trace output directly
without re-running everything.

---

## State of code modifications — DO NOT revert

All of the following are in place on `main` and must not be reverted
without a fresh re-read of the files:

### Build system

- **`CMakeLists.txt`** — `SSB64_STAGE_CYCLE_DEMO` option (OFF by
  default). Enabled builds propagate `-DPORT_STAGE_CYCLE_DEMO=1`.

### Scene manager / autodemo

- **`src/sc/scmanager.c`** (around line 875) — demo-mode scene
  override under `PORT_STAGE_CYCLE_DEMO`.
- **`src/sc/sccommon/scautodemo.c`** — `scAutoDemoExit` loop patch,
  `demo_fkind[]` seed with Mario/Fox, `scAutoDemoDetectExit` no-op,
  all under `#ifdef PORT_STAGE_CYCLE_DEMO`.

### Runtime instrumentation

- **`port/bridge/lbreloc_byteswap.cpp`** — `SSB64_STAGE_AUDIT=1` env
  var gates all counters. No-op when unset.
- **`port/bridge/lbreloc_bridge.cpp`** — calls
  `portStageAuditEmitLoadSummary` after each file load. No-op when
  `SSB64_STAGE_AUDIT` unset.

The audit infrastructure is the investigation's primary source of
data. Leave it in tree even after Sector Z's root cause lands —
future stage bugs will use the same counters.

### Modified (unrelated to this investigation)

- **`src/lb/lbcommon.c`** — working-tree modifications pre-existing
  this session's investigation; not part of the stage-rendering work.

---

## Related reading

- **`docs/bugs/ground_geometry_collision_2026-04-08.md`** — prior
  fix in the same subsystem (PORT_RESOLVE on ground-desc tokens +
  all-u16 collision struct byte-swap). Confirms the MPGround* path
  is no longer the likely culprit.
- **`docs/bugs/mpvertex_byte_swap_2026-04-11.md`** — MPVertexData
  byteswap deferral. Related but distinct from the stage-geometry
  DL path.
- **`docs/debug_gbi_trace.md`** — the tracing infrastructure to
  actually pin down which RDP state flips between working and broken
  stages.
- **`docs/bugs/color_image_to_zbuffer_draws_2026-04-20.md`** — note
  under "open risk": `GfxSpTri1` still lacks the color_image ==
  z_buf_address guard; the `mvOpeningRoom` transition case could in
  principle regress here. **Not the cause of Sector Z** (wrong
  symptom class, but worth flagging in any stage-rendering handoff).

---

## Tooling quick-reference

Reproduce the Sector Z break:

```
cmake -S . -B build -DSSB64_STAGE_CYCLE_DEMO=ON
cmake --build build --target ssb64
SSB64_STAGE_AUDIT=1 build/ssb64 > ssb64.log 2>&1 &
# wait ~100s into the 2nd cycle, screencapture
```

Inspect per-file dispatch counts:

```
grep 'portStageAuditEmitLoadSummary' ssb64.log | less
grep 'tex_fixup' ssb64.log | awk -F: '{…}'
```

Compare Sector Z vs Jungle texture paths:

```
grep -E 'Stage(Sector|Jungle)|GR(Sector|Jungle)Map|FoxSpecial3|DonkeyMain' ssb64.log
```

---

## GBI-trace diff results (addendum, captured later in the same session)

Ran `SSB64_GBI_TRACE=1 SSB64_GBI_TRACE_FRAMES=8000 SSB64_MAX_FRAMES=8200 ./build/ssb64`.
Extracted frame 5000 (mid-Jungle, cycle 4) and frame 6500 (mid-Sector, cycle 5)
via `awk` from the 2.3GB `debug_traces/port_trace.gbi`. Extracts saved at
`/tmp/frame_jungle.gbi` and `/tmp/frame_sector.gbi` (280 KB / 139 KB).

**Top-level counts:**

```
Jungle frame 5000:  3,463 commands
Sector frame 6500:  1,763 commands   ← 50% fewer
```

**Opcode distribution:**

| Opcode   | Jungle | Sector | Δ     |
|---       |---     |---     |---    |
| G_TRI2   | 355    | 162    | -193  |
| G_VTX    | 150    |  22    | -128  |
| G_DL     | 134    |  78    |  -56  |
| G_ENDDL  |  98    |  51    |  -47  |
| G_NOOP   |   0    | 276    | +276  |
| G_TEXRECT|  49    |  59    |  +10  |

Sector Z draws about half as much geometry as Jungle. The 276 G_NOOPs
are unique to Sector Z and cluster at d=4 in 20-ish-entry runs, each
bracketed by a G_DL call + POPMTX pair. Likely these are Arwing-slot
render passes during a frame with no active Arwings — expected empty
state, not the bug.

**Vertex address format:** Both stages' G_VTX commands carry
"token-style" raw w1 values (small integers like `0x111` or `0x8C6`).
Added temporary logging in `portRelocTryResolvePointer` — across the
full demo run, **0 resolution failures** on the token path. The larger
OOR values (`0xF000000`, `0xE000000+offset`) are classic N64 segment-0xE
addresses that SegAddr's fallback handles via `mSegmentPointers`.
libultraship's `Interpreter::SegAddr` (interpreter.cpp:3489) already
has the correct logic: token-lookup first, segment-fallback second.

So the token plumbing is fine. **Sector Z simply calls fewer DLs and
emits fewer tris.** The Great Fox hull isn't missing because its DL
addresses point at garbage — it's missing because the DL chain doesn't
*reach* the hull's geometry DLs, or reaches DLs that legitimately
return immediately.

**Next lead:** compare G_DL targets — which sub-DLs does Jungle call
that Sector Z doesn't? Frame 5000 has 134 G_DL cmds, frame 6500 has
78 — 60 calls worth of sub-DLs are absent from Sector Z. Some of those
almost certainly contain the Great Fox hull geometry. To drill in,
extract w1 values from both traces' G_DL lines, bucket by segment,
and diff by hull-offset patterns. Follow-up in a future session.

---

## Open questions for next session

1. **Identify the missing Sector Z G_DL targets.** Extracts live at
   `/tmp/frame_jungle.gbi` and `/tmp/frame_sector.gbi`; full trace at
   `build/debug_traces/port_trace.gbi` (2.3 GB).
2. **Zebes and Dream Land revisit.** User flagged both, demo-cycle
   didn't clearly reproduce. Need longer capture windows or specific
   camera framings.
3. **Yoshi's Island flower horizontal stripes.** If this is a
   sprite-decode-stride issue (per `docs/bugs/sprite_decode_stride_mismatch_2026-04-20.md`),
   the fingerprint is stripe-spacing = `width_img - width`. Measure
   from a screenshot if it reproduces.
4. **VS-mode crashes.** Separate class of bug from the rendering
   artifacts. Start with a stack trace from an actual crash under
   `SSB64_STAGE_AUDIT=1` — the per-file counters will narrow which
   stage file's load last completed before the fault.
