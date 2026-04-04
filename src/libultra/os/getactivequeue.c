#include <PR/os_internal.h>
#include <osint.h>

OSThread* __osGetActiveQueue(void) {
    return __osActiveQueue;
}
