# Stage Struct ROM-Layout Audit — 2026-04-24

Target: explain why "some stages render properly, some don't" from struct-layout side (stage render vertex streams via Fast3D are out of scope).

**Scope:**
- `src/mp/mptypes.h`: `MPGroundData`, `MPGroundDesc`, `MPGeometryData`, `MPVertexData`, `MPVertexInfo`, `MPVertexLinks`, `MPLineData`, `MPLineInfo`, `MPMapObjData`, `MPItemWeights`, `MPYakumonoDObj`, `MPCollData`.
- `src/gr/grtypes.h`: `GRAttackColl`, `GRHazard`, `GRObstacle`, `GRDisplayDesc`, `GRFileInfo`, `GRStruct` union.
- `src/gr/grvars.h`: per-stage `GRCommonGroundVars*` / `GRBonusGroundVars*` variants.
- `src/gr/grcommon/`, `src/gr/grbonus/`: stage init code.

---

## 1. Summary — observed stage rendering issues mapped to struct causes

| Stage | Known / likely struct-layout cause |
|---|---|
| **Break the Targets (1P) — all 12 fighter variants** | `sc1pbonusstage.c:452-453` reads `gMPCollisionGroundData->gr_desc[1].dobjdesc` as a `uintptr_t` without `PORT_RESOLVE`, producing a low-address junk pointer. Targets' DObjDesc + animation-joint loads dereference that → targets can't be laid out / crash / invisible. **High-confidence bug.** |
| **Final Destination (Master Hand fight)** | `sc1pgame.c:1571/1573/2042/2044` and `sc1pgameboss.c:1020` build `file_head` from `(uintptr_t)gr_desc[1].dobjdesc - llGRLastMapFileHead` with no `PORT_RESOLVE`. Boss camera animations + boss `file_head` arithmetic land on junk → visual/animation breakage on Final Destination. **High-confidence bug.** |
| **Any battle stage with ambient ground effects (falling leaves, fireflies, bubbles, etc.)** | `efground.c:1594` computes `sEFGroundActor.file_head` via the same unresolved `gr_desc[1].dobjdesc` cast. Drops a junk base into the effect actor; downstream effect decodes garbage or crashes. Affects every stage whose `dEFGroundDatas[gkind]` entry is non-NULL. **High-confidence bug.** |
| **Sector Z (arwing hull missing)** | Already known / fixed by `portFixupStructU32` on `MPGroundData.layer_mask` (see `project_mpgrounddata_layer_mask_u8.md`). Noted for completeness — not a new finding. |
| **All stages — player-magnify fog overlay** | `MPGroundData.fog_color` (3 u8) + `fog_alpha` (u8) live inside one u32 at offset 0x4C that gets blanket pass1 BSWAP32'd; nothing un-bswaps it. Reads pick up byte-reversed RGB for the magnify-lens overlay (`ifcommon.c:1600`). **Cosmetic; not why a stage fails to load.** |
| **All stages — player emblem colors** | Same class as fog: `emblem_colors[GMCOMMON_PLAYERS_MAX]` is 4 packed `SYColorRGB` (12 u8) at 0x50..0x5C. Three u32 words each get pass1-bswapped with no undo. Player-emblem sprite RGB (`ifcommon.c:995-997`) reads scrambled bytes. **Cosmetic.** |
| **Dream Land (Whispy canopy striping)** | Not a stage-vars struct issue. See existing `project_whispy_curr_eq_next.md` — it's a matanim texture_id_next problem inside gm/gc code, not in `MPGroundData`/`GRStruct`. |
| **Everything else (Castle, Zebes, Yoshi, Kongo, Hyrule, Saffron, Mushroom, Dream Land base rendering)** | No struct-layout issue found in the audited files. Battle-mode stages all use `mpCollisionInitGroundData`'s fixup path which is correct for geometry pointers. Render issues on these stages — if any remain — are outside this audit (Fast3D display-list, texture, or matanim). |

---

## 2. Inventory and classification

Classification key:
- **A** = has `#if IS_BIG_ENDIAN` bitfields → full step 1–5 disasm audit needed.
- **B** = plain-typed, contains pointers or mixed u8/u16 fields inside u32 words → check PORT-token coverage and fixup-range coverage.
- **C** = purely scalar u32/f32, no subword fields, no pointers → pass1 BSWAP32 alone is correct.

### `src/mp/mptypes.h`

