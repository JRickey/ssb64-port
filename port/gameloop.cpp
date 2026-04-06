/**
 * gameloop.cpp — PC game loop implementation for the SSB64 port.
 *
 * This file implements the frame loop that drives SSB64's game logic on PC.
 * The N64 game uses 5 OS threads communicating via message queues. On PC,
 * we run the entire game in a single coroutine that yields at blocking
 * points (osRecvMesg with OS_MESG_BLOCK).
 *
 * Architecture:
 *   main() loop
 *     -> PortPushFrame()
 *       -> post INTR_VRETRACE to scheduler queue
 *       -> resume game coroutine
 *         -> scheduler coroutine processes tick (sends to clients)
 *         -> game logic runs one frame
 *         -> display list built, submitted to scheduler
 *         -> scheduler calls osSpTaskStartGo -> port_submit_display_list
 *         -> Fast3D renders via DrawAndRunGraphicsCommands
 *         -> game coroutine yields at next osRecvMesg(BLOCK) on empty queue
 */

#include "gameloop.h"
#include "coroutine.h"
#include "port.h"

#include <libultraship/libultraship.h>
#include <fast/Fast3dWindow.h>

#include <cstdio>
#include <unordered_map>

#include "port_log.h"

/* ========================================================================= */
/*  External game symbols (C linkage)                                        */
/* ========================================================================= */

extern "C" {

/* The N64 game entry point — starts the whole boot chain. */
extern void syMainLoop(void);

/* Scheduler task message queue — we post INTR_VRETRACE here each frame. */
extern OSMesgQueue gSYSchedulerTaskMesgQueue;

/* VI retrace interrupt value (matches scheduler.c local define). */
#define INTR_VRETRACE 1

} /* extern "C" */

/* ========================================================================= */
/*  Game coroutine state                                                     */
/* ========================================================================= */

static PortCoroutine *sGameCoroutine = NULL;

/* ========================================================================= */
/*  Game coroutine entry point                                               */
/* ========================================================================= */

/**
 * Wrapper for syMainLoop that matches the coroutine entry signature.
 *
 * syMainLoop creates Thread 1 (idle), which creates Thread 5 (game).
 * Each osStartThread creates a sub-coroutine. When Thread 1 finishes
 * (after starting Thread 5 and returning on PORT), syMainLoop returns.
 *
 * At that point, Thread 5's coroutine exists but is suspended. We need
 * to keep the game coroutine alive and act as the "scheduler" that
 * resumes Thread 5 (and other service threads) each frame.
 *
 * The game coroutine entry function runs syMainLoop (boot), then enters
 * an infinite yield loop. PortPushFrame resumes it each frame, and it
 * yields back immediately — the actual frame work happens when
 * PortPushFrame resumes the individual thread coroutines.
 */
static void game_coroutine_entry(void *arg)
{
	(void)arg;
	port_log("SSB64: Game coroutine started — entering syMainLoop\n");
	syMainLoop();
	port_log("SSB64: syMainLoop returned — boot chain complete\n");
	/* All thread coroutines are now created and suspended.
	 * PortPushFrame will resume them directly via port_resume_service_threads. */
}

/* ========================================================================= */
/*  Display list submission                                                  */
/* ========================================================================= */

/**
 * Called from osSpTaskStartGo (n64_stubs.c) when a GFX task is submitted.
 * Routes the N64 display list through Fast3D for rendering.
 */
static int sDLSubmitCount = 0;

extern "C" int port_get_display_submit_count(void)
{
	return sDLSubmitCount;
}

extern "C" void port_submit_display_list(void *dl)
{
	sDLSubmitCount++;
	if (sDLSubmitCount <= 60 || (sDLSubmitCount % 60 == 0)) {
		port_log("SSB64: port_submit_display_list #%d dl=%p\n", sDLSubmitCount, dl);
	}

	if (dl == NULL) {
		port_log("SSB64: WARNING — display list is NULL!\n");
		return;
	}

	auto context = Ship::Context::GetInstance();
	if (!context) {
		port_log("SSB64: WARNING — no Ship::Context in display list submit!\n");
		return;
	}

	auto window = std::dynamic_pointer_cast<Fast::Fast3dWindow>(context->GetWindow());
	if (!window) {
		port_log("SSB64: WARNING — no Fast3dWindow in display list submit!\n");
		return;
	}

	std::unordered_map<Mtx *, MtxF> mtxReplacements;
	window->DrawAndRunGraphicsCommands(static_cast<Gfx *>(dl), mtxReplacements);

	if (sDLSubmitCount <= 60) {
		port_log("SSB64: DrawAndRunGraphicsCommands returned OK\n");
	}
}

/* ========================================================================= */
/*  Public API                                                               */
/* ========================================================================= */

void PortGameInit(void)
{
	port_log("SSB64: PortGameInit — initializing coroutine system\n");

	/* Convert the main thread to a fiber so it can participate in
	 * coroutine switching. */
	port_coroutine_init_main();

	/* Create the game coroutine with a large stack (1 MB).
	 * This will host the entire game: syMainLoop -> Thread 1 -> Thread 5
	 * -> scheduler, controller, audio init -> scManagerRunLoop. */
	sGameCoroutine = port_coroutine_create(game_coroutine_entry, NULL, 1024 * 1024);
	if (sGameCoroutine == NULL) {
		port_log("SSB64: FATAL — failed to create game coroutine\n");
		return;
	}

	/* Resume the game coroutine to start the boot chain.
	 * It will run through syMainLoop -> osInitialize -> create Thread 1
	 * -> start Thread 1 -> Thread 1 creates Thread 5 -> Thread 5 inits
	 * peripherals, creates scheduler/audio/controller threads.
	 *
	 * Each thread runs in its own sub-coroutine (created by osStartThread).
	 * They all yield when they hit osRecvMesg(BLOCK) on empty queues.
	 * Eventually control returns here after the entire boot chain has
	 * progressed as far as it can without VI ticks. */
	port_log("SSB64: Starting game coroutine (boot sequence)\n");
	port_coroutine_resume(sGameCoroutine);
	port_log("SSB64: Boot sequence yielded — entering frame loop\n");
}

static int sFrameCount = 0;

void PortPushFrame(void)
{
	/* Pump SDL events so the window stays responsive and WindowIsRunning
	 * detects the close button. HandleEvents also updates controller state. */
	auto context = Ship::Context::GetInstance();
	if (context) {
		auto window = context->GetWindow();
		if (window) {
			window->HandleEvents();
		}
	}

	/* Post a VI retrace event to the scheduler's message queue.
	 * This is what the N64 hardware does at ~60Hz. */
	osSendMesg(&gSYSchedulerTaskMesgQueue, (OSMesg)INTR_VRETRACE, OS_MESG_NOBLOCK);

	/* Resume all service thread coroutines that are waiting for messages.
	 * This runs multiple rounds to handle cascading messages:
	 *   Round 1: Scheduler picks up VRETRACE, sends ticks to clients
	 *   Round 2: Controller reads input, game logic runs one frame
	 *   Round 3+: Display list submitted, scheduler processes GFX task, etc.
	 * Each thread runs until it yields at osRecvMesg(BLOCK) on empty queue. */
	port_resume_service_threads();

	sFrameCount++;
	if (sFrameCount <= 60 || (sFrameCount % 60 == 0)) {
		port_log("SSB64: Frame %d complete\n", sFrameCount);
	}
}

void PortGameShutdown(void)
{
	if (sGameCoroutine != NULL) {
		port_coroutine_destroy(sGameCoroutine);
		sGameCoroutine = NULL;
	}
	port_log("SSB64: Game coroutine destroyed\n");
}
