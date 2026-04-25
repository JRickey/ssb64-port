# N_ALCSPlayer alt-bank pointer-array stride (LP64)

**Date:** 2026-04-24
**Status:** Fix shipped. Audio synthesis still silent — see handoff `docs/audio_buildout_2026-04-24.md`.
**Class:** LP64 pointer-array stride over mismatched field widths — same family as `file_pointer_array_lp64_stride` and `mball_effect_filehead_lp64`.

## Symptom

With audio threading + asset loading + synthesis loop all functional, BGM synthesis produced **only zeros** even though MIDI events flowed correctly. `n_alAudioFrame` ran, `portAudioSubmitFrame` pushed frames, `sSYAudioCSPlayerStatuses` advanced 1→2→3, and `n_alCSeqNew` parsed `validTracks=0xf97f` with `division=480`. No "first non-zero sample detected" log ever fired.

## Root cause

`N_ALSeqPlayer` and `N_ALCSPlayer` store extra bank slots immediately after the primary `ALBank *bank;` field:

```c
ALBank *bank;           /* 8 bytes on LP64, 4 on N64 */
s32 unknown0;           /* HAL added */
s32 unknown1;           /* HAL added */
```

The MIDI handler in `n_env.c` picks between the three banks via the idiom `*(&seqp->bank + idx)` — a poor-man's pointer array that **only works when all three slots share pointer width.**

- **N64:** `ALBank *` is 4 bytes and `s32` is 4 bytes. `(&seqp->bank)[0..2]` walks three 4-byte slots cleanly, each interpreted as an `ALBank *`.
- **LP64:** `ALBank *` is 8 bytes, `s32` is 4 bytes. `(&seqp->bank)[1]` reads 8 bytes starting 8 bytes past `&bank`, which means it reads `unknown0 || unknown1` concatenated — a garbage pointer. `(&seqp->bank)[2]` reads 8 bytes past that into `uspt`.

Compounding the problem, the alt-bank *writes* at `case (AL_SEQ_END_EVT+20)/+21` used `(s32)(intptr_t)seqp->nextEvent.msg.spseq.seq` — the exact pattern in the [Implicit-int LP64 trunc trap](../project_itstruct_arrow_trunc.md) memo, truncating a 64-bit host pointer to 32 bits before storing.

Effect: any `AL_MIDI_FX_CTRL_4` with `byte2 ∈ {1, 2}` would read a bogus pointer; any `n_alCSPSetBank_Alt(seqp, bank, 1|2)` would truncate the bank pointer on write.

## Fix

`include/n_audio/n_libaudio.h` — widen `unknown0` / `unknown1` to `ALBank *` on PORT in both `N_ALSeqPlayer` and `N_ALCSPlayer` so the `(&seqp->bank)[N]` idiom walks three adjacent `ALBank *` slots correctly:

```c
ALBank *bank;
#ifdef PORT
ALBank *unknown0;
ALBank *unknown1;
#else
s32 unknown0;
s32 unknown1;
#endif
```

`src/libultra/n_audio/n_env.c` — drop the `(s32)(intptr_t)` truncation cast in the two bank-set handlers; write the pointer directly under PORT.

## Scope of this fix

This is a latent correctness fix: SSB64 BGM 33 (attract demo) doesn't actually use alt-banks — all `AL_MIDI_FX_CTRL_4` events in the log had `byte2=5`, which is outside the `if (byte2 > 2) break;` gate in the handler. So the fix did not immediately make audio audible. But any scene or sequence that uses CC22 with value 1 or 2 would have hit undefined behaviour — typically a SIGSEGV on the first alt-bank note.

## Still-open issue

The real reason we can't yet hear audio is a separate problem: for every NoteOn in BGM 33, `__n_lookupSoundQuick` returns NULL. The music bank's instrument has `soundCount=322` but all its keymaps look degenerate (`keyMin=keyMax=0` with `velMin=velMax=N`), so the binary search never matches the MIDI `key=86..105` notes actually played. That investigation is captured in `docs/audio_buildout_2026-04-24.md`.

## Fingerprint for the next time

Any N64 decomp struct that declares "scratch" `s32`/`u32` fields immediately adjacent to a true pointer field, where game code addresses the block via `&ptr_field + idx` — audit it. The HAL compact-sequence-player is one example; expect more wherever the SSB64 audio stack keeps parallel ROM-addressable tables.