| Struct | Class | Notes |
|---|---|---|
| `MPGroundData` | **B** | Contains 4× `MPGroundDesc`, u32 pointer tokens, u8 `layer_mask`, u8 `fog_alpha`, `SYColorRGB` (3×u8) fields, `SYColorRGB emblem_colors[4]`, and trailing s16 bounds. Many fixups already in place (see `mpcollision.c:3998+`). New findings below. |
| `MPGroundDesc` | **B** | 4 pointer tokens on PORT. Field offsets match ROM 0x10 stride. No subword fields. Pass1 BSWAP32 alone is correct. **OK.** |
| `MPGeometryData` | **B** | u16 + 4 u32 pointer tokens + u16 + u32. Fixup at `mpcollision.c:4001-4002` covers both u16 fields. **OK.** |
| `MPVertexData` | **B** | Vec2h (2 s16) + u16 `vertex_flags` = 6 bytes. Fixup via packed-array rotation `mpcollision.c:4074`. **OK.** |
| `MPVertexInfo` | **B** | {u8, u8, s16, s16, s16, s16} = 10 bytes. Runtime-allocated, not ROM-loaded (see `mpCollisionAllocVertexInfo`). **OK.** |
| `MPVertexLinks` | **C** | {u16, u16} = 4 bytes = 1 u32. Fixup at `mpcollision.c:4035`. **OK.** |
| `MPLineData` | **C** | {u16, u16}. Inside `MPLineInfo` fixup. **OK.** |
| `MPLineInfo` | **B** | u16 + 4× `MPLineData` = 18 bytes. Fixup `mpcollision.c:4023`. **OK.** |
| `MPMapObjData` | **C** | {u16, Vec2h} = 6 bytes all u16. Fixup `mpcollision.c:4014`. **OK.** |
| `MPItemWeights` | **C** | `u8 values[1]`. Byte-indexed. No fixup needed (byte reads are endian-invariant). **OK.** |
| `MPYakumonoDObj` | **C** | Array of `DObj*` pointers. Runtime-allocated. **OK.** |
| `MPCollData` | **C** | Huge runtime state only, never ROM-loaded. **OK.** |

### `src/gr/grtypes.h`

| Struct | Class | Notes |
|---|---|---|
| `GRDisplayDesc` | **C** | Compile-time C array `dGRDisplayDescs[]` in `grdisplay.c`. Function pointers set at link time. Not ROM-loaded. **OK.** |
| `GRAttackColl` | **C** | 7× s32 = 28 bytes, all word-sized. ROM-loaded by Zebes acid & Inishie powerblock via `lbRelocGetFileData`. Pass1 BSWAP32 handles every field. **OK.** |
| `GRObstacle`, `GRHazard` | **C** | In-memory only (GObj* + function pointer). Not ROM-loaded. **OK.** |
| `GRFileInfo` | **Special** | `intptr_t file_id, offset` — on LP64 each entry is 16 bytes (vs. 8 on N64). **This is fine** because the arrays (`dMPCollisionGroundFileInfos`, `dMNMapsFileInfos`) are compile-time C initializers of symbolic constants — no ROM data is being indexed by this layout. **OK.** |
| `GRStruct` union | **C** | Union of per-stage variant structs; allocated in RAM, never ROM-loaded. Per-variant details in next section. |

### `src/gr/grvars.h` — per-stage variants

All of these are **runtime** structs (game state held in `gGRCommonStruct` at `grcommon/*.c`). Not ROM-loaded. Filled in by per-stage `grXInitAll` with runtime pointers / computed values. **All Class C from the ROM-load perspective.**

| Variant | Stage | ROM-loaded? | Notes |
|---|---|---|---|
| `GRCommonGroundVarsPupupu` | Dream Land | no | Pointers + u8/u16 state; all set at runtime. |
| `GRCommonGroundVarsSector` | Sector Z | no | `map_file` is a runtime-computed file-head pointer. |
| `GRCommonGroundVarsZebes` | Zebes | no | `attack_coll` loaded separately via `lbRelocGetFileData`. |
| `GRCommonGroundVarsYoster` | Yoshi's Island | no | `clouds[3]` runtime DObj refs. |
| `GRCommonGroundVarsInishie` | Mushroom K. | no | `pblock_pos_ids` set from a resolved map_head offset. |
| `GRCommonGroundVarsJungle` | Kongo Jungle | no | Tarucan state. |
| `GRCommonGroundVarsHyrule` | Hyrule Castle | no | Tornado state. |
| `GRCommonGroundVarsYamabuki` | Saffron City | no | Monster/gate state. |
| `GRCommonGroundVarsCastle` | Peach's Castle | no | Bumper state. |
| `GRBonusGroundVarsBonus1/2/3` | Targets / Platforms / Race | no | Runtime counters + `interface_gobj`. |

