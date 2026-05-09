/*
 * mod_bridge.cpp - C-callable host-side functions exposed to TCC mods.
 *
 * TCC mods call these names directly via TCC's link against
 * BattleShip.def. They wrap the engine subsystems the mods need from
 * inside MOD_INIT and at runtime: HookManager for installing detours
 * and SymbolResolver for looking up engine functions by name.
 *
 *     mod_install_hook(name, repl, &orig)
 *     mod_resolve_symbol(name)
 *
 * Cross-mod calls (one mod calling into another) go through
 * libultraship's ScriptGetFunction, not through this bridge.
 *
 * Symbols are dllexport on the host side; mod.h declares them
 * dllimport for mods. The anchor in port.cpp prevents the linker from
 * dropping this TU at link time.
 */
#if defined(_WIN32)
#  define MOD_BRIDGE_EXPORT extern "C" __declspec(dllexport)
#else
#  define MOD_BRIDGE_EXPORT extern "C" __attribute__((visibility("default")))
#endif

#include "HookManager.h"
#include "SymbolResolver.h"

MOD_BRIDGE_EXPORT int mod_install_hook(const char* symbol_name, void* replacement, void** original_out) {
    return ssb64::mods::HookManager::InstallHook(symbol_name, replacement, original_out);
}

MOD_BRIDGE_EXPORT void* mod_resolve_symbol(const char* symbol_name) {
    return ssb64::mods::SymbolResolver::Resolve(symbol_name);
}
