# BattleShip TCC Mod Template

Starter scaffold for native C mods loaded by BattleShip's runtime TCC scripting layer (ported from Starship / Kenix3 libultraship).

Your mod ships as a `.o2r` archive that wraps the same folder layout described below. The engine compiles each `.c` file at runtime via libtcc, links against `BattleShip.def` (auto-generated from the engine binary's export table), and calls `ModInit` on load / `ModExit` on unload.

The runtime loader (`port/port.cpp:MountModsDir`) handles `.o2r` archives and folder layouts identically through libultraship's `ArchiveManager`, so during development you can drop the unpacked `output/` folder straight into `<install>/mods/` for fast iteration. For distribution, pack it as an `.o2r`.

## Layout

```
template/
├── CMakeLists.txt        # Packaging build: amalgamation + asset copy
├── manifest.json         # Mod metadata (name, version, code-version, main)
├── preview.png           # Optional. Shown in the Mods menu UI. .jpg / .jpeg also work.
├── include/
│   └── mod.h             # MOD_INIT / MOD_EXIT / HM_API macros
├── src/
│   └── demo.c            # Mod logic + event listeners
├── assets/               # Textures, audio, etc. (copied verbatim into output/)
└── output/               # Generated. Drop the folder into BattleShip/mods/.
    ├── manifest.json
    ├── preview.png       # Copied from the source folder if present.
    ├── build.gen
    └── dist/<file>.c
```

## Build

For development iteration (folder install):

```bash
cmake -B build
cmake --build build
```

`output/` now contains the packaged mod. Drop it into `<BattleShip-install>/mods/`. The engine reads `manifest.json`, follows `main: build.gen`, compiles each listed `dist/*.c`, and calls `ModInit`.

For distribution, pack it as `.o2r`. When the template is built inside the engine tree, the engine's `cmake/ModO2R.cmake` module wires up an opt-in target:

```bash
cmake --build build --target mod_template_o2r
```

That runs `torch.exe pack output/ template.o2r o2r` and drops the archive next to `output/`. Drop the `.o2r` into `<BattleShip-install>/mods/` and the runtime loads it the same way it loads the folder.

Outside the engine tree (standalone builds with no `torch.exe` on PATH), pack manually:

```bash
torch pack output/ template.o2r o2r
```

## Mod lifecycle

```c
#include "mod.h"
#include "port/hooks/Events.h"

ListenerID gFrameListenerID;

void OnFrame(IEvent* event) {
    /* per-frame logic */
}

MOD_INIT() {
    gFrameListenerID = REGISTER_LISTENER(GamePostUpdateEvent,
                                         EVENT_PRIORITY_NORMAL, OnFrame);
}

MOD_EXIT() {
    UNREGISTER_LISTENER(GamePostUpdateEvent, gFrameListenerID);
}
```

Available events live in `port/hooks/list/EngineEvent.h` (and any other `list/*.h` headers added later).

## Calling engine functions

Any function in BattleShip's export table is callable from your mod. The post-build step runs `tcc.exe -impdef <BattleShip.exe>` and produces `BattleShip.def`, which libtcc automatically links against. You don't have to declare engine functions — `#include` the relevant decomp / port header and call them.

## Cross-mod calls

If your mod depends on another mod, declare it in `manifest.json`:

```json
{
    "name": "MyMod",
    "dependencies": ["MyLib"],
    ...
}
```

The engine resolves dependency order and loads `MyLib` first. Then call into it via the symbol/typedef pair the dependency exports:

```c
#define MyLib_FooSymbol "MyLib_Foo"
typedef int (*MyLib_FooFunc)(int x, const char *name);

int slot = CALL_FUNC("MyLib", MyLib_Foo, 42, "hello");
```

`CALL_FUNC` looks up `MyLib_Foo` in the named mod's export table and calls it through the typedef. If the lookup fails (mod not loaded, symbol misspelled), it returns NULL through the function pointer and crashes on call - guard with `ScriptGetFunction` directly if you need a soft-fail.

## Hooks

`mod_install_hook(name, replacement, original_out)` detours a named host function so calls to it land in your replacement. The original function pointer comes back through `original_out` so you can chain it. Returns 0 on success.

```c
typedef sb32 (*orig_fn)(GObj *);
static orig_fn s_orig = NULL;

static sb32 my_replacement(GObj *g) {
    /* preamble */
    sb32 rv = s_orig(g);  /* call original */
    /* postamble */
    return rv;
}

MOD_INIT() {
    mod_install_hook("itTomatoFallProcUpdate", my_replacement, (void **)&s_orig);
}
```

Hooks are owned by the mod that installed them. When the mod is unloaded (engine shutdown or hot-reload), the engine removes each of its hooks cleanly before `MOD_INIT` runs again, so reloading a hook-installing mod is safe.

## Resolving engine symbols at runtime

Most engine calls work via `#include` + a normal call (the linker resolves them through `BattleShip.def`). For cases where you need the address of a function as data (passing a callback to vanilla code, building a function table), use:

```c
extern void *mod_resolve_symbol(const char *name);

void *fn = mod_resolve_symbol("itManagerMakeItem");
```

## Logging

`mod_log(fmt, ...)` routes through `port_log` and writes to `%APPDATA%/BattleShip/ssb64.log` immediately (no buffering). Format string is printf-style.

In Debug builds on Windows, libultraship allocates a console at startup and redirects `stdout` / `stderr` to it, so `printf` and `fprintf(stderr, ...)` from a mod show up in that second window too. In Release builds there's no console, so stdout goes nowhere - `mod_log` is the only thing you can rely on across both configurations.

## Hot reload

The engine's Mods menu has a Reload action. It unloads every mod (running each `MOD_EXIT`, removing each mod's owned hooks, freeing its compiled image), recompiles every mod's sources from disk, and re-runs `MOD_INIT` on the fresh build. Works the same for listener-only mods and hook-installing mods.
