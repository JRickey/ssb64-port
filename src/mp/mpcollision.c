#include <mp/map.h>
#include <gr/ground.h>
#include <sc/scene.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x8012C4E0
f32 dMPCollisionMaterialFrictions[/* */] =
{
    4.0F, 3.0F,
    3.0F, 1.0F,
    2.0F, 2.0F,
    4.0F, 4.0F,
    4.0F, 4.0F,
    4.0F, 4.0F,
    4.0F, 4.0F,
    4.0F, 4.0F
};

// 0x8012C520
GRFileInfo dMPCollisionGroundFileInfos[/* */] =
{
    { &llGRCastleMapFileID,        &llGRCastleMapMapHeader }, // Peach's Castle
    { &llGRSectorMapFileID,        &llGRSectorMapMapHeader }, // Sector Z
    { &llGRJungleMapFileID,        &llGRJungleMapMapHeader }, // Kongo Jungle
    { &llGRZebesMapFileID,         &llGRZebesMapMapHeader }, // Planet Zebes
    { &llGRHyruleMapFileID,        &llGRHyruleMapMapHeader }, // Hyrule Castle
    { &llGRYosterMapFileID,        &llGRYosterMapMapHeader }, // Yoshi's Island
    { &llGRPupupuMapFileID,        &llGRPupupuMapMapHeader }, // Dream Land
    { &llGRYamabukiMapFileID,      &llGRYamabukiMapMapHeader }, // Saffron City
    { &llGRInishieMapFileID,       &llGRInishieMapMapHeader }, // Mushroom Kingdom
    { &llGRPupupuSmallMapFileID,   &llGRPupupuSmallMapMapHeader }, // Beta Dream Land
    { &llGRPupupuTestMapFileID,    &llGRPupupuTestMapMapHeader }, // Test Stage
    { &llGRExplainMapFileID,       &llGRExplainMapMapHeader }, // How to Play
    { &llGRYosterSmallMapFileID,   &llGRYosterSmallMapMapHeader }, // Small Yoshi's Island (1P Game)
    { &llGRMetalMapFileID,         &llGRMetalMapMapHeader }, // Meta Crystal
    { &llGRZakoMapFileID,          &llGRZakoMapMapHeader }, // Duel Zone
    { &llGRBonus3MapFileID,        &llGRBonus3MapMapHeader }, // Race to the Finish
    { &llGRLastMapFileID,          &llGRLastMapMapHeader }, // Final Destination
    { &llGRBonus1MarioMapFileID,   &llGRBonus1MarioMapMapHeader }, // Break the Targets Mario
    { &llGRBonus1FoxMapFileID,     &llGRBonus1FoxMapMapHeader }, // Break the Targets Fox
    { &llGRBonus1DonkeyMapFileID,  &llGRBonus1DonkeyMapMapHeader }, // Break the Targets Donkey Kong
    { &llGRBonus1SamusMapFileID,   &llGRBonus1SamusMapMapHeader }, // Break the Targets Samus
    { &llGRBonus1LuigiMapFileID,   &llGRBonus1LuigiMapMapHeader }, // Break the Targets Luigi
    { &llGRBonus1LinkMapFileID,    &llGRBonus1LinkMapMapHeader }, // Break the Targets Link
    { &llGRBonus1YoshiMapFileID,   &llGRBonus1YoshiMapMapHeader }, // Break the Targets Yoshi
    { &llGRBonus1CaptainMapFileID, &llGRBonus1CaptainMapMapHeader }, // Break the Targets Captain Falcon
    { &llGRBonus1KirbyMapFileID,   &llGRBonus1KirbyMapMapHeader }, // Break the Targets Kirby
    { &llGRBonus1PikachuMapFileID, &llGRBonus1PikachuMapMapHeader }, // Break the Targets Pikachu
    { &llGRBonus1PurinMapFileID,   &llGRBonus1PurinMapMapHeader }, // Break the Targets Jigglypuff
    { &llGRBonus1NessMapFileID,    &llGRBonus1NessMapMapHeader }, // Break the Targets Ness
    { &llGRBonus2MarioMapFileID,   &llGRBonus2MarioMapMapHeader }, // Board the Platforms Mario
    { &llGRBonus2FoxMapFileID,     &llGRBonus2FoxMapMapHeader }, // Board the Platforms Fox
    { &llGRBonus2DonkeyMapFileID,  &llGRBonus2DonkeyMapMapHeader }, // Board the Platforms Donkey Kong
    { &llGRBonus2SamusMapFileID,   &llGRBonus2SamusMapMapHeader }, // Board the Platforms Samus
    { &llGRBonus2LuigiMapFileID,   &llGRBonus2LuigiMapMapHeader }, // Board the Platforms Luigi
    { &llGRBonus2LinkMapFileID,    &llGRBonus2LinkMapMapHeader }, // Board the Platforms Link
    { &llGRBonus2YoshiMapFileID,   &llGRBonus2YoshiMapMapHeader }, // Board the Platforms Yoshi
    { &llGRBonus2CaptainMapFileID, &llGRBonus2CaptainMapMapHeader }, // Board the Platforms Captain Falcon
    { &llGRBonus2KirbyMapFileID,   &llGRBonus2KirbyMapMapHeader }, // Board the Platforms Kirby
    { &llGRBonus2PikachuMapFileID, &llGRBonus2PikachuMapMapHeader }, // Board the Platforms Pikachu
    { &llGRBonus2PurinMapFileID,   &llGRBonus2PurinMapMapHeader }, // Board the Platforms Jigglypuff
    { &llGRBonus2NessMapFileID,    &llGRBonus2NessMapMapHeader }  // Board the Platforms Ness
};

// 0x8012C668 - I can't for the life of me figure out if these are referenced at all
f32 dMPCollisionUnusedFloats[/* */] =
{
    5000.0, -2500.0, 7000.0, -7000.0,
    9000.0, -5000.0, 12000.0, -12000.0,
    20.0, 45.0, 1557.3, 0.0,
    2670.0, -2670.0, 4000.0, -2500.0,
    5500.0, -5500.0, 9000.0, -5000.0,
    11000.0, -11000.0, 20.0, 45.0,
    1550.4, 0.0, 2016.9, -2016.9,
    5000.0, -2500.0, 6500.0, -6500.0,
    9000.0, -5000.0, 11500.0, -11500.0,
    0.0, 90.0, 1570.5, 0.0,
    2668.8, -2782.8, 4000.0, -2500.0,
    5500.0, -5500.0, 9000.0, -5000.0,
    11000.0, -11000.0, 20.0, 45.0,
    1550.4, 0.0, 2016.9, -2016.9,
    5000.0, -2500.0, 7000.0, -7000.0,
    9000.0, -5000.0, 12000.0, -12000.0,
    20.0, 45.0, 1557.3, 0.0,
    2670.0, -2670.0, 5000.0, -2500.0,
    7000.0, -7000.0, 9000.0, -5000.0,
    12000.0, -12000.0, 20.0, 45.0,
    1557.3, 0.0, 2670.0, -2670.0,
    5000.0, -2500.0, 7000.0, -7000.0,
    9000.0, -5000.0, 12000.0, -12000.0,
    20.0, 45.0, 1557.3, 0.0,
    2670.0, -2670.0, 5000.0, -2500.0,
    7000.0, -7000.0, 9000.0, -5000.0,
    12000.0, -12000.0, 20.0, 45.0,
    1557.3, 0.0, 2670.0, -2670.0
};

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x80131300
MPGroundData *gMPCollisionGroundData;

// 0x80131304
MPYakumonoDObj *gMPCollisionYakumonoDObjs;

// 0x80131308
MPAllBounds gMPCollisionBounds;

// 0x80131348
MPLineGroup gMPCollisionLineGroups[nMPLineKindEnumCount];

// 0x80131368
MPGeometryData *gMPCollisionGeometry;

// 0x8013136C
MPVertexInfoContainer *gMPCollisionVertexInfo;

// 0x80131370
MPVertexPosContainer *gMPCollisionVertexData; // Vertex positions

// 0x80131374
MPVertexArray *gMPCollisionVertexIDs;

// 0x80131378
MPVertexLinks *gMPCollisionVertexLinks;

// 0x8013137C - distance each line collision moved
Vec3f *gMPCollisionSpeeds;

// 0x80131380
MPMapObjContainer *gMPCollisionMapObjs;

// 0x80131384
s32 gMPCollisionLinesNum;

// 0x80131388
SYColorRGBA gMPCollisionLightColor;

// 0x8013138C
s32 gMPCollisionYakumonosNum;

// 0x80131390
f32 gMPCollisionLightAngleX;

// 0x80131394
f32 gMPCollisionLightAngleY;

// 0x80131398
u16 gMPCollisionUpdateTic;

// 0x8013139C
u32 gMPCollisionBGMCurrent;

// 0x801313A0
u32 gMPCollisionBGMDefault;

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x800F39F0
f32 mpCollisionGetLineDistanceFC(f32 opx, s32 v1x, s32 v1y, s32 v2x, s32 v2y)
{
    return v1y + (((opx - v1x) / (v2x - v1x)) * (v2y - v1y));
}

// 0x800F3A34
f32 mpCollisionGetLineDistanceLR(f32 opy, s32 v1x, s32 v1y, s32 v2x, s32 v2y)
{
    return v1x + (((opy - v1y) / (v2y - v1y)) * (v2x - v1x));
}

// 0x800F3A78
sb32 mpCollisionGetFCCommon(s32 line_id, Vec3f *object_pos, f32 *dist, u32 *flags, Vec3f *angle, s32 ud)
{
    MPVertexLinks *vlinks;
    DObj *yakumono_dobj;
    f32 vnear;
    u16 *pv;
    s32 v1x;
    s32 v1y;
    s32 v2x;
    s32 v2y;
    f32 vfar;
    f32 object_pos_x;
    f32 object_pos_y;
    s32 vid;
    u16 vertex1;
    s32 i;

    if ((line_id == -1) || (line_id == -2)) // -1 = not above ground, -2 = ???
    {
        while (TRUE)
        {
            syDebugPrintf("mpGetUUCommon() id = %d\n", line_id); // Since we're exclusively checking ground collision and the line ID passed isn't ground-valid, report the error

            scManagerRunPrintGObjStatus();
        }
    }
    yakumono_dobj = gMPCollisionYakumonoDObjs->dobjs[gMPCollisionVertexInfo->vertex_info[line_id].yakumono_id];

    if (yakumono_dobj->user_data.s >= nMPYakumonoStatusOff)
    {
        while (TRUE)
        {
            syDebugPrintf("mpGetUUCommon() no collsion\n");

            scManagerRunPrintGObjStatus();
        }
    }
    vlinks = &gMPCollisionVertexLinks[line_id];

    if ((yakumono_dobj->anim_joint.event32 != NULL) || (yakumono_dobj->user_data.s != nMPYakumonoStatusNone))
    {
        object_pos_x = object_pos->x - yakumono_dobj->translate.vec.f.x;
        object_pos_y = object_pos->y - yakumono_dobj->translate.vec.f.y;
    }
    else
    {
        object_pos_x = object_pos->x;
        object_pos_y = object_pos->y;
    }
    vertex1 = vlinks->vertex1;
    pv = &gMPCollisionVertexIDs->vertex_id[vertex1];
    v1x = gMPCollisionVertexData->vpos[pv[0]].pos.x;
    vid = vlinks->vertex2 - 1;
    v2x = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex1 + vid]].pos.x;

    if (v2x < v1x)
    {
        vfar = v1x;
        vnear = v2x;
    }
    else
    {
        vfar = v2x;
        vnear = v1x;
    }
    if ((object_pos_x <= (vnear - 0.001F)) || ((vfar + 0.001F) <= object_pos_x))
    {
        return FALSE;
    }
    if (object_pos_x <= vnear)
    {
        object_pos_x = vnear;
    }
    else if (vfar <= object_pos_x)
    {
        object_pos_x = vfar;
    }
    if (vid != 1)
    {
        for (i = 0; i < vid; i++, pv++)
        {
            v1x = gMPCollisionVertexData->vpos[pv[0]].pos.x;
            v2x = gMPCollisionVertexData->vpos[pv[1]].pos.x;

            if (v2x == v1x)
            {
                while (TRUE)
                {
                    syDebugPrintf("same vtx x error mpGetUUCommon\n");
                    scManagerRunPrintGObjStatus();
                }
            }
            if (((v1x <= object_pos_x) && (object_pos_x <= v2x)) || ((v2x <= object_pos_x) && (object_pos_x <= v1x)))
            {
                break;
            }
        }
    }
    v1y = gMPCollisionVertexData->vpos[pv[0]].pos.y;
    v2y = gMPCollisionVertexData->vpos[pv[1]].pos.y;

    if (flags != NULL)
    {
        *flags = gMPCollisionVertexData->vpos[pv[0]].vertex_flags;
    }
    if (dist != NULL)
    {
        *dist = mpCollisionGetLineDistanceFC(object_pos_x, v1x, v1y, v2x, v2y) - object_pos_y;
    }
    if (angle != NULL)
    {
        mpCollisionGetFCAngle(angle, v1x, v1y, v2x, v2y, ud);
    }
    return TRUE;
}

// 0x800F3DD8
sb32 mpCollisionGetFCCommonFloor(s32 line_id, Vec3f *object_pos, f32 *dist, u32 *flags, Vec3f *angle)
{
    return mpCollisionGetFCCommon(line_id, object_pos, dist, flags, angle, +1);
}

// 0x800F3E04
sb32 mpCollisionGetFCCommonCeil(s32 line_id, Vec3f *object_pos, f32 *dist, u32 *flags, Vec3f *angle)
{
    return mpCollisionGetFCCommon(line_id, object_pos, dist, flags, angle, -1);
}

// 0x800F3E30
sb32 mpCollisionGetLRCommon(s32 line_id, Vec3f *object_pos, f32 *dist, u32 *flags, Vec3f *angle, s32 lr)
{
    MPVertexLinks *vlinks;
    DObj *yakumono_dobj;
    f32 vnear;
    f32 vfar;
    s32 v1x;
    s32 v1y;
    s32 v2x;
    s32 v2y;
    f32 object_pos_y;
    f32 object_pos_x;
    s32 vid;
    u16 vertex1;
    u16 *pv;
    s32 i;

    if ((line_id == -1) || (line_id == -2)) // -1 = not above ground, -2 = ???
    {
        while (TRUE)
        {
            syDebugPrintf("mpGetLRCommon() id = %d\n", line_id);

            scManagerRunPrintGObjStatus();
        }
    }
    yakumono_dobj = gMPCollisionYakumonoDObjs->dobjs[gMPCollisionVertexInfo->vertex_info[line_id].yakumono_id];

    if (yakumono_dobj->user_data.s >= nMPYakumonoStatusOff)
    {
        while (TRUE)
        {
            syDebugPrintf("mpGetLRCommon() no collision\n");

            scManagerRunPrintGObjStatus();
        }
    }
    vlinks = &gMPCollisionVertexLinks[line_id];

    if ((yakumono_dobj->anim_joint.event32 != NULL) || (yakumono_dobj->user_data.s != nMPYakumonoStatusNone))
    {
        object_pos_x = object_pos->x - yakumono_dobj->translate.vec.f.x;
        object_pos_y = object_pos->y - yakumono_dobj->translate.vec.f.y;
    }
    else
    {
        object_pos_x = object_pos->x;
        object_pos_y = object_pos->y;
    }
    vertex1 = vlinks->vertex1;
    pv = &gMPCollisionVertexIDs->vertex_id[vertex1];
    v1y = gMPCollisionVertexData->vpos[pv[0]].pos.y;
    vid = vlinks->vertex2 - 1;
    v2y = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex1 + vid]].pos.y;

    if (v2y < v1y)
    {
        vfar = v1y;
        vnear = v2y;
    }
    else
    {
        vfar = v2y;
        vnear = v1y;
    }
    if ((object_pos_y <= (vnear - 0.001F)) || ((vfar + 0.001F) <= object_pos_y))
    {
        return FALSE;
    }
    if (object_pos_y <= vnear)
    {
        object_pos_y = vnear;
    }
    else if (vfar <= object_pos_y)
    {
        object_pos_y = vfar;
    }
    if (vid != 1)
    {
        for (i = 0; i < vid; i++, pv++)
        {
            v1y = gMPCollisionVertexData->vpos[pv[0]].pos.y;
            v2y = gMPCollisionVertexData->vpos[pv[1]].pos.y;

            if (v2y == v1y)
            {
                while (TRUE)
                {
                    syDebugPrintf("same vtx y error mpGetLRCommon\n");
                    scManagerRunPrintGObjStatus();
                }
            }
            if (((v1y <= object_pos_y) && (object_pos_y <= v2y)) || ((v2y <= object_pos_y) && (object_pos_y <= v1y)))
            {
                break;
            }
        }
    }
    v1x = gMPCollisionVertexData->vpos[pv[0]].pos.x;
    v2x = gMPCollisionVertexData->vpos[pv[1]].pos.x;

    if (flags != NULL)
    {
        *flags = gMPCollisionVertexData->vpos[pv[0]].vertex_flags;
    }
    if (dist != NULL)
    {
        *dist = mpCollisionGetLineDistanceLR(object_pos_y, v1x, v1y, v2x, v2y) - object_pos_x;
    }
    if (angle != NULL)
    {
        mpCollisionGetLRAngle(angle, v1x, v1y, v2x, v2y, lr);
    }
    return TRUE;
}

// 0x800F4194
sb32 mpCollisionGetLRCommonLWall(s32 line_id, Vec3f *object_pos, f32 *dist, u32 *flags, Vec3f *angle)
{
    return mpCollisionGetLRCommon(line_id, object_pos, dist, flags, angle, -1);
}

// 0x800F41C0
sb32 mpCollisionGetLRCommonRWall(s32 line_id, Vec3f *object_pos, f32 *dist, u32 *flags, Vec3f *angle)
{
    return mpCollisionGetLRCommon(line_id, object_pos, dist, flags, angle, +1);
}

