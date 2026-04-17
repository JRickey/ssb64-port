#pragma once

/**
 * port_watchdog.h — background hang-detection for the cooperative scheduler.
 *
 * The PC port runs the game inside cooperative coroutines. If any coroutine
 * enters an unyielded spin loop (e.g. a busy-wait on audio state that the
 * audio coroutine is supposed to clear), the main thread never returns from
 * port_resume_service_threads and the frame pump halts. Visible to the user
 * as a frozen window / macOS spinning beach ball.
 *
 * This module runs a separate OS thread that polls cheap liveness counters
 * (yield count, frame count) and, when they stop advancing, logs a snapshot
 * of which service-thread coroutine was last active so the bug can be
 * narrowed down. It does not interrupt or kill the hang — just surfaces it
 * in ssb64.log and on stderr.
 *
 * Thread-safety: all note_* entry points are lock-free and safe to call
 * from any thread (including coroutines running on the main thread).
 */

#ifdef __cplusplus
extern "C" {
#endif

void port_watchdog_init(void);
void port_watchdog_shutdown(void);

void port_watchdog_note_yield(void);
void port_watchdog_note_resume_start(int thread_id);
void port_watchdog_note_resume_end(int thread_id);
void port_watchdog_note_frame_end(void);

#ifdef __cplusplus
}
#endif
