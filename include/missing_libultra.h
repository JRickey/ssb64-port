#ifndef MISSING_LIBULTRA_H
#define MISSING_LIBULTRA_H

#include <PR/ultratypes.h>

s32 osAfterPreNMI(void); // this function is in 2.0J, but where?

// libultra internal and not decomped (need to get some J headers...)
extern s32 __osMotorAccess(OSPfs* pfs, s32 start);
#define MOTOR_START 1
#define MOTOR_STOP 0
#define osMotorStart(pfs) __osMotorAccess((pfs), MOTOR_START)
#define osMotorStop(pfs) __osMotorAccess((pfs), MOTOR_STOP)

#endif /* MISSING_LIBULTRA_H */
