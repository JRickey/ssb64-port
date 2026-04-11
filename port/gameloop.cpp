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
#include <fast/interpreter.h>

#include <cstdio>
#include <chrono>
#include <unordered_map>
#include <unordered_set>
#include <filesystem>
#include <string>
#include <cstdlib>
#include <cstring>

/* GBI trace system */
#include "../debug_tools/gbi_trace/gbi_trace.h"

#include "port_log.h"
#include "renderdoc_trigger.h"

/* Backbuffer screenshot capture — implemented in libultraship's DX11 backend.
 * Returns 1 on success, 0 on failure (silent). Never throws. */
extern "C" int portFastCaptureBackbufferPNG(const char *path);

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

/*
 * port_make_os_mesg_int() — fully-initialised OSMesg from an integer code.
 *
 * The decomp treats OSMesg as `void*` (PR/os.h); libultraship treats it as a
 * union { u8; u16; u32; void*; } (libultraship/libultra/message.h). Both are
 * 8 bytes, so the calling convention matches, but writing only one union
 * member from C++ leaves the remaining bytes uninitialised — whatever was on
 * the stack in that slot. On MSVC/x64 those bytes happened to be zero and
 * the bug was latent; on macOS/arm64 they held a stack pointer, and the
 * scheduler's `(SYTaskInfo*)mesg` cast in sySchedulerThreadMain then jumped
 * through a garbage `fnCheck` pointer.
 *
 * All port-layer (C++) sends of integer interrupt codes should go through
 * this helper so the full 8 bytes are well-defined on every platform.
 */
static inline OSMesg port_make_os_mesg_int(uint32_t code)
{
	OSMesg m{};          /* zero-initialise every union member */
	m.data32 = code;     /* then set the scalar we care about */
	return m;
}

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
static int sGbiTraceInitDone = 0;

