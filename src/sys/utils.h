#ifndef SYS_TRIG_RAND_H
#define SYS_TRIG_RAND_H

#include <PR/ultratypes.h>

extern f32 syUtilsTan(f32 angle);
extern f32 syUtilsArcTan(f32 yDivX);
extern f32 syUtilsArcTan2(f32 y, f32 x);
extern f32 syUtilsArcSin(f32 x);
extern f32 syUtilsArcCos(f32 x);
extern f32 syUtilsCsc(f32 x);
extern f32 syUtilsSec(f32 x);
extern f32 syUtilsCot(f32 x);
extern f32 __sinf(f32);
extern f32 __cosf(f32);

extern void syUtilsSetRandomSeedPtr(s32 *seedptr);
extern u16 syUtilsRandUShort(void);
extern f32 syUtilsRandFloat(void);
extern s32 syUtilsRandIntRange(s32 range); // Does this actually return u32? Found a function that needs to cast this to s32, and assigning to a variable first would bump the stack
extern u8 syUtilsRandTimeUChar(void);
extern f32 syUtilsRandTimeFloat(void);
extern s32 syUtilsRandTimeUCharRange(s32 range);

#endif /* SYS_TRIG_RAND_H */
