#ifndef _MPDEF_H_
#define _MPDEF_H_

#define MAP_FLAG_LWALL (1 << 0)		    // 0x1
#define MAP_FLAG_RWALL (1 << 5)		    // 0x20
#define MAP_FLAG_CEIL (1 << 10)		    // 0x400
#define MAP_FLAG_FLOOR (1 << 11)        // 0x800
#define MAP_FLAG_LCLIFF (1 << 12)	    // 0x1000
#define MAP_FLAG_RCLIFF (1 << 13)	    // 0x2000
#define MAP_FLAG_CEILHEAVY (1 << 14)    // 0x4000 - hard ceiling, can't pass through
#define MAP_FLAG_FLOOREDGE (1 << 15)    // 0x8000 - edge of floor collision

#define MAP_FLAG_MAIN_MASK (MAP_FLAG_FLOOR | MAP_FLAG_CEIL | MAP_FLAG_RWALL | MAP_FLAG_LWALL)   // Mask every main collision flag
#define MAP_FLAG_CLIFF_MASK (MAP_FLAG_LCLIFF | MAP_FLAG_RCLIFF)			                        // Mask all ledge flags

#define MAP_VERTEX_ATTR_BITS 8

#define MAP_VERTEX_COLL_PASS (1 << 14)   // Line ID can be passed through
#define MAP_VERTEX_COLL_CLIFF (1 << 15)  // Line ID can be hung onto

#define MAP_VERTEX_COLL_BITS (0xFF << (MAP_VERTEX_ATTR_BITS * 1))
#define MAP_VERTEX_MAT_BITS (0xFF << (MAP_VERTEX_ATTR_BITS * 0))

#define MAP_VERTEX_COLL_MASK (~MAP_VERTEX_MAT_BITS)  // This is what the game covers when checking flags & 0xFFFF00FF
#define MAP_VERTEX_MAT_MASK (~MAP_VERTEX_COLL_BITS)  // This is what the game looks for when checking 0xFFFF00FF

#define MAP_VERTEX_ALL_BITS (MAP_VERTEX_COLL_BITS | MAP_VERTEX_MAT_BITS)

#define MAP_PROC_TYPE_DEFAULT (0)
#define MAP_PROC_TYPE_CLIFF (1 << 0)
#define MAP_PROC_TYPE_CLIFFEDGE (1 << 0)
#define MAP_PROC_TYPE_PROJECT (1 << 1)
#define MAP_PROC_TYPE_STOPEDGE (1 << 1)
#define MAP_PROC_TYPE_PASS (1 << 2)
#define MAP_PROC_TYPE_CEILHEAVY (1 << 3)

typedef enum MPKinetics
{
	nMPKineticsGround,
	nMPKineticsAir,
    nMPKineticsEnumCount

} MPKinetics;

typedef enum MPMaterial
{
	nMPMaterialCommon,      // Common material, used everywhere
	nMPMaterial1,           // ???
	nMPMaterial2,           // ???
	nMPMaterial3,           // ??? (Presumably ice due to low traction?)
	nMPMaterial4,           // ???
	nMPMaterial5,           // ???
	nMPMaterial6,           // ???
	nMPMaterialFireWeakS1,  // Weak horizontal fire knockback 1
	nMPMaterialFireStrongS1,// Strong horizontal fire knockback 1
	nMPMaterialFireWeakHi1, // Weak vertical fire knockback 1
	nMPMaterialSpikes,      // Bed of spikes (e.g. Pikachu Board the Platforms)
	nMPMaterialFireWeakHi2, // Weak vertical fire knockback 2
	nMPMaterialDokanL,      // Left pipe (Mushroom Kingdom)
	nMPMaterialDokanR,      // Right pipe (Mushroom Kingdom)
	nMPMaterialDetect,      // Board the Platforms / Race to the Finish gate / Saffron City gate
	nMPMaterialFireWeakHi3, // Weak vertical fire knockback 3
	nMPMaterialEnumCount

} MPMaterial;

typedef enum MPLineType
{
	nMPLineKindFloor,
	nMPLineKindCeil,
	nMPLineKindRWall,
	nMPLineKindLWall,
	nMPLineKindEnumCount

} MPLineType;

