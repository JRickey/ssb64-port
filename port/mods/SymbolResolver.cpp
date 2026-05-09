#include "SymbolResolver.h"
#include "../port_log.h"

#include <cstring>

#ifdef _WIN32
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#  include <dbghelp.h>
#  pragma comment(lib, "dbghelp.lib")
#elif defined(__linux__) || defined(__APPLE__)
#  include <dlfcn.h>
#endif

namespace ssb64::mods {

std::mutex SymbolResolver::sMutex;
std::unordered_map<std::string, void*> SymbolResolver::sCache;
bool SymbolResolver::sInitialized = false;

bool SymbolResolver::Init()
{
    std::lock_guard<std::mutex> lock(sMutex);
    if (sInitialized) {
        return true;
    }

#ifdef _WIN32
    /* port.cpp already calls SymInitialize/SymRefreshModuleList during
     * crash handler setup, but only on first crash. We need it eagerly
     * during startup so mods can resolve symbols before any crash.
     * Calling SymInitialize twice on the same process returns FALSE
     * with ERROR_INVALID_PARAMETER but leaves the previous handle
     * valid — safe to ignore that specific failure. */
    HANDLE proc = GetCurrentProcess();
    SymSetOptions(SymGetOptions() | SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);
    if (!SymInitialize(proc, nullptr, TRUE)) {
        DWORD err = GetLastError();
        if (err != ERROR_INVALID_PARAMETER) {
            port_log("[mods] SymInitialize failed (err=%lu)\n", err);
            return false;
        }
        /* Already initialized — refresh module list to pick up any
         * DLLs (mods + libultraship) loaded since the prior init. */
        SymRefreshModuleList(proc);
    }
    sInitialized = true;
    return true;
#else
    /* Linux/macOS: use dlsym(RTLD_DEFAULT) for resolution. No init
     * needed, but flag as initialized for the resolve fast-path. */
    sInitialized = true;
    return true;
#endif
}

void SymbolResolver::Shutdown()
{
    std::lock_guard<std::mutex> lock(sMutex);
    if (!sInitialized) {
        return;
    }
#ifdef _WIN32
    SymCleanup(GetCurrentProcess());
#endif
    sCache.clear();
    sInitialized = false;
}

void *SymbolResolver::Resolve(const char *name)
{
    if (name == nullptr || name[0] == '\0') {
        return nullptr;
    }
    std::lock_guard<std::mutex> lock(sMutex);
    if (!sInitialized) {
        return nullptr;
    }

    /* Fast path: cached. */
    auto it = sCache.find(name);
    if (it != sCache.end()) {
        return it->second;
    }

    void *addr = nullptr;

#ifdef _WIN32
    /* SYMBOL_INFO has a flexible array at the end for the demangled
     * name; allocate enough room for MAX_SYM_NAME chars. */
    constexpr size_t kBufSize = sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR);
    char buf[kBufSize];
    SYMBOL_INFO *si = reinterpret_cast<SYMBOL_INFO*>(buf);
    std::memset(si, 0, kBufSize);
    si->SizeOfStruct = sizeof(SYMBOL_INFO);
    si->MaxNameLen   = MAX_SYM_NAME;

    if (SymFromName(GetCurrentProcess(), name, si)) {
        addr = reinterpret_cast<void*>(static_cast<uintptr_t>(si->Address));
    } else {
        /* Some symbols (statics, file-scope) are namespaced in the PDB
         * by their compilation unit. Try a wildcard lookup as a fallback;
         * SymEnumSymbols with a `*name` mask matches any container. */
        DWORD err = GetLastError();
        if (err != ERROR_MOD_NOT_FOUND) {
            /* Reserved for future enhancement; for now, just log misses. */
        }
    }
#elif defined(__linux__) || defined(__APPLE__)
    addr = dlsym(RTLD_DEFAULT, name);
#endif

    if (addr != nullptr) {
        sCache.emplace(name, addr);
    }
    return addr;
}

} // namespace ssb64::mods
