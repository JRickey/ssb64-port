#include <ft/fighter.h>
#include <wp/weapon.h>
#include <it/item.h>

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x80130E20 
sb32 (*sMPCommonProcPass)(GObj*);

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x800DD820 - new file?
sb32 mpCommonCheckSetFighterCliffEdge(GObj *fighter_gobj, s32 floor_line_id)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    MPObjectColl *map_coll = &fp->coll_data.map_coll;
    Vec3f *translate = fp->coll_data.p_translate;
    Vec3f edge_pos;
    Vec3f sp4C;
    Vec3f angle;
    u32 flags;
    f32 floor_dist;

    if (mpCollisionCheckExistLineID(floor_line_id) == FALSE)
    {
        return FALSE;
    }
    else
    {
        mpCollisionGetFloorEdgeL(floor_line_id, &edge_pos);

        if (translate->x <= edge_pos.x)
        {
            if ((fp->lr == -1) && (fp->input.pl.stick_range.x > -60))
            {
                edge_pos.x += 40.0F;

                mpCollisionGetFCCommonFloor(floor_line_id, &edge_pos, &floor_dist, &flags, &angle);

                edge_pos.y += floor_dist;
                sp4C.x = map_coll->width + edge_pos.x;
                sp4C.y = (map_coll->center + edge_pos.y) - map_coll->bottom;

                if (mpCollisionCheckLWallLineCollisionSame(&edge_pos, &sp4C, NULL, NULL, NULL, NULL) == FALSE)
                {
                    fp->lr = -1;

                    goto setground;
                }
            }
        }
        else if ((fp->lr == +1) && (fp->input.pl.stick_range.x < 60))
        {
            mpCollisionGetFloorEdgeR(floor_line_id, &edge_pos);

            edge_pos.x -= 40.0F;

            mpCollisionGetFCCommonFloor(floor_line_id, &edge_pos, &floor_dist, &flags, &angle);

            edge_pos.y += floor_dist;
            sp4C.x = edge_pos.x - map_coll->width;
            sp4C.y = (map_coll->center + edge_pos.y) - map_coll->bottom;

            if (mpCollisionCheckRWallLineCollisionSame(&edge_pos, &sp4C, NULL, NULL, NULL, NULL) == FALSE)
            {
                fp->lr = +1;

                goto setground;
            }
        }
    }
    return FALSE;

setground: // ???
    *translate = edge_pos; // This is what causes the infamous Dream Land teleport

    fp->coll_data.floor_line_id = floor_line_id;
    fp->coll_data.floor_flags = flags;
    fp->coll_data.floor_angle = angle;
    fp->coll_data.floor_dist = 0.0F;

    fp->coll_data.mask_stat |= MAP_FLAG_FLOOREDGE;

    return TRUE;
}

// 0x800DDA6C
sb32 mpCommonCheckSetFighterEdge(GObj *fighter_gobj, s32 floor_line_id)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    MPObjectColl *map_coll = &fp->coll_data.map_coll;
    Vec3f *translate = fp->coll_data.p_translate;
    Vec3f edge_pos;
    Vec3f sp4C;
    Vec3f sp40;
    Vec3f angle;
    u32 flags;
    f32 floor_dist;

    if (mpCollisionCheckExistLineID(floor_line_id) == FALSE)
    {
        return FALSE;
    }
    else
    {
        mpCollisionGetFloorEdgeL(floor_line_id, &edge_pos);

        if (translate->x <= edge_pos.x)
        {
            if (mpCollisionGetFCCommonFloor(floor_line_id, &edge_pos, &floor_dist, &flags, &angle));

            sp4C.x = edge_pos.x + 1.0F;
            sp4C.y = edge_pos.y + 1.0F;

            sp40.x = map_coll->width + edge_pos.x;
            sp40.y = (map_coll->center + edge_pos.y) - map_coll->bottom;

            if (mpCollisionCheckLWallLineCollisionSame(&sp4C, &sp40, NULL, NULL, NULL, NULL) == FALSE)
            {
                if (TRUE) goto setground;
            }
        }
        else
        {
            mpCollisionGetFloorEdgeR(floor_line_id, &edge_pos);

            if (mpCollisionGetFCCommonFloor(floor_line_id, &edge_pos, &floor_dist, &flags, &angle));

            sp4C.x = edge_pos.x - 1.0F;
            sp4C.y = edge_pos.y + 1.0F;

            sp40.x = edge_pos.x - map_coll->width;
            sp40.y = (map_coll->center + edge_pos.y) - map_coll->bottom;

            if (mpCollisionCheckRWallLineCollisionSame(&sp4C, &sp40, NULL, NULL, NULL, NULL) == FALSE)
            {
                if (TRUE) goto setground;
            }
        }
    }
    return FALSE;