// 0x800F41EC
void mpCollisionGetLREdge(s32 line_id, Vec3f *object_pos, s32 lr)
{
    MPVertexLinks *vlinks;
    DObj *yakumono_dobj;
    s32 v1x;
    s32 v1y;
    s32 v2x;
    s32 v2y;
    s32 vid;
    u16 vertex1;
    sb32 cmp_lt;

    if ((line_id == -1) || (line_id == -2))
    {
        while (TRUE)
        {
            syDebugPrintf("mpGetLREdge() id = %d\n", line_id);
            scManagerRunPrintGObjStatus();
        }
    }
    yakumono_dobj = gMPCollisionYakumonoDObjs->dobjs[gMPCollisionVertexInfo->vertex_info[line_id].yakumono_id];

    if (yakumono_dobj->user_data.s >= nMPYakumonoStatusOff)
    {
        while (TRUE)
        {
            syDebugPrintf("mpGetLREdge() no collision\n");
            scManagerRunPrintGObjStatus();
        }
    }
    vlinks = &gMPCollisionVertexLinks[line_id];
    vertex1 = vlinks->vertex1;
    vid = (vlinks->vertex2 + vertex1) - 1;
    v1x = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex1]].pos.x;
    v2x = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vid]].pos.x;

    cmp_lt = (lr < 0) ? (v1x < v2x) : (v2x < v1x);

    if (cmp_lt != FALSE)
    {
        object_pos->x = v1x;
        object_pos->y = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex1]].pos.y;
    }
    else
    {
        object_pos->x = v2x;
        object_pos->y = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vid]].pos.y;
    }
    object_pos->z = 0.0F;

    if ((yakumono_dobj->anim_joint.event32 != NULL) || (yakumono_dobj->user_data.s != nMPYakumonoStatusNone))
    {
        object_pos->x += yakumono_dobj->translate.vec.f.x;
        object_pos->y += yakumono_dobj->translate.vec.f.y;
    }
}

// 0x800F4408 - Get right edge of ground line ID
void mpCollisionGetFloorEdgeR(s32 line_id, Vec3f *object_pos)
{
    mpCollisionGetLREdge(line_id, object_pos, +1);
}

// 0x800F4428 - Get left edge of ground line ID
void mpCollisionGetFloorEdgeL(s32 line_id, Vec3f *object_pos)
{
    mpCollisionGetLREdge(line_id, object_pos, -1);
}

// 0x800F4448 - Get right edge of ceiling line ID (!?)
void mpCollisionGetCeilEdgeR(s32 line_id, Vec3f *object_pos)
{
    mpCollisionGetLREdge(line_id, object_pos, +1);
}

// 0x800F4468 - Get left edge of ceiling line ID (!?)
void mpCollisionGetCeilEdgeL(s32 line_id, Vec3f *object_pos)
{
    mpCollisionGetLREdge(line_id, object_pos, -1);
}

// 0x800F4488
void mpCollisionGetUDEdge(s32 line_id, Vec3f *object_pos, s32 ud)
{
    MPVertexLinks *vlinks;
    DObj *yakumono_dobj;
    MPVertexData *vpos1;
    MPVertexData *vpos2;
    s32 v1x;
    s32 v1y;
    s32 v2x;
    s32 v2y;
    s32 vid;
    u16 vertex1;
    sb32 cmp_lt;

    if ((line_id == -1) || (line_id == -2))
    {
        while (TRUE)
        {
            syDebugPrintf("mpGetUDEdge() id = %d\n", line_id);
            scManagerRunPrintGObjStatus();
        }
    }
    yakumono_dobj = gMPCollisionYakumonoDObjs->dobjs[gMPCollisionVertexInfo->vertex_info[line_id].yakumono_id];

    if (yakumono_dobj->user_data.s >= nMPYakumonoStatusOff)
    {
        while (TRUE)
        {
            syDebugPrintf("mpGetUDEdge() no collision\n");
            scManagerRunPrintGObjStatus();
        }
    }
    vlinks = &gMPCollisionVertexLinks[line_id];
    vertex1 = vlinks->vertex1;
    vid = (vlinks->vertex2 + vertex1) - 1;
    vpos1 = &gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex1]];
    vpos2 = &gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vid]];

    v1y = vpos1->pos.y;
    v2y = vpos2->pos.y;

    cmp_lt = (ud < 0) ? (v1y < v2y) : (v2y < v1y);

    if (cmp_lt != FALSE)
    {
        object_pos->x = vpos1->pos.x;
        object_pos->y = v1y;
    }
    else
    {
        object_pos->x = vpos2->pos.x;
        object_pos->y = v2y;
    }
    object_pos->z = 0.0F;

    if ((yakumono_dobj->anim_joint.event32 != NULL) || (yakumono_dobj->user_data.s != nMPYakumonoStatusNone))
    {
        object_pos->x += yakumono_dobj->translate.vec.f.x;
        object_pos->y += yakumono_dobj->translate.vec.f.y;
    }
}

// 0x800F4650 - Get upper edge of left wall
void mpCollisionGetLWallEdgeU(s32 line_id, Vec3f *object_pos)
{
    mpCollisionGetUDEdge(line_id, object_pos, +1);
}

// 0x800F4670 - Get lower edge of left wall
void mpCollisionGetLWallEdgeD(s32 line_id, Vec3f *object_pos)
{
    mpCollisionGetUDEdge(line_id, object_pos, -1);
}

// 0x800F4690 - Get upper edge of right wall
void mpCollisionGetRWallEdgeU(s32 line_id, Vec3f *object_pos)
{
    mpCollisionGetUDEdge(line_id, object_pos, +1);
}

// 0x800F46B0 - Get lower edge of right wall
void mpCollisionGetRWallEdgeD(s32 line_id, Vec3f *object_pos)
{
    mpCollisionGetUDEdge(line_id, object_pos, -1);
}

// 0x800F46D0
void mpCollisionGetFCAngle(Vec3f *angle, s32 v1x, s32 v1y, s32 v2x, s32 v2y, s32 ud)
{
    f32 py, sqrt_py;
    f32 dist_y;

    angle->z = 0.0F;

    dist_y = (v2y - v1y);

    if (dist_y == 0.0F)
    {
        angle->x = 0.0F;
        angle->y = ud;

        return;
    }
    py = -(dist_y / (v2x - v1x));

    sqrt_py = 1.0F / sqrtf(SQUARE(py) + 1.0F);

    if (ud < 0)
    {
        angle->x = (-py * sqrt_py);
        angle->y = -sqrt_py;
    }
    else
    {
        angle->x = (py * sqrt_py);
        angle->y = sqrt_py;
    }
    syVectorNorm3D(angle);
}

// 0x800F47AC - Also calculates angle? Runs whenever character lands directly on a line surface that isn't 0 degrees
sb32 mpCollisionCheckFloorSurfaceTilt(s32 v1x, s32 v1y, s32 v2x, s32 v2y, f32 d1x, f32 d1y, f32 d2x, f32 d2y, f32 *dfx, f32 *dfy)
{
    s32 vfary;
    s32 vneary;
    f32 ddist_x;
    f32 ddist_y;
    s32 vfarx;
    s32 vnearx;
    s32 vdist_y;
    s32 vdist_x;
    f32 temp_f12_2;
    f32 temp_f14;
    f32 vddiv;
    f32 vddist_x;
    f32 vddist_y;
    f32 vddistdiv;
    f32 vdscale;
    f32 scale;
    f32 var_f18;

    vdist_y = v2y - v1y;
    ddist_y = d1y - d2y;

    if (vdist_y < 0)
    {
        vneary = v2y;
        vfary = v1y;
    }
    else
    {
        vfary = v2y;
        vneary = v1y;
    }
    if (ddist_y > 0.0F)
    {
        if (((vfary + 0.001F) < d2y) || (d1y < (vneary - 0.001F)))
        {
            return FALSE;
        }
    }
    else if (((vfary + 0.001F) < d1y) || (d2y < (vneary - 0.001F)))
    {
        return FALSE;
    }
    ddist_x = d1x - d2x;
    vdist_x = v2x - v1x;

    if (vdist_x < 0)
    {
        vnearx = v2x;
        vfarx = v1x;
    }
    else
    {
        vfarx = v2x;
        vnearx = v1x;
    }
    if (ddist_x > 0.0F)
    {
        if ((vfarx < d2x) || (d1x < vnearx))
        {
            return FALSE;
        }
    }
    else if ((vfarx < d1x) || (d2x < vnearx))
    {
        return FALSE;
    }
    if ((d2y - (v1y + (((d2x - v1x) / vdist_x) * vdist_y))) <= (-0.001F))
    {
        temp_f12_2 = v1y + (((d1x - v1x) / vdist_x) * vdist_y);

        if ((d1y - temp_f12_2) < 0.001F)
        {
            if (((-0.001F) < (d1y - temp_f12_2)) && (d1x <= vfarx) && (vnearx <= d1x))
            {
                *dfx = d1x;
                *dfy = temp_f12_2;

                return TRUE;
            }
        }
        else
        {
            vddist_x = v1x - d1x;
            vddist_y = v1y - d1y;

            var_f18 = (ddist_y * vddist_x) - (ddist_x * vddist_y);
            scale = (vdist_y * ddist_x) - (vdist_x * ddist_y);

            vddiv = var_f18 / scale;

            if (vddiv < 0.0F)
            {
                if (vddiv < -0.001F)
                {
                    return FALSE;
                }
                var_f18 = 0.0F;
            }
            else if (vddiv > 1.0F)
            {
                if (vddiv > 1.001F)
                {
                    return FALSE;
                }
                var_f18 = scale;
            }
            vddistdiv = ((vdist_x * vddist_y) - (vdist_y * vddist_x)) / scale;

            if ((vddistdiv < -0.001F) || (vddistdiv > 1.001F))
            {
                return FALSE;
            }
            vdscale = 1.0F / scale;

            *dfx = v1x + (var_f18 * vdist_x * vdscale);
            *dfy = v1y + (var_f18 * vdist_y * vdscale);

            return TRUE;
        }
    }
    return FALSE;
}

// 0x800F4BD8
sb32 mpCollisionCheckFloorLineCollisionSame(Vec3f *position, Vec3f *translate, Vec3f *ga_last, s32 *stand_line_id, u32 *stand_coll_flags, Vec3f *angle)
{
    MPLineInfo *line_info;
    MPLineData *line_data;
    DObj *yakumono_dobj;
    s32 i;
    f32 vdist1;
    f32 vdist2;
    f32 vpdist_x, vpdist_y;
    f32 vtdist_x, vtdist_y;
    f32 line_project_pos;
    f32 vedge_x;
    f32 vedge_y;
    s32 line_id, vertex_id;
    s32 temp_s0;
    s32 temp_s1;
    s32 vpos_x;
    s32 vpos_y;
    f32 scrollv;
    f32 spAC;

    line_project_pos = F32_MAX;
    line_info = gMPCollisionGeometry->line_info;

    for (i = 0; i < gMPCollisionGeometry->yakumono_count; i++, line_info++)
    {
        line_data = &line_info->line_data[nMPLineKindFloor];
        yakumono_dobj = gMPCollisionYakumonoDObjs->dobjs[line_info->yakumono_id];

        if ((line_data->line_count != 0) && (yakumono_dobj->user_data.s < nMPYakumonoStatusOff))
        {
            if ((yakumono_dobj->anim_joint.event32 != NULL) || (yakumono_dobj->user_data.s != nMPYakumonoStatusNone))
            {
                // This runs when the corner of the character's map collision diamond collides with a wall?
                vedge_x = yakumono_dobj->translate.vec.f.x;
                vedge_y = yakumono_dobj->translate.vec.f.y;

                vpdist_x = position->x - vedge_x;
                vpdist_y = position->y - vedge_y;

                vtdist_x = translate->x - vedge_x;
                vtdist_y = translate->y - vedge_y;
            }
            else
            {
                vedge_x = vedge_y = 0.0F;

                vpdist_x = position->x;
                vpdist_y = position->y;

                vtdist_x = translate->x;
                vtdist_y = translate->y;
            }
            if (vpdist_y < vtdist_y)
            {
                vdist1 = vpdist_y - 0.001F;
                vdist2 = vtdist_y + 0.001F;
            }
            else
            {
                vdist1 = vtdist_y - 0.001F;
                vdist2 = vpdist_y + 0.001F;
            }
            for (line_id = line_data->group_id; line_id < (line_data->group_id + line_data->line_count); line_id++)
            {
                if (gMPCollisionVertexInfo->vertex_info[line_id].coll_pos_prev > vdist2)
                {
                    continue;
                }
                else if (gMPCollisionVertexInfo->vertex_info[line_id].coll_pos_next < vdist1)
                {
                    goto l_break; // Bruhhhhhhhhhhhhhhh
                }
                else
                {
                    vertex_id = gMPCollisionVertexLinks[line_id].vertex1;

                    vpos_x = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id]].pos.x;
                    vpos_y = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id]].pos.y;

                    for (vertex_id = gMPCollisionVertexLinks[line_id].vertex1; vertex_id < (gMPCollisionVertexLinks[line_id].vertex1 + gMPCollisionVertexLinks[line_id].vertex2) - 1; vertex_id++)
                    {
                        temp_s1 = vpos_x;
                        temp_s0 = vpos_y;

                        vpos_x = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id + 1]].pos.x;
                        vpos_y = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id + 1]].pos.y;

                        if (temp_s0 == vpos_y)
                        {
                            if (vtdist_y < vpdist_y)
                            {
                                if (mpCollisionCheckFCSurfaceFlat(temp_s1, temp_s0, vpos_x, vpdist_x, vpdist_y, vtdist_x, vtdist_y, &scrollv, &spAC) == TRUE)
                                {
                                    if (line_project_pos <= ((spAC < vpdist_y) ? -(spAC - vpdist_y) : (spAC - vpdist_y))) continue;

                                    if (ga_last != NULL)
                                    {
                                        ga_last->x = scrollv + vedge_x;
                                        ga_last->y = spAC + vedge_y;
                                        ga_last->z = 0.0F;
                                    }
                                    if (stand_line_id != NULL)
                                    {
                                        *stand_line_id = line_id;
                                    }
                                    if (angle != NULL)
                                    {
                                        angle->x = angle->z = 0.0F;
                                        angle->y = 1.0F;
                                    }
                                    if (stand_coll_flags != NULL)
                                    {
                                        *stand_coll_flags = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id]].vertex_flags;
                                    }
                                    line_project_pos = (spAC < vpdist_y) ? -(spAC - vpdist_y) : (spAC - vpdist_y);
                                }
                            }
                        }
                        else if (mpCollisionCheckFloorSurfaceTilt(temp_s1, temp_s0, vpos_x, vpos_y, vpdist_x, vpdist_y, vtdist_x, vtdist_y, &scrollv, &spAC) == TRUE)
                        {
                            if (line_project_pos <= ((spAC < vpdist_y) ? -(spAC - vpdist_y) : (spAC - vpdist_y))) continue;

                            if (ga_last != NULL)
                            {
                                ga_last->x = scrollv + vedge_x;
                                ga_last->y = spAC + vedge_y;
                                ga_last->z = 0.0F;
                            }
                            if (stand_line_id != NULL)
                            {
                                *stand_line_id = line_id;
                            }
                            if (angle != NULL)
                            {
                                mpCollisionGetFCAngle(angle, temp_s1, temp_s0, vpos_x, vpos_y, +1);
                            }
                            if (stand_coll_flags != NULL)
                            {
                                *stand_coll_flags = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id]].vertex_flags;
                            }
                            line_project_pos = (spAC < vpdist_y) ? -(spAC - vpdist_y) : (spAC - vpdist_y);
                        }
                    }
                }
            }
        l_break: // BRUHHHHHHHHHHHHHHHHH
            continue;
        }
    }
    if (line_project_pos == F32_MAX)
    {
        return FALSE;
    }
    else return TRUE;
}

