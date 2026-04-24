# Training Mode Entry SIGSEGV — Wallpaper Pointer/Token Truncation (2026-04-24) — FIXED

**Symptom:** Starting Training Mode from 1P → Training crashes immediately.
Never reaches frame 1 of the training scene.

```
SSB64: scManagerRunLoop — entering scene 54
SSB64: syTaskmanStartTask — entered, arena_start=0x1012ec7f0 ...
SSB64: syTaskmanLoadScene — about to call func_start=0x100b9b5e4
SSB64: !!!! CRASH SIGSEGV fault_addr=0x34
0  lbCommonMakeSObjForGObj + 32
1  lbCommonMakeSObjForGObj + 28
2  lbCommonMakeSpriteGObj + 164
3  grWallpaperMakeStatic + 100
4  grWallpaperMakeDecideKind + 44
5  sc1PTrainingModeFuncStart + 92
```

## Root cause

`MPGroundData::wallpaper` has two very different types between N64 and
the port:

```c
// src/mp/mptypes.h
#ifdef PORT
    u32 wallpaper;         // reloc token (index into sPointerTable)
#else
    Sprite *wallpaper;     // raw pointer
#endif
```

`sc1PTrainingModeLoadWallpaper` (src/sc/sc1pmode/sc1ptrainingmode.c:661)
stores the **raw host pointer** returned by `lbRelocGetFileData` directly
into this field:

```c
gMPCollisionGroundData->wallpaper = (uintptr_t)lbRelocGetFileData(Sprite*, ...);
```

On N64 that's fine — the field is a `Sprite*`. On the port with LP64:

1. The 64-bit host pointer is truncated to 32 bits when assigned to
   `u32 wallpaper`. The low 32 bits of the pointer end up in the field.
2. `grWallpaperMakeStatic` (src/gr/grwallpaper.c:178) reads it back
   through `PORT_RESOLVE`:
   ```c
   (Sprite*)PORT_RESOLVE(gMPCollisionGroundData->wallpaper)
   ```
3. `PORT_RESOLVE` treats the value as a token into `sPointerTable`. The
   low 32 bits of a real host pointer are almost always `>= sNextToken`,
   so `portRelocResolvePointer` logs `invalid token` and returns NULL.
4. NULL `Sprite*` flows into `lbCommonMakeSObjForGObj`. `portFixupSprite`
   returns early on NULL, then the next line `sprite->bitmap` reads
   offset `0x34` (= `offsetof(Sprite, bitmap)`) off NULL → SIGSEGV with
   `fault_addr=0x34`.

This is the ONLY post-load write to `wallpaper` in the entire codebase;
regular stages get a valid token written by the byte-swap/reloc pass
when the stage's ground-data reloc file is loaded, so only training mode
trips this bug.

## Fix

`src/sc/sc1pmode/sc1ptrainingmode.c` — register the raw pointer as a
token before storing it, so `PORT_RESOLVE` round-trips:

```c
Sprite *sprite = lbRelocGetFileData(Sprite*, ...);
#ifdef PORT
    gMPCollisionGroundData->wallpaper = PORT_REGISTER(sprite);
#else
    gMPCollisionGroundData->wallpaper = sprite;
#endif
```

`PORT_REGISTER` (`portRelocRegisterPointer`) allocates a new token, maps
it to the pointer, and returns the token.

## Class of bug

Same family as the N64-pointer-as-u32 issues fixed earlier in the port,
but manifests specifically in fields whose **port struct declaration
already uses `u32` as a reloc token**. Any post-load code that writes a
raw host pointer into such a field needs `PORT_REGISTER`. Audit target:
anywhere PORT's `#ifdef PORT` branch promotes a pointer field to `u32`
and the decomp later assigns to it outside of a reloc-file-loading path.

## Not related to

- `007a82b` (VS stage-select crash — evict sprite-fixup idempotency on
  heap reuse): that fix handles stale `sStructU16Fixups` keys when bump
  heaps reuse addresses. It still runs for training mode via
  `lbRelocLoadAndRelocFile`, but the crash here is a pointer/token
  confusion at a level above that.
