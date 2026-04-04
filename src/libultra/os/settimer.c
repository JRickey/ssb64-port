#include <PR/os_internal.h>
#include <PR/ultraerror.h>
#include <PR/viint.h>
#include <osint.h>

int osSetTimer(OSTimer* t, OSTime countdown, OSTime interval, OSMesgQueue* mq, OSMesg msg) {
	
	OSTime time;

	t->next = NULL;
	t->prev = NULL;
	t->interval = interval;
	t->value = (countdown != 0) ? countdown : interval;
	t->mq = mq;
	t->msg = msg;
	time = __osInsertTimer(t);
	if (__osTimerList->next == t) {
		__osSetTimerIntr(time);
	}

	return 0;
}