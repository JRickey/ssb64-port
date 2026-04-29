# Release-Pass Audit — 2026-04-28

Ten parallel sub-agents swept the codebase for known bug-class fingerprints from `docs/bugs/`. This doc consolidates what was checked, what's clean, and what still wants follow-up.

## Verdict

**The port is in good shape for release.** Eight of ten audits found no new bugs. Two audits surfaced findings that are **not yet verified as live shipped bugs** but are worth investigating before a final release tag.

---

## Audits & Status

| # | Audit | Status | New findings |
|---|-------|--------|--------------|
| 1 | LP64 pointer-array stride | CLEAN | All 5 known bugs fixed; no new sites |
| 2 | Halfswap data corruption (figatree f32/Vec3f) | CLEAN | All vulnerable paths covered |
| 3 | u8/u16 in loaded structs | MOSTLY CLEAN | 2 loader paths to spot-check (ITAttributes, WPAttributes) |
| 4 | Bitfield positional initializer | CLEAN | FTMotionFlags + GMRumbleEvent macro defended |
| 5 | PORT u32 token vs raw pointer | FALSE POSITIVE | Hits in dead code only |
| 6 | Implicit-int LP64 truncation | CLEAN | `-Werror=implicit-function-declaration` already enforced |
| 7 | Custom init missing portFixupMObjSub | 1 SUSPECT | sc1pgameboss.c boss wallpaper — needs trace |
| 8 | Static GObj* slot reset | NEEDS TRIAGE | ~15 menus/scenes flagged; most likely benign per analysis below |
| 9 | Fast3D / libultraship gaps | MINOR | YUV finding was false positive; only D3D11 wrap-mode + cull DL stubs remain (low risk) |
| 10 | Port glue / bridge safety | FALSE POSITIVE | `sizeof(OSThread)` claim invalid (C TU only sees decomp definition) |

Per-audit detail reports written to disk (5 of 10 agents wrote files; the rest are summarized below):
- `docs/audit_lp64_stride_2026-04-28.md`
- `docs/audit_u8_u16_struct_fields_2026-04-28.md`
- `docs/audit_bitfield_positional_init_2026-04-28.md`
- `docs/audit_fast3d_gaps_2026-04-28.md`
- `docs/audit_port_glue_2026-04-28.md`

---

## False positives (so future audits don't redo this work)

### Audit 5 — PORT u32 token vs raw pointer

Agent flagged `Sprite::rsp_dl_next` writes/reads in:
- `src/libultra/sp/sprite.c:160,162,453`
- `src/ovl8/ovl8_3.c`, `src/ovl8/ovl8_6.c`
- `src/sys/objdisplay.c:3041,3045`

**Why it's a false positive:** `CMakeLists.txt:97` excludes `src/ovl8/` and `src/libultra/` from the build, and `src/sys/objdisplay.c:3030` wraps the offending block in `#ifndef PORT`. The `port/stubs/n64_stubs.c` `spDraw` is a stub returning NULL.

### Audit 9 — Fast3D YUV claim

Agent flagged `SPDLOG_ERROR("YUV Textures not supported")` at `libultraship/src/fast/interpreter.cpp:1925` as an active bug.

**Why it's a false positive:** Grep for `G_IM_FMT_YUV` in compiled-in code returns nothing. The only references are in the non-compiled `src/libultra/sp/sprite.c`. SSB64 does not use YUV format; the error path never fires.

### Audit 10 — `sizeof(OSThread)` claim in n64_stubs.c

Agent flagged `port/stubs/n64_stubs.c:127` `memset(t, 0, sizeof(OSThread))` as a 432-byte LUS-shadowed memset into a decomp-sized buffer.

**Why it's a false positive:** `n64_stubs.c` is C-compiled and includes `<PR/os.h>` (decomp's `OSThread`). The libultraship `OSThread` lives in `libultraship/include/libultraship/libultra/thread.h` which is C++ and never included by this TU. `sizeof(OSThread)` resolves to the decomp size matching the caller's buffer.

---

## Real findings to investigate

### Finding A — `sc1pgameboss.c` boss wallpaper MObjSub fixup (Audit 7)

**Site:** `src/sc/sc1pmode/sc1pgameboss.c:825`, function `sc1PGameBossSetupBackgroundDObjs`

```c
while (mobjsub != NULL) {
    gcAddMObjForDObj(dobj, mobjsub);  // <-- no portFixupMObjSub first
    mobjsubs++;
    mobjsub = *mobjsubs;
}
```

The MObjSub triple-pointer is computed as a raw cast at the caller (line 886): `(MObjSub***)(addr + o_mobjsub)` — bypassing the normal lbCommon load path that would have applied the fixup.

**Same pattern as:** `docs/bugs/bumper_wrong_colors_2026-04-28.md` — fixed by adding `portFixupMObjSub` before `gcAddMObjForDObj`.

**Risk:** Boss wallpaper materials may render with wrong colors / texture indices on 1P boss stages. **Not yet observed in playtesting.** May be benign if `addr + o_mobjsub` resolves to a region the lbReloc path already swept (needs trace).

