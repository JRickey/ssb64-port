#include "RelocPointerTable.h"

#include <cstdlib>
#include <cstring>
#include <vector>
#include <spdlog/spdlog.h>

/**
 * Per-slot generational handle table mapping 32-bit token ↔ 64-bit pointer.
 *
 * Token layout: [12 bits slot-generation][20 bits slot-index]
 *   - Slot generations are PER-SLOT, not global. Each slot starts at gen 1
 *     and bumps on every release (reuse). A token resolves only if its
 *     embedded gen matches the slot's current gen — stale tokens (whose
 *     slot has been reused) fail decode and resolve to NULL.
 *   - 1M indices, 4096 generations per slot — effectively infinite reuse
 *     headroom in any realistic session.
 *
 * Why per-slot instead of global gen:
 *   Earlier design bumped a single global gen on every scene reset and
 *   memset'd the entire table. That invalidated tokens for files in the
 *   intern buffer (mainmotion, submotion, model, special1-4, shieldpose)
 *   which legitimately persist across scenes — their underlying memory is
 *   still valid, but the tokens encoding it had a stale gen. Downstream
 *   resolvers returned NULL, downstream consumers (gcSetupCustomDObjs,
 *   ftMainSetStatus joint init, etc.) didn't always NULL-check, and
 *   crashed. With per-slot gens we never artificially invalidate a slot
 *   whose memory is still live; portRelocInvalidateRange() selectively
 *   bumps only those slots whose backing pointer falls in a recycled
 *   memory range (scene arena, freed reloc file, etc.).
 *
 * Recovery: invalidated slots go on a free list; subsequent registrations
 * pull from it before extending sNextIndex. The table doesn't grow without
 * bound across long sessions.
 */

namespace {

struct Slot {
    void *ptr;
    uint32_t gen;        /* 0 = unregistered (token gen=0 always invalid).
                          * 1..GEN_MAX = registered; reuse bumps. */
};

constexpr uint32_t TOKEN_GENERATION_SHIFT = 20;
constexpr uint32_t TOKEN_INDEX_MASK       = 0x000FFFFFu;       /* 20 bits = 1M-1 */
constexpr uint32_t TOKEN_GENERATION_MAX   = 0xFFFu;            /* 12 bits */
constexpr uint32_t INITIAL_CAPACITY       = 256 * 1024;        /* ~4 MB initial. */

static Slot     *sSlots       = nullptr;
static uint32_t  sNextIndex   = 1;                              /* Index 0 reserved. */
static uint32_t  sCapacity    = 0;

/* Free list of indices whose slots were invalidated (memset to NULL via
 * portRelocInvalidateRange). Reused FIFO-ish so that recently-freed
 * tokens don't collide with brand-new ones in test scenarios. Vector is
 * a small overhead per slot but the simplicity is worth it. */
static std::vector<uint32_t> sFreeIndices;

static void ensureCapacity(void)
{
    if (sSlots == nullptr) {
        sCapacity = INITIAL_CAPACITY;
        sSlots = (Slot *)calloc(sCapacity, sizeof(Slot));
        return;
    }
    if (sNextIndex >= sCapacity) {
        uint32_t newCapacity = sCapacity * 2;
        if (newCapacity > TOKEN_INDEX_MASK) {
            spdlog::error("RelocPointerTable: token index capacity exhausted");
            abort();
        }
        spdlog::info("RelocPointerTable: growing {} -> {} entries",
                     sCapacity, newCapacity);
        sSlots = (Slot *)realloc(sSlots, newCapacity * sizeof(Slot));
        memset(sSlots + sCapacity, 0, (newCapacity - sCapacity) * sizeof(Slot));
        sCapacity = newCapacity;
    }
}

static uint32_t bumpSlotGeneration(uint32_t gen)
{
    /* gen 0 reserved for "unregistered". Bump to 1 on first register;
     * cycle 1..TOKEN_GENERATION_MAX with wrap back to 1. */
    if (gen == 0) return 1;
    if (gen >= TOKEN_GENERATION_MAX) return 1;
    return gen + 1;
}

static uint32_t makeToken(uint32_t index, uint32_t gen)
{
    return (gen << TOKEN_GENERATION_SHIFT) | (index & TOKEN_INDEX_MASK);
}

static bool decodeToken(uint32_t token, uint32_t *outIndex)
{
    uint32_t tokenGen   = token >> TOKEN_GENERATION_SHIFT;
    uint32_t tokenIndex = token & TOKEN_INDEX_MASK;
    if (token == 0 || tokenGen == 0 || tokenIndex == 0 || tokenIndex >= sNextIndex) {
        return false;
    }
    if (sSlots == nullptr) {
        return false;
    }
    if (sSlots[tokenIndex].gen != tokenGen) {
        return false;
    }
    *outIndex = tokenIndex;
    return true;
}

} /* namespace */

