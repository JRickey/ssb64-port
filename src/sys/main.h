#ifndef SYS_MAIN_H
#define SYS_MAIN_H

#include <PR/os.h>
#include <PR/ultratypes.h>

extern OSMesgQueue gSYMainThreadingMesgQueue;
extern u64 gSYMainRspBootCode[0x20];
extern OSThread gSYMainThread5;

extern void syMainVerifyStackProbes(void);

#endif /* SYS_MAIN_H */
