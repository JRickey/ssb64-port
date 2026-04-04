#ifndef _FIGHTER_H_
#define _FIGHTER_H_

#include "fttypes.h"
#include "ftfunctions.h"
#include "ftcommondata.h"

extern sb32 gFTMainIsDamageDetect[4];
extern sb32 gFTMainIsAttackDetect[4];

extern Gfx dFTDisplayMainHurtCollisionCuboidDL[/* */];
extern Gfx dFTDisplayMainHitCollisionEdgeDL[/* */];
extern Gfx dFTDisplayMainHitCollisionBlendDL[/* */];
extern Gfx dFTDisplayMainHitCollisionCubeDL[/* */];
extern Gfx dFTDisplayMainMapCollisionBottomDL[/* */];
extern Gfx dFTDisplayMainMapCollisionTopDL[/* */];

extern FTOpeningDesc *D_ovl1_80390D20[/* */];
extern FTOpeningDesc D_ovl1_80390BE8;

#define ftGetStruct(fighter_gobj) 	((FTStruct*)(fighter_gobj)->user_data.p)
#define ftGetParts(fighter_dobj) 	((FTParts *)(fighter_dobj)->user_data.p)

#define ftSetupDropItem(fp)                          	\
{                                                     	\
	Vec3f vel;                                        	\
	vel.x = vel.y = vel.z = 0.0F;                       \
	itMainSetFighterDrop((fp)->item_gobj, &vel, 1.0F);	\
}

#define ftKeyEventCast(script, type) ((type*)(script))

#define ftKeyGetButtons(script) (*(ftKeyEventCast((script), u16)))

#define ftKeyGetStickRange(script) (ftKeyEventCast((script), Vec2b))

#define ftGetComTargetFighter(com) \
((FTStruct*)(com)->target_user)

#define ftGetComTargetWeapon(com) \
((WPStruct*)(com)->target_user)

#define ftGetComTargetItem(com) \
((ITStruct*)(com)->target_user)

// Macro to check if a move has been interrupted by any standard action
#define ftCommonGroundCheckInterrupt(fighter_gobj)                    	\
(																		\
	(ftCommonSpecialNCheckInterruptCommon(fighter_gobj) != FALSE)  	|| 	\
	(ftCommonSpecialHiCheckInterruptCommon(fighter_gobj) != FALSE) 	||  \
	(ftCommonSpecialLwCheckInterruptCommon(fighter_gobj) != FALSE) 	||	\
	(ftCommonCatchCheckInterruptCommon(fighter_gobj) != FALSE) 		||  \
	(ftCommonAttackS4CheckInterruptCommon(fighter_gobj) != FALSE) 	|| 	\
	(ftCommonAttackHi4CheckInterruptCommon(fighter_gobj) != FALSE) 	|| 	\
	(ftCommonAttackLw4CheckInterruptCommon(fighter_gobj) != FALSE)	|| 	\
	(ftCommonAttackS3CheckInterruptCommon(fighter_gobj) != FALSE) 	||  \
	(ftCommonAttackHi3CheckInterruptCommon(fighter_gobj) != FALSE) 	||  \
	(ftCommonAttackLw3CheckInterruptCommon(fighter_gobj) != FALSE) 	||  \
	(ftCommonAttack1CheckInterruptCommon(fighter_gobj) != FALSE) 	|| 	\
	(ftCommonGuardOnCheckInterruptCommon(fighter_gobj) != FALSE) 	|| 	\
	(ftCommonAppealCheckInterruptCommon(fighter_gobj) != FALSE) 	|| 	\
	(ftCommonKneeBendCheckInterruptCommon(fighter_gobj) != FALSE) 	|| 	\
	(ftCommonDashCheckInterruptCommon(fighter_gobj) != FALSE) 		|| 	\
	(ftCommonPassCheckInterruptCommon(fighter_gobj) != FALSE) 		|| 	\
	(ftCommonDokanStartCheckInterruptCommon(fighter_gobj) != FALSE) || 	\
	(ftCommonSquatCheckInterruptCommon(fighter_gobj) != FALSE) 		|| 	\
	(ftCommonTurnCheckInterruptCommon(fighter_gobj) != FALSE) 		|| 	\
	(ftCommonWalkCheckInterruptCommon(fighter_gobj) != FALSE) 			\
)

#endif
