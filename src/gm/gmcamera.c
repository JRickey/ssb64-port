#include <ft/fighter.h>
#include <gr/ground.h>
#include <wp/weapon.h>
#include <if/interface.h>
#include <sc/scene.h>
#include <sys/rdp.h>
// #include <sys/taskman.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x8012EB70
GCPersp dGMCameraPerspDefault = { NULL, 0, 38.0F, 15.0F / 11.0F, 256.0F, 39936.0F, 1.0F };

// 0x8012EB8C
CObjVec dGMCameraCObjVecDefault =
{
    NULL,
    { 1500.0F, 0.0F, 0.0F },
    {    0.0F, 0.0F, 0.0F },
    {    0.0F, 1.0F, 0.0F }
};

// 0x8012EBB4
void (*dGMCameraFuncList[/* */])(GObj*) =
{
    gmCameraDefaultFuncCamera,
    gmCameraPlayerZoomFuncCamera,
    gmCameraAnimFuncCamera,
    gmCameraInishieFuncCamera,
    gmCameraMapZoomFuncCamera,
    gmCameraPlayerFollowFuncCamera,
    gmCameraZebesFuncCamera
};

// 0x8012EBD0
f32 dGMCameraPlayerZoomRanges[/* */] =
{
    0.00F,  // No players
    1.50F,  // 1 Player
    1.32F,  // 2 Players
    1.16F,  // 3 Players
    1.00F   // 4 Players
};

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x80131460
GObj *gGMCameraGObj;

// 0x80131464
f32 gGMCameraPauseCameraEyeY;

// 0x80131468
f32 gGMCameraPauseCameraEyeX;

// 0x80131470
Mtx44f gGMCameraMatrix;

// 0x801314B0
GMCamera gGMCameraStruct;

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8010B810
u32 gmCameraGetBoundsMask(Vec3f *pos)
{
    u32 bounds = 0;

    if (pos->x < gMPCollisionGroundData->camera_bound_left)
    {
        bounds |= CAMERA_FLAG_BOUND_LEFT;
    }
    if (pos->x > gMPCollisionGroundData->camera_bound_right)
    {
        bounds |= CAMERA_FLAG_BOUND_RIGHT;
    }
    if (pos->y < gMPCollisionGroundData->camera_bound_bottom)
    {
        bounds |= CAMERA_FLAG_BOUND_BOTTOM;
    }
    if (pos->y > gMPCollisionGroundData->camera_bound_top)
    {
        bounds |= CAMERA_FLAG_BOUND_TOP;
    }
    return bounds;
}

// 0x8010B8BC
void gmCameraSetBoundsPosition(Vec3f *pos)
{
    while (TRUE)
    {
        u32 bounds = gmCameraGetBoundsMask(pos);

        if (bounds != 0)
        {
            if (bounds & CAMERA_FLAG_BOUND_LEFT)
            {
                pos->x = gMPCollisionGroundData->camera_bound_left;
            }
            else if (bounds & CAMERA_FLAG_BOUND_RIGHT)
            {
                pos->x = gMPCollisionGroundData->camera_bound_right;
            }
            else if (bounds & CAMERA_FLAG_BOUND_BOTTOM)
            {
                pos->y = gMPCollisionGroundData->camera_bound_bottom;
            }
            else if (bounds & CAMERA_FLAG_BOUND_TOP)
            {
                pos->y = gMPCollisionGroundData->camera_bound_top;
            }
        }
        else break;
    }
}

// 0x8010B98C
u32 gmCameraGetTeamBoundsMask(Vec3f *pos)
{
    u32 bounds = 0;

    if (pos->x < gMPCollisionGroundData->camera_bound_team_left)
    {
        bounds |= CAMERA_FLAG_BOUND_LEFT;
    }
    if (pos->x > gMPCollisionGroundData->camera_bound_team_right)
    {
        bounds |= CAMERA_FLAG_BOUND_RIGHT;
    }
    if (pos->y < gMPCollisionGroundData->camera_bound_team_bottom)
    {
        bounds |= CAMERA_FLAG_BOUND_BOTTOM;
    }
    if (pos->y > gMPCollisionGroundData->camera_bound_team_top)
    {
        bounds |= CAMERA_FLAG_BOUND_TOP;
    }
    return bounds;
}

// 0x8010BA38
void gmCameraSetTeamBoundsPosition(Vec3f *pos)
{
    while (TRUE)
    {
        u32 bounds = gmCameraGetTeamBoundsMask(pos);

        if (bounds != 0)
        {
            if (bounds & CAMERA_FLAG_BOUND_LEFT)
            {
                pos->x = gMPCollisionGroundData->camera_bound_team_left;
            }
            else if (bounds & CAMERA_FLAG_BOUND_RIGHT)
            {
                pos->x = gMPCollisionGroundData->camera_bound_team_right;
            }
            else if (bounds & CAMERA_FLAG_BOUND_BOTTOM)
            {
                pos->y = gMPCollisionGroundData->camera_bound_team_bottom;
            }
            else if (bounds & CAMERA_FLAG_BOUND_TOP)
            {
                pos->y = gMPCollisionGroundData->camera_bound_team_top;
            }
        }
        else break;
    }
}

// 0x8010BB08
void gmCameraSetDeadUpStarPosition(Vec3f *pos)
{
    pos->x = (gMPCollisionGroundData->camera_bound_top * pos->x) / gMPCollisionGroundData->map_bound_top;
    pos->y = gMPCollisionGroundData->camera_bound_top;
}

// 0x8010BB58
f32 gmCameraGetPlayerNumZoomRange(s32 players_num)
{
    f32 zoom = dGMCameraPlayerZoomRanges[players_num];

    if (gSCManagerBattleState->game_type == nSCBattleGameTypeExplain)
    {
        zoom *= 0.75F;
    }
    return zoom;
}

// 0x8010BB98
f32 gmCameraCalcFighterZoomRange(FTStruct *fp, f32 camera_zoom)
{
    camera_zoom *= fp->camera_zoom_frame;
    camera_zoom *= fp->camera_zoom_range;

    if ((fp->status_id == nFTCommonStatusWait) && (fp->status_total_tics >= 120))
    {
        camera_zoom *= 0.75F;
    }
    return camera_zoom;
}

// 0x8010BBE4
f32 gmCameraGetTargetAtY(f32 dist)
{
    if (dist > 2000.0F)
    {
        return 0.0682F;
    }
    else if (dist < 1000.0F)
    {
        return 0.0F;
    }
    else return ((dist - 1000.0F) / 1000) * 0.0682F;
}

