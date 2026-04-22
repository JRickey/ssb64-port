# MPGroundData.layer_mask read as 0 after blanket u32 byteswap — 2026-04-22

**Symptom.** Sector Z Great Fox hull geometry missing. Other stages showing
partial rendering (Hyrule tower reports, Zebes empty reports, Dream Land trees
middle reports from the 2026-04-22 stage rendering handoff). Engine-flame
billboards and fighters still render, but the main hull geometry is absent.

**GBI trace fingerprint.** The frame's d=3 `G_DL` targets dispatch into
addresses that contain zero-opcode "NOOP runs" with sequential payloads:

```
[d=3] G_DL w1=047A1328
[d=4] G_NOOP w0=00000000 w1=00000131
[d=4] G_NOOP w0=00000004 w1=00000000
[d=4] G_NOOP w0=00000000 w1=00000132
[d=4] G_NOOP w0=00000004 w1=00000000
...  (runs of 20-32 commands per block)
```

Nine consecutive d=3 dispatches at 0x10 stride each yield zero drawn tris.
The pre-fix Sector Z frame 6500 had 276 `G_NOOP`s and 162 `G_TRI2`s; Jungle
at the same cadence had zero `G_NOOP`s and 355 `G_TRI2`s.

## Root cause

`MPGroundData.layer_mask` is a `u8` field living at byte offset 68 (inside a
4-byte-aligned u32 word) of the loaded stage header:

```c
struct MPGroundData {
    MPGroundDesc gr_desc[4];   // 0-63
    u32          map_geometry; // 64-67
    u8           layer_mask;   // 68
    u32          wallpaper;    // 72
    ...
};
```

The port's bridge performs a blanket `BSWAP32` over every loaded file's bytes
(pass 1). That swap reverses the four bytes of each u32 word. For the
`layer_mask` word, ROM layout `[M 00 00 00]` (where `M` is the real mask value
and the following three bytes are struct padding) becomes `[00 00 00 M]` in
memory. A native `u8` read at struct offset 68 returns `0x00` — the padding
byte, not `M`.

The consequence: `grDisplayMakeGeometryLayer` (in
`src/gr/grdisplay.c`) uses

```c
if (gMPCollisionGroundData->layer_mask & (1 << gr_desc_id))
    proc_display = dGRDisplayDescs[gr_desc_id].sec_proc_display;
else
    proc_display = dGRDisplayDescs[gr_desc_id].pri_proc_display;
```

to select the per-layer draw function. With `layer_mask` mis-read as 0, every
layer picks `pri_proc_display` (`gcDrawDObjTreeForGObj`) regardless of what
the stage author intended. The "secondary" draw path
(`gcDrawDObjTreeDLLinksForGObj`) treats the DObj's `dv` as a chain of
`DObjDLLink` records (`{s32 list_id; u32 dl_token}` pairs terminated by
`list_id == 4`). On affected stages, the DObj's `dv` is set up to point at
such a DObjDLLink chain — so dispatching it as a raw `Gfx*` walks the
chain bytes:

- entry 0: `{list_id=0, dl=<token>}` → decodes as `G_NOOP w1=<token>` + a
  second GBI command.
- entry 1 (terminator): `{list_id=4, dl=0}` → decodes as `G_NOOP w1=0`.

Fast3D's `portNormalizeDisplayListPointer` sees opcode 0 (G_NOOP is a valid
F3DEX2 opcode), accepts the command, and continues walking until it hits an
invalid opcode or runs out of room — then appends a synthetic `G_ENDDL`.
Result: the entire chain renders nothing, which is exactly the GBI-trace
pattern above.

## Why only some stages

Stages whose `layer_mask` is legitimately `0` (Pupupu in this audit) are
unaffected — the mis-read value equals the true value. Stages with
`layer_mask != 0` break. Observed during the 2026-04-22 stage cycle:

| Stage slot | Raw bytes at off 68 | True mask | Pre-fix read |
|------------|---------------------|-----------|--------------|
| Pupupu     | `00 00 00 00`       | 0x00      | 0x00 ✓       |
| (later)    | `00 00 00 02`       | 0x02      | 0x00 ✗       |
| (later)    | `00 00 00 03`       | 0x03      | 0x00 ✗       |

Byte at offset 68 is what the `u8` read picks up on LE. Byte at offset 71
is what the real mask is after the byteswap.

## Fix

New helper `portFixupStructU32(base, byte_offset, num_words)` in
`port/bridge/lbreloc_byteswap.cpp` that re-`BSWAP32`s a specified u32 word
(undoing pass 1 for that word). Idempotent via the existing
`sStructU16Fixups` tracker.

In `mpCollisionInitAll` (`src/mp/mpcollision.c`) where the existing s16-pair
fixups live, unswap the `layer_mask` u32 word so the `u8` field lands at its
declared offset:

```c
unsigned int lmask_off =
    (unsigned int)((uintptr_t)&gMPCollisionGroundData->layer_mask
                   - (uintptr_t)gMPCollisionGroundData);
portFixupStructU32(gMPCollisionGroundData, lmask_off & ~3U, 1);
```

## Verification

Pre-fix GBI trace of a Sector Z frame: 276 `G_NOOP`s, 162 `G_TRI2`s, 22
`G_VTX`s — the flame billboard draws but the hull is absent.

Post-fix GBI trace over 5,470 frames (full demo cycle plus partial second
cycle): **zero** `G_NOOP`s anywhere. Per-frame draw counts range 2,400–4,500
commands with healthy `G_TRI2`/`G_VTX` distributions. The Sector Z
DObjDLLink-as-raw-DL dispatch pattern is fully eliminated.

## Related

- `docs/stage_rendering_investigation_2026-04-22.md` — investigation handoff
  that led here.
- Same class of bug as [aobjevent32_halfswap](aobjevent32_halfswap_2026-04-18.md)
  and [mpvertex_byte_swap](mpvertex_byte_swap_2026-04-11.md): blanket pass-1
  byteswap shifts sub-u32 fields (u8/u16) out of their declared positions and
  specific structs need per-field fixups at load time.
- Any future u8 field in a loaded file struct that gates control flow (draw
  path, layer mode, flag bits) is a suspect — audit against the declaration-
  offset vs post-swap-offset mismatch.
