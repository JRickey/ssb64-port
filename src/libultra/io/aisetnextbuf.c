#include <PR/os_internal.h>
#include <PR/ultraerror.h>
#include <PR/rcp.h>
#include <osint.h>

s32 osAiSetNextBuffer(void* bufPtr, u32 size) {
	static u8 hdwrBugFlag = FALSE;
	char* bptr;

	bptr = bufPtr;

	if (hdwrBugFlag) {
		bptr = (u8*)bufPtr - 0x2000;
	}

	if ((((u32)bufPtr + size) & 0x1fff) == 0) {
		hdwrBugFlag = TRUE;
	} else {
		hdwrBugFlag = FALSE;
	}

	if (__osAiDeviceBusy()) {
		return -1;
	}

	IO_WRITE(AI_DRAM_ADDR_REG, osVirtualToPhysical(bptr));
	IO_WRITE(AI_LEN_REG, size);
	return 0;
}