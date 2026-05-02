# SDL2 backend Destroy() uses uninitialized pointers — every-shutdown SIGSEGV

**Status:** RESOLVED 2026-05-01

**Where:** `libultraship` — `include/fast/backends/gfx_sdl.h`, `src/fast/backends/gfx_sdl2.cpp`. Commit `053ec17` on `JRickey/libultraship#ssb64`.

## Symptom

Every clean shutdown of the Linux AppImage builds segfaulted in libSDL2 *after* the main loop exited:

```
SSB64: main loop exited cleanly at frame=120 (WindowIsRunning=0)
SSB64: Game coroutine destroyed
SSB64: PortGameShutdown returned
SSB64: !!!! CRASH SIGSEGV fault_addr=0x596abcf39da3
SSB64: pc=0x7969fabf868f lr=0x0 sp=0x... fp=0x596abcf39ce3
/tmp/.mount_BattlehOPEia/usr/lib/libSDL2-2.0.so.0(+0x5468f)[0x7969fabf868f]
```

Reproducible on a 2-second session that never reached gameplay, ruling out save-data, scene-state, or BTP-completion theories. PC always at `libSDL2-2.0.so.0+0x5468f`. `lr=0x0` and the unwinder always died after one frame, indicating a freed-or-garbage struct read inside SDL2 itself. `fault_addr` always sat near `fp` in the heap region.

## Root cause

`GfxWindowBackendSDL2` declared its three SDL handles **without member initializers** and used `= default` for the constructor, so the fields contained heap garbage at construction:

```cpp
SDL_Window* mWnd;        // garbage
SDL_GLContext mCtx;      // garbage
SDL_Renderer* mRenderer; // garbage
```

`Init()` chooses one of two mutually exclusive paths:

- OpenGL (default on Linux/Windows): sets `mWnd` and `mCtx`, **never touches `mRenderer`**.
- Metal (macOS default for SSB64): sets `mWnd` and `mRenderer`, **never touches `mCtx`**.

`Destroy()` then unconditionally fed every field — including the leftover garbage one — to SDL:

```cpp
SDL_GL_DeleteContext(mCtx);     // garbage on Metal
SDL_DestroyWindow(mWnd);
SDL_DestroyRenderer(mRenderer); // garbage on OpenGL
SDL_Quit();
```

SDL_DestroyRenderer is documented to no-op on `NULL`, but a non-`NULL` garbage pointer dereferences arbitrary memory — exactly what the Linux AppImage logs showed. SDL_GL_DeleteContext is *not* documented to accept `NULL` and exhibits the same hazard on macOS Metal.

A second, latent bug compounded it: the destroy order put `SDL_DestroyRenderer` *after* `SDL_DestroyWindow`. SDL2 renderers hold a back-reference to their window and dereference it during teardown, so even if both pointers were valid, the renderer would have used freed window memory.

## Fix

1. Zero-init the three pointer members in the header:

   ```cpp
   SDL_Window* mWnd = nullptr;
   SDL_GLContext mCtx = nullptr;
   SDL_Renderer* mRenderer = nullptr;
   ```

2. Null-guard each call in `Destroy()` and reorder so renderer dies before window:

   ```cpp
   if (mRenderer) { SDL_DestroyRenderer(mRenderer); mRenderer = nullptr; }
   if (mCtx)      { SDL_GL_DeleteContext(mCtx);     mCtx      = nullptr; }
   if (mWnd)      { SDL_DestroyWindow(mWnd);        mWnd      = nullptr; }
   SDL_Quit();
   ```

## Verification

macOS Metal smoke test that reliably reproduced the crash before the fix:

```sh
SSB64_MAX_FRAMES=120 ./BattleShip
```

Post-fix log ends with `SSB64: PortGameShutdown returned` and the process exits 0 — zero crash markers, zero `fault_addr` lines. Same fix covers the Linux/OpenGL path because `mRenderer` is the symmetric uninit field there.

## Class

Use-after-uninit in C++ aggregate types whose path-dependent `Init()` does not set every member. Two cheap defenses any time a class wraps multiple optional native handles: (a) member-initialize all of them to `nullptr` so `Destroy()` can safely null-guard; (b) match SDL2's required `Renderer → Window` teardown order even when both handles look set.

## Diagnostic fingerprint

- **PC inside libSDL2 at a fixed offset** (here `+0x5468f`) on every shutdown.
- **`lr = 0` and unwinder produces a single frame** — the SDL function read a vtable / struct field through a garbage pointer and immediately faulted, no real call frame to walk.
- **`fault_addr` near `fp` in the heap region** — both are random heap addresses that happen to be close because the garbage pointer came from the same bump allocator that placed the surrounding objects.
- **Reproducible regardless of session length, scene, or save data** — independent of any in-game state.

If a shutdown SIGSEGV ever shows these four properties together, suspect another path-dependent `Init()` / unconditional `Destroy()` mismatch in libultraship.
