#include <PR/os_internal.h>
#include <PR/controller.h>

#if BUILD_VERSION >= VERSION_J
s32 __osPfsSelectBank(OSPfs* pfs, u8 bank) {
	u8 temp[BLOCKSIZE];
	int i;
	s32 ret = 0;

	for (i = 0; i < BLOCKSIZE; i++) {
		temp[i] = bank;
	}

	ret = __osContRamWrite(pfs->queue, pfs->channel, CONT_BLOCK_DETECT, temp, FALSE);

	if (ret == 0)
		pfs->activebank = bank;

	return ret;
}
#endif
