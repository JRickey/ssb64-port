# Samus charge whoosh persists after pause (no SP handshake to gate the FGM bytecode tick on the port)

**Resolved 2026-05-03**

## Symptom

Pausing during Samus's neutral-special charge (the looping plasma "whoosh"
that plays while the charge meter ramps from level 0 to 7) left the loop
SFX audibly playing through the pause menu and on into the resumed game.
The user described it as the whoosh "persisting and not turning back off"
— the loop kept retriggering until Samus's status next changed (firing
the shot, getting hit out of charge, escaping the move via shield/jump).
Worked correctly on IDO/N64.

## Root cause — the IDO/LP64 divergence

The pause-entry path `ifCommonBattlePauseInitInterface`
(`src/if/ifcommon.c:3029`) does not contain an explicit "silence active
loops" call on either platform. Reading the function:

```c
void ifCommonBattlePauseInitInterface(s32 player)
{
    /* … */
    func_80026594_27194();                            // (1)
    func_800269C0_275C0(nSYAudioFGMGamePause);        // (2) play pause beep
    syAudioSetBGMVolume(0, 0x3C00);                   // (3) dim BGM
}
```

The naïve read of (1) is "stop all active SFX" — but
`func_80026594_27194` (`src/libultra/n_audio/n_env.c:5414`) is a
**garbage collector**, not a stop-all. It walks `unk_alsound_0x3C` /
`unk_alsound_0x40` and only relocates entries already tagged with the
deletion bit (`unk2B & 0x80` / `_0x1F & 0x80`). The fighter-loop tag is
sourced from the FGM bytecode (`0xD3` opcode) and from
`D_8009EDD0_406D0.unk_alsound_0x4C`, neither of which sets the bit for
the charge-loop FGM, so on IDO too `func_80026594_27194` walks past the
whoosh's siz34 slot. (2) and (3) likewise do not stop existing voices.

So how does IDO silence the whoosh? **By the audio thread descheduling
on its own.** Look at the N64-side audio thread (`src/sys/audio.c:1350`+):

```c
while (TRUE) {
    /* … build audio Acmd list via n_alAudioFrame … */
    /* … submit SP audio task … */
    osRecvMesg(&sSYAudioTicMesgQueue, NULL, OS_MESG_BLOCK);    /* VI tick   */
    osRecvMesg(&sSYAudioSPTaskMesgQueue, NULL, OS_MESG_BLOCK); /* SP done   */
    /* … per-frame queue maintenance, FGM-player cleanup … */
}
```

Each iteration submits an SP task and then blocks on **two** message
queues: VI retrace tick and SP-task-done. While the pause menu is up,
the SP is busy with the menu's display-list workload and the audio
thread blocks on `osRecvMesg(&sSYAudioSPTaskMesgQueue)`. The FGM
bytecode interpreter (`func_80026B90_27790` / `func_80027460_28060`)
stops ticking, the looping voice's bytecode stops re-arming the next
note, and the voice winds down within an audio frame or two.

The port doesn't have an SP. Its audio thread (PORT branch in
`src/sys/audio.c:1139`) is a plain OS thread driven by the VI tick
queue alone:

```c
while (TRUE) {
    osRecvMesg(&sSYAudioTicMesgQueue, NULL, OS_MESG_BLOCK);
    /* … n_alAudioFrame + portAudioSubmitFrame … */
}
```

No SP handshake. The thread keeps rolling at full VI cadence regardless
of game state, the FGM bytecode keeps cycling, and the loop voice keeps
re-triggering — audibly — through the entire pause and on into the
resumed game until Samus's status next changes.

Compounding the issue (not the primary cause but the same family), the
resume-side queue-maintenance routine `func_800264A4_270A4`
(`src/libultra/n_audio/n_env.c:5479`) is short-circuited under
`#ifdef PORT` to dodge a macOS/arm64 NULL deref, leaving the
deletion-queue dance the original game uses for FGM bookkeeping
half-broken on the port too.

## Why the obvious fix didn't work

