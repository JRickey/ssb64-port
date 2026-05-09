/**
 * SymbolResolver.h — Resolve names of functions/globals in the host
 * binary via the embedded PDB (Windows) / DWARF (Linux/macOS).
 *
 * Used by HookManager to locate functions to hook, and by mods to
 * call into engine internals from their handlers.
 */
#pragma once

#include <string>
#include <unordered_map>
#include <mutex>

namespace ssb64::mods {

class SymbolResolver
{
public:
    /* Initialize debug-info access. On Windows this calls SymInitialize
     * with the host process and loads the BattleShip.pdb sitting next
     * to the executable. Safe to call multiple times.
     *
     * Returns true on success, false if debug info is unavailable
     * (e.g. PDB stripped, no DWARF). When false, all subsequent
     * Resolve() calls return nullptr. */
    static bool Init();

    /* Tear down debug-info access. Called from PortShutdown. */
    static void Shutdown();

    /* Resolve a symbol name (function or global) to its address.
     * Returns nullptr if the symbol is not present in the debug info.
     * Cached — repeated lookups of the same name are O(1). */
    static void *Resolve(const char *name);

private:
    static std::mutex sMutex;
    static std::unordered_map<std::string, void*> sCache;
    static bool sInitialized;
};

} // namespace ssb64::mods