typedef enum MPMapObjKind
{
    nMPMapObjKindBattlePlayerStart,
    nMPMapObjKindBattlePlayer1 = nMPMapObjKindBattlePlayerStart,                    // 0x0
    nMPMapObjKindBattlePlayer2,                                                     // 0x1
    nMPMapObjKindBattlePlayer3,                                                     // 0x2
    nMPMapObjKindBattlePlayer4,                                                     // 0x3
    nMPMapObjKindBattlePlayerEnd = nMPMapObjKindBattlePlayer4,
    nMPMapObjKindItem,                                                              // 0x4
    nMPMapObjKindScaleL,                                                            // 0x5
    nMPMapObjKindScaleR,                                                            // 0x6
    nMPMapObjKindPakkunL,                                                           // 0x7
    nMPMapObjKindPakkunR,                                                           // 0x8
    nMPMapObjKindPowerBlock,                                                        // 0x9
    nMPMapObjKindDokanL,                                                            // 0xA
    nMPMapObjKindDokanR,                                                            // 0xB
    nMPMapObjKindAcid,                                                              // 0xC
    nMPMapObjKindTwister,                                                           // 0xD
    nMPMapObjKindMonster,                                                           // 0xE
    nMPMapObjKindMonsterUnused1,                                                    // 0xF
    nMPMapObjKindMonsterUnused2,                                                    // 0x10
    nMPMapObjKindMonsterUnused3,                                                    // 0x11
    nMPMapObjKindMonsterUnused4,                                                    // 0x12
    nMPMapObjKindBumper,                                                            // 0x13
    nMPMapObjKindDokanWall,                                                         // 0x14

    nMPMapObjKindMoviePlayer1,                                                      // 0x15
    nMPMapObjKindMoviePlayer2,                                                      // 0x16
    nMPMapObjKindMoviePlayer3,                                                      // 0x17

    nMPMapObjKindAutoDemoPlayer1,                                                   // 0x18
    nMPMapObjKindAutoDemoPlayer2,                                                   // 0x19
    nMPMapObjKindAutoDemoPlayer3,                                                   // 0x1A
    nMPMapObjKindAutoDemoPlayer4,                                                   // 0x1B
    nMPMapObjKindAutoDemoPlayer5,                                                   // 0x1C
    nMPMapObjKindAutoDemoPlayer6,                                                   // 0x1D
    nMPMapObjKindAutoDemoPlayer7,                                                   // 0x1E
    nMPMapObjKindAutoDemoPlayer8,                                                   // 0x1F
    nMPMapObjKindRebirth,                                                           // 0x20
    nMPMapObjKind1PGamePlayer,                                                      // 0x21

    nMPMapObjKind1PGameAllyStart,                                                   // 0x22
    nMPMapObjKind1PGameAlly1 = nMPMapObjKind1PGameAllyStart,                        // 0x22
    nMPMapObjKind1PGameAlly2,                                                       // 0x23
    nMPMapObjKind1PGameAlly3,                                                       // 0x24
    nMPMapObjKind1PGameAllyEnd = nMPMapObjKind1PGameAlly3,                          // 0x24

    nMPMapObjKind1PGameEnemyStart,                                                  // 0x25
    nMPMapObjKind1PGameEnemy1 = nMPMapObjKind1PGameEnemyStart,                      // 0x25
    nMPMapObjKind1PGameEnemy2,                                                      // 0x26
    nMPMapObjKind1PGameEnemy3,                                                      // 0x27
    nMPMapObjKind1PGameEnemy4,                                                      // 0x28
    nMPMapObjKind1PGameEnemyEnd = nMPMapObjKind1PGameEnemy4,                        // 0x28

    nMPMapObjKind1PGameBonus3TaruBomb,                                              // 0x29
    nMPMapObjKind1PGameUnknown,                                                     // 0x2A
    nMPMapObjKind1PGameEnemyTeam,                                                   // 0x2B
    nMPMapObjKind1PGameChallengerPlayer,                                            // 0x2C

    nMPMapObjKind1PGameChallengerEnemyStart,                                        // 0x2D
    nMPMapObjKind1PGameChallengerEnemy1 = nMPMapObjKind1PGameChallengerEnemyStart,  // 0x2D
    nMPMapObjKind1PGameChallengerEnemy2,                                            // 0x2E
    nMPMapObjKind1PGameChallengerEnemy3,                                            // 0x2F
    nMPMapObjKind1PGameChallengerEnemyEnd = nMPMapObjKind1PGameChallengerEnemy3     // 0x2F
    
} MPMapObjKind;

typedef enum MPYakumonoStatus   // States of line collisions + stage elements
{
	nMPYakumonoStatusNone,      // Nada
	nMPYakumonoStatusOn,        // Turned on
	nMPYakumonoStatusShow,      // Previously off, recently turned on
	nMPYakumonoStatusOff,       // Turned off
    nMPYakumonoStatusHidden     // Previously on, recently turned off

} MPYakumonoStatus;

typedef struct MPCollData MPCollData;
typedef struct MPVertexInfo MPVertexInfo;
typedef struct MPVertexInfoContainer MPVertexInfoContainer;
typedef struct MPVertexLinks MPVertexLinks;
typedef struct MPVertexLinksContainer MPVertexLinksContainer;
typedef struct MPVertexArray MPVertexArray;
typedef struct MPVertexData MPVertexData;
typedef struct MPVertexPosContainer MPVertexPosContainer;
typedef struct MPLineData MPLineData;
typedef struct MPLineInfo MPLineInfo;
typedef struct MPLineGroup MPLineGroup;
typedef struct MPBounds MPBounds;
typedef struct MPAllBounds MPAllBounds;
typedef struct MPGeometryData MPGeometryData;
typedef struct MPYakumonoDObj MPYakumonoDObj;
typedef struct MPMapObjData MPMapObjData;
typedef struct MPMapObjContainer MPMapObjContainer;
typedef struct MPObjectColl MPObjectColl;
typedef struct MPItemWeights MPItemWeights;
typedef struct MPGroundDesc MPGroundDesc;
typedef struct MPGroundData MPGroundData;

#endif