The first attempt was the polite "do what fire-time already does":
walk every fighter, call `ftParamStopLoopSFX(fp)` on each. That
routes through `func_80026738_27338`, which sets `EE0C->unk2A = 2`
on the EE0C cached at `siz34->unk_0x28`. The audio thread's FGM
interpreter (`func_80027460_28060`) then handles `unk2A == 2` by
calling `n_alSynSetVol(&voice, 0, fade)` and transitioning to
`unk2A = 0`; next audio frame, `func_800293A8_29FA8` stops + frees
the voice. Same pattern Samus's fire-time path already uses at
`ftsamusspecialn.c:224`.

**It didn't silence the whoosh.** Per-frame instrumentation pinned
the failure: the EE0C we marked was *not on the FGM tick list*
`unk_alsound_0x3C` at the moment we set `unk2A = 2`. Sample log:

```
LoopSFX: PLAY fp=…DBE0 sfx_id=239 → p_loop_sfx=…E680 loop_sfx_id=20
LoopSFX: ftParamStopAllFightersLoopSFX ENTER
LoopSFX: STOP fp=…DBE0 p_loop_sfx=…E680 live_id=20 snap_id=20
LoopSFX: func_80026738_27338 arg=…E680 match=1 with_ee0c=1 marked_ee0c=…DAB0
LoopSFX: PRE-tick[0] whoosh_ee0c=…DAB0 on_0x3C=0 unk2A=2 unk28=0
LoopSFX: PRE-tick[1] whoosh_ee0c=…DAB0 on_0x3C=0 unk2A=2 unk28=0
…
```

`on_0x3C=0` for every audio frame after pause: the EE0C ptr cached
in `siz34->unk_0x28` was already stale. The whoosh is implemented
as a chain of short notes — each iteration of the bytecode allocates
an EE0C, plays a note (`case 3` → `case 1`), then transitions
`case 2 → case 0` and the cleanup pass frees the EE0C back to
`unk_alsound_0x34` and removes it from `0x3C`. The bytecode then
allocates a *new* EE0C for the next note and overwrites
`siz34->unk_0x28`. By the time pause-init reads the cached ptr, it
points at an EE0C that's either freed or repurposed; the *audible*
voice is on a different EE0C we never touched.

This is the LP64 divergence. On IDO/N64 the same race window exists
in principle, but the audio thread is gated each frame by
`osRecvMesg(&sSYAudioSPTaskMesgQueue)` (the SP-task-done handshake).
With the SP busy serving the pause menu, that message stops landing,
the audio thread blocks, the FGM bytecode interpreter stops ticking
entirely, and the looping voice winds down because the wavetable
loop region stops getting re-armed by `case 1` (params update) ticks.
The port has no SP and no handshake — the audio thread runs on the
VI tick alone, so the bytecode keeps cycling and the voice keeps
playing through whatever wavetable loop region the synthesizer is
holding.

## Fix

Walk `unk_alsound_0x3C` directly — the live FGM EE0C list — and
force-stop every voice on it via `n_alSynStopVoice` +
`n_alSynFreeVoice` (the synthesizer-level kill, bypassing the
envelope-mixer fade-to-zero path). That hits whichever EE0C is
currently holding the audible voice, by definition.

`decomp/src/libultra/n_audio/n_env.c` — new PORT-only routine after
the existing `portAudioSaveAndBlockFGMs` / `portAudioRestoreFGMs`
accessors:

```c
void portAudioPurgeFGMs(void)
{
    u32 sp = osSetIntMask(1);
    ALWhatever8009EE0C *node = D_8009EDD0_406D0.unk_alsound_0x3C;
    while (node != NULL) {
        ALWhatever8009EE0C *next = (ALWhatever8009EE0C*)node->next;
        N_ALVoice *v = (N_ALVoice *)&node->voice;
        if (v->pvoice != NULL) {
            n_alSynStopVoice(v);
            n_alSynFreeVoice(v);
        }
        node->unk28 = 0;
        node->unk2A = 0;
        node->unk48 = 0;
        node = next;
    }
    {
        ALWhatever8009EDD0_siz34 *s = D_8009EDD0_406D0.unk_alsound_0x40;
        while (s != NULL) {
            s->unkALWhatever8009EDD0_siz34_0x28 = NULL;
            s = s->next;
        }
    }
    osSetIntMask(sp);
}
```

`decomp/src/ft/ftparam.c` — the per-fighter walker now just clears
each fighter's cached loop-SFX state and delegates the actual voice
termination to the n_env.c routine:

