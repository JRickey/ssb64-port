/**
 * n64_stubs.c — Stub implementations for N64 OS functions and hardware symbols.
 *
 * On N64, these are provided by the libultra SDK and handle threading,
 * RSP/RDP task management, controller polling, and other hardware
 * operations.  In the PC port, the threading model is collapsed to
 * single-threaded, RSP tasks are handled by Fast3D/libultraship, and
 * controllers are managed by libultraship's ControlDeck.
 *
 * These provide PC-compatible implementations — coroutine-based threading,
 * coroutine-aware message queues, and no-ops for hardware operations.
 */

#include <ssb_types.h>
#include <PR/os.h>
#include <PR/sptask.h>
#include <PR/mbi.h>
#include <PR/os_internal.h>

#include <stdio.h>
#include <string.h>

#include "port_log.h"

/* ========================================================================= */
/*  Threading — coroutine-based emulation                                    */
/* ========================================================================= */

/*
 * On N64, the game uses a multi-threaded model with separate threads for
 * the game loop, scheduler, audio, and controller polling.  In the port,
 * each N64 OS thread runs as a coroutine (Win32 Fiber / ucontext).
 *
 * osCreateThread captures the entry point and argument.
 * osStartThread creates a coroutine (lazily) and resumes it.
 * osStopThread(NULL) yields the current coroutine.
 * osDestroyThread cleans up the coroutine.
 */

#include "coroutine.h"
#include "port_watchdog.h"

/* Default stack sizes for coroutines.  N64 stacks are tiny (1-4KB) but
 * PC calling conventions and 64-bit pointers need much more. */
#define PORT_STACK_SERVICE  (256 * 1024)  /* OS threads 1-8 */
#define PORT_STACK_GOBJ     (64 * 1024)   /* GObj thread processes */

/* ---- Thread registry ----
 * Tracks all service threads (ID < 100) so PortPushFrame can resume them.
 * GObj threads (ID >= 100) are not registered — they're resumed by
 * gcRunGObjProcess via osStartThread.
 */
#define PORT_MAX_SERVICE_THREADS 16
static OSThread *sServiceThreads[PORT_MAX_SERVICE_THREADS];
static s32 sServiceThreadCount = 0;

static void port_register_service_thread(OSThread *t)
{
	if (t->id >= 100) return;  /* Skip GObj threads */
	for (s32 i = 0; i < sServiceThreadCount; i++) {
		if (sServiceThreads[i] == t) return;  /* Already registered */
	}
	if (sServiceThreadCount < PORT_MAX_SERVICE_THREADS) {
		sServiceThreads[sServiceThreadCount++] = t;
	}
}

static s32 sResumeDebugCount = 0;

/* Called by PortPushFrame to resume all service thread coroutines
 * that have messages waiting in their blocked queues. */
void port_resume_service_threads(void)
{
	if (sResumeDebugCount < 3) {
		port_log( "SSB64: port_resume_service_threads: %d threads registered\n",
		        (int)sServiceThreadCount);
		for (s32 i = 0; i < sServiceThreadCount; i++) {
			OSThread *t = sServiceThreads[i];
			port_log( "  Thread %d: state=%d coroutine=%p finished=%d\n",
			        (int)t->id, (int)t->state,
			        t->port_coroutine,
			        t->port_coroutine ? port_coroutine_is_finished((PortCoroutine *)t->port_coroutine) : -1);
		}
		sResumeDebugCount++;
	}

	/* Resume threads in priority order (higher priority first).
	 * Simple bubble: scheduler(120) > controller(115) > audio(110) > game(50) */
	for (s32 round = 0; round < 8; round++) {
		s32 progress = 0;
		for (s32 i = 0; i < sServiceThreadCount; i++) {
			OSThread *t = sServiceThreads[i];
			if (t == NULL || t->port_coroutine == NULL) continue;
			if (port_coroutine_is_finished((PortCoroutine *)t->port_coroutine)) continue;
			if (t->state != OS_STATE_WAITING) continue;

			/* Resume this thread — it will run until it yields again. */
			t->state = OS_STATE_RUNNING;
			port_watchdog_note_resume_start((int)t->id);
			port_coroutine_resume((PortCoroutine *)t->port_coroutine);
			port_watchdog_note_resume_end((int)t->id);
			if (port_coroutine_is_finished((PortCoroutine *)t->port_coroutine)) {
				t->state = OS_STATE_STOPPED;
			} else {
				t->state = OS_STATE_WAITING;
			}
			if (sResumeDebugCount <= 5) {
				port_log("  round %d: resumed thread %d (finished=%d)\n",
				         (int)round, (int)t->id,
				         port_coroutine_is_finished((PortCoroutine *)t->port_coroutine));
			}
			progress = 1;
		}
		if (!progress) {
			if (sResumeDebugCount <= 5) {
				port_log("  round %d: no progress, breaking\n", (int)round);
			}
			break;  /* No thread made progress — done for this frame */
		}
	}
	if (sResumeDebugCount <= 5) sResumeDebugCount++;
}