extern "C" {

uint32_t portRelocRegisterPointer(void *ptr)
{
    if (ptr == nullptr) {
        return 0;
    }
    ensureCapacity();

    uint32_t index;
    uint32_t gen;
    if (!sFreeIndices.empty()) {
        /* Reuse a previously-invalidated slot. The slot's gen has already
         * been bumped on invalidation; we just set the ptr. */
        index = sFreeIndices.back();
        sFreeIndices.pop_back();
        if (sSlots[index].gen == 0) {
            /* Shouldn't happen — invalidation bumps gen — but recover. */
            sSlots[index].gen = 1;
        }
        gen = sSlots[index].gen;
    } else {
        index = sNextIndex++;
        ensureCapacity();
        gen = bumpSlotGeneration(sSlots[index].gen);
        sSlots[index].gen = gen;
    }
    sSlots[index].ptr = ptr;
    return makeToken(index, gen);
}

void *portRelocResolvePointer(uint32_t token)
{
    return portRelocResolvePointerDebug(token, nullptr, 0);
}

/* Diagnostic counters — periodically dumped to spdlog so we can verify
 * the per-slot table is actually resolving valid tokens (not silently
 * NULLing everything, which would explain a "no vertices rendered" no-
 * crash regression). Reset every gPortDebugLogPeriod resolves. */
static uint64_t sResolveHits   = 0;
static uint64_t sResolveMisses = 0;
static uint64_t sResolveZero   = 0;
constexpr uint64_t kResolveLogPeriod = 100000;

void *portRelocResolvePointerDebug(uint32_t token, const char *file, int line)
{
    if (token == 0) {
        sResolveZero++;
        return nullptr;
    }
    uint32_t index = 0;
    if (!decodeToken(token, &index)) {
        sResolveMisses++;
        uint32_t tokenGen   = token >> TOKEN_GENERATION_SHIFT;
        uint32_t tokenIndex = token & TOKEN_INDEX_MASK;
        uint32_t slotGen    = (sSlots && tokenIndex < sNextIndex) ? sSlots[tokenIndex].gen : 0;
        if (file != nullptr) {
            spdlog::error("RelocPointerTable: invalid/stale token 0x{:08X} "
                          "(token_gen=0x{:03X} slot_gen=0x{:03X} index={} max={}, caller={}:{})",
                          token, tokenGen, slotGen, tokenIndex, sNextIndex - 1, file, line);
        } else {
            spdlog::error("RelocPointerTable: invalid/stale token 0x{:08X} "
                          "(token_gen=0x{:03X} slot_gen=0x{:03X} index={} max={})",
                          token, tokenGen, slotGen, tokenIndex, sNextIndex - 1);
        }
        return nullptr;
    }
    sResolveHits++;
    /* Periodic diag dump: ratio of hits / misses confirms the table is
     * actually serving valid resolves vs silently NULLing everything. */
    if (((sResolveHits + sResolveMisses) % kResolveLogPeriod) == 0) {
        spdlog::info("RelocPointerTable: resolves so far hits={} misses={} zero={} (next_index={} free={})",
                     sResolveHits, sResolveMisses, sResolveZero, sNextIndex,
                     sFreeIndices.size());
    }
    return sSlots[index].ptr;
}

void *portRelocTryResolvePointer(uint32_t token)
{
    uint32_t index = 0;
    if (!decodeToken(token, &index)) {
        return nullptr;
    }
    return sSlots[index].ptr;
}

/**
 * Selectively invalidate slots whose pointer falls in [base, base+size).
 * Each invalidated slot has its ptr cleared and gen bumped; the slot
 * index is pushed onto the free list for reuse.
 *
 * Called from port_taskman_evict_arena_caches with the scene arena range
 * — tokens for arena-backed data become stale, tokens for intern-buffer
 * data (which persists across scenes) remain valid.
 */
void portRelocInvalidateRange(const void *base, size_t size)
{
    if (sSlots == nullptr || base == nullptr || size == 0) {
        return;
    }
    uintptr_t lo = reinterpret_cast<uintptr_t>(base);
    uintptr_t hi = lo + size;
    size_t invalidated = 0;
    for (uint32_t i = 1; i < sNextIndex; ++i) {
        if (sSlots[i].gen == 0) continue;       /* already free */
        uintptr_t p = reinterpret_cast<uintptr_t>(sSlots[i].ptr);
        if (p >= lo && p < hi) {
            sSlots[i].ptr = nullptr;
            sSlots[i].gen = bumpSlotGeneration(sSlots[i].gen);
            sFreeIndices.push_back(i);
            invalidated++;
        }
    }
    if (invalidated > 0) {
        spdlog::info("RelocPointerTable: invalidated {} slots in range "
                     "[{:p}, {:p}) — {} on free list",
                     invalidated, base, (void *)hi, sFreeIndices.size());
    }
}

/**
 * Legacy whole-table reset. Now a no-op for the common scene-reset path
 * because invalidation is range-based via portRelocInvalidateRange().
 * Kept as a public API for diagnostic or test paths that want a hard
 * reset (e.g. switching ROMs); when called, it clears all slots and
 * resets the free list.
 *
 * Existing callers in lbRelocInitSetup() and friends used to call this
 * on every scene init. With the per-slot model, that wholesale reset
 * is the source of the variant-1/2/3 crash family (it invalidated
 * tokens for files still loaded in the intern buffer). The fix is to
 * stop calling this from scene-init paths; the range-based invalidator
 * does the right thing for scene-arena recycling, and persistent
 * intern-buffer file tokens stay valid.
 */
void portRelocResetPointerTable(void)
{
    if (sSlots != nullptr) {
        memset(sSlots, 0, sCapacity * sizeof(Slot));
    }
    sNextIndex = 1;
    sFreeIndices.clear();
}

} /* extern "C" */
