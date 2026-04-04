#include <PR/os_internal.h>
#include <PR/rcp.h>

// TODO: this comes from a header
#ident "$Revision: 1.17 $"

int __osDpDeviceBusy(void) {
	register u32 stat = IO_READ(DPC_STATUS_REG);

	if (stat & DPC_STATUS_DMA_BUSY) {
		return TRUE;
	} else {
		return FALSE;
	}
}
