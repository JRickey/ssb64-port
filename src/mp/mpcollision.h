#ifndef _MPCOLLISION_H_
#define _MPCOLLISION_H_

#include <PR/gbi.h>
#include <ssb_types.h>
#include <sys/objdef.h>
#include <mp/mpdef.h>

extern f32 mpCollisionGetLineDistanceFC(f32 opx, s32 vx1, s32 vy1, s32 vx2, s32 vy2);
extern f32 mpCollisionGetLineDistanceLR(f32 opy, s32 vx1, s32 vy1, s32 vx2, s32 vy2);
extern sb32 mpCollisionGetFCCommon(s32 line_id, Vec3f *object_pos, f32 *dist, u32 *flags, Vec3f *angle, s32 ud);
extern sb32 mpCollisionGetFCCommonFloor(s32 line_id, Vec3f *object_pos, f32 *dist, u32 *flags, Vec3f *angle);
extern sb32 mpCollisionGetFCCommonCeil(s32 line_id, Vec3f *object_pos, f32 *dist, u32 *flags, Vec3f *angle);
extern sb32 mpCollisionGetLRCommon(s32 line_id, Vec3f *object_pos, f32 *dist, u32 *flags, Vec3f *angle, s32 lr);
extern sb32 mpCollisionGetLRCommonLWall(s32 line_id, Vec3f *object_pos, f32 *dist, u32 *flags, Vec3f *angle);
extern sb32 mpCollisionGetLRCommonRWall(s32 line_id, Vec3f *object_pos, f32 *dist, u32 *flags, Vec3f *angle);
extern void mpCollisionGetLREdge(s32 line_id, Vec3f *object_pos, s32 lr);
extern void mpCollisionGetFloorEdgeR(s32 line_id, Vec3f *object_pos);
extern void mpCollisionGetFloorEdgeL(s32 line_id, Vec3f *object_pos);
extern void mpCollisionGetCeilEdgeR(s32 line_id, Vec3f *object_pos);
extern void mpCollisionGetCeilEdgeL(s32 line_id, Vec3f *object_pos);
extern void mpCollisionGetUDEdge(s32 line_id, Vec3f *object_pos, s32 ud);
extern void mpCollisionGetLWallEdgeU(s32 line_id, Vec3f *object_pos);
extern void mpCollisionGetLWallEdgeD(s32 line_id, Vec3f *object_pos);
extern void mpCollisionGetRWallEdgeU(s32 line_id, Vec3f *object_pos);
extern void mpCollisionGetRWallEdgeD(s32 line_id, Vec3f *object_pos);
extern void mpCollisionGetFCAngle(Vec3f *angle, s32 v1x, s32 v1y, s32 v2x, s32 v2y, s32 lr);
extern sb32 mpCollisionCheckFloorSurfaceTilt(s32 v1x, s32 v1y, s32 v2x, s32 v2y, f32 d1x, f32 d1y, f32 d2x, f32 d2y,
										f32 *dfx, f32 *dfy);
extern sb32 mpCollisionCheckFloorLineCollisionSame(Vec3f *position, Vec3f *translate, Vec3f *ga_last, s32 *stand_line_id,
											  u32 *stand_coll_flags, Vec3f *angle);
extern sb32 mpCollisionCheckFloorLineCollisionDiff(Vec3f *position, Vec3f *translate, Vec3f *ga_last, s32 *stand_line_id,
											  u32 *stand_coll_flags, Vec3f *angle);

extern sb32 mpCollisionCheckFCSurfaceFlat(s32 v1x, s32 v1y, s32 vpos_y, f32 vpdist_x, f32 vpdist_y, f32 vtdist_x, f32 vtdist_y, f32 *arg7, f32 *arg8);

extern sb32 mpCollisionCheckCeilSurfaceTilt(s32 v1x, s32 v1y, s32 v2x, s32 v2y, f32 d1x, f32 d1y, f32 d2x, f32 d2y, f32 *dfx,
									  f32 *dfy);
extern sb32 mpCollisionCheckCeilLineCollisionSame(Vec3f *position, Vec3f *translate, Vec3f *ga_last, s32 *stand_line_id,
											u32 *stand_coll_flags, Vec3f *angle);
extern sb32 mpCollisionCheckCeilLineCollisionDiff(Vec3f *position, Vec3f *translate, Vec3f *ga_last, s32 *stand_line_id,
											u32 *stand_coll_flags, Vec3f *angle);
extern sb32 mpCollisionCheckRWallLineCollisionSame(Vec3f *position, Vec3f *translate, Vec3f *ga_last, s32 *stand_line_id,
											 u32 *stand_coll_flags, Vec3f *angle);
extern void mpCollisionGetLRAngle(Vec3f *angle, s32 v1x, s32 v1y, s32 v2x, s32 v2y, s32 lr);
extern sb32 mpCollisionCheckRWallSurfaceTilt(s32 v1x, s32 v1y, s32 v2x, s32 v2y, f32 d1x, f32 d1y, f32 d2x, f32 d2y, f32 *dfx,
									   f32 *dfy);