void osCreateThread(OSThread *t, OSId id, void (*entry)(void *), void *arg,
                    void *sp, OSPri p)
{
	memset(t, 0, sizeof(OSThread));
	t->id = id;
	t->priority = p;
	t->state = OS_STATE_STOPPED;
	t->port_entry = entry;
	t->port_arg = arg;
	t->port_coroutine = NULL;
	port_register_service_thread(t);
	port_log( "SSB64: osCreateThread id=%d entry=%p (registered=%d total)\n",
	        (int)id, (void *)entry, (int)sServiceThreadCount);
}

void osStartThread(OSThread *t)
{
	if (t == NULL) {
		return;
	}

	/* Create the coroutine lazily on first start. */
	if (t->port_coroutine == NULL && t->port_entry != NULL) {
		size_t stack_size = (t->id < 100) ? PORT_STACK_SERVICE : PORT_STACK_GOBJ;
		t->port_coroutine = port_coroutine_create(t->port_entry, t->port_arg, stack_size);
		if (t->port_coroutine == NULL) {
			port_log( "SSB64: failed to create coroutine for thread %d\n", (int)t->id);
			return;
		}
	}

	/* Resume the coroutine.  It will run until it yields (at an
	 * osRecvMesg BLOCK or osStopThread) then return here. */
	if (t->port_coroutine != NULL) {
		t->state = OS_STATE_RUNNING;
		port_coroutine_resume((PortCoroutine *)t->port_coroutine);
		if (port_coroutine_is_finished((PortCoroutine *)t->port_coroutine)) {
			t->state = OS_STATE_STOPPED;
		} else {
			t->state = OS_STATE_WAITING;
		}
	}
}

void osStopThread(OSThread *t)
{
	if (t == NULL) {
		/* Stop current thread = yield the current coroutine. */
		port_coroutine_yield();
	}
	/* Stopping a specific thread other than self is a no-op on PC.
	 * The N64 OS would remove it from the run queue. */
}

void osDestroyThread(OSThread *t)
{
	if (t == NULL) {
		return;
	}
	if (t->port_coroutine != NULL) {
		port_coroutine_destroy((PortCoroutine *)t->port_coroutine);
		t->port_coroutine = NULL;
	}
	t->port_entry = NULL;
	t->state = OS_STATE_STOPPED;
}

void osSetThreadPri(OSThread *t, OSPri p)
{
	if (t != NULL) {
		t->priority = p;
	}
}

OSPri osGetThreadPri(OSThread *t)
{
	if (t != NULL) {
		return t->priority;
	}
	return 0;
}

/* ========================================================================= */
/*  Message queues — coroutine-aware blocking                                */
/* ========================================================================= */

/*
 * These override libultraship's non-blocking message queue functions.
 * The key difference: osRecvMesg with OS_MESG_BLOCK yields the current
 * coroutine when the queue is empty, instead of spinning or returning -1.
 */

void osCreateMesgQueue(OSMesgQueue *mq, OSMesg *msg, s32 count)
{
	mq->mtqueue = NULL;
	mq->fullqueue = NULL;
	mq->validCount = 0;
	mq->first = 0;
	mq->msgCount = count;
	mq->msg = msg;
}

s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag)
{
	if (mq->validCount >= mq->msgCount) {
		if (flag == OS_MESG_NOBLOCK) {
			return -1;
		}
		/* OS_MESG_BLOCK on full queue — yield until space available. */
		while (mq->validCount >= mq->msgCount) {
			if (port_coroutine_in_coroutine()) {
				port_coroutine_yield();
			} else {
				/* Main thread can't yield — drop the message. */
				return -1;
			}
		}
	}

	s32 idx = (mq->first + mq->validCount) % mq->msgCount;
	mq->msg[idx] = msg;
	mq->validCount++;
	return 0;
}