/* Thin C wrapper for the trace callback (matches GbiTraceCallbackFn signature) */
static void gbi_trace_callback(uintptr_t w0, uintptr_t w1, int dl_depth)
{
	gbi_trace_log_cmd((unsigned long long)w0, (unsigned long long)w1, dl_depth);
}

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

	/* Lazy-init the GBI trace system on first DL submit */
	if (!sGbiTraceInitDone) {
		sGbiTraceInitDone = 1;
		gbi_trace_init();
		if (gbi_trace_is_enabled()) {
			gfx_set_trace_callback(gbi_trace_callback);
		}
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

	/* Begin trace frame before Fast3D processes the display list */
	gbi_trace_begin_frame();

	std::unordered_map<Mtx *, MtxF> mtxReplacements;
	try {
		window->DrawAndRunGraphicsCommands(static_cast<Gfx *>(dl), mtxReplacements);
	} catch (long hr) {
		port_log("SSB64: CAUGHT DX shader exception HRESULT=0x%08lX on DL #%d\n", hr, sDLSubmitCount);
		gbi_trace_end_frame();
		return;
	} catch (...) {
		port_log("SSB64: CAUGHT unknown exception on DL #%d\n", sDLSubmitCount);
		gbi_trace_end_frame();
		return;
	}

	/* End trace frame after processing */
	gbi_trace_end_frame();

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

/* ========================================================================= */
/*  Screenshot capture (env-var driven)                                      */
/* ========================================================================= */
/*
 * Env vars:
 *   SSB64_SCREENSHOT_FRAMES=10,55,100  — frames to capture
 *   SSB64_SCREENSHOT_FRAMES=all        — capture every frame (heavy!)
 *   SSB64_SCREENSHOT_DIR=<path>        — output dir (default debug_traces/screenshots)
 *
 * When unset/empty, feature is OFF and adds only one bool check per frame.
 */

static bool sScreenshotInited = false;
static bool sScreenshotEnabled = false;
static bool sScreenshotAllFrames = false;
static std::unordered_set<int> sScreenshotFrames;
static std::string sScreenshotDir;

static void port_screenshot_init_once(void)
{
	if (sScreenshotInited) {
		return;
	}
	sScreenshotInited = true;

	const char *frames_env = std::getenv("SSB64_SCREENSHOT_FRAMES");
	if (frames_env == nullptr || frames_env[0] == '\0') {
		sScreenshotEnabled = false;
		return;
	}

	const char *dir_env = std::getenv("SSB64_SCREENSHOT_DIR");
	sScreenshotDir = (dir_env != nullptr && dir_env[0] != '\0')
		? dir_env
		: "debug_traces/screenshots";

	if (std::strcmp(frames_env, "all") == 0) {
		sScreenshotAllFrames = true;
	} else {
		/* Parse comma-separated integer list. Ignores malformed tokens. */
		const char *p = frames_env;
		while (*p != '\0') {
			while (*p == ',' || *p == ' ' || *p == '\t') {
				p++;
			}
			if (*p == '\0') {
				break;
			}
			char *end = nullptr;
			long v = std::strtol(p, &end, 10);
			if (end != p && v >= 0 && v <= 0x7FFFFFFF) {
				sScreenshotFrames.insert(static_cast<int>(v));
			}
			if (end == p) {
				/* Couldn't parse — skip a char to avoid infinite loop. */
				p++;
			} else {
				p = end;
			}
		}
	}

	sScreenshotEnabled = sScreenshotAllFrames || !sScreenshotFrames.empty();

	if (sScreenshotEnabled) {
		std::error_code ec;
		std::filesystem::create_directories(sScreenshotDir, ec);
		if (ec) {
			port_log("SSB64: screenshot: failed to create dir '%s': %s\n",
				sScreenshotDir.c_str(), ec.message().c_str());
		}
		if (sScreenshotAllFrames) {
			port_log("SSB64: screenshot capture ENABLED (all frames) dir='%s'\n",
				sScreenshotDir.c_str());
		} else {
			port_log("SSB64: screenshot capture ENABLED (%zu frames) dir='%s'\n",
				sScreenshotFrames.size(), sScreenshotDir.c_str());
		}
	}
}

static void port_screenshot_maybe_capture(int frame)
{
	if (!sScreenshotEnabled) {
		return;
	}
	if (!sScreenshotAllFrames && sScreenshotFrames.count(frame) == 0) {
		return;
	}

	char path[1024];
	std::snprintf(path, sizeof(path), "%s/frame_%d.png",
		sScreenshotDir.c_str(), frame);

	int ok = portFastCaptureBackbufferPNG(path);
	if (ok) {
		port_log("SSB64: screenshot frame %d -> %s\n", frame, path);
	} else {
		port_log("SSB64: screenshot frame %d FAILED -> %s\n", frame, path);
	}
}

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

	/* Post a VI retrace event to the scheduler's message queue. See
	 * port_make_os_mesg_int() above for why we don't just write
	 * `(OSMesg)INTR_VRETRACE` here. */
	osSendMesg(&gSYSchedulerTaskMesgQueue, port_make_os_mesg_int(INTR_VRETRACE), OS_MESG_NOBLOCK);

	/* Resume all service thread coroutines that are waiting for messages.
	 * This runs multiple rounds to handle cascading messages:
	 *   Round 1: Scheduler picks up VRETRACE, sends ticks to clients
	 *   Round 2: Controller reads input, game logic runs one frame
	 *   Round 3+: Display list submitted, scheduler processes GFX task, etc.
	 * Each thread runs until it yields at osRecvMesg(BLOCK) on empty queue. */
	port_resume_service_threads();

	sFrameCount++;

	/* Screenshot capture: env-var driven, zero cost when disabled. */
	port_screenshot_init_once();
	port_screenshot_maybe_capture(sFrameCount);

	/* RenderDoc capture trigger: env-var driven, zero cost when disabled.
	 * TriggerCapture() tells RenderDoc to capture the NEXT Present interval,
	 * so calling it here causes frame (sFrameCount + 1) to be captured.
	 * The one-frame lag is small and consistent — document it in the
	 * feature's env-var contract. */
	portRenderDocOnFrame(static_cast<unsigned int>(sFrameCount));

	{
		static auto sStartTime = std::chrono::steady_clock::now();
		auto now = std::chrono::steady_clock::now();
		double elapsed = std::chrono::duration<double>(now - sStartTime).count();
		if (sFrameCount <= 60 || (sFrameCount % 60 == 0)) {
			port_log("SSB64: Frame %d complete (t=%.2fs)\n", sFrameCount, elapsed);
		}
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
