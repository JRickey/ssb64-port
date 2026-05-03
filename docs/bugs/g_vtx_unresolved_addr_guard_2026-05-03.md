# G_VTX handler dereferences unresolved N64 segment address — 2026-05-03

## Symptom

ssb64-port issue #103. v0.7-beta on Linux. SIGSEGV in
`Fast::Interpreter::GfxSpVertex+0x84` with `fault_addr=0x248d2a`. Reporter
hit it twice, both times after extended classic-mode play. Trigger pattern:
gameplay scene exit (e.g. classic round → next round screen, BTT → CSS).
Visible precursors before the crash: "weird flickering green head of DK with
no eyes" on CSS hover, music cuts to silence, severe lag, then crash on
hovering more fighters. Not reliably reproducible.

Crash dump (excerpt):

```
SSB64: !!!! CRASH SIGSEGV fault_addr=0x248d2a
SSB64: pc=...64bc94 lr=0x0 sp=...0a30 fp=0x248d2a
SSB64: x3=0x248d2a x5=0x77b30e18b730   # x5 = figatree pointer
/.../BattleShip(_ZN4Fast11Interpreter11GfxSpVertexEmmPKNS_6F3DVtxE+0x84)
```

`fp = x3 = fault_addr = 0x248d2a` — sub-32-bit fault address that ARG3
(the `vertices` parameter to `GfxSpVertex`) was supposed to be a host
pointer to.

## Root cause (proximate)

`Interpreter::SegAddr` (libultraship `src/fast/interpreter.cpp:4013`) tries
three resolution paths for a GBI command word:
1. `portRelocTryResolvePointer((uint32_t)w1)` — port-side reloc token table
2. `mSegmentPointers[(uint32_t)(w1 >> 24)]` + low 24 bits — N64 segment table
3. **Falls through and returns `(void*)w1` unchanged.**

If a GBI command stream contains a vertex address that's neither a known
reloc token nor a populated-segment N64 address, the raw value flows to
`gfx_vtx_handler_f3dex2` → `GfxSpVertex` → `vertices[i].v.ob[0]` and faults.

Upstream PR #1042 (LUS commit `67861a9`) added the same guard for
`gfx_set_timg_handler_rdp` (SETTIMG) but never propagated it to the three
G_VTX handlers (`f3dex2`, `f3dex`, `f3d`). Issue #103 hits the gap.

## Root cause (upstream — still unknown)

The proximate fix prevents the SIGSEGV but does not explain *why* a small
N64-shaped value is reaching `gfx_vtx_handler_f3dex2`'s `cmd->words.w1` in
the first place. Candidates, ordered by family fit:

1. **Heap-reuse / state-leak across scene transitions.** SSB64's PC port
   doesn't re-DMA overlay BSS the way N64 does (see
   `feedback_overlay_bss_audit`). A scene transition reuses heap regions
   that retain partially-overwritten data from a prior scene's display
   list, which then gets walked as GBI. Same family as the BTT result-screen
   leak (`btt_result_screen_state_leak_2026-04-30.md`).
2. **Fixup idempotency on heap reuse** (`project_fixup_idempotency_heap_reuse`).
   `sStructU16Fixups`/`sDeswizzle4cFixups` key on pointer; bump-reset heaps
   reuse addresses. If a freshly-loaded scene's data lands at an address
   that's still in the fixup cache, the new bytes get skipped → garbage
   GBI.
3. **CSS material-setup sub-DL stride.** Same family as
   `display_list_widening_2026-04-06.md` and
   `94926c9` (segment 0x0E stride mismatch) — runtime-built sub-DLs whose
   stride or addressing is mis-resolved.

The reporter's CSS visual symptoms (flickering green DK with no eyes, audio
cut) confirm GBI-stream-corruption-already-happened *before* the SIGSEGV.
The crash is the last consequence; the bad pointer was seeded several frames
upstream.

## Fix (defensive — proximate only)

Bump `libultraship` to add the same `<= 0x0FFFFFFF` filter from PR #1042 to
the three `gfx_vtx_handler_*` functions in `src/fast/interpreter.cpp`. Each
guard logs `n_vertices`, `dest`, and the raw `w1` via `SPDLOG_ERROR` so the
next user repro yields a diagnostic instead of just a fault address.

Skipping the G_VTX leaves `mRsp->loaded_vertices[dest..]` uninitialized for
that load — subsequent G_TRI1/G_TRI2 commands may render garbage triangles
until the next valid G_VTX overwrites the slots. This is preferable to a
hard crash.

## What this fix does NOT do

- Does not prevent the visual corruption upstream of the crash.
- Does not address why the bad pointer is reaching the GBI stream.
- Does not propagate the same guard to the OTR-filepath VTX handler at
  line 4453 (resource manager path; different failure mode — bypassed by
  port).

## Audit hook

Any new SIGSEGV in `Fast::Interpreter::GfxSp*` with a sub-`0x0FFFFFFF`
`fault_addr` is the same family — a raw N64 address leaking past `SegAddr`'s
fallback. Check for an `SPDLOG_ERROR` log line matching `G_VTX(...) skipped:
unresolved addr=0x...` to confirm and to get the calling-DL context.
