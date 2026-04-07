#pragma once

/* PORT DMA callback for audio — passthrough (data already in memory). */

#ifdef PORT

#ifdef __cplusplus
extern "C" {
#endif

/* Returns an ALDMAproc-compatible function pointer.
 * Declared as void* to avoid requiring libaudio.h in port/ code. */
void *portAudioDmaNew(void **state);

#ifdef __cplusplus
}
#endif

#endif /* PORT */
