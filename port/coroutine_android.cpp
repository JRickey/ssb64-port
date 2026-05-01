/**
 * coroutine_android.cpp — Spike-only stub for Android.
 *
 * Android's bionic libc removed getcontext/makecontext/swapcontext, so the
 * POSIX ucontext path in coroutine_posix.cpp doesn't compile. This file is
 * a placeholder that lets the rest of the codebase link; any call into the
 * coroutine API at runtime will abort with a clear message.
 *
 * A real Android impl needs either:
 *   - pthread + condvar (each coroutine = pthread, ping-pong semaphores), or
 *   - aarch64 swapcontext written in ~30 lines of assembly (boost::context-style).
 */

#if defined(__ANDROID__)

#include "coroutine.h"
#include <stdio.h>
#include <stdlib.h>

struct PortCoroutine { int dummy; };

static void android_coroutine_unimpl(const char *fn) {
    fprintf(stderr, "SSB64 Android: %s called but coroutine impl is a spike stub\n", fn);
    abort();
}

void port_coroutine_init_main(void) { /* no-op */ }

PortCoroutine *port_coroutine_create(void (*)(void *), void *, size_t) {
    android_coroutine_unimpl(__func__);
    return nullptr;
}

void port_coroutine_destroy(PortCoroutine *) { /* no-op */ }

void port_coroutine_resume(PortCoroutine *) {
    android_coroutine_unimpl(__func__);
}

void port_coroutine_yield(void) {
    android_coroutine_unimpl(__func__);
}

int port_coroutine_is_finished(PortCoroutine *) { return 1; }

int port_coroutine_in_coroutine(void) { return 0; }

#endif /* __ANDROID__ */
