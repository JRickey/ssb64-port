#ifndef _MPOBJPROC_H_
#define _MPOBJPROC_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <ft/ftdef.h>
#include <mp/mpdef.h>

void mpProcessResetMultiWallCount();
void mpProcessSetMultiWallLineID(s32 line_id);
void mpProcessSetLastWallCollideLeft();
void mpProcessSetLastWallCollideRight();
void mpProcessSetLastWallCollideStats(f32 pos, s32 line_id, u32 flags, Vec3f* angle);
void mpProcessGetLastWallCollideStats(f32* pos, s32* line_id, u32 *flags, Vec3f* angle);
sb32 mpProcessCheckCeilEdgeCollisionL(MPCollData* coll_data);
void mpProcessCeilEdgeAdjustLeft(MPCollData* coll_data);
sb32 mpProcessCheckCeilEdgeCollisionR(MPCollData* coll_data);
void mpProcessCeilEdgeAdjustRight(MPCollData* coll_data);
void mpProcessRunCeilEdgeAdjust(MPCollData* coll_data);
sb32 mpProcessCheckFloorEdgeCollisionL(MPCollData* coll_data);
void mpProcessFloorEdgeLAdjust(MPCollData* coll_data);
sb32 mpProcessCheckFloorEdgeCollisionR(MPCollData* coll_data);
void mpProcessFloorEdgeRAdjust(MPCollData* coll_data);
void mpProcessRunFloorEdgeAdjust(MPCollData* coll_data);
void mpProcessSetCollProjectFloorID(MPCollData* coll_data);
sb32 mpProcessUpdateMain(MPCollData* coll_data, sb32 (*proc_coll)(MPCollData*, GObj*, u32), GObj* gobj,
									u32 flags);
sb32 mpProcessCheckTestLWallCollision(MPCollData* coll_data);
void mpProcessRunLWallCollision(MPCollData* coll_data);
sb32 mpProcessCheckTestRWallCollision(MPCollData* coll_data);
void mpProcessRunRWallCollision(MPCollData* coll_data);
sb32 mpProcessCheckTestFloorCollisionNew(MPCollData* coll_data);
sb32 mpProcessCheckTestFloorCollision(MPCollData* coll_data, s32 line_id);
sb32 mpProcessCheckTestLCliffCollision(MPCollData* coll_data);
sb32 mpProcessCheckTestRCliffCollision(MPCollData* coll_data);
sb32 mpProcessCheckTestLWallCollisionAdjNew(MPCollData* coll_data);
void mpProcessRunLWallCollisionAdjNew(MPCollData* coll_data);
sb32 mpProcessCheckTestRWallCollisionAdjNew(MPCollData* coll_data);
void mpProcessRunRWallCollisionAdjNew(MPCollData* coll_data);
sb32 mpProcessCheckTestCeilCollisionAdjNew(MPCollData* coll_data);
void mpProcessRunCeilCollisionAdjNew(MPCollData* coll_data);
sb32 mpProcessCheckTestFloorCollisionAdjNew(MPCollData* coll_data, sb32 (*proc_map)(GObj*), GObj* gobj);
sb32 mpProcessRunFloorCollisionAdjNewNULL(MPCollData* coll_data);
void mpProcessSetLandingFloor(MPCollData* coll_data);
void mpProcessSetCollideFloor(MPCollData* coll_data);

#endif
