# DK Intro Wallpaper — Mushroom Kingdom Bug — Handoff (2026-04-26)

## RESOLVED 2026-04-26

Fast3D's `mTextureCache` keys on `{addr, fmt, siz, sizeBytes, masks, maskt,
w, h}` only — not the cached pixel bytes themselves. The bump-allocated
stage heap reuses heap addresses across scenes, and every stage's
wallpaper is the same shape (300×6 RGBA16 tiles at the same struct
offset). When a wallpaper tile in scene N+1 lands at the heap address a
prior scene's wallpaper tile occupied, `TextureCacheLookup` returns the
GPU texture object that was uploaded from the prior file's bytes.

Diagnostic confirmation (tail of `ssb64.log`, scene 45 wallpaper draws):

```
SSB64_RENDER_DIAG cache-hit path='reloc_stages/StageJungle' \
  addr=0x100b1bc70 file=92 off=0x1d120 fmt=0 siz=2 line=600 \
  fullLine=3600 size=3600 tileWH=300x6
```

Address, file_id (92 = StageJungle), and offset all correct — the cache
just had a stale upload at that key.

**Fix** (`8cfca40` / outer commit):
1. `Interpreter::TextureCacheDeleteRange(base, size)` evicts every cache
   entry whose `texture_addr` falls in the range; new `extern "C"
   portTextureCacheDeleteRange` exposes it to the port bridge.
2. `lbRelocLoadAndRelocFile` calls `portTextureCacheDeleteRange(ram_dst,
   copySize)` right next to the existing `portEvictStructFixupsInRange`,
   before `memcpy`. Same justification — bump-reset heap reuse — applied
   to the GPU-side cache.

The instrumentation that caught this (wallpaper buf reloc info dump,
`portDiagArmImportCapture`, `SSB64_DIAG_ARM_WALLPAPER_SCENES`) is left in
place for now; it's behind env vars so it's free at runtime.

The original handoff text below is preserved for context.

---


## Symptom

`mvOpeningJungle` (scene 45, DK + Samus on Kongo Jungle) renders the
**Mushroom Kingdom** wallpaper backdrop (Peach's Castle silhouette) instead
of Kongo Jungle. Foreground 3D stage is correct — only the painted
backdrop sprite is wrong.

## What's now confirmed

The data path is **fully innocent**. The bug is renderer-side in libultraship.

Probe: extended `port_log` in `grWallpaperMakeCommon` (`src/gr/grwallpaper.c`)
to dump the resolved Sprite struct, its first Bitmap, the bitmap buffer
token + resolved address, and the **first 16 bytes** of pixel data.

Comparison from `ssb64.log` on a natural-attract run that reproduces the
bug:

| field | scene 31 (renders correct Kongo Jungle) | scene 45 (renders Mushroom Kingdom) |
|---|---|---|
| `wp_tok` | 0x14e | 0x14e |
| sprite w/h/bmH/bmHreal | 220/300/6/5 | 220/300/6/5 |
| `bmfmt`/`bmsiz`/`nbm` | 0/0/552 | 0/0/552 |
| `buf_tok` | **0x121** | **0x121** |
| `bm[0]` first 16 bytes | `01 88 01 80 01 88 01 88 01 88 01 88 01 88 01 88` | **identical** |

Pointers differ only by the 16-byte alignment slack in the bump-allocator
between the two loads. The Sprite, the Bitmap, and the underlying pixel
buffer in RAM are byte-for-byte the **same Kongo Jungle wallpaper data**
that scene 31 paints correctly.

Previous open question: "wallpaper-skip workaround in `mvopeningroom`
made DK render correctly in one mid-investigation build." That's
consistent with a renderer-state-leak class of bug — fewer draws in the
prior scene shifts the offending bit of state.

## What's also confirmed

- `SSB64_START_SCENE=45` (skip the chain, boot directly into scene 45)
  → DK wallpaper renders correctly. Bug is reached only via the natural
  intro chain.
- Pause-before / resume-after the scene 45 transition fixes it (per
  prior doc).

So the offending state is built up by the chain leading into scene 45 and
is sensitive to inter-scene timing.

## What's ruled out

- The `gMPCollisionGroundData->wallpaper` reloc token (0x14e) resolves to
  the correct Sprite struct.
- The Sprite's `bitmap` reloc token (0x14d) resolves to the correct Bitmap.
- The Bitmap's `buf` reloc token (0x121) resolves to a buffer holding
  the correct Kongo Jungle wallpaper pixels.
- The Fast3D `TextureCacheKey` is keyed by raw `texture_addr` (plus
  fmt/siz/dims/etc.), so two stages' wallpapers cannot share a cache
  slot — different `texture_addr` → distinct entries
  (`libultraship/include/fast/interpreter.h:177-194`).
- Fighter intro scenes (30..37) are independent taskman tasks; gobj
  cross-scene leaks are unlikely. Heap addresses confirm the file-load
  buffer is reset between scenes (each scene allocates fresh, gd
  pointers shift but not by full file sizes).

## Most-likely remaining hypotheses

1. **Torch imported-asset-by-name lookup with a stage-keyed alias.**
   If the wallpaper sprite's `rsp_dl` issues a textured rectangle whose
   image is resolved by Ship resource path (rather than raw RDRAM
   pointer), and that path is something that varies by load order
   (e.g. "OPENING/wallpaper" registered with Inishie's bytes earlier
   in the chain), Fast3D would render those bytes regardless of what
   `gDPSetTextureImage(rawAddr)` says. Look for `mImportedTextures`
   handling in `libultraship/src/fast/interpreter.cpp` and the Torch
   yaml that emits the wallpaper sprite.

2. **TMEM/tile state survives across the scene transition.**
   Some scene `N` in the chain does a `LoadBlock` for Inishie's
   wallpaper into TMEM tile 0, sets up a tile descriptor, then
   relinquishes. Scene 45's wallpaper draw issues `gDPSetTextureImage`
   + sets up its own LoadBlock — but if Fast3D is short-circuiting
   (e.g. "tile already configured for this address-region, reuse")
   it could keep painting the prior bytes. Verify by adding a one-shot
   log inside `Interpreter::TextureCacheLookup` printing
   `key.texture_addr` for any draw at the wallpaper SObj's screen
   position, then diff scene 31 vs 45.

3. **Wallpaper SObj from a prior scene survives the taskman reset.**
   Lower probability given heap evidence, but worth a sanity check by
   logging `gGCCommonLinks[nGCCommonLinkIDWallpaper]` chain length at
   the top of `mvOpeningJungleFuncRun`.

The cheapest discriminator between (1) and (2) is to instrument
`Interpreter::TextureCacheLookup` (or equivalent) and print every
`texture_addr` looked up during scene 45's first wallpaper-rendering
frame. If scene 45 looks up address `0x104c4eb58` (its own buffer) →
hypothesis (2) and Fast3D is somehow short-circuiting earlier. If it
looks up some entirely different address that traces back to an Inishie
load → hypothesis (1) and we follow the imported-asset path.

## New instrumentation (2026-04-26 PM)

Two new pieces are in place to drive the next capture:

1. **Wallpaper buf reloc info.** `grWallpaperMakeCommon` now also logs the
   resolved bitmap buf's owning file_id, byte offset, file size, and
   resource path via `portRelocDescribePointer`. Look for the
   `[wallpaper] bm[0] reloc has=…` line right after the bytes dump.
   Confirms which O2R file actually backs the wallpaper pixels for each
   scene — we can then filter Fast3D diag by that file_id.

2. **Armed ImportTexture capture.** Added `portDiagArmImportCapture(int n)`
   in `libultraship/src/fast/interpreter.cpp`. Each call sets an atomic
   "log next N ImportTexture calls unconditionally" counter, bypassing
   the `SSB64_RENDER_DIAG_FILTER` / `_FILE_ID` gates and the slot
   limiter. `grWallpaperMakeCommon` arms it from the env list
   `SSB64_DIAG_ARM_WALLPAPER_SCENES=<scene,scene,…>` (default count
   4000, override with `SSB64_DIAG_ARM_WALLPAPER_N`). On a match it
   logs `[wallpaper] DIAG armed: capturing next N ImportTexture calls`.

### Capture command

```bash
SSB64_RENDER_DIAG=1 \
SSB64_DIAG_ARM_WALLPAPER_SCENES=31,45 \
SSB64_DIAG_ARM_WALLPAPER_N=4000 \
./build/ssb64
```

Let attract chain reach scene 31 (correct render) and then scene 45 (bug);
diff the two `SSB64_RENDER_DIAG import` blocks for the wallpaper-sized
draw (220×300, fmt=0/siz=0 ≈ RGBA16). Compare `addr=` and `file=` —
if scene 45's wallpaper-shaped draw points at a different file_id than
scene 31's, that's the smoking gun for hypothesis (1) or (2) in the
prior section.

## Files / state

- `src/gr/grwallpaper.c` — extended `grWallpaperMakeCommon` port_log
  with sprite + bitmap[0] + first-16-bytes dump. Useful baseline; safe
  to leave in place.
- `src/sc/scmanager.c` — `SSB64_START_SCENE=N` env override (already
  shipped 2026-04-25 in `ee3820e`).
- `src/mp/mpcollision.c` — `mpCollisionInitGroundData` already logs
  scene/gkind/file_id/gd.
- `ssb64.log` (natural-attract repro of 2026-04-26) — captured baseline
  for both correct (scene 31) and broken (scene 45) renders. Contains
  byte-identical evidence above.

## Where it fits in the bigger picture

This is the third Fast3D-renderer-side bug in this codebase whose
fingerprint is "data is correct, pixels on screen aren't":

- Whispy canopy (`memory/project_whispy_canopy_metal_fix.md`) —
  `mCurrentTextureIds[1]` aliasing the screen drawable on Metal.
- gDPSetPrimDepth (`docs/bugs/primdepth_unimplemented_2026-04-25.md`)
  — Z source for sprites coming from geometry mode instead of the
  PRIM-DEPTH register.
- This bug — wallpaper renders prior-scene-cached pixels.

Pattern: when game-side data verifiably matches a working case but
output diverges, jump straight to libultraship instrumentation rather
than chasing decomp-side fixes.