setground: // Absolutely ridiculous match
    *translate = edge_pos;

    fp->coll_data.floor_line_id = floor_line_id;
    fp->coll_data.floor_flags = flags;
    fp->coll_data.floor_angle = angle;
    fp->coll_data.floor_dist = 0.0F;

    return TRUE;
}

// 0x800DDC50
sb32 mpCommonRunFighterAllCollisions(MPCollData *coll_data, GObj *fighter_gobj, u32 flags)
{
    s32 floor_line_id = coll_data->floor_line_id;   // Floor collision line ID
    sb32 is_floor = FALSE;                          // Whether fighter is making contact with a floor collision

    if (mpProcessCheckTestLWallCollision(coll_data) != FALSE)
    {
        mpProcessRunLWallCollision(coll_data);

        coll_data->is_coll_end = TRUE;
    }
    if (mpProcessCheckTestRWallCollision(coll_data) != FALSE)
    {
        mpProcessRunRWallCollision(coll_data);

        coll_data->is_coll_end = TRUE;
    }
    if (mpProcessCheckTestFloorCollisionNew(coll_data) != FALSE)
    {
        if (coll_data->mask_stat & MAP_FLAG_FLOOR)
        {
            mpProcessRunFloorEdgeAdjust(coll_data);

            is_floor = TRUE;
        }
    }
    else if (flags & MAP_PROC_TYPE_CLIFFEDGE)
    {
        mpCommonCheckSetFighterCliffEdge(fighter_gobj, floor_line_id);

        coll_data->is_coll_end = TRUE;
    }
    else if (flags & MAP_PROC_TYPE_STOPEDGE)
    {
        if (mpCommonCheckSetFighterEdge(fighter_gobj, floor_line_id) != FALSE)
        {
            is_floor = TRUE;
        }
        else coll_data->is_coll_end = TRUE;
    }
    else coll_data->is_coll_end = TRUE;

    if (mpProcessCheckTestFloorCollision(coll_data, floor_line_id) != FALSE)
    {
        mpProcessSetLandingFloor(coll_data);

        if (coll_data->mask_stat & MAP_FLAG_FLOOR)
        {
            mpProcessRunFloorEdgeAdjust(coll_data);

            is_floor = TRUE;
        }
        coll_data->mask_stat &= ~MAP_FLAG_FLOOREDGE;
        coll_data->is_coll_end = FALSE;
    }
    return is_floor;
}

// 0x800DDDA8
sb32 mpCommonCheckFighterOnFloor(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    return mpProcessUpdateMain(&fp->coll_data, mpCommonRunFighterAllCollisions, fighter_gobj, MAP_PROC_TYPE_DEFAULT);
}

// 0x800DDDDC
sb32 mpCommonProcFighterOnFloor(GObj *fighter_gobj, void (*proc_map)(GObj*))
{
    if (mpCommonCheckFighterOnFloor(fighter_gobj) == FALSE)
    {
        proc_map(fighter_gobj);

        return FALSE;
    }
    else return TRUE;
}

// 0x800DDE1C
sb32 mpCommonCheckFighterOnCliffEdge(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    return mpProcessUpdateMain(&fp->coll_data, mpCommonRunFighterAllCollisions, fighter_gobj, MAP_PROC_TYPE_CLIFFEDGE);
}

// 0x800DDE50
sb32 mpCommonCheckFighterOnEdge(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    return mpProcessUpdateMain(&fp->coll_data, mpCommonRunFighterAllCollisions, fighter_gobj, MAP_PROC_TYPE_STOPEDGE);
}

// 0x800DDE84
sb32 mpCommonProcFighterOnEdge(GObj *fighter_gobj, void (*proc_map)(GObj*))
{
    if (mpCommonCheckFighterOnEdge(fighter_gobj) == FALSE)
    {
        proc_map(fighter_gobj);

        return FALSE;
    }
    else return TRUE;
}