static s32 sRecvDebugCount = 0;

s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag)
{
	/* Non-blocking: return immediately if empty. */
	if (mq->validCount == 0) {
		if (flag == OS_MESG_NOBLOCK) {
			return -1;
		}
		/* OS_MESG_BLOCK: yield until a message arrives. */
		if (sRecvDebugCount < 50) {
			port_log("SSB64: osRecvMesg BLOCK on mq=%p (valid=%d cap=%d) — yielding\n",
			         (void *)mq, (int)mq->validCount, (int)mq->msgCount);
			sRecvDebugCount++;
		}
		while (mq->validCount == 0) {
			if (port_coroutine_in_coroutine()) {
				port_coroutine_yield();
			} else {
				/* Main thread can't yield — return failure.
				 * This should only happen during init before coroutines. */
				return -1;
			}
		}
		if (sRecvDebugCount < 50) {
			port_log("SSB64: osRecvMesg BLOCK on mq=%p — woke up (valid=%d)\n",
			         (void *)mq, (int)mq->validCount);
		}
	}

	if (msg != NULL) {
		*msg = mq->msg[mq->first];
	}
	mq->first = (mq->first + 1) % mq->msgCount;
	mq->validCount--;
	return 0;
}

s32 osJamMesg(OSMesgQueue *mq, OSMesg msg, s32 flag)
{
	if (mq->validCount >= mq->msgCount) {
		if (flag == OS_MESG_NOBLOCK) {
			return -1;
		}
		while (mq->validCount >= mq->msgCount) {
			if (port_coroutine_in_coroutine()) {
				port_coroutine_yield();
			} else {
				return -1;
			}
		}
	}

	mq->first = (mq->first + mq->msgCount - 1) % mq->msgCount;
	mq->msg[mq->first] = msg;
	mq->validCount++;
	return 0;
}

/*
 * Event-message table — maps hardware events to message queues.
 * On PC, only used for SP/DP completion if needed.  VI events
 * are posted manually by PortPushFrame.
 */
typedef struct {
	OSMesgQueue *queue;
	OSMesg msg;
} __OSEventState;

#define OS_NUM_EVENTS 15
__OSEventState __osEventStateTab[OS_NUM_EVENTS] = { { 0 } };

void osSetEventMesg(OSEvent event, OSMesgQueue *mq, OSMesg msg)
{
	if (event < OS_NUM_EVENTS) {
		__osEventStateTab[event].queue = mq;
		__osEventStateTab[event].msg = msg;
	}
}

/* ========================================================================= */
/*  OS initialisation                                                        */
/* ========================================================================= */

void osInitialize(void)
{
}

/* ========================================================================= */
/*  VI manager — disabled on PC                                              */
/* ========================================================================= */

/*
 * On N64, osCreateViManager creates a system thread that sends VI retrace
 * messages at ~60Hz.  LUS implements this with an SDL timer.  In the port,
 * we post VI messages manually from PortPushFrame() so we override these
 * to prevent the SDL timer from flooding the queue.
 */

void osCreateViManager(OSPri pri)
{
	/* No-op: VI messages are posted manually by PortPushFrame. */
}

void osViSetEvent(OSMesgQueue *mq, OSMesg msg, u32 retrace_count)
{
	/* No-op: VI events not needed — we drive timing from PortPushFrame. */
}

