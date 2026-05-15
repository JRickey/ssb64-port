#pragma once

/* C-ABI bridge between libultraship's gfx_register_hires_hook() and the
 * port-side HiResPack singleton. Implementation lives in HiResHook.cpp;
 * registration happens once during PortInit. */

#ifdef __cplusplus
extern "C" {
#endif

/* Wire the hook into libultraship. Idempotent; subsequent calls overwrite
 * the registered callback (last-call-wins). Safe to call any time after
 * Ship::Context is alive — the hook itself is not invoked until the first
 * texture cache miss after a frame begins. */
void ssb64_hires_register(void);

#ifdef __cplusplus
}
#endif
