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
#include "port_watchdog.h"
#include "frame_interpolation.h"
#include "xr/xr_runtime.h"

#include <libultraship/libultraship.h>
#include <libultraship/bridge/consolevariablebridge.h>
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

/* Simulated N64 VI VBlank: propagates the queued framebuffer to "current"
 * so the scheduler's CheckReadyFramebuffer fnCheck can correctly stall a
 * gfx task when VI is still using the slot the game wants to draw into.
 * Implemented in port/stubs/n64_stubs.c. */
extern "C" void port_vi_simulate_vblank(void);

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

/* Frame-interpolation multiplier — CVar (gSettings.FrameInterpolationMult),
 * env var fallback (SSB64_INTERP_MULT), and a hard cap.
 *
 *   N=1: disabled, identical to historical behavior.
 *   N=2: render twice per tick (e.g. 30→60 Hz).
 *   N=3: render thrice.
 *   N=4: 30→120 Hz on a 120 Hz display.
 *
 * Resolved every frame so the slider in Settings → Graphics → Frame
 * Interpolation Mult takes effect immediately, no relaunch needed.
 *
 * KNOWN LIMITATION (slow motion): on a vsynced 60 Hz display, each call to
 * DrawAndRunGraphicsCommands blocks for ~1/60 s on present, so calling it
 * N times per game tick stretches each tick to N/60 s, slowing game logic
 * to 60/N Hz. The proper fix requires a separate renderer thread or
 * vsync-bypass on intermediate presents (libultraship lacks the API to
 * present without a vsync block from the port side). On a 120 Hz monitor
 * mult=2 works as intended; on a 60 Hz monitor it's a slo-mo demo only. */
#define SSB64_INTERP_MULT_MAX 8
#define CVAR_FRAME_INTERP_MULT "gSettings.FrameInterpolationMult"

static int port_get_interp_mult(void)
{
	int env_default = 1;
	if (const char *e = std::getenv("SSB64_INTERP_MULT")) {
		int v = std::atoi(e);
		if (v >= 1 && v <= SSB64_INTERP_MULT_MAX) env_default = v;
	}
	int n = CVarGetInteger(CVAR_FRAME_INTERP_MULT, env_default);
	if (n < 1) n = 1;
	if (n > SSB64_INTERP_MULT_MAX) n = SSB64_INTERP_MULT_MAX;

	/* Log only on transitions so a stable session doesn't spam the log. */
	static int s_last_logged = -1;
	if (n != s_last_logged) {
		port_log("SSB64: frame interpolation mult = %d (1 = disabled)\n", n);
		s_last_logged = n;
	}
	return n;
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

	/* The game just finished building this frame's display list and all
	 * matrices it touched are now sitting in g_current. Stop recording so
	 * the replay loop sees a stable snapshot. */
	FrameInterpolation_StopRecord();

	const int mult = port_get_interp_mult();

	/* Begin trace frame before Fast3D processes the display list */
	gbi_trace_begin_frame();

	/* Slow-motion fix: when mult>1, the DXGI backend throttles BOTH on the
	 * vsync interval (each Present() blocks ~1/60s) AND on a CPU-side wait
	 * loop targeting `mTargetFps` (default 60, gfx_dxgi.cpp:43). Together
	 * they cap presents at 60 Hz, so doing mult presents per game tick
	 * stretches each tick to mult/60s and slows logic to 60/mult Hz.
	 *
	 * Workaround:
	 *   1. Disable vsync for intermediate presents by flipping
	 *      gVsyncEnabled (DXGI re-reads it at SwapBuffersBegin).
	 *   2. Bump target FPS to 60*mult so the wait loop completes faster.
	 * Both restored before the final present so it runs with the user's
	 * chosen vsync setting and tear-free.
	 *
	 * Intermediate presents will tear (no vsync), but they're transient —
	 * the display will hit a clean vsync on the final present each tick.
	 *
	 * No-op when mult == 1. */
	const bool needsTimingToggle = (mult > 1);
	int origVsync = 1;
	int origTargetFps = 60;
	if (needsTimingToggle) {
		auto cv = context->GetConsoleVariables();
		origVsync = cv ? cv->GetInteger("gVsyncEnabled", 1) : 1;
		origTargetFps = window->GetTargetFps();
		window->SetTargetFps(60 * mult);
	}
	auto restore_timing = [&]() {
		if (!needsTimingToggle) return;
		auto cv = context->GetConsoleVariables();
		if (cv) cv->SetInteger("gVsyncEnabled", origVsync);
		window->SetTargetFps(origTargetFps);
	};

	for (int i = 1; i <= mult; i++) {
		std::unordered_map<Mtx *, MtxF> mtxReplacements;
		bool isIntermediate = (mult > 1 && i < mult);
		if (isIntermediate) {
			/* Intermediate frames: build a replacement map for lerp factor t.
			 * t in (0, 1); the final pass (i==mult, t==1) uses an empty map
			 * so the GBI's actual matrices flow through unchanged — this
			 * keeps the last display frame bit-exact with the un-instrumented
			 * path and serves as the self-test invariant. */
			float t = static_cast<float>(i) / static_cast<float>(mult);
			mtxReplacements = FrameInterpolation_Interpolate(t);
		}

		/* Vsync toggle: OFF on intermediate, restored on final. */
		if (needsTimingToggle) {
			auto cv = context->GetConsoleVariables();
			if (cv) {
				cv->SetInteger("gVsyncEnabled", isIntermediate ? 0 : origVsync);
			}
		}

		try {
			window->DrawAndRunGraphicsCommands(static_cast<Gfx *>(dl), mtxReplacements);
		} catch (long hr) {
			port_log("SSB64: CAUGHT DX shader exception HRESULT=0x%08lX on DL #%d (interp pass %d/%d)\n",
				hr, sDLSubmitCount, i, mult);
			gbi_trace_end_frame();
			restore_timing();
			return;
		} catch (...) {
			port_log("SSB64: CAUGHT unknown exception on DL #%d (interp pass %d/%d)\n",
				sDLSubmitCount, i, mult);
			gbi_trace_end_frame();
			restore_timing();
			return;
		}
	}
	restore_timing();

	/* End trace frame after processing */
	gbi_trace_end_frame();

	if (sDLSubmitCount <= 60) {
		port_log("SSB64: DrawAndRunGraphicsCommands returned OK (mult=%d)\n", mult);
	}
}