// 0x800DDEC4
void mpCommonSetFighterFallOnGroundBreak(GObj *fighter_gobj)
{
    mpCommonProcFighterOnFloor(fighter_gobj, ftCommonFallSetStatus);
}

// 0x800DDEE8
void mpCommonProcFighterOnCliffEdge(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (mpCommonCheckFighterOnCliffEdge(fighter_gobj) == FALSE)
    {
        if (fp->coll_data.mask_stat & MAP_FLAG_FLOOREDGE)
        {
            ftCommonOttottoSetStatus(fighter_gobj);
        }
        else ftCommonFallSetStatus(fighter_gobj);
    }
}

// 0x800DDF44
void mpCommonSetFighterFallOnEdgeBreak(GObj *fighter_gobj)
{
    if (mpCommonCheckFighterOnEdge(fighter_gobj) == FALSE)
    {
        ftCommonFallSetStatus(fighter_gobj);
    }
}

// 0x800DDF74
sb32 func_ovl2_800DDF74(GObj *fighter_gobj, FTStruct *fp, FTAttributes *attr, DObj *target_joint, Vec3f *vec)
{
    Vec3f sp64;
    Vec3f vec_translate;
    Vec3f sp4C;
    f32 sp48;
    DObj *joint;
    f32 tangent;
    u32 sp3C;
    f32 ternary;
    f32 translate_y;

    if (mpCollisionGetFCCommonFloor(fp->coll_data.floor_line_id, vec, &sp48, &sp3C, &sp64) != FALSE)
    {
        translate_y = (vec->y + sp48) - DObjGetStruct(fighter_gobj)->translate.vec.f.y;
    }
    else
    {
        mpCollisionGetFloorEdgeL(fp->coll_data.floor_line_id, &vec_translate);

        if (vec_translate.x < vec->x)
        {
            mpCollisionGetFloorEdgeR(fp->coll_data.floor_line_id, &vec_translate);
        }
        translate_y = vec_translate.y - DObjGetStruct(fighter_gobj)->translate.vec.f.y;
    }

    ternary = ABSF(translate_y);

    if (ternary < 0.0001F)
    {
        return FALSE;
    }

    vec->y += translate_y;

    joint = DObjGetStruct(fighter_gobj);

    if (vec->y < joint->translate.vec.f.y)
    {
        ternary = ((joint->translate.vec.f.x < vec->x) ? -(joint->translate.vec.f.x - vec->x) : (joint->translate.vec.f.x - vec->x));

        tangent = DObjGetStruct(fighter_gobj)->translate.vec.f.y - ternary * syUtilsTan(attr->unk_0x320);

        if (vec->y < tangent)
        {
            vec->y = tangent;
        }
    }

    sp4C.x = sp4C.y = sp4C.z = 0;

    gmCollisionGetFighterPartsWorldPosition(target_joint, &sp4C);

    if (vec->y > sp4C.y - attr->unk_0x31C)
    {
        vec->y = sp4C.y - attr->unk_0x31C;
    }
    return TRUE;
}

// 0x800DE150
void mpCommonUpdateFighterSlopeContour(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;
    DObj *joint;
    Vec3f sp30;
    f32 sp2C;

    if (fp->ga == nMPKineticsGround)
    {
        if ((fp->coll_data.floor_line_id != -1) && (fp->coll_data.floor_line_id != -2) && (fp->hitlag_tics <= 0))
        {
            if (fp->slope_contour & FTSLOPECONTOUR_FLAG_RFOOT)
            {
                joint = fp->joints[attr->joint_rfoot_id];

                func_ovl2_800EBC0C(fp, &sp30, &sp2C, attr->joint_rfoot_rotate, joint);

                if (func_ovl2_800DDF74(fighter_gobj, fp, attr, joint, &sp30) != FALSE)
                {
                    func_ovl2_800EE018(fp->joints[attr->joint_rfoot_id], &sp30);
                    func_ovl2_800EBD08(fp->joints[attr->joint_rfoot_id], attr->joint_rfoot_rotate, &sp30, sp2C);
                }
            }
            if (fp->slope_contour & FTSLOPECONTOUR_FLAG_LFOOT)
            {
                joint = fp->joints[attr->joint_lfoot_id];

                func_ovl2_800EBC0C(fp, &sp30, &sp2C, attr->joint_lfoot_rotate, joint);

                if (func_ovl2_800DDF74(fighter_gobj, fp, attr, joint, &sp30) != FALSE)
                {
                    func_ovl2_800EE018(fp->joints[attr->joint_lfoot_id], &sp30);
                    func_ovl2_800EBD08(fp->joints[attr->joint_lfoot_id], attr->joint_lfoot_rotate, &sp30, sp2C);
                }
            }
            if (fp->slope_contour & FTSLOPECONTOUR_FLAG_FULL)
            {
                DObjGetStruct(fighter_gobj)->rotate.vec.f.x = (syUtilsArcTan2(fp->coll_data.floor_angle.x, fp->coll_data.floor_angle.y) * fp->lr);
            }
        }
    }
    else return;
}