**Suggested fix:**
```c
mobjsub = *mobjsubs;
while (mobjsub != NULL) {
#ifdef PORT
    portFixupMObjSub(mobjsub);
#endif
    gcAddMObjForDObj(dobj, mobjsub);
    mobjsubs++;
    mobjsub = *mobjsubs;
}
```

`portFixupMObjSub` is idempotent (keys on pointer in `sMObjSubFixups`), so a redundant call when the lbCommon path already ran is harmless.

### Finding B — Static GObj* slots without reset in InitVars (Audit 8)

The agent flagged ~15 files. **Spot-check of the highest-risk candidate (`mnmodeselect.c`) shows the pattern is mostly benign in this codebase:**

```c
// Each scene-init unconditionally re-allocates:
sMNModeSelectOption1PModeGObj = gobj = gcMakeGObjSPAfter(...);
// The stale value (if any) is overwritten before any read.
```

The canonical 2026-04-20 bug (`mnplayers1pgame_timegobj_stale`) only fired because:
1. The slot was conditionally re-allocated (`if (slot != NULL) gcEjectGObj(slot); slot = ...;`)
2. AND scene re-entry could find the slot pointing to freed-bump-heap memory
3. AND `gcEjectGObj` was called on the stale value

For most flagged sites, the unconditional re-assignment in the scene init function makes the missing reset benign. **Recommended approach:** Don't fix unless a crash actually surfaces — adding stray `slot = NULL;` lines to InitVars would be defensive cleanup, not a real bug fix.

**Sites that DO match the canonical pattern (slot conditionally ejected on re-alloc) and warrant a closer look:**

- `src/mn/mnplayers/mnplayers1pbonus.c` — `sMNPlayers1PBonusHiScoreGObj` self-clears at lines 1050/1123 but is not in `mnPlayers1PBonusInitVars()`. If the scene exits without going through the eject path, the next entry still sees the stale slot, and the `if (slot != NULL) gcEjectGObj(slot);` at line 1049 would deref freed memory.
- `src/mn/mn1pmode/mn1pcontinue.c` — 12 GObj slots; needs detailed trace to determine which are conditionally vs unconditionally re-assigned.

**Suggested fix (only if confirmed):** Add `slot = NULL;` lines to the scene's `*InitVars` for any slot whose first post-init read is a `if (slot != NULL) gcEjectGObj(slot)` style guard.

### Finding C — `ITAttributes` / `WPAttributes` u8/u16 loader path (Audit 3) — CONFIRMED CLEAN

Verified 2026-04-28:
- `src/it/itmanager.c:264` — `portFixupStructU16(attr, 0x14, 8)` covers the s16/Vec3h block 0x14..0x33 (8 u32 words). Single load site (`lbRelocGetFileData`); all consumers go through `ip->attr`.
- `src/wp/wpmanager.c:119` — `portFixupStructU16(attr, 0x10, 6)` covers Vec3h[2] + map_coll s16s + size + _angle_raw at 0x10..0x27 (6 u32 words). Single load site.

Both fixups are correctly bounded; no missing or stray writers.

### Finding D — Minor Fast3D gaps (Audit 9)

- **D3D11 `G_TX_MIRROR | G_TX_CLAMP` wrap mode** — incomplete in `gfx_direct3d11.cpp`. Mac/Linux/OpenGL backends are correct. Three-line trivial fix, low priority.
- **F3DEX2 cull DL handler** — unimplemented. Game does not appear to use culling DLs, so latent rather than active.
- **`bg scaling` TODO at `interpreter.cpp:3928`** — `dsdx`/`dtdy` hardcoded to identity. Has not produced a known artifact yet.

---

## Coverage summary

The 10 audits sweep the bug classes documented in `docs/bugs/README.md`. Bug families that are now confirmed clean across the codebase:

- LP64 pointer-array stride mismatches
- `*(void**)` over 4-byte tokens
- Adjacent `s32 unknown` slots that should widen to pointer
- Figatree halfswap corruption of f32/Vec3f data blocks
- Halfswap of `SYInterpDesc` for both figatree and non-figatree resources
- u8/u16 fields in pass-1 BSWAP32 structs (with 2 minor follow-ups)
- Endian-conditional bitfield positional initializers
- PORT u32 token-vs-raw-pointer assignments in compiled code
- Implicit-int LP64 pointer-return truncation
- Mixer `#include` order regression
- LUS-vs-decomp typename shadowing in compiled C TUs
- port_log variadic ARM64 garbling

**Bug families with possible residuals (worth a closer look before final release tag):**
- Custom item/object init paths missing `portFixupMObjSub` (1 site flagged: sc1pgameboss.c)
- Static GObj* slot reset (~15 sites flagged; most appear benign by spot-check; a few warrant trace)
- Minor Fast3D feature gaps (D3D11 wrap-mode, cull DL, bg-scaling TODO)
