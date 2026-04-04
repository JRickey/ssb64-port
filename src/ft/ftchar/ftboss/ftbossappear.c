#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8015AEE0
void ftBossAppearProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    DObj *topn_joint = fp->joints[nFTPartsJointTopN], *transn_joint = fp->joints[nFTPartsJointTransN];

    topn_joint->translate.vec.f = transn_joint->translate.vec.f;
}