/*
 * Simulated N64 VI framebuffer rotation.
 *
 * The decomp scheduler installs `sySchedulerCheckReadyFramebuffer` (and
 * scene-specific siblings like `mvOpeningRoomCheckSetFramebuffer`) as the
 * `fnCheck` on each gfx task. Those callbacks decide whether the gfx task
 * is allowed to run *now* by asking VI which framebuffer is currently on
 * screen and which is queued to swap, and refusing to start the task if
 * the slot the game wants to draw into is one of those two. When the
 * check fails, the gfx task stays in the scheduler's paused queue and
 * the game thread blocks on its retVal `osRecvMesg`. The visual freeze
 * during fighter intros (and the `mvOpeningRoom` desk→stage transition)
 * is exactly that: VI hangs on the previous framebuffer for one or more
 * extra retraces while the gfx pipeline catches up.
 *
 * Stubbing both getters to NULL made every check trivially succeed, so
 * the game thread never blocked and the freezes never appeared. We now
 * model the two-slot rotation explicitly:
 *
 *   - osViSwapBuffer(fb)      -> queues `fb` as VI's next-up slot.
 *   - port_vi_simulate_vblank -> propagates next -> current (called once
 *                                per PortPushFrame, before game logic).
 *   - osViGetNextFramebuffer  -> returns the queued slot.
 *   - osViGetCurrentFramebuffer -> returns the slot VI is showing.
 *
 * After osViSwapBuffer, current != next until the next VBlank propagates
 * the swap; the scheduler's `next == curr` test then correctly drives
 * the rest of the rotation.
 */
static void *sPortViCurrentFB;
static void *sPortViNextFB;

void osViSwapBuffer(void *frameBufPtr)
{
	sPortViNextFB = frameBufPtr;
}

void port_vi_simulate_vblank(void)
{
	if (sPortViNextFB != NULL) {
		sPortViCurrentFB = sPortViNextFB;
	}
}

void osViSetMode(OSViMode *mode)
{
	/* No-op: video mode managed by the window backend. */
}

void osViBlack(u8 active)
{
	/* No-op: blackout handled at higher level. */
}

void osViSetSpecialFeatures(u32 func)
{
}

void *osViGetNextFramebuffer(void)
{
	return sPortViNextFB;
}

void *osViGetCurrentFramebuffer(void)
{
	return sPortViCurrentFB;
}

void osViSetXScale(f32 value)
{
}

void osViSetYScale(f32 value)
{
}

/* ========================================================================= */
/*  RSP task stubs                                                           */
/* ========================================================================= */

/*
 * RSP task submission is replaced by Fast3D in the port.
 *
 * When the scheduler submits a GFX task via osSpTaskStart (which calls
 * osSpTaskLoad + osSpTaskStartGo), we intercept GFX tasks and route the
 * display list to Fast3D via port_submit_display_list().
 *
 * Audio tasks are no-ops (audio is currently stubbed).
 */

/* Defined in gameloop.cpp */
extern void port_submit_display_list(void *dl);

void osSpTaskLoad(OSTask *tp)
{
}

/* Scheduler queue — declared in scheduler.c, externed in scheduler.h */
extern OSMesgQueue gSYSchedulerTaskMesgQueue;

/* INTR values matching scheduler.c local defines */
#define PORT_INTR_SP_TASK_DONE 2
#define PORT_INTR_DP_FULL_SYNC 3

static s32 sTaskGoCount = 0;

void osSpTaskStartGo(OSTask *tp)
{
	sTaskGoCount++;
	if (tp == NULL) {
		port_log("SSB64: osSpTaskStartGo #%d — NULL task!\n", (int)sTaskGoCount);
		return;
	}

	if (sTaskGoCount <= 60 || (sTaskGoCount % 60 == 0)) {
		port_log("SSB64: osSpTaskStartGo #%d type=%d data_ptr=%p\n",
		         (int)sTaskGoCount, (int)tp->t.type, (void *)tp->t.data_ptr);
	}

	if (tp->t.type == M_GFXTASK && tp->t.data_ptr != NULL) {
		/* Render the display list synchronously via Fast3D. */
		port_submit_display_list((void *)tp->t.data_ptr);
	}

	/* Simulate hardware completion: post SP_TASK_DONE and DP_FULL_SYNC
	 * to the scheduler queue so it can process task completion. */
	osSendMesg(&gSYSchedulerTaskMesgQueue, (OSMesg)PORT_INTR_SP_TASK_DONE, OS_MESG_NOBLOCK);
	osSendMesg(&gSYSchedulerTaskMesgQueue, (OSMesg)PORT_INTR_DP_FULL_SYNC, OS_MESG_NOBLOCK);
}

void osSpTaskYield(void)
{
}

OSYieldResult osSpTaskYielded(OSTask *tp)
{
	return 0;
}

/* ========================================================================= */
/*  Controller stubs                                                         */
/* ========================================================================= */

