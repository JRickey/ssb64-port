#include <wp/weapon.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80167880
sb32 wpMapProcLRWallCheckFloor(MPCollData *coll_data, GObj *weapon_gobj, u32 flags)
{
    s32 floor_line_id = coll_data->floor_line_id;
    sb32 is_collide_floor = FALSE;

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

            is_collide_floor = TRUE;
        }
    }
    else coll_data->is_coll_end = TRUE;
    
    if (mpProcessCheckTestFloorCollision(coll_data, floor_line_id) != FALSE)
    {
        mpProcessSetLandingFloor(coll_data);

        if (coll_data->mask_stat & MAP_FLAG_FLOOR)
        {
            mpProcessRunFloorEdgeAdjust(coll_data);

            is_collide_floor = TRUE;
        }
        coll_data->is_coll_end = FALSE;
    }
    return is_collide_floor;
}

// 0x8016796C
sb32 wpMapTestLRWallCheckFloor(GObj *weapon_gobj)
{
    return mpProcessUpdateMain(&wpGetStruct(weapon_gobj)->coll_data, wpMapProcLRWallCheckFloor, weapon_gobj, 0);
}

// 0x801679A0
sb32 wpMapProcAll(MPCollData *coll_data, GObj *weapon_gobj, u32 flags)
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
    return FALSE;
}

// 0x80167A58
sb32 wpMapTestAll(GObj *weapon_gobj)
{
    return mpProcessUpdateMain(&wpGetStruct(weapon_gobj)->coll_data, wpMapProcAll, weapon_gobj, 0);
}

// 0x80167A8C
sb32 wpMapProcAllCheckFloor(MPCollData *coll_data, GObj *weapon_gobj, u32 flags)
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
        mpProcessSetLandingFloor(coll_data);

        if (coll_data->mask_stat & MAP_FLAG_FLOOR)
        {
            mpProcessRunFloorEdgeAdjust(coll_data);

            coll_data->is_coll_end = TRUE;

            return TRUE;
        }
    }
    return FALSE;
}

// 0x80167B58
sb32 wpMapTestAllCheckFloor(GObj *weapon_gobj)
{
    return mpProcessUpdateMain(&wpGetStruct(weapon_gobj)->coll_data, wpMapProcAllCheckFloor, weapon_gobj, 0);
}

// 0x80167B8C
sb32 wpMapProcAllCheckCollEnd(MPCollData *coll_data, GObj *weapon_gobj, u32 flags)
{
    if (mpProcessCheckTestLWallCollisionAdjNew(coll_data) != FALSE)
    {
        coll_data->is_coll_end = TRUE;
    }
    if (mpProcessCheckTestRWallCollisionAdjNew(coll_data) != FALSE)
    {
        coll_data->is_coll_end = TRUE;
    }
    if (mpProcessCheckTestCeilCollisionAdjNew(coll_data) != FALSE)
    {
        coll_data->is_coll_end = TRUE;
    }
    if (mpProcessRunFloorCollisionAdjNewNULL(coll_data) != FALSE)
    {
        coll_data->is_coll_end = TRUE;
    }
    return coll_data->is_coll_end;
}

// 0x80167C04
sb32 wpMapTestAllCheckCollEnd(GObj *weapon_gobj)
{
    return mpProcessUpdateMain(&wpGetStruct(weapon_gobj)->coll_data, wpMapProcAllCheckCollEnd, weapon_gobj, 0);
}

// 0x80167C38
sb32 wpMapCheckAllRebound(GObj *weapon_gobj, u32 check_flags, f32 mod_vel, Vec3f *pos) // Modify velocity based on angle of collision
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    MPCollData *coll_data = &wp->coll_data;
    sb32 return_bool = FALSE;
    Vec3f mod_pos, *translate = &DObjGetStruct(weapon_gobj)->translate.vec.f;
    u16 coll_flags = (wp->coll_data.mask_prev ^ wp->coll_data.mask_curr) & wp->coll_data.mask_curr & MAP_FLAG_MAIN_MASK;
    u32 unused;

    if (coll_flags & check_flags & MAP_FLAG_LWALL)
    {
        if (lbCommonSim2D(&wp->physics.vel_air, &coll_data->lwall_angle) < 0.0F)
        {
            lbCommonReflect2D(&wp->physics.vel_air, &coll_data->lwall_angle);

            return_bool = TRUE;

            mod_pos.x = translate->x + coll_data->map_coll.width;
            mod_pos.y = translate->y + coll_data->map_coll.center;
        }
    }
    if (coll_flags & check_flags & MAP_FLAG_RWALL)
    {
        if (lbCommonSim2D(&wp->physics.vel_air, &coll_data->rwall_angle) < 0.0F)
        {
            return_bool = TRUE;

            lbCommonReflect2D(&wp->physics.vel_air, &coll_data->rwall_angle);

            mod_pos.x = translate->x - coll_data->map_coll.width;
            mod_pos.y = translate->y + coll_data->map_coll.center;
        }
    }
    if (coll_flags & check_flags & MAP_FLAG_CEIL)
    {
        if (lbCommonSim2D(&wp->physics.vel_air, &coll_data->ceil_angle) < 0.0F)
        {
            return_bool = TRUE;

            lbCommonReflect2D(&wp->physics.vel_air, &coll_data->ceil_angle);

            mod_pos.x = translate->x;
            mod_pos.y = translate->y + coll_data->map_coll.top;
        }
    }
    if (coll_flags & check_flags & MAP_FLAG_FLOOR)
    {
        if (lbCommonSim2D(&wp->physics.vel_air, &coll_data->floor_angle) < 0.0F)
        {
            return_bool = TRUE;

            lbCommonReflect2D(&wp->physics.vel_air, &coll_data->floor_angle);

            mod_pos.x = translate->x;
            mod_pos.y = translate->y + coll_data->map_coll.bottom;
        }
    }
    if (return_bool != FALSE)
    {
        lbCommonScale2D(&wp->physics.vel_air, mod_vel);

        if (pos != NULL)
        {
            pos->x = mod_pos.x;
            pos->y = mod_pos.y;
            pos->z = translate->z;
        }
    }
    return return_bool;
}

// 0x80167E78
void wpMapSetGround(WPStruct *wp) // Make weapon grounded
{
    wp->ga = nMPKineticsGround;
    wp->physics.vel_ground = wp->physics.vel_air.x * wp->lr;
}

// 0x80167E9C
void wpMapSetAir(WPStruct *wp) // Make item airborne
{
    wp->ga = nMPKineticsAir;
}
