# CMake warning audit — handoff for flags 5 & 6 (2026-04-20)

## Status

Started 2026-04-20. Plan: `docs/cmake_warning_audit_2026-04-20.md`.

- Flags 1–4 **done** (commits `b53ffc2`, `c59cbde`, `a61dc7e`, `a4d2cc9`).
- Flag 5 (`-Werror=int-conversion`) — **paused mid-investigation**. This doc.
- Flag 6 (`-Werror=incompatible-pointer-types`) — **not started**. Expected the biggest blast radius.

Category B leftovers (`-Wno-implicit-int`, `-Wno-shift-negative-value`, `-Wno-constant-conversion`, `-Wno-tautological-constant-out-of-range-compare`) remain `-Wno-*`. Lower priority — not LP64 landmines.

## What flag 5 looks like after a clean build

Flipping `-Wno-int-conversion` → `-Werror=int-conversion` in `CMakeLists.txt:201` produces **158 unique errors** after `cmake --build build --target ssb64 --clean-first -- -k`. Build log collected at `/tmp/build_ic.log` during the original session — regenerate with the same command.

### Category breakdown (by diagnostic message)

| Count | Diagnostic | Where |
|------:|-----------|-------|
| ~100  | `initializing 'intptr_t' with ... s32[N]` | `sc/scsubsys/scsubsysdata*.c`, `ft/ftdata.c` — FTMotionDesc tables |
| 12    | `initializing 'uintptr_t' with ... uintptr_t *' (aka 'unsigned long *'); remove &` | `sys/audio.c` SYAudioSettings init (`&B1_sounds*_ctl_ROM_START` etc.) |
| 10    | `(intptr_t *)` ↔ `(uintptr_t)` param mismatches | `gr/grcommon/grpupupu.c`, `gr/grcommon/gryoster.c`, `it/itmanager.c` |
| 8     | `(uintptr_t)` → pointer param | `gr/grcommon/grjungle.c`, `gryamabuki.c`, `gryoster.c`, `it/itharisen.c` |
| ~8    | `void *` ↔ integer (real LP64 landmines) | `sys/objscript.c:28,38`; `sys/audio.c:955,980`; `sys/dma.c:147`; `libultra/n_audio/n_env.c:2705`; `ft/ftmain.c:1070,4886,4896,4913`; `ef/efmanager.c:5260,5969`; `gr/grcommon/gryoster.c:208`; `lb/lbparticle.c:1318` |
| 4     | Misc (`u64` → `s16*`, etc.) | `sys/audio.c:1308`, `sys/audio.c:839,1386`, `lb/lbcommon.c:1514` |

## The hard question: `FTMotionDesc.offset` (~100 of the 158 errors)

`src/ft/fttypes.h:92`:

```c
struct FTMotionDesc {
    u32 anim_file_id;
    intptr_t offset;       // <-- all the warnings
    FTAnimDesc anim_desc;
};
```

Initializer pattern (e.g. `src/sc/scsubsys/scsubsysdataness.c:91`):

```c
FTMotionDesc dFTNessSubMotionDescs[] = {
    ll_1664_FileID, D_ovl1_80392694, 0x00000000,
    // ^file id     ^script array    ^anim desc bits
    ...
};
```

Where `D_ovl1_80392694` is a real `s32[]` array of motion-command bytecode defined earlier in the same file. Also seen as the literal sentinel `0x80000000` meaning "no script".

Runtime use at `src/ft/ftmain.c:4880–4918`:

```c
if (motion_desc->offset != 0x80000000) {
    event_file_head = *fp->data->p_file_submotion;   // or _mainmotion
    event_script_ptr = (void*) ((intptr_t)motion_desc->offset + (intptr_t)event_file_head);
}
```

So at runtime, `offset` is treated as **an integer offset into an asset file**, added to `event_file_head` to produce the actual script pointer. This only makes sense if the stored value is a small integer (offset-from-file-start), not a real pointer.

**The unresolved semantic question:**

- On original MIPS N64, the overlay linker resolved `D_ovl1_80392694` to a small integer offset at build time, and the `offset + event_file_head` arithmetic produced the correct pointer.
- In our recompiled port, `D_ovl1_80392694` is a normal C array living in `.data`/`.bss`. Its address is a full 64-bit heap/bss pointer. Adding `event_file_head` to it produces garbage.

**Before touching the 100 initializers, trace whether the `motion_desc->offset + event_file_head` code path is live on the port.** Possibilities:
1. It's dead (the port loads motion scripts differently), in which case the initializers can be simplified safely.
2. It's live but silently broken (we got lucky / path is rarely hit / `p_file_submotion` is NULL so the `(event_file_head != NULL) ? … : NULL` bails).
3. It's live and silently works because of some relocation/trampoline I haven't spotted.

Recommended starting point: grep for `is_use_submotion_script` / `is_use_mainmotion_script` assignments; add a `port_log` at `ftmain.c:4891` logging `motion_desc->offset`, `event_file_head`, and the resulting `event_script_ptr`; boot the game and watch the log during any fighter animation.

This is NOT a "fix the warning" task — it's a semantic audit that incidentally fixes warnings.