// 0x800F521C
sb32 mpCollisionCheckFloorLineCollisionDiff(Vec3f *position, Vec3f *translate, Vec3f *ga_last, s32 *stand_line_id, u32 *stand_coll_flags, Vec3f *angle)
{
    MPLineInfo *line_info;
    MPLineData *line_data;
    DObj *yakumono_dobj;
    s32 i;
    f32 vdist1;
    f32 vdist2;
    f32 vpdist_x, vpdist_y;
    f32 vtdist_x, vtdist_y;
    f32 line_project_pos;
    f32 vedge_y;
    f32 dynamic_y;
    f32 vedge_x;
    s32 line_id, vertex_id;
    s32 temp_s0;
    s32 temp_s1;
    s32 vpos_x;
    s32 vpos_y;
    f32 scrollv;
    f32 spAC;

    line_project_pos = F32_MAX;
    line_info = gMPCollisionGeometry->line_info;

    for (i = 0; i < gMPCollisionGeometry->yakumono_count; i++, line_info++)
    {
        line_data = &line_info->line_data[nMPLineKindFloor];
        yakumono_dobj = gMPCollisionYakumonoDObjs->dobjs[line_info->yakumono_id];

        if ((line_data->line_count != 0) && (yakumono_dobj->user_data.s < nMPYakumonoStatusOff))
        {
            if ((yakumono_dobj->anim_joint.event32 != NULL) || (yakumono_dobj->user_data.s != nMPYakumonoStatusNone))
            {
                // This runs when the corner of the character's map collision diamond collides with a wall?
                vedge_x = yakumono_dobj->translate.vec.f.x;
                vedge_y = yakumono_dobj->translate.vec.f.y;

                dynamic_y = gMPCollisionSpeeds[line_info->yakumono_id].y;

                vpdist_x = (position->x - vedge_x) + gMPCollisionSpeeds[line_info->yakumono_id].x;
                vpdist_y = (position->y - vedge_y) + dynamic_y;

                vtdist_x = translate->x - vedge_x;
                vtdist_y = translate->y - vedge_y;
            }
            else
            {
                dynamic_y = 0.0F;
                vedge_x = vedge_y = 0.0F;

                vpdist_x = position->x;
                vpdist_y = position->y;

                vtdist_x = translate->x;
                vtdist_y = translate->y;
            }
            if (vpdist_y < vtdist_y)
            {
                vdist1 = vpdist_y - 0.001F;
                vdist2 = vtdist_y + 0.001F;
            }
            else
            {
                vdist1 = vtdist_y - 0.001F;
                vdist2 = vpdist_y + 0.001F;
            }
            for (line_id = line_data->group_id; line_id < (line_data->group_id + line_data->line_count); line_id++)
            {
                if (gMPCollisionVertexInfo->vertex_info[line_id].coll_pos_prev > vdist2)
                {
                    continue;
                }
                if (gMPCollisionVertexInfo->vertex_info[line_id].coll_pos_next < vdist1)
                {
                    goto l_break; // Bruhhhhhhhhhhhhhhh
                }
                else
                {
                    vertex_id = gMPCollisionVertexLinks[line_id].vertex1;

                    vpos_x = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id]].pos.x;
                    vpos_y = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id]].pos.y;

                    for (vertex_id = gMPCollisionVertexLinks[line_id].vertex1; vertex_id < (gMPCollisionVertexLinks[line_id].vertex1 + gMPCollisionVertexLinks[line_id].vertex2) - 1; vertex_id++)
                    {
                        temp_s1 = vpos_x;
                        temp_s0 = vpos_y;

                        vpos_x = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id + 1]].pos.x;
                        vpos_y = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id + 1]].pos.y;

                        if (temp_s0 == vpos_y)
                        {
                            if (vtdist_y < vpdist_y)
                            {
                                if (mpCollisionCheckFCSurfaceFlat(temp_s1, temp_s0, vpos_x, vpdist_x, vpdist_y, vtdist_x, vtdist_y, &scrollv, &spAC) == TRUE)
                                {
                                    if (line_project_pos <= ((spAC < (vpdist_y - dynamic_y)) ? -(spAC - (vpdist_y - dynamic_y)) : (spAC - (vpdist_y - dynamic_y)))) continue;

                                    if (ga_last != NULL)
                                    {
                                        ga_last->x = scrollv + vedge_x;
                                        ga_last->y = spAC + vedge_y;
                                        ga_last->z = 0.0F;
                                    }
                                    if (stand_line_id != NULL)
                                    {
                                        *stand_line_id = line_id;
                                    }
                                    if (angle != NULL)
                                    {
                                        angle->x = angle->z = 0.0F;
                                        angle->y = 1.0F;
                                    }
                                    if (stand_coll_flags != NULL)
                                    {
                                        *stand_coll_flags = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id]].vertex_flags;
                                    }
                                    line_project_pos = (spAC < (vpdist_y - dynamic_y)) ? -(spAC - (vpdist_y - dynamic_y)) : (spAC - (vpdist_y - dynamic_y));
                                }
                            }
                        }
                        else if (mpCollisionCheckFloorSurfaceTilt(temp_s1, temp_s0, vpos_x, vpos_y, vpdist_x, vpdist_y, vtdist_x, vtdist_y, &scrollv, &spAC) == TRUE)
                        {
                            if (line_project_pos <= ((spAC < (vpdist_y - dynamic_y)) ? -(spAC - (vpdist_y - dynamic_y)) : (spAC - (vpdist_y - dynamic_y)))) continue;

                            if (ga_last != NULL)
                            {
                                ga_last->x = scrollv + vedge_x;
                                ga_last->y = spAC + vedge_y;
                                ga_last->z = 0.0F;
                            }
                            if (stand_line_id != NULL)
                            {
                                *stand_line_id = line_id;
                            }
                            if (angle != NULL)
                            {
                                mpCollisionGetFCAngle(angle, temp_s1, temp_s0, vpos_x, vpos_y, +1);
                            }
                            if (stand_coll_flags != NULL)
                            {
                                *stand_coll_flags = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id]].vertex_flags;
                            }
                            line_project_pos = (spAC < (vpdist_y - dynamic_y)) ? -(spAC - (vpdist_y - dynamic_y)) : (spAC - (vpdist_y - dynamic_y));
                        }
                    }
                }
            }
        l_break: // BRUHHHHHHHHHHHHHHHHH
            continue;
        }
    }
    if (line_project_pos == F32_MAX)
    {
        return FALSE;
    }
    else return TRUE;
}

// 0x800F58A0
sb32 mpCollisionCheckFCSurfaceFlat(s32 v1x, s32 v1y, s32 vpos_y, f32 vpdist_x, f32 vpdist_y, f32 vtdist_x, f32 vtdist_y, f32 *arg7, f32 *arg8)
{
    f32 temp_f18;
    s32 vnear;
    s32 vfar;
    f32 ddist_x;
    s32 vdist_y = vpos_y - v1x;
    f32 ddist_y = vpdist_y - vtdist_y;
    f32 vddist_y = v1y - vpdist_y;

    if (ddist_y > 0.0F)
    {
        if (((v1y - 0.001F) > vpdist_y) || (vtdist_y >= v1y))
        {
            return FALSE;
        }
    }
    else if (((v1y + 0.001F) < vpdist_y) || (vtdist_y <= v1y))
    {
        return FALSE;
    }
    ddist_x = vpdist_x - vtdist_x;

    if (vdist_y < 0)
    {
        vnear = vpos_y;
        vfar = v1x;
    }
    else
    {
        vfar = vpos_y;
        vnear = v1x;
    }

    if (ddist_x > 0.0F)
    {
        if ((vfar < vtdist_x) || (vpdist_x < vnear))
        {
            return FALSE;
        }
    }
    else if ((vfar < vpdist_x) || (vtdist_x < vnear))
    {
        return FALSE;
    }
    temp_f18 = ((vddist_y / ddist_y) * ddist_x) + vpdist_x;

    if ((temp_f18 < vnear) || (temp_f18 > vfar))
    {
        return FALSE;
    }

    *arg7 = temp_f18;
    *arg8 = v1y;

    return TRUE;
}

// 0x800F5A64
sb32 mpCollisionCheckCeilSurfaceTilt(s32 v1x, s32 v1y, s32 v2x, s32 v2y, f32 d1x, f32 d1y, f32 d2x, f32 d2y, f32 *dfx, f32 *dfy)
{
    s32 vfary;
    s32 vneary;
    f32 ddist_x;
    f32 ddist_y;
    s32 vfarx;
    s32 vnearx;
    s32 vdist_y;
    s32 vdist_x;
    f32 temp_f12_2;
    f32 temp_f14;
    f32 vddiv;
    f32 vddist_x;
    f32 vddist_y;
    f32 vddistdiv;
    f32 vdscale;
    f32 scale;
    f32 var_f18;

    vdist_y = v2y - v1y;
    ddist_y = d1y - d2y;

    if (vdist_y < 0)
    {
        vneary = v2y;
        vfary = v1y;
    }
    else
    {
        vfary = v2y;
        vneary = v1y;
    }
    if (ddist_y > 0.0F)
    {
        if (((vfary + 0.001F) < d2y) || (d1y < (vneary - 0.001F)))
        {
            return FALSE;
        }
    }
    else if (((vfary + 0.001F) < d1y) || (d2y < (vneary - 0.001F)))
    {
        return FALSE;
    }
    ddist_x = d1x - d2x;
    vdist_x = v2x - v1x;

    if (vdist_x < 0)
    {
        vnearx = v2x;
        vfarx = v1x;
    }
    else
    {
        vfarx = v2x;
        vnearx = v1x;
    }
    if (ddist_x > 0.0F)
    {
        if ((vfarx < d2x) || (d1x < vnearx))
        {
            return FALSE;
        }
    }
    else if ((vfarx < d1x) || (d2x < vnearx))
    {
        return FALSE;
    }
    if ((d2y - (v1y + (((d2x - v1x) / vdist_x) * vdist_y))) >= 0.001F)
    {
        temp_f12_2 = v1y + (((d1x - v1x) / vdist_x) * vdist_y);

        if ((d1y - temp_f12_2) > -0.001F)
        {
            if (((d1y - temp_f12_2) < 0.001F) && (d1x <= vfarx) && (vnearx <= d1x))
            {
                *dfx = d1x;
                *dfy = temp_f12_2;

                return TRUE;
            }
        }
        else
        {
            vddist_x = v1x - d1x;
            vddist_y = v1y - d1y;

            var_f18 = (ddist_y * vddist_x) - (ddist_x * vddist_y);
            scale = (vdist_y * ddist_x) - (vdist_x * ddist_y);
            vddiv = var_f18 / scale;

            if (vddiv < 0.0F)
            {
                if (vddiv < -0.001F)
                {
                    return FALSE;
                }
                var_f18 = 0.0F;
            }
            else if (vddiv > 1.0F)
            {
                if (vddiv > 1.001F)
                {
                    return FALSE;
                }
                var_f18 = scale;
            }
            vddistdiv = ((vdist_x * vddist_y) - (vdist_y * vddist_x)) / scale;

            if ((vddistdiv < -0.001F) || (vddistdiv > 1.001F))
            {
                return FALSE;
            }
            vdscale = 1.0F / scale;

            *dfx = v1x + (var_f18 * vdist_x * vdscale);
            *dfy = v1y + (var_f18 * vdist_y * vdscale);

            return TRUE;
        }
    }
    return FALSE;
}

// 0x800F5E90
sb32 mpCollisionCheckCeilLineCollisionSame(Vec3f *position, Vec3f *translate, Vec3f *ga_last, s32 *stand_line_id, u32 *stand_coll_flags, Vec3f *angle)
{
    MPLineInfo *line_info;
    MPLineData *line_data;
    DObj *yakumono_dobj;
    s32 i;
    f32 vdist1;
    f32 vdist2;
    f32 vpdist_x, vpdist_y;
    f32 vtdist_x, vtdist_y;
    f32 line_project_pos;
    f32 vedge_x;
    f32 vedge_y;
    s32 line_id, vertex_id;
    s32 temp_s0;
    s32 temp_s1;
    s32 vpos_x;
    s32 vpos_y;
    f32 scrollv;
    f32 spAC;

    line_project_pos = F32_MAX;
    line_info = gMPCollisionGeometry->line_info;

    for (i = 0; i < gMPCollisionGeometry->yakumono_count; i++, line_info++)
    {
        line_data = &line_info->line_data[nMPLineKindCeil];
        yakumono_dobj = gMPCollisionYakumonoDObjs->dobjs[line_info->yakumono_id];

        if ((line_data->line_count != 0) && (yakumono_dobj->user_data.s < nMPYakumonoStatusOff))
        {
            if ((yakumono_dobj->anim_joint.event32 != NULL) || (yakumono_dobj->user_data.s != nMPYakumonoStatusNone))
            {
                // This runs when the corner of the character's map collision diamond collides with a wall?
                vedge_x = yakumono_dobj->translate.vec.f.x;
                vedge_y = yakumono_dobj->translate.vec.f.y;

                vpdist_x = position->x - vedge_x;
                vpdist_y = position->y - vedge_y;

                vtdist_x = translate->x - vedge_x;
                vtdist_y = translate->y - vedge_y;
            }
            else
            {
                vedge_x = vedge_y = 0.0F;

                vpdist_x = position->x;
                vpdist_y = position->y;

                vtdist_x = translate->x;
                vtdist_y = translate->y;
            }
            if (vpdist_y < vtdist_y)
            {
                vdist1 = vpdist_y - 0.001F;
                vdist2 = vtdist_y + 0.001F;
            }
            else
            {
                vdist1 = vtdist_y - 0.001F;
                vdist2 = vpdist_y + 0.001F;
            }
            for (line_id = line_data->group_id; line_id < (line_data->group_id + line_data->line_count); line_id++)
            {
                if (gMPCollisionVertexInfo->vertex_info[line_id].coll_pos_next < vdist1)
                {
                    continue;
                }
                else if (gMPCollisionVertexInfo->vertex_info[line_id].coll_pos_prev > vdist2)
                {
                    goto l_break; // Bruhhhhhhhhhhhhhhh
                }
                else
                {
                    vertex_id = gMPCollisionVertexLinks[line_id].vertex1;

                    vpos_x = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id]].pos.x;
                    vpos_y = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id]].pos.y;

                    for (vertex_id = gMPCollisionVertexLinks[line_id].vertex1; vertex_id < (gMPCollisionVertexLinks[line_id].vertex1 + gMPCollisionVertexLinks[line_id].vertex2) - 1; vertex_id++)
                    {
                        temp_s1 = vpos_x;
                        temp_s0 = vpos_y;

                        vpos_x = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id + 1]].pos.x;
                        vpos_y = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id + 1]].pos.y;

                        if (temp_s0 == vpos_y)
                        {
                            if (vtdist_y > vpdist_y)
                            {
                                if (mpCollisionCheckFCSurfaceFlat(temp_s1, temp_s0, vpos_x, vpdist_x, vpdist_y, vtdist_x, vtdist_y, &scrollv, &spAC) == TRUE)
                                {
                                    if (line_project_pos <= ((spAC < vpdist_y) ? -(spAC - vpdist_y) : (spAC - vpdist_y))) continue;

                                    if (ga_last != NULL)
                                    {
                                        ga_last->x = scrollv + vedge_x;
                                        ga_last->y = spAC + vedge_y;
                                        ga_last->z = 0.0F;
                                    }
                                    if (stand_line_id != NULL)
                                    {
                                        *stand_line_id = line_id;
                                    }
                                    if (angle != NULL)
                                    {
                                        angle->x = angle->z = 0.0F;
                                        angle->y = -1.0F;
                                    }
                                    if (stand_coll_flags != NULL)
                                    {
                                        *stand_coll_flags = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id]].vertex_flags;
                                    }
                                    line_project_pos = (spAC < vpdist_y) ? -(spAC - vpdist_y) : (spAC - vpdist_y);
                                }
                            }
                        }
                        else if (mpCollisionCheckCeilSurfaceTilt(temp_s1, temp_s0, vpos_x, vpos_y, vpdist_x, vpdist_y, vtdist_x, vtdist_y, &scrollv, &spAC) == TRUE)
                        {
                            if (line_project_pos <= ((spAC < vpdist_y) ? -(spAC - vpdist_y) : (spAC - vpdist_y))) continue;

                            if (ga_last != NULL)
                            {
                                ga_last->x = scrollv + vedge_x;
                                ga_last->y = spAC + vedge_y;
                                ga_last->z = 0.0F;
                            }
                            if (stand_line_id != NULL)
                            {
                                *stand_line_id = line_id;
                            }
                            if (angle != NULL)
                            {
                                mpCollisionGetFCAngle(angle, temp_s1, temp_s0, vpos_x, vpos_y, -1);
                            }
                            if (stand_coll_flags != NULL)
                            {
                                *stand_coll_flags = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id]].vertex_flags;
                            }
                            line_project_pos = (spAC < vpdist_y) ? -(spAC - vpdist_y) : (spAC - vpdist_y);
                        }
                    }
                }
            }
        l_break: // BRUHHHHHHHHHHHHHHHHH
            continue;
        }
    }
    if (line_project_pos == F32_MAX)
    {
        return FALSE;
    }
    else return TRUE;
}

// 0x800F64D4
sb32 mpCollisionCheckCeilLineCollisionDiff(Vec3f *position, Vec3f *translate, Vec3f *ga_last, s32 *stand_line_id, u32 *stand_coll_flags, Vec3f *angle)
{
    MPLineInfo *line_info;
    MPLineData *line_data;
    DObj *yakumono_dobj;
    s32 i;
    f32 vdist1;
    f32 vdist2;
    f32 vpdist_x, vpdist_y;
    f32 vtdist_x, vtdist_y;
    f32 line_project_pos;
    f32 vedge_y;
    f32 dynamic_y;
    f32 vedge_x;
    s32 line_id, vertex_id;
    s32 temp_s0;
    s32 temp_s1;
    s32 vpos_x;
    s32 vpos_y;
    f32 scrollv;
    f32 spAC;

    line_project_pos = F32_MAX;
    line_info = gMPCollisionGeometry->line_info;

    for (i = 0; i < gMPCollisionGeometry->yakumono_count; i++, line_info++)
    {
        line_data = &line_info->line_data[nMPLineKindCeil];
        yakumono_dobj = gMPCollisionYakumonoDObjs->dobjs[line_info->yakumono_id];

        if ((line_data->line_count != 0) && (yakumono_dobj->user_data.s < nMPYakumonoStatusOff))
        {
            if ((yakumono_dobj->anim_joint.event32 != NULL) || (yakumono_dobj->user_data.s != nMPYakumonoStatusNone))
            {
                // This runs when the corner of the character's map collision diamond collides with a wall?
                vedge_x = yakumono_dobj->translate.vec.f.x;
                vedge_y = yakumono_dobj->translate.vec.f.y;

                dynamic_y = gMPCollisionSpeeds[line_info->yakumono_id].y;

                vpdist_x = (position->x - vedge_x) + gMPCollisionSpeeds[line_info->yakumono_id].x;
                vpdist_y = (position->y - vedge_y) + dynamic_y;

                vtdist_x = translate->x - vedge_x;
                vtdist_y = translate->y - vedge_y;
            }
            else
            {
                dynamic_y = 0.0F;
                vedge_x = vedge_y = 0.0F;

                vpdist_x = position->x;
                vpdist_y = position->y;

                vtdist_x = translate->x;
                vtdist_y = translate->y;
            }
            if (vpdist_y < vtdist_y)
            {
                vdist1 = vpdist_y - 0.001F;
                vdist2 = vtdist_y + 0.001F;
            }
            else
            {
                vdist1 = vtdist_y - 0.001F;
                vdist2 = vpdist_y + 0.001F;
            }
            for (line_id = line_data->group_id; line_id < (line_data->group_id + line_data->line_count); line_id++)
            {
                if (gMPCollisionVertexInfo->vertex_info[line_id].coll_pos_next < vdist1)
                {
                    continue;
                }
                if (gMPCollisionVertexInfo->vertex_info[line_id].coll_pos_prev > vdist2)
                {
                    goto l_break; // Bruhhhhhhhhhhhhhhh
                }
                else
                {
                    vertex_id = gMPCollisionVertexLinks[line_id].vertex1;

                    vpos_x = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id]].pos.x;
                    vpos_y = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id]].pos.y;

                    for (vertex_id = gMPCollisionVertexLinks[line_id].vertex1; vertex_id < (gMPCollisionVertexLinks[line_id].vertex1 + gMPCollisionVertexLinks[line_id].vertex2) - 1; vertex_id++)
                    {
                        temp_s1 = vpos_x;
                        temp_s0 = vpos_y;

                        vpos_x = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id + 1]].pos.x;
                        vpos_y = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id + 1]].pos.y;

                        if (temp_s0 == vpos_y)
                        {
                            if (vtdist_y > vpdist_y)
                            {
                                if (mpCollisionCheckFCSurfaceFlat(temp_s1, temp_s0, vpos_x, vpdist_x, vpdist_y, vtdist_x, vtdist_y, &scrollv, &spAC) == TRUE)
                                {
                                    if (line_project_pos <= ((spAC < (vpdist_y - dynamic_y)) ? -(spAC - (vpdist_y - dynamic_y)) : (spAC - (vpdist_y - dynamic_y)))) continue;

                                    if (ga_last != NULL)
                                    {
                                        ga_last->x = scrollv + vedge_x;
                                        ga_last->y = spAC + vedge_y;
                                        ga_last->z = 0.0F;
                                    }
                                    if (stand_line_id != NULL)
                                    {
                                        *stand_line_id = line_id;
                                    }
                                    if (angle != NULL)
                                    {
                                        angle->x = angle->z = 0.0F;
                                        angle->y = -1.0F;
                                    }
                                    if (stand_coll_flags != NULL)
                                    {
                                        *stand_coll_flags = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id]].vertex_flags;
                                    }
                                    line_project_pos = (spAC < (vpdist_y - dynamic_y)) ? -(spAC - (vpdist_y - dynamic_y)) : (spAC - (vpdist_y - dynamic_y));
                                }
                            }
                        }
                        else if (mpCollisionCheckCeilSurfaceTilt(temp_s1, temp_s0, vpos_x, vpos_y, vpdist_x, vpdist_y, vtdist_x, vtdist_y, &scrollv, &spAC) == TRUE)
                        {
                            if (line_project_pos <= ((spAC < (vpdist_y - dynamic_y)) ? -(spAC - (vpdist_y - dynamic_y)) : (spAC - (vpdist_y - dynamic_y)))) continue;

                            if (ga_last != NULL)
                            {
                                ga_last->x = scrollv + vedge_x;
                                ga_last->y = spAC + vedge_y;
                                ga_last->z = 0.0F;
                            }
                            if (stand_line_id != NULL)
                            {
                                *stand_line_id = line_id;
                            }
                            if (angle != NULL)
                            {
                                mpCollisionGetFCAngle(angle, temp_s1, temp_s0, vpos_x, vpos_y, -1);
                            }
                            if (stand_coll_flags != NULL)
                            {
                                *stand_coll_flags = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id]].vertex_flags;
                            }
                            line_project_pos = (spAC < (vpdist_y - dynamic_y)) ? -(spAC - (vpdist_y - dynamic_y)) : (spAC - (vpdist_y - dynamic_y));
                        }
                    }
                }
            }
        l_break: // BRUHHHHHHHHHHHHHHHHH
            continue;
        }
    }
    if (line_project_pos == F32_MAX)
    {
        return FALSE;
    }
    else return TRUE;
}

