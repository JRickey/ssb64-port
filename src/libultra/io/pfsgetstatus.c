#include <PR/os_internal.h>
#include <PR/controller.h>
#include <siint.h>

#if defined(REGION_US)
void __osPfsRequestOneChannel(int channel, u8 cmd);
#else
void __osPfsRequestOneChannel(int channel);
#endif
void __osPfsGetOneChannelData(int channel, OSContStatus* data);

s32 __osPfsGetStatus(OSMesgQueue* queue, int channel) {
	s32 ret = 0;
	OSMesg dummy;
	OSContStatus data;

	__osPfsInodeCacheBank = 250;
#if defined(REGION_US)
	__osPfsRequestOneChannel(channel, CONT_CMD_REQUEST_STATUS);
#else
	__osPfsRequestOneChannel(channel);
#endif

	ret = __osSiRawStartDma(OS_WRITE, &__osPfsPifRam);
	osRecvMesg(queue, &dummy, OS_MESG_BLOCK);

	ret = __osSiRawStartDma(OS_READ, &__osPfsPifRam);
	osRecvMesg(queue, &dummy, OS_MESG_BLOCK);

	__osPfsGetOneChannelData(channel, &data);

	if (((data.status & CONT_CARD_ON) != 0) && ((data.status & CONT_CARD_PULL) != 0)) {
		return PFS_ERR_NEW_PACK;
	} else if ((data.errno != 0) || ((data.status & CONT_CARD_ON) == 0)) {
		return PFS_ERR_NOPACK;
	} else if ((data.status & CONT_ADDR_CRC_ER) != 0) {
		return PFS_ERR_CONTRFAIL;
	}

	return ret;
}

#if defined(REGION_US)
void __osPfsRequestOneChannel(int channel, u8 cmd) {
#else
void __osPfsRequestOneChannel(int channel) {
#endif
	u8* ptr;
	__OSContRequesFormatShort requestformat;
	int i;

#if defined(REGION_US)
	__osContLastCmd = CONT_CMD_END;
#else
	__osContLastCmd = CONT_CMD_REQUEST_STATUS;
#endif
	__osPfsPifRam.pifstatus = CONT_CMD_READ_BUTTON;

	ptr = (u8*)&__osPfsPifRam;

	requestformat.txsize = CONT_CMD_REQUEST_STATUS_TX;
	requestformat.rxsize = CONT_CMD_REQUEST_STATUS_RX;
#if defined(REGION_US)
	requestformat.cmd = cmd;
#else
	requestformat.cmd = CONT_CMD_REQUEST_STATUS;
#endif
	requestformat.typeh = CONT_CMD_NOP;
	requestformat.typel = CONT_CMD_NOP;
	requestformat.status = CONT_CMD_NOP;

	for (i = 0; i < channel; i++) {
		*ptr++ = CONT_CMD_REQUEST_STATUS;
	}

	*(__OSContRequesFormatShort*)ptr = requestformat;
	ptr += sizeof(__OSContRequesFormatShort);
	*ptr = CONT_CMD_END;
}

void __osPfsGetOneChannelData(int channel, OSContStatus* data) {
	u8* ptr = (u8*)&__osPfsPifRam;
	__OSContRequesFormatShort requestformat;
	int i;

	for (i = 0; i < channel; i++) {
		ptr++;
	}

	requestformat = *(__OSContRequesFormatShort*)ptr;
	data->errno = CHNL_ERR(requestformat);

	if (data->errno != 0) {
		return;
	}

	data->type = (requestformat.typel << 8) | (requestformat.typeh);
	data->status = requestformat.status;
}
