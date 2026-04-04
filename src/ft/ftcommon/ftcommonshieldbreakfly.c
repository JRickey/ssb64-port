#include <ft/fighter.h>
#include <sc/scene.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80149440
void ftCommonShieldBreakFlyProcUpdate(GObj *fighter_gobj)
{
    ftAnimEndCheckSetStatus(fighter_gobj, ftCommonShieldBreakFallSetStatus);
}

// 0x80149464
void ftCommonShieldBreakFlyProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterLanding(fighter_gobj, ftCommonShieldBreakDownSetStatus);
}

// 0x80149488
void ftCommonShieldBreakFlySetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    mpCommonSetFighterAir(fp);
    ftMainSetStatus(fighter_gobj, nFTCommonStatusShieldBreakFly, 0.0F, 1.0F, FTSTATUS_PRESERVE_DAMAGEPLAYER);
    ftMainPlayAnimEventsAll(fighter_gobj);

    fp->physics.vel_air.x = 0.0F;
    fp->physics.vel_air.y = attr->shield_break_vel_y;

    ftParamCheckSetFighterColAnimID(fighter_gobj, nGMColAnimFighterShieldBreakFly, 0);
    func_800269C0_275C0(nSYAudioFGMShieldBreak);
}

// 0x80149510
void ftCommonShieldBreakFlyCommonSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    Vec3f offset;

    offset.x = 0.0F;
    offset.y = 0.0F;
    offset.z = 0.0F;

    gmCollisionGetFighterPartsWorldPosition(fp->joints[nFTPartsJointYRotN], &offset);

    if (fp->fkind == nFTKindYoshi)
    {
        efManagerYoshiEggExplodeMakeEffect(&DObjGetStruct(fighter_gobj)->translate.vec.f);
        efManagerEggBreakMakeEffect(&DObjGetStruct(fighter_gobj)->translate.vec.f);
    }
    else efManagerShieldBreakMakeEffect(&offset);

    ftParamUpdate1PGameDamageStats(fp, fp->shield_player, nFTHitLogObjectNone, 0, 0, 0);

    if ((gSCManagerBattleState->game_type == nSCBattleGameType1PGame) && (fp->shield_damage != 0))
    {
        if ((fp->shield_player == gSCManagerSceneData.player) && (fp->shield_player != fp->player))
        {
            gSC1PGameBonusShieldBreaker = TRUE;
        }
    }
    ftCommonShieldBreakFlySetStatus(fighter_gobj);
}

// 0x80149608
void ftCommonShieldBreakFlyReflectorSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTSpecialColl *special_coll = fp->special_coll;
    Vec3f offset = special_coll->offset;

    gmCollisionGetFighterPartsWorldPosition(fp->joints[special_coll->joint_id], &offset);
    efManagerReflectBreakMakeEffect(&offset, fp->reflect_lr);
    ftCommonShieldBreakFlySetStatus(fighter_gobj);
}
