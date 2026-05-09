/*
 * mod.h — BattleShip native C-mod author API.
 *
 * #include this from your mod's .c source. The TCC scripting layer
 * compiles your source at runtime, links against BattleShip.def
 * (auto-generated from the engine binary's export table), and calls
 * your ModInit / ModExit entry points.
 *
 * Mod lifecycle:
 *   - Engine boot loads each mod's manifest.json, compiles its sources,
 *     resolves ModInit by name and calls it. Subscribe to events here.
 *   - Engine shutdown / hot-reload calls ModExit. Unregister listeners
 *     and free anything you allocated.
 *
 * Symbol visibility:
 *   - HM_API marks ModInit / ModExit as exported so the engine's
 *     dlsym/GetProcAddress finds them. Don't use HM_API on internal
 *     helpers — `static` is preferred.
 *
 * Cross-mod calls:
 *   - ScriptGetFunction(modName, "FunctionName") returns an opaque
 *     function pointer to a symbol in another loaded mod. Use the
 *     CALL_FUNC helper macro for the convention `<Func>Symbol` /
 *     `<Func>Func` typedef:
 *
 *       #define MyFooSymbol "MyFoo"
 *       typedef int (*MyFooFunc)(int x);
 *       int result = CALL_FUNC("MyLib", MyFoo, 42);
 */
#pragma once

/* The host engine is compiled with -DPORT, which selects a different
 * layout for many decomp structs (FTAttributes, ITAttributes, etc —
 * pointer fields collapse to u32 reloc tokens under the PORT path).
 * Mods MUST see the same layout, otherwise field offsets shift and
 * cross-boundary struct reads return garbage (e.g. fp->joints[joint_id]
 * lands on random memory mid-FTAttributes). Define before any decomp
 * header is pulled in. */
#ifndef PORT
#define PORT 1
#endif

#if defined(_WIN32) || defined(__CYGWIN__)
    #define HM_API   __declspec(dllexport)
    #define HM_IMPORT __declspec(dllimport)
#else
    #define HM_API    __attribute__((visibility("default")))
    #define HM_IMPORT
#endif

#define MOD_INIT HM_API void ModInit
#define MOD_EXIT HM_API void ModExit

extern void* ScriptGetFunction(const char* module, const char* function);

#define CALL_FUNC(mod, func, ...) ((func##Func)ScriptGetFunction(mod, func##Symbol))(__VA_ARGS__)

/* Logging from inside a mod.
 *
 * mod_log(fmt, ...) routes through the engine's port_log, which writes
 * to %APPDATA%/BattleShip/ssb64.log. Output appears immediately
 * (port_log fflushes after every write). Format string is printf-style.
 *
 * In Debug builds on Windows, libultraship allocates a console at
 * startup and redirects stdout / stderr to it, so printf works and
 * shows up in that window. Release builds have no console, so stdout
 * goes nowhere. mod_log works in both configurations.
 */
extern HM_IMPORT void port_log(const char* fmt, ...);
#define mod_log port_log

/* Hook installation + symbol resolution.
 *
 * mod_install_hook(symbol_name, replacement, original_out) detours the
 * named host function in memory: calls to it now jump to `replacement`,
 * and `*original_out` is set to a trampoline that runs the unhooked
 * original. Returns 0 on success.
 *
 * mod_resolve_symbol(symbol_name) returns the address of any host
 * symbol by name (used to call engine functions that aren't statically
 * resolvable from the mod, e.g. when the function pointer is needed).
 *
 * Hooks are owned by the mod that installed them. When the mod is
 * unloaded (engine shutdown or hot-reload), HookManager walks the
 * mod's chain and removes each hook cleanly, so reloading a hook-
 * installing mod is safe.
 */
extern HM_IMPORT int   mod_install_hook(const char* symbol_name, void* replacement, void** original_out);
extern HM_IMPORT void* mod_resolve_symbol(const char* symbol_name);
