#define SDL_MAIN_HANDLED
#include "port.h"
#include "gameloop.h"

#include <libultraship/libultraship.h>
#include <libultraship/controller/controldeck/ControlDeck.h>
#include <fast/Fast3dWindow.h>
#include <ship/resource/File.h>
#include <string>
#include <vector>
#include <cstdio>

#include "resource/ResourceType.h"
#include "resource/RelocFileFactory.h"

static std::shared_ptr<Ship::Context> sContext;

// Debug trace log — InitLogging redirects stderr on Windows debug builds,
// so we use our own file for traces that survive the redirect.
static FILE* sTraceLog = nullptr;

static void TraceLog(const char* msg) {
	if (!sTraceLog) {
		sTraceLog = fopen("ssb64_init_trace.log", "w");
	}
	if (sTraceLog) {
		fprintf(sTraceLog, "%s\n", msg);
		fflush(sTraceLog);
	}
	// Also try stderr in case it still works (pre-InitLogging)
	fprintf(stderr, "%s\n", msg);
	fflush(stderr);
}

extern "C" {

void port_trace(const char* msg) {
	fprintf(stderr, "%s", msg);
	fflush(stderr);
}

int PortInit(int argc, char* argv[]) {
	TraceLog("SSB64: PortInit entered");

	// Step-by-step initialization is required because:
	// 1. InitWindow/InitControlDeck don't create defaults — we must provide them
	// 2. ControlDeck must exist before Window (DXGI wndproc accesses it on WM_SETFOCUS)
	// 3. ControlDeck construction needs ConsoleVariables (for GlobalSDLDeviceSettings)
	// 4. Audio needs SDL initialized (done inside Fast3dWindow::Init)
	sContext = Ship::Context::CreateUninitializedInstance(
		"Super Smash Bros. 64",
		"ssb64",
		"ssb64.cfg.json"
	);

	if (!sContext) {
		TraceLog("SSB64: Failed to create context instance");
		return 1;
	}

	TraceLog("SSB64: Context instance created");

	// Phase 1: Core systems (no SDL dependency)
	if (!sContext->InitLogging()) { TraceLog("SSB64: InitLogging failed"); return 1; }
	TraceLog("SSB64: Logging OK");

	if (!sContext->InitConfiguration()) { TraceLog("SSB64: InitConfiguration failed"); return 1; }
	if (!sContext->InitConsoleVariables()) { TraceLog("SSB64: InitConsoleVariables failed"); return 1; }
	TraceLog("SSB64: Config + CVars OK");

	std::vector<std::string> archivePaths = { "ssb64.o2r" };
	if (!sContext->InitResourceManager(archivePaths)) { TraceLog("SSB64: InitResourceManager failed"); return 1; }
	TraceLog("SSB64: ResourceManager OK");

	if (!sContext->InitCrashHandler()) { TraceLog("SSB64: InitCrashHandler failed"); return 1; }
	if (!sContext->InitConsole()) { TraceLog("SSB64: InitConsole failed"); return 1; }
	TraceLog("SSB64: CrashHandler + Console OK");

	// ControlDeck MUST be initialized before Window — the DXGI window proc
	// calls ControllerUnblockGameInput on WM_SETFOCUS during window creation.
	// ControlDeck needs ConsoleVariables (for GlobalSDLDeviceSettings), which
	// are already initialized above.
	auto controlDeck = std::make_shared<LUS::ControlDeck>();
	if (!sContext->InitControlDeck(controlDeck)) { TraceLog("SSB64: InitControlDeck failed"); return 1; }
	TraceLog("SSB64: ControlDeck OK");

	// Window initializes SDL and the graphics backend (DXGI/OpenGL)
	auto window = std::make_shared<Fast::Fast3dWindow>();
	if (!sContext->InitWindow(window)) { TraceLog("SSB64: InitWindow failed"); return 1; }
	TraceLog("SSB64: Window OK");

	if (!sContext->InitAudio({})) { TraceLog("SSB64: InitAudio failed"); return 1; }
	if (!sContext->InitGfxDebugger()) { TraceLog("SSB64: InitGfxDebugger failed"); return 1; }
	if (!sContext->InitFileDropMgr()) { TraceLog("SSB64: InitFileDropMgr failed"); return 1; }
	TraceLog("SSB64: All subsystems initialized");

	// Register SSB64-specific resource factories
	auto loader = sContext->GetResourceManager()->GetResourceLoader();
	loader->RegisterResourceFactory(
		std::make_shared<ResourceFactoryBinaryRelocFileV0>(),
		RESOURCE_FORMAT_BINARY,
		"SSB64Reloc",
		static_cast<uint32_t>(SSB64::ResourceType::SSB64Reloc),
		0
	);

	TraceLog("SSB64: Resource factories registered — init complete");
	return 0;
}

void PortShutdown(void) {
	sContext.reset();
	if (sTraceLog) {
		fclose(sTraceLog);
		sTraceLog = nullptr;
	}
}

int PortIsRunning(void) {
	return WindowIsRunning() ? 1 : 0;
}

} // extern "C"

int main(int argc, char* argv[]) {
	// SDL_MAIN_HANDLED is defined above to prevent SDL from hijacking main().

	if (PortInit(argc, argv) != 0) {
		return 1;
	}

	// Initialize the game boot sequence (coroutines, thread init, etc.)
	PortGameInit();

	// Main frame loop — each iteration runs one frame of game logic
	// and rendering through the coroutine system. PortPushFrame posts
	// a VI tick, resumes the game coroutine, and display lists are
	// rendered via DrawAndRunGraphicsCommands inside the coroutine.
	while (WindowIsRunning()) {
		PortPushFrame();
	}

	PortGameShutdown();

	PortShutdown();
	return 0;
}