```c
#ifdef PORT
extern void portAudioPurgeFGMs(void);

void ftParamStopAllFightersLoopSFX(void)
{
    GObj *fighter_gobj = gGCCommonLinks[nGCCommonLinkIDFighter];

    while (fighter_gobj != NULL) {
        FTStruct *fp = ftGetStruct(fighter_gobj);
        fp->p_loop_sfx = NULL;
        fp->loop_sfx_id = 0;
        fighter_gobj = fighter_gobj->link_next;
    }
    portAudioPurgeFGMs();
}
#endif
```

`decomp/src/if/ifcommon.c` — call from
`ifCommonBattlePauseInitInterface` before the existing
`func_80026594_27194`:

```c
#ifdef PORT
    ftParamStopAllFightersLoopSFX();
#endif
```

## Hotfix vs root cause

This is a **hotfix** at pause-init only. It addresses the specific
symptom (Samus's charge whoosh on pause) and any other per-fighter
loop SFX that lands in `fp->p_loop_sfx`. Two real root-cause fixes
exist; both have wider blast radius and are out of scope for this
patch:

- **(a) Gate the port's `n_alAudioFrame` on game state.** Skip the
  audio frame entirely while `gSCManagerBattleState->game_status ==
  Pause`. Emulates the SP-starvation deschedule on N64. Side effect:
  short SFX in their decay tail at the moment of pause cut off
  abruptly.
- **(b) Add real game/audio thread synchronisation around FGM state.**
  Lock `siz34->unk_0x28` reads/writes so the cached EE0C ptr is never
  stale when game-thread code reaches it. Lets per-fighter
  `ftParamStopLoopSFX` work as designed. Invasive across the audio
  engine.

Either would replace `portAudioPurgeFGMs` and the
`ftParamStopAllFightersLoopSFX` hook; until then the queue-wide purge
is the working hotfix.

## Behaviour after the fix

- Pause-init force-stops every active FGM voice (charge whoosh and
  anything else in flight on `unk_alsound_0x3C`). All EE0C nodes have
  their voices torn down at the synthesizer level.
- Short SFX in their decay tail at the moment of pause cut off too —
  acceptable trade for a transition into the muted-pause state.
- The pause beep (`func_800269C0_275C0(nSYAudioFGMGamePause)`) is
  allocated *after* the purge on a fresh slot from the EE0C / siz34
  free lists, so it plays normally.
- BGM is on the `CSPlayer` / `CSeq` pipeline, not the FGM siz34/EE0C
  system; untouched.
- The loop does **not** re-arm on resume. For the charge whoosh that's
  fine: the weapon GObj is still alive, the charge level keeps ramping,
  and the next status change tears the (now-already-stopped) loop down
  with the weapon GObj.

## File references

- `decomp/src/ft/ftparam.c:339` — `ftParamPlayLoopSFX`
- `decomp/src/ft/ftparam.c:350` — `ftParamStopLoopSFX`
- `decomp/src/ft/ftparam.c:363` — new `ftParamStopAllFightersLoopSFX` (PORT)
- `decomp/src/if/ifcommon.c:3029` — `ifCommonBattlePauseInitInterface`
- `decomp/src/libultra/n_audio/n_env.c` — new `portAudioPurgeFGMs` (PORT)
- `decomp/src/wp/wpsamus/wpsamuschargeshot.c:296` — charge whoosh start site
- `decomp/src/sys/audio.c:1350` — IDO audio thread loop (SP-handshake gated)
- `decomp/src/sys/audio.c:1139` — PORT audio thread loop (VI-only, no gate)
- `decomp/src/libultra/n_audio/n_env.c:5414` — `func_80026594_27194` (GC, not stop-all)
- `decomp/src/libultra/n_audio/n_env.c:5479` — `func_800264A4_270A4` (PORT stub)
- `decomp/src/libultra/n_audio/n_env.c:5324` — `func_80026738_27338` (single-EE0C polite stop, unreliable here)

## Audit hook

Anywhere on N64 the audio thread's voice shutdown was implicitly driven
by the SP handshake — i.e. "let the FGM bytecode wind down because the
audio thread will block waiting for the SP" — the port has to stop
voices explicitly. Pause is the obvious case; cinematic transitions,
scene swaps, and any state where the original game deliberately starves
the SP for one or more audio frames are the next likely candidates if a
similar "voice keeps playing past where it should" symptom appears.
