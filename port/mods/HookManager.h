/**
 * HookManager.h — Runtime function detouring for mods.
 *
 * Wraps MinHook (Windows) behind a clean interface. Each call to
 * InstallHook resolves the symbol via SymbolResolver, registers a
 * MinHook detour with the resolved address as the target, enables it,
 * and writes the trampoline pointer to *original_out so the mod can
 * delegate to the original function.
 *
 * Per-mod ownership: each install records the current owner (set via
 * SetCurrentOwner before MOD_INIT runs). UninstallHooksForOwner walks
 * those records and tears them down. Three removal cases are handled:
 *   - top of chain, chain.size == 1 → MH_RemoveHook, free inner_thunk
 *   - top of chain, chain.size  > 1 → re-point MinHook at chain[i-1]
 *   - mid-chain                     → re-stitch chain[i+1]'s trampoline
 *                                      thunk to skip past us
 *
 * Mid-chain re-stitch means a single mod can be hot-reloaded even
 * when another mod is stacked on top of one of its hooks — the
 * upper mod stays installed and its calls to "original" now bypass
 * the removed mod's replacement and reach the engine code directly.
 *
 * On engine shutdown, Shutdown() disables and uninstalls all hooks
 * at once regardless of owner.
 */
#pragma once

#include <vector>
#include <mutex>
#include <string>
#include <map>

namespace ssb64::mods {

class HookManager
{
public:
    /* Initialize MinHook. Must be called once at startup. */
    static bool Init();

    /* Uninstall all hooks and tear down MinHook. */
    static void Shutdown();

    /* Resolve `symbol_name` via SymbolResolver, install a detour to
     * `replacement` at the resolved address, write the trampoline
     * function pointer to *original_out.
     *
     * Returns 0 on success. On failure: non-zero, *original_out is
     * set to nullptr, and the engine logs the cause. */
    static int InstallHook(const char *symbol_name,
                           void       *replacement,
                           void      **original_out);

    /* Set the current "owner" for InstallHook calls. Each subsequent
     * install records this as its owner_id until ClearCurrentOwner.
     * ScriptLoader's LoadAll wraps each ModInit with these calls. */
    static void SetCurrentOwner(const char *owner_id);
    static void ClearCurrentOwner();

    /* Walk every installed hook owned by `owner_id`, remove from
     * MinHook, free trampoline thunks, sweep sHooks. Returns the
     * number of hooks removed. Mid-chain entries (where another
     * mod's hook is on top) are left in place and a warning is
     * logged — hot-reload of just that one mod isn't safe; the
     * normal UnloadAll-then-LoadAll cycle removes top-to-bottom and
     * never trips this. Safe to call BEFORE FreeLibrary'ing the
     * owner's DLL. */
    static int UninstallHooksForOwner(const char *owner_id);

private:
    struct HookRecord {
        std::string symbol_name;
        void       *target;
        std::string owner_id;
    };

    /* Per-symbol chain state for stacking multiple hooks on the same
     * function. Three parallel vectors track each chain entry:
     *
     *   chain[i]              — replacement function pointer
     *   chain_owners[i]       — owner_id at install time
     *   chain_trampolines[i]  — the thunk the mod stashed as `original`.
     *                           For i=0 this == inner_thunk (shared).
     *                           For i>=1 it's an AllocThunk pointing
     *                           at chain[i-1] (so chain[i]'s "original
     *                           fn" walks down the chain). Used during
     *                           uninstall to free the per-entry thunk. */
    struct ChainState {
        void *mh_trampoline;
        void *inner_thunk;
        std::vector<void *>       chain;
        std::vector<std::string>  chain_owners;
        std::vector<void *>       chain_trampolines;
    };

    static std::mutex sMutex;
    static std::vector<HookRecord> sHooks;
    static std::map<void *, ChainState> sChains;
    static std::string sCurrentOwner;
    static bool sInitialized;

    /* Allocate an executable thunk that does `jmp [ptr]` where ptr is
     * stored inline. Returns the thunk address; the caller updates it
     * by writing to ThunkPointerSlot(thunk). */
    static void *AllocThunk(void *initial_target);
    static void  SetThunkTarget(void *thunk, void *new_target);
};

} // namespace ssb64::mods