// 0x8010BC54
void gmCameraUpdateInterests(Vec3f *vec, f32 *hz, f32 *vt)
{
    s32 players_num;
    s32 i;
    FTCamera cams[GMCOMMON_PLAYERS_MAX];
    FTStruct *fp;
    WPStruct *wp;
    f32 pos_top;
    f32 pos_left;
    f32 pos_bottom;
    f32 ft_right;
    f32 ft_top;
    f32 ft_left;
    f32 ft_bottom;
    f32 zoom;
    GObj *fighter_gobj;
    FTStruct *cam_fp;
    f32 wp_top;
    f32 wp_bottom;
    f32 wp_left;
    f32 wp_right;
    f32 adjust;
    f32 gm_top;
    f32 gm_bottom;
    f32 gm_left;
    f32 gm_right;
    f32 pos_right;
    GObj *weapon_gobj;
    s32 lr;
    Vec3f weapon_pos;

    fighter_gobj = gGCCommonLinks[nGCCommonLinkIDFighter];

    players_num = 0;

    while (fighter_gobj != NULL)
    {
        fp = ftGetStruct(fighter_gobj);

        switch (fp->camera_mode)
        {
        default:
            if (players_num >= ARRAY_COUNT(cams))
            {
                while (TRUE)
                {
                    syDebugPrintf("Player Num is Over for Camera!\n");
                    scManagerRunPrintGObjStatus();
                }
            }
            cams[players_num].target_fp = fp;

            switch (fp->camera_mode)
            {
            default:
                cams[players_num].target_pos = DObjGetStruct(fighter_gobj)->translate.vec.f;
                break;

            case nFTCameraModeEntry:
            case nFTCameraModeExplain:
                cams[players_num].target_pos = fp->entry_pos;
                break;

            case nFTCameraModeDeadUp:
                cams[players_num].target_pos = fp->status_vars.common.dead.pos;
                break;
            }
            cams[players_num].target_pos.y += fp->attr->cam_offset_y;

            if ((gSCManagerBattleState->game_type == nSCBattleGameType1PGame) && (gSCManagerBattleState->players[fp->player].is_spgame_enemy != FALSE))
            {
                switch (fp->camera_mode)
                {
                default: 
                    break;

                    // WHAT!?!? There are a few ways to match this, but it appears to be a control flow issue more than anything.
                    // Other solution #1: cast &cams[players_num].target_pos to an integer
                    // Other solution #2: explicitly define any of the Vec3f* functions below as s32 functions (just forget about it)
                }
                gmCameraSetTeamBoundsPosition(&cams[players_num].target_pos);
            }
            else switch (fp->camera_mode)
            {
            case nFTCameraModeDeadUp:
                gmCameraSetDeadUpStarPosition(&cams[players_num].target_pos);
                break;

            default:
                gmCameraSetBoundsPosition(&cams[players_num].target_pos);
                break;
            }
            players_num++;
            break;

        case nFTCameraModeGhost:
            break;
        }
        fighter_gobj = fighter_gobj->link_next;
    }
    if (players_num != 0)
    {
        ft_top = 65536.0F;
        ft_bottom = -65536.0F;
        ft_left = -65536.0F;
        ft_right = 65536.0F;

        gm_bottom = 65536.0F;
        gm_top = -65536.0F;
        gm_left = 65536.0F;
        gm_right = -65536.0F;

        zoom = gmCameraGetPlayerNumZoomRange(players_num);

        for (i = 0; i < players_num; i++)
        {
            cam_fp = cams[i].target_fp;

            adjust = gmCameraCalcFighterZoomRange(cam_fp, zoom);

            lr = ((cam_fp->camera_mode == nFTCameraModeEntry) || (cam_fp->camera_mode == nFTCameraModeExplain)) ?
            cam_fp->status_vars.common.entry.lr : cam_fp->lr;

            if (lr == -1)
            {
                pos_left = cams[i].target_pos.x - (1000.0F * adjust);
                pos_right = cams[i].target_pos.x + (700.0F * adjust);
            }
            else
            {
                pos_left = cams[i].target_pos.x - (700.0F * adjust);
                pos_right = cams[i].target_pos.x + (1000.0F * adjust);
            }
            if (gm_left > pos_left)
            {
                gm_left = pos_left;
            }
            if (gm_right < pos_right)
            {
                gm_right = pos_right;
            }
            pos_top = cams[i].target_pos.y + (700.0F * adjust);
            pos_bottom = cams[i].target_pos.y + (-700.0F * adjust);

            if (gm_bottom > pos_bottom)
            {
                gm_bottom = pos_bottom;
            }
            if (gm_top < pos_top)
            {
                gm_top = pos_top;
            }
            if (cams[i].target_pos.x < ft_right) // ft_right = ft_right?
            {
                ft_right = cams[i].target_pos.x;
            }
            if (cams[i].target_pos.x > ft_left) // ft_left = ft_left?
            {
                ft_left = cams[i].target_pos.x;
            }
            if (cams[i].target_pos.y < ft_top) // ft_top = ft_top?
            {
                ft_top = cams[i].target_pos.y;
            }
            if (cams[i].target_pos.y > ft_bottom) // ft_bottom = ft_bottom?
            {
                ft_bottom = cams[i].target_pos.y;
            }
            cams[i].unk_ftcobj_0x10 = adjust;
        }
        weapon_gobj = gGCCommonLinks[nGCCommonLinkIDWeapon];

        while (weapon_gobj != NULL)
        {
            wp = wpGetStruct(weapon_gobj);

            if (wp->is_camera_follow)
            {
                weapon_pos = DObjGetStruct(weapon_gobj)->translate.vec.f;

                wp_left = ft_right - 1000.0F; // wp_left = left?
                wp_right = ft_left + 1000.0F; // wp_right = right?
                wp_bottom = ft_top - 1000.0F; // wp_bottom = bottom?
                wp_top = ft_bottom + 1000.0F; // wp_top = top?

                gmCameraSetBoundsPosition(&weapon_pos);

                if (weapon_pos.x < wp_left)
                {
                    weapon_pos.x = wp_left;
                }
                if (weapon_pos.x > wp_right)
                {
                    weapon_pos.x = wp_right;
                }
                if (weapon_pos.y < wp_bottom)
                {
                    weapon_pos.y = wp_bottom;
                }
                if (weapon_pos.y > wp_top)
                {
                    weapon_pos.y = wp_top;
                }
                if ((weapon_pos.x - 1000.0F) < gm_left) // gm_left = gm_left?
                {
                    gm_left = (weapon_pos.x - 1000.0F);
                }
                if ((weapon_pos.x + 1000.0F) > gm_right) // gm_right = gm_right?
                {
                    gm_right = (weapon_pos.x + 1000.0F);
                }
                if ((weapon_pos.y - 1000.0F) < gm_bottom) // gm_bottom = gm_bottom?
                {
                    gm_bottom = (weapon_pos.y - 1000.0F);
                }
                if ((weapon_pos.y + 1000.0F) > gm_top) // gm_top = gm_top?
                {
                    gm_top = (weapon_pos.y + 1000.0F);
                }
            }
            weapon_gobj = weapon_gobj->link_next;
        }
        *hz = (gm_right - gm_left) * 0.5F;
        *vt = (gm_top - gm_bottom) * 0.5F;

        vec->x = ((gm_left + gm_right) * 0.5F);
        vec->y = (0.5F - gmCameraGetTargetAtY((*vt < *hz) ? *hz : *vt)) * (gm_bottom + gm_top);
        vec->z = 0.0F;
    }
    else
    {
        vec->x = vec->y = vec->z = 0.0F;

        *hz = *vt = 2000.0F;
    }
}

