# CMake warning audit — handoff for flag 5 & 6 (updated 2026-04-20)

## Status

Flag 5 (`-Werror=int-conversion`) — **cleared, all 158 errors resolved, flag promoted to `-Werror=int-conversion` in `CMakeLists.txt:202` (uncommitted working tree).**
Flag 6 (`-Werror=incompatible-pointer-types`) — not started.

### Flag 5 progress this pass

| Commit | What it cleared | New total |
|--------|----------------|----------:|
| `055aacc` | `(intptr_t)` cast for submotion `D_ovl1_*` script pointer inits across 13 `scsubsysdata*.c` files | 158 → 81 |
| `82982ec` | `(uintptr_t)` cast for ROM-symbol address inits in `ft/ftdata.c` (12) and `sys/audio.c` (11) | 81 → 57 |
| *(pending commit)* | Remaining 57 fixes across categories C/D/E/F — see "Final pass" below | 57 → 0 |

### Final pass (uncommitted, 29 files)

Categories C, D, F — mechanical casts at call sites:
- `mn/mncommon/mntitle.c:1490` — `(uintptr_t)&SYMBOL` for 4 args (C).
- `gr/grcommon/grpupupu.c:690`, `gr/grcommon/gryoster.c:257`, `it/itmanager.c:159-162` — same `(uintptr_t)&SYMBOL` pattern (D).
- `gr/grcommon/gryoster.c:208,229,240,245`, `gr/grcommon/grjungle.c:122`, `gr/grcommon/gryamabuki.c:122`, `it/itcommon/itharisen.c:246`, `sc/sccommon/scexplain.c:372` — `(TargetType *)` casts on `(uintptr_t)base + (intptr_t)offset` call args (D).
- `mn/mncommon/mncongra.c:113-115`, `sc/sccommon/scstaffroll.c:2256-2258` — `(void *)` on `SYVIDEO_DEFINE_FRAMEBUFFER_ADDR` initializers (F).
- `mn/mnplayers/mnplayers1pbonus.c:2469` — `(GObj *)(intptr_t)player` cast (F).

Category E — LP64 landmines:
- `mn/mnvsmode/mnvsoptions.c:84` — root-cause fix: changed `s32 sMNVSOptionsDamageGObj` → `GObj *sMNVSOptionsDamageGObj` (file-local; fixes 3 errors at 456/1420/1487 at once).
- `sys/objscript.c` + `sys/objhelper.{c,h}` + `if/ifcommon.{c,h}` + `sc/sc1pmode/sc1pgame.{c,h}` — **widened `u32 param` → `uintptr_t param` across gc* helper family** (`gcFuncGObjByLink`, `gcFuncGObjAll`, `gcFuncGObjByLinkEx`, `gcFuncGObjAllEx`, `gcGetGObjByID`, `gcAddGObjScript`). Updated 2 `ifCommon*GObj` and 3 `sc1PGameBoss*` callback signatures. `gcAddGObjScript` reshaped to take `uintptr_t` and internally cast to `GObjScript *` (was the 64-bit-pointer-truncation hazard at `objscript.c:38`).
- `ft/ftmain.c:4886,4896,4913` — added `(intptr_t)` cast on `*fp->data->p_file_{submotion,mainmotion}` reads so `intptr_t event_file_head` assignment is valid; replaced the `!= NULL` sentinel on the integer with `!= 0`.
- `ft/ftmain.c:1070` — `(void *)(uintptr_t)` on `GMColEventDefault->value` (u32 bitfield) being stored into `void * p_subroutine[]` slot.
- `ef/efmanager.c:5260,5969` — `(void *)` wrap on reloc-pointer subtraction expression (symmetric sites).
- `sys/audio.c:839` — `(u8 *)(uintptr_t)` on `sbk_start` (already `uintptr_t`) passed to `alSeqFileNew(... u8 *base)`.
- `sys/audio.c:955` — `(void *)(intptr_t)` on dead `unk38` (branch never executes; `unk34 == 0` is the live path).
- `sys/audio.c:980` — `(s32)(uintptr_t)` on `unk44` (value is NULL; stored-as-0 preserved).
- `sys/audio.c:1308` — `(s16 *)(uintptr_t)` on PORT-override `osVirtualToPhysical` return (`u64` under PORT).
- `sys/audio.c:1386` — `(uintptr_t)` on `seqArray[i].offset` (u8* → uintptr_t rom arg).
- `sys/dma.c:142,147` — `(u32)(uintptr_t)PHYS_TO_K1(...)` on both comparison and assignment to HW-register `baseAddress` field.
- `lb/lbparticle.c:1318` — `(u16)` cast replaces wrong `(u8*)` cast; RHS computes a bytecode offset not a pointer.
- `lb/lbcommon.c:1514` — `(Gfx **)(intptr_t)1` on no-op stub's unused param (body is just `return`).
- `mp/mpcommon.c:379,391` — `(s32)(intptr_t)fp` on unused `s32 arg0` of `func_ovl2_800EBC0C` (arg truly unreferenced in body).
- `libultra/n_audio/n_env.c:2705,2710` — `(s32)(intptr_t)` on dead `unknown0`/`unknown1` stores.
- `libultra/n_audio/n_env.c:4458` — `(ALWaveTable *)(intptr_t)` on `arg0->unk40` (s32 stored earlier from pointer; under PORT the upstream truncates — see follow-up below).
- `libultra/n_audio/n_env.c:5021,5022` — `(s32)(uintptr_t)arg0` on `void *` → `s32` store (short-circuited by early-return in PORT; paths dead).
- `libultra/n_audio/n_env.c:5453` — `(ALWhatever8009EE0C_3 *)(intptr_t)` on `unk_80026204_0x1C` s32 → pointer.

