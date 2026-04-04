#include <ft/fighter.h>
// // // // // // // // // // // //
//                               //
//             MACROS            //
//                               //
// // // // // // // // // // // //

#define ftDonkeyThrowFWalkCheckInterrupt(fighter_gobj)                                    \
(                                                                                   \
    (ftCommonHeavyThrowCheckInterruptCommon(fighter_gobj) != FALSE) ||            \
    (ftDonkeyThrowFFCheckInterruptThrowFCommon(fighter_gobj) != FALSE) ||         \
    (ftDonkeyThrowFKneeBendCheckInterruptThrowFCommon(fighter_gobj) != FALSE) ||  \
    (ftDonkeyThrowFFallCheckInterruptPass(fighter_gobj) != FALSE) ||              \
    (ftDonkeyThrowFWaitCheckInterruptThrowFWalk(fighter_gobj) != FALSE)           \
)

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8014D530
f32 ftDonkeyThrowFWalkGetWalkAnimLength(FTStruct *fp, s32 status_id)
{
    f32 walk_anim_length;

    switch (status_id)
    {
    case nFTDonkeyStatusThrowFWalkSlow:
        walk_anim_length = fp->attr->throw_walkslow_anim_length;
        break;

    case nFTDonkeyStatusThrowFWalkMiddle:
        walk_anim_length = fp->attr->throw_walkmiddle_anim_length;
        break;

    case nFTDonkeyStatusThrowFWalkFast:
        walk_anim_length = fp->attr->throw_walkfast_anim_length;
        break;
    }
    return walk_anim_length;
}

// 0x8014D590
void ftDonkeyThrowFWalkProcInterrupt(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (!(ftDonkeyThrowFWalkCheckInterrupt(fighter_gobj)))
    {
        s32 status_id = ftCommonWalkGetWalkStatus(ABS(fp->input.pl.stick_range.x)) + (nFTDonkeyStatusThrowFWalkSlow - nFTCommonStatusWalkSlow);

        if (status_id != fp->status_id)
        {
            f32 div = ftDonkeyThrowFWalkGetWalkAnimLength(fp, fp->status_id);
            f32 mul = ftDonkeyThrowFWalkGetWalkAnimLength(fp, status_id);

            ftDonkeyThrowFWalkSetStatusParam(fighter_gobj, (s32) (mul * (fighter_gobj->anim_frame / div)));
        }
    }
}

// 0x8014D68C
void ftDonkeyThrowFWalkSetStatusParam(GObj *fighter_gobj, f32 frame_begin)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, ftCommonWalkGetWalkStatus(fp->input.pl.stick_range.x) + (nFTDonkeyStatusThrowFWalkSlow - nFTCommonStatusWalkSlow), frame_begin, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
}

// 0x8014D6D8
void ftDonkeyThrowFWalkSetStatusDefault(GObj *fighter_gobj)
{
    ftDonkeyThrowFWalkSetStatusParam(fighter_gobj, 0.0F);
}

// 0x8014D6F8
sb32 ftDonkeyThrowFWalkCheckInterruptThrowFWait(GObj *fighter_gobj)
{
    if (ftCommonWalkCheckInputSuccess(fighter_gobj) != FALSE)
    {
        ftDonkeyThrowFWalkSetStatusDefault(fighter_gobj);

        return TRUE;
    }
    else return FALSE;
}