`GRSectorDesc` (Sector Z Arwing descriptor) — defined in `grvars.h:65-88` — **IS ROM-loaded**. Already has correct `#ifdef PORT` u32 token alternates for all 4 `AObjEvent32*` + 2 `void*` pointers, and uses `filler_0x4[0x1C-0x4]` = 24 bytes of pass-through. All fields are pointer-sized on ROM; pass1 BSWAP32 handles them correctly. **OK.**

---

## 3. Per-struct findings

### `MPGroundData` — two new bugs, both cosmetic

Offsets (LP64, after `gr_desc[4]` = 0x40):

```
0x40  u32 map_geometry          (token, pass1-swapped) OK
0x44  u8  layer_mask            +3 pad — fixed by portFixupStructU32 lmask_off
0x48  u32 wallpaper             (token) OK
0x4C  SYColorRGB fog_color (3×u8) + u8 fog_alpha  ONE u32 WORD — no undo of pass1
0x50  SYColorRGB emblem_colors[4] (12 u8)         THREE u32 WORDS — no undo of pass1
0x5C  s32 unused
0x60  Vec3f light_angle
0x6C  s16×4 camera_bound_*      fixup 4 words at bounds_off OK
0x74  s16×4 map_bound_*         covered by bounds_off fixup OK
0x7C  u32 bgm_id
0x80  u32 map_nodes             (token) OK — always resolved via PORT_RESOLVE
0x84  u32 item_weights          (token) OK — always resolved via PORT_RESOLVE
0x88  s16 alt_warning … 0xA6 zoom_end.z   fixup 8 words at team_off OK (2-byte overshoot into post-struct padding; inconsequential)
```

**Bug #1 — `fog_color` / `fog_alpha` byte order.** After pass1 BSWAP32 the u32 at 0x4C has bytes [fog_alpha, b, g, r] instead of [r, g, b, fog_alpha]. Reads in `ifcommon.c:1600` get `color->r = old fog_alpha`, `.g = b`, `.b = g`. `fog_alpha` is documented as "Unused padding?" and the magnify overlay renders with transposed RGB. Cosmetic on the zoom-in lens effect.

**Bug #2 — `emblem_colors[4]` byte order.** Same class: three u32 words at 0x50..0x5C hold 4 × {r,g,b} packed. Each word reverses to {b_next, g_n, r_n, b_prev?}. Player emblem sprite tinting (`ifcommon.c:995-997`) reads scrambled bytes. Each player's emblem is tinted wrong but deterministic. Cosmetic.

**Fix shape (for both):** add a `portFixupStructU32(gMPCollisionGroundData, 0x4C, 4)` in `mpCollisionInitGroundData` to undo pass1 for the 4 u32 words covering `fog_color`+`fog_alpha`+`emblem_colors[4]`. (Same pattern already used for `layer_mask`.)

### `MPGroundDesc` — OK

Four pointer-sized fields on PORT as `u32` tokens (`dobjdesc`, `anim_joints`, `p_mobjsubs`, `p_matanim_joints`). ROM stride 0x10. `grdisplay.c:200+208` always resolves with `PORT_RESOLVE` — correct.

**Systemic issue with callers (not the struct itself):** 7 call sites in `sc1pmode/*.c` and `ef/efground.c` do `(uintptr_t)gr_desc[1].dobjdesc` without `PORT_RESOLVE`. See §1 stage table. Listed inline here for grep convenience:

- `src/sc/sc1pmode/sc1pbonusstage.c:452` — Break the Targets dobjdesc
- `src/sc/sc1pmode/sc1pbonusstage.c:453` — Break the Targets anim_joints
- `src/sc/sc1pmode/sc1pgame.c:1571, 1573` — Final Destination camera anim
- `src/sc/sc1pmode/sc1pgame.c:2042, 2044` — FD boss-defeat camera anim
- `src/sc/sc1pmode/sc1pgameboss.c:1020` — Master Hand file_head
- `src/ef/efground.c:1594` — ambient effects file_head

