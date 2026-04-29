# 1P/Training CSS Re-entry Loses Selected Animation (2026-04-29) — FIXED

**Symptom (issue #7):** When the user re-enters the 1P or Training-Mode
character-select screen (e.g. coming back from the map screen, or after
a 1P-game match ends), the previously-chosen character's portrait is
visible on the slot but the 3D model renders **facing the camera and
playing the default idle animation** instead of replaying the
character's "Selected" win pose. VS-mode CSS re-entry does *not* hit
this — it correctly replays the win pose every time.

## Root cause

The 1P-game and Training-mode slot structs share an
`is_status_selected` flag with the VS slot. The flag is checked inside
`*FighterProcUpdate`:

```c
// src/mn/mnplayers/mnplayers1pgame.c:1717  (analogous block in 1ptraining.c)
if (sMNPlayers1PGameSlot.is_fighter_selected == TRUE) {
    if (DObjGetStruct(fighter_gobj)->rotate.vec.f.y < F_CLC_DTOR32(0.1F)) {
        if (sMNPlayers1PGameSlot.is_status_selected == FALSE) {
            scSubsysFighterSetStatus(.., mnPlayers1PGameGetStatusSelected(fkind));
            sMNPlayers1PGameSlot.is_status_selected = TRUE;
        }
    } else {
        // spin until full revolution, then set status
        rotate.y += 20°;
        if (rotate.y > 360°) {
            rotate.y = 0;
            scSubsysFighterSetStatus(...);
            sMNPlayers1PGameSlot.is_status_selected = TRUE;
        }
    }
}
```

So the win pose is applied once, then `is_status_selected = TRUE`
suppresses the re-apply on every subsequent ProcUpdate.

`mnPlayers1PGameInitPlayer` and `mnPlayers1PTrainingInitPlayer` —
called from each scene's `*InitVars` on every CSS entry — set
`is_fighter_selected`, `is_selected`, etc., but **don't** clear
`is_status_selected`. Re-entering the CSS therefore keeps
`is_status_selected = TRUE` from the prior visit.

`*MakeFighter` then spawns a fresh `FTStruct` with `rotate.y = 0` and
the fighter's default idle status. Next ProcUpdate:

- `is_fighter_selected == TRUE` ✓
- `rotate.y < 0.1F` ✓ (just-spawned fighter is at 0)
- `is_status_selected == TRUE` (stale) ✗ → skip the
  `scSubsysFighterSetStatus(... Win ...)` call

Result: fighter renders forward-facing in idle indefinitely.

`mnPlayersVSInitPlayer` (`src/mn/mnplayers/mnplayersvs.c:4630/4637`)
explicitly sets `is_status_selected = FALSE` in **both** the
`fkind == NULL` and `fkind != NULL` branches, which is why VS-mode
re-entry replays the selected animation correctly. The 1P/training
versions just don't.

## Fix

Add the same `is_status_selected = FALSE` reset to
`mnPlayers1PGameInitPlayer` and `mnPlayers1PTrainingInitPlayer`,
gated by `#ifdef PORT` so the decomp's byte-matching layout for the
N64 build path is left alone:

```c
// src/mn/mnplayers/mnplayers1pgame.c (after the if/else that sets
// is_fighter_selected and is_selected)
#ifdef PORT
    sMNPlayers1PGameSlot.is_status_selected = FALSE;
#endif
```

```c
// src/mn/mnplayers/mnplayers1ptraining.c (same shape, indexed by player)
#ifdef PORT
    sMNPlayers1PTrainingSlots[player].is_status_selected = FALSE;
#endif
```

## Why this isn't observable on N64

The decomp shows the original N64 ROM has the same omission — neither
`InitPlayer` resets `is_status_selected`. On the original hardware
the symptom likely doesn't trigger because `is_status_selected` is
zero-initialised at boot and the user never manages to roundtrip
through CSS without crossing a code path that resets the slot to
zeros (e.g., a top-menu trip that re-runs the static-data init). On
the port, the static-storage struct persists across scene
transitions exactly as designed, so the stale `TRUE` survives and
the bug surfaces. Wrapping the reset in `#ifdef PORT` keeps the
divergence labelled and doesn't disturb the decomp's matching
behavior.

## Why it's not a generic "reset everything" patch

I considered adding a blanket `memset(&sMNPlayers1PGameSlot, 0, ...)`
at the top of each `InitVars`. That'd fix this *and* any other stale
field — but it'd also wipe the few fields that 1P-game intentionally
carries forward (`fkind` / `costume` come from
`gSCManagerSceneData.fkind` set externally before the scene enters,
and the slot's fkind is computed inside `InitPlayer` itself). The
targeted reset matches the VS-mode pattern exactly and is the
minimum diff that makes 1P/training behave like VS.

## Files

- `src/mn/mnplayers/mnplayers1pgame.c` — `mnPlayers1PGameInitPlayer`
- `src/mn/mnplayers/mnplayers1ptraining.c` — `mnPlayers1PTrainingInitPlayer`
