#pragma once

/**
 * coroutine.h — Platform-agnostic coroutine API for the SSB64 port.
 *
 * Provides cooperative multitasking to emulate N64 OS threads on PC.
 * Each N64 thread (scheduler, audio, controller, game logic) runs as
 * a coroutine that yields at blocking points (osRecvMesg with OS_MESG_BLOCK).
 * GObj thread processes also use coroutines for their yield/resume cycle.
 *
 * Platform backends:
 *   - Windows: Win32 Fibers (CreateFiber / SwitchToFiber)
 *   - POSIX:   ucontext_t  (makecontext / swapcontext)
 */

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct PortCoroutine PortCoroutine;

/**
 * Initialize the main thread for coroutine support.
 * On Windows, this converts the calling thread to a fiber.
 * Must be called once before any other coroutine functions.
 */
void port_coroutine_init_main(void);

/**
 * Create a new coroutine.
 *
 * @param entry      Entry point function. Called with @p arg.
 *                   When entry returns, the coroutine is marked finished
 *                   and control returns to whoever last resumed it.
 * @param arg        Argument passed to entry.
 * @param stack_size Stack size in bytes. Minimum 16384 (16 KB).
 * @return           Opaque coroutine handle, or NULL on failure.
 */
PortCoroutine *port_coroutine_create(void (*entry)(void *), void *arg, size_t stack_size);

/**
 * Destroy a coroutine and free its resources.
 * Must not be called on a currently executing coroutine.
 */
void port_coroutine_destroy(PortCoroutine *co);

/**
 * Resume a coroutine from the main thread (or from another coroutine).
 * Execution transfers to the coroutine until it calls port_coroutine_yield()
 * or its entry function returns. Then control returns here.
 */
void port_coroutine_resume(PortCoroutine *co);

/**
 * Yield from the currently executing coroutine back to whoever resumed it.
 * Must only be called from within a coroutine (not the main thread).
 */
void port_coroutine_yield(void);

/**
 * Returns non-zero if the coroutine's entry function has returned.
 */
int port_coroutine_is_finished(PortCoroutine *co);

/**
 * Returns non-zero if the calling code is currently inside a coroutine
 * (as opposed to the main thread).
 */
int port_coroutine_in_coroutine(void);

#ifdef __cplusplus
}
#endif
