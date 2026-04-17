/**
 * port_aobj_fixup.cpp — see port_aobj_fixup.h for architecture notes.
 *
 * The opcode size table below mirrors the decomp parser's switch in
 * src/sys/objanim.c — gcParseDObjAnimJoint (opcodes 0-17) and
 * gcParseMObjMatAnimJoint (opcodes 0-14, 18-22).  Extending the parser
 * with a new opcode means extending this walker too.
 */

#include "port_aobj_fixup.h"
#include "port_log.h"
#include "resource/RelocPointerTable.h"

#include <cstdint>
#include <unordered_set>
#include <vector>

namespace {

std::unordered_set<uintptr_t> sUnswappedHeads;
std::unordered_set<uintptr_t> sRejectedHeads;  /* walked once, didn't look like EVENT32 — don't retry */

/* Registered fighter-figatree file ranges (base, end).  The walker only
 * operates on head pointers that fall within one of these — this guards
 * against a stream from a non-halfswapped file being un-halfswapped,
 * which would silently corrupt it. */
struct Range { uintptr_t base; uintptr_t end; };
std::vector<Range> sHalfswappedRanges;

bool is_in_halfswapped_range(const void *p) {
    uintptr_t addr = reinterpret_cast<uintptr_t>(p);
    for (const auto &r : sHalfswappedRanges) {
        if (addr >= r.base && addr < r.end) return true;
    }
    return false;
}

/* Safety cap per stream: long-form figatree animations rarely exceed a
 * few hundred events.  65k u32s = 256 KB of stream data — far beyond
 * any realistic authored animation and well short of "walk forever". */
constexpr int kStreamStepLimit = 65536;

inline uint32_t unhalfswap(uint32_t v) {
    return (v << 16) | (v >> 16);
}

inline uint32_t cmd_opcode(uint32_t cmd) {
    /* AObjEvent32.command layout matches src/sys/objtypes.h LE variant:
     * payload:15 (bits 0..14), flags:10 (bits 15..24), opcode:7 (25..31). */
    return (cmd >> 25) & 0x7F;
}

inline uint32_t cmd_flags(uint32_t cmd) {
    return (cmd >> 15) & 0x3FF;
}

inline int popcount_bits(uint32_t v, int first_bit, int count) {
    int c = 0;
    for (int i = 0; i < count; i++) {
        if (v & (1u << (first_bit + i))) c++;
    }
    return c;
}

/* Un-halfswap a data u32 in place and advance. */
inline void unswap_data(uint32_t *&ev) {
    *ev = unhalfswap(*ev);
    ev++;
}

/* Advance past a token slot without modifying it.  Reloc chain wrote the
 * token index in native u32 byte order; portRelocFixupFighterFigatree
 * skipped that slot, so it is already correct. */
inline void skip_token(uint32_t *&ev) {
    ev++;
}

/* Walker uses a two-phase approach: first phase validates the stream by
 * simulating the walk without modifying data (it computes the
 * un-halfswapped u32 and reads the opcode from it but doesn't write
 * anywhere).  It records the set of u32 addresses that WOULD be
 * modified.  If the simulation completes — hits a proper terminator
 * (End, Jump, SetAnim) with only valid opcodes — the second phase
 * applies the halfswap fix to every collected address.  If the
 * simulation hits an unknown opcode or runs away, the stream is added
 * to sRejectedHeads and left untouched.
 *
 * This protects streams that aren't truly EVENT32 from silent
 * corruption: gcParseDObjAnimJoint gets called with some pointers
 * that point to EVENT16 data or non-figatree data (because the same
 * call site handles multiple animation types). */

struct ScanCtx {
    std::vector<uint32_t *> pending;   /* u32 addresses to un-halfswap */
    std::unordered_set<uintptr_t> visited;
    int total_steps;
};

bool scan(uint32_t *head, ScanCtx &ctx);

bool scan_recurse(uint32_t token, ScanCtx &ctx) {
    if (token == 0) return true;
    void *target = portRelocTryResolvePointer(token);
    if (target == nullptr) return true; /* bad token — ignore, let parser deal */
    return scan(static_cast<uint32_t *>(target), ctx);
}

bool scan(uint32_t *head, ScanCtx &ctx) {
    if (head == nullptr) return true;
    if (!is_in_halfswapped_range(head)) return true; /* not our data; skip silently */
    uintptr_t key = reinterpret_cast<uintptr_t>(head);
    if (sUnswappedHeads.count(key)) return true;
    if (sRejectedHeads.count(key)) return false;
    if (!ctx.visited.insert(key).second) return true; /* already scanning this head */

    uint32_t *ev = head;

    while (true) {
        if (++ctx.total_steps > kStreamStepLimit) {
            return false;
        }

        uint32_t cmd_raw = *ev;
        uint32_t cmd = unhalfswap(cmd_raw);
        /* Queue the command word for later un-halfswap. */
        ctx.pending.push_back(ev);
        ev++;

        uint32_t opcode = cmd_opcode(cmd);
        uint32_t flags_in_cmd = cmd_flags(cmd);

        switch (opcode) {
        case 0:  /* End */
            return true;

        case 1:  /* Jump — [token] */
        case 14: /* SetAnim — [token] */
            if (!scan_recurse(*ev, ctx)) return false;
            ev++;
            return true;

        case 2:  /* Wait */
        case 15: /* SetFlags */
        case 16: /* ANIM_CMD_16 */
            break;

        case 13: /* SetInterp — [token] (no recurse) */
            ev++;
            break;

        /* Flags come from the command word itself (cmd.flags) — the
         * decomp reads them via AObjAnimAdvance (post-increment) so
         * what looks like "advance past header to get flags from next"
         * is actually just reading the just-consumed command's flags.
         * All of the opcodes below consume the command plus zero or
         * more f32 payload slots. */

        case 3: case 4: /* SetValBlock, SetVal */
        case 7:         /* SetTargetRate */
        case 8: case 9: /* SetVal0RateBlock, SetVal0Rate */
        case 10: case 11: /* SetValAfterBlock, SetValAfter */
        case 17:        /* ANIM_CMD_17 */
        case 18: case 19: case 20: case 21: /* SetExtVal* (MObj) */
        {
            int n = popcount_bits(flags_in_cmd, 0, 10);
            if (n > 10) return false;
            for (int k = 0; k < n; k++) {
                ctx.pending.push_back(ev);
                ev++;
            }
            break;
        }

        case 5: case 6: /* SetValRate(Block) — value+rate pairs */
        {
            int n = popcount_bits(flags_in_cmd, 0, 10);
            if (n > 10) return false;
            for (int k = 0; k < 2 * n; k++) {
                ctx.pending.push_back(ev);
                ev++;
            }
            break;
        }

        case 12: /* ANIM_CMD_12 — no f values, flags-only effect */
            break;

        case 22: /* ANIM_CMD_22 (MObj) — N u32 from flag bits [0..4] */
        {
            int n = popcount_bits(flags_in_cmd, 0, 5);
            if (n > 5) return false;
            for (int k = 0; k < n; k++) {
                ctx.pending.push_back(ev);
                ev++;
            }
            break;
        }

        default:
            return false; /* invalid opcode — reject the whole stream */
        }
    }
}

void walk(uint32_t *head) {
    if (head == nullptr) return;
    if (!is_in_halfswapped_range(head)) return;
    uintptr_t key = reinterpret_cast<uintptr_t>(head);
    if (sUnswappedHeads.count(key)) return;
    if (sRejectedHeads.count(key)) return;

    ScanCtx ctx;
    ctx.total_steps = 0;
    if (scan(head, ctx)) {
        for (uint32_t *p : ctx.pending) {
            *p = unhalfswap(*p);
        }
        for (uintptr_t k : ctx.visited) {
            sUnswappedHeads.insert(k);
        }
    } else {
        sRejectedHeads.insert(key);
    }
}

} // namespace

extern "C" void port_aobj_event32_unhalfswap_stream(void *head) {
    walk(static_cast<uint32_t *>(head));
}

extern "C" void port_aobj_register_halfswapped_range(void *base, unsigned long size) {
    if (base == nullptr || size == 0) return;
    uintptr_t b = reinterpret_cast<uintptr_t>(base);
    sHalfswappedRanges.push_back({b, b + static_cast<uintptr_t>(size)});
}

extern "C" void port_aobj_event32_unhalfswap_reset(void) {
    sUnswappedHeads.clear();
    sRejectedHeads.clear();
    sHalfswappedRanges.clear();
}
