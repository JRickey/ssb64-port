# CSS Selected Animation VFX/SFX Missing — Investigation Handoff (2026-04-29)

**Issue:** [#8 Graphical effects and sound effects missing from
character selected animations](https://github.com/JRickey/BattleShip/issues/8)
— when a character is locked in on the CSS, the win pose plays its
body animation but the per-character VFX (e.g. Link's sword spark)
and SFX (Link sword swoosh, DK growl, Samus sword unsheathe, Yoshi
"Yoshi!" voice) don't trigger. Reporter video confirms across all
characters.

**Status:** Not fixed. The win-pose animation's motion script triggers
the SFX/VFX via `nFTMotionEventPlayFGM*` and effect-spawn events; the
question is which step in the chain isn't firing on the port.

## Where the events live

- `scSubsysFighterSetStatus(player, nFTDemoStatusWinN)` →
  `ftMainSetStatus(...)` swaps in the win-pose animation + motion
  script for that character.
- Each scene's `*FighterProcUpdate` calls
  `scSubsysFighterProcUpdate(...)` → `ftMainPlayAnimEventsAll(...)` →
  `ftMainPlayAnim(...)` + `ftMainUpdateMotionEventsAll(...)`.
- `ftMainParseMotionEvent` (`src/ft/ftmain.c:158`) handles the event
  opcodes, including `nFTMotionEventPlayFGMStoreInfo`,
  `nFTMotionEventMakeEffect`, etc.

So the selected animation already runs body motion (issue #7's fix
covers that), and the motion-script events are *parsed* — but
something between "event parsed" and "speaker beep / particle on
screen" is missing.

## Theories ranked by likelihood

### Theory A — Motion script isn't fully running for demo-status fighters

`ftMainParseMotionEvent` has an unusual amount of branching on
`fp->pkind != nFTPlayerKindDemo` — it skips many event kinds for
demo-status fighters (the CSS character is `pkind = Demo`-ish during
the selected pose). If `nFTMotionEventPlayFGMStoreInfo` or the
effect-spawn events are similarly gated and the gate is set wrong on
the port, the events parse but no audible/visible side-effect runs.

Validate: read `ftMainParseMotionEvent` for every event opcode the
selected animation uses and check whether each one has a
`pkind != Demo` guard. If the SFX play paths are inside such a
guard, that's the bug. (Note that the *attack-collision* events being
gated is intentional — demo fighters don't damage anything — but SFX
ought to play.)

### Theory B — Motion script halfswap/byteswap corruption

The selected-animation motion scripts are stored in the per-fighter
figatree heap, which has been a long-running source of byteswap and
halfswap bugs (see `aobjevent32_halfswap_2026-04-18.md`,
`spline_interp_block_halfswap_2026-04-25.md`,
`fighter_appearr_body_frozen_2026-04-22.md`). If the FTMotionScript
event stream for the win pose has bytes in the wrong order, the
parser reads garbage opcodes and either skips the SFX/effect events
or hits a no-op opcode where they should be.

Validate: dump `ms->p_script` bytes at the moment
`scSubsysFighterSetStatus(... Win ...)` returns and compare against
the equivalent on N64 (or compare the figatree extraction against the
ROM's). If bytes are halfswapped, this is the bug.

### Theory C — `ifPublicTryStartCall` voice path is locked out

Voices like Yoshi's "Yoshi!" run through `ftPublicTryStartCall`/
`ftPublicPlayCommon`. The CSS has a separate audio context from
in-game; if the CSS scene's BGM-only audio routing prevents per-
fighter voices from reaching the mixer, voices are silent even though
their motion event fires. Sword-swoosh / spark VFX would still be
suppressed by a *different* gate, so this would be a partial
explanation only.

Validate: instrument `ftPublicTryStartCall` to log every entry. If
the CSS path enters but the mixer doesn't pick up the FGM, the
routing is the bug. If the CSS path doesn't enter, the motion-event
parser isn't firing the call.

### Theory D — pkind-based guard on FX

Many engine effects skip if `fp->pkind == nFTPlayerKindDemo` to avoid
a CPU-controlled or background fighter spamming effects during cut-
scenes. The CSS fighter is set with a Demo-ish pkind during the win
pose; check `efManager*` and any effect-spawn paths reachable from
`ftMainParseMotionEvent`'s effect events.

## Where to start

1. Pick Yoshi (his "Yoshi!" voice is the cleanest test — no body
   contact, no projectile, just a voice clip). Step through
   `ftMainParseMotionEvent` for the first 60 frames after
   `scSubsysFighterSetStatus(... Win2 ...)` and watch the opcode
   trace.
2. The reporter's GBI trace covers Link/Yoshi/DK CSS entries
   sequentially. Cross-reference frame numbers between trace and
   audio capture (the reporter also attached the video) to nail down
   *exact* missing SFX times.

## Why I'm not patching blind

The win-pose motion scripts are character-specific and the events use
several gating bits. A speculative "always play FGM regardless of
pkind" patch would unblock the SFX *and* unblock attack hits during
demo fighters, which is wrong. Same for VFX. Need a real motion-event
trace before changing any gate.