// 0x8010C200
void gmCameraGetClampDimensionsMax(f32 hz, f32 vt, f32 *max)
{
    f32 maxd;

    vt /= lbCommonTan(F_CLC_DTOR32(gGMCameraStruct.fovy * 0.5F));
    hz /= ((lbCommonTan(F_CLC_DTOR32(gGMCameraStruct.fovy * 0.5F)) *
    gGMCameraStruct.viewport_width) / gGMCameraStruct.viewport_height);

    maxd = (hz > vt) ? hz : vt;

    if (maxd < 2500.0F)
    {
        maxd = 2500.0F;
    }
    if (maxd > 30000.0F)
    {
        maxd = 30000.0F;
    }
    *max = maxd;
}

// 0x8010C320
void gmCameraGetAdjustAtAngle(Vec3f *at, Vec3f *vec, f32 x, f32 y)
{
    f32 angle_x, angle_y;

    angle_x = gGMCameraPauseCameraEyeY + y + gMPCollisionGroundData->light_angle.z;

    vec->y = -lbCommonSin(angle_x);
    vec->z = lbCommonCos(angle_x);

    angle_y = gGMCameraPauseCameraEyeX + x;

    vec->x = (lbCommonSin(angle_y) * vec->z);
    vec->z *= lbCommonCos(angle_y);
}

// 0x8010C3C0
void func_ovl2_8010C3C0(Vec3f *at, Vec3f *vec)
{
    f32 x, y;

    y = -F_CLC_DTOR32((at->y + (-900.0F)) / 133.0F);

    if (y > F_CLC_DTOR32(5.0F)) // 0.08726647F
    {
        y = F_CLC_DTOR32(5.0F);
    }
    if (y < F_CLC_DTOR32(-7.0F)) // -0.122173056F
    {
        y = F_CLC_DTOR32(-7.0F);
    }
    x = -F_CLC_DTOR32(at->x / 133.0F);

    if (x > F_CLC_DTOR32(17.5F)) // 0.30543265F
    {
        x = F_CLC_DTOR32(17.5F);
    }
    if (x < F_CLC_DTOR32(-17.5F)) // -0.30543265F
    {
        x = F_CLC_DTOR32(-17.5F);
    }
    gmCameraGetAdjustAtAngle(at, vec, x, y);
}

// 0x8010C4A4
void gmCameraUpdateInishieFocus(Vec3f *arg0, Vec3f *arg1)
{
    gmCameraGetAdjustAtAngle(arg0, arg1, 0.0F, 0.0F);
}

// 0x8010C4D0
f32 func_ovl2_8010C4D0(void)
{
    if (gGMCameraStruct.target_dist > 15000.0F)
    {
        return 0.1F;
    }
    else if (gGMCameraStruct.target_dist < 2000.0F)
    {
        return 0.05F;
    }
    return ((1.0F - ((gGMCameraStruct.target_dist - 2000.0F) / 13000.0F)) * 0.05F) + .05F; // Needs to be two different 0.05s lol
}

// 0x8010C55C
void gmCameraPan(CObj *cobj, Vec3f *pos, f32 scale)
{
    f32 mag;
    f32 unused;
    Vec3f add;

    syVectorDiff3D(&add, pos, &cobj->vec.at);
    mag = syVectorMag3D(&add) * scale;
    syVectorNorm3D(&add);
    syVectorScale3D(&add, mag);
    syVectorAdd3D(&cobj->vec.at, &add);
}

// 0x8010C5C0
void func_ovl2_8010C5C0(CObj *cobj, Vec3f *scale)
{
    Vec3f sp34;
    Vec3f pan;
    f32 unused;
    f32 mag;

    pan.x = cobj->vec.at.x + (gGMCameraStruct.target_dist * scale->x);
    pan.y = cobj->vec.at.y + (gGMCameraStruct.target_dist * scale->y);
    pan.z = cobj->vec.at.z + (gGMCameraStruct.target_dist * scale->z);

    syVectorDiff3D(&sp34, &pan, &cobj->vec.eye);
    mag = syVectorMag3D(&sp34) * 0.1F;
    syVectorNorm3D(&sp34);
    syVectorScale3D(&sp34, mag);
    syVectorAdd3D(&cobj->vec.eye, &sp34);
}

// 0x8010C670
void func_ovl2_8010C670(f32 dist)
{
    f32 temp_f0;
    f32 temp_f14;

    temp_f0 = gGMCameraStruct.target_dist - dist;

    temp_f14 = temp_f0 * 0.075F;

    if (temp_f0 <= temp_f14)
    {
        gGMCameraStruct.target_dist = dist;
    }
    else gGMCameraStruct.target_dist -= temp_f14;
}

// 0x8010C6B8
void gmCameraApplyVel(CObj *cobj)
{
    syVectorAdd3D(&cobj->vec.at, &gGMCameraStruct.vel_at);
    gGMCameraStruct.vel_at.x = gGMCameraStruct.vel_at.y = gGMCameraStruct.vel_at.z = 0.0F;
}

// 0x8010C6FC
void gmCameraApplyFOV(CObj *cobj)
{
    cobj->projection.persp.fovy = gGMCameraStruct.fovy;
}

// 0x8010C70C
void gmCameraAdjustFOV(f32 fovy)
{
    gGMCameraStruct.fovy += ((fovy - gGMCameraStruct.fovy) * 0.1F);
}

// 0x8010C734
void gmCameraDefaultFuncCamera(GObj *camera_gobj)
{
    CObj *cobj;
    f32 sp48;
    Vec3f sp3C;
    Vec3f sp30;
    f32 sp2C;
    f32 sp28;

    cobj = CObjGetStruct(camera_gobj);

    gmCameraUpdateInterests(&sp30, &sp2C, &sp28);
    gmCameraAdjustFOV(38.0F);
    gmCameraGetClampDimensionsMax(sp2C, sp28, &sp48);
    func_ovl2_8010C670(sp48);
    gmCameraPan(cobj, &sp30, func_ovl2_8010C4D0());
    func_ovl2_8010C3C0(&cobj->vec.at, &sp3C);
    func_ovl2_8010C5C0(cobj, &sp3C);
    gmCameraApplyVel(cobj);
    gmCameraApplyFOV(cobj);
}

