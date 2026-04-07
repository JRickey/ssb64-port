/**
 * audio_dma.c — PORT DMA callback for audio sample loading
 *
 * On N64, the audio DMA callback (syAudioDma) transfers ADPCM sample data
 * from ROM into RDRAM via the PI.  On PORT, all sample data is already in
 * memory — the audio bridge loaded TBL blobs into the audio heap and
 * wavetable->base pointers reference that data directly.
 *
 * This DMA callback simply returns the address it receives.  No copy needed.
 */

#include <stdint.h>
#include <stddef.h>

#ifdef PORT

/* Match ALDMAproc signature: uintptr_t(uintptr_t addr, s32 len, void* state) */
typedef uintptr_t (*PortAudioDMAproc)(uintptr_t addr, int len, void *state);

/* ------------------------------------------------------------------ */
/*  DMA proc — identity: data is already in memory                    */
/* ------------------------------------------------------------------ */

static uintptr_t portAudioDma(uintptr_t addr, int len, void *state)
{
	(void)len;
	(void)state;
	return addr;
}

/* ------------------------------------------------------------------ */
/*  DMA new — allocates state (none needed) and returns proc          */
/* ------------------------------------------------------------------ */

/* Returns function pointer compatible with ALDMAproc.
 * Called via syn_config.dmaproc in syAudioMakeBGMPlayers. */
void *portAudioDmaNew(void **state)
{
	if (state != NULL)
	{
		*state = NULL;
	}
	return (void *)portAudioDma;
}

#endif /* PORT */