// 0x800F6B58
sb32 mpCollisionCheckRWallLineCollisionSame(Vec3f *position, Vec3f *translate, Vec3f *ga_last, s32 *stand_line_id, u32 *stand_coll_flags, Vec3f *angle)
{
    MPLineInfo *line_info;
    MPLineData *line_data;
    DObj *yakumono_dobj;
    s32 i;
    f32 vdist1;
    f32 vdist2;
    f32 vpdist_x, vpdist_y;
    f32 vtdist_x, vtdist_y;
    f32 line_project_pos;
    f32 vedge_x;
    f32 vedge_y;
    s32 line_id, vertex_id;
    s32 temp_s0;
    s32 temp_s1;
    s32 vpos_x;
    s32 vpos_y;
    f32 scrollv;
    f32 spAC;

    line_project_pos = F32_MAX;
    line_info = gMPCollisionGeometry->line_info;

    for (i = 0; i < gMPCollisionGeometry->yakumono_count; i++, line_info++)
    {
        line_data = &line_info->line_data[nMPLineKindRWall];
        yakumono_dobj = gMPCollisionYakumonoDObjs->dobjs[line_info->yakumono_id];

        if ((line_data->line_count != 0) && (yakumono_dobj->user_data.s < nMPYakumonoStatusOff))
        {
            if ((yakumono_dobj->anim_joint.event32 != NULL) || (yakumono_dobj->user_data.s != nMPYakumonoStatusNone))
            {
                // This runs when the corner of the character's map collision diamond collides with a wall?
                vedge_y = yakumono_dobj->translate.vec.f.x;
                vedge_x = yakumono_dobj->translate.vec.f.y;

                vpdist_x = position->x - vedge_y;
                vpdist_y = position->y - vedge_x;

                vtdist_x = translate->x - vedge_y;
                vtdist_y = translate->y - vedge_x;
            }
            else
            {
                vedge_y = vedge_x = 0.0F;

                vpdist_x = position->x;
                vpdist_y = position->y;

                vtdist_x = translate->x;
                vtdist_y = translate->y;
            }
            if (vpdist_x < vtdist_x)
            {
                vdist1 = vpdist_x - 0.001F;
                vdist2 = vtdist_x + 0.001F;
            }
            else
            {
                vdist1 = vtdist_x - 0.001F;
                vdist2 = vpdist_x + 0.001F;
            }
            for (line_id = line_data->group_id; line_id < (line_data->group_id + line_data->line_count); line_id++)
            {
                if (gMPCollisionVertexInfo->vertex_info[line_id].coll_pos_prev > vdist2)
                {
                    continue;
                }
                else if (gMPCollisionVertexInfo->vertex_info[line_id].coll_pos_next < vdist1)
                {
                    goto l_break; // Bruhhhhhhhhhhhhhhh
                }
                else
                {
                    vertex_id = gMPCollisionVertexLinks[line_id].vertex1;

                    vpos_x = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id]].pos.x;
                    vpos_y = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id]].pos.y;

                    for (vertex_id = gMPCollisionVertexLinks[line_id].vertex1; vertex_id < (gMPCollisionVertexLinks[line_id].vertex1 + gMPCollisionVertexLinks[line_id].vertex2) - 1; vertex_id++)
                    {
                        temp_s0 = vpos_x;
                        temp_s1 = vpos_y;

                        vpos_x = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id + 1]].pos.x;
                        vpos_y = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id + 1]].pos.y;

                        if (temp_s0 == vpos_x)
                        {
                            if (vtdist_x < vpdist_x)
                            {
                                if (mpCollisionCheckLRSurfaceFlat(temp_s0, temp_s1, vpos_y, vpdist_x, vpdist_y, vtdist_x, vtdist_y, &scrollv, &spAC) == TRUE)
                                {
                                    if (line_project_pos <= ((scrollv < vpdist_x) ? -(scrollv - vpdist_x) : (scrollv - vpdist_x))) continue;

                                    if (ga_last != NULL)
                                    {
                                        ga_last->x = scrollv + vedge_y;
                                        ga_last->y = spAC + vedge_x;
                                        ga_last->z = 0.0F;
                                    }
                                    if (stand_line_id != NULL)
                                    {
                                        *stand_line_id = line_id;
                                    }
                                    if (angle != NULL)
                                    {
                                        angle->y = angle->z = 0.0F;
                                        angle->x = 1.0F;
                                    }
                                    if (stand_coll_flags != NULL)
                                    {
                                        *stand_coll_flags = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id]].vertex_flags;
                                    }
                                    line_project_pos = (scrollv < vpdist_x) ? -(scrollv - vpdist_x) : (scrollv - vpdist_x);
                                }
                            }
                        }
                        else if (mpCollisionCheckRWallSurfaceTilt(temp_s0, temp_s1, vpos_x, vpos_y, vpdist_x, vpdist_y, vtdist_x, vtdist_y, &scrollv, &spAC) == TRUE)
                        {
                            if (line_project_pos <= ((scrollv < vpdist_x) ? -(scrollv - vpdist_x) : (scrollv - vpdist_x))) continue;

                            if (ga_last != NULL)
                            {
                                ga_last->x = scrollv + vedge_y;
                                ga_last->y = spAC + vedge_x;
                                ga_last->z = 0.0F;
                            }
                            if (stand_line_id != NULL)
                            {
                                *stand_line_id = line_id;
                            }
                            if (angle != NULL)
                            {
                                mpCollisionGetLRAngle(angle, temp_s0, temp_s1, vpos_x, vpos_y, +1);
                            }
                            if (stand_coll_flags != NULL)
                            {
                                *stand_coll_flags = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id]].vertex_flags;
                            }
                            line_project_pos = (scrollv < vpdist_x) ? -(scrollv - vpdist_x) : (scrollv - vpdist_x);
                        }
                    }
                }
            }
        l_break: // BRUHHHHHHHHHHHHHHHHH
            continue;
        }
    }
    if (line_project_pos == F32_MAX)
    {
        return FALSE;
    }
    else return TRUE;
}

// 0x800F71A0
void mpCollisionGetLRAngle(Vec3f *angle, s32 v1x, s32 v1y, s32 v2x, s32 v2y, s32 lr)
{
    f32 py, sqrt_py;
    f32 dist_x;

    angle->z = 0.0F;

    dist_x = (v2x - v1x);

    if (dist_x == 0.0F)
    {
        angle->x = lr;
        angle->y = 0.0F;

        return;
    }
    py = -(dist_x / (v2y - v1y));

    sqrt_py = 1.0F / sqrtf(SQUARE(py) + 1.0F);

    if (lr < 0)
    {
        // -sqrt_py
        angle->x = -sqrt_py;
        angle->y = (-py * sqrt_py);
    }
    else
    {
        angle->x = sqrt_py;
        angle->y = (py * sqrt_py);
    }
    syVectorNorm3D(angle);
}

// 0x800F7270
sb32 mpCollisionCheckRWallSurfaceTilt(s32 v1x, s32 v1y, s32 v2x, s32 v2y, f32 d1x, f32 d1y, f32 d2x, f32 d2y, f32 *dfx, f32 *dfy)
{
    s32 vfarx;
    s32 vnearx;
    f32 ddist_x;
    f32 ddist_y;
    s32 vfary;
    s32 vneary;
    s32 vdist_x;
    s32 vdist_y;
    f32 temp_f12_2;
    f32 temp_f14;
    f32 vddiv;
    f32 vddist_x;
    f32 vddist_y;
    f32 vddistdiv;
    f32 vdscale;
    f32 scale;
    f32 var_f18;

    vdist_x = v2x - v1x;
    ddist_x = d1x - d2x;

    if (vdist_x < 0)
    {
        vnearx = v2x;
        vfarx = v1x;
    }
    else
    {
        vfarx = v2x;
        vnearx = v1x;
    }
    if (ddist_x > 0.0F)
    {
        if (((vfarx + 0.001F) < d2x) || (d1x < (vnearx - 0.001F)))
        {
            return FALSE;
        }
    }
    else if (((vfarx + 0.001F) < d1x) || (d2x < (vnearx - 0.001F)))
    {
        return FALSE;
    }
    ddist_y = d1y - d2y;
    vdist_y = v2y - v1y;

    if (vdist_y < 0)
    {
        vneary = v2y;
        vfary = v1y;
    }
    else
    {
        vfary = v2y;
        vneary = v1y;
    }
    if (ddist_y > 0.0F)
    {
        if ((vfary < d2y) || (d1y < vneary))
        {
            return FALSE;
        }
    }
    else if ((vfary < d1y) || (d2y < vneary))
    {
        return FALSE;
    }
    if ((d2x - (v1x + (((d2y - v1y) / (v2y - v1y)) * vdist_x))) <= (-0.001F))
    {
        temp_f12_2 = v1x + (((d1y - v1y) / vdist_y) * vdist_x);

        if ((d1x - temp_f12_2) < 0.001F)
        {
            if ((((-0.001F) < (d1x - temp_f12_2)) && (d1y <= vfary)) && (vneary <= d1y))
            {
                *dfx = temp_f12_2;
                *dfy = d1y;

                return TRUE;
            }
        }
        else
        {
            vddist_x = v1x - d1x;
            vddist_y = v1y - d1y;

            var_f18 = (ddist_y * vddist_x) - (ddist_x * vddist_y);
            scale = (vdist_y * ddist_x) - (vdist_x * ddist_y);

            vddiv = var_f18 / scale;

            if (vddiv < 0.0F)
            {
                if (vddiv < -0.001F)
                {
                    return FALSE;
                }
                var_f18 = 0.0F;
            }
            else if (vddiv > 1.0F)
            {
                if (vddiv > 1.001F)
                {
                    return FALSE;
                }
                var_f18 = scale;
            }
            vddistdiv = ((vdist_x * vddist_y) - (vdist_y * vddist_x)) / scale;

            if ((vddistdiv < -0.001F) || (vddistdiv > 1.001F))
            {
                return FALSE;
            }
            vdscale = 1.0F / scale;

            *dfx = v1x + ((var_f18 * vdist_x) * vdscale);
            *dfy = v1y + ((var_f18 * vdist_y) * vdscale);

            return TRUE;
        }
    }
    return FALSE;
}

// 0x800F769C
sb32 mpCollisionCheckRWallLineCollisionDiff(Vec3f *position, Vec3f *translate, Vec3f *ga_last, s32 *stand_line_id, u32 *stand_coll_flags, Vec3f *angle)
{
    MPLineInfo *line_info;
    MPLineData *line_data;
    DObj *yakumono_dobj;
    s32 i;
    f32 vdist1;
    f32 vdist2;
    f32 vpdist_x, vpdist_y;
    f32 vtdist_x, vtdist_y;
    f32 line_project_pos;
    f32 vedge_x;
    f32 dynamic_x;
    f32 vedge_y;
    s32 line_id, vertex_id;
    s32 temp_s0;
    s32 temp_s1;
    s32 vpos_x;
    s32 vpos_y;
    f32 scrollv;
    f32 spAC;

    line_project_pos = F32_MAX;
    line_info = gMPCollisionGeometry->line_info;

    for (i = 0; i < gMPCollisionGeometry->yakumono_count; i++, line_info++)
    {
        line_data = &line_info->line_data[nMPLineKindRWall];
        yakumono_dobj = gMPCollisionYakumonoDObjs->dobjs[line_info->yakumono_id];

        if ((line_data->line_count != 0) && (yakumono_dobj->user_data.s < nMPYakumonoStatusOff))
        {
            if ((yakumono_dobj->anim_joint.event32 != NULL) || (yakumono_dobj->user_data.s != nMPYakumonoStatusNone))
            {
                // This runs when the corner of the character's map collision diamond collides with a wall?
                vedge_y = yakumono_dobj->translate.vec.f.x;
                vedge_x = yakumono_dobj->translate.vec.f.y;

                dynamic_x = gMPCollisionSpeeds[line_info->yakumono_id].x;

                vpdist_x = (position->x - vedge_y) + dynamic_x;
                vpdist_y = (position->y - vedge_x) + gMPCollisionSpeeds[line_info->yakumono_id].y;

                vtdist_x = translate->x - vedge_y;
                vtdist_y = translate->y - vedge_x;
            }
            else
            {
                dynamic_x = 0.0F;
                vedge_y = vedge_x = 0.0F;

                vpdist_x = position->x;
                vpdist_y = position->y;

                vtdist_x = translate->x;
                vtdist_y = translate->y;
            }
            if (vpdist_x < vtdist_x)
            {
                vdist1 = vpdist_x - 0.001F;
                vdist2 = vtdist_x + 0.001F;
            }
            else
            {
                vdist1 = vtdist_x - 0.001F;
                vdist2 = vpdist_x + 0.001F;
            }
            for (line_id = line_data->group_id; line_id < (line_data->group_id + line_data->line_count); line_id++)
            {
                if (gMPCollisionVertexInfo->vertex_info[line_id].coll_pos_prev > vdist2)
                {
                    continue;
                }
                else if (gMPCollisionVertexInfo->vertex_info[line_id].coll_pos_next < vdist1)
                {
                    goto l_break; // Bruhhhhhhhhhhhhhhh
                }
                else
                {
                    vertex_id = gMPCollisionVertexLinks[line_id].vertex1;

                    vpos_x = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id]].pos.x;
                    vpos_y = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id]].pos.y;

                    for (vertex_id = gMPCollisionVertexLinks[line_id].vertex1; vertex_id < (gMPCollisionVertexLinks[line_id].vertex1 + gMPCollisionVertexLinks[line_id].vertex2) - 1; vertex_id++)
                    {
                        temp_s0 = vpos_x;
                        temp_s1 = vpos_y;

                        vpos_x = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id + 1]].pos.x;
                        vpos_y = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id + 1]].pos.y;

                        if (temp_s0 == vpos_x)
                        {
                            if (vtdist_x < vpdist_x)
                            {
                                if (mpCollisionCheckLRSurfaceFlat(temp_s0, temp_s1, vpos_y, vpdist_x, vpdist_y, vtdist_x, vtdist_y, &scrollv, &spAC) == TRUE)
                                {
                                    if (line_project_pos <= ((scrollv < (vpdist_x - dynamic_x)) ? -(scrollv - (vpdist_x - dynamic_x)) : (scrollv - (vpdist_x - dynamic_x)))) continue;

                                    if (ga_last != NULL)
                                    {
                                        ga_last->x = scrollv + vedge_y;
                                        ga_last->y = spAC + vedge_x;
                                        ga_last->z = 0.0F;
                                    }
                                    if (stand_line_id != NULL)
                                    {
                                        *stand_line_id = line_id;
                                    }
                                    if (angle != NULL)
                                    {
                                        angle->y = angle->z = 0.0F;
                                        angle->x = 1.0F;
                                    }
                                    if (stand_coll_flags != NULL)
                                    {
                                        *stand_coll_flags = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id]].vertex_flags;
                                    }
                                    line_project_pos = (scrollv < (vpdist_x - dynamic_x)) ? -(scrollv - (vpdist_x - dynamic_x)) : (scrollv - (vpdist_x - dynamic_x));
                                }
                            }
                        }
                        else if (mpCollisionCheckRWallSurfaceTilt(temp_s0, temp_s1, vpos_x, vpos_y, vpdist_x, vpdist_y, vtdist_x, vtdist_y, &scrollv, &spAC) == TRUE)
                        {
                            if (line_project_pos <= ((scrollv < (vpdist_x - dynamic_x)) ? -(scrollv - (vpdist_x - dynamic_x)) : (scrollv - (vpdist_x - dynamic_x)))) continue;

                            if (ga_last != NULL)
                            {
                                ga_last->x = scrollv + vedge_y;
                                ga_last->y = spAC + vedge_x;
                                ga_last->z = 0.0F;
                            }
                            if (stand_line_id != NULL)
                            {
                                *stand_line_id = line_id;
                            }
                            if (angle != NULL)
                            {
                                mpCollisionGetLRAngle(angle, temp_s0, temp_s1, vpos_x, vpos_y, +1);
                            }
                            if (stand_coll_flags != NULL)
                            {
                                *stand_coll_flags = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id]].vertex_flags;
                            }
                            line_project_pos = (scrollv < (vpdist_x - dynamic_x)) ? -(scrollv - (vpdist_x - dynamic_x)) : (scrollv - (vpdist_x - dynamic_x));
                        }
                    }
                }
            }
        l_break: // BRUHHHHHHHHHHHHHHHHH
            continue;
        }
    }
    if (line_project_pos == F32_MAX)
    {
        return FALSE;
    }
    else return TRUE;
}

