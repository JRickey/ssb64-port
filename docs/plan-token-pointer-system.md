# Plan: 64-bit Pointer Strategy for lbReloc File Loading

## Context

The lbReloc system loads binary blobs from ROM (now .o2r) and patches 32-bit pointer
slots via relocation chains. On 64-bit PC, `void*` is 8 bytes — writing it into a
4-byte slot corrupts adjacent data, and struct layouts change (e.g. DObjDesc goes
from 44 bytes to 52). The bridge code written earlier loads/caches files correctly
but the relocation itself is broken on 64-bit.

### Why pure typed resource parsing is impractical

Exploration revealed a critical fact: **reloc files are heterogeneous blobs, not
single-type arrays**. The game accesses sub-structures via byte offsets:

```c
// Typical pattern — base pointer + byte offset, cast to type
sprite = lbRelocGetFileData(Sprite*, file_base, offset_to_sprite);
model  = lbRelocGetFileData(DObjDesc*, file_base, offset_to_model);
anim   = lbRelocGetFileData(AObjEvent32**, file_base, offset_to_anim);
```

A single file can contain DObjDesc arrays, inline Gfx display lists, AObjEvent32
sequences, and pointer tables — all at different byte offsets. Writing per-file-type
parsers would require annotating the internal layout of all 2132 files. This is not
feasible.

## Recommended Approach: Token-Based Pointer Indirection

Keep files as flat blobs. Change pointer fields in data structs to 32-bit tokens.
The relocation system writes tokens (not 64-bit pointers) into the existing 4-byte
slots. A global table maps tokens to real 64-bit pointers.

**Why this works:**
- Struct sizes stay identical on all platforms (no layout/stride changes)
- File data is loaded as-is (no blob rewriting or expansion)
- The reloc chain still identifies pointer slots correctly
- Game code resolves tokens at access sites via a macro
- Torch extraction pipeline needs NO changes
- The existing RelocFile resource type is sufficient

## Current Status (2026-04-04)

### Phase 0: COMPLETE — Token System Infrastructure

All of the following compile and link into `ssb64.exe`:

| File | Purpose |
|------|---------|
| `port/resource/RelocPointerTable.h` | Token table API: `portRelocRegisterPointer()`, `portRelocResolvePointer()`, `RELOC_RESOLVE()` macro |
| `port/resource/RelocPointerTable.cpp` | Flat-array implementation, O(1) lookup, 256K initial capacity |
| `port/resource/RelocFileTable.h` | Maps file_id (0-2131) to .o2r resource path |
| `port/resource/RelocFileTable.cpp` | Auto-generated from yamls/us/reloc_*.yml (regenerate via `python tools/generate_reloc_table.py`) |
| `port/resource/RelocFile.h` | LUS Resource type holding decompressed file data + reloc metadata |
| `port/resource/RelocFileFactory.h/.cpp` | LUS factory: reads RELO resources from .o2r |
| `port/resource/ResourceType.h` | SSB64Reloc = 0x52454C4F ("RELO") |
| `port/bridge/lbreloc_bridge.cpp` | Full replacement of `src/lb/lbreloc.c` — loads from LUS ResourceManager, token-based relocation |
| `port/bridge/port_types.h` | Decomp type definitions (u32, s32, etc.) without pulling in `include/` which shadows system headers |
| `src/lb/lbreloc.c` | Wrapped in `#ifndef PORT` — excluded from port build |

The bridge (`lbreloc_bridge.cpp`):
- Loads files via `Ship::Context::GetInstance()->GetResourceManager()->LoadResource(path)`
- Copies decompressed data into the game's heap allocations (same memory semantics as original)
- Walks internal reloc chain: computes real pointer, registers as token, writes 32-bit token into 4-byte slot
- Walks external reloc chain: loads dependency file, computes target, registers token
- Maintains status buffer caching (identical to original)
- All functions have C linkage matching `src/lb/lbreloc.h` declarations

### Phase 1: COMPLETE — DObjDesc family struct changes + PORT_RESOLVE

6 structs in `objtypes.h` changed, 23 access sites wrapped, static_asserts added.
See Phase 1 section below for details.

### Phase 2: COMPLETE — AObjEvent32 struct change + PORT_RESOLVE (2026-04-04)

### Phase 3a: COMPLETE — Sprite/Bitmap struct changes + PORT_REGISTER (2026-04-04)

### Phase 3b: COMPLETE — FTAttributes + FTSprites + stock_luts (2026-04-04)

