/*
 * coroutine_test.cpp — standalone validation harness for the Android
 * aarch64 coroutine backend.
 *
 * Build via -DSSB64_BUILD_COROUTINE_TEST=ON. Produces a tiny aarch64
 * executable `coroutine_test`. Push to /data/local/tmp/ and run via
 * `adb shell` to verify create/resume/yield/destroy + nested resume.
 *
 * The test does NOT link the rest of the game; it pulls in only the
 * coroutine impl and a minimal stub for port_watchdog_note_yield. If
 * this binary passes on the emulator, the asm context-switch is sound.
 */

#include "coroutine.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>

/* Minimal stub for the symbol coroutine_android.cpp references but we
 * don't want to drag the full watchdog into the test binary. */
extern "C" void port_watchdog_note_yield(void) {}

namespace {

int g_seq[64];
int g_seq_n = 0;

#define EXPECT(cond) \
    do { if (!(cond)) { fprintf(stderr, "FAIL %s:%d %s\n", __FILE__, __LINE__, #cond); std::exit(1); } } while (0)

void worker(void *arg) {
    int id = (int)(intptr_t)arg;
    for (int i = 0; i < 3; i++) {
        g_seq[g_seq_n++] = id * 10 + i;
        port_coroutine_yield();
    }
}

/* Round-robin alternation: two coroutines, three yields each. The
 * expected interleave is a0,b0,a1,b1,a2,b2 because the main loop
 * resumes A then B each round. */
void test_round_robin() {
    g_seq_n = 0;
    PortCoroutine *a = port_coroutine_create(worker, (void *)(intptr_t)1, 64 * 1024);
    PortCoroutine *b = port_coroutine_create(worker, (void *)(intptr_t)2, 64 * 1024);
    EXPECT(a && b);

    /* 4 rounds is more than enough — workers finish after 3 yields each. */
    for (int round = 0; round < 4; round++) {
        if (!port_coroutine_is_finished(a)) port_coroutine_resume(a);
        if (!port_coroutine_is_finished(b)) port_coroutine_resume(b);
    }

    EXPECT(port_coroutine_is_finished(a));
    EXPECT(port_coroutine_is_finished(b));
    EXPECT(g_seq_n == 6);

    int expected[] = {10, 20, 11, 21, 12, 22};
    for (int i = 0; i < 6; i++) {
        EXPECT(g_seq[i] == expected[i]);
    }

    port_coroutine_destroy(a);
    port_coroutine_destroy(b);
}

/* Nested resume: outer creates inner, resumes it, gets yielded back to,
 * yields out to main. Validates that sCurrentCoroutine save/restore in
 * port_coroutine_resume preserves the chain. */
PortCoroutine *g_inner_co = nullptr;
unsigned g_nest_log = 0;

void inner_entry(void *) {
    g_nest_log |= 0x02;
    port_coroutine_yield();   /* yields back to outer */
    g_nest_log |= 0x08;
}

void outer_entry(void *) {
    g_nest_log |= 0x01;
    port_coroutine_resume(g_inner_co);
    g_nest_log |= 0x04;
    port_coroutine_yield();   /* yields back to main */
    g_nest_log |= 0x10;
}

void test_nested() {
    g_nest_log = 0;
    g_inner_co = port_coroutine_create(inner_entry, nullptr, 64 * 1024);
    PortCoroutine *outer = port_coroutine_create(outer_entry, nullptr, 64 * 1024);
    EXPECT(g_inner_co && outer);

    port_coroutine_resume(outer);
    /* By now: outer ran (1), resumed inner (2), inner yielded back to
     * outer, outer continued (4), outer yielded back to main. So 0x07. */
    EXPECT(g_nest_log == 0x07);

    port_coroutine_resume(g_inner_co);
    EXPECT((g_nest_log & 0x08) != 0);

    port_coroutine_resume(outer);
    EXPECT((g_nest_log & 0x10) != 0);

    EXPECT(port_coroutine_is_finished(g_inner_co));
    EXPECT(port_coroutine_is_finished(outer));

    port_coroutine_destroy(g_inner_co);
    port_coroutine_destroy(outer);
}

/* Stack hygiene: the worker writes a recognisable pattern to a chunk of
 * its own stack, yields, runs again. The data must survive across the
 * yield (i.e. the swap really restored the right sp). */
void stack_worker(void *arg) {
    volatile uint64_t pattern[256];
    uint64_t seed = (uint64_t)(intptr_t)arg;
    for (int i = 0; i < 256; i++) pattern[i] = seed ^ (uint64_t)i;
    port_coroutine_yield();
    for (int i = 0; i < 256; i++) {
        if (pattern[i] != (seed ^ (uint64_t)i)) {
            fprintf(stderr, "FAIL: stack corrupted at %d (seed=%llx)\n",
                    i, (unsigned long long)seed);
            std::exit(1);
        }
    }
}

void test_stack_preservation() {
    PortCoroutine *a = port_coroutine_create(stack_worker, (void *)0xCAFEBABE, 128 * 1024);
    PortCoroutine *b = port_coroutine_create(stack_worker, (void *)0xDEADBEEF, 128 * 1024);
    port_coroutine_resume(a);
    port_coroutine_resume(b);
    port_coroutine_resume(a);
    port_coroutine_resume(b);
    EXPECT(port_coroutine_is_finished(a));
    EXPECT(port_coroutine_is_finished(b));
    port_coroutine_destroy(a);
    port_coroutine_destroy(b);
}

/* in_coroutine query: false from main, true from inside a worker. */
volatile int g_in_coro_seen = -1;
void in_coro_worker(void *) {
    g_in_coro_seen = port_coroutine_in_coroutine();
}
void test_in_coroutine() {
    EXPECT(port_coroutine_in_coroutine() == 0);
    PortCoroutine *c = port_coroutine_create(in_coro_worker, nullptr, 64 * 1024);
    port_coroutine_resume(c);
    EXPECT(g_in_coro_seen == 1);
    EXPECT(port_coroutine_in_coroutine() == 0);
    port_coroutine_destroy(c);
}

} /* namespace */

int main() {
    port_coroutine_init_main();

    test_round_robin();
    test_nested();
    test_stack_preservation();
    test_in_coroutine();

    puts("PASS coroutine_test");
    return 0;
}