// 0x800DE324
void mpCommonSetFighterProjectFloor(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    mpProcessSetCollProjectFloorID(&fp->coll_data);
}

// 0x800DE348
void mpCommonUpdateFighterProjectFloor(GObj *fighter_gobj)
{
    mpCommonSetFighterProjectFloor(fighter_gobj);
}

// 0x800DE368
void mpCommonSetFighterLandingParams(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->public_knockback != 0.0F)
    {
        if (fp->public_knockback >= 100.0F)
        {
            if 
            (
                (fp->joints[nFTPartsJointTopN]->translate.vec.f.x < (gMPCollisionBounds.current.left + 450.0F)) || 
                (fp->joints[nFTPartsJointTopN]->translate.vec.f.x > (gMPCollisionBounds.current.right - 450.0F))
            )
            {
                ftPublicPlayCliffReact(fighter_gobj, fp->public_knockback);
            }   
        }
        fp->public_knockback = 0.0F;
    }
    switch (fp->fkind)
    {
    case nFTKindMario:
    case nFTKindMMario:
    case nFTKindNMario:
        fp->passive_vars.mario.is_expend_tornado = FALSE;
        break;

    case nFTKindSamus:
    case nFTKindNSamus:
        fp->passive_vars.samus.charge_recoil = 0;
        return;

    case nFTKindLuigi:
    case nFTKindNLuigi:
        fp->passive_vars.mario.is_expend_tornado = FALSE;
        break;

    case nFTKindCaptain:
    case nFTKindNCaptain:
        fp->passive_vars.captain.falcon_punch_unk = FALSE;
        break;

    case nFTKindPurin:
    case nFTKindNPurin:
        fp->passive_vars.purin.unk_0x0 = FALSE;
        break;
    }
}