// 0x8010C7D0
void gmCameraUpdateAcidZoom(CObj *cobj, Vec3f *scale)
{
    Vec3f add;
    Vec3f pos;
    f32 unused;
    f32 mag;
    f32 current;
    f32 step;

    pos.x = cobj->vec.at.x + (gGMCameraStruct.target_dist * scale->x);
    pos.y = cobj->vec.at.y + (gGMCameraStruct.target_dist * scale->y);
    pos.z = cobj->vec.at.z + (gGMCameraStruct.target_dist * scale->z);

    grZebesAcidGetLevelInfo(&current, &step);

    current += (step + 3000.0F);

    if (pos.y < current)
    {
        pos.y = current;
    }
    syVectorDiff3D(&add, &pos, &cobj->vec.eye);
    mag = syVectorMag3D(&add) * 0.1F;
    syVectorNorm3D(&add);
    syVectorScale3D(&add, mag);
    syVectorAdd3D(&cobj->vec.eye, &add);
}

// 0x8010C8C4
void gmCameraZebesFuncCamera(GObj *camera_gobj)
{
    CObj *cobj = CObjGetStruct(camera_gobj);
    f32 max;
    Vec3f scale;
    Vec3f sp30;
    f32 hz;
    f32 vt;

    gmCameraUpdateInterests(&sp30, &hz, &vt);
    gmCameraAdjustFOV(38.0F);
    gmCameraGetClampDimensionsMax(hz, vt, &max);
    func_ovl2_8010C670(max);
    gmCameraPan(cobj, &sp30, func_ovl2_8010C4D0());
    func_ovl2_8010C3C0(&cobj->vec.at, &scale);
    gmCameraUpdateAcidZoom(cobj, &scale);
    gmCameraApplyVel(cobj);
    gmCameraApplyFOV(cobj);
}

// 0x8010C960
void gmCameraUpdatePlayerZoom(GObj *camera_gobj)
{
    CObj *cobj;
    FTStruct *fp;
    Vec3f eye;
    Vec3f angle;
    Vec3f unused;
    Vec3f pos;
    f32 unused2;

    cobj = CObjGetStruct(camera_gobj);
    pos = DObjGetStruct(gGMCameraStruct.pzoom_fighter_gobj)->translate.vec.f;
    fp = ftGetStruct(gGMCameraStruct.pzoom_fighter_gobj);

    pos.y += fp->attr->cam_offset_y;

    gmCameraAdjustFOV(gGMCameraStruct.pzoom_fov);

    gGMCameraStruct.target_dist = gGMCameraStruct.pzoom_dist;

    gmCameraPan(cobj, &pos, gGMCameraStruct.pzoom_pan_scale);

    eye.y = gGMCameraPauseCameraEyeX + gGMCameraStruct.pzoom_eye_x;
    eye.x = gGMCameraPauseCameraEyeY + gGMCameraStruct.pzoom_eye_y;

    angle.y = -lbCommonSin(eye.x);
    angle.z = lbCommonCos(eye.x);
    angle.x = lbCommonSin(eye.y) * angle.z;
    angle.z *= lbCommonCos(eye.y);

    func_ovl2_8010C5C0(cobj, &angle);
    gmCameraApplyVel(cobj);
    gmCameraApplyFOV(cobj);
}

// 0x8010CA7C
sb32 gmCameraCheckPausePlayerOutBounds(Vec3f *pos)
{
    if ((gmCameraGetBoundsMask(pos) != 0) || (pos->z < -1000.0F) || (pos->z > 1000.0F))
    {
        return TRUE;
    }
    else return FALSE;
}

// 0x8010CAE0
void gmCameraPlayerZoomFuncCamera(GObj *camera_gobj)
{
    if (gmCameraCheckPausePlayerOutBounds(&DObjGetStruct(gGMCameraStruct.pzoom_fighter_gobj)->translate.vec.f) != FALSE)
    {
        dGMCameraFuncList[gGMCameraStruct.status_default](camera_gobj);
    }
    else gmCameraUpdatePlayerZoom(camera_gobj);
}

// 0x8010CB48
void gmCameraInishieFuncCamera(GObj *camera_gobj)
{
    CObj *cobj;
    f32 sp48;
    Vec3f sp3C;
    Vec3f sp30;
    f32 sp2C;
    f32 sp28;

    cobj = CObjGetStruct(camera_gobj);

    gmCameraUpdateInterests(&sp30, &sp2C, &sp28);
    gmCameraAdjustFOV(38.0F);
    gmCameraGetClampDimensionsMax(sp2C, sp28, &sp48);
    func_ovl2_8010C670(sp48);
    gmCameraPan(cobj, &sp30, func_ovl2_8010C4D0());
    gmCameraUpdateInishieFocus(&cobj->vec.at, &sp3C);
    func_ovl2_8010C5C0(cobj, &sp3C);
    gmCameraApplyVel(cobj);
    gmCameraApplyFOV(cobj);
}

// 0x8010CBE4
void gmCameraUpdateAnimVel(GObj *camera_gobj)
{
    CObj *cobj;

    gcPlayCamAnim(camera_gobj);

    cobj = CObjGetStruct(camera_gobj);

    cobj->vec.at.x += gGMCameraStruct.vel_all.x;
    cobj->vec.at.y += gGMCameraStruct.vel_all.y;
    cobj->vec.at.z += gGMCameraStruct.vel_all.z;
    cobj->vec.eye.x += gGMCameraStruct.vel_all.x;
    cobj->vec.eye.y += gGMCameraStruct.vel_all.y;
    cobj->vec.eye.z += gGMCameraStruct.vel_all.z;
}

// 0x8010CC74
void gmCameraAnimFuncCamera(GObj *camera_gobj)
{
    gmCameraUpdateAnimVel(camera_gobj);

    if (CObjGetStruct(camera_gobj)->anim_wait == AOBJ_ANIM_NULL)
    {
        gmCameraSetStatusDefault();
    }
}

// 0x8010CCC0
void gmCameraMapZoomFuncCamera(GObj *camera_gobj)
{
    CObj *cobj;
    Vec3f sp30;
    f32 sp2C;
    f32 sp28;

    cobj = CObjGetStruct(camera_gobj);

    gmCameraAdjustFOV(38.0F);

    syVectorDiff3D(&sp30, &gGMCameraStruct.zoom_origin_pos, &cobj->vec.at);
    sp28 = syVectorMag3D(&sp30);
    sp2C = func_ovl2_8010C4D0() * sp28;
    syVectorNorm3D(&sp30);
    syVectorScale3D(&sp30, sp2C);
    syVectorAdd3D(&cobj->vec.at, &sp30);

    syVectorDiff3D(&sp30, &gGMCameraStruct.zoom_target_pos, &cobj->vec.eye);
    sp2C = syVectorMag3D(&sp30) * 0.1F;
    syVectorNorm3D(&sp30);
    syVectorScale3D(&sp30, sp2C);
    syVectorAdd3D(&cobj->vec.eye, &sp30);
    gmCameraApplyVel(cobj);
    gmCameraApplyFOV(cobj);
}

