/**
 * port_dl_ranges.cpp — registry of valid DL memory ranges for the GFX
 * walker's bounds check. See port_dl_ranges.h for the rationale.
 *
 * Wired into libultraship at startup via port_dl_ranges_init(), which
 * registers our port-side bounds-check and address classifier as the
 * callbacks libultraship's Fast::Interpreter uses to validate DL
 * pointers and label diagnostic dumps. libultraship has no compile-time
 * dependency on this file — both callbacks are optional.
 */

#include "port_dl_ranges.h"
#include "fast/interpreter.h"

#include <atomic>
#include <cstdio>
#include <cstring>
#include <mutex>
#include <vector>

namespace {

struct Entry {
    uintptr_t base;
    size_t size;
    const char *label;
};

/* Small registry (< 100 ranges typical). Registration/unregistration
 * happen only at file load/unload (rare); port_dl_check_addr is called
 * once per GBI command from gfx_step (extremely hot — tens of thousands
 * of calls per frame in a fight). The writer-side vector is guarded by
 * sRangesMtx, but the reader hot path must NOT take that mutex or do a
 * linear scan per command, or busy scenes (fights) tank to ~30 fps while
 * light scenes (menus) stay fine.
 *
 * Hot-path strategy: the GFX walker advances cmd linearly within one DL,
 * so consecutive calls almost always land in the same range. A
 * thread-local single-entry cache of the last in-range hit answers the
 * common case with two comparisons and one relaxed atomic load — no
 * mutex, no scan. sRangesGen is bumped on every mutating register/
 * unregister so a stale cached range can never mask a registry change;
 * on a generation mismatch or cache miss the call falls through to the
 * original locked scan, preserving exact classification behavior. */
std::vector<Entry> sRanges;
std::mutex sRangesMtx;
std::atomic<uint64_t> sRangesGen{0};

struct HitCache {
    uint64_t gen = ~0ull;   /* generation this cache entry was validated at */
    uintptr_t base = 0;
    size_t size = 0;
};
thread_local HitCache sHitCache;

/* "Walked past" threshold: how far past a registered range do we still
 * recognise as runaway-from-that-range? One 64 KiB window comfortably
 * covers the gap to the next mmap'd allocation while staying tight
 * enough to fire on the actual walk-off cases. */
constexpr size_t kWalkPastWindow = 0x10000;

} /* namespace */

extern "C" void port_dl_range_register(const void *base, size_t size, const char *label) {
    if ((base == nullptr) || (size == 0)) {
        return;
    }
    uintptr_t b = reinterpret_cast<uintptr_t>(base);
    std::lock_guard<std::mutex> lk(sRangesMtx);
    for (auto &e : sRanges) {
        if (e.base == b) {
            if (e.size != size || e.label != label) {
                e.size = size;
                e.label = label;
                sRangesGen.fetch_add(1, std::memory_order_release);
            }
            return;
        }
    }
    sRanges.push_back({b, size, label});
    sRangesGen.fetch_add(1, std::memory_order_release);
}

extern "C" void port_dl_range_unregister(const void *base) {
    if (base == nullptr) return;
    uintptr_t b = reinterpret_cast<uintptr_t>(base);
    std::lock_guard<std::mutex> lk(sRangesMtx);
    for (auto it = sRanges.begin(); it != sRanges.end(); ++it) {
        if (it->base == b) {
            sRanges.erase(it);
            sRangesGen.fetch_add(1, std::memory_order_release);
            return;
        }
    }
}

extern "C" int port_dl_check_addr(uintptr_t addr) {
    if (addr == 0) return PORT_DL_UNKNOWN;

    /* Hot path: per-GBI-command. The walker advances cmd linearly, so the
     * previous in-range hit almost always still covers addr. Answer that
     * case with no mutex and no scan. Only valid while the registry hasn't
     * changed (generation match); any register/unregister bumps the gen
     * and forces a re-validation through the locked scan below. */
    const uint64_t gen = sRangesGen.load(std::memory_order_acquire);
    {
        const HitCache c = sHitCache;
        if (c.gen == gen && c.size != 0 &&
            (addr >= c.base) && ((addr - c.base) < c.size)) {
            return PORT_DL_IN_RANGE;
        }
    }

    std::lock_guard<std::mutex> lk(sRangesMtx);
    bool walked_past = false;
    for (const auto &e : sRanges) {
        if ((addr >= e.base) && ((addr - e.base) < e.size)) {
            sHitCache = HitCache{gen, e.base, e.size};
            return PORT_DL_IN_RANGE;
        }
        if ((addr >= e.base + e.size) && ((addr - (e.base + e.size)) < kWalkPastWindow)) {
            walked_past = true;
            /* Don't return immediately — another registered range further
             * along could legitimately contain addr (overlapping ranges
             * are allowed at registration). Only commit to WALKED_PAST
             * after confirming addr isn't in any other range. */
        }
    }
    return walked_past ? PORT_DL_WALKED_PAST : PORT_DL_UNKNOWN;
}

extern "C" int port_dl_range_classify_str(uintptr_t addr, char *buf, size_t buf_size) {
    if (buf == nullptr || buf_size == 0) return 0;
    if (addr == 0) {
        std::snprintf(buf, buf_size, "null");
        return 0;
    }
    std::lock_guard<std::mutex> lk(sRangesMtx);
    for (const auto &e : sRanges) {
        if ((addr >= e.base) && ((addr - e.base) < e.size)) {
            std::snprintf(buf, buf_size, "%s+0x%lx",
                          e.label ? e.label : "?",
                          static_cast<unsigned long>(addr - e.base));
            return 1;
        }
    }
    /* Fall through to a heuristic label so the diag still tells us
     * something useful for unregistered ranges. */
    if (addr <= 0x0FFFFFFFu) {
        std::snprintf(buf, buf_size, "n64_seg[%u]+0x%x",
                      static_cast<unsigned>((addr >> 24) & 0xFF),
                      static_cast<unsigned>(addr & 0x00FFFFFFu));
    } else if (addr < 0x100000000ull) {
        std::snprintf(buf, buf_size, "low_brk@0x%lx", static_cast<unsigned long>(addr));
    } else {
        std::snprintf(buf, buf_size, "other@0x%lx", static_cast<unsigned long>(addr));
    }
    return 0;
}

/* Called once at port startup to wire our bounds-check and classifier
 * into libultraship's GFX walker and diag dump. libultraship has zero
 * knowledge of port_dl_* symbols — only the function pointers it
 * receives from these registrations. */
extern "C" void port_dl_ranges_init(void) {
    Fast::RegisterDLBoundsCheck(&port_dl_check_addr);
    Fast::RegisterAddressClassifier(&port_dl_range_classify_str);
}