// 0x800DE45C
sb32 mpCommonRunFighterSpecialCollisions(MPCollData *coll_data, GObj *fighter_gobj, u32 flags)
{
    FTStruct *this_fp = ftGetStruct(fighter_gobj);
    GObj *cliffcatch_gobj;
    FTStruct *cliffcatch_fp;
    s32 unused;
    sb32 is_ceilstop = FALSE;
    sb32 is_collide;

    if (mpProcessCheckTestLWallCollisionAdjNew(coll_data) != FALSE)
    {
        mpProcessRunLWallCollisionAdjNew(coll_data);
    }
    if (mpProcessCheckTestRWallCollisionAdjNew(coll_data) != FALSE)
    {
        mpProcessRunRWallCollisionAdjNew(coll_data);
    }
    if (mpProcessCheckTestCeilCollisionAdjNew(coll_data) != FALSE)
    {
        mpProcessRunCeilCollisionAdjNew(coll_data);

        if (coll_data->mask_stat & MAP_FLAG_CEIL)
        {
            mpProcessRunCeilEdgeAdjust(coll_data);
        }
        if ((flags & MAP_PROC_TYPE_CEILHEAVY) && (this_fp->physics.vel_air.y >= 30.0F))
        {
            coll_data->mask_curr |= MAP_FLAG_CEILHEAVY;

            is_ceilstop = TRUE;

            coll_data->is_coll_end = TRUE;
        }
    }
                                         is_collide 
                             
                                            =
        
                            (flags & MAP_PROC_TYPE_PASS) 

                                            ? 
        
    mpProcessCheckTestFloorCollisionAdjNew(coll_data, sMPCommonProcPass, fighter_gobj) 
        
                                            :
        
                    mpProcessRunFloorCollisionAdjNewNULL(coll_data);

    if (is_collide != FALSE)
    {
        if (flags & MAP_PROC_TYPE_PROJECT)
        {
            mpProcessSetCollideFloor(coll_data);

            if (coll_data->mask_stat & MAP_FLAG_FLOOR)
            {
                mpProcessRunFloorEdgeAdjust(coll_data);
            }
            else mpProcessSetCollProjectFloorID(coll_data);
        }
        else
        {
            mpProcessSetLandingFloor(coll_data);
            mpCommonSetFighterLandingParams(fighter_gobj);

            if (coll_data->mask_stat & MAP_FLAG_FLOOR)
            {
                mpProcessRunFloorEdgeAdjust(coll_data);

                coll_data->is_coll_end = TRUE;

                return TRUE;
            }
        }
    }
    else mpProcessSetCollProjectFloorID(coll_data);

    if ((flags & MAP_PROC_TYPE_CLIFF) && (this_fp->cliffcatch_wait == 0))
    {
        if ((mpProcessCheckTestLCliffCollision(coll_data) != FALSE) || (mpProcessCheckTestRCliffCollision(coll_data) != FALSE))
        {
            cliffcatch_gobj = gGCCommonLinks[nGCCommonLinkIDFighter];

            while (cliffcatch_gobj != NULL)
            {
                if (cliffcatch_gobj != fighter_gobj)
                {
                    cliffcatch_fp = ftGetStruct(cliffcatch_gobj);

                    if ((cliffcatch_fp->is_cliff_hold) && (this_fp->coll_data.cliff_id == cliffcatch_fp->coll_data.cliff_id) && (this_fp->lr == cliffcatch_fp->lr))
                    {
                        return is_ceilstop;
                    }
                    else goto next_gobj; // Bruh
                }
            next_gobj:
                cliffcatch_gobj = cliffcatch_gobj->link_next;
            }
            mpCommonSetFighterLandingParams(fighter_gobj);

            coll_data->is_coll_end = TRUE;

            return TRUE;
        }
    }
    return is_ceilstop;
}

// 0x800DE6B0
sb32 mpCommonCheckFighterLanding(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    return mpProcessUpdateMain(&fp->coll_data, mpCommonRunFighterSpecialCollisions, fighter_gobj, MAP_PROC_TYPE_DEFAULT);
}

// 0x800DE6E4
sb32 mpCommonProcFighterLanding(GObj *fighter_gobj, void(*proc_map)(GObj*))
{
    if (mpCommonCheckFighterLanding(fighter_gobj) != FALSE)
    {
        proc_map(fighter_gobj);

        return TRUE;
    }
    else return FALSE;
}

// 0x800DE724 - keep fighter airborne but project onto ground?
sb32 mpCommonCheckFighterProject(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    return mpProcessUpdateMain(&fp->coll_data, mpCommonRunFighterSpecialCollisions, fighter_gobj, MAP_PROC_TYPE_PROJECT);
}

// 0x800DE758
sb32 mpCommonCheckFighterPass(GObj *fighter_gobj, sb32(*proc_map)(GObj*))
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    sMPCommonProcPass = proc_map;

    return mpProcessUpdateMain(&fp->coll_data, mpCommonRunFighterSpecialCollisions, fighter_gobj, MAP_PROC_TYPE_PASS);
}

// 0x800DE798
sb32 mpCommonCheckFighterPassCliff(GObj *fighter_gobj, sb32(*proc_map)(GObj*))
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    sMPCommonProcPass = proc_map;

    return mpProcessUpdateMain(&fp->coll_data, mpCommonRunFighterSpecialCollisions, fighter_gobj, MAP_PROC_TYPE_PASS | MAP_PROC_TYPE_CLIFF);
}

// 0x800DE7D8
sb32 mpCommonCheckFighterCliff(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    return mpProcessUpdateMain(&fp->coll_data, mpCommonRunFighterSpecialCollisions, fighter_gobj, MAP_PROC_TYPE_CLIFF);
}