/* ========================================================================= */
/*  Public API                                                               */
/* ========================================================================= */

void PortGameInit(void)
{
	port_log("SSB64: PortGameInit — initializing coroutine system\n");

	/* Run frame-interpolation unit tests if SSB64_FRAME_INTERP_UNITTEST is set.
	 * Exits the process with status 2 on failure. No-op otherwise. */
	FrameInterpolation_RunSelfTestIfRequested();

	/* OpenXR session init (opt-in via SSB64_XR_ENABLE=1). When the build is
	 * not configured with -DSSB64_ENABLE_OPENXR=ON, this logs a stub message
	 * and returns non-zero, leaving xr_runtime_is_active() == 0 so the
	 * per-frame hooks no-op. See port/xr/xr_runtime.h for completion notes. */
	if (xr_runtime_init() == 0) {
		port_log("SSB64: XR runtime active: %s\n", xr_runtime_status());
	}

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

	/* Start the hang watchdog now that the boot chain has yielded back.
	 * This avoids firing false alarms during the long synchronous boot. */
	port_watchdog_init();
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

	/* Propagate the previous frame's queued framebuffer (if any) to VI's
	 * "current" slot. The scheduler's CheckReadyFramebuffer fnCheck reads
	 * osViGetCurrent/NextFramebuffer to decide whether the slot the game
	 * wants to draw into is still locked by VI; without this rotation
	 * those getters would always report NULL and the scheduler would
	 * never stall, so the intentional freeze frames during fighter
	 * intros and the desk-to-stage transition would never appear. Run
	 * this BEFORE posting INTR_VRETRACE so sySchedulerSwapBuffer and
	 * the gfx-task fnCheck see the rotated state. */
	port_vi_simulate_vblank();

	/* Post a VI retrace event to the scheduler's message queue. See
	 * port_make_os_mesg_int() above for why we don't just write
	 * `(OSMesg)INTR_VRETRACE` here. */
	osSendMesg(&gSYSchedulerTaskMesgQueue, port_make_os_mesg_int(INTR_VRETRACE), OS_MESG_NOBLOCK);

	/* Begin a fresh recording for this frame's matrix builds. The current
	 * recording becomes the "previous" snapshot for the next frame's lerp.
	 * This MUST happen before threads run (they call syMatrix* which records
	 * into g_current). It MUST happen after the previous frame's render
	 * completed, otherwise we'd lose ops still being replayed.
	 *
	 * The corresponding StopRecord lives in port_submit_display_list, which
	 * fires synchronously inside port_resume_service_threads below. */
	FrameInterpolation_StartRecord();

	/* OpenXR per-frame begin (no-op when XR inactive). */
	xr_runtime_begin_frame();

	/* Resume all service thread coroutines that are waiting for messages.
	 * This runs multiple rounds to handle cascading messages:
	 *   Round 1: Scheduler picks up VRETRACE, sends ticks to clients
	 *   Round 2: Controller reads input, game logic runs one frame
	 *   Round 3+: Display list submitted, scheduler processes GFX task, etc.
	 * Each thread runs until it yields at osRecvMesg(BLOCK) on empty queue. */
	port_resume_service_threads();

	sFrameCount++;

	/* Tell the hang watchdog a frame completed. */
	port_watchdog_note_frame_end();

	/* OpenXR per-frame end — composites the desktop framebuffer into the XR
	 * quad-layer swapchain and submits xrEndFrame. No-op when XR inactive. */
	xr_runtime_end_frame();

	/* Frame-interpolation telemetry (env-var gated, ~60 ticks per log line). */
	FrameInterpolation_TelemetryTick();

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
	xr_runtime_shutdown();
	port_watchdog_shutdown();
	if (sGameCoroutine != NULL) {
		port_coroutine_destroy(sGameCoroutine);
		sGameCoroutine = NULL;
	}
	port_log("SSB64: Game coroutine destroyed\n");
}