// 0x8010CDAC
void gmCameraPlayerFollowFuncCamera(GObj *camera_gobj)
{
    Vec3f unused;
    f32 eye_x;
    f32 eye_y;
    Vec3f angle;
    Vec3f unused2;
    Vec3f pos;
    CObj *cobj;
    FTStruct *fp;

    cobj = CObjGetStruct(camera_gobj);
    pos = DObjGetStruct(gGMCameraStruct.pfollow_fighter_gobj)->translate.vec.f;
    fp = ftGetStruct(gGMCameraStruct.pfollow_fighter_gobj);

    pos.y += fp->attr->cam_offset_y;
    pos.z = 0.0F;

    gmCameraSetBoundsPosition(&pos);
    gmCameraAdjustFOV(gGMCameraStruct.pfollow_fov);

    gGMCameraStruct.target_dist = gGMCameraStruct.pfollow_dist;

    gmCameraPan(cobj, &pos, gGMCameraStruct.pfollow_pan_scale);

    eye_x = gGMCameraPauseCameraEyeX + gGMCameraStruct.pfollow_eye_x;
    eye_y = gGMCameraPauseCameraEyeY + gGMCameraStruct.pfollow_eye_y;

    angle.y = -lbCommonSin(eye_y);
    angle.z = lbCommonCos(eye_y);
    angle.x = lbCommonSin(eye_x) * angle.z;
    angle.z *= lbCommonCos(eye_x);

    func_ovl2_8010C5C0(cobj, &angle);
    gmCameraApplyVel(cobj);
    gmCameraApplyFOV(cobj);
}

// 0x8010CECC
void gmCameraRunFuncCamera(GObj *camera_gobj)
{
    gGMCameraStruct.func_camera(camera_gobj);
}

// 0x8010CEF4
void gmCameraSetStatus(s32 status_id)
{
    gGMCameraStruct.status_prev = gGMCameraStruct.status_curr;
    gGMCameraStruct.status_curr = status_id;

    gGMCameraStruct.func_camera = dGMCameraFuncList[status_id];
}

// 0x8010CF20
void gmCameraSetStatusDefault(void)
{
    gmCameraSetStatus(gGMCameraStruct.status_default);
}

// 0x8010CF44
void gmCameraSetStatusPlayerZoom(GObj *fighter_gobj, f32 eye_x, f32 eye_y, f32 dist, f32 pan_scale, f32 fov)
{
    gmCameraSetStatus(nGMCameraStatusPlayerZoom);

    gGMCameraStruct.pzoom_fighter_gobj = fighter_gobj;
    gGMCameraStruct.pzoom_eye_x = eye_x;
    gGMCameraStruct.pzoom_eye_y = eye_y;
    gGMCameraStruct.pzoom_dist = dist;
    gGMCameraStruct.pzoom_pan_scale = pan_scale;
    gGMCameraStruct.pzoom_fov = fov;
}

// 0x8010CFA8
void gmCameraSetStatusPlayerFollow(GObj *fighter_gobj, f32 eye_x, f32 eye_y, f32 dist, f32 pan_scale, f32 fov)
{
    gmCameraSetStatus(nGMCameraStatusPlayerFollow);

    gGMCameraStruct.pfollow_fighter_gobj = fighter_gobj;
    gGMCameraStruct.pfollow_eye_x = eye_x;
    gGMCameraStruct.pfollow_eye_y = eye_y;
    gGMCameraStruct.pfollow_dist = dist;
    gGMCameraStruct.pfollow_pan_scale = pan_scale;
    gGMCameraStruct.pfollow_fov = fov;
}

// 0x8010D00C
void gmCameraSetStatusPrev(void)
{
    gmCameraSetStatus(gGMCameraStruct.status_prev);
}

// 0x8010D030
void gmCameraSetStatusAnim(AObjEvent32 *camanim_joint, f32 anim_frame, Vec3f *vel)
{
    gmCameraSetStatus(nGMCameraStatusAnim);

    gGMCameraStruct.vel_all = *vel;

    gcAddCObjCamAnimJoint(CObjGetStruct(gGMCameraGObj), camanim_joint, anim_frame);
    gmCameraUpdateAnimVel(gGMCameraGObj);
}

// 0x8010D0A4
void gmCameraSetStatusMapZoom(Vec3f *origin, Vec3f *target)
{
    Vec3f dist;

    gmCameraSetStatus(nGMCameraStatusMapZoom);

    gGMCameraStruct.zoom_origin_pos = *origin;
    gGMCameraStruct.zoom_target_pos = *target;

    syVectorDiff3D(&dist, origin, target);

    gGMCameraStruct.target_dist = syVectorMag3D(&dist);
}

// 0x8010D128
void gmCameraSetVelAt(Vec3f *vel)
{
    gGMCameraStruct.vel_at = *vel;
}

// 0x8010D14C - Unused?
void func_ovl2_8010D14C(GObj *gobj)
{
    return;
}

// 0x8010D154
f32 gmCameraGetMatrixMax(void)
{
    f32 ret, abs;
    s32 i, j;

    ret = 0.0F;

    for (i = 0; i < ARRAY_COUNT(gGMCameraMatrix[i]); i++)
    {
        for (j = 0; j < ARRAY_COUNT(gGMCameraMatrix); j++)
        {
            abs = ABSF(gGMCameraMatrix[i][j]);

            if (ret < abs)
            {
                ret = abs;
            }
        }
    }
    return ret;
}

// 0x8010D250
sb32 gmCameraLookAtFuncMatrix(Mtx *mtx, CObj *cobj, Gfx **dls)
{
    Mtx *temp_mtx;
    Mtx44f sp5C;
    u16 *perspnorm;
    f32 max;
    s32 unused;

    syMatrixAdvanceW(temp_mtx, gSYTaskmanGraphicsHeap);

    syMatrixPerspFastF(gGCMatrixPerspF, &cobj->projection.persp.norm, cobj->projection.persp.fovy, cobj->projection.persp.aspect, cobj->projection.persp.near, cobj->projection.persp.far, cobj->projection.persp.scale);
    syMatrixF2L(&gGCMatrixPerspF, temp_mtx);

    sGCMatrixProjectL = temp_mtx;

    syMatrixLookAtReflectF(&sp5C, &gGMCameraStruct.look_at, cobj->vec.eye.x, cobj->vec.eye.y, cobj->vec.eye.z, cobj->vec.at.x, cobj->vec.at.y, cobj->vec.at.z, cobj->vec.up.x, cobj->vec.up.y, cobj->vec.up.z);
    guMtxCatF(sp5C, gGCMatrixPerspF, gGMCameraMatrix);

    max = gmCameraGetMatrixMax();

    if (max > 32000.0F)
    {
        syMatrixPerspFastF(gGCMatrixPerspF, &cobj->projection.persp.norm, cobj->projection.persp.fovy, cobj->projection.persp.aspect, cobj->projection.persp.near, cobj->projection.persp.far, 32000.0F / max);
        syMatrixF2L(&gGCMatrixPerspF, temp_mtx);

        sGCMatrixProjectL = temp_mtx;

        syMatrixLookAtReflectF(&sp5C, &gGMCameraStruct.look_at, cobj->vec.eye.x, cobj->vec.eye.y, cobj->vec.eye.z, cobj->vec.at.x, cobj->vec.at.y, cobj->vec.at.z, cobj->vec.up.x, cobj->vec.up.y, cobj->vec.up.z);
        guMtxCatF(sp5C, gGCMatrixPerspF, gGMCameraMatrix);
    }
    syMatrixF2L(&gGMCameraMatrix, mtx);

    return 0;
}

