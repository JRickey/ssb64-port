#include <ft/fighter.h>
// // // // // // // // // // // //
//                               //
//             MACROS            //
//                               //
// // // // // // // // // // // //

#define ftCommonOttottoCheckInterrupt(fighter_gobj)                     \
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
    (ftCommonDokanStartCheckInterruptCommon(fighter_gobj) != FALSE) ||  \
    (ftCommonSquatCheckInterruptCommon(fighter_gobj) != FALSE)      ||  \
    (ftCommonTurnCheckInterruptCommon(fighter_gobj) != FALSE)           \
)

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80142850
void ftCommonOttottoProcUpdate(GObj *fighter_gobj)
{
    ftAnimEndCheckSetStatus(fighter_gobj, ftCommonOttottoWaitSetStatus);
}

// 0x80142874
void ftCommonOttottoProcInterrupt(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (!(ftCommonOttottoCheckInterrupt(fighter_gobj)))
    {
        if ((fp->input.pl.stick_range.x * fp->lr) >= FTCOMMON_OTTOTTO_WALK_STICK_RANGE_MIN)
        {
            ftCommonWalkCheckInterruptCommon(fighter_gobj);
        }
    }
}

// 0x801429F4
void ftCommonOttottoProcMap(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    Vec3f pos;
    f32 dist_x;

    if (mpCommonCheckFighterOnFloor(fighter_gobj) == FALSE)
    {
        ftCommonFallSetStatus(fighter_gobj);
    }
    else
    {
        if (fp->lr == +1)
        {
            mpCollisionGetFloorEdgeR(fp->coll_data.floor_line_id, &pos);
        }
        else mpCollisionGetFloorEdgeL(fp->coll_data.floor_line_id, &pos);

        if (DObjGetStruct(fighter_gobj)->translate.vec.f.x < pos.x)
        {
            dist_x = -(DObjGetStruct(fighter_gobj)->translate.vec.f.x - pos.x);
        }
        else dist_x = DObjGetStruct(fighter_gobj)->translate.vec.f.x - pos.x;

        if (dist_x > FTCOMMON_OTTOTTO_WALK_DIST_X_MIN)
        {
            ftCommonWaitSetStatus(fighter_gobj);
        }
    }
}

// 0x80142AC4
void ftCommonOttottoWaitSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTCommonStatusOttottoWait, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);

    fp->is_special_interrupt = TRUE;
}

// 0x80142B08
void ftCommonOttottoSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTCommonStatusOttotto, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);

    fp->physics.vel_air.x = fp->physics.vel_air.y = fp->physics.vel_air.z = 0.0F;
    fp->physics.vel_ground.x = 0.0F;

    fp->is_special_interrupt = TRUE;
}