All seven need `PORT_RESOLVE(gMPCollisionGroundData->gr_desc[1].dobjdesc)` in place of the bare cast. Pattern is identical to fixes already applied in `grinishie.c:568`, `grpupupu.c:666`, etc.

### `MPGeometryData` — OK

Verified offsets and fixup range. `mpcollision.c:4001-4002` covers both u16s. All four pointer tokens are resolved via `PORT_RESOLVE(gdata->vertex_data)` etc. at `mpcollision.c:4004-4007`.

### `MPVertex*` family — OK

Fixup coverage driven by counts derived at runtime (`gMPCollisionLinesNum`, `max_vid_index`, `max_vpos_index + 1`). Already audited in `project_mpvertex_byte_swap` / `project_mpgrounddata_layer_mask_u8`. No new findings.

### `MPLineInfo` / `MPLineData` / `MPMapObjData` — OK

All u16-packed; fixup widths are correct in `mpcollision.c:4014-4023`.

### `GRAttackColl` — OK

All 7 fields are s32 at 4-byte alignment. ROM-loaded for Zebes (`grzebes.c:108`) and Inishie (`grinishie.c:533`) via base + offset arithmetic. Pass1 BSWAP32 alone is correct for every field. No u16/u8 subword packing.

### `GRDisplayDesc` — OK

Compile-time table. `dl_link` is set as a C literal — no ROM load.

### `GRSectorDesc` — OK

Properly declared with PORT/BE branches; 24-byte `filler_0x4` preserved between pointers; sizeof matches ROM 0x30. No issues.

### `GRStruct` union and per-stage variants — OK

All runtime state, allocated by `gcMakeGObj*`-family code paths. No ROM load on these structs. The "bloated by LP64 pointer widening" worry raised in the task prompt doesn't bite because:

1. Nothing reads these variants by computed offset out of a file.
2. Widening the union shifts nothing else — each variant sits as a whole at the same union base.
3. Runtime allocation doesn't care that `void *map_head` is 8 bytes instead of 4.

---

## 4. Recommended fix priority

| # | Fix | Impact | Confidence | Effort |
|---|---|---|---|---|
| 1 | Add `PORT_RESOLVE` to the 7 `gr_desc[1].dobjdesc` cast sites | Unblocks Break the Targets (12 fighters), Final Destination boss camera, ambient ground effects on every battle stage | High | 10 min, 7 one-liners |
| 2 | Extend `mpCollisionInitGroundData` fixup to cover `fog_color`+`emblem_colors` (offset 0x4C, 4 words) | Fixes magnify-lens RGB and player emblem RGB | High | 2 lines in `mpcollision.c` |
| 3 | No action needed for the rest of the stage structs audited — classification and coverage are correct | — | — | — |

Fix #1 is the high-value item and has the strongest mapping to "some stages render properly, some don't" — specifically the 1P-mode stages and any battle stage whose `dEFGroundDatas` entry is populated. Fix #2 is a cosmetic follow-up.

---

## 5. Out-of-scope / reaffirmed known bugs

- Whispy canopy stripes — `project_whispy_curr_eq_next.md`, matanim problem, not struct-layout.
- Sector Z arwing hull — already fixed by `portFixupStructU32(lmask_off & ~3U, 1)`.
- Yoster 5-stripe horizontal band — `project_loadtile_fixup_stride.md`, sprite decoder not struct-layout.

No new bitfield-class (A) structs were found in the audited files — the stage-side ROM data is plain-typed apart from the union-of-pointer-tokens pattern, so no IDO BE probe-disasm pass was needed here.

---

## Appendix — search methodology

- Enumerated per-stage files: `ls src/gr/grcommon/ src/gr/grbonus/` (10 stage .c/.h files).
- Grepped for `portFixupStruct*` in `src/mp/` and `src/gr/` — all calls live in `mpcollision.c`.
- Grepped for `PORT_RESOLVE` + `gr_desc[*].dobjdesc` — 90 resolved, 7 unresolved → the unresolved set is the §1 bug list.
- Read `port/bridge/lbreloc_byteswap.cpp:985-1017` to confirm `portFixupStructU16` is rotate16 and `portFixupStructU32` is `__builtin_bswap32`.
- No rabbitizer disasm needed: no `IS_BIG_ENDIAN` bitfield structs in the audited headers.