// Check if fighter becomes grounded, allow CliffCatch interrupt
sb32 mpCommonProcFighterCliff(GObj *fighter_gobj, void (*proc_map)(GObj*))
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (mpCommonCheckFighterCliff(fighter_gobj) != FALSE)
    {
        if (fp->coll_data.mask_stat & MAP_FLAG_CLIFF_MASK)
        {
            ftCommonCliffCatchSetStatus(fighter_gobj);

            return TRUE;
        }
        else proc_map(fighter_gobj);

        return TRUE;
    }
    else return FALSE;
}

// 0x800DE87C
sb32 mpCommonCheckFighterCeilHeavyCliff(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    return mpProcessUpdateMain(&fp->coll_data, mpCommonRunFighterSpecialCollisions, fighter_gobj, MAP_PROC_TYPE_CEILHEAVY | MAP_PROC_TYPE_CLIFF);
}

// 0x800DE8B0
sb32 mpCommonCheckFighterCeilHeavy(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    return mpProcessUpdateMain(&fp->coll_data, mpCommonRunFighterSpecialCollisions, fighter_gobj, MAP_PROC_TYPE_CEILHEAVY);
}

// 0x800DE8E4
void mpCommonSetFighterWaitOrLanding(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->physics.vel_air.y > FTCOMMON_ATTACKAIR_SKIPLANDING_VEL_Y_MAX)
    {
        ftCommonWaitSetStatus(fighter_gobj);
    }
    else ftCommonLandingSetStatus(fighter_gobj);
}

// 0x800DE934
void mpCommonProcFighterWaitOrLanding(GObj *fighter_gobj)
{
    mpCommonProcFighterLanding(fighter_gobj, mpCommonSetFighterWaitOrLanding);
}

// 0x800DE958
void mpCommonProcFighterProject(GObj *fighter_gobj)
{
    mpCommonCheckFighterProject(fighter_gobj);
}

// 0x800DE978
void mpCommonProcFighterCliffWaitOrLanding(GObj *fighter_gobj)
{
    mpCommonProcFighterCliff(fighter_gobj, mpCommonSetFighterWaitOrLanding);
}

// 0x800DE99C
void mpCommonProcFighterCliffFloorCeil(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (mpCommonCheckFighterCeilHeavyCliff(fighter_gobj) != FALSE)
    {
        if (fp->coll_data.mask_stat & MAP_FLAG_CLIFF_MASK)
        {
            ftCommonCliffCatchSetStatus(fighter_gobj);
        }
        else if (fp->coll_data.mask_stat & MAP_FLAG_FLOOR)
        {
            mpCommonSetFighterWaitOrLanding(fighter_gobj);
        }
        else if (fp->coll_data.mask_curr & MAP_FLAG_CEILHEAVY) // Enter ceiling bonk if true?
        {
            ftCommonStopCeilSetStatus(fighter_gobj);
        }
    }
}

