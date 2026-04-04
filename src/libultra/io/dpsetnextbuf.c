#include <PR/os_internal.h>
#include <PR/ultraerror.h>
#include <PR/rcp.h>
#include <osint.h>

// TODO: this comes from a header
#ident "$Revision: 1.17 $"

s32 osDpSetNextBuffer(void* bufPtr, u64 size) {
	register u32 stat;

#ifdef _DEBUG
	if ((u32)bufPtr & 0x7) {
		__osError(ERR_OSDPSETNEXTBUFFER_ADDR, 1, bufPtr);
		return -1;
	}
	if (size & 0x7) {
		__osError(ERR_OSDPSETNEXTBUFFER_SIZE, 1, size);
		return -1;
	}
#endif

	if (__osDpDeviceBusy()) {
		return -1;
	}

	IO_WRITE(DPC_STATUS_REG, DPC_CLR_XBUS_DMEM_DMA);

	while (TRUE) {
		stat = IO_READ(DPC_STATUS_REG);
		if ((stat & DPC_STATUS_XBUS_DMEM_DMA) == 0) {
			break;
		}
	}

	IO_WRITE(DPC_START_REG, osVirtualToPhysical(bufPtr));
	IO_WRITE(DPC_END_REG, osVirtualToPhysical(bufPtr) + size);
	return 0;
}
