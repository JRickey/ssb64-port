#include <PR/rcp.h>
#include <PR/ultraerror.h>
#include <osint.h>

// TODO: not sure if this should be here
extern s32 osViClock;

// TODO: this comes from a header
#ident "$Revision: 1.17 $"

s32 osAiSetFrequency(u32 frequency) {
	register unsigned int dacRate;
	register unsigned char bitRate;
	register float f;

#ifdef _DEBUG
	if (osViClock == VI_PAL_CLOCK) {
		if (frequency < AI_PAL_MIN_FREQ || frequency > AI_PAL_MAX_FREQ) {
			__osError(ERR_OSAISETFREQUENCY, 3, AI_PAL_MIN_FREQ, AI_PAL_MAX_FREQ, frequency);
			return -1;
		}
	} else if (osViClock == VI_MPAL_CLOCK) {
		if (frequency < AI_MPAL_MIN_FREQ || frequency > AI_MPAL_MAX_FREQ) {
			__osError(ERR_OSAISETFREQUENCY, 3, AI_MPAL_MIN_FREQ, AI_MPAL_MAX_FREQ, frequency);
			return -1;
		}
	} else {
		if (frequency < AI_NTSC_MIN_FREQ || frequency > AI_NTSC_MAX_FREQ) {
			__osError(ERR_OSAISETFREQUENCY, 3, AI_NTSC_MIN_FREQ, AI_NTSC_MAX_FREQ, frequency);
			return -1;
		}
	}
#endif

	f = osViClock / (float)frequency + .5f;
	dacRate = f;

	if (dacRate < AI_MIN_DAC_RATE) {
		return -1;
	}

	bitRate = dacRate / 66;

	if (bitRate > AI_MAX_BIT_RATE) {
		bitRate = AI_MAX_BIT_RATE;
	}

	IO_WRITE(AI_DACRATE_REG, dacRate - 1);
	IO_WRITE(AI_BITRATE_REG, bitRate - 1);
#if defined(REGION_JP)
	IO_WRITE(AI_CONTROL_REG, AI_CONTROL_DMA_ON);
#endif
	return osViClock / (s32)dacRate;
}
