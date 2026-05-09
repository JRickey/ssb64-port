#include "HookManager.h"
#include "SymbolResolver.h"
#include "../port_log.h"

#include <MinHook.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <algorithm>
#include <cstring>

namespace ssb64::mods {

std::mutex HookManager::sMutex;
std::vector<HookManager::HookRecord> HookManager::sHooks;
std::map<void *, HookManager::ChainState> HookManager::sChains;
std::string HookManager::sCurrentOwner;
bool HookManager::sInitialized = false;

/* x86-64 thunk that does an indirect JMP through a pointer slot
 * stored inline. 14 bytes total:
 *   FF 25 00 00 00 00     jmp [rip+0]      ; jump to qword at next 8 bytes
 *   PP PP PP PP PP PP PP PP                ; pointer literal (the call target)
 *
 * Updating the thunk's target = overwrite the 8 bytes at offset 6.
 * Allocated with PAGE_EXECUTE_READWRITE so we can rewrite the slot
 * without VirtualProtect dance on each update.
 */
#define IE_THUNK_SIZE   14
#define IE_THUNK_PTR_OFF 6

void *HookManager::AllocThunk(void *initial_target)
{
    void *mem = VirtualAlloc(nullptr, IE_THUNK_SIZE,
                             MEM_COMMIT | MEM_RESERVE,
                             PAGE_EXECUTE_READWRITE);
    if (mem == nullptr) {
        return nullptr;
    }
    unsigned char *p = (unsigned char *)mem;
    p[0] = 0xFF; p[1] = 0x25;
    p[2] = 0x00; p[3] = 0x00; p[4] = 0x00; p[5] = 0x00;
    std::memcpy(p + IE_THUNK_PTR_OFF, &initial_target, sizeof(void *));
    return mem;
}

void HookManager::SetThunkTarget(void *thunk, void *new_target)
{
    if (thunk == nullptr) return;
    unsigned char *p = (unsigned char *)thunk;
    std::memcpy(p + IE_THUNK_PTR_OFF, &new_target, sizeof(void *));
}

static const char *MhStatusName(MH_STATUS s)
{
    switch (s) {
    case MH_OK:                     return "MH_OK";
    case MH_ERROR_ALREADY_INITIALIZED: return "MH_ERROR_ALREADY_INITIALIZED";
    case MH_ERROR_NOT_INITIALIZED:  return "MH_ERROR_NOT_INITIALIZED";
    case MH_ERROR_ALREADY_CREATED:  return "MH_ERROR_ALREADY_CREATED";
    case MH_ERROR_NOT_CREATED:      return "MH_ERROR_NOT_CREATED";
    case MH_ERROR_ENABLED:          return "MH_ERROR_ENABLED";
    case MH_ERROR_DISABLED:         return "MH_ERROR_DISABLED";
    case MH_ERROR_NOT_EXECUTABLE:   return "MH_ERROR_NOT_EXECUTABLE";
    case MH_ERROR_UNSUPPORTED_FUNCTION: return "MH_ERROR_UNSUPPORTED_FUNCTION";
    case MH_ERROR_MEMORY_ALLOC:     return "MH_ERROR_MEMORY_ALLOC";
    case MH_ERROR_MEMORY_PROTECT:   return "MH_ERROR_MEMORY_PROTECT";
    case MH_ERROR_MODULE_NOT_FOUND: return "MH_ERROR_MODULE_NOT_FOUND";
    case MH_ERROR_FUNCTION_NOT_FOUND: return "MH_ERROR_FUNCTION_NOT_FOUND";
    default:                        return "MH_UNKNOWN";
    }
}

bool HookManager::Init()
{
    std::lock_guard<std::mutex> lock(sMutex);
    if (sInitialized) {
        return true;
    }
    MH_STATUS s = MH_Initialize();
    if (s != MH_OK) {
        port_log("[mods] MH_Initialize failed: %s\n", MhStatusName(s));
        return false;
    }
    sInitialized = true;
    return true;
}

void HookManager::Shutdown()
{
    std::lock_guard<std::mutex> lock(sMutex);
    if (!sInitialized) {
        return;
    }
    /* DisableAll + RemoveAll then Uninitialize. MinHook tracks all
     * installed hooks internally; we don't need to walk sHooks
     * individually. The vector is just for diagnostic logging. */
    MH_DisableHook(MH_ALL_HOOKS);
    MH_Uninitialize();
    sHooks.clear();
    sInitialized = false;
}

int HookManager::InstallHook(const char *symbol_name,
                             void       *replacement,
                             void      **original_out)
{
    if (original_out != nullptr) {
        *original_out = nullptr;
    }
    if (symbol_name == nullptr || replacement == nullptr) {
        port_log("[mods] InstallHook: bad args (sym=%p repl=%p)\n",
                 (const void*)symbol_name, replacement);
        return 1;
    }

    std::lock_guard<std::mutex> lock(sMutex);
    if (!sInitialized) {
        port_log("[mods] InstallHook(%s): not initialized\n", symbol_name);
        return 1;
    }

    void *target = SymbolResolver::Resolve(symbol_name);
    if (target == nullptr) {
        port_log("[mods] InstallHook(%s): symbol not found in debug info\n", symbol_name);
        return 1;
    }

    auto chain_it = sChains.find(target);
    if (chain_it == sChains.end()) {
        /* First hook on this target — standard MinHook setup, plus
         * an inner_thunk that the first installer will use as its
         * trampoline (so we can re-route it on subsequent installs). */
        void *mh_trampoline = nullptr;
        MH_STATUS s = MH_CreateHook(target, replacement, &mh_trampoline);
        if (s != MH_OK) {
            port_log("[mods] InstallHook(%s): MH_CreateHook failed: %s\n",
                     symbol_name, MhStatusName(s));
            return 1;
        }
        s = MH_EnableHook(target);
        if (s != MH_OK) {
            port_log("[mods] InstallHook(%s): MH_EnableHook failed: %s\n",
                     symbol_name, MhStatusName(s));
            MH_RemoveHook(target);
            return 1;
        }
        void *inner_thunk = AllocThunk(mh_trampoline);
        if (inner_thunk == nullptr) {
            port_log("[mods] InstallHook(%s): inner thunk alloc failed\n", symbol_name);
            MH_RemoveHook(target);
            return 1;
        }
        ChainState st;
        st.mh_trampoline = mh_trampoline;
        st.inner_thunk   = inner_thunk;
        st.chain.push_back(replacement);
        st.chain_owners.push_back(sCurrentOwner);
        st.chain_trampolines.push_back(inner_thunk);
        sChains[target] = st;

        sHooks.push_back({std::string(symbol_name), target, sCurrentOwner});
        if (original_out != nullptr) {
            *original_out = inner_thunk;   /* mod calls this → MinHook trampoline → original */
        }
        port_log("[mods] hooked %s @ %p (trampoline=%p, chain=1, owner=%s)\n",
                 symbol_name, target, inner_thunk,
                 sCurrentOwner.empty() ? "(unowned)" : sCurrentOwner.c_str());
        return 0;
    }

    /* Subsequent hook on a target that's already detoured. Re-create
     * the MinHook hook so the outer-most replacement is the new mod's
     * function. The new mod's trampoline points at the previous
     * outermost replacement (so calling it walks down the chain).
     * The inner_thunk is updated to the new MinHook trampoline so
     * the bottom of the chain still reaches the original function. */
    ChainState &st = chain_it->second;
    void *prev_outer = st.chain.back();

    MH_STATUS s = MH_RemoveHook(target);
    if (s != MH_OK) {
        port_log("[mods] InstallHook(%s): MH_RemoveHook failed: %s\n",
                 symbol_name, MhStatusName(s));
        return 1;
    }
    void *new_mh_trampoline = nullptr;
    s = MH_CreateHook(target, replacement, &new_mh_trampoline);
    if (s != MH_OK) {
        port_log("[mods] InstallHook(%s): re-create MH_CreateHook failed: %s\n",
                 symbol_name, MhStatusName(s));
        sChains.erase(chain_it);
        return 1;
    }
    s = MH_EnableHook(target);
    if (s != MH_OK) {
        port_log("[mods] InstallHook(%s): re-enable MH_EnableHook failed: %s\n",
                 symbol_name, MhStatusName(s));
        MH_RemoveHook(target);
        sChains.erase(chain_it);
        return 1;
    }
    /* Reflow the inner thunk to the fresh MinHook trampoline. */
    SetThunkTarget(st.inner_thunk, new_mh_trampoline);
    st.mh_trampoline = new_mh_trampoline;

    /* The new mod's trampoline = a thunk that jumps to the previous
     * outer replacement (which has its own trampoline pointing inward
     * down the chain). */
    void *new_trampoline = AllocThunk(prev_outer);
    if (new_trampoline == nullptr) {
        port_log("[mods] InstallHook(%s): chain thunk alloc failed\n", symbol_name);
        return 1;
    }
    st.chain.push_back(replacement);
    st.chain_owners.push_back(sCurrentOwner);
    st.chain_trampolines.push_back(new_trampoline);
    sHooks.push_back({std::string(symbol_name), target, sCurrentOwner});
    if (original_out != nullptr) {
        *original_out = new_trampoline;
    }
    port_log("[mods] chained %s @ %p (trampoline=%p, chain=%zu, owner=%s)\n",
             symbol_name, target, new_trampoline, st.chain.size(),
             sCurrentOwner.empty() ? "(unowned)" : sCurrentOwner.c_str());
    return 0;
}

void HookManager::SetCurrentOwner(const char *owner_id)
{
    std::lock_guard<std::mutex> lock(sMutex);
    sCurrentOwner = (owner_id != nullptr) ? owner_id : "";
}

void HookManager::ClearCurrentOwner()
{
    std::lock_guard<std::mutex> lock(sMutex);
    sCurrentOwner.clear();
}

int HookManager::UninstallHooksForOwner(const char *owner_id)
{
    if (owner_id == nullptr) {
        return 0;
    }
    std::lock_guard<std::mutex> lock(sMutex);
    if (!sInitialized) {
        return 0;
    }
    std::string oid(owner_id);
    int removed = 0;

    /* Walk every chain top-down, removing all entries owned by oid.
     * Three cases per entry:
     *   - is top + chain.size==1: remove last entry, MH_RemoveHook,
     *     free inner_thunk, the chain disappears.
     *   - is top + chain.size>1:  re-point MinHook at chain[i-1].
     *   - is mid-chain:           re-point chain[i+1]'s trampoline
     *     thunk to skip past us. If we're the bottom (i==0), the
     *     entry above us inherits inner_thunk's role. */
    for (auto chain_it = sChains.begin(); chain_it != sChains.end();) {
        ChainState &st = chain_it->second;

        /* Iterate top-down. After each removal vector indices shift,
         * so we restart the inner walk. Cheap — chains stay short. */
        bool any_removed = true;
        while (any_removed) {
            any_removed = false;
            for (int i = (int)st.chain.size() - 1; i >= 0; --i) {
                if (st.chain_owners[i] != oid) {
                    continue;
                }
                void *removed_tramp = st.chain_trampolines[i];
                bool is_top    = (i == (int)st.chain.size() - 1);
                bool is_bottom = (i == 0);

                if (is_top && st.chain.size() == 1) {
                    MH_RemoveHook(chain_it->first);
                    if (st.inner_thunk != nullptr) {
                        VirtualFree(st.inner_thunk, 0, MEM_RELEASE);
                        st.inner_thunk = nullptr;
                    }
                } else if (is_top) {
                    void *new_top = st.chain[i - 1];
                    MH_RemoveHook(chain_it->first);
                    void *new_mh_tramp = nullptr;
                    MH_STATUS s = MH_CreateHook(chain_it->first, new_top, &new_mh_tramp);
                    if (s == MH_OK) {
                        MH_EnableHook(chain_it->first);
                        SetThunkTarget(st.inner_thunk, new_mh_tramp);
                        st.mh_trampoline = new_mh_tramp;
                    } else {
                        port_log("[mods] uninstall %p (owner=%s): MH_CreateHook failed (%d) — chain broken\n",
                                 chain_it->first, oid.c_str(), (int)s);
                    }
                    if (removed_tramp != st.inner_thunk && removed_tramp != nullptr) {
                        VirtualFree(removed_tramp, 0, MEM_RELEASE);
                    }
                } else {
                    /* Mid-chain: re-point chain[i+1]'s trampoline thunk
                     * to skip our entry. If we're the bottom, the
                     * upstairs thunk now points at mh_trampoline (the
                     * MinHook trampoline that runs original engine
                     * code) and itself becomes the new inner_thunk. */
                    void *new_target = is_bottom ? st.mh_trampoline : st.chain[i - 1];
                    SetThunkTarget(st.chain_trampolines[i + 1], new_target);

                    if (is_bottom) {
                        if (st.inner_thunk != nullptr) {
                            VirtualFree(st.inner_thunk, 0, MEM_RELEASE);
                        }
                        st.inner_thunk = st.chain_trampolines[i + 1];
                    } else {
                        if (removed_tramp != st.inner_thunk && removed_tramp != nullptr) {
                            VirtualFree(removed_tramp, 0, MEM_RELEASE);
                        }
                    }
                }

                st.chain.erase(st.chain.begin() + i);
                st.chain_owners.erase(st.chain_owners.begin() + i);
                st.chain_trampolines.erase(st.chain_trampolines.begin() + i);

                port_log("[mods] uninstalled %p[%d] (owner=%s, %s, chain=%zu)\n",
                         chain_it->first, i, oid.c_str(),
                         is_top ? (st.chain.empty() ? "chain emptied" : "was top")
                                : (is_bottom ? "was bottom (mid-chain skip)" : "mid-chain"),
                         st.chain.size());
                removed++;
                any_removed = true;
                break; /* restart after vector mutation */
            }
        }

        if (st.chain.empty()) {
            chain_it = sChains.erase(chain_it);
        } else {
            ++chain_it;
        }
    }

    /* Sweep the diagnostic sHooks list. */
    sHooks.erase(
        std::remove_if(sHooks.begin(), sHooks.end(),
            [&oid](const HookRecord &h) { return h.owner_id == oid; }),
        sHooks.end());

    return removed;
}

} // namespace ssb64::mods
