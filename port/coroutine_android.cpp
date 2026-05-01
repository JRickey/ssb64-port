/**
 * coroutine_android.cpp — aarch64 coroutine impl for Android (bionic).
 *
 * Bionic libc dropped getcontext/makecontext/swapcontext (they were
 * never supported on aarch64), so the POSIX ucontext path in
 * coroutine_posix.cpp doesn't compile here. Instead, we ship a
 * minimal asm context-switch (port/coroutine_aarch64.S) that saves
 * the AAPCS64 callee-saved set — same approach as boost::context's
 * fcontext_t — and drive it from this file.
 *
 * The CoroCtx struct in this file MUST stay in lockstep with the
 * offsets in coroutine_aarch64.S. The static_asserts below catch any
 * drift at compile time.
 */

#if defined(__ANDROID__)

#if !defined(__aarch64__)
#  error "Android coroutine backend currently supports arm64-v8a only. "  \
         "Add an x86_64 / armv7 swap if you need those ABIs."
#endif

#include "coroutine.h"
#include "port_watchdog.h"

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define MIN_STACK_SIZE 32768

extern "C" {

struct alignas(16) CoroCtx {
    uint64_t x19, x20, x21, x22;   /* offsets   0..24 */
    uint64_t x23, x24, x25, x26;   /*          32..56 */
    uint64_t x27, x28, x29, x30;   /*          64..88 */
    uint64_t sp;                    /*             96 */
    uint64_t pad;                   /*            104 (keep d8 at 16-aligned 112) */
    double   d8,  d9,  d10, d11;   /*         112..136 */
    double   d12, d13, d14, d15;   /*         144..168 */
};
static_assert(sizeof(CoroCtx) == 176,           "CoroCtx size must match asm");
static_assert(__builtin_offsetof(CoroCtx, x19) == 0,      "x19 offset");
static_assert(__builtin_offsetof(CoroCtx, x29) == 80,     "x29 offset");
static_assert(__builtin_offsetof(CoroCtx, x30) == 88,     "x30 offset");
static_assert(__builtin_offsetof(CoroCtx, sp)  == 96,     "sp offset");
static_assert(__builtin_offsetof(CoroCtx, d8)  == 112,    "d8 offset");
static_assert(__builtin_offsetof(CoroCtx, d15) == 168,    "d15 offset");

struct PortCoroutine {
    CoroCtx ctx;            /* this coroutine's saved state */
    CoroCtx caller_ctx;     /* whoever last resumed us */
    void  (*entry)(void *); /* user entry */
    void   *arg;            /* arg passed to entry */
    int     finished;       /* 1 once entry returns */
    void   *stack_mem;      /* aligned_alloc'd by posix_memalign */
    size_t  stack_size;
};

/* Implemented in port/coroutine_aarch64.S */
void port_coroutine_swap(CoroCtx *from, CoroCtx *to);
void port_coroutine_trampoline_aarch64(void);

static thread_local PortCoroutine *sCurrentCoroutine = nullptr;

/* Called from the asm trampoline with PortCoroutine* in x0.
 * Marked extern "C" so the symbol matches the asm reference exactly. */
__attribute__((noreturn))
void port_coroutine_trampoline_c(PortCoroutine *co) {
    co->entry(co->arg);
    co->finished = 1;
    sCurrentCoroutine = nullptr;
    /* Permanent yield. There is no caller frame on this coroutine's stack
     * to return to, so we swap back to whoever last resumed us. They will
     * see the finished flag and never resume us again. */
    port_coroutine_swap(&co->ctx, &co->caller_ctx);
    /* If we got back here, the caller resumed a finished coroutine. That's
     * caller error — port_coroutine_resume guards against it, but a hand-
     * rolled swap could trip this. */
    fprintf(stderr, "SSB64: coroutine resumed after finish\n");
    abort();
}

/* ========================================================================= */
/*  Public API                                                               */
/* ========================================================================= */

void port_coroutine_init_main(void) {
    /* No-op. caller_ctx is populated by the first port_coroutine_swap;
     * before that, the main thread isn't tracked anywhere. */
}

PortCoroutine *port_coroutine_create(void (*entry)(void *), void *arg, size_t stack_size) {
    if (stack_size < MIN_STACK_SIZE) {
        stack_size = MIN_STACK_SIZE;
    }
    /* Round stack up to 16 bytes. AAPCS64 requires sp to be 16-aligned
     * at all public entry points. */
    stack_size = (stack_size + 15) & ~size_t{15};

    PortCoroutine *co = (PortCoroutine *)calloc(1, sizeof(PortCoroutine));
    if (!co) {
        return nullptr;
    }

    /* posix_memalign over aligned_alloc — the latter only landed in bionic
     * at API 28; we target 26. */
    void *stk = nullptr;
    if (posix_memalign(&stk, 16, stack_size) != 0) {
        free(co);
        return nullptr;
    }
    co->stack_mem  = stk;
    co->stack_size = stack_size;
    co->entry      = entry;
    co->arg        = arg;
    co->finished   = 0;

    /* Initial saved-context state. On first swap into this coroutine:
     *   sp  := top of stack (high address, 16-aligned)
     *   x19 := PortCoroutine* (read by the asm trampoline -> mov x0, x19)
     *   x30 := trampoline entry (lr, popped by `ret` at end of swap)
     * All other regs are zero-init via calloc. */
    char *stack_top = (char *)co->stack_mem + stack_size;
    co->ctx.sp  = (uint64_t)stack_top;
    co->ctx.x19 = (uint64_t)co;
    co->ctx.x30 = (uint64_t)&port_coroutine_trampoline_aarch64;

    return co;
}

void port_coroutine_destroy(PortCoroutine *co) {
    if (!co) return;
    if (co == sCurrentCoroutine) {
        fprintf(stderr, "SSB64: port_coroutine_destroy on current coroutine\n");
        abort();
    }
    if (co->stack_mem) {
        free(co->stack_mem);
    }
    free(co);
}

void port_coroutine_resume(PortCoroutine *co) {
    if (!co || co->finished) return;

    /* Save the previous current so nested resumes restore correctly.
     * Same semantics as the POSIX backend. Example: main resumes Thread5,
     * Thread5 resumes a GObj coroutine. When the GObj yields,
     * sCurrentCoroutine must be restored to Thread5 (not nullptr) so
     * Thread5 can yield later. */
    PortCoroutine *prev = sCurrentCoroutine;
    sCurrentCoroutine = co;

    port_coroutine_swap(&co->caller_ctx, &co->ctx);

    sCurrentCoroutine = prev;
}

void port_coroutine_yield(void) {
    PortCoroutine *co = sCurrentCoroutine;
    if (!co) {
        fprintf(stderr, "SSB64: port_coroutine_yield called outside coroutine\n");
        return;
    }
    port_watchdog_note_yield();
    sCurrentCoroutine = nullptr;
    port_coroutine_swap(&co->ctx, &co->caller_ctx);
    /* Returns here when resumed. */
}

int port_coroutine_is_finished(PortCoroutine *co) {
    return co ? co->finished : 1;
}

int port_coroutine_in_coroutine(void) {
    return sCurrentCoroutine != nullptr;
}

} /* extern "C" */

#endif /* __ANDROID__ */
