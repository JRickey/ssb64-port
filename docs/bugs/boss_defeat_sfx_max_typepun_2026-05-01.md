# Boss-defeat audio crash via N64-only struct type-pun on `D_8009EDD0_406D0`

**Resolved 2026-05-01**

## Symptom

Defeating Master Hand crashed the audio thread with `SIGSEGV` ~1 second
after the boss's death animation started.  The game thread was healthy —
the boss had successfully transitioned through `nFTBossStatusOkutsubushiStart`
(0xf8) → `nFTBossStatusDeadLeft` (0xf9), the death-defeat interface had
fired its 90-tick countdown, and the FGM-defeat sound was queued.  The
crash was on the audio side:

```
0   func_80027460_28060 + 144
1   func_80027460_28060 + 132
2   func_800293A8_29FA8 + 536
3   n_alAudioFrame + 188
4   syAudioThreadMain + 488
fault_addr=0x0  (or another small/garbage value depending on run)
```

`func_80027460_28060` is the FGM bytecode interpreter; it dereferences
`arg0->unk20` (a `u8 *ucode` pointer) inside an active FGM-table node.
`unk20` was set to garbage at allocation time, and the dereference faulted.

## Root cause

`D_8009EDD0_406D0` is **defined** in `src/libultra/n_audio/n_env.c`:

```c
ALWhatever8009EDD0 D_8009EDD0_406D0;     // u8 **fgm_table_data, etc.
```

But it's **declared** in `src/sc/sc1pmode/sc1pgame.c` as a different type:

```c
extern alSoundEffect D_8009EDD0_406D0;   // u16 sfx_max, etc.
```

Both struct types share the convention "field name encodes its N64 byte
offset" — `unk_alsound_0x14`, `unk_alsound_0x18`, etc. — and on N64 the
two layouts coincidentally aligned.  In particular, byte offset `0x28`
held:

- in `alSoundEffect` view: `u16 sfx_max`
- in `ALWhatever8009EDD0` view: `u16 fgm_ucode_count`

So the boss-defeat sequence's

```c
sSC1PGameBossDefeatSoundTerminateTemp = D_8009EDD0_406D0.sfx_max;
D_8009EDD0_406D0.sfx_max = 0;
// … boss-defeat cinematic plays …
D_8009EDD0_406D0.sfx_max = sSC1PGameBossDefeatSoundTerminateTemp;
```

was an intentional N64-side type-pun for "save / zero / restore the
FGM playback gate."  Setting `fgm_ucode_count = 0` made
`func_800269C0_275C0(id)` short-circuit on `id >= count` and refuse to
queue any more FGMs while the cinematic ran.

On LP64 the layouts diverge.  In `ALWhatever8009EDD0`:

- `ALPlayer node` grew from 16 → 32 bytes (3 widened pointers + 1 s32)
- 3 `**` fields (`unk_alsound_0x18`, `fgm_ucode_data`, `fgm_table_data`)
  each grew 4 → 8 bytes

so `fgm_ucode_count` shifted from offset 0x28 → 0x40.

In `alSoundEffect`:

- 4 `void *` fields (`next`, `unk_0x4`, `unk_0x8`, `unk_0xC`) each grew
  4 → 8 bytes (+16)

so `sfx_max` shifted from offset 0x28 → 0x38.

The two views no longer coincide.  At LP64 offset 0x38 in
`ALWhatever8009EDD0` lives the LOW 16 BITS of `fgm_table_data` — a `u8 **`
pointer.  The boss-defeat write `sfx_max = 0` zeroed those 16 bits and
left the high 48 bits intact, leaving a wild pointer.  Every subsequent
FGM-table lookup `D_8009EDD0_406D0.fgm_table_data[id]` read garbage from
that wild address; the audio thread eventually picked up an FGM whose
ucode pointer was either NULL or a junk u64 and faulted on the
bytecode-parser's `*ucode++`.

## Diagnostic that nailed it

Watchpoint at the entry of `func_800293A8_29FA8` (the audio frame
handler) and four allocator/play-entry sites in `n_env.c`, comparing
`D_8009EDD0_406D0.fgm_table_data` to its initialised value:

```
SSB64: WATCH init    fgm_table_data=0x100b251b8 site=alSnd_audioFrame_entry
SSB64: WATCH CHANGED fgm_table_data was=0x100b251b8 now=0x100b20000 site=alSnd_audioFrame_entry
```

`0x100b251b8 → 0x100b20000` zeroed the low 16 bits exactly — the
fingerprint of a u16 write of 0 at the field's address.  None of the
inner sites (`FGM_play_entry`, `FGMtable_play_entry`, allocator
entries, FGM-tick entry/after-save) ever observed the change, which
forced the search outside `n_env.c` and pointed at sc1pgame.c's
`sfx_max = 0`.

## Fix

Two `#ifdef PORT` accessors in `src/libultra/n_audio/n_env.c` that
operate on `D_8009EDD0_406D0.fgm_ucode_count` (the field that actually
carried the "FGM gate" semantic on N64):

```c
void portAudioSaveAndBlockFGMs(u16 *out_saved);
void portAudioRestoreFGMs(u16 saved);
```

`src/sc/sc1pmode/sc1pgame.c` calls these under `#ifdef PORT` instead of
the type-punned `D_8009EDD0_406D0.sfx_max` reads/writes.  The non-PORT
path keeps the original alSoundEffect-typed accesses so IDO byte-matching
on the decomp source isn't disturbed.

## Class

LUS-vs-decomp typename shadow — same family as `oscontpad_lus_sizeof_overrun_2026-04-24`
(libultraship `OSContPad` size mismatch overrunning the decomp's 24-byte
buffer).  Generally: any global declared with two different struct types
across translation units is fine on N64 if the layouts coincide there,
but LP64 widening of pointers/`**` arrays can shift layouts independently
in each view, breaking the alignment.  Look for `extern X Y;` in one TU
and `Z Y;` in another with `X != Z` — those need either explicit per-platform
accessors or a single shared type.
