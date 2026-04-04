#include <piint.h>
#include <PR/ultraerror.h>

s32 osEPiStartDma(OSPiHandle* pihandle, OSIoMesg* mb, s32 direction) {
	register s32 ret;

	if (!__osPiDevMgr.active) {
#ifdef _DEBUG
		__osError(ERR_OSPISTARTDMA_PIMGR, 0);
#endif
		return -1;
	}

#ifdef _DEBUG
	if ((mb->hdr.pri != OS_MESG_PRI_NORMAL) && (mb->hdr.pri != OS_MESG_PRI_HIGH)) {
		__osError(ERR_OSPISTARTDMA_PRI, 1, mb->hdr.pri);
		return -1;
	}

	if ((direction != OS_READ) && (direction != OS_WRITE)) {
		__osError(ERR_OSPISTARTDMA_DIR, 1, direction);
		return -1;
	}

	if (mb->devAddr & 0x1) {
		__osError(ERR_OSPISTARTDMA_DEVADDR, 1, mb->devAddr);
		return -1;
	}

	if ((u32)mb->dramAddr & 0x7) {
		__osError(ERR_OSPISTARTDMA_ADDR, 1, mb->dramAddr);
		return -1;
	}

	if (mb->size & 0x1) {
		__osError(ERR_OSPISTARTDMA_SIZE, 1, mb->size);
		return -1;
	}

	if ((mb->size == 0) || (mb->size > (16 * 1024 * 1024))) {
		__osError(ERR_OSPISTARTDMA_RANGE, 1, mb->size);
		return -1;
	}
#endif

	mb->piHandle = pihandle;

	if (direction == OS_READ) {
		mb->hdr.type = OS_MESG_TYPE_EDMAREAD;
	} else {
		mb->hdr.type = OS_MESG_TYPE_EDMAWRITE;
	}

	if (mb->hdr.pri == OS_MESG_PRI_HIGH) {
		ret = osJamMesg(osPiGetCmdQueue(), (OSMesg)mb, OS_MESG_NOBLOCK);
	} else {
		ret = osSendMesg(osPiGetCmdQueue(), (OSMesg)mb, OS_MESG_NOBLOCK);
	}

	return ret;
}
