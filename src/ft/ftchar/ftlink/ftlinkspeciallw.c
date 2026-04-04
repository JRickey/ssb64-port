#include <ft/fighter.h>
#include <it/item.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x801643D0
void ftLinkSpecialLwMakeBomb(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    Vec3f pos;
    Vec3f vel;

    if (fp->motion_vars.flags.flag0 != 0)
    {
        fp->motion_vars.flags.flag0 = 0;

        pos = DObjGetStruct(fighter_gobj)->translate.vec.f;

        vel.x = 0.0F;
        vel.y = 0.0F;
        vel.z = 0.0F;

        itLinkBombMakeItem(fighter_gobj, &pos, &vel);
    }
}

// 0x80164434
void ftLinkSpecialLwProcUpdate(GObj *fighter_gobj)
{
    ftLinkSpecialLwMakeBomb(fighter_gobj);
    ftAnimEndSetWait(fighter_gobj);
}

// 0x8016445C
void ftLinkSpecialAirLwProcUpdate(GObj *fighter_gobj)
{
    ftLinkSpecialLwMakeBomb(fighter_gobj);
    ftAnimEndSetFall(fighter_gobj);
}

// 0x80164484
void ftLinkSpecialLwProcMap(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (mpCommonCheckFighterOnFloor(fighter_gobj) == FALSE)
    {
        mpCommonSetFighterAir(fp);
        ftMainSetStatus(fighter_gobj, nFTLinkStatusSpecialAirLw, fighter_gobj->anim_frame, 1.0F, FTSTATUS_PRESERVE_NONE);
    }
}

// 0x801644D4
void ftLinkSpecialAirLwProcMap(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (mpCommonCheckFighterLanding(fighter_gobj) != FALSE)
    {
        mpCommonSetFighterGround(fp);
        ftMainSetStatus(fighter_gobj, nFTLinkStatusSpecialLw, fighter_gobj->anim_frame, 1.0F, FTSTATUS_PRESERVE_NONE);
    }
}

// 0x80164524
sb32 ftLinkSpecialLwCheckGotoItemThrow(GObj *fighter_gobj, sb32 is_ground)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->item_gobj != NULL)
    {
        ITStruct *ip = itGetStruct(fp->item_gobj);

        if (ip->kind == nITKindLinkBomb)
        {
            if (is_ground == TRUE)
            {
                ftCommonItemThrowSetStatus(fighter_gobj, nFTCommonStatusLightThrowF4);

                return TRUE;
            }
            else ftCommonItemThrowSetStatus(fighter_gobj, nFTCommonStatusLightThrowAirF4);
        }
        return TRUE;
    }
    return FALSE;
}

// 0x8016458C
void ftLinkSpecialLwSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (ftLinkSpecialLwCheckGotoItemThrow(fighter_gobj, TRUE) != TRUE)
    {
        fp->motion_vars.flags.flag0 = 0;

        ftMainSetStatus(fighter_gobj, nFTLinkStatusSpecialLw, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
        ftMainPlayAnimEventsAll(fighter_gobj);
    }
}

// 0x801645E8
void ftLinkSpecialAirLwSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (ftLinkSpecialLwCheckGotoItemThrow(fighter_gobj, FALSE) != TRUE)
    {
        fp->motion_vars.flags.flag0 = 0;

        ftMainSetStatus(fighter_gobj, nFTLinkStatusSpecialAirLw, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
        ftMainPlayAnimEventsAll(fighter_gobj);
    }
}