### Phase 4: COMPLETE — MObjSub sprites/palettes (2026-04-04)

### Phase 5: COMPLETE — LBScriptDesc / LBTextureDesc / LBTexture (2026-04-04)

## Affected Struct Types

Pointer fields in FILE DATA (loaded from .o2r) that need `#ifdef PORT` changes:

| Struct | Pointer field(s) | File |
|--------|-----------------|------|
| `LBRelocDesc` | `void *p` | `src/lb/lbtypes.h` |
| `DObjDesc` | `void *dl` | `src/sys/objtypes.h` |
| `DObjTraDesc` | `void *dl` | `src/sys/objtypes.h` |
| `DObjMultiList` | `Gfx *dl1, *dl2` | `src/sys/objtypes.h` |
| `DObjDLLink` | `Gfx *dl` | `src/sys/objtypes.h` |
| `DObjDistDL` | `Gfx *dl` | `src/sys/objtypes.h` |
| `DObjDistDLLink` | `DObjDLLink *dl_link` | `src/sys/objtypes.h` |
| `AObjEvent32` | `void *p` (union) | `src/sys/objtypes.h` |
| `MObjSub` | `void **sprites`, `void **palettes` | `src/sys/objtypes.h` |
| `Sprite` | `Bitmap *bitmap`, `Gfx *rsp_dl`, `Gfx *rsp_dl_next`, `int *LUT` | `include/PR/sp.h` |
| `Bitmap` | `void *buf` | `include/PR/sp.h` |
| `LBScriptDesc` | `LBScript *scripts[1]` | `src/lb/lbtypes.h` |
| `LBTextureDesc` | `LBTexture *textures[1]` | `src/lb/lbtypes.h` |
| `LBTexture` | `void *data[1]` | `src/lb/lbtypes.h` |

**NOT affected** (runtime-allocated, not in file data): GObj, DObj, MObj, AObj, CObj,
SObj, GObjProcess, LBGenerator, LBParticle, LBTransform — these are created by game
code at runtime with proper 64-bit pointers.

**NOT affected** (no C pointers): Gfx display lists are 64-bit words with segment
addresses handled by Fast3D. Raw textures, audio, animation keyframes are pure data.

## Remaining Implementation Phases

### Phase 1: DObjDesc — COMPLETE (2026-04-04)

Changed pointer fields to `u32` under `#ifdef PORT` in `src/sys/objtypes.h`:
- DObjDesc.dl, DObjTraDesc.dl, DObjMultiList.dl1/dl2, DObjDLLink.dl,
  DObjDistDL.dl, DObjDistDLLink.dl_link

Added `PORT_RESOLVE()` macro in `objtypes.h` — resolves tokens on PORT,
no-op passthrough on non-PORT. Wrapped 23 access sites across 10 files:
- `src/sys/objanim.c` (6), `src/sys/objdisplay.c` (8), `src/sys/objhelper.c` (2)
- `src/ft/ftparam.c` (2), `src/ft/ftmain.c` (1)
- `src/ef/efground.c` (2), `src/gr/grmodelsetup.c` (2), `src/it/itmanager.c` (2)
- `src/lb/lbcommon.c` (6), `src/sc/sc1pmode/sc1pgameboss.c` (2)

NULL checks (`field != NULL`) work as-is since token 0 == NULL.
`_Static_assert` size checks added for all 6 structs. Build passes clean.

**Note**: FTModelPart.dl and FTAccessPart.dl (in `src/ft/fttypes.h`) are also
file-data pointer fields that need the same treatment — discovered during Phase 1
but deferred to a new phase since they have additional pointer fields (mobjsubs,
matanim_joints) that belong to later phases.

### Phase 2: AObjEvent32 — COMPLETE (2026-04-04)

Changed `void *p` to `u32 p` under `#ifdef PORT` in AObjEvent32 union.
Union stays at 4 bytes. `_Static_assert` added.

Wrapped 9 access sites in `src/sys/objanim.c` with `PORT_RESOLVE()`:
- DObj animation: SetAnim (L513), Jump (L525), SetInterp (L561)
- MObj material animation: SetAnim (L1088), Jump (L1095)
- CObj camera animation: SetAnim (L2732), Jump (L2739), SetInterp (L2776, L2790)

All accesses are read-only. 6 are script pointer chaining (SetAnim/Jump),
3 are interpolation function pointer loads (SetInterp). Build passes clean.

### Phase 3a: Sprite / Bitmap struct changes — COMPLETE (2026-04-04)

