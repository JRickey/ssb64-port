# Particle Banks Silent Empty — FIXED (2026-04-20)

Supersedes [particle_bank_dma_2026-04-08.md](particle_bank_dma_2026-04-08.md).

## Symptoms

Every in-game particle effect silently failed to render, while the underlying
projectiles and sprites still drew:

- Hit sparks between clashing fighters (no sparkle, damage still registers).
- Mario Fire Flower — fire halo animation missing; flower sprite still renders.
- Explosion fire — bomb body renders, flame cloud doesn't.
- Samus aerial-side-B fire breath — hitbox applies, no flames.
- Mario up-B coins — jump animation works, no coin visuals.
- Fireball-vs-fireball collision sparkles absent.
- Title / attract / stage-hazard particles all missing.

No crash, no error in the log, no warning — effects simply did not emit.

## Root cause

The 2026-04-08 particle_bank_dma fix stopped a segfault by registering a
dummy bank inside `efParticleGetLoadBankID`'s `#ifdef PORT` block. The note
on that fix claimed "particle emission code has bounds checks that handle
empty banks gracefully." That was half-true:

1. `efParticleGetLoadBankID` never called `lbParticleSetupBankID`.
2. `lbParticleSetupBankID` is the only writer of
   `sLBParticleScriptBanksNum[bank_id]` and
   `sLBParticleTextureBanksNum[bank_id]` in `src/lb/lbparticle.c`.
3. Those counter arrays live in `.bss` and stayed zero-initialized.
4. Every `lbParticleMakeChildScriptID(id, script_id)` call hit the bounds
   check `if (script_id >= sLBParticleScriptBanksNum[id]) return NULL;`
   with `scripts_num == 0` and returned NULL.
5. Callers did not treat NULL as an error; the emission just dropped.

So the "graceful empty-bank handling" was actually "silently eats every
particle spawn for every bank, forever." That's why every particle effect
across every scene disappeared at once.

## Fix

Real bank loading through the O2R pipeline.

- `yamls/us/particles.yml` already defined 12 BLOB entries (6 script +
  6 texture pairs, plus 3 extra `unk0/1/2` pairs used by fighter-data
  banks). Torch packs them into `ssb64.o2r` under `particles/<name>`.
- New bridge `port/bridge/particle_bank_bridge.{h,cpp}` exposes
  `portParticleLoadBank(scripts_lo, bank_id)`:
  1. Maps `scripts_lo` (the address of `l<Module>ParticleScriptBankLo`
     or `particles_unk{0,1,2}_scb_ROM_START`) to the matching O2R path
     pair via a 9-row static table.
  2. `ensurePristine()` loads the blob via
     `Ship::Context::GetResourceManager()->LoadResource("__OTR__" + path)`,
     casts to `Ship::Blob`, copies into an owned vector, and byte-swaps:
     — Script blob: `scripts_num` + offset table + each `LBScript`
       header through offset 0x30 (the 4 u16s + 10 u32/f32 fields).
       Bytecode past 0x30 is NOT swapped; the dispatcher's
       `lbParticleReadFloatBigEnd`/`lbParticleReadUShort` helpers read
       multi-byte operands as big-endian at runtime, with a `#ifdef PORT`
       branch that handles LE hosts correctly.
     — Texture blob: `textures_num` + offset table + each `LBTexture`
       header (6 u32/s32 fields) + each `data[]` entry (`count` images,
       plus 1 or `count` palettes for CI format). Raw pixel/palette
       bytes are NOT swapped (the TMEM/sprite decoder pipeline handles
       their byte order).
  3. Pristine copies are keyed by archive path and kept forever — one
     load+swap per unique bank across the whole process lifetime.
  4. For each call, evicts any prior working entry with the same
     `scripts_lo` (its tokens were invalidated by
     `portRelocResetPointerTable()` during scene transition), memcpies
     a fresh copy out of the pristine cache, and hands it to
     `lbParticleSetupBankID` which pointerizes the offset tables into
     `PORT_REGISTER` tokens as the decomp layer expects.
- `src/ef/efparticle.c` `#ifdef PORT` block now calls `portParticleLoadBank`
  immediately after assigning the bank_id. If the lookup or load fails
  the bank stays empty (matching the prior stub behavior), logged via
  `port_log` for diagnosis.

## Byte-swap scope reference

Post-load, the following fields are in native-LE order after the bridge
runs. Anything not listed here is raw IDO BE bytes — `LBScript.bytecode`,
`LBTexture` image pixel data, `LBTexture` CI palette bytes — and is
consumed by code that expects BE, either via the bytecode BE readers or
via the Fast3D TMEM swizzle pipeline.

Script blob:
- `scripts_num` (u32 at +0x00)
- `scripts[i]` offset table (u32 each)
- Per `LBScript` at its offset: `kind`, `texture_id`,
  `generator_lifetime`, `particle_lifetime` (u16 each, +0x00..+0x07);
  `flags` (u32 at +0x08); `gravity`, `friction`, `vel.x/y/z`,
  `unk_0x20`, `unk_0x24`, `update_rate`, `size` (f32 each, +0x0C..+0x2F).

Texture blob:
- `textures_num` (u32 at +0x00)
- `textures[i]` offset table (u32 each)
- Per `LBTexture` at its offset: `count`, `fmt`, `siz`, `width`,
  `height`, `flags` (u32 each, +0x00..+0x17);
  `data[i]` offsets (u32 each, total = `count` images + 1 or `count`
  palettes when `fmt == G_IM_FMT_CI`).

## Files touched

- `port/bridge/particle_bank_bridge.h` (new).
- `port/bridge/particle_bank_bridge.cpp` (new).
- `src/ef/efparticle.c` — PORT guard now calls the bridge; the old
  "register dummy bank" paragraph replaced with a docstring that
  describes the bridge path and the empty-bank fallback.

## Follow-up

None known for this bug. Future particle-related failures should look for:

- Visual garbling (wrong colors/sizes/timings) → would indicate a
  byte-swap scope bug in the bridge, not the bytecode interpreter.
- An effect type failing to render on a brand-new bank not in the
  9-row table → add an entry; search for `efParticleGetLoadBankID`
  callers to find any new stub-symbol address.
- Scene-reload memory growth → check `sWorkingEntries` isn't leaking
  (it's evicted on re-load for the same `scripts_lo`, but a new
  stub-address pathway would keep piling up).
