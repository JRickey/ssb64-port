# Linux scene-boundary stale-data crashes — additional variants

**Date:** 2026-05-11
**Class:** Issue #103 / #128 family — long-lived references to scene-arena memory
**Platform:** Linux glibc + NVIDIA proprietary OpenGL driver (others untested for these variants)
**Status:** PARTIAL — one defensive guard shipped (`efmanager.c`); three other variants observed but not fixed.
**Files:** `decomp/src/ef/efmanager.c` (defensive guard); see related links for the broader family

## Symptom

Four distinct Linux-only intermittent SIGSEGVs observed during a single autonomous attract-demo session on Fedora 43 / NVIDIA 595.58.03. None reproduce on Windows or macOS with the same checkout.

All four crashes share the same shape: a long-lived global / GObj / segment-table entry holds a reference that survived a scene boundary; the data it points to has been freed and reused; the dereferencing consumer reads garbage that — on Linux glibc — happens to be unmapped or N64-pattern bits, faulting visibly. On Win/Mac the same memory address either re-resolves to compatible bytes or stays zero, so the crash hides.

This is the same family as
[`dl_link_stale_pointer_guard_2026-05-09`](dl_link_stale_pointer_guard_2026-05-09.md) — which has the
detailed write-up of *why* this manifests on Linux specifically (glibc's `madvise(MADV_DONTNEED)`
unmaps freed pages so stale reads fault; macOS Magazine / Windows LFH leave them readable).

## Variants observed

### 1. Kirby Cutter Draw effect — stale `*file_head`

`ftKirbySpecialHiUpdateEffect` → `efManagerKirbyCutterDrawMakeEffect` → `efManagerMakeEffect` →
`gcSetupCustomDObjs`. Backtrace ends at `gcAddChildForDObj+0x1d` with `x0=0` and `fault_addr=0x20`.

Root cause: `*effect_desc->file_head` (i.e. `gFTDataKirbySpecial2`) is non-NULL but points at
freed scene-arena memory after the Kirby Special2 file's prior scene unloaded. The DObjDesc array
fetched via `addr + offset` reads garbage; the `dl` field reads as token `0xF40C9009` — both gen
(`0xF40`) and idx (`823305`) are impossible values (current gen `0x099`, max index ~12K). The
token rejects to NULL, then downstream null-deref.

**Defensive guard shipped this commit:** NULL-check on `*effect_desc->file_head` in
`efManagerMakeEffect` (`decomp/src/ef/efmanager.c`). Bails safely so the caller's
`!= NULL` test skips the effect cleanly. **Does not fire** in the observed crash because the
pointer was non-NULL-but-stale, not NULL. Ships anyway as cheap safety net for the cleaner
"file unloaded → global NULLed" variant of this pattern.

Trigger: ~90 sec into the auto-demo, after several scene cycles, depends on the demo
permutation including a Kirby battle.

### 2. mnCharacters fighter init — stale `joints[i]`

`mnCharactersFighterProcUpdate` → `ftMainSetStatus`. Faulting line is
`decomp/src/ft/ftmain.c:4816` — `joint->translate.vec.f = dobjdesc->translate;` inside the
common-parts joint-init loop. Register dump shows `x0=0xe002b01` (token-pattern bits),
`x2=0x80808000ffffff00` (N64 KSEG1 pattern in the loaded translate value).

Root cause: `fp->joints[i]` for an early common-parts slot (i=4) is non-NULL-but-bogus. The
preceding line resolves `attr->commonparts_container` to a stale FTCommonPartContainer, whose
`commonparts[detail].dobjdesc` token resolves to a freed `DObjDesc` array containing N64-segment-
prefixed bits (`0x80808000…`). Either the fighter's `attr` itself is stale (file unloaded) or
the joints array was partially initialized.

Trigger: very early after scene start (~5 sec). Did not reproduce after light instrumentation
was added to the same function (Heisenbug-class — depends on heap layout).

### 3. Stale MObjSub token at `objanim.c:2869`

`RelocPointerTable: invalid/stale token 0x000E0000 (tokenGen=0x000, max_index=917504)` — i.e.
gen=0 (impossible; min is 0x80) with garbage index. The caller is
`gcAddMObjForDObj`'s token-walk loop. The existing `if (mobjsub == NULL) break;` guard catches
this and exits the loop cleanly — **no crash here**, but the warning indicates upstream MObjSub
table memory is reused/stale. Likely the same root as variant 2 manifesting at a different
consumer.

### 4. Segment-E DL pointer at `gfx_step` opcode fetch

`Fast::gfx_step` (libultraship `interpreter.cpp:6039`) — `int8_t opcode = cmd->words.w0 >> 24`,
faulting because `cmd = 0xe000000` (N64 segment-E base, never resolved by the segment-table
lookup that's supposed to happen before this point). Backtrace runs through
`sySchedulerExecuteTaskGfx` → `port_submit_display_list` → `DrawAndRunGraphicsCommands` →
`Run` → `gfx_step`.

Root cause: a `gsSPDisplayList` somewhere pushed a stale N64-segmented pointer onto the DL
stack without going through the segment-table resolution. Sibling of
[`Issue #128: per-load DL cache eviction + per-frame SP segment clear`](https://github.com/JRickey/libultraship/commit/190a1f9c)
which addressed the same family at a different site.

Trigger: ~9 min into the auto-demo, deep into the cycle.

## Why the family is wide

The decomp port replaced N64 KSEG addresses with PC pointers, but every long-lived reference
needs to either:
- Live for the program's lifetime (e.g. permanently-loaded files), or
- Get explicitly cleared when its backing memory is freed (e.g. scene-arena resets, file
  unloads, GObj ejections).

Existing scene-boundary cleanup has been incrementally tightened over time (`f714a0d`,
`b0bcdffaf`, `00230ff`, `190a1f9c`, etc.) but coverage is leaf-by-leaf. Each missed cleanup
becomes a latent stale-data crash that's invisible on Win/Mac and visible on Linux glibc.

## Audit hook

When triaging a new Linux-only crash that reads N64-segment-pattern bits (`0x80808000…`,
`0x0e000000`, etc.) or token-pattern values with impossible generation / index:

1. **Identify the consumer site** (where the deref faults). Find what kind of stale reference
   it holds — file-data global, GObj pointer, segment-table entry, etc.
2. **Find the holder's lifetime.** If it's BSS / a static global / a GObj BSS slot, walk back
   to the scene-boundary teardown and check whether it's cleared.
3. **Audit-hook pattern:** for every long-lived reference, either add a NULL-clear at the
   scene-boundary teardown (preferred — cheap, localized) or a defensive NULL-guard at the
   consumer (acceptable when the holder is hard to find — costs one branch per access).

The structural fix — generation-stamping all long-lived references and checking generation at
access — is on the same shape as the existing `RelocPointerTable` token system. Worth doing
when leaf-by-leaf fixes have diminishing returns.

## Related

- [`dl_link_stale_pointer_guard_2026-05-09`](dl_link_stale_pointer_guard_2026-05-09.md) — the
  family doc with the platform-difference explanation
- [`linux_intro_skip_crash_handoff_2026-05-09`](../linux_intro_skip_crash_handoff_2026-05-09.md)
  — sibling handoff in the top-level `docs/` tree
- libultraship `190a1f9c` (per-frame SP segment clear) — the precedent for variant 4
