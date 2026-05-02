# Architecture

## Source Organization
```
src/
  sys/    — System: main loop, DMA, scheduling, audio, controllers, threading
  ft/     — Fighters: per-character logic (ftmario/, ftkirby/, ftfox/, etc.)
  sc/     — Scene management
  gm/     — Game modes
  gr/     — Graphics/stage rendering
  mn/     — Menus
  it/     — Items
  ef/     — Effects/particles
  lb/     — Library utilities
  mp/     — Maps
  wp/     — Weapons/projectiles
  mv/     — Movie/cutscene
  if/     — Interface/HUD
  credits/ — Credits sequence
  libultra/ — Decompiled N64 SDK (replaced by LUS in port)
```

## Port Target Structure
```
port/         — Modern C++ port layer (Ship::Context, resource factories, bridges)
yamls/us/     — Torch YAML asset extraction configs
libultraship/ — Git submodule
torch/        — Git submodule
CMakeLists.txt
```

## Project Status
- Decompilation is complete: 23/27 functions matched, 4 non-matching (debug only, excluded from port)
- All ~950 function placeholders and ~88 data placeholders renamed
- All 24 debug struct types renamed to descriptive names
- Port repo created, planning phase
- Target integration: libultraship (LUS) + Torch asset pipeline

## ROM Info
- File: `baserom.us.z64` — Z64 big-endian, NTSC-U v1.0
- SHA1: `e2929e10fccc0aa84e5776227e798abc07cedabf`
- MD5: `f7c52568a31aadf26e14dc2b6416b2ed`
- Game code: NALE, internal name "SMASH BROTHERS"

## Key Dependencies
- **libultraship** (Kenix3/libultraship) — replaces libultra with PC-native rendering, audio, input, resource management
- **Torch** (HarbourMasters/Torch) — asset extraction from ROM into .o2r archives via YAML configs
- **Fast3D** — translates N64 F3DEX2 GBI display lists to OpenGL/D3D11/D3D12/Metal
- **SDL2** — windowing, input, audio backend
- Reference ports: HarbourMasters/Starship (Star Fox 64), HarbourMasters/SpaghettiKart (Mario Kart 64)

## Related Architecture Notes
- `docs/netplay_architecture.md` — netplay input staging, prediction, saved input history, and the rollback input boundary

## What NOT to Include in Port
- `src/ovl8/` — Debug overlay (developer tools)
- `src/db/` — Debug battle/menu testing
- Assembly files (`asm/`) — Port uses C code only
- N64 ROM or copyrighted assets — Users provide their own ROM
