#include "port.h"

#include <libultraship/libultraship.h>
#include <string>
#include <vector>

static std::shared_ptr<Ship::Context> sContext;

extern "C" {

int PortInit(int argc, char* argv[]) {
	// Archive paths: the game will look for these O2R files at runtime.
	// "ssb64.o2r" contains extracted game assets (from Torch).
	// Additional O2R files can be added for port-specific resources.
	std::vector<std::string> archivePaths = { "ssb64.o2r" };

	sContext = Ship::Context::CreateInstance(
		"Super Smash Bros. 64",  // Window title
		"ssb64",                 // Short name (used for config paths)
		"ssb64.cfg.json",       // Config file path
		archivePaths
	);

	if (!sContext) {
		return 1;
	}

	return 0;
}

void PortShutdown(void) {
	sContext.reset();
}

int PortIsRunning(void) {
	return WindowIsRunning() ? 1 : 0;
}

} // extern "C"
