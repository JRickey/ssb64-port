#pragma once

/**
 * gameloop.h — PC game loop interface for the SSB64 port.
 *
 * Replaces the N64 multi-threaded model with a single-threaded frame loop.
 * The game's original code runs inside coroutines that yield at blocking
 * points (osRecvMesg BLOCK), and the main loop resumes them once per frame.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize the game boot sequence.
 * Creates the game loop coroutine and starts the N64 boot chain
 * (syMainLoop -> thread creation -> scManagerRunLoop).
 * Must be called after PortInit().
 */
void PortGameInit(void);

/**
 * Run one frame of the game loop.
 * Posts a VI retrace message, resumes the game coroutine (which runs
 * until it yields at the next osRecvMesg BLOCK), and presents the frame.
 * Must be called from the main loop after PortGameInit().
 */
void PortPushFrame(void);

/**
 * Shut down the game loop and destroy all coroutines.
 */
void PortGameShutdown(void);

/**
 * Resume all registered service thread coroutines that are waiting.
 * Called internally by PortPushFrame. Defined in n64_stubs.c.
 */
void port_resume_service_threads(void);

#ifdef __cplusplus
}
#endif
