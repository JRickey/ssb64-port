#ifndef _FTPURIN_H_
#define _FTPURIN_H_

#include "ftpurinfunctions.h"

#define FTPURIN_JUMPAERIAL_VEL_MUL 0.8F
#define FTPURIN_POUND_VEL_BASE 65.0F
#define FTPURIN_POUND_VEL_MUL 0.92F

extern FTStatusDesc dFTPurinSpecialStatusDescs[/* */];

extern void *gFTDataPurinMain;
extern void *gFTDataPurinMainMotion;
extern void *gFTDataPurinModel;
extern void *gFTDataPurinSpecial2;
extern s32 gFTDataPurinParticleBankID;

typedef enum ftPurinMotion
{
    nFTPurinMotionAttack100Start = nFTCommonMotionSpecialStart,
    nFTPurinMotionAttack100Loop,
    nFTPurinMotionAttack100End,            // These three above are unused
    nFTPurinMotionJumpAerialF1,
    nFTPurinMotionJumpAerialF2,
    nFTPurinMotionJumpAerialF3,
    nFTPurinMotionJumpAerialF4,
    nFTPurinMotionJumpAerialF5,
    nFTPurinMotionAppearR,
    nFTPurinMotionAppearL,
    nFTPurinMotionSpecialN,
    nFTPurinMotionSpecialAirN,
    nFTPurinMotionSpecialHi,
    nFTPurinMotionSpecialLw

} ftPurinMotion;

typedef enum ftPurinStatus
{
    nFTPurinStatusAttack100Start = nFTCommonStatusSpecialStart,
    nFTPurinStatusAttack100Loop,
    nFTPurinStatusAttack100End,            // These three above are unused
    nFTPurinStatusJumpAerialF1,
    ftStatus_purin_JumpAerialF2,
    nFTPurinStatusJumpAerialF3,
    nFTPurinStatusJumpAerialF4,
    nFTPurinStatusJumpAerialF5,
    nFTPurinStatusAppearR,
    nFTPurinStatusAppearL,
    nFTPurinStatusSpecialN,
    nFTPurinStatusSpecialAirN,
    nFTPurinStatusSpecialHi,
    nFTPurinStatusSpecialAirHi,
    nFTPurinStatusSpecialLw,
    nFTPurinStatusSpecialAirLw

} ftPurinStatus;

typedef struct FTPurinPassiveVars
{
    u32 unk_0x0;

} FTPurinPassiveVars;

#endif
