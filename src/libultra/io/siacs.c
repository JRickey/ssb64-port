#include <PR/os_internal.h>

#define SI_Q_BUF_LEN 1
static OSMesg siAccessBuf[SI_Q_BUF_LEN];
OSMesgQueue __osSiAccessQueue;
u32 __osSiAccessQueueEnabled = 0;

void __osSiCreateAccessQueue()
{
	__osSiAccessQueueEnabled = 1;
	osCreateMesgQueue(&__osSiAccessQueue, siAccessBuf, SI_Q_BUF_LEN);
	osSendMesg(&__osSiAccessQueue, NULL, OS_MESG_NOBLOCK);
}
void __osSiGetAccess()
{
	OSMesg dummyMesg;
	if (!__osSiAccessQueueEnabled)
		__osSiCreateAccessQueue();
	osRecvMesg(&__osSiAccessQueue, &dummyMesg, OS_MESG_BLOCK);
}
void __osSiRelAccess() { osSendMesg(&__osSiAccessQueue, NULL, OS_MESG_NOBLOCK); }