/*
 * Controller input is handled by libultraship's ControlDeck, which maps
 * modern gamepad/keyboard input to OSContPad format.
 */

s32 osContStartQuery(OSMesgQueue *mq)
{
	if (mq != NULL) {
		osSendMesg(mq, NULL, OS_MESG_NOBLOCK);
	}
	return 0;
}

void osContGetQuery(OSContStatus *data)
{
	s32 i;

	if (data == NULL) {
		return;
	}
	for (i = 0; i < MAXCONTROLLERS; i++) {
		data[i].errno = 0;
		data[i].status = CONT_CARD_ON;
		data[i].type = 0;
	}
}

/* ========================================================================= */
/*  I/O stubs                                                                */
/* ========================================================================= */

s32 osDpSetNextBuffer(void *buf, u64 size)
{
	return 0;
}

s32 osEPiLinkHandle(OSPiHandle *handle)
{
	return 0;
}

/*
 * DMA transfers — osEPiStartDma and osPiStartDma are provided by
 * libultraship (libultra/os.cpp, libultra/os_pi.cpp).  We no longer
 * define them here to avoid duplicate symbol errors.
 *
 * The game's syDmaCopy/syDmaLoadOverlay are no-opped via #ifdef PORT
 * in dma.c, so these LUS stubs are never called from game code.
 */

s32 osAfterPreNMI(void)
{
	return 0;
}

s32 osAiSetFrequency(u32 freq)
{
	/* PORT: N64 version programs the AI DAC and returns the actual
	 * achievable frequency.  On PC we use SDL audio at whatever rate
	 * the game requests, so just return freq unchanged. */
	return (s32)freq;
}

/* ========================================================================= */
/*  Internal OS function stubs                                               */
/* ========================================================================= */

OSThread *__osGetActiveQueue(void)
{
	return NULL;
}

void __osSetWatchLo(u32 val)
{
}

/* ========================================================================= */
/*  Global OS variables                                                      */
/* ========================================================================= */

s32 osTvType = 1;     /* OS_TV_NTSC */
s32 osResetType = 0;  /* Cold reset */

/*
 * OSViMode structs for NTSC and MPAL low-resolution non-interlaced modes.
 * On PC these are never used for actual video mode setting — libultraship
 * manages the window through SDL2.  Zero-initialised to satisfy the linker.
 */
OSViMode osViModeNtscLan1 = { 0 };
OSViMode osViModeMpalLan1 = { 0 };

/* ========================================================================= */
/*  RSP microcode symbols                                                    */
/* ========================================================================= */

/*
 * On N64 these point to the F3DEX2 graphics microcode and the N64 audio
 * microcode loaded onto the RSP.  In the port, Fast3D replaces the RSP
 * for graphics and audio is handled by SDL2.
 *
 * Declared as long long int[] in include/PR/ucode.h and
 * include/n_audio/n_libaudio.h.
 */
long long int gspF3DEX2_fifoTextStart[1] = { 0 };
long long int gspF3DEX2_fifoDataStart[1] = { 0 };
long long int n_aspMainTextStart[1] = { 0 };
long long int n_aspMainDataStart[1] = { 0 };

/* ========================================================================= */
/*  Sprite library stubs                                                     */
/* ========================================================================= */

/*
 * The N64 sprite library (libultra sp/) renders 2D sprites by building
 * GBI display lists.  The decomp's sprite.c depends on drawbitmap which
 * is MIPS assembly only, so we can't compile it.  These stubs allow the
 * game to link — sprite rendering will be reimplemented in the port layer.
 *
 * Declared in include/PR/sp.h.
 */

#include <PR/sp.h>

void spInit(Gfx **glistp)
{
}

void spScissor(s32 xmin, s32 xmax, s32 ymin, s32 ymax)
{
}

Gfx *spDraw(Sprite *sp)
{
	return NULL;
}

void spFinish(Gfx **glistp)
{
}

/* ========================================================================= */
/*  IDO assert                                                               */
/* ========================================================================= */

/*
 * The decomp's include/assert.h defines assert() using __assert(), which
 * is an IDO runtime function.  On PC we just print and abort.
 */

#include <stdio.h>
#include <stdlib.h>

void __assert(const char *expr, const char *file, int line)
{
	port_log( "Assertion failed: %s, file %s, line %d\n", expr, file, line);
	abort();
}