// 0x800F7D24
sb32 mpCollisionCheckLRSurfaceFlat(s32 v1x, s32 v1y, s32 vpos_x, f32 vpdist_x, f32 vpdist_y, f32 vtdist_x, f32 vtdist_y, f32 *arg7, f32 *arg8)
{
    s32 unused[2];
    f32 temp_f18;
    s32 vnear;
    s32 vfar;
    f32 ddist_y;
    s32 vdist_x = vpos_x - v1y;
    f32 ddist_x = vpdist_x - vtdist_x;
    f32 vddist_x = v1x - vpdist_x;

    if (ddist_x > 0.0F)
    {
        if (((v1x - 0.001F) > vpdist_x) || (vtdist_x >= v1x))
        {
            return FALSE;
        }
    }
    else if (((v1x + 0.001F) < vpdist_x) || (vtdist_x <= v1x))
    {
        return FALSE;
    }
    ddist_y = vpdist_y - vtdist_y;

    if (vdist_x < 0)
    {
        vnear = vpos_x;
        vfar = v1y;
    }
    else
    {
        vfar = vpos_x;
        vnear = v1y;
    }

    if (ddist_y > 0.0F)
    {
        if ((vfar < vtdist_y) || (vpdist_y < vnear))
        {
            return FALSE;
        }
    }
    else if ((vfar < vpdist_y) || (vtdist_y < vnear))
    {
        return FALSE;
    }
    temp_f18 = ((vddist_x / ddist_x) * ddist_y) + vpdist_y;

    if ((temp_f18 < vnear) || (temp_f18 > vfar))
    {
        return FALSE;
    }

    *arg7 = v1x;
    *arg8 = temp_f18;

    return TRUE;
}

// 0x800F7F00
sb32 mpCollisionCheckLWallLineCollisionSame(Vec3f *position, Vec3f *translate, Vec3f *ga_last, s32 *stand_line_id, u32 *stand_coll_flags, Vec3f *angle)
{
    MPLineInfo *line_info;
    MPLineData *line_data;
    DObj *yakumono_dobj;
    s32 i;
    f32 vdist1;
    f32 vdist2;
    f32 vpdist_x, vpdist_y;
    f32 vtdist_x, vtdist_y;
    f32 line_project_pos;
    f32 vedge_x;
    f32 vedge_y;
    s32 line_id, vertex_id;
    s32 temp_s0;
    s32 temp_s1;
    s32 vpos_x;
    s32 vpos_y;
    f32 scrollv;
    f32 spAC;

    line_project_pos = F32_MAX;
    line_info = gMPCollisionGeometry->line_info;

    for (i = 0; i < gMPCollisionGeometry->yakumono_count; i++, line_info++)
    {
        line_data = &line_info->line_data[nMPLineKindLWall];
        yakumono_dobj = gMPCollisionYakumonoDObjs->dobjs[line_info->yakumono_id];

        if ((line_data->line_count != 0) && (yakumono_dobj->user_data.s < nMPYakumonoStatusOff))
        {
            if ((yakumono_dobj->anim_joint.event32 != NULL) || (yakumono_dobj->user_data.s != nMPYakumonoStatusNone))
            {
                // This runs when the corner of the character's map collision diamond collides with a wall?
                vedge_y = yakumono_dobj->translate.vec.f.x;
                vedge_x = yakumono_dobj->translate.vec.f.y;

                vpdist_x = position->x - vedge_y;
                vpdist_y = position->y - vedge_x;

                vtdist_x = translate->x - vedge_y;
                vtdist_y = translate->y - vedge_x;
            }
            else
            {
                vedge_y = vedge_x = 0.0F;

                vpdist_x = position->x;
                vpdist_y = position->y;

                vtdist_x = translate->x;
                vtdist_y = translate->y;
            }
            if (vpdist_x < vtdist_x)
            {
                vdist1 = vpdist_x - 0.001F;
                vdist2 = vtdist_x + 0.001F;
            }
            else
            {
                vdist1 = vtdist_x - 0.001F;
                vdist2 = vpdist_x + 0.001F;
            }
            for (line_id = line_data->group_id; line_id < (line_data->group_id + line_data->line_count); line_id++)
            {
                if (gMPCollisionVertexInfo->vertex_info[line_id].coll_pos_next < vdist1)
                {
                    continue;
                }
                else if (gMPCollisionVertexInfo->vertex_info[line_id].coll_pos_prev > vdist2)
                {
                    goto l_break; // Bruhhhhhhhhhhhhhhh
                }
                else
                {
                    vertex_id = gMPCollisionVertexLinks[line_id].vertex1;

                    vpos_x = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id]].pos.x;
                    vpos_y = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id]].pos.y;

                    for (vertex_id = gMPCollisionVertexLinks[line_id].vertex1; vertex_id < (gMPCollisionVertexLinks[line_id].vertex1 + gMPCollisionVertexLinks[line_id].vertex2) - 1; vertex_id++)
                    {
                        temp_s0 = vpos_x;
                        temp_s1 = vpos_y;

                        vpos_x = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id + 1]].pos.x;
                        vpos_y = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id + 1]].pos.y;

                        if (temp_s0 == vpos_x)
                        {
                            if (vtdist_x > vpdist_x)
                            {
                                if (mpCollisionCheckLRSurfaceFlat(temp_s0, temp_s1, vpos_y, vpdist_x, vpdist_y, vtdist_x, vtdist_y, &scrollv, &spAC) == TRUE)
                                {
                                    if (line_project_pos <= ((scrollv < vpdist_x) ? -(scrollv - vpdist_x) : (scrollv - vpdist_x))) continue;

                                    if (ga_last != NULL)
                                    {
                                        ga_last->x = scrollv + vedge_y;
                                        ga_last->y = spAC + vedge_x;
                                        ga_last->z = 0.0F;
                                    }
                                    if (stand_line_id != NULL)
                                    {
                                        *stand_line_id = line_id;
                                    }
                                    if (angle != NULL)
                                    {
                                        angle->y = angle->z = 0.0F;
                                        angle->x = -1.0F;
                                    }
                                    if (stand_coll_flags != NULL)
                                    {
                                        *stand_coll_flags = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id]].vertex_flags;
                                    }
                                    line_project_pos = (scrollv < vpdist_x) ? -(scrollv - vpdist_x) : (scrollv - vpdist_x);
                                }
                            }
                        }
                        else if (mpCollisionCheckLWallSurfaceTilt(temp_s0, temp_s1, vpos_x, vpos_y, vpdist_x, vpdist_y, vtdist_x, vtdist_y, &scrollv, &spAC) == TRUE)
                        {
                            if (line_project_pos <= ((scrollv < vpdist_x) ? -(scrollv - vpdist_x) : (scrollv - vpdist_x))) continue;

                            if (ga_last != NULL)
                            {
                                ga_last->x = scrollv + vedge_y;
                                ga_last->y = spAC + vedge_x;
                                ga_last->z = 0.0F;
                            }
                            if (stand_line_id != NULL)
                            {
                                *stand_line_id = line_id;
                            }
                            if (angle != NULL)
                            {
                                mpCollisionGetLRAngle(angle, temp_s0, temp_s1, vpos_x, vpos_y, -1);
                            }
                            if (stand_coll_flags != NULL)
                            {
                                *stand_coll_flags = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id]].vertex_flags;
                            }
                            line_project_pos = (scrollv < vpdist_x) ? -(scrollv - vpdist_x) : (scrollv - vpdist_x);
                        }
                    }
                }
            }
        l_break: // BRUHHHHHHHHHHHHHHHHH
            continue;
        }
    }
    if (line_project_pos == F32_MAX)
    {
        return FALSE;
    }
    else return TRUE;
}

// 0x800F8548
sb32 mpCollisionCheckLWallSurfaceTilt(s32 v1x, s32 v1y, s32 v2x, s32 v2y, f32 d1x, f32 d1y, f32 d2x, f32 d2y, f32 *dfx, f32 *dfy)
{
    s32 vfarx;
    s32 vnearx;
    f32 ddist_x;
    f32 ddist_y;
    s32 vfary;
    s32 vneary;
    s32 vdist_x;
    s32 vdist_y;
    f32 temp_f12_2;
    f32 temp_f14;
    f32 vddiv;
    f32 vddist_x;
    f32 vddist_y;
    f32 vddistdiv;
    f32 vdscale;
    f32 scale;
    f32 var_f18;

    vdist_x = v2x - v1x;
    ddist_x = d1x - d2x;

    if (vdist_x < 0)
    {
        vnearx = v2x;
        vfarx = v1x;
    }
    else
    {
        vfarx = v2x;
        vnearx = v1x;
    }
    if (ddist_x > 0.0F)
    {
        if (((vfarx + 0.001F) < d2x) || (d1x < (vnearx - 0.001F)))
        {
            return FALSE;
        }
    }
    else if (((vfarx + 0.001F) < d1x) || (d2x < (vnearx - 0.001F)))
    {
        return FALSE;
    }
    ddist_y = d1y - d2y;
    vdist_y = v2y - v1y;

    if (vdist_y < 0)
    {
        vneary = v2y;
        vfary = v1y;
    }
    else
    {
        vfary = v2y;
        vneary = v1y;
    }
    if (ddist_y > 0.0F)
    {
        if ((vfary < d2y) || (d1y < vneary))
        {
            return FALSE;
        }
    }
    else if ((vfary < d1y) || (d2y < vneary))
    {
        return FALSE;
    }
    if ((d2x - (v1x + (((d2y - v1y) / (v2y - v1y)) * vdist_x))) >= 0.001F)
    {
        temp_f12_2 = v1x + (((d1y - v1y) / vdist_y) * vdist_x);

        if ((d1x - temp_f12_2) > -0.001F)
        {
            if (((d1x - temp_f12_2) < 0.001F) && (d1y <= vfary) && (vneary <= d1y))
            {
                *dfx = temp_f12_2;
                *dfy = d1y;

                return TRUE;
            }
        }
        else
        {
            vddist_x = v1x - d1x;
            vddist_y = v1y - d1y;

            var_f18 = (ddist_y * vddist_x) - (ddist_x * vddist_y);
            scale = (vdist_y * ddist_x) - (vdist_x * ddist_y);

            vddiv = var_f18 / scale;

            if (vddiv < 0.0F)
            {
                if (vddiv < -0.001F)
                {
                    return FALSE;
                }
                var_f18 = 0.0F;
            }
            else if (vddiv > 1.0F)
            {
                if (vddiv > 1.001F)
                {
                    return FALSE;
                }
                var_f18 = scale;
            }
            vddistdiv = ((vdist_x * vddist_y) - (vdist_y * vddist_x)) / scale;

            if ((vddistdiv < -0.001F) || (vddistdiv > 1.001F))
            {
                return FALSE;
            }
            vdscale = 1.0F / scale;

            *dfx = v1x + ((var_f18 * vdist_x) * vdscale);
            *dfy = v1y + ((var_f18 * vdist_y) * vdscale);

            return TRUE;
        }
    }
    return FALSE;
}

// 0x800F8974
sb32 mpCollisionCheckLWallLineCollisionDiff(Vec3f *position, Vec3f *translate, Vec3f *ga_last, s32 *stand_line_id, u32 *stand_coll_flags, Vec3f *angle)
{
    MPLineInfo *line_info;
    MPLineData *line_data;
    DObj *yakumono_dobj;
    s32 i;
    f32 vdist1;
    f32 vdist2;
    f32 vpdist_x, vpdist_y;
    f32 vtdist_x, vtdist_y;
    f32 line_project_pos;
    f32 vedge_x;
    f32 dynamic_x;
    f32 vedge_y;
    s32 line_id, vertex_id;
    s32 temp_s0;
    s32 temp_s1;
    s32 vpos_x;
    s32 vpos_y;
    f32 scrollv;
    f32 spAC;

    line_project_pos = F32_MAX;
    line_info = gMPCollisionGeometry->line_info;

    for (i = 0; i < gMPCollisionGeometry->yakumono_count; i++, line_info++)
    {
        line_data = &line_info->line_data[nMPLineKindLWall];
        yakumono_dobj = gMPCollisionYakumonoDObjs->dobjs[line_info->yakumono_id];

        if ((line_data->line_count != 0) && (yakumono_dobj->user_data.s < nMPYakumonoStatusOff))
        {
            if ((yakumono_dobj->anim_joint.event32 != NULL) || (yakumono_dobj->user_data.s != nMPYakumonoStatusNone))
            {
                // This runs when the corner of the character's map collision diamond collides with a wall?
                vedge_y = yakumono_dobj->translate.vec.f.x;
                vedge_x = yakumono_dobj->translate.vec.f.y;

                dynamic_x = gMPCollisionSpeeds[line_info->yakumono_id].x;

                vpdist_x = (position->x - vedge_y) + dynamic_x;
                vpdist_y = (position->y - vedge_x) + gMPCollisionSpeeds[line_info->yakumono_id].y;

                vtdist_x = translate->x - vedge_y;
                vtdist_y = translate->y - vedge_x;
            }
            else
            {
                dynamic_x = 0.0F;
                vedge_y = vedge_x = 0.0F;

                vpdist_x = position->x;
                vpdist_y = position->y;

                vtdist_x = translate->x;
                vtdist_y = translate->y;
            }
            if (vpdist_x < vtdist_x)
            {
                vdist1 = vpdist_x - 0.001F;
                vdist2 = vtdist_x + 0.001F;
            }
            else
            {
                vdist1 = vtdist_x - 0.001F;
                vdist2 = vpdist_x + 0.001F;
            }
            for (line_id = line_data->group_id; line_id < (line_data->group_id + line_data->line_count); line_id++)
            {
                if (gMPCollisionVertexInfo->vertex_info[line_id].coll_pos_next < vdist1)
                {
                    continue;
                }
                else if (gMPCollisionVertexInfo->vertex_info[line_id].coll_pos_prev > vdist2)
                {
                    goto l_break; // Bruhhhhhhhhhhhhhhh
                }
                else
                {
                    vertex_id = gMPCollisionVertexLinks[line_id].vertex1;

                    vpos_x = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id]].pos.x;
                    vpos_y = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id]].pos.y;

                    for (vertex_id = gMPCollisionVertexLinks[line_id].vertex1; vertex_id < (gMPCollisionVertexLinks[line_id].vertex1 + gMPCollisionVertexLinks[line_id].vertex2) - 1; vertex_id++)
                    {
                        temp_s0 = vpos_x;
                        temp_s1 = vpos_y;

                        vpos_x = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id + 1]].pos.x;
                        vpos_y = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id + 1]].pos.y;

                        if (temp_s0 == vpos_x)
                        {
                            if (vtdist_x > vpdist_x)
                            {
                                if (mpCollisionCheckLRSurfaceFlat(temp_s0, temp_s1, vpos_y, vpdist_x, vpdist_y, vtdist_x, vtdist_y, &scrollv, &spAC) == TRUE)
                                {
                                    if (line_project_pos <= ((scrollv < (vpdist_x - dynamic_x)) ? -(scrollv - (vpdist_x - dynamic_x)) : (scrollv - (vpdist_x - dynamic_x)))) continue;

                                    if (ga_last != NULL)
                                    {
                                        ga_last->x = scrollv + vedge_y;
                                        ga_last->y = spAC + vedge_x;
                                        ga_last->z = 0.0F;
                                    }
                                    if (stand_line_id != NULL)
                                    {
                                        *stand_line_id = line_id;
                                    }
                                    if (angle != NULL)
                                    {
                                        angle->y = angle->z = 0.0F;
                                        angle->x = -1.0F;
                                    }
                                    if (stand_coll_flags != NULL)
                                    {
                                        *stand_coll_flags = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id]].vertex_flags;
                                    }
                                    line_project_pos = (scrollv < (vpdist_x - dynamic_x)) ? -(scrollv - (vpdist_x - dynamic_x)) : (scrollv - (vpdist_x - dynamic_x));
                                }
                            }
                        }
                        else if (mpCollisionCheckLWallSurfaceTilt(temp_s0, temp_s1, vpos_x, vpos_y, vpdist_x, vpdist_y, vtdist_x, vtdist_y, &scrollv, &spAC) == TRUE)
                        {
                            if (line_project_pos <= ((scrollv < (vpdist_x - dynamic_x)) ? -(scrollv - (vpdist_x - dynamic_x)) : (scrollv - (vpdist_x - dynamic_x)))) continue;

                            if (ga_last != NULL)
                            {
                                ga_last->x = scrollv + vedge_y;
                                ga_last->y = spAC + vedge_x;
                                ga_last->z = 0.0F;
                            }
                            if (stand_line_id != NULL)
                            {
                                *stand_line_id = line_id;
                            }
                            if (angle != NULL)
                            {
                                mpCollisionGetLRAngle(angle, temp_s0, temp_s1, vpos_x, vpos_y, -1);
                            }
                            if (stand_coll_flags != NULL)
                            {
                                *stand_coll_flags = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id]].vertex_flags;
                            }
                            line_project_pos = (scrollv < (vpdist_x - dynamic_x)) ? -(scrollv - (vpdist_x - dynamic_x)) : (scrollv - (vpdist_x - dynamic_x));
                        }
                    }
                }
            }
        l_break: // BRUHHHHHHHHHHHHHHHHH
            continue;
        }
    }
    if (line_project_pos == F32_MAX)
    {
        return FALSE;
    }
    else return TRUE;
}

