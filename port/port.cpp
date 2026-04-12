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
#include <cstdlib>

#include "resource/ResourceType.h"
#include "resource/RelocFileFactory.h"
#include <ship/resource/factory/BlobFactory.h>
#include <ship/resource/ResourceType.h>

#include "bridge/audio_bridge.h"
#include "renderdoc_trigger.h"

static std::shared_ptr<Ship::Context> sContext;

extern "C" {

int PortInit(int argc, char* argv[]) {
	port_log("SSB64: PortInit entered\n");

	sContext = Ship::Context::CreateUninitializedInstance(
		"Super Smash Bros. 64",
		"ssb64",
		"ssb64.cfg.json"
	);

	if (!sContext) {
		port_log("SSB64: Failed to create context instance\n");
		return 1;
	}

	port_log("SSB64: Context instance created\n");

	if (!sContext->InitLogging()) { port_log("SSB64: InitLogging failed\n"); return 1; }
	port_log("SSB64: Logging OK\n");

	if (!sContext->InitConfiguration()) { port_log("SSB64: InitConfiguration failed\n"); return 1; }
	if (!sContext->InitConsoleVariables()) { port_log("SSB64: InitConsoleVariables failed\n"); return 1; }
	port_log("SSB64: Config + CVars OK\n");

	std::vector<std::string> archivePaths = {
		"ssb64.o2r",
		"f3d.o2r"
	};
	if (!sContext->InitResourceManager(archivePaths)) { port_log("SSB64: InitResourceManager failed\n"); return 1; }
	port_log("SSB64: ResourceManager OK\n");

	if (!sContext->InitCrashHandler()) { port_log("SSB64: InitCrashHandler failed\n"); return 1; }
	if (!sContext->InitConsole()) { port_log("SSB64: InitConsole failed\n"); return 1; }
	port_log("SSB64: CrashHandler + Console OK\n");

	// ControlDeck MUST be initialized before Window — the DXGI window proc
	// calls ControllerUnblockGameInput on WM_SETFOCUS during window creation.
	auto controlDeck = std::make_shared<LUS::ControlDeck>();
	if (!sContext->InitControlDeck(controlDeck)) { port_log("SSB64: InitControlDeck failed\n"); return 1; }
	port_log("SSB64: ControlDeck OK\n");

	auto window = std::make_shared<Fast::Fast3dWindow>();
	if (!sContext->InitWindow(window)) { port_log("SSB64: InitWindow failed\n"); return 1; }
	port_log("SSB64: Window OK\n");

	if (!sContext->InitAudio({})) { port_log("SSB64: InitAudio failed\n"); return 1; }
	if (!sContext->InitGfxDebugger()) { port_log("SSB64: InitGfxDebugger failed\n"); return 1; }
	if (!sContext->InitFileDropMgr()) { port_log("SSB64: InitFileDropMgr failed\n"); return 1; }
	port_log("SSB64: All subsystems initialized\n");

	// Register resource factories
	auto loader = sContext->GetResourceManager()->GetResourceLoader();
	loader->RegisterResourceFactory(
		std::make_shared<ResourceFactoryBinaryRelocFileV0>(),
		RESOURCE_FORMAT_BINARY,
		"SSB64Reloc",
		static_cast<uint32_t>(SSB64::ResourceType::SSB64Reloc),
		0
	);
	loader->RegisterResourceFactory(
		std::make_shared<Ship::ResourceFactoryBinaryBlobV0>(),
		RESOURCE_FORMAT_BINARY,
		"Blob",
		static_cast<uint32_t>(Ship::ResourceType::Blob),
		0
	);

	port_log("SSB64: Resource factories registered — init complete\n");
	return 0;
}

void PortShutdown(void) {
	// Drop audio bridge resource references before Ship::Context goes away.
	// Otherwise their shared_ptrs survive into __cxa_finalize_ranges and
	// Ship::IResource::~IResource() lands on a shut-down spdlog.
	portAudioShutdownAssets();
	sContext.reset();
	port_log_close();
}

int PortIsRunning(void) {
	return WindowIsRunning() ? 1 : 0;
}

} // extern "C"

int main(int argc, char* argv[]) {
	port_log_init("ssb64.log");

	// Initialize RenderDoc trigger BEFORE PortInit so the RenderDoc DLL
	// can hook D3D11 before LUS creates the device.
	portRenderDocInit();

	if (PortInit(argc, argv) != 0) {
		return 1;
	}

	// Initialize the game boot sequence (coroutines, thread init, etc.)
	PortGameInit();

	// Main frame loop — each iteration runs one frame of game logic
	// and rendering through the coroutine system. PortPushFrame posts
	// a VI tick, resumes the game coroutine, and display lists are
	// rendered via DrawAndRunGraphicsCommands inside the coroutine.
	//
	// SSB64_MAX_FRAMES=N — debug aid that forces a clean shutdown
	// after N frames. Goes through the same code path as the user
	// closing the window (Window::Close() sets mIsRunning=false).
	int maxFrames = 0;
	if (const char* env = std::getenv("SSB64_MAX_FRAMES")) {
		maxFrames = std::atoi(env);
	}
	int frame = 0;
	while (WindowIsRunning()) {
		PortPushFrame();
		frame++;
		if (maxFrames > 0 && frame >= maxFrames) {
			port_log("SSB64: SSB64_MAX_FRAMES=%d reached — triggering clean shutdown\n", maxFrames);
			if (auto ctx = Ship::Context::GetInstance()) {
				if (auto win = ctx->GetWindow()) {
					win->Close();
				}
			}
			break;
		}
	}

	PortGameShutdown();

	PortShutdown();
	portRenderDocShutdown();
	return 0;
}