## The real LP64 landmines (~8 errors)

These are the whole reason to tackle flag 5 at all. Each deserves its own investigation, since they're assigning pointers to `s32`/`u32` or returning them from int-returning functions:

### `src/sys/objscript.c:28,38`

```c
GObj* gcAddGObjScript(GObj *gobj, GObjScript *gobjscript) {
    return gcSetupGObjScript(gobj, gobjscript->id, gobjscript->next_gobj);
}
// Line 38:
gcFuncGObjByLinkEx(link, gcAddGObjScript, &gobjscript, FALSE);
```

- Line 28: `gcSetupGObjScript` returns `s32` (value 0 per line 23) but `gcAddGObjScript` declared as returning `GObj*`. Declaration vs. definition mismatch? Or `gcSetupGObjScript` is wrong type?
- Line 38: passing `&gobjscript` (a stack address) as a `u32` parameter. Truncated on LP64.

Need to find the declaration of `gcFuncGObjByLinkEx` and understand whether callers actually pass pointers as the third arg. Likely a real port bug.

### `src/sys/audio.c:955,980`

```c
audio_config.inst_sound_array = sSYAudioSequenceBank1->instArray[0]->soundArray; // 955
audio_config.unk_80026204_0x1C = sSYAudioCurrentSettings.unk44;                   // 980
```

`audio_config` is some struct in the audio.c port path. `inst_sound_array` is declared `s32` but holds a pointer. Check the struct def in `src/libultra/n_audio/` — probably needs widening.

### `src/sys/dma.c:147`

```c
u32 some_var = (void *)ptr_expr;  // truncates on LP64
```

Real truncation bug.

### `src/libultra/n_audio/n_env.c:2705`

```c
s32 x = some_void_ptr;
```

Likely same class. Widen `s32` → `uintptr_t` or fix the assignment.

### `src/ft/ftmain.c:1070,4886,4896,4913`

- 1070: `void *` assigned from `u32` — the `u32` is probably a truncated pointer passed across an API boundary.
- 4886/4896/4913: `intptr_t` assigned from `void *` — these are the `event_file_head = *fp->data->p_file_submotion;` lines from the FTMotionDesc context above. Probably correctable with `(intptr_t)` cast, but interpret in context of the offset question.

### `src/ef/efmanager.c:5260,5969`

```c
void *p = some_uintptr_t;
```

Likely explicit cast needed, not a truncation bug.

### `src/gr/grcommon/gryoster.c:208`

Same pattern.

### `src/lb/lbparticle.c:1318`

```c
u16 something = (u8 *)ptr;
```

u16 is way too narrow — real truncation. Needs investigation.

## The "remove &" cluster (12 errors in `sys/audio.c`)

```c
SYAudioSettings sSYAudioCurrentSettings = {
    ...
    &B1_sounds2_ctl_ROM_START,   // clang: "remove &"
    &B1_sounds2_ctl_ROM_END,
    ...
};
```

`SYAudioSettings`'s `bank*_start`/`bank*_end` fields are `uintptr_t`. The init uses `&SYMBOL` which produces a pointer (`uintptr_t *`) because those symbols are themselves declared as `uintptr_t`. Clang's "remove &" suggestion is literal: drop the `&`.

Check: are `B1_sounds*_ctl_ROM_START` etc. declared as `uintptr_t` or as a char array? If char array, `&X` gives `char (*)[N]` which still isn't `uintptr_t`. If `uintptr_t`, then `&X` gives `uintptr_t *` and dropping `&` gives `uintptr_t` — correct.

Likely mechanical: drop the `&` at each init. ~12 lines in `sys/audio.c:116–141`. But verify the symbol declarations first.

## The param-type mismatches (`grpupupu.c`, `itmanager.c`, etc.)

E.g. `src/it/itmanager.c:159–162` passes `intptr_t *` where `uintptr_t` is expected. Probably the function signature is wrong or the call site needs `(uintptr_t)(&X)` cast. Read each function's declaration to decide.

## Suggested session plan

1. **Don't start by flipping the flag** — start with tracing `FTMotionDesc.offset` at runtime. If that path is dead, scope collapses dramatically. If live, the semantic model needs a port-side fix (relocation table?) before touching initializers.
2. **Then tackle the ~8 real LP64 landmines one by one**, each with its own investigation. These are high-value fixes independent of flag 5 — they could land as separate commits without promoting the flag.
3. **Only then consider flipping the flag** once you've seen what's left and whether the remaining warnings are cosmetic enough to drive to zero.

For flag 6 (`-Werror=incompatible-pointer-types`), expect similar scale but mostly struct-layout punning between mismatched pointers — the audit doc called it "likely the largest blast radius; save for last." No investigation done yet.

## Related

- `docs/cmake_warning_audit_2026-04-20.md` — original plan, updated after each flag completes.
- `docs/bugs/item_arrow_gobj_implicit_int_2026-04-20.md` — the motivating LP64 truncation incident.
- `MEMORY.md` → *Implicit-int LP64 trunc trap* — fingerprint for recognizing the crash class.
