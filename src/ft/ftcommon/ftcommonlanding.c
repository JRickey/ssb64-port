#include <ft/fighter.h>
// // // // // // // // // // // //
//                               //
//             MACROS            //
//                               //
// // // // // // // // // // // //

#define ftCommonLandingCheckInterrupt(fighter_gobj)                     \
(                                                                       \
    (ftCommonSpecialNCheckInterruptCommon(fighter_gobj) != FALSE)   ||  \
    (ftCommonSpecialHiCheckInterruptCommon(fighter_gobj) != FALSE)  ||  \
    (ftCommonSpecialLwCheckInterruptCommon(fighter_gobj) != FALSE)  ||  \
    (ftCommonCatchCheckInterruptCommon(fighter_gobj) != FALSE)      ||  \
    (ftCommonAttackS4CheckInterruptCommon(fighter_gobj) != FALSE)   ||  \
    (ftCommonAttackHi4CheckInterruptCommon(fighter_gobj) != FALSE)  ||  \
    (ftCommonAttackLw4CheckInterruptCommon(fighter_gobj) != FALSE)  ||  \
    (ftCommonAttackS3CheckInterruptCommon(fighter_gobj) != FALSE)   ||  \
    (ftCommonAttackHi3CheckInterruptCommon(fighter_gobj) != FALSE)  ||  \
    (ftCommonAttackLw3CheckInterruptCommon(fighter_gobj) != FALSE)  ||  \
    (ftCommonAttack1CheckInterruptCommon(fighter_gobj) != FALSE)    ||  \
    (ftCommonGuardOnCheckInterruptCommon(fighter_gobj) != FALSE)    ||  \
    (ftCommonAppealCheckInterruptCommon(fighter_gobj) != FALSE)     ||  \
    (ftCommonKneeBendCheckInterruptCommon(fighter_gobj) != FALSE)   ||  \
    (ftCommonDashCheckInterruptCommon(fighter_gobj) != FALSE)       ||  \
    (ftCommonPassCheckInterruptCommon(fighter_gobj) != FALSE)       ||  \
    (ftCommonDokanStartCheckInterruptCommon(fighter_gobj) != FALSE)     \
)

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80142B70
void ftCommonLandingProcInterrupt(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fighter_gobj->anim_frame < FTCOMMON_LANDING_INTERRUPT_BEGIN)
    {
        return;
    }
    else if ((fp->status_vars.common.landing.is_allow_interrupt != FALSE) && !(ftCommonLandingCheckInterrupt(fighter_gobj)))
    {
        if ((fighter_gobj->anim_frame >= FTCOMMON_LANDING_INTERRUPT_BEGIN) && (fighter_gobj->anim_frame < (FTCOMMON_LANDING_INTERRUPT_BEGIN + DObjGetStruct(fighter_gobj)->anim_speed)))
        {
            if (ftCommonSquatWaitCheckInterruptLanding(fighter_gobj) != FALSE) return;
        }
        else if (ftCommonSquatCheckInterruptCommon(fighter_gobj) != FALSE) return;

        if (ftCommonTurnCheckInterruptCommon(fighter_gobj) == FALSE)
        {
            ftCommonWalkCheckInterruptCommon(fighter_gobj);
        }
    }
}

// 0x80142D44
void ftCommonLandingSetStatusParam(GObj *fighter_gobj, s32 status_id, sb32 is_allow_interrupt, f32 anim_speed)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    mpCommonSetFighterGround(fp);
    ftMainSetStatus(fighter_gobj, status_id, 0.0F, anim_speed, FTSTATUS_PRESERVE_NONE);

    fp->status_vars.common.landing.is_allow_interrupt = is_allow_interrupt;
}

// 0x80142D9C
void ftCommonLandingSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if ((fp->is_fastfall) && (fp->physics.vel_air.y <= -fp->attr->tvel_fast))
    {
        ftCommonLandingSetStatusParam(fighter_gobj, nFTCommonStatusLandingHeavy, TRUE, FTCOMMON_LANDING_HEAVY_ANIM_SPEED);
    }
    else ftCommonLandingSetStatusParam(fighter_gobj, nFTCommonStatusLandingLight, TRUE, FTCOMMON_LANDING_LIGHT_ANIM_SPEED);
}

// 0x80142E10
void ftCommonLandingAirNullSetStatus(GObj *fighter_gobj, f32 anim_speed)
{
    ftCommonLandingSetStatusParam(fighter_gobj, nFTCommonStatusLandingAirNull, FALSE, anim_speed);
}

// 0x80142E3C
void ftCommonLandingFallSpecialSetStatus(GObj *fighter_gobj, sb32 is_allow_interrupt, f32 anim_speed)
{
    ftCommonLandingSetStatusParam(fighter_gobj, nFTCommonStatusLandingFallSpecial, is_allow_interrupt, anim_speed);
}