// 0x8010D428
sb32 gmCameraPrepLookAtFuncMatrix(Mtx *mtx, CObj *cobj, Gfx **dls)
{
    gSPLookAtX(dls[0]++, &gGMCameraStruct.look_at.l[0]);
    gSPLookAtY(dls[0]++, &gGMCameraStruct.look_at.l[1]);

    gSPMatrix(dls[0]++, mtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);

    gSPPerspNormalize(dls[0]++, cobj->projection.persp.norm);

    return 0;
}

// 0x8010D4B0
void func_ovl2_8010D4B0(Mtx *mtx, CObj *cobj, Gfx **dls)
{
    return;
}

// 0x8010D4C0
void gmCameraDefaultProcDisplay(GObj *camera_gobj)
{
    CObj *cobj = CObjGetStruct(camera_gobj);

    gcSetCameraMatrixMode(3);
    func_8001663C(gSYTaskmanDLHeads, cobj, 0);
    gcPrepCameraMatrix(gSYTaskmanDLHeads, cobj);
    gcRunFuncCamera(cobj, 0);

    gIFCommonPlayerInterface.magnify_mode = 0;
    gIFCommonPlayerInterface.arrows_flags = 0;

    // gIFCommonPlayerInterface.unk1 = 0;
    // gIFCommonPlayerInterface.unkE = 0;

    gDPSetRenderMode(gSYTaskmanDLHeads[1]++, G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2);

    camera_gobj->camera_mask = COBJ_MASK_DLLINK(2) | COBJ_MASK_DLLINK(1);

    gcCaptureCameraGObj(camera_gobj, (cobj->flags & COBJ_FLAG_IDENTIFIER) ? 1 : 0);
    syTaskmanUpdateDLBuffers();

    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gDPSetRenderMode(gSYTaskmanDLHeads[1]++, G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2);

    camera_gobj->camera_mask = COBJ_MASK_DLLINK(4);

    gcCaptureCameraGObj(camera_gobj, (cobj->flags & COBJ_FLAG_IDENTIFIER) ? 1 : 0);
    syTaskmanUpdateDLBuffers();

    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gDPSetRenderMode(gSYTaskmanDLHeads[1]++, G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2);

    camera_gobj->camera_mask = COBJ_MASK_DLLINK(12) | COBJ_MASK_DLLINK(11) | 
                               COBJ_MASK_DLLINK(10) | COBJ_MASK_DLLINK(9)  |
                               COBJ_MASK_DLLINK(7)  | COBJ_MASK_DLLINK(6);

    gcCaptureCameraGObj(camera_gobj, (cobj->flags & COBJ_FLAG_IDENTIFIER) ? 1 : 0);
    syTaskmanUpdateDLBuffers();

    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gDPSetRenderMode(gSYTaskmanDLHeads[1]++, G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2);

    camera_gobj->camera_mask = COBJ_MASK_DLLINK(15) | COBJ_MASK_DLLINK(14) | COBJ_MASK_DLLINK(13);

    gcCaptureCameraGObj(camera_gobj, (cobj->flags & COBJ_FLAG_IDENTIFIER) ? 1 : 0);
    syTaskmanUpdateDLBuffers();

    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gDPSetRenderMode(gSYTaskmanDLHeads[1]++, G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2);

    camera_gobj->camera_mask = COBJ_MASK_DLLINK(18) | COBJ_MASK_DLLINK(17) | COBJ_MASK_DLLINK(16);

    gcCaptureCameraGObj(camera_gobj, (cobj->flags & COBJ_FLAG_IDENTIFIER) ? 1 : 0);
    syTaskmanUpdateDLBuffers();

    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gDPSetRenderMode(gSYTaskmanDLHeads[1]++, G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2);

    camera_gobj->camera_mask = COBJ_MASK_DLLINK(20) | COBJ_MASK_DLLINK(19);

    gcCaptureCameraGObj(camera_gobj, (cobj->flags & COBJ_FLAG_IDENTIFIER) ? 1 : 0);
    syTaskmanUpdateDLBuffers();
}

// 0x8010D7E8
GObj* gmCameraMakeDefaultCamera(u8 tk1, u8 tk2, void (*proc)(GObj*))
{
    GObj *camera_gobj;
    CObj *cobj;
    Vec3f sp4C;
    f32 temp_f0;

    camera_gobj = gcMakeCameraGObj
    (
        nGCCommonKindMainCamera,
        NULL,
        nGCCommonLinkIDCamera,
        GOBJ_PRIORITY_DEFAULT,
        gmCameraDefaultProcDisplay,
        50,
        0,
        ~0,
        FALSE,
        nGCProcessKindFunc,
        proc,
        3,
        FALSE
    );
    gGMCameraGObj = camera_gobj;

    cobj = CObjGetStruct(camera_gobj);

    gcAddXObjForCamera(cobj, tk1, 0);

    if (tk2 != nGCMatrixKindNull)
    {
        gcAddXObjForCamera(cobj, tk2, 0);
    }
    cobj->projection.persp = dGMCameraPerspDefault;
    cobj->vec = dGMCameraCObjVecDefault;

    syRdpSetViewport
    (
        &cobj->viewport,
        gGMCameraStruct.viewport_ulx,
        gGMCameraStruct.viewport_uly,
        gGMCameraStruct.viewport_lrx,
        gGMCameraStruct.viewport_lry
    );

    // This (f32) cast is NECESSARY! scissor_ulx through scissor_lry are signed integers!
    cobj->projection.persp.aspect =
    ((f32)(gGMCameraStruct.viewport_lrx - gGMCameraStruct.viewport_ulx) / (f32)(gGMCameraStruct.viewport_lry - gGMCameraStruct.viewport_uly));

    cobj->flags |= COBJ_FLAG_DLBUFFERS;

    gGMCameraStruct.target_dist = 10000.0F;

    gGMCameraPauseCameraEyeY = gGMCameraPauseCameraEyeX = 0.0F;

    sp4C.y = -lbCommonSin(gGMCameraPauseCameraEyeY);
    sp4C.z = lbCommonCos(gGMCameraPauseCameraEyeY);
    sp4C.x = lbCommonSin(gGMCameraPauseCameraEyeX) * sp4C.z;
    sp4C.z *= lbCommonCos(gGMCameraPauseCameraEyeX);

    cobj->vec.at.x = cobj->vec.at.z = 0.0F;
    cobj->vec.at.y = 300.0F;

    cobj->vec.eye.x = (gGMCameraStruct.target_dist * sp4C.x);
    cobj->vec.eye.y = cobj->vec.at.y + (gGMCameraStruct.target_dist * sp4C.y);
    cobj->vec.eye.z = (gGMCameraStruct.target_dist * sp4C.z);

    gGMCameraStruct.vel_at.x = gGMCameraStruct.vel_at.y = gGMCameraStruct.vel_at.z = 0;

    switch (gSCManagerBattleState->gkind)
    {
    case nGRKindZebes:
        gGMCameraStruct.status_curr = nGMCameraStatusZebes;
        break;

    case nGRKindInishie:
        gGMCameraStruct.status_curr = nGMCameraStatusInishie;
        break;

    default:
        gGMCameraStruct.status_curr = nGMCameraStatusDefault;
        // No break? Doesn't match otherwise :brainshock:
    }
    gGMCameraStruct.status_default = gGMCameraStruct.status_prev = gGMCameraStruct.status_curr;
    gGMCameraStruct.func_camera = dGMCameraFuncList[gGMCameraStruct.status_curr];
    gGMCameraStruct.fovy = 38.0F;

    return camera_gobj;
}

