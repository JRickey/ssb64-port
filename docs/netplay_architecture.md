# Netplay Architecture

## Purpose

`src/sys/netinput.c` and `src/sys/netinput.h` prepare the VS Mode controller path for rollback netcode without adding networking or save-state rewind yet. The module owns deterministic per-player input samples for each VS simulation tick, records those samples in bounded history buffers, and republishes the resolved input through the existing `SYController` globals that battle code already consumes.

Companion handoff notes for future agent sessions live in `docs/netcode_agent_rules.md`.

This keeps the first netplay boundary at the controller layer:

- local hardware input still comes from `src/sys/controller.c`
- battle simulation still reads `gSYControllerDevices`
- netplay-only input policy lives in `src/sys/netinput.c`
- debug replay file I/O lives in `src/sys/netreplay.c`

## Current Integration

VS battle uses `syNetInputFuncRead` as its controller callback. 1P Game, Training Mode, Bonus 1 Practice, Bonus 2 Practice, menus, and other non-VS scenes remain on `syControllerFuncRead`.

The VS update order is:

```text
VS scene start
  -> syNetInputStartVSSession()
  -> syNetReplayStartVSSession()
  -> netinput tick starts at 0

taskman game tick during VS
  -> scene controller callback
  -> syNetInputFuncRead()
  -> syControllerFuncRead()
  -> resolve one SYNetInputFrame per player for the current tick
  -> publish resolved frames into gSYControllerDevices
  -> advance netinput tick
  -> scene_update()
  -> syNetReplayUpdate()
  -> fighter input derivation
```

`syNetInputGetTick()` returns a VS-local tick counter reset by `syNetInputStartVSSession()`. `dSYTaskmanUpdateCount` remains part of the engine, but netplay input history is keyed by match-local time so rematches and scene transitions do not reuse stale global tick assumptions.

## Canonical Input Frame

`SYNetInputFrame` is the deterministic input record used by netplay:

```c
typedef struct SYNetInputFrame
{
    u32 tick;
    u16 buttons;
    s8 stick_x;
    s8 stick_y;
    u8 source;
    ub8 is_predicted;
    ub8 is_valid;

} SYNetInputFrame;
```

The canonical frame stores held buttons and analog stick position only. `SYController.button_tap`, `button_release`, and `button_update` are derived when publishing back into the legacy controller globals.

## Input Sources

Each controller slot has a `SYNetInputSource`:

| Source | Meaning |
| ------ | ------- |
| `nSYNetInputSourceLocal` | Read the current local `gSYControllerDevices[player]` state after `syControllerFuncRead()` updates it. |
| `nSYNetInputSourceRemoteConfirmed` | Use a remote input sample already delivered for this tick. |
| `nSYNetInputSourceRemotePredicted` | Use confirmed remote input if present; otherwise predict from the last confirmed remote input, or neutral if none exists. |
| `nSYNetInputSourceSaved` | Use a saved input sample for replay or deterministic validation. |

Slots default to local input. This means routing VS battle through `syNetInputFuncRead()` should preserve local controller behavior until a caller explicitly changes a VS slot source.

## History Buffers

`netinput.c` keeps three bounded ring buffers per player:

- `sSYNetInputHistory` — resolved and published inputs used by simulation
- `sSYNetInputRemoteHistory` — confirmed remote inputs staged by tick
- `sSYNetInputSavedHistory` — saved/replay inputs staged by tick

All buffers use `SYNETINPUT_HISTORY_LENGTH` and index by `tick % SYNETINPUT_HISTORY_LENGTH`. Reads validate both `is_valid` and exact `tick` to reject stale ring entries.

For full-match debug replay files, `netinput.c` also keeps a separate replay frame stream capped by `SYNETINPUT_REPLAY_MAX_FRAMES`. This avoids treating the 720-frame rollback ring as permanent replay storage.

## Publishing To SYController

`syNetInputPublishFrame()` converts a canonical frame back into the existing `SYController` shape:

- `button_hold` comes from `SYNetInputFrame.buttons`
- `stick_range.x/y` come from `stick_x/stick_y`
- `button_tap` is computed from the previous published canonical buttons
- `button_release` is computed from the previous published canonical buttons
- `button_update` currently mirrors newly pressed buttons

`syNetInputPublishMainController()` mirrors player 0 into `gSYControllerMain` for systems that read the main controller global.

## Public API

`src/sys/netinput.h` exposes the current VS netplay input surface:

