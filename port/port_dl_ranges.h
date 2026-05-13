#ifndef PORT_DL_RANGES_H
#define PORT_DL_RANGES_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* DL-source range registry for the GFX walker's bounds check.
 *
 * Background: gfx_step walks display lists one F3DGfx at a time. When a
 * dynamically-built game DL (segment-0xE stride-corrected path in
 * gfx_dl_handler_common) lacks a gsSPEndDisplayList terminator, the walker
 * advances past the DL's intended end. Since taskman.c memsets the scene
 * arena to zero between scenes, the post-DL bytes are G_SPNOOP (opcode 0)
 * which silently advances cmd. The walker eventually steps off the end of
 * the arena into unmapped memory and SIGSEGVs.
 *
 * The registry tracks valid DL memory ranges so gfx_step can detect when
 * cmd has walked outside any registered range and terminate the frame
 * safely (as if G_ENDDL had been encountered).
 *
 * Three subsystems register their ranges:
 *   - taskman.c       — the 16 MiB scene arena (once on alloc)
 *   - lbreloc_bridge  — each reloc file range (on load / unload)
 *   - interpreter.cpp — libultraship's widened-DL cache (on emplace / evict)
 *
 * The check is conservative: an unregistered address is treated as
 * "unknown" rather than "invalid", so missing registrations don't break
 * valid DLs. Only the specific "walked past a registered range" case
 * triggers rejection. */

/* Classification returned by port_dl_check_addr. */
enum PortDLAddrClass {
    PORT_DL_UNKNOWN = 0,             /* Not in any registered range; let through. */
    PORT_DL_IN_RANGE,                /* Confirmed valid. */
    PORT_DL_WALKED_PAST,             /* Just past a registered range — runaway walker. */
};

/* Register a DL memory range. Label must be a string literal (lifetime ≥
 * process). Idempotent: re-registering the same base updates size/label. */
void port_dl_range_register(const void *base, size_t size, const char *label);

/* Unregister by exact base. No-op if not found. */
void port_dl_range_unregister(const void *base);

/* Classify addr against the registry. Hot path: called from gfx_step
 * before every deref. Should be O(N) for small N (~50 ranges). */
int port_dl_check_addr(uintptr_t addr);

/* Diagnostic-only: write a human-readable classification (label+offset)
 * into buf. Returns true if addr was in a registered range. */
int port_dl_range_classify_str(uintptr_t addr, char *buf, size_t buf_size);

#ifdef __cplusplus
}
#endif

#endif /* PORT_DL_RANGES_H */