// 0x800DEA20
sb32 mpCommonProcFighterDamage(MPCollData *coll_data, GObj *fighter_gobj, u32 flags)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    sb32 is_collide = FALSE;

    if (mpProcessCheckTestLWallCollisionAdjNew(coll_data) != FALSE)
    {
        mpProcessRunLWallCollisionAdjNew(coll_data);

        if (!(fp->status_vars.common.damage.coll_mask_prev & MAP_FLAG_LWALL) && (lbCommonMag2D(&coll_data->pos_diff) > 30.0F) && (syVectorAngleDiff3D(&coll_data->pos_diff, &coll_data->lwall_angle) > F_CLC_DTOR32(110.0F))) // 1.9198622F
        {
            fp->status_vars.common.damage.coll_mask_curr |= MAP_FLAG_LWALL;

            is_collide = TRUE;

            coll_data->is_coll_end = TRUE;
        }
        else if (!(coll_data->mask_prev & MAP_FLAG_LWALL))
        {
            fp->status_vars.common.damage.coll_mask_ignore |= MAP_FLAG_LWALL;
        }
    }
    if (mpProcessCheckTestRWallCollisionAdjNew(coll_data) != FALSE)
    {
        mpProcessRunRWallCollisionAdjNew(coll_data);

        if (!(fp->status_vars.common.damage.coll_mask_prev & MAP_FLAG_RWALL) && (lbCommonMag2D(&coll_data->pos_diff) > 30.0F) && (syVectorAngleDiff3D(&coll_data->pos_diff, &coll_data->rwall_angle) > F_CLC_DTOR32(110.0F))) // 1.9198622F
        {
            fp->status_vars.common.damage.coll_mask_curr |= MAP_FLAG_RWALL;

            is_collide = TRUE;

            coll_data->is_coll_end = TRUE;
        }
        else if (!(coll_data->mask_prev & MAP_FLAG_RWALL))
        {
            fp->status_vars.common.damage.coll_mask_ignore |= MAP_FLAG_RWALL;
        }
    }
    if (mpProcessCheckTestCeilCollisionAdjNew(coll_data) != FALSE)
    {
        mpProcessRunCeilCollisionAdjNew(coll_data);

        if (coll_data->mask_stat & MAP_FLAG_CEIL)
        {
            mpProcessRunCeilEdgeAdjust(coll_data);
        }
        if (!(fp->status_vars.common.damage.coll_mask_prev & MAP_FLAG_CEIL) && (lbCommonMag2D(&coll_data->pos_diff) > 30.0F) && (syVectorAngleDiff3D(&coll_data->pos_diff, &coll_data->ceil_angle) > F_CLC_DTOR32(110.0F)))
        {
            fp->status_vars.common.damage.coll_mask_curr |= MAP_FLAG_CEIL;

            is_collide = TRUE;

            coll_data->is_coll_end = TRUE;
        }
        else if (!(coll_data->mask_prev & MAP_FLAG_CEIL))
        {
            fp->status_vars.common.damage.coll_mask_ignore |= MAP_FLAG_CEIL;
        }
    }
    if (mpProcessRunFloorCollisionAdjNewNULL(coll_data) != FALSE)
    {
        if (fp->hitlag_tics > 0)
        {
            mpProcessSetCollideFloor(coll_data);

            if (coll_data->mask_stat & MAP_FLAG_FLOOR)
            {
                mpProcessRunFloorEdgeAdjust(coll_data);
            }
            else mpProcessSetCollProjectFloorID(coll_data);
        }
        else
        {
            if (syVectorAngleDiff3D(&coll_data->pos_diff, &coll_data->floor_angle) > F_CLC_DTOR32(110.0F))
            {
                mpProcessSetLandingFloor(coll_data);
                mpCommonSetFighterLandingParams(fighter_gobj);

                if (coll_data->mask_stat & MAP_FLAG_FLOOR)
                {
                    mpProcessRunFloorEdgeAdjust(coll_data);

                    fp->status_vars.common.damage.coll_mask_curr |= MAP_FLAG_FLOOR;

                    is_collide = TRUE;

                    coll_data->is_coll_end = TRUE;
                }
            }
            else
            {
                mpProcessSetCollideFloor(coll_data);

                if (coll_data->mask_stat & MAP_FLAG_FLOOR)
                {
                    mpProcessRunFloorEdgeAdjust(coll_data);

                    if (!(coll_data->mask_prev & MAP_FLAG_FLOOR))
                    {
                        fp->status_vars.common.damage.coll_mask_ignore |= MAP_FLAG_FLOOR;

                        fp->status_vars.common.damage.wall_collide_angle = coll_data->floor_angle;
                    }
                }
                else mpProcessSetCollProjectFloorID(coll_data);
            }
        }
    }
    else mpProcessSetCollProjectFloorID(coll_data);

    return is_collide;
}

// 0x800DEDAC
sb32 mpCommonCheckFighterDamageCollision(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->status_vars.common.damage.coll_mask_prev = fp->status_vars.common.damage.coll_mask_curr;
    fp->status_vars.common.damage.coll_mask_curr = 0;
    fp->status_vars.common.damage.coll_mask_ignore = 0;

    return mpProcessUpdateMain(&fp->coll_data, mpCommonProcFighterDamage, fighter_gobj, MAP_PROC_TYPE_DEFAULT);
}

// 0x800DEDF0
void mpCommonUpdateFighterKinetics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->ga == nMPKineticsAir)
    {
        if (mpCommonCheckFighterLanding(fighter_gobj) != FALSE)
        {
            mpCommonSetFighterGround(fp);
        }
    }
    else if (mpCommonCheckFighterOnFloor(fighter_gobj) == FALSE)
    {
        mpCommonSetFighterAir(fp);
    }
}