// 0x800F8FFC
sb32 func_ovl2_800F8FFC(Vec3f *position)
{
    MPLineInfo *line_info;
    MPLineData *line_data;
    s32 i, j;
    DObj *yakumono_dobj;
    f32 vpdist_x, vpdist_y;
    s32 line_id;

    line_info = gMPCollisionGeometry->line_info;

    for (i = 0; i < gMPCollisionGeometry->yakumono_count; i++, line_info++)
    {
        line_data = &line_info->line_data[nMPLineKindFloor];
        yakumono_dobj = gMPCollisionYakumonoDObjs->dobjs[line_info->yakumono_id];

        if ((line_data->line_count != 0) && (yakumono_dobj->user_data.s < nMPYakumonoStatusOff))
        {
            if ((yakumono_dobj->anim_joint.event32 != NULL) || (yakumono_dobj->user_data.s != nMPYakumonoStatusNone))
            {
                // This runs when the corner of the character's map collision diamond collides with a wall?
                vpdist_x = (position->x - yakumono_dobj->translate.vec.f.x);
                vpdist_y = (position->y - yakumono_dobj->translate.vec.f.y) + 0.001F;
            }
            else
            {
                vpdist_x = position->x;
                vpdist_y = position->y + 0.001F;
            }
            for (line_id = line_data->group_id; line_id < (line_data->group_id + line_data->line_count); line_id++)
            {
                if (gMPCollisionVertexInfo->vertex_info[line_id].coll_pos_prev <= vpdist_y)
                {
                    s32 vertex_id1 = gMPCollisionVertexLinks[line_id].vertex1;
                    s32 vertex_id2 = gMPCollisionVertexLinks[line_id].vertex2 - 1;

                    s32 x1 = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id1]].pos.x;
                    s32 x2 = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id1 + vertex_id2]].pos.x;

                    if ((x1 <= vpdist_x && x2 >= vpdist_x) || (x2 <= vpdist_x && x1 >= vpdist_x))
                    {
                        if (vertex_id2 != 1)
                        {
                            for (j = 0; j < vertex_id2; j++, vertex_id1++)
                            {
                                x1 = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id1]].pos.x;
                                x2 = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id1 + 1]].pos.x;

                                if (mpCollisionGetLineDistanceFC(vpdist_x, x1, gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id1]].pos.y, x2, gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id1 + 1]].pos.y) < vpdist_y)
                                {
                                    return TRUE;
                                }
                            }
                        }
                        else if (mpCollisionGetLineDistanceFC(vpdist_x, x1, gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id1]].pos.y, x2, gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id1 + 1]].pos.y) < vpdist_y)
                        {
                            return TRUE;
                        }
                    }
                }
            }
        }
    }
    return FALSE;
}

// 0x800F9348
sb32 mpCollisionCheckProjectFloor(Vec3f *position, s32 *project_line_id, f32 *ga_dist, u32 *stand_coll_flags, Vec3f *angle)
{
    MPLineInfo *line_info;
    MPLineData *line_data;
    DObj *yakumono_dobj;
    s32 i, j;
    f32 vpdist_x, vpdist_y;
    s32 line_id;
    f32 line_project_pos;
    f32 fpos;
    f32 gdist;

    line_project_pos = F32_MAX;
    line_info = gMPCollisionGeometry->line_info;

    for (i = 0; i < gMPCollisionGeometry->yakumono_count; i++, line_info++)
    {
        line_data = &line_info->line_data[nMPLineKindFloor];
        yakumono_dobj = gMPCollisionYakumonoDObjs->dobjs[line_info->yakumono_id];

        if ((line_data->line_count != 0) && (yakumono_dobj->user_data.s < nMPYakumonoStatusOff))
        {
            if ((yakumono_dobj->anim_joint.event32 != NULL) || (yakumono_dobj->user_data.s != nMPYakumonoStatusNone))
            {
                // This runs when the corner of the character's map collision diamond collides with a wall?
                vpdist_x = (position->x - yakumono_dobj->translate.vec.f.x);
                vpdist_y = (position->y - yakumono_dobj->translate.vec.f.y);
            }
            else
            {
                vpdist_x = position->x;
                vpdist_y = position->y;
            }
            for (line_id = line_data->group_id; line_id < (line_data->group_id + line_data->line_count); line_id++)
            {
                if (gMPCollisionVertexInfo->vertex_info[line_id].coll_pos_prev <= vpdist_y)
                {
                    s32 vertex_id1 = gMPCollisionVertexLinks[line_id].vertex1;
                    s32 vertex_id2 = gMPCollisionVertexLinks[line_id].vertex2 - 1;

                    s32 x1 = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id1]].pos.x;
                    s32 x2 = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id1 + vertex_id2]].pos.x;
                    s32 y1;
                    s32 y2;

                    if ((x1 <= vpdist_x && x2 >= vpdist_x) || (x2 <= vpdist_x && x1 >= vpdist_x))
                    {
                        if (vertex_id2 != 1)
                        {
                            for (j = 0; j < vertex_id2; j++, vertex_id1++)
                            {
                                x1 = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id1]].pos.x;
                                x2 = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id1 + 1]].pos.x;

                                if ((x1 <= vpdist_x && x2 >= vpdist_x) || (x2 <= vpdist_x && x1 >= vpdist_x))
                                {
                                    break;
                                }
                            }
                        }
                        y1 = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id1]].pos.y;
                        y2 = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id1 + 1]].pos.y;

                        fpos = mpCollisionGetLineDistanceFC(vpdist_x, x1, y1, x2, y2);

                        gdist = fpos - vpdist_y;

                        if (fpos <= vpdist_y)
                        {
                            if (ABSF(gdist) < line_project_pos)
                            {
                                if (project_line_id != NULL)
                                {
                                    *project_line_id = line_id;
                                }
                                if (ga_dist != NULL)
                                {
                                    *ga_dist = gdist;
                                }
                                if (stand_coll_flags != NULL)
                                {
                                    *stand_coll_flags = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id1]].vertex_flags;
                                }
                                if (angle != NULL)
                                {
                                    mpCollisionGetFCAngle(angle, x1, y1, x2, y2, +1);
                                }
                                line_project_pos = ABSF(gdist);
                            }
                        }
                    }
                }
            }
        }
    }
    if (line_project_pos == F32_MAX)
    {
        return FALSE;
    }
    else return TRUE;
}

// Unused?
sb32 mpCollisionCheckProjectCeil(Vec3f *position, s32 *project_line_id, f32 *ga_dist, u32 *stand_coll_flags, Vec3f *angle)
{
    MPLineInfo *line_info;
    MPLineData *line_data;
    DObj *yakumono_dobj;
    s32 i, j;
    f32 vpdist_x, vpdist_y;
    s32 line_id;
    f32 line_project_pos;
    f32 fpos;
    f32 gdist;

    line_project_pos = F32_MAX;
    line_info = gMPCollisionGeometry->line_info;

    for (i = 0; i < gMPCollisionGeometry->yakumono_count; i++, line_info++)
    {
        line_data = &line_info->line_data[nMPLineKindCeil];
        yakumono_dobj = gMPCollisionYakumonoDObjs->dobjs[line_info->yakumono_id];

        if ((line_data->line_count != 0) && (yakumono_dobj->user_data.s < nMPYakumonoStatusOff))
        {
            if ((yakumono_dobj->anim_joint.event32 != NULL) || (yakumono_dobj->user_data.s != nMPYakumonoStatusNone))
            {
                // This runs when the corner of the character's map collision diamond collides with a wall?
                vpdist_x = (position->x - yakumono_dobj->translate.vec.f.x);
                vpdist_y = (position->y - yakumono_dobj->translate.vec.f.y);
            }
            else
            {
                vpdist_x = position->x;
                vpdist_y = position->y;
            }
            for (line_id = line_data->group_id; line_id < (line_data->group_id + line_data->line_count); line_id++)
            {
                if (gMPCollisionVertexInfo->vertex_info[line_id].coll_pos_next >= vpdist_y)
                {
                    s32 vertex_id1 = gMPCollisionVertexLinks[line_id].vertex1;
                    s32 vertex_id2 = gMPCollisionVertexLinks[line_id].vertex2 - 1;

                    s32 x1 = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id1]].pos.x;
                    s32 x2 = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id1 + vertex_id2]].pos.x;
                    s32 y1;
                    s32 y2;

                    if ((x1 <= vpdist_x && x2 >= vpdist_x) || (x2 <= vpdist_x && x1 >= vpdist_x))
                    {
                        if (vertex_id2 != 1)
                        {
                            for (j = 0; j < vertex_id2; j++, vertex_id1++)
                            {
                                x1 = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id1]].pos.x;
                                x2 = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id1 + 1]].pos.x;

                                if ((x1 <= vpdist_x && x2 >= vpdist_x) || (x2 <= vpdist_x && x1 >= vpdist_x))
                                {
                                    break;
                                }
                            }
                        }
                        y1 = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id1]].pos.y;
                        y2 = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id1 + 1]].pos.y;

                        fpos = mpCollisionGetLineDistanceFC(vpdist_x, x1, y1, x2, y2);

                        gdist = fpos - vpdist_y;

                        if (fpos >= vpdist_y)
                        {
                            if (ABSF(gdist) < line_project_pos)
                            {
                                if (project_line_id != NULL)
                                {
                                    *project_line_id = line_id;
                                }
                                if (ga_dist != NULL)
                                {
                                    *ga_dist = gdist;
                                }
                                if (stand_coll_flags != NULL)
                                {
                                    *stand_coll_flags = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id1]].vertex_flags;
                                }
                                if (angle != NULL)
                                {
                                    mpCollisionGetFCAngle(angle, x1, y1, x2, y2, -1);
                                }
                                line_project_pos = ABSF(gdist);
                            }
                        }
                    }
                }
            }
        }
    }
    if (line_project_pos == F32_MAX)
    {
        return FALSE;
    }
    else return TRUE;
}

// 0x800F9C30
sb32 mpCollisionCheckProjectRWall(Vec3f *position, s32 *project_line_id, f32 *ga_dist, u32 *stand_coll_flags, Vec3f *angle)
{
    MPLineInfo *line_info;
    MPLineData *line_data;
    DObj *yakumono_dobj;
    s32 i, j;
    f32 vpdist_x, vpdist_y;
    s32 line_id;
    f32 line_project_pos;
    f32 fpos;
    f32 gdist;

    line_project_pos = F32_MAX;
    line_info = gMPCollisionGeometry->line_info;

    for (i = 0; i < gMPCollisionGeometry->yakumono_count; i++, line_info++)
    {
        line_data = &line_info->line_data[nMPLineKindRWall];
        yakumono_dobj = gMPCollisionYakumonoDObjs->dobjs[line_info->yakumono_id];

        if ((line_data->line_count != 0) && (yakumono_dobj->user_data.s < nMPYakumonoStatusOff))
        {
            if ((yakumono_dobj->anim_joint.event32 != NULL) || (yakumono_dobj->user_data.s != nMPYakumonoStatusNone))
            {
                // This runs when the corner of the character's map collision diamond collides with a wall?
                vpdist_x = (position->x - yakumono_dobj->translate.vec.f.x);
                vpdist_y = (position->y - yakumono_dobj->translate.vec.f.y);
            }
            else
            {
                vpdist_x = position->x;
                vpdist_y = position->y;
            }
            for (line_id = line_data->group_id; line_id < (line_data->group_id + line_data->line_count); line_id++)
            {
                if (gMPCollisionVertexInfo->vertex_info[line_id].coll_pos_prev <= vpdist_x)
                {
                    s32 vertex_id1 = gMPCollisionVertexLinks[line_id].vertex1;
                    s32 vertex_id2 = gMPCollisionVertexLinks[line_id].vertex2 - 1;

                    s32 y1 = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id1]].pos.y;
                    s32 y2 = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id1 + vertex_id2]].pos.y;
                    s32 x1;
                    s32 x2;

                    if ((y1 <= vpdist_y && y2 >= vpdist_y) || (y2 <= vpdist_y && y1 >= vpdist_y))
                    {
                        if (vertex_id2 != 1)
                        {
                            for (j = 0; j < vertex_id2; j++, vertex_id1++)
                            {
                                y1 = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id1]].pos.y;
                                y2 = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id1 + 1]].pos.y;

                                if ((y1 <= vpdist_y && y2 >= vpdist_y) || (y2 <= vpdist_y && y1 >= vpdist_y))
                                {
                                    break;
                                }
                            }
                        }
                        x1 = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id1]].pos.x;
                        x2 = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id1 + 1]].pos.x;

                        fpos = mpCollisionGetLineDistanceLR(vpdist_y, x1, y1, x2, y2);

                        gdist = fpos - vpdist_x;

                        if (fpos <= vpdist_x)
                        {
                            if (ABSF(gdist) < line_project_pos)
                            {
                                if (project_line_id != NULL)
                                {
                                    *project_line_id = line_id;
                                }
                                if (ga_dist != NULL)
                                {
                                    *ga_dist = gdist;
                                }
                                if (stand_coll_flags != NULL)
                                {
                                    *stand_coll_flags = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id1]].vertex_flags;
                                }
                                if (angle != NULL)
                                {
                                    mpCollisionGetFCAngle(angle, x1, y1, x2, y2, +1);
                                }
                                line_project_pos = ABSF(gdist);
                            }
                        }
                    }
                }
            }
        }
    }
    if (line_project_pos == F32_MAX)
    {
        return FALSE;
    }
    else return TRUE;
}

// Unused?
sb32 mpCollisionCheckProjectLWall(Vec3f *position, s32 *project_line_id, f32 *ga_dist, u32 *stand_coll_flags, Vec3f *angle)
{
    MPLineInfo *line_info;
    MPLineData *line_data;
    DObj *yakumono_dobj;
    s32 i, j;
    f32 vpdist_x, vpdist_y;
    s32 line_id;
    f32 line_project_pos;
    f32 fpos;
    f32 gdist;

    line_project_pos = F32_MAX;
    line_info = gMPCollisionGeometry->line_info;

    for (i = 0; i < gMPCollisionGeometry->yakumono_count; i++, line_info++)
    {
        line_data = &line_info->line_data[nMPLineKindLWall];
        yakumono_dobj = gMPCollisionYakumonoDObjs->dobjs[line_info->yakumono_id];

        if ((line_data->line_count != 0) && (yakumono_dobj->user_data.s < nMPYakumonoStatusOff))
        {
            if ((yakumono_dobj->anim_joint.event32 != NULL) || (yakumono_dobj->user_data.s != nMPYakumonoStatusNone))
            {
                // This runs when the corner of the character's map collision diamond collides with a wall?
                vpdist_x = (position->x - yakumono_dobj->translate.vec.f.x);
                vpdist_y = (position->y - yakumono_dobj->translate.vec.f.y);
            }
            else
            {
                vpdist_x = position->x;
                vpdist_y = position->y;
            }
            for (line_id = line_data->group_id; line_id < (line_data->group_id + line_data->line_count); line_id++)
            {
                if (gMPCollisionVertexInfo->vertex_info[line_id].coll_pos_next >= vpdist_x)
                {
                    s32 vertex_id1 = gMPCollisionVertexLinks[line_id].vertex1;
                    s32 vertex_id2 = gMPCollisionVertexLinks[line_id].vertex2 - 1;

                    s32 y1 = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id1]].pos.y;
                    s32 y2 = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id1 + vertex_id2]].pos.y;
                    s32 x1;
                    s32 x2;

                    if ((y1 <= vpdist_y && y2 >= vpdist_y) || (y2 <= vpdist_y && y1 >= vpdist_y))
                    {
                        if (vertex_id2 != 1)
                        {
                            for (j = 0; j < vertex_id2; j++, vertex_id1++)
                            {
                                y1 = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id1]].pos.y;
                                y2 = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id1 + 1]].pos.y;

                                if ((y1 <= vpdist_y && y2 >= vpdist_y) || (y2 <= vpdist_y && y1 >= vpdist_y))
                                {
                                    break;
                                }
                            }
                        }
                        x1 = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id1]].pos.x;
                        x2 = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id1 + 1]].pos.x;

                        fpos = mpCollisionGetLineDistanceLR(vpdist_y, x1, y1, x2, y2);

                        gdist = fpos - vpdist_x;

                        if (fpos >= vpdist_x)
                        {
                            if (ABSF(gdist) < line_project_pos)
                            {
                                if (project_line_id != NULL)
                                {
                                    *project_line_id = line_id;
                                }
                                if (ga_dist != NULL)
                                {
                                    *ga_dist = gdist;
                                }
                                if (stand_coll_flags != NULL)
                                {
                                    *stand_coll_flags = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vertex_id1]].vertex_flags;
                                }
                                if (angle != NULL)
                                {
                                    mpCollisionGetFCAngle(angle, x1, y1, x2, y2, -1);
                                }
                                line_project_pos = ABSF(gdist);
                            }
                        }
                    }
                }
            }
        }
    }
    if (line_project_pos == F32_MAX)
    {
        return FALSE;
    }
    else return TRUE;
}

// 0x800FA518
s32 mpCollisionGetVertexCountLineID(s32 line_id)
{
    DObj *yakumono_dobj;

    if ((line_id == -1) || (line_id == -2))
    {
        while (TRUE)
        {
            syDebugPrintf("mpGetNbVertex() id = %d\n", line_id);
            scManagerRunPrintGObjStatus();
        }
    }
    yakumono_dobj = gMPCollisionYakumonoDObjs->dobjs[gMPCollisionVertexInfo->vertex_info[line_id].yakumono_id];

    if (yakumono_dobj->user_data.s >= nMPYakumonoStatusOff)
    {
        while (TRUE)
        {
            syDebugPrintf("mpGetNbVertex() no collision\n");
            scManagerRunPrintGObjStatus();
        }
    }
    return gMPCollisionVertexLinks[line_id].vertex2;
}

// 0x800F15E8
void mpCollisionGetVertexPositionID(s32 line_id, s32 vertex_id, Vec3f *pos)
{
    DObj *yakumono_dobj;

    if ((line_id == -1) || (line_id == -2))
    {
        while (TRUE)
        {
            syDebugPrintf("mpGetVertex() id = %d\n", line_id);
            scManagerRunPrintGObjStatus();
        }
    }
    yakumono_dobj = gMPCollisionYakumonoDObjs->dobjs[gMPCollisionVertexInfo->vertex_info[line_id].yakumono_id];

    if (yakumono_dobj->user_data.s >= nMPYakumonoStatusOff)
    {
        while (TRUE)
        {
            syDebugPrintf("mpGetVertex() no collision\n");
            scManagerRunPrintGObjStatus();
        }
    }
    pos->x = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[gMPCollisionVertexLinks[line_id].vertex1 + vertex_id]].pos.x;
    pos->y = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[gMPCollisionVertexLinks[line_id].vertex1 + vertex_id]].pos.y;
    pos->z = 0.0F;

    yakumono_dobj = gMPCollisionYakumonoDObjs->dobjs[gMPCollisionVertexInfo->vertex_info[line_id].yakumono_id];

    if ((yakumono_dobj->anim_joint.event32 != NULL) || (yakumono_dobj->user_data.s != nMPYakumonoStatusNone))
    {
        pos->x += yakumono_dobj->translate.vec.f.x;
        pos->y += yakumono_dobj->translate.vec.f.y;
    }
}

