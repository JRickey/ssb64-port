#ifndef _FTLUIGI_H_
#define _FTLUIGI_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <ft/ftdef.h>

// #include "ftmariofunctions.h"

extern FTStatusDesc dFTLuigiSpecialStatusDescs[/* */];

extern void *gFTDataLuigiMain;
extern void *gFTDataLuigiMainMotion;
extern void *gFTDataLuigiModel;
extern void *gFTDataLuigiSpecial1;
extern void *gFTDataLuigiSpecial2;
extern void *gFTDataLuigiSpecial3;
extern s32 gFTDataLuigiParticleBankID;

typedef enum ftLuigiMotion
{
    nFTLuigiMotionAttack13 = nFTCommonMotionSpecialStart,
    nFTLuigiMotionAppearR,
    nFTLuigiMotionAppearL,
    nFTLuigiMotionSpecialN,
    nFTLuigiMotionSpecialAirN,
    nFTLuigiMotionSpecialHi,
    nFTLuigiMotionSpecialAirHi,
    nFTLuigiMotionSpecialLw,
    nFTLuigiMotionSpecialAirLw

} ftLuigiMotion;

typedef enum ftLuigiStatus
{
    nFTLuigiStatusAttack13 = nFTCommonStatusSpecialStart,
    nFTLuigiStatusAppearR,
    nFTLuigiStatusAppearL,
    nFTLuigiStatusSpecialN,
    nFTLuigiStatusSpecialAirN,
    nFTLuigiStatusSpecialHi,
    nFTLuigiStatusSpecialAirHi,
    nFTLuigiStatusSpecialLw,
    nFTLuigiStatusSpecialAirLw

} ftLuigiStatus;

#endif /* _FTLUIGI_H_ */
