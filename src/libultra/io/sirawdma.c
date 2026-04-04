#include <PR/os_internal.h>
#include <assert.h>
#include <siint.h>

// Adjust line numbers to match assert
#if BUILD_VERSION < VERSION_J
#line 49
#endif

// TODO: this comes from a header
#ident "$Revision: 1.17 $"

s32 __osSiRawStartDma(s32 direction, void* dramAddr) {
	assert(((u32)dramAddr & 0x3) == 0);

#if BUILD_VERSION >= VERSION_J
	if (IO_READ(SI_STATUS_REG) & (SI_STATUS_DMA_BUSY | SI_STATUS_RD_BUSY)) {
		return -1;
	}
#else
	if (__osSiDeviceBusy()) {
		return -1;
	}
#endif


	if (direction == OS_WRITE) {
		osWritebackDCache(dramAddr, 64);
	}

	IO_WRITE(SI_DRAM_ADDR_REG, osVirtualToPhysical(dramAddr));

	if (direction == OS_READ) {
		IO_WRITE(SI_PIF_ADDR_RD64B_REG, PIF_RAM_START);
	} else {
		IO_WRITE(SI_PIF_ADDR_WR64B_REG, PIF_RAM_START);
	}

	if (direction == OS_READ) {
		osInvalDCache(dramAddr, 64);
	}

	return 0;
}