// 0x800FA7B8
void mpCollisionGetSpeedLineID(s32 line_id, Vec3f *speed)
{
    DObj *yakumono_dobj;
    u8 yakumono_id;

    if ((line_id == -1) || (line_id == -2))
    {
        while (TRUE)
        {
            syDebugPrintf("mpGetSpeedId() id = %d\n", line_id);
            scManagerRunPrintGObjStatus();
        }
    }
    yakumono_id = gMPCollisionVertexInfo->vertex_info[line_id].yakumono_id;
    yakumono_dobj = gMPCollisionYakumonoDObjs->dobjs[yakumono_id];

    if (yakumono_dobj->user_data.s >= nMPYakumonoStatusOff)
    {
        while (TRUE)
        {
            syDebugPrintf("mpGetSpeedId() no collision\n");
            scManagerRunPrintGObjStatus();
        }
    }
    *speed = gMPCollisionSpeeds[yakumono_id];
}

// 0x800FA8A4 - Get the direction a line ID pushes outwards
s32 mpCollisionGetLineTypeID(s32 line_id)
{
    DObj *yakumono_dobj;
    MPVertexInfo *vertex_info;

    if ((line_id == -1) || (line_id == -2))
    {
        while (TRUE)
        {
            syDebugPrintf("mpGetKindId() id = %d\n", line_id);
            scManagerRunPrintGObjStatus();
        }
    }
    vertex_info = &gMPCollisionVertexInfo->vertex_info[line_id];
    yakumono_dobj = gMPCollisionYakumonoDObjs->dobjs[vertex_info->yakumono_id];

    if (yakumono_dobj->user_data.s == nMPYakumonoStatusOff)
    {
        while (TRUE)
        {
            syDebugPrintf("mpGetKindId() no collision\n");
            scManagerRunPrintGObjStatus();
        }
    }
    return vertex_info->line_type;
}

// 0x800FA964 - Get line ID connecting to the right edge of a ground
s32 mpCollisionGetEdgeUnderRLineID(s32 line_id)
{
    DObj *yakumono_dobj;
    MPVertexInfo *vertex_info;

    if ((line_id == -1) || (line_id == -2))
    {
        while (TRUE)
        {
            syDebugPrintf("mpGetEdgeUnderRId() id = %d\n", line_id);
            scManagerRunPrintGObjStatus();
        }
    }
    vertex_info = &gMPCollisionVertexInfo->vertex_info[line_id];
    yakumono_dobj = gMPCollisionYakumonoDObjs->dobjs[vertex_info->yakumono_id];

    if (yakumono_dobj->user_data.s >= nMPYakumonoStatusOff)
    {
        while (TRUE)
        {
            syDebugPrintf("mpGetEdgeUnderRId() no collision\n");
            scManagerRunPrintGObjStatus();
        }
    }
    return vertex_info->edge_next_line_id;
}

// 0x800FAA24 - Get line ID connecting to the left edge of a ground
s32 mpCollisionGetEdgeUnderLLineID(s32 line_id)
{
    DObj *yakumono_dobj;
    MPVertexInfo *vertex_info;

    if ((line_id == -1) || (line_id == -2))
    {
        while (TRUE)
        {
            syDebugPrintf("mpGetEdgeUnderLId() id = %d\n", line_id);
            scManagerRunPrintGObjStatus();
        }
    }
    vertex_info = &gMPCollisionVertexInfo->vertex_info[line_id];
    yakumono_dobj = gMPCollisionYakumonoDObjs->dobjs[vertex_info->yakumono_id];

    if (yakumono_dobj->user_data.s >= nMPYakumonoStatusOff)
    {
        while (TRUE)
        {
            syDebugPrintf("mpGetEdgeUnderLId() no collision\n");
            scManagerRunPrintGObjStatus();
        }
    }
    return vertex_info->edge_prev_line_id;
}

// 0x800FAAE4 - Get line ID connecting to the right edge of a ceiling
s32 mpCollisionGetEdgeUpperRLineID(s32 line_id)
{
    DObj *yakumono_dobj;
    MPVertexInfo *vertex_info;

    if ((line_id == -1) || (line_id == -2))
    {
        while (TRUE)
        {
            syDebugPrintf("mpGetEdgeUpperRId() id = %d\n", line_id);
            scManagerRunPrintGObjStatus();
        }
    }
    vertex_info = &gMPCollisionVertexInfo->vertex_info[line_id];
    yakumono_dobj = gMPCollisionYakumonoDObjs->dobjs[vertex_info->yakumono_id];

    if (yakumono_dobj->user_data.s >= nMPYakumonoStatusOff)
    {
        while (TRUE)
        {
            syDebugPrintf("mpGetEdgeUpperRId() no collision\n");
            scManagerRunPrintGObjStatus();
        }
    }
    return vertex_info->edge_next_line_id;
}

// 0x800FABA4 - Get line ID connecting to the left edge of a ceiling
s32 mpCollisionGetEdgeUpperLLineID(s32 line_id)
{
    DObj *yakumono_dobj;
    MPVertexInfo *vertex_info;

    if ((line_id == -1) || (line_id == -2))
    {
        while (TRUE)
        {
            syDebugPrintf("mpGetEdgeUpperLId() id = %d\n", line_id);
            scManagerRunPrintGObjStatus();
        }
    }
    vertex_info = &gMPCollisionVertexInfo->vertex_info[line_id];
    yakumono_dobj = gMPCollisionYakumonoDObjs->dobjs[vertex_info->yakumono_id];

    if (yakumono_dobj->user_data.s >= nMPYakumonoStatusOff)
    {
        while (TRUE)
        {
            syDebugPrintf("mpGetEdgeUpperLId() no collision\n");
            scManagerRunPrintGObjStatus();
        }
    }
    return vertex_info->edge_prev_line_id;
}

// 0x800FAC64 - Get line ID connecting to the upper edge of a left wall
s32 mpCollisionGetEdgeRightULineID(s32 line_id)
{
    DObj *yakumono_dobj;
    MPVertexInfo *vertex_info;

    if ((line_id == -1) || (line_id == -2))
    {
        while (TRUE)
        {
            syDebugPrintf("mpGetEdgeRightUId() id = %d\n", line_id);
            scManagerRunPrintGObjStatus();
        }
    }
    vertex_info = &gMPCollisionVertexInfo->vertex_info[line_id];
    yakumono_dobj = gMPCollisionYakumonoDObjs->dobjs[vertex_info->yakumono_id];

    if (yakumono_dobj->user_data.s >= nMPYakumonoStatusOff)
    {
        while (TRUE)
        {
            syDebugPrintf("mpGetEdgeRightUId() no collision\n");
            scManagerRunPrintGObjStatus();
        }
    }
    return vertex_info->edge_next_line_id;
}

// 0x800FAD24 - Get line ID connecting to the lower edge of a left wall
s32 mpCollisionGetEdgeRightDLineID(s32 line_id)
{
    DObj *yakumono_dobj;
    MPVertexInfo *vertex_info;

    if ((line_id == -1) || (line_id == -2))
    {
        while (TRUE)
        {
            syDebugPrintf("mpGetEdgeRightDId() id = %d\n", line_id);
            scManagerRunPrintGObjStatus();
        }
    }
    vertex_info = &gMPCollisionVertexInfo->vertex_info[line_id];
    yakumono_dobj = gMPCollisionYakumonoDObjs->dobjs[vertex_info->yakumono_id];

    if (yakumono_dobj->user_data.s >= nMPYakumonoStatusOff)
    {
        while (TRUE)
        {
            syDebugPrintf("mpGetEdgeRightDId() no collision\n");
            scManagerRunPrintGObjStatus();
        }
    }
    return vertex_info->edge_prev_line_id;
}

// 0x800FADE4 - Get line ID connecting to the upper edge of a right wall
s32 mpCollisionGetEdgeLeftULineID(s32 line_id)
{
    DObj *yakumono_dobj;
    MPVertexInfo *vertex_info;

    if ((line_id == -1) || (line_id == -2))
    {
        while (TRUE)
        {
            syDebugPrintf("mpGetEdgeLeftUId() id = %d\n", line_id);
            scManagerRunPrintGObjStatus();
        }
    }
    vertex_info = &gMPCollisionVertexInfo->vertex_info[line_id];
    yakumono_dobj = gMPCollisionYakumonoDObjs->dobjs[vertex_info->yakumono_id];

    if (yakumono_dobj->user_data.s >= nMPYakumonoStatusOff)
    {
        while (TRUE)
        {
            syDebugPrintf("mpGetEdgeLeftUId() no collision\n");
            scManagerRunPrintGObjStatus();
        }
    }
    return vertex_info->edge_next_line_id;
}

// 0x800FAEA4 - Get line ID connecting to the lower edge of a right wall
s32 mpCollisionGetEdgeLeftDLineID(s32 line_id)
{
    DObj *yakumono_dobj;
    MPVertexInfo *vertex_info;

    if ((line_id == -1) || (line_id == -2))
    {
        while (TRUE)
        {
            syDebugPrintf("mpGetEdgeLeftDId() id = %d\n", line_id);
            scManagerRunPrintGObjStatus();
        }
    }
    vertex_info = &gMPCollisionVertexInfo->vertex_info[line_id];
    yakumono_dobj = gMPCollisionYakumonoDObjs->dobjs[vertex_info->yakumono_id];

    if (yakumono_dobj->user_data.s >= nMPYakumonoStatusOff)
    {
        while (TRUE)
        {
            syDebugPrintf("mpGetEdgeLeftDId() no collision\n");
            scManagerRunPrintGObjStatus();
        }
    }
    return vertex_info->edge_prev_line_id;
}

// 0x800FAF64
void mpCollisionGetPlayerMapObjPosition(s32 player, Vec3f *pos)
{
    if (!(gMPCollisionGeometry->mapobj_count))
    {
        pos->x = pos->y = pos->z = 0.0F;
    }
    else
    {
        s32 i, id = player;

        for (i = 0; i < gMPCollisionGeometry->mapobj_count; i++)
        {
            if (id == gMPCollisionMapObjs->mapobjs[i].mapobj_kind)
            {
                pos->x = gMPCollisionMapObjs->mapobjs[i].pos.x;
                pos->y = gMPCollisionMapObjs->mapobjs[i].pos.y;
                pos->z = 0;

                break;
            }
        }
    }
}

// 0x800FB010
void mpCollisionAllocVertexInfo(void)
{
    gMPCollisionVertexInfo = syTaskmanMalloc(gMPCollisionLinesNum * sizeof(MPVertexInfo), 0x8);
}

// 0x800FB04C
void func_ovl2_800FB04C(void)
{
    s32 line_id;
    s32 i;
    MPLineInfo *line_info;
    MPLineData *line_data;
    s32 j;
    s32 k;
    s32 l;
    s32 yakumono_id;
    s32 vnear;
    s32 vfar;
    s32 vp2;
    s32 index;

    line_info = gMPCollisionGeometry->line_info;

    for (i = l = 0; i < gMPCollisionGeometry->yakumono_count; i++, line_info++)
    {
        line_data = &line_info->line_data[0];
        yakumono_id = line_info->yakumono_id;

        for (j = 0; j < ARRAY_COUNT(line_info->line_data); j++)
        {
            for (index = line_data[j].group_id, line_id = 0; line_id < line_data[j].line_count; line_id++, index++)
            {
                MPVertexLinks *vlinks = &gMPCollisionVertexLinks[index];

                vfar = vnear = (j < ARRAY_COUNT(line_info->line_data) / 2) ? gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vlinks->vertex1]].pos.y:
                                                                             gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[vlinks->vertex1]].pos.x;

                for (k = vlinks->vertex1 + 1; k < (vlinks->vertex1 + vlinks->vertex2); k++)
                {
                    // There are various ways to fake-match this function (e.g. do {} while (0), if(1), if(!gMPCollisionVertexData->vpos) right where this comment is)...
                    vp2 = (j < ARRAY_COUNT(line_info->line_data) / 2) ? gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[k]].pos.y:
                                                                        gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[k]].pos.x;

                    if (vp2 >= vfar)
                    {
                        vfar = vp2;
                    }
                    if (vnear >= vp2)
                    {
                        vnear = vp2;
                    }
                    else continue; // ...but this seems to be the most realistic solution for now.
                }
                gMPCollisionVertexInfo->vertex_info[l].coll_pos_next = vfar;
                gMPCollisionVertexInfo->vertex_info[l].coll_pos_prev = vnear;
                gMPCollisionVertexInfo->vertex_info[l].yakumono_id = yakumono_id;

                l++;
            }
        }
    }
}

// 0x800FB2A0
void mpCollisionInitLineTypesAll(void)
{
    s32 line_type, line_id;

    for (line_type = 0; line_type < ARRAY_COUNT(gMPCollisionLineGroups); line_type++)
    {
        for (line_id = 0; line_id < gMPCollisionLineGroups[line_type].line_count; line_id++)
        {
            gMPCollisionVertexInfo->vertex_info[gMPCollisionLineGroups[line_type].line_id[line_id]].line_type = line_type;
        }
    }
}

// 0x800FB31C
void func_ovl2_800FB31C(void)
{
    s32 line_id;
    s32 line_next;
    s32 line_prev;
    s32 i;
    sb32 unk_bool;
    s32 vertex_id1;
    s32 vertex_id2;
    s32 vid;
    s32 vtemp1, vtemp2;

    for (i = 0; i < gMPCollisionLinesNum; i++)
    {
        line_next = -1;
        line_prev = -1;

        unk_bool = FALSE;

        vtemp1 = gMPCollisionVertexLinks[i].vertex1;

        vertex_id1 = gMPCollisionVertexIDs->vertex_id[vtemp1];
        vertex_id2 = gMPCollisionVertexIDs->vertex_id[vtemp1 + gMPCollisionVertexLinks[i].vertex2 - 1];

        for (line_id = 0; line_id < gMPCollisionLinesNum; line_id++)
        {
            if (i != line_id)
            {
                vtemp2 = gMPCollisionVertexLinks[line_id].vertex1;

                vid = vtemp2 + gMPCollisionVertexLinks[line_id].vertex2 - 1;

                if ((vertex_id1 == gMPCollisionVertexIDs->vertex_id[vtemp2]) || (vertex_id1 == gMPCollisionVertexIDs->vertex_id[vid]))
                {
                    line_prev = line_id;
                }
                if ((vertex_id2 == gMPCollisionVertexIDs->vertex_id[vtemp2]) || (vertex_id2 == gMPCollisionVertexIDs->vertex_id[vid]))
                {
                    line_next = line_id;
                }
            }
            continue;
        }
        switch (gMPCollisionVertexInfo->vertex_info[i].line_type)
        {
        default:
            break;

        case nMPLineKindFloor:
        case nMPLineKindCeil:
            if (gMPCollisionVertexData->vpos[vertex_id2].pos.x < gMPCollisionVertexData->vpos[vertex_id1].pos.x)
            {
                unk_bool = TRUE;
            }
            break;

        case nMPLineKindRWall:
        case nMPLineKindLWall:
            if (gMPCollisionVertexData->vpos[vertex_id2].pos.y < gMPCollisionVertexData->vpos[vertex_id1].pos.y)
            {
                unk_bool = TRUE;
            }
            break;
        }
        if (unk_bool != FALSE)
        {
            gMPCollisionVertexInfo->vertex_info[i].edge_next_line_id = line_prev;
            gMPCollisionVertexInfo->vertex_info[i].edge_prev_line_id = line_next;
        }
        else
        {
            gMPCollisionVertexInfo->vertex_info[i].edge_next_line_id = line_next;
            gMPCollisionVertexInfo->vertex_info[i].edge_prev_line_id = line_prev;
        }
        continue;
    }
}

// 0x800FB554
void func_ovl2_800FB554(void)
{
    func_ovl2_800FB04C();
    mpCollisionInitLineTypesAll();
    func_ovl2_800FB31C();
}

// 0x800FB584
void mpCollisionAllocYakumono(DObjDesc *dobjdesc)
{
    s32 yakumonos_num;
    s32 i;

    for (yakumonos_num = 0; dobjdesc->id != DOBJ_ARRAY_MAX; yakumonos_num++)
    {
        dobjdesc++;
    }
    gMPCollisionYakumonoDObjs = syTaskmanMalloc(yakumonos_num * sizeof(gMPCollisionYakumonoDObjs), 0x4);
    gMPCollisionSpeeds = syTaskmanMalloc(yakumonos_num * sizeof(Vec3f), 0x4);

    for (i = 0; i < yakumonos_num; i++)
    {
        gMPCollisionSpeeds[i].x = gMPCollisionSpeeds[i].y = gMPCollisionSpeeds[i].z = 0.0F;
    }
    gMPCollisionYakumonosNum = yakumonos_num;
}

// 0x800FB808
void mpCollisionUpdateBoundsCurrent(void)
{
    s32 index;
    MPBounds bounds;
    MPLineData *line_data;
    MPLineInfo *line_info;
    DObj *yakumono_dobj;
    MPVertexLinks *vlinks;
    s32 i, j, k, l;

    bounds = gMPCollisionBounds.stop;

    line_info = gMPCollisionGeometry->line_info;

    for (i = 0; i < gMPCollisionGeometry->yakumono_count; i++, line_info++)
    {
        yakumono_dobj = gMPCollisionYakumonoDObjs->dobjs[line_info->yakumono_id];

        if ((yakumono_dobj->anim_joint.event32 != NULL) || (yakumono_dobj->user_data.s < nMPYakumonoStatusOff) && (yakumono_dobj->user_data.s != nMPYakumonoStatusNone))
        {
            f32 x = yakumono_dobj->translate.vec.f.x, y = yakumono_dobj->translate.vec.f.y;

            line_data = &line_info->line_data[0];

            for (j = 0; j < ARRAY_COUNT(line_info->line_data); j++)
            {
                for (index = line_data[j].group_id, k = 0; k < line_data[j].line_count; k++, index++)
                {
                    vlinks = &gMPCollisionVertexLinks[index];

                    for (l = vlinks->vertex1; l < (vlinks->vertex1 + vlinks->vertex2); l++)
                    {
                        s32 vx = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[l]].pos.x + x;
                        s32 vy = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[l]].pos.y + y;

                        if (bounds.top < vy)
                        {
                            bounds.top = vy;
                        }
                        else if (bounds.bottom > vy)
                        {
                            bounds.bottom = vy;
                        }
                        if (bounds.right < vx)
                        {
                            bounds.right = vx;
                        }
                        else if (bounds.left > vx)
                        {
                            bounds.left = vx;
                        }
                    }
                }
            }
        }
    }
    gMPCollisionBounds.current = bounds;
}