Changed 5 pointer fields to `u32` under `#ifdef PORT` in `include/PR/sp.h`:
- `Bitmap.buf`, `Sprite.LUT`, `Sprite.bitmap`, `Sprite.rsp_dl`, `Sprite.rsp_dl_next`

Added `PORT_REGISTER()` macro in `objtypes.h` — wraps `portRelocRegisterPointer()`
to convert a runtime pointer to a token for storage in token fields.

`_Static_assert` size checks: `sizeof(Bitmap) == 16`, `sizeof(Sprite) == 68`.

Wrapped 14 access sites in lbcommon.c with `PORT_RESOLVE()`:
- `lbCommonDecodeSpriteBitmapsSiz4b`: sprite->bitmap (1), bitmap[].buf (3)
- `lbCommonDrawSObjBitmap`: bitmap->buf (7 — NULL check, cache compare, 4 gDPSetTextureImage, cache store)
- `lbCommonDrawSpriteSetup`: sprite->LUT (3 — gDPLoadTLUT ×2, cache compare)
- `lbCommonPrepSObjDraw`: sprite->bitmap (1)
- `lbCommonSetExternSpriteParams`: sprite->LUT (1)

Wrapped `unref_800162C8` in `#ifndef PORT` (objdisplay.c) — unreferenced N64
sprite draw that writes runtime Gfx* into rsp_dl_next (incompatible with u32).

`PORT_REGISTER()` at 7 direct LUT assignment sites (lbRelocGetFileData → u32 token):
- `src/mn/mnoption/mnbackupclear.c` (4 assignments, 1 PORT_RESOLVE comparison)
- `src/mn/mnplayers/mnplayersvs.c` (2), `mnplayers1ptraining.c` (2),
  `mnplayers1pgame.c` (1), `mnplayers1pbonus.c` (1)

Changed `sMNBackupClearOptionConfirmLUTOrigin` type to `u32` under PORT (mnbackupclear.c).

`#ifdef PORT` block in `sc1pstageclear.c:2133` — chained `->bitmap->buf` dereference
resolved through two PORT_RESOLVE steps.

Build passes clean on MSVC.

### Phase 3b: FTAttributes + FTSprites + stock_luts — COMPLETE (2026-04-04)

**FTAttributes** (`fttypes.h:870-973`) — 840-byte file-data struct with 21 pointer-
sized slots (13 distinct pointer fields + 8 `shield_anim_joints[]` elements). All
changed to `u32` under `#ifdef PORT`. `_Static_assert(sizeof(FTAttributes) == 0x348)`.

Changed fields: `setup_parts`, `animlock`, `hiddenparts`, `commonparts_container`,
`dobj_lookup`, `shield_anim_joints[8]`, `translate_scales`, `modelparts_container`,
`accesspart`, `textureparts_container`, `thrown_status`, `sprites`, `skeleton`.

~45 access sites wrapped with `PORT_RESOLVE()` across 8 files:
- `ftparam.c` (16), `ftmain.c` (9), `ftmanager.c` (6), `ftcommonguard1.c` (8)
- `ftdisplaymain.c` (4 — skeleton double-deref with nested PORT_RESOLVE)
- `ftcommonthrow.c` (2)

**FTSprites** (`fttypes.h:739-744`) — 3 pointer fields changed to u32:
`stock_sprite`, `stock_luts`, `emblem`. `_Static_assert(sizeof(FTSprites) == 12)`.

~16 sprites/stock_luts access sites wrapped in 4 files using `#ifdef PORT` blocks:
- `ifcommon.c` (12 — stock_sprite deref, stock_luts[costume] via u32* cast, NULL checks)
- `sc1pgame.c` (4 — sSC1PGameEnemyTeamSprites resolved at assignment)
- `mnvsresults.c` (2), `mnplayers1pgame.c` (2)

Build passes clean on MSVC.

### Phase 4: MObjSub sprites/palettes — COMPLETE (2026-04-04)

Changed 2 pointer fields to `u32` under `#ifdef PORT` in `src/sys/objtypes.h`:
- `void **sprites` → `u32 sprites` (token to array of texture image tokens)
- `void **palettes` → `u32 palettes` (token to array of palette tokens)

`_Static_assert(sizeof(MObjSub) == 0x78)` added.