### Follow-up concerns (not blocking)

1. `libultra/n_audio/n_env.c` has several `s32` struct fields (`unk20`, `unk24`, `unk40`, `unknown0/1`) that are storing actual pointers on N64. On LP64 these silently truncate. The FGM synth path is currently short-circuited (`PORT` early-return at `n_env.c:5000`), so the truncation is dead storage today — but if/when FGM is wired up for real these fields must be widened to `uintptr_t` or `void *`. Track alongside Phase 5/6 audio work.
2. `mp/mpcommon.c` — `func_ovl2_800EBC0C(s32 arg0, ...)` declares an unused first parameter; callers pass `FTStruct *`. Body ignores it. If anyone ever reads `arg0`, they'd get a truncated pointer. Consider widening the prototype to `FTStruct *` since no other caller uses it.
3. The `gcFunc*` helper family's `u32 param` → `uintptr_t param` widen changes function-pointer types. All compatible callback sites have been updated (2 in `ifcommon.c`, 3 in `sc1pgame.c`, 1 in `objscript.c`). `gcGetGObjByID`'s `u32 id` comparison still treats the low 32 bits (`(u32)id` cast in body) — intentional because IDs are u32 values; widening is zero-extending semantics on both sides.

To reproduce the error list *before this pass*: revert these 29 files then flip `CMakeLists.txt:202` from `-Werror=int-conversion` to `-Wno-int-conversion`, then `cmake --build build --target ssb64 --clean-first -j 8 -- -k`.

### FTMotionDesc offset question — **resolved**

Prior handoff flagged this as a blocker. Reading `docs/fighter_intro_animation_handoff_2026-04-13.md` (section 2, "`FTData::p_file_submotion == NULL`") confirms:

- **Submotion** descs: `offset` field holds a real pointer (to a `D_ovl1_*` event-script array in `.data`). N64 did `0 + ptr = ptr` via the `*NULL` UB; port's safe guard at `ftmain.c:4886` short-circuits to `event_script_ptr = NULL` and the stored pointer is never dereferenced. Adding `(intptr_t)` to the initializer is purely cosmetic.
- **Mainmotion** descs: `offset` is a small byte integer (e.g. `0x0000007C`) used as offset inside the loaded-file blob. Integer literals need no cast; these lines weren't in the error list.

Submotion scripts are silently skipped on port (affects mouth blinks, voice lines, one-shot texture flips — *not* projectile spawning). That's a known latent issue tracked in the fighter-intro docs and is orthogonal to the warning cleanup. Do not conflate the two.

## Remaining 57 errors — by category

```
 8 gr/grcommon/gryoster.c
 6 libultra/n_audio/n_env.c
 5 sys/audio.c
 4 mn/mncommon/mntitle.c
 4 it/itmanager.c
 4 gr/grcommon/grpupupu.c
 3 sc/sccommon/scstaffroll.c
 3 mn/mnvsmode/mnvsoptions.c
 3 mn/mncommon/mncongra.c
 3 ft/ftmain.c
 2 sys/objscript.c
 2 mp/mpcommon.c
 2 ef/efmanager.c
 1 each: sys/dma.c, sc/sccommon/scexplain.c,
        mn/mnplayers/mnplayers1pbonus.c, lb/lbparticle.c,
        lb/lbcommon.c, it/itcommon/itharisen.c,
        gr/grcommon/gryamabuki.c, gr/grcommon/grjungle.c
```

### Category C — mechanical "remove &" (4 errors, 1 line)

