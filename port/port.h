#pragma once

// Port-wide header for SSB64 PC port
// Include this from decomp code that needs port-specific behavior

#include "port_log.h"

#ifdef __cplusplus
extern "C" {
#endif

// Initialize the port engine (Ship::Context, resource manager, window, etc.)
// Called once at startup before the game loop begins.
// Returns 0 on success, non-zero on failure.
int PortInit(int argc, char* argv[]);

// Shut down the port engine and release all resources.
void PortShutdown(void);

// Returns non-zero while the port window is running (not closed).
int PortIsRunning(void);

#ifdef __cplusplus
}
#endif