| Function | Role |
| -------- | ---- |
| `syNetInputReset()` | Reset slot sources and all input histories. |
| `syNetInputStartVSSession()` | Reset netinput state for a new VS match and default slots to local input. |
| `syNetInputGetTick()` | Return the current VS-local simulation tick used for input history. |
| `syNetInputSetTick()` | Set the VS-local tick, reserved for future rollback/replay control. |
| `syNetInputSetSlotSource()` | Select local, remote, predicted, or saved input for a player slot. |
| `syNetInputGetSlotSource()` | Inspect a player slot's current source. |
| `syNetInputSetRemoteInput()` | Stage a confirmed remote input sample for a tick. |
| `syNetInputSetSavedInput()` | Stage a saved/replay input sample for a tick. |
| `syNetInputGetHistoryFrame()` | Read the resolved input actually published for a player/tick. |
| `syNetInputGetPublishedFrame()` | Read the latest published input for a player. |
| `syNetInputGetHistoryChecksum()` | Produce a lightweight checksum over resolved input history for validation. |
| `syNetInputGetHistoryInputChecksum()` | Produce a source-independent checksum over published buttons/sticks for replay validation. |
| `syNetInputSetRecordingEnabled()` | Enable or disable recording of resolved VS input frames. |
| `syNetInputGetRecordingEnabled()` | Inspect whether resolved VS input recording is enabled. |
| `syNetInputGetRecordedFrameCount()` | Read the number of VS ticks recorded since recording was enabled. |
| `syNetInputClearReplayFrames()` | Clear the full-match replay frame stream. |
| `syNetInputSetReplayFrame()` | Store one replay frame for a player and tick. |
| `syNetInputGetReplayFrame()` | Read one replay frame for a player and tick. |
| `syNetInputGetReplayInputChecksum()` | Produce a source-independent checksum over the full-match replay stream. |
| `syNetInputSetReplayMetadata()` | Stage metadata needed by future saved VS replay files. |
| `syNetInputGetReplayMetadata()` | Read staged VS replay metadata if available. |
| `syNetInputFuncRead()` | VS controller callback that resolves and publishes current-tick inputs. |

`src/sys/netreplay.h` exposes the debug runner surface:

| Function | Role |
| -------- | ---- |
| `syNetReplayInitDebugEnv()` | Read debug replay environment variables at port startup. |
| `syNetReplayStartVSSession()` | Configure record or playback state after a clean VS netinput session starts. |
| `syNetReplayUpdate()` | Write a record file at the frame limit or verify playback once enough frames have run. |
| `syNetReplayWriteDebugFile()` | Write explicit replay metadata and input frames to disk. |
| `syNetReplayLoadDebugFile()` | Load a debug replay file for playback. |

## VS Replay Direction

Saved VS replays are deterministic engine re-runs, not video captures. The current debug replay file includes:

- magic and version
- metadata size, frame size, frame count, player count, and input checksum
- VS scene/mode identifier
- initial battle settings and player slot setup
- stage, stocks/time/rules, item settings, characters, costumes, teams, handicaps, CPU levels, and match-start RNG seed
- per-player `SYNetInputFrame` stream
- source-independent input checksum for record/playback comparison

Playback resets the VS netinput session, loads metadata, stages full-match replay inputs, sets replayed slots to `nSYNetInputSourceSaved`, restores the match-start RNG seed, and runs the normal game engine.

The debug runner is controlled with environment variables:

```sh
cd build
SSB64_REPLAY_RECORD=/tmp/test.ssb64r SSB64_REPLAY_RECORD_FRAMES=1800 ./BattleShip
SSB64_REPLAY_PLAY=/tmp/test.ssb64r ./BattleShip
```

`SSB64_REPLAY_RECORD_FRAMES` is optional and defaults to 1800 frames. Record mode still uses the normal VS menus; playback mode loads the file and jumps directly into VS battle using the saved metadata.

## Validation Path

Before adding sockets or rollback state restoration, use the saved-input path to validate deterministic VS input replay:

1. Run a local battle with all slots set to local input.
2. Capture `sSYNetInputHistory` through `syNetInputGetHistoryFrame()`.
3. Re-stage those samples with `syNetInputSetSavedInput()`.
4. Set the relevant slots to `nSYNetInputSourceSaved`.
5. Compare `syNetInputGetHistoryInputChecksum()` against the replay file checksum.

This verifies the input layer can reproduce the same per-tick controller stream before introducing network transport or state rewind.

## Non-Goals

This module does not yet implement:

- network sockets or packet formats
- matchmaking, lobby, or peer/session ownership
- game-state snapshot/restore
- framebuffer rollback
- full determinism hashing of fighter/world state
- netplay support for 1P Game, Training Mode, Bonus 1 Practice, or Bonus 2 Practice

Those systems should build on top of this input boundary rather than bypassing it.