**`mn/mncommon/mntitle.c:1490`** passes `&SYMBOL_1, &SYMBOL_2, &SYMBOL_3, &SYMBOL_4` where the parameter type is `uintptr_t`. Same pattern as the audio.c cluster already fixed. Apply `(uintptr_t)&` per site.

### Category D — param-type mismatches (12 errors)

All four are the same syndrome: callee declared with `uintptr_t` or `AObjEvent32**` parameter, caller passes `&SYMBOL` (pointer-to-array). Each needs inspection of the callee signature.

- **`gr/grcommon/grpupupu.c:690`** — 4 args to `efParticleGetLoadBankID(&lGRPupupuParticleScriptBankLo, ...)`. Parameter is `uintptr_t`; caller passes `intptr_t*`. Apply `(uintptr_t)` cast per site OR drop `&` if the symbol type supports it. Check the 4 `lGRPupupu*` declarations.
- **`gr/grcommon/gryoster.c:257`** — identical 4-arg pattern as `grpupupu.c:690`. Same fix.
- **`it/itmanager.c:159-162`** — same 4-arg pattern. Same fix.
- **Passing `uintptr_t` to `AObjEvent32**` / `DObjDesc*` / `MObjSub***` / `void*`**: `gr/grcommon/{grjungle.c:122, gryamabuki.c:122, gryoster.c:229,240,245}`, `it/itcommon/itharisen.c:246`, `sc/sccommon/scexplain.c:372`. These are the inverse: callee wants a pointer, caller passes a `uintptr_t`. Probably wrap call-site with `(TypeName*)` cast, but inspect each — the symbol being passed is likely a linker stub that needs a reloc-resolver lookup.

### Category E — real LP64 landmines

#### 1. `src/sys/objscript.c:28,38` — callback `u32 param` truncates pointer

`gcFuncGObjByLinkEx(link, gcAddGObjScript, &gobjscript, FALSE)` at line 38 passes a stack-address `&gobjscript` as the `u32 param` — truncates on LP64. Line 28 returns an `s32` success code from a function declared `GObj*` return.

**Fix:** widen the callback infrastructure's `u32 param` → `uintptr_t param` across:
- `src/sys/objhelper.h` (5 extern decls)
- `src/sys/objhelper.c` (5 definitions)
- `src/if/ifcommon.{h,c}` — `ifCommonBattleInterface{Pause,Resume}GObj` (`u32 unused`)
- `src/sc/sc1pmode/sc1pgame.{h,c}` — 3 boss callbacks (`u32 unused`)
- `src/sys/objscript.c:26-29` — rewrite `gcAddGObjScript` to `return (GObj*)(intptr_t)gcSetupGObjScript(...)` (preserves ROM semantics; caller discards the return anyway)

`gcGetGObjByID(GObj*, u32 id)` keeps working since `u32 id` widens cleanly into `uintptr_t id`. All 5 `u32 unused` callbacks don't care. Single atomic commit — ~7 files.

#### 2. `src/sys/audio.c:955,980,839,1308,1386` — struct field type mismatches

- **L955**: `audio_config.inst_sound_array = sSYAudioCurrentSettings.unk38;` — `inst_sound_array` is `void*`, `unk38` is `s32`. `unk34` is initialized to 0 in `dSYAudioPublicSettings`, so this branch never executes at runtime. Change `unk38` field type to `void*` (or cast the RHS). The adjacent `unk34 = 0` branch at 966-975 is the actual live path on port.
- **L980**: `audio_config.unk_80026204_0x1C = sSYAudioCurrentSettings.unk44;` — `unk_80026204_0x1C` is `s32`, `unk44` is `uintptr_t*`. `unk44` is initialized `NULL`, so value stored is 0. Cast to silence: `(s32)(uintptr_t)settings.unk44`, OR widen `unk_80026204_0x1C` to `uintptr_t` (affects `N_ALUnk80026204` struct at `libultra/n_audio/n_env.c:54` + the reader at `n_env.c:5453`).
- **L839, L1308, L1386**: not yet inspected — please audit before fixing. Look at what's being passed/returned and pick cast vs widen.

#### 3. `src/sys/dma.c:147` — hardware register field

`sSYDmaSramPiHandle.baseAddress = PHYS_TO_K1(PI_DOM2_ADDR2);` — PORT `PHYS_TO_K1` returns `void*`, but `OSPiHandle.baseAddress` is `u32` (N64 HW register). `PI_DOM2_ADDR2` is a small constant → value fits → no real data loss. Two options:

1. Cast: `sSYDmaSramPiHandle.baseAddress = (u32)(uintptr_t)PHYS_TO_K1(PI_DOM2_ADDR2);` — preserves behavior.
2. `#ifdef PORT` → direct literal assignment; N64 path unchanged.

The comparison on line 142 needs the same fix.

