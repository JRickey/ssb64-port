#include <ft/fighter.h>
#include <it/item.h>
#include <reloc_data.h>

// 0x8014D0F0
void ftCommonCaptureCaptainUpdatePositions(GObj *fighter_gobj, GObj *capture_gobj, Vec3f *pos)
{
    // 0x80188A20
    Vec3f offset = { 0.0F, 0.0F, 0.0F };

    FTStruct *this_fp = ftGetStruct(fighter_gobj);
    FTStruct *capture_fp = ftGetStruct(capture_gobj);
    Vec2h *offset_add = lbRelocGetFileData(Vec2h*, gFTDataCaptainMainMotion, &llCaptainMainMotionSpecialHiVec2h);
    s32 unused;

    pos->x = 0.0F;
    pos->y = 0.0F;
    pos->z = 0.0F;

    gmCollisionGetFighterPartsWorldPosition(this_fp->joints[29], pos);
    gmCollisionGetFighterPartsWorldPosition(capture_fp->joints[nFTPartsJointTopN], &offset);

    offset.x += (offset_add[capture_fp->fkind].x * this_fp->lr);
    offset.y += offset_add[capture_fp->fkind].y;

    syVectorSub3D(pos, &offset);
}

// 0x8014D200
void ftCommonCaptureCaptainProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    Vec3f pos;

    if (fp->status_vars.common.capturecaptain.capture_flag & FTCOMMON_CAPTURECAPTAIN_MASK_THROW)
    {
        ftCommonCaptureCaptainRelease(fighter_gobj);
    }
    else if (!(fp->status_vars.common.capturecaptain.capture_flag & FTCOMMON_CAPTURECAPTAIN_MASK_NOUPDATE))
    {
        ftCommonCaptureCaptainUpdatePositions(fp->capture_gobj, fighter_gobj, &pos);

        if (syVectorMag3D(&pos) > 180.0F)
        {
            syVectorNorm3D(&pos);
            syVectorScale3D(&pos, 180.0F);
        }
        syVectorAdd3D(&DObjGetStruct(fighter_gobj)->translate.vec.f, &pos);
    }
}

// 0x8014D2A0
void ftCommonCaptureCaptainProcCapture(GObj *fighter_gobj, GObj *capture_gobj)
{
    FTStruct *this_fp = ftGetStruct(fighter_gobj);
    FTStruct *capture_fp;

    ftParamStopVoiceRunProcDamage(fighter_gobj);

    if ((this_fp->item_gobj != NULL) && (itGetStruct(this_fp->item_gobj)->weight == nITWeightHeavy))
    {
        ftSetupDropItem(this_fp);
    }
    if (this_fp->catch_gobj != NULL)
    {
        ftCommonThrownSetStatusDamageRelease(this_fp->catch_gobj);

        this_fp->catch_gobj = NULL;
    }
    this_fp->capture_gobj = capture_gobj;

    capture_fp = ftGetStruct(capture_gobj);

    this_fp->lr = -capture_fp->lr;

    this_fp->status_vars.common.capturecaptain.capture_flag = 0;

    if (this_fp->ga != nMPKineticsAir)
    {
        this_fp->status_vars.common.capturecaptain.capture_flag |= FTCOMMON_CAPTURECAPTAIN_MASK_NOUPDATE;
        this_fp->is_catch_or_capture = TRUE;
    }
    else this_fp->is_catch_or_capture = FALSE;

    mpCommonSetFighterAir(this_fp);
    ftMainSetStatus(fighter_gobj, nFTCommonStatusCaptureCaptain, FTCOMMON_CAPTURECAPTAIN_FRAME_BEGIN, FTCOMMON_CAPTURECAPTAIN_ANIM_SPEED, FTSTATUS_PRESERVE_NONE);
    ftParamSetCaptureImmuneMask(this_fp, FTCATCHKIND_MASK_ALL);
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftPhysicsStopVelAll(fighter_gobj);
    ftCommonCaptureCaptainProcPhysics(fighter_gobj);
}

// 0x8014D3BC
void ftCommonCaptureCaptainRelease(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftCommonThrownReleaseThrownUpdateStats(fighter_gobj, fp->lr, 0, 0);
    ftParamSetCaptureImmuneMask(fp, FTCATCHKIND_MASK_NONE);
}