// 0x8010DB00
void gmCameraMakeBattleCamera(void)
{
    gmCameraMakeDefaultCamera(0x4C, nGCMatrixKindNull, gmCameraRunFuncCamera);
}

// 0x8010DB2C
GObj* gmCameraMakeMovieCamera(void (*func_camera)(GObj*))
{
    return gmCameraMakeDefaultCamera(nGCMatrixKindPerspFastF, 8, func_camera);
}

// 0x8010DB54
GObj* gmCameraMakeWallpaperCamera(void)
{
    GObj *camera_gobj = gcMakeCameraGObj
    (
        nGCCommonKindWallpaperCamera,
        NULL,
        nGCCommonLinkIDCamera,
        GOBJ_PRIORITY_DEFAULT,
        lbCommonDrawSprite,
        80,
        COBJ_MASK_DLLINK(0),
        -1,
        FALSE,
        nGCProcessKindFunc,
        NULL,
        1,
        FALSE
    );
    CObj *cobj = CObjGetStruct(camera_gobj);

    syRdpSetViewport
    (
        &cobj->viewport,
        gGMCameraStruct.viewport_ulx,
        gGMCameraStruct.viewport_uly,
        gGMCameraStruct.viewport_lrx,
        gGMCameraStruct.viewport_lry
    );
    return camera_gobj;
}

// 0x8010DC24
void gmCameraScreenFlashProcDisplay(GObj *camera_gobj)
{
    CObj *cobj = CObjGetStruct(camera_gobj);

    if (gIFCommonPlayerInterface.magnify_mode != 0)
    {
        Vp_t *viewport;
        s32 ulx, uly, lrx, lry;

        gSPViewport(gSYTaskmanDLHeads[0]++, &CObjGetStruct(gGMCameraGObj)->viewport);

        viewport = &CObjGetStruct(gGMCameraGObj)->viewport.vp;

        ulx = (viewport->vtrans[0] / 4) - (viewport->vscale[0] / 4);
        uly = (viewport->vtrans[1] / 4) - (viewport->vscale[1] / 4);

        lrx = (viewport->vtrans[0] / 4) + (viewport->vscale[0] / 4);
        lry = (viewport->vtrans[1] / 4) + (viewport->vscale[1] / 4);

        gDPSetScissor(gSYTaskmanDLHeads[0]++, G_SC_NON_INTERLACE, ulx, uly, lrx, lry);
    }
    gcCaptureCameraGObj(camera_gobj, (cobj->flags & COBJ_FLAG_IDENTIFIER) ? 1 : 0);
    func_80017CC8(cobj);
}

// 0x8010DDC4
void gmCameraScreenFlashMakeCamera(void)
{
    CObj *cobj = CObjGetStruct
    (
        gcMakeCameraGObj
        (
            nGCCommonKindScissorCamera,
            NULL,
            nGCCommonLinkIDCamera,
            GOBJ_PRIORITY_DEFAULT,
            gmCameraScreenFlashProcDisplay,
            20,
            COBJ_MASK_DLLINK(22),
            ~0,
            FALSE,
            nGCProcessKindFunc,
            NULL,
            1,
            FALSE
        )
    );
    cobj->flags |= COBJ_FLAG_DLBUFFERS;
}

// 0x8010DE48
sb32 gmCameraPlayerMagnifyFuncMatrix(Mtx *mtx, CObj *cobj, Gfx **dls)
{
    f32 unused1;
    Mtx44f spA4;
    Mtx44f sp64;
    Vec3f *eye;
    Vec3f *at;
    Vec3f sp50;
    f32 unused2;
    f32 dist_x;
    f32 dist_y;
    f32 dist_z;

    eye = &CObjGetStruct(gGMCameraGObj)->vec.eye;
    at = &CObjGetStruct(gGMCameraGObj)->vec.at;

    dist_x = eye->x - at->x;
    dist_y = eye->y - at->y;
    dist_z = eye->z - at->z;

    syMatrixLookAtF(&sp64, 0.0F, 300.0F, sqrtf(SQUARE(dist_x) + SQUARE(dist_y) + SQUARE(dist_z)), 0.0F, 300.0F, 0.0F, 0.0F, 1.0F, 0.0F);
    guMtxCatF(sp64, gGCMatrixPerspF, spA4);

    sp50.z = 0.0F;
    sp50.y = 900.0F;
    sp50.x = 0.0F;

    func_ovl2_800EB924(CObjGetStruct(gGMCameraGObj), spA4, &sp50, &dist_x, &dist_y);

    gIFCommonPlayerInterface.magnify_scale = (dist_y / 18.0F);

    if (gIFCommonPlayerInterface.magnify_scale > 3.0F)
    {
        gIFCommonPlayerInterface.magnify_scale = 3.0F;
    }
    syMatrixOrthoF(&spA4, -450.0F, 450.0F, -450.0F, 450.0F, 256.0F, 39936.0F, 1.0F);
    guMtxCatF(sp64, spA4, spA4);
    syMatrixF2L(&spA4, mtx);

    return 0;
}

// 0x8010E00C
sb32 gmCameraOrthoLookAtFuncMatrix(Mtx *mtx, CObj *cobj, Gfx **dls)
{
    Mtx44f sp78;
    Mtx44f sp38;
    f32 width;
    f32 height;

    width = (gGMCameraStruct.viewport_width / 2);
    height = (gGMCameraStruct.viewport_height / 2);

    syMatrixOrthoF(&sp78, -width, width, -height, height, 100.0F, 12800.0F, 1.0F);
    syMatrixLookAtF(&sp38, 0.0F, 0.0F, 1000.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F);

    guMtxCatF(sp38, sp78, sp78);
    syMatrixF2L(&sp78, mtx);

    return 0;
}