#### 4. `src/libultra/n_audio/n_env.c:2705,2710,5021,5022,5453,4458` — HAL "unknown" fields

- **2705/2710**: `seqp->unknown0 = spseq.seq;` — field is `s32`, RHS is `void*`. `unknown0`/`unknown1` are written but never read anywhere in the codebase (confirmed via grep). Dead storage. Either cast `(s32)(intptr_t)` at the assignment to silence, or delete the assignments entirely. Since HAL added these fields for a reason we don't know, casting is safer.
- **5021/5022/5453/4458**: not yet inspected — investigate similarly. They're also in the HAL audio layer. 5453 is the `unk_80026204_0x1C` reader discussed above.

#### 5. `src/ft/ftmain.c:1070,4886,4896`

- **4886, 4896**: `event_file_head = *fp->data->p_file_{submotion,mainmotion};` — `event_file_head` is declared `intptr_t` but deref'd into `void*`. Add explicit `(intptr_t)` cast. Same semantics.
- **1070**: `void *` assigned from `u32`. Not inspected; likely similar truncation-through-API issue.

#### 6. `src/ef/efmanager.c:5260,5969`

```c
file = ((uintptr_t)*p_file - (intptr_t)llITCommonDataMBallThrownDObjDesc);
```

`file` is `void*`; RHS is `uintptr_t` arithmetic. Wrap RHS in `(void*)` cast, since this is computing a pointer-relative offset and treating it as a pointer. Both sites are symmetric.

#### 7. `src/lb/lbparticle.c:1318` — u16 from u8*

```c
u16 something = (u8 *)ptr;
```

u16 is way too narrow. Real truncation — needs investigation. Grep for the field; likely wants widening to `uintptr_t` or the RHS should be an offset, not a pointer.

#### 8. `src/lb/lbcommon.c:1514` — int → `Gfx**`

Passing `int` to a `Gfx**` parameter. Likely a caller mistake — probably wants `NULL` or a real `Gfx**`. Inspect call site.

#### 9. `src/mp/mpcommon.c:379,391` — `FTStruct*` to `s32`

Callee parameter is `s32`, caller passes `FTStruct*`. Pointer truncation. Widen callee signature to `uintptr_t` or `FTStruct*`, or (if the callee only uses it for identity comparison) leave the truncation and cast explicitly to acknowledge the loss.

### Category F — cosmetic (unsigned-long in void* initializer)

- `mn/mncommon/mncongra.c:113-115` (3) — `void *X = someUnsignedLong;`. Cast `(void*)`.
- `sc/sccommon/scstaffroll.c:2256-2258` (3) — same pattern.
- `mn/mnvsmode/mnvsoptions.c:456,1420,1487` (3) — `s32`↔`GObj*` mixing. Cast at each site.
- `mn/mnplayers/mnplayers1pbonus.c:2469` (1) — `s32 → GObj*`. Cast.

## Suggested next session plan

Order by effort-to-value:

1. **Category C** (mntitle.c, 4 errors, 1 line) — 30s. Same fix as audio.c cluster.
2. **Category D grpupupu/gryoster/itmanager pattern** (12 errors) — pattern-match against the 3 identical call shapes; one commit.
3. **Category D pointer-from-uintptr cluster** (8 errors in grjungle/gryamabuki/gryoster/itharisen/scexplain) — inspect each, should be mechanical casts.
4. **Category F cosmetic** (10 errors) — one commit of `(void*)` casts + `s32`↔`GObj*` casts.
5. **Category E landmines** — the hard ones. Tackle:
   - objscript.c callback widen (#1) — biggest refactor, ~7 files
   - audio.c struct field decisions (#2) — each needs judgment
   - ftmain.c `(intptr_t)` casts (#5) — mechanical
   - efmanager.c `(void*)` cast (#6) — mechanical
   - dma.c hw register (#3) — trivial
   - n_env.c dead-store casts (#4) — mechanical  
   - lbparticle.c, lbcommon.c, mpcommon.c — each needs investigation
6. **Flip flag + commit** — `CMakeLists.txt:201`.

For flag 6 (`-Werror=incompatible-pointer-types`), expect similar scale but mostly struct-layout punning between mismatched pointers — "likely the largest blast radius; save for last."

## Related

- `docs/cmake_warning_audit_2026-04-20.md` — original plan.
- `docs/fighter_intro_animation_handoff_2026-04-13.md` — the `p_file_submotion == NULL` context that resolves the FTMotionDesc question.
- `docs/bugs/item_arrow_gobj_implicit_int_2026-04-20.md` — motivating LP64 truncation incident.
- `MEMORY.md` → *Implicit-int LP64 trunc trap* — fingerprint for recognizing the crash class.