extern sb32 mpCollisionCheckRWallLineCollisionDiff(Vec3f *position, Vec3f *translate, Vec3f *ga_last, s32 *stand_line_id,
											 u32 *stand_coll_flags, Vec3f *angle);
extern sb32 mpCollisionCheckLRSurfaceFlat(s32 v1x, s32 v1y, s32 vpos_x, f32 vpdist_x, f32 vpdist_y, f32 vtdist_x,
									f32 vtdist_y, f32 *arg7, f32 *arg8);
extern sb32 mpCollisionCheckLWallLineCollisionSame(Vec3f *position, Vec3f *translate, Vec3f *ga_last, s32 *stand_line_id,
											 u32 *stand_coll_flags, Vec3f *angle);
extern sb32 mpCollisionCheckLWallSurfaceTilt(s32 v1x, s32 v1y, s32 v2x, s32 v2y, f32 d1x, f32 d1y, f32 d2x, f32 d2y, f32 *dfx,
									   f32 *dfy);
extern sb32 mpCollisionCheckLWallLineCollisionDiff(Vec3f *position, Vec3f *translate, Vec3f *ga_last, s32 *stand_line_id,
											 u32 *stand_coll_flags, Vec3f *angle);
extern sb32 func_ovl2_800F8FFC(Vec3f *position);
extern sb32 mpCollisionCheckProjectFloor(Vec3f *position, s32 *project_line_id, f32 *ga_dist, u32 *stand_coll_flags, Vec3f *angle);
extern sb32 mpCollisionCheckProjectCeil(Vec3f *position, s32 *project_line_id, f32 *ga_dist, u32 *stand_coll_flags, Vec3f *angle);
extern sb32 mpCollisionCheckProjectRWall(Vec3f *position, s32 *project_line_id, f32 *ga_dist, u32 *stand_coll_flags, Vec3f *angle);
extern sb32 mpCollisionCheckProjectLWall(Vec3f *position, s32 *project_line_id, f32 *ga_dist, u32 *stand_coll_flags, Vec3f *angle);
extern s32 mpCollisionGetVertexCountLineID(s32 line_id);
extern void mpCollisionGetVertexPositionID(s32 line_id, s32 vertex_id, Vec3f *pos);
extern void mpCollisionGetSpeedLineID(s32 line_id, Vec3f *speed);
extern s32 mpCollisionGetLineTypeID(s32 line_id);
extern s32 mpCollisionGetEdgeUnderRLineID(s32 line_id);
extern s32 mpCollisionGetEdgeUnderLLineID(s32 line_id);
extern s32 mpCollisionGetEdgeUpperRLineID(s32 line_id);
extern s32 mpCollisionGetEdgeUpperLLineID(s32 line_id);
extern s32 mpCollisionGetEdgeRightULineID(s32 line_id);
extern s32 mpCollisionGetEdgeRightDLineID(s32 line_id);
extern s32 mpCollisionGetEdgeLeftULineID(s32 line_id);
extern s32 mpCollisionGetEdgeLeftDLineID(s32 line_id);
extern void mpCollisionGetPlayerMapObjPosition(s32 player, Vec3f *pos);
extern void mpCollisionAllocVertexInfo();
extern void func_ovl2_800FB04C();
extern void mpCollisionInitLineTypesAll();
extern void func_ovl2_800FB31C();
extern void func_ovl2_800FB554();
extern void mpCollisionAllocYakumono(DObjDesc *dobjdesc);
extern void mpCollisionUpdateBoundsCurrent();
extern void mpCollisionUpdateBoundsDiff();
extern void mpCollisionPlayYakumonoAnim(GObj *ground_gobj);
extern void mpCollisionAdvanceUpdateTic(GObj *ground_gobj);
extern void mpCollisionInitYakumonoAll();
extern s32 mpCollisionAllocLinesGetCountTotal();
extern void mpCollisionInitLineIDsAll();
extern void mpCollisionInitGroundData();
extern void mpCollisionSetPlayBGM();
extern void mpCollisionSetBGM();
extern void mpCollisionClearYakumonoAll();
extern void mpCollisionSetYakumonoPosID(s32 line_id, Vec3f *yakumono_pos);
extern void mpCollisionSetYakumonoOnID(s32 line_id);
extern void mpCollisionSetYakumonoOffID(s32 line_id);
extern sb32 mpCollisionCheckExistLineID(s32 line_id);
extern s32 mpCollisionSetDObjNoID(s32 line_id);
extern s32 mpCollisionGetMapObjCountKind(s32 mapobj_kind);
extern void mpCollisionGetMapObjIDsKind(s32 mapobj_kind, s32 *mapobjs);
extern void mpCollisionGetMapObjPositionID(s32 mapobj_kind, Vec3f *pos);
extern s32 mpCollisionGetLineCountType(s32 line_type);
extern void mpCollisionGetLineIDsTypeCount(s32 type, s32 count, s32 *line_ids);
extern u8 mpCollisionSetLightColorGetAlpha(Gfx **dls);
extern sb32 mpCollisionCheckExistPlatformLineID(s32 line_id);
extern u16 mpCollisionGetVertexFlagsLineID(s32 line_id);

#endif /* _MPCOLLISION_H_ */