// 0x8010E10C
sb32 gmCameraPrepProjectionFuncMatrix(Mtx *mtx, CObj *cobj, Gfx **dls)
{
    gSPMatrix(dls[0]++, mtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);

    return 0;
}

// 0x8010E134
void gmCameraPlayerMagnifyProcDisplay(GObj *camera_gobj)
{
    if (gIFCommonPlayerInterface.magnify_mode != 0)
    {
        CObj *cobj = CObjGetStruct(camera_gobj);

        gcPrepCameraMatrix(gSYTaskmanDLHeads, cobj);

        gcCaptureCameraGObj(camera_gobj, (cobj->flags & COBJ_FLAG_IDENTIFIER) ? 1 : 0);
        func_80017CC8(cobj);
    }
}

// 0x8010E1A4
void gmCameraMakePlayerMagnifyCamera(void)
{
    CObj *cobj = CObjGetStruct
    (
        gcMakeCameraGObj
        (
            nGCCommonKindPlayerMagnifyCamera,
            NULL,
            nGCCommonLinkIDCamera,
            GOBJ_PRIORITY_DEFAULT,
            gmCameraPlayerMagnifyProcDisplay,
            30,
            COBJ_MASK_DLLINK(9),
            ~0,
            FALSE,
            nGCProcessKindFunc,
            NULL,
            1,
            FALSE
        )
    );
    lbCommonInitCameraVec(cobj, 0x4D, 0);
    lbCommonInitCameraOrtho(cobj, 0x4E, 1);

    cobj->flags |= COBJ_FLAG_DLBUFFERS;

    gIFCommonPlayerInterface.magnify_mode = 0;
}

// 0x8010E254
void gmCameraPlayerArrowsProcDisplay(GObj *camera_gobj)
{
    gcSetCameraMatrixMode(1);

    if (gIFCommonPlayerInterface.arrows_flags != 0)
    {
        CObj *cobj = CObjGetStruct(camera_gobj);

        gcPrepCameraMatrix(gSYTaskmanDLHeads, cobj);

        gcCaptureCameraGObj(camera_gobj, (cobj->flags & COBJ_FLAG_IDENTIFIER) ? 1 : 0);
        func_80017CC8(cobj);
    }
}

// 0x8010E2D4
void gmCameraMakePlayerArrowsCamera(void)
{
    CObj *cobj = CObjGetStruct
    (
        gcMakeCameraGObj
        (
            nGCCommonKindPlayerArrowsCamera,
            NULL,
            nGCCommonLinkIDCamera,
            GOBJ_PRIORITY_DEFAULT,
            gmCameraPlayerArrowsProcDisplay,
            35,
            COBJ_MASK_DLLINK(8),
            ~0,
            FALSE,
            nGCProcessKindFunc,
            NULL,
            1,
            FALSE
        )
    );
#if defined(REGION_US)
    lbCommonInitCameraOrtho(cobj, 0x54, 1);
#else
    lbCommonInitCameraOrtho(cobj, 0x53, 1);
#endif

    cobj->flags |= COBJ_FLAG_DLBUFFERS;

    gIFCommonPlayerInterface.arrows_flags = 0;
}

// 0x8010E374
GObj* gmCameraMakeInterfaceCamera(void)
{
    GObj *camera_gobj = gcMakeCameraGObj
    (
        nGCCommonKindScissorCamera,
        NULL,
        nGCCommonLinkIDCamera,
        GOBJ_PRIORITY_DEFAULT,
        lbCommonDrawSprite,
        20,
        COBJ_MASK_DLLINK(24) | COBJ_MASK_DLLINK(23),
        ~0,
        FALSE,
        nGCProcessKindFunc,
        NULL,
        1,
        FALSE
    );
    CObj *cobj = CObjGetStruct(camera_gobj);

    syRdpSetViewport(&cobj->viewport, (f32)gGMCameraStruct.viewport_ulx, (f32)gGMCameraStruct.viewport_uly, (f32)gGMCameraStruct.viewport_lrx, (f32)gGMCameraStruct.viewport_lry);

    cobj->flags |= COBJ_FLAG_DLBUFFERS;

    return camera_gobj;
}

// 0x8010E458
void gmCameraEffectProcDisplay(GObj *camera_gobj)
{
    CObj *cobj = CObjGetStruct(camera_gobj);

    gcCaptureCameraGObj(camera_gobj, (cobj->flags & COBJ_FLAG_IDENTIFIER) ? 1 : 0);
}

// 0x8010E498 - Not entirely sure what this does, no apparent effect when it's disabled...
GObj* gmCameraMakeEffectCamera(void)
{
    GObj *camera_gobj = gcMakeCameraGObj
    (
        nGCCommonKindScissorCamera,
        NULL,
        nGCCommonLinkIDCamera,
        GOBJ_PRIORITY_DEFAULT,
        gmCameraEffectProcDisplay,
        15,
        COBJ_MASK_DLLINK(25),
        ~0,
        FALSE,
        nGCProcessKindFunc,
        NULL,
        1,
        FALSE
    );
    CObj *cobj = CObjGetStruct(camera_gobj);

    syRdpSetViewport
    (
        &cobj->viewport,
        gGMCameraStruct.viewport_ulx,
        gGMCameraStruct.viewport_uly,
        gGMCameraStruct.viewport_lrx,
        gGMCameraStruct.viewport_lry
    );
    cobj->projection.persp.aspect = (f32) gGMCameraStruct.viewport_width / (f32) gGMCameraStruct.viewport_height;

    return camera_gobj;
}

// 0x8010E598 - Are lrx and lry used as width and height respectively?
void gmCameraSetViewportDimensions(s32 ulx, s32 uly, s32 lrx, s32 lry)
{
    gGMCameraStruct.viewport_ulx = ulx;
    gGMCameraStruct.viewport_uly = uly;
    gGMCameraStruct.viewport_lrx = lrx;
    gGMCameraStruct.viewport_lry = lry;
    gGMCameraStruct.viewport_center_x = (ulx + lrx) / 2;
    gGMCameraStruct.viewport_center_y = (uly + lry) / 2;
    gGMCameraStruct.viewport_width = lrx - ulx;
    gGMCameraStruct.viewport_height = lry - uly;
}

// 0x8010E5F4 - Checks if input position is within the camera's viewport
sb32 gmCameraCheckTargetInBounds(f32 pos_x, f32 pos_y)
{
    f32 viewport_width = (gGMCameraStruct.viewport_width / 2);
    f32 viewport_height = (gGMCameraStruct.viewport_height / 2);

    if ((pos_x < -viewport_width) || (pos_x > viewport_width) || (pos_y < -viewport_height) || (pos_y > viewport_height))
    {
        return FALSE;
    }
    return TRUE;
}
