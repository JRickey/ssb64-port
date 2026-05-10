# Linux/OpenGL Intro-Skip + Autonomous Samus Crash — Defensive `dl_link` Guard

**Resolved:** 2026-05-09 (Linux/OpenGL — defensive)
**Issue:** Latent Issue #128 family; surfaced by PR #144 (`free(sPrevHeap)`)
and libultraship `beb5b9a1` (`gfx_set_timg_handler` 4 GB tighten).
**Affects:** Linux/OpenGL only. macOS/Metal and Windows allocators don't
SIGSEGV on the same stale-pointer reads — their freed pages stay readable so
the underlying BSS leak silently corrupts but doesn't crash.

## Symptom

1. **Intro-skip crash.** Press Start during `mvOpeningRoom` → SIGSEGV in
   `Fast::Interpreter::ImportTextureRgba16/32`.
2. **Autonomous Samus pose SIGSEGV.** Letting the autonomous attract play,
   geometry corruption appears once Master Hand brings fighters to life;
   Samus's pose SIGSEGVs in `gcDrawDObjTreeDLLinks+0xba`. Crash insn is
   `cmp 0x14b69d0(,%rcx,8),%rax` where `%rcx = dl_link->list_id` —
   indexing past `sGCForwardDLs[4]`.

`v0.7.4-beta` was clean; bisect identified two post-v0.7.4 commits
(PR #144's `free(sPrevHeap)`; libultraship `beb5b9a1`). See the Session 1/2
handoff docs in `docs/` for the full bisect timeline.

## Root cause

A long-lived BSS holder retains a `DObjDLLink*` pointer into a previous
scene's heap. After scene transition the heap is recycled (or pre-band-aid,
`free()`'d) and the pointer dangles. `gcDrawDObjTreeDLLinks` walks
`dobj->dl_link` and reads through it, indexing `sGCForwardDLs[]` /
`gSYTaskmanDLHeads[]` with whatever the first `u32` of that memory holds.

Why Linux only: glibc `madvise(MADV_DONTNEED)`'s freed brk pages and
`munmap`'s freed mmap blocks (16 MiB scene heaps land above the mmap
threshold), so the dangling read SIGSEGVs deterministically. macOS Magazine
and Windows LFH typically retain freed pages in allocator caches, so reads
silently return junk instead of faulting.

The libultraship `beb5b9a1` tighten layered on top: it rejected any host
pointer below 4 GB, intending to catch unresolved N64 segment tokens. On
non-PIE Linux binaries the brk arena lives in the
`0x10000000`–`0x80000000` range, so the new guard dropped legitimate
widened SETTIMG `w1` payloads, blanking textures in scenes whose reloc
files happen to land there.

## Fix

Three-part defense-in-depth:

1. **Recycle the scene heap** (`decomp/src/sys/taskman.c:syTaskmanStartTask`).
   Replace `free + malloc(16 MiB)` per scene with a singleton 16 MiB
   allocation that's `memset(0)` between scenes. Same VA stays mapped+
   writable forever; stale reads on Linux now return zero/stale bytes (like
   macOS) instead of faulting. Calls `port_taskman_evict_arena_caches`
   (new wrapper in `port/bridge/lbreloc_bridge.cpp`) to clear the four
   port-side caches that hold pointers into the arena (DL widening,
   texture upload, struct fixup, reloc file ranges).

2. **PORT-only `dl_link` bound check** in `gcDrawDObjTreeDLLinks` and
   `gcDrawDObjDLLinks` (`decomp/src/sys/objdisplay.c`). Cast `list_id`
   through `u32` so any out-of-range value (negative, or `> ARRAY_COUNT`)
   trips a new bound check; cap iterations at 64 to handle the all-zero
   recycled-memory case where `list_id == 0` && `dl == NULL` would otherwise
   walk indefinitely past the relocData sentinel `{ ARRAY_COUNT, NULL }`.
   First occurrence per frame is logged via `port_log` for triage.

3. **Revert libultraship `beb5b9a1`** (`src/fast/interpreter.cpp`). Drop
   the new top-of-handler `< 4 GB` guard; restore the original 256 MB
   N64-segmented-range guard at the bottom of the handler. Catches
   unresolved tokens (always `≤ 0x0FFFFFFF` on hardware) without rejecting
   valid low-brk pointers.

Parts 1 and 3 together restore v0.7.4-equivalent Linux behaviour. Part 2
is a safety net that catches the symptom universally if the latent BSS
leak fires through any other future path.

## What's NOT fixed

The specific BSS holder of the stale `DObjDLLink*` is not identified. The
defensive guard catches the symptom but doesn't locate the upstream owner.

To resume the hunt: rebuild with `mallopt(M_MMAP_MAX, 0)` in `taskman.c`
(forces every `malloc` through the brk arena → makes scene-heap layout
deterministically reuse low addresses) plus restore `free(sPrevHeap)`.
The Samus crash then reproduces within ~7 scene transitions instead of the
~22+ when mmap addresses keep getting handed out fresh. The defensive
guard's `port_log` line gives `dobj`, `dl_link`, `dobj->dl_link` (root),
`list_id`, and frame number — enough breadcrumbs to walk back through
`gcCaptureTaggedGObjs` to whoever injected the stale GObj.

Suspected leak shapes (from Session 2 handoff):
- Effect-system free/eject lifecycle.
- Long-lived `DObjDLLink*` arrays in BSS or relocData populated by scene
  init with pointers into the per-scene arena.
- DObj `child`/`sib_next` cleanup on eject.

## Verification

Linux/OpenGL, non-PIE Clang build of main HEAD with all three parts
applied: autonomous attract runs cleanly through `mnStartup` → entire
`mvOpening*` per-character cutscene chain (including `mvOpeningSamus`,
the original crash point) and into the demo battle. Defensive guard
didn't fire — Parts 1 and 3 alone are sufficient on this hardware /
build configuration.

D3D11/Metal/Windows untouched: the band-aid behaves like their native
allocators; the PORT-gated guards in objdisplay.c don't compile on N64;
the libultraship revert restores upstream behavior.

## Class

Stale-pointer read in a hot draw loop, surfaced by glibc's aggressive
freed-page reclamation. Same family as
`g_vtx_unresolved_addr_guard_2026-05-03` (the same upstream BSS leak
surfacing through the G_VTX path).

## Audit hook

Any decomp draw path walking a sentinel-terminated array (`while
(p->id != ARRAY_COUNT)`) where the head pointer comes from a struct field
that may survive scene transitions is a candidate for the same defensive
guard. Other walks in `objdisplay.c` (`gcDrawDObjMultiList`,
`gcDrawDObjDistDLLinks`, dist-DL variants) match the shape but haven't
been observed crashing on Linux yet.

## References

- `decomp/src/sys/taskman.c:1316–1352` — Part 1 band-aid
- `port/bridge/lbreloc_bridge.cpp:146–162` — Part 1 eviction wrapper
- `decomp/src/sys/objdisplay.c:gcDrawDObjTreeDLLinks` — Part 2 guard
- `libultraship/src/fast/interpreter.cpp:gfx_set_timg_handler_rdp` —
  Part 3 revert
- `docs/linux_intro_skip_crash_handoff_2026-05-09.md` — investigation
- `docs/linux_intro_skip_crash_handoff_2026-05-09-session2.md` — bisect
  + recommended ship state
- `docs/bugs/g_vtx_unresolved_addr_guard_2026-05-03.md` — sibling