Wrapped 3 access sites in `src/sys/objdisplay.c` with double PORT_RESOLVE:
- Line ~1264: `palettes[(s32)mobj->palette_id]` — palette lookup for MOBJ_FLAG_PALETTE
- Line ~1367: `sprites[mobj->texture_id_next]` — texture image for MOBJ_FLAG_FRAC|SPLIT
- Line ~1438: `sprites[mobj->texture_id_curr]` — texture image for MOBJ_FLAG_FRAC|ALPHA

Double-resolve pattern: `PORT_RESOLVE(((u32*)PORT_RESOLVE(mobj->sub.sprites))[index])`
— first resolve gets array base, cast to `u32*` (token array), index into it,
second resolve gets the actual texture/palette data pointer.

Note: `#ifdef PORT` blocks wrap entire `gDPSetTextureImage` calls (not inline in args)
because preprocessor directives cannot appear inside macro arguments.

Build passes clean on MSVC.

### Phase 5: LBScriptDesc / LBTextureDesc / LBTexture — COMPLETE (2026-04-04)

Changed 3 pointer array fields to `u32` under `#ifdef PORT` in `src/lb/lbtypes.h`:
- `LBScriptDesc.scripts[1]`, `LBTextureDesc.textures[1]`, `LBTexture.data[1]`

`_Static_assert` size checks: `sizeof(LBScriptDesc) == 8`, `sizeof(LBTextureDesc) == 8`,
`sizeof(LBTexture) == 28`.

Changed 2 static variable types in `src/lb/lbparticle.c` under `#ifdef PORT`:
- `sLBParticleScriptBanks` → `u32 *[]` (was `LBScript **[]`)
- `sLBParticleTextureBanks` → `u32 *[]` (was `LBTexture **[]`)

Wrapped access sites in `src/lb/lbparticle.c`:
- `lbParticleSetupBankID` (3 loops): `PORT_REGISTER` for offset→token conversion,
  `PORT_RESOLVE` + local `LBTexture *tex` for inner data resolution loops
- `lbParticleMakeChildScriptID`: `PORT_RESOLVE` for script bank lookup + texture flags
- `lbParticleMakePosVel`: same pattern as above
- `lbParticleCreateGenerator` (~25 lines): `#ifdef PORT` block with local `LBScript*`
  resolved once, texture bank flags resolved via `PORT_RESOLVE`
- Render function (~line 1857): `#ifdef PORT` block with local `LBTexture *tex`,
  double-resolve for `image`/`palette` from `data[]` tokens
- `p_palette` type changed to `u32*` under PORT (was `void**`)

Build passes clean on MSVC.

## Build Strategy for the Bridge

The bridge (`port/bridge/lbreloc_bridge.cpp`) needs both decomp types and LUS C++
APIs. The `include/` dir shadows system headers, so it can't go on the C++ target's
include path.

**Solution**: The bridge is a C++ file that includes decomp type headers from `src/`
(already on the path). For types from `include/` (like `ssb_types.h`), a thin
`port/bridge/port_types.h` provides the needed typedefs (u32, s32, u16, etc.)
using `<cstdint>` — no dependency on the decomp `include/` directory.

The bridge re-declares the structs it needs (LBFileNode, LBRelocSetup, LBInternBuffer,
LBTableEntry) locally — these MUST stay ABI-compatible with the decomp definitions
in `src/lb/lbtypes.h`.

## Known Issues / Future Work

- **Endianness**: RESOLVED — `portRelocByteSwapBlob()` in `port/bridge/lbreloc_byteswap.cpp`
  performs a two-pass byte-swap on each file blob after memcpy, before the reloc chain walk:
  Pass 1 blanket-swaps every u32 word (fixes DL commands, struct fields, reloc descriptors).
  Pass 2 parses native-endian DL commands to find vertex/texture regions and applies targeted
  fixups (vertex: rotate16 + byte-restore; texture 4b/8b: undo swap; 16b: rotate16; palette: rotate16).
  Remaining edge case: particle bytecode byte order (Phase 4, deferred until runtime testing).

- **Token table lifetime**: RESOLVED — `portRelocResetPointerTable()` now called at
  the top of `lbRelocInitSetup()`, which runs once per scene transition before any
  files are loaded. Prevents unbounded token growth and stale pointers to freed heaps.

## Verification

After each phase, build and verify:
1. `cmake --build build --target ssb64` links clean
2. No duplicate symbol errors (original lbreloc.c guarded by `#ifndef PORT`)
3. Struct sizes verified with `static_assert(sizeof(DObjDesc) == 44)` etc.
4. Token table round-trips: register a pointer, resolve it, get back the same value
