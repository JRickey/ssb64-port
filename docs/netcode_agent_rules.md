# Netcode Agent Rules

## Purpose

This file tracks netcode-specific guidance for future agent sessions. It is a project handoff document, not a Cursor rules file, and it should not override the repository's main developer guidance.

Use this document when working on rollback netcode, netplay input, saved input replay, prediction, session ownership, or interactions between those systems and the core game loop.

## Current Netcode Goal

Build rollback netcode in phases:

1. Resolve local, remote, predicted, and saved inputs into a deterministic per-tick input stream.
2. Publish resolved inputs through existing game-facing controller globals.
3. Add deterministic validation for replayed inputs.
4. Add state snapshot/restore.
5. Add networking and remote input delivery.

## Implemented So Far

- `src/sys/netinput.h` defines `SYNetInputFrame`, `SYNetInputSource`, input history APIs, remote/saved input staging APIs, VS session lifecycle APIs, replay metadata scaffolding, and the VS controller callback.
- `src/sys/netinput.c` owns per-player input source state, resolved input history, remote confirmed input history, saved input history, full-match replay frame storage, prediction fallback, VS-local ticks, recording counters, replay metadata, and publishing into `gSYControllerDevices`.
- `src/sys/netreplay.c` and `src/sys/netreplay.h` implement a debug VS replay file runner for explicit metadata plus `SYNetInputFrame` streams.
- `src/sc/sccommon/scvsbattle.c` routes VS battle input through `syNetInputFuncRead`.
- `src/sc/sccommon/scvsbattle.c` calls `syNetInputStartVSSession()` at VS match start so repeated matches do not inherit previous input history or slot ownership.
- `src/sc/sccommon/scvsbattle.c` calls `syNetReplayStartVSSession()` and `syNetReplayUpdate()` for debug record/playback.
- `src/sc/sc1pmode/sc1pgame.c`, `src/sc/sc1pmode/sc1ptrainingmode.c`, and `src/sc/sc1pmode/sc1pbonusstage.c` remain on `syControllerFuncRead`.
- `docs/netplay_architecture.md` documents the current input architecture.

## Architecture Rules

- Keep `src/sys/controller.c` focused on local hardware input and N64-style controller behavior.
- Put new netplay-only input functions in `src/sys/netinput.c` and declarations in `src/sys/netinput.h`.
- Do not add remote, predicted, or saved input metadata to `SYController`.
- Resolve input source first, then publish the selected result into `gSYControllerDevices[player]`.
- Existing battle/fighter code should continue reading `SYController` fields without knowing the input source.
- Treat `gSYControllerDevices[player]` as the compatibility boundary, not as the source of truth for rollback history.
- Keep netinput integration scoped to VS battle unless the project explicitly decides to support rollback in another mode.
- Keep replay files explicit. Do not serialize raw `SCBattleState`; serialize named metadata fields and rebuild battle state on load.
- Do not involve framebuffer contents in rollback input design. Framebuffers are render output, not simulation input state.

## Match And Session Lifecycle

- Reset net input state at the start of every VS match/session before configuring player slots.
- Clear saved input history unless the session is intentionally a replay.
- Clear confirmed remote input history and last confirmed prediction seed at match start.
- Clear last published input so `button_tap` and `button_release` derive from the new match only.
- Configure player slot ownership after reset: local, remote confirmed/predicted, saved, or future CPU/spectator modes.
- Use the VS-local netinput tick for input frame history; do not key rollback input history directly on `dSYTaskmanUpdateCount`.
- Debug replay record/playback is controlled by `SSB64_REPLAY_RECORD`, `SSB64_REPLAY_PLAY`, and optional `SSB64_REPLAY_RECORD_FRAMES`.

## Determinism Rules

- Canonical netplay input is held buttons plus stick X/Y for a player and tick.
- `button_tap`, `button_release`, and `button_update` are derived fields when publishing to `SYController`.
- Prediction should be deterministic: repeat last confirmed input, or neutral if no confirmed input exists.
- Saved input replay should feed the same canonical frame stream into the same publish path as remote/local input.
- Saved VS replay files should be deterministic engine re-runs using metadata plus `SYNetInputFrame` streams, not video captures.
- Use source-independent input checksums for replay validation so recorded local inputs and replayed saved inputs compare by tick/buttons/stick, not by source tag.
- Any future state hash should be separate from the input checksum and should cover gameplay state, not framebuffer pixels.

## Current Risks

- `SYNETINPUT_HISTORY_LENGTH` is fixed at 720 frames. Revisit this when delay, max rollback window, replay length, and memory ownership are better defined.
- `button_update` currently mirrors newly pressed buttons. Confirm whether any battle path requires the original controller repeat behavior before relying on it for menus or non-battle scenes.
- The current replay serializer is a debug runner, not a final user-facing replay browser or stable long-term file contract.
- Playback currently proves input-stream determinism first. Strong gameplay-state hashes still need to be added.
- No network packet format, session owner model, or snapshot/restore API exists yet.

## Suggested Next Steps

- Define a player slot ownership model: local player, remote peer, saved replay, CPU, inactive.
- Add a narrow VS gameplay-state hash after debug replay input checksums are passing.
- Design the final VS-facing menu flow: Local VS, Netplay, and Replays as separate user-facing entries.
- Define save-state boundaries for rollback after replayed input determinism is validated.

## Verification Expectations

- Build after touching netplay input or battle controller callbacks.
- Test local device input in VS battle after routing through `syNetInputFuncRead`.
- Test that menus, 1P Game, Training Mode, Bonus 1 Practice, and Bonus 2 Practice still use `syControllerFuncRead`.
- For replay work, run from `build/`, record with `SSB64_REPLAY_RECORD=/tmp/test.ssb64r`, replay with `SSB64_REPLAY_PLAY=/tmp/test.ssb64r`, compare the logged input checksum first, then add stronger gameplay-state validation.

## Packaging Note

`scripts/package-linux.sh` currently includes host DT_RELR handling for AppImage packaging. If Linux AppImage generation stops before creating `dist/BattleShip.AppDir/AppRun`, inspect the DT_RELR probe and avoid early-exit pipelines under `set -euo pipefail`.