// 0x800DEE54
void mpCommonSetFighterWaitOrFall(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->ga == nMPKineticsAir)
    {
        ftCommonFallSetStatus(fighter_gobj);
    }
    else ftCommonWaitSetStatus(fighter_gobj);
}

// 0x800DEE98
void mpCommonSetFighterGround(FTStruct *fp)
{
    fp->physics.vel_ground.x = fp->physics.vel_air.x * fp->lr;

    fp->ga = nMPKineticsGround;

    fp->jumps_used = 0;

    fp->stat_flags.ga = nMPKineticsGround;
}

// 0x800DEEC8
void mpCommonSetFighterAir(FTStruct *fp)
{
    fp->ga = nMPKineticsAir;

    fp->physics.vel_air.z = fp->joints[nFTPartsJointTopN]->translate.vec.f.z = 0.0F;

    fp->jumps_used = 1;
}

// 0x800DEEF4
void mpCommonRunDefaultCollision(MPCollData *coll_data, GObj *gobj, u32 flags)
{
    if (mpProcessCheckTestLWallCollisionAdjNew(coll_data) != FALSE)
    {
        mpProcessRunLWallCollisionAdjNew(coll_data);
    }
    if (mpProcessCheckTestRWallCollisionAdjNew(coll_data) != FALSE)
    {
        mpProcessRunRWallCollisionAdjNew(coll_data);
    }
    if (mpProcessCheckTestCeilCollisionAdjNew(coll_data) != FALSE)
    {
        mpProcessRunCeilCollisionAdjNew(coll_data);

        if (coll_data->mask_stat & MAP_FLAG_CEIL)
        {
            mpProcessRunCeilEdgeAdjust(coll_data);
        }
    }
    if (mpProcessRunFloorCollisionAdjNewNULL(coll_data) != FALSE)
    {
        mpProcessSetCollideFloor(coll_data);

        if (coll_data->mask_stat & MAP_FLAG_FLOOR)
        {
            mpProcessRunFloorEdgeAdjust(coll_data);
        }
    }
    else mpProcessSetCollProjectFloorID(coll_data);
}

// 0x800DEFBC
void mpCommonCopyCollDataStats(MPCollData *this_coll_data, Vec3f *pos, MPCollData *other_coll_data)
{
    this_coll_data->pos_prev = *pos;

    this_coll_data->p_map_coll = &other_coll_data->map_coll;
    this_coll_data->mask_curr = 0;
    this_coll_data->mask_unk = 0;
    this_coll_data->mask_stat = 0;
    this_coll_data->is_coll_end = FALSE;
    this_coll_data->update_tic = other_coll_data->update_tic;
}

// 0x800DEFF8
void mpCommonResetCollDataStats(MPCollData *coll_data)
{
    coll_data->p_map_coll = &coll_data->map_coll;

    coll_data->update_tic = gMPCollisionUpdateTic;
    coll_data->mask_curr = 0;
}

// 0x800DF014
void mpCommonRunFighterCollisionDefault(GObj *fighter_gobj, Vec3f *pos, MPCollData *coll_data)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    mpCommonCopyCollDataStats(&fp->coll_data, pos, coll_data);
    mpCommonRunDefaultCollision(&fp->coll_data, fighter_gobj, MAP_PROC_TYPE_DEFAULT);
    mpCommonResetCollDataStats(&fp->coll_data);
}

// 0x800DF058
void mpCommonRunItemCollisionDefault(GObj *item_gobj, Vec3f *pos, MPCollData *coll_data)
{
    ITStruct *ip = itGetStruct(item_gobj);

    mpCommonCopyCollDataStats(&ip->coll_data, pos, coll_data);
    mpCommonRunDefaultCollision(&ip->coll_data, item_gobj, MAP_PROC_TYPE_DEFAULT);
    mpCommonResetCollDataStats(&ip->coll_data);
}

// 0x800DF09C
void mpCommonRunWeaponCollisionDefault(GObj *weapon_gobj, Vec3f *pos, MPCollData *coll_data)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    mpCommonCopyCollDataStats(&wp->coll_data, pos, coll_data);
    mpCommonRunDefaultCollision(&wp->coll_data, weapon_gobj, MAP_PROC_TYPE_DEFAULT);
    mpCommonResetCollDataStats(&wp->coll_data);
}