// 0x800FBA84
void mpCollisionUpdateBoundsDiff(void)
{
    gMPCollisionBounds.diff.top    = gMPCollisionBounds.current.top    - gMPCollisionBounds.start.top;
    gMPCollisionBounds.diff.bottom = gMPCollisionBounds.current.bottom - gMPCollisionBounds.start.bottom;
    gMPCollisionBounds.diff.right  = gMPCollisionBounds.current.right  - gMPCollisionBounds.start.right;
    gMPCollisionBounds.diff.left   = gMPCollisionBounds.current.left   - gMPCollisionBounds.start.left;
}

// 0x800FBAD0
void mpCollisionPlayYakumonoAnim(GObj *ground_gobj)
{
    DObj *dobj = DObjGetStruct(ground_gobj);
    s32 i = 0;

    while (dobj != NULL)
    {
        MObj *mobj;
        Vec3f translate;

        if (dobj == gMPCollisionYakumonoDObjs->dobjs[i])
        {
            if ((dobj->user_data.s != nMPYakumonoStatusOn) && (dobj->user_data.s != nMPYakumonoStatusOff))
            {
                u8 flags = dobj->flags;

                gcParseDObjAnimJoint(dobj);

                translate = dobj->translate.vec.f;

                gcPlayDObjAnimJoint(dobj);

                gMPCollisionSpeeds[i].x = dobj->translate.vec.f.x - translate.x;
                gMPCollisionSpeeds[i].y = dobj->translate.vec.f.y - translate.y;
                gMPCollisionSpeeds[i].z = dobj->translate.vec.f.z - translate.z;

                if (flags == DOBJ_FLAG_NONE)
                {
                    if (dobj->flags != DOBJ_FLAG_NONE)
                    {
                        dobj->user_data.s = nMPYakumonoStatusHidden;
                    }
                }
                else if (dobj->flags == DOBJ_FLAG_NONE)
                {
                    dobj->user_data.s = nMPYakumonoStatusShow;
                }
            }
            if (i < (gMPCollisionYakumonosNum - 1))
            {
                i++;
            }
        }
        else
        {
            gcParseDObjAnimJoint(dobj);
            gcPlayDObjAnimJoint(dobj);
        }
        mobj = dobj->mobj;

        while (mobj != NULL)
        {
            gcParseMObjMatAnimJoint(mobj);
            gcPlayMObjMatAnim(mobj);

            mobj = mobj->next;
        }
        if (dobj->child != NULL)
        {
            dobj = dobj->child;
        }
        else if (dobj->sib_next != NULL)
        {
            dobj = dobj->sib_next;
        }
        else while (TRUE)
        {
            if (dobj->parent == DOBJ_PARENT_NULL)
            {
                dobj = NULL;

                break;
            }
            else if (dobj->parent->sib_next != NULL)
            {
                dobj = dobj->parent->sib_next;

                break;
            }
            else
            {
                dobj = dobj->parent;

                continue;
            }
        }
    }
    mpCollisionUpdateBoundsCurrent();
    mpCollisionUpdateBoundsDiff();

    gMPCollisionUpdateTic++;
}

// 0x800FBCF8
void mpCollisionAdvanceUpdateTic(GObj *ground_gobj)
{
    gMPCollisionUpdateTic++;
}

// 0x800FBD14
void mpCollisionInitYakumonoAll(void)
{
    s32 index;
    DObj *yakumono_dobj;
    MPBounds bounds_moved;
    MPBounds bounds_static;
    s32 vx;
    s32 vy;
    f32 tx;
    f32 ty;
    MPLineData *line_data;
    MPLineInfo *line_info;
    MPVertexLinks *vlinks;
    s32 i, j, k, l;
    s32 unused[2];

    line_info = gMPCollisionGeometry->line_info;

    bounds_moved.top = -65536.0F;
    bounds_moved.right = -65536.0F;
    bounds_static.top = -65536.0F;
    bounds_static.right = -65536.0F;

    bounds_static.left = 65536.0F;
    bounds_static.bottom = 65536.0F;
    bounds_moved.left = 65536.0F;
    bounds_moved.bottom = 65536.0F;

    for (i = 0; i < gMPCollisionGeometry->yakumono_count; i++, line_info++)
    {
        yakumono_dobj = gMPCollisionYakumonoDObjs->dobjs[line_info->yakumono_id];

        if ((yakumono_dobj->user_data.s != nMPYakumonoStatusOn) && (yakumono_dobj->user_data.s != nMPYakumonoStatusOff))
        {
            if (yakumono_dobj->anim_joint.event32 != NULL)
            {
                tx = yakumono_dobj->translate.vec.f.x;
                ty = yakumono_dobj->translate.vec.f.y;
            }
            else
            {
                tx = 0.0F;
                ty = 0.0F;
            }
            line_data = &line_info->line_data[0];

            for (j = 0; j < ARRAY_COUNT(line_info->line_data); j++)
            {
                for (index = line_data[j].group_id, k = 0; k < line_data[j].line_count; k++, index++)
                {
                    vlinks = &gMPCollisionVertexLinks[index];

                    for (l = vlinks->vertex1; l < (vlinks->vertex1 + vlinks->vertex2); l++)
                    {
                        vx = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[l]].pos.x;
                        vy = gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[l]].pos.y;

                        if (yakumono_dobj->anim_joint.event32 == NULL)
                        {
                            if (bounds_static.top < vy)
                            {
                                bounds_static.top = vy;
                            }
                            if (bounds_static.bottom > vy)
                            {
                                bounds_static.bottom = vy;
                            }
                            if (bounds_static.right < vx)
                            {
                                bounds_static.right = vx;
                            }
                            if (bounds_static.left > vx)
                            {
                                bounds_static.left = vx;
                            }
                        }
                        vx += tx;
                        vy += ty;

                        if (bounds_moved.top < vy)
                        {
                            bounds_moved.top = vy;
                        }
                        if (bounds_moved.bottom > vy)
                        {
                            bounds_moved.bottom = vy;
                        }
                        if (bounds_moved.right < vx)
                        {
                            bounds_moved.right = vx;
                        }
                        if (bounds_moved.left > vx)
                        {
                            bounds_moved.left = vx;
                        }
                    }
                }
            }
        }
    }
    // Slight WARNING: not sure about this. The member names in the struct don't seem to correspond to their suggested purpose...
    // ...or I just don't understand what is going on. Or both.
    gMPCollisionBounds.start = bounds_moved;
    gMPCollisionBounds.stop = bounds_static;
    gMPCollisionBounds.current = bounds_moved;
    gMPCollisionBounds.diff.top = gMPCollisionBounds.diff.bottom = gMPCollisionBounds.diff.right = gMPCollisionBounds.diff.left = 0.0F;
}

// 0x800FC09C
s32 mpCollisionAllocLinesGetCountTotal(void)
{
    MPLineInfo *line_info;
    MPLineData *line_data;
    s32 i;
    s32 line_count[nMPLineKindEnumCount];
    s32 line_total, j;

    for (i = 0; i < ARRAY_COUNT(line_count); i++)
    {
        line_count[i] = 0;
    }
    line_info = gMPCollisionGeometry->line_info;

    for (i = 0; i < gMPCollisionGeometry->yakumono_count; i++, line_info++)
    {
        line_data = &line_info->line_data[0];

        for (j = 0; j < (ARRAY_COUNT(line_count) + ARRAY_COUNT(line_info->line_data)) / 2; j++, line_data++)
        {
            line_count[j] += line_data->line_count;
        }
    }
    for (i = 0, line_total = 0; i < (ARRAY_COUNT(line_count) + ARRAY_COUNT(gMPCollisionLineGroups)) / 2; i++)
    {
        gMPCollisionLineGroups[i].line_count = line_count[i];

        line_total += line_count[i];

        if (line_count[i] != 0)
        {
            gMPCollisionLineGroups[i].line_id = (u16*) syTaskmanMalloc(line_count[i] * sizeof(*gMPCollisionLineGroups[i].line_id), 0x2);
        }
    }
    return line_total;
}

// 0x800FC1A4
void mpCollisionInitLineIDsAll(void)
{
    MPLineData *line_data;
    MPLineInfo *line_info;
    s32 i;
    s32 line_count[nMPLineKindEnumCount];
    s32 j, line_id;

    for (i = 0; i < ARRAY_COUNT(line_count); i++)
    {
        line_count[i] = 0;
    }
    line_info = gMPCollisionGeometry->line_info;

    for (i = 0; i < gMPCollisionGeometry->yakumono_count; i++, line_info++)
    {
        line_data = &line_info->line_data[0];

        for (j = 0; j < (ARRAY_COUNT(line_count) + ARRAY_COUNT(line_info->line_data)) / 2; j++, line_data++)
        {
            for (line_id = line_data->group_id; line_id < (line_data->group_id + line_data->line_count); line_id++)
            {
                gMPCollisionLineGroups[j].line_id[line_count[j]] = line_id;
                line_count[j]++;
            }
        }
    }
}

// 0x800FC284
void mpCollisionInitGroundData(void)
{
    MPGeometryData *gdata;

    gMPCollisionGroundData = lbRelocGetFileData
    (
        MPGroundData*,
        lbRelocGetExternHeapFile
        (
            dMPCollisionGroundFileInfos[gSCManagerBattleState->gkind].file_id, 
            syTaskmanMalloc
            (
                lbRelocGetFileSize(dMPCollisionGroundFileInfos[gSCManagerBattleState->gkind].file_id),
                0x10
            )
        ),
        dMPCollisionGroundFileInfos[gSCManagerBattleState->gkind].offset
    );

    gMPCollisionGeometry = gMPCollisionGroundData->map_geometry;
    gdata = gMPCollisionGeometry;

    if (gdata == NULL)
    {
        while (TRUE)
        {
            syDebugPrintf("not found cll data!\n");
            scManagerRunPrintGObjStatus();
        }
    }
    gMPCollisionVertexData  = gdata->vertex_data;
    gMPCollisionVertexIDs   = gdata->vertex_id;
    gMPCollisionVertexLinks = gdata->vertex_links;
    gMPCollisionMapObjs     = gdata->mapobjs;

    gMPCollisionLinesNum = mpCollisionAllocLinesGetCountTotal();

    mpCollisionInitLineIDsAll();
    mpCollisionAllocVertexInfo();
    func_ovl2_800FB554();
    mpCollisionAllocYakumono(gMPCollisionGroundData->gr_desc[1].dobjdesc);

    gMPCollisionLightColor.r = 0xFF;
    gMPCollisionLightColor.g = 0xFF;
    gMPCollisionLightColor.b = 0xFF;
    gMPCollisionLightColor.a = 0xFF;

    gMPCollisionLightAngleX = gMPCollisionGroundData->light_angle.x;
    gMPCollisionLightAngleY = gMPCollisionGroundData->light_angle.y;
}

// 0x800FC3E8
void mpCollisionSetPlayBGM(void)
{
    gMPCollisionBGMDefault = gMPCollisionGroundData->bgm_id;

    syAudioPlayBGM(0, gMPCollisionBGMDefault);

    gMPCollisionBGMCurrent = gMPCollisionBGMDefault;
}

// 0x800FC42C
void mpCollisionSetBGM(void)
{
    gMPCollisionBGMCurrent = gMPCollisionBGMDefault = gMPCollisionGroundData->bgm_id;
}

// 0x800FC450
void mpCollisionClearYakumonoAll(void)
{
    DObjDesc *dobjdesc = gMPCollisionGroundData->gr_desc[1].dobjdesc;
    s32 i;

    for (i = 0; dobjdesc->id != DOBJ_ARRAY_MAX; i++, dobjdesc++)
    {
        gMPCollisionYakumonoDObjs->dobjs[i]->user_data.s = nMPYakumonoStatusNone;
    }
    gMPCollisionUpdateTic = 0;
}

// 0x800FC4A8
void mpCollisionSetYakumonoPosID(s32 line_id, Vec3f *yakumono_pos)
{
    DObj *yakumono_dobj;

    if ((line_id == -1) || (line_id == -2))
    {
        while (TRUE)
        {
            syDebugPrintf("mpSetYakumonoPosId() id = %d\n", line_id);
            scManagerRunPrintGObjStatus();
        }
    }
    yakumono_dobj = gMPCollisionYakumonoDObjs->dobjs[line_id];

    gMPCollisionSpeeds[line_id].x = yakumono_pos->x - yakumono_dobj->translate.vec.f.x;
    gMPCollisionSpeeds[line_id].y = yakumono_pos->y - yakumono_dobj->translate.vec.f.y;
    gMPCollisionSpeeds[line_id].z = yakumono_pos->z - yakumono_dobj->translate.vec.f.z;

    yakumono_dobj->translate.vec.f.x = yakumono_pos->x;
    yakumono_dobj->translate.vec.f.y = yakumono_pos->y;
    yakumono_dobj->translate.vec.f.z = yakumono_pos->z;
}

// 0x800FC58C
void mpCollisionSetYakumonoOnID(s32 line_id)
{
    if ((line_id == -1) || (line_id == -2))
    {
        while (TRUE)
        {
            syDebugPrintf("mpSetYakumonoOnId() id = %d\n", line_id);
            scManagerRunPrintGObjStatus();
        }
    }
    gMPCollisionYakumonoDObjs->dobjs[line_id]->user_data.s = nMPYakumonoStatusOn;
}

// 0x800FC604
void mpCollisionSetYakumonoOffID(s32 line_id)
{
    if ((line_id == -1) || (line_id == -2))
    {
        while (TRUE)
        {
            syDebugPrintf("mpSetYakumonoOffId() id = %d\n", line_id);
            scManagerRunPrintGObjStatus();
        }
    }
    gMPCollisionYakumonoDObjs->dobjs[line_id]->user_data.s = nMPYakumonoStatusOff;
}

// 0x800FC67C
sb32 mpCollisionCheckExistLineID(s32 line_id)
{
    if (line_id == -1)
    {
        while (TRUE)
        {
            syDebugPrintf("mpGetExistCollisionId() id = %d\n", line_id);
            scManagerRunPrintGObjStatus();
        }
    }
    if (line_id == -2)
    {
        return FALSE;
    }
    if (gMPCollisionYakumonoDObjs->dobjs[gMPCollisionVertexInfo->vertex_info[line_id].yakumono_id]->user_data.s < nMPYakumonoStatusOff)
    {
        return TRUE;
    }
    else return FALSE;
}

// 0x800FC72C
s32 mpCollisionSetDObjNoID(s32 line_id)
{
    if ((line_id == -1) || (line_id == -2))
    {
        while (TRUE)
        {
            syDebugPrintf("mpSetDObjNoId() id = %d\n", line_id);
            scManagerRunPrintGObjStatus();
        }
    }
    return gMPCollisionVertexInfo->vertex_info[line_id].yakumono_id;
}

// 0x800FC7A4
s32 mpCollisionGetMapObjCountKind(s32 mapobj_kind)
{
    s32 i, count;

    if (!(gMPCollisionGeometry->mapobj_count))
    {
        return 0;
    }
    else for (i = count = 0; i < gMPCollisionGeometry->mapobj_count; i++)
    {
        if (mapobj_kind == gMPCollisionMapObjs->mapobjs[i].mapobj_kind)
        {
            count++;
        }
    }
    return count;
}

// 0x800FC814
void mpCollisionGetMapObjIDsKind(s32 mapobj_kind, s32 *mapobjs)
{
    s32 i, count;

    if (gMPCollisionGeometry->mapobj_count)
    {
        for (i = count = 0; i < gMPCollisionGeometry->mapobj_count; i++)
        {
            if (mapobj_kind == gMPCollisionMapObjs->mapobjs[i].mapobj_kind)
            {
                mapobjs[count] = i;

                count++;
            }
        }
    }
}

// 0x800FC894
void mpCollisionGetMapObjPositionID(s32 mapobj_kind, Vec3f *pos)
{
    pos->x = gMPCollisionMapObjs->mapobjs[mapobj_kind].pos.x;
    pos->y = gMPCollisionMapObjs->mapobjs[mapobj_kind].pos.y;
    pos->z = 0.0F;
}

// 0x800FC8EC
s32 mpCollisionGetLineCountType(s32 line_type)
{
    return gMPCollisionLineGroups[line_type].line_count;
}

// 0x800FC900
void mpCollisionGetLineIDsTypeCount(s32 type, s32 count, s32 *line_ids)
{
    s32 i;

    for (i = 0; i < count; i++)
    {
        line_ids[i] = gMPCollisionLineGroups[type].line_id[i];
    }
}

// 0x800FC9C8
u8 mpCollisionSetLightColorGetAlpha(Gfx **dls)
{
    gDPSetEnvColor(dls[0]++, gMPCollisionLightColor.r, gMPCollisionLightColor.g, gMPCollisionLightColor.b, gMPCollisionLightColor.a);

    return gMPCollisionLightColor.a;
}

// 0x800FCA18
sb32 mpCollisionCheckExistPlatformLineID(s32 line_id)
{
    DObj *yakumono_dobj;

    if ((line_id == -1) || (line_id == -2))
    {
        while (TRUE)
        {
            syDebugPrintf("mpGetCllFloatId() id = %d\n", line_id);
            scManagerRunPrintGObjStatus();
        }
    }
    yakumono_dobj = gMPCollisionYakumonoDObjs->dobjs[gMPCollisionVertexInfo->vertex_info[line_id].yakumono_id];

    if ((yakumono_dobj->user_data.s != nMPYakumonoStatusNone) || (yakumono_dobj->anim_joint.event32 != NULL))
    {
        return TRUE;
    }
    else return FALSE;
}

// 0x800FCAC8
u16 mpCollisionGetVertexFlagsLineID(s32 line_id)
{
    if ((line_id == -1) || (line_id == -2))
    {
        while (TRUE)
        {
            syDebugPrintf("mpGetAttrId() id = %d\n", line_id);
            scManagerRunPrintGObjStatus();
        }
    }
    return gMPCollisionVertexData->vpos[gMPCollisionVertexIDs->vertex_id[gMPCollisionVertexLinks[line_id].vertex1]].vertex_flags;
}
