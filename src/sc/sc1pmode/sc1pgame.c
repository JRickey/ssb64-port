#include <ft/fighter.h>
#include <gr/ground.h>
#include <if/interface.h>
#include <sc/scene.h>
#include <sys/video.h>
#include <sys/dma.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       EXTERNAL VARIABLES      //
//                               //
// // // // // // // // // // // //

extern alSoundEffect D_8009EDD0_406D0;

extern intptr_t D_NF_00006010;
extern intptr_t D_NF_00006450;

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x80192800
u8 dSC1PGameKirbyTeamCopyKinds[/* */] = 
{ 
    nFTKindMario,
    nFTKindDonkey,
    nFTKindLink,
    nFTKindSamus,
    nFTKindYoshi,
    nFTKindFox,
    nFTKindPikachu
};

// 0x80192808
f32 dSC1PGameZoomEyeX[/* */] = { -15.0F, 0.0F, 15.0F, 30.0F, 45.0F, 60.0F };

// 0x80192820
f32 dSC1PGameZoomEyeY[/* */] = { 1.0F, 2.0F, -8.0F, -30.0F };

// 0x80192830
SC1PGameComputer dSC1PGameComputerDesc[/* */] = 
{
    // VS Link
    {
        TRUE,                       // Is team attack enabled?
        nSCBattleItemSwitchLow,     // Item Switch
#if defined(REGION_US)
        {  1,  2,  3,  6,  8 },     // Enemy CPU levels per difficulty setting
#else
        {  1,  1,  3,  6,  8 },     // Enemy CPU levels per difficulty setting
#endif
        {  9,  9,  9,  9,  9 },     // Enemy handicaps per difficulty setting
        {  1,  1,  1,  1,  1 },     // Ally CPU levels per difficulty setting
        {  9,  9,  9,  9,  9 }      // Ally handicaps per difficulty setting
    },

    // VS Yoshi Team
    {
        TRUE,                       // Is team attack enabled?
        nSCBattleItemSwitchMiddle,  // Item Switch
#if defined(REGION_US)
        {  1,  2,  4,  6,  8 },     // Enemy CPU levels per difficulty setting
#else
        {  1,  2,  3,  5,  6 },     // Enemy CPU levels per difficulty setting
#endif
        { 10, 11, 12, 13, 14 },     // Enemy handicaps per difficulty setting
        {  1,  1,  1,  1,  1 },     // Ally CPU levels per difficulty setting
        {  9,  9,  9,  9,  9 }      // Ally handicaps per difficulty setting
    },

    // VS Fox
    {
        TRUE,                       // Is team attack enabled?
        nSCBattleItemSwitchMiddle,  // Item Switch
#if defined(REGION_US)
        {  2,  3,  5,  7,  9 },     // Enemy CPU levels per difficulty setting
#else
        {  2,  3,  4,  6,  8 },     // Enemy CPU levels per difficulty setting
#endif
        {  9,  9,  9,  9,  9 },     // Enemy handicaps per difficulty setting
        {  1,  1,  1,  1,  1 },     // Ally CPU levels per difficulty setting
        {  9,  9,  9,  9,  9 }      // Ally handicaps per difficulty setting
    },

    // Bonus Stage: Break the Targets!
    {
        TRUE,                       // Is team attack enabled?
        nSCBattleItemSwitchNone,    // Item Switch
        {  1,  1,  1,  1,  1 },     // Enemy CPU levels per difficulty setting
        {  9,  9,  9,  9,  9 },     // Enemy handicaps per difficulty setting
        {  1,  1,  1,  1,  1 },     // Ally CPU levels per difficulty setting
        {  9,  9,  9,  9,  9 }      // Ally handicaps per difficulty setting
    },

    // VS Mario Bros.
    {
        FALSE,                      // Is team attack enabled?
        nSCBattleItemSwitchMiddle,  // Item Switch
#if defined(REGION_US)
        {  2,  3,  5,  7,  9 },     // Enemy CPU levels per difficulty setting
#else
        {  2,  3,  5,  6,  7 },     // Enemy CPU levels per difficulty setting
#endif
        {  9,  9,  9,  9,  9 },     // Enemy handicaps per difficulty setting
#if defined(REGION_US)
        {  5,  5,  5,  4,  2 },     // Ally CPU levels per difficulty setting
#else
        {  6,  5,  4,  3,  2 },     // Ally CPU levels per difficulty setting
#endif
        {  9,  9,  9,  9,  9 }      // Ally handicaps per difficulty setting
    },

    // VS Pikachu
    {
        TRUE,                       // Is team attack enabled?
        nSCBattleItemSwitchMiddle,  // Item Switch
        {  3,  4,  5,  7,  9 },     // Enemy CPU levels per difficulty setting
        {  9,  9,  9,  9,  9 },     // Enemy handicaps per difficulty setting
        {  1,  1,  1,  1,  1 },     // Ally CPU levels per difficulty setting
        {  9,  9,  9,  9,  9 }      // Ally handicaps per difficulty setting
    },

    // VS Giant Donkey Kong
    {
        FALSE,                      // Is team attack enabled?
        nSCBattleItemSwitchHigh,    // Item Switch
#if defined(REGION_US)
        {  2,  4,  6,  7,  8 },     // Enemy CPU levels per difficulty setting
#else
        {  2,  3,  4,  6,  7 },     // Enemy CPU levels per difficulty setting
#endif
        { 25, 26, 27, 28, 29 },     // Enemy handicaps per difficulty setting
#if defined(REGION_US)
        {  4,  4,  4,  3,  2 },     // Ally CPU levels per difficulty setting
        {  7,  7,  7,  7,  7 }      // Ally handicaps per difficulty setting
#else
        {  4,  3,  2,  1,  1 },     // Ally CPU levels per difficulty setting
        {  8,  7,  7,  7,  7 }      // Ally handicaps per difficulty setting
#endif
    },

    // Bonus Stage: Board the Platforms!
    {
        TRUE,                       // Is team attack enabled?
        nSCBattleItemSwitchNone,    // Item Switch
        {  1,  1,  1,  1,  1 },     // Enemy CPU levels per difficulty setting
        {  9,  9,  9,  9,  9 },     // Enemy handicaps per difficulty setting
        {  1,  1,  1,  1,  1 },     // Ally CPU levels per difficulty setting
        {  9,  9,  9,  9,  9 }      // Ally handicaps per difficulty setting
    },

    // VS Kirby Team
    {
        TRUE,                       // Is team attack enabled?
        nSCBattleItemSwitchMiddle,  // Item Switch
#if defined(REGION_US)
        {  3,  4,  5,  6,  7 },     // Enemy CPU levels per difficulty setting
#else
        {  2,  3,  4,  5,  7 },     // Enemy CPU levels per difficulty setting
#endif
        { 15, 16, 17, 18, 19 },     // Enemy handicaps per difficulty setting
        {  1,  1,  1,  1,  1 },     // Ally CPU levels per difficulty setting
        {  9,  9,  9,  9,  9 }      // Ally handicaps per difficulty setting
    },

    // VS Samus
    {
        TRUE,                       // Is team attack enabled?
        nSCBattleItemSwitchMiddle,  // Item Switch
#if defined(REGION_US)
        {  5,  6,  8,  9,  9 },     // Enemy CPU levels per difficulty setting
        {  9,  9,  9,  9, 40 },     // Enemy handicaps per difficulty setting
#else
        {  4,  5,  7,  8,  9 },     // Enemy CPU levels per difficulty setting
        {  9,  9,  9,  9,  9 },     // Enemy handicaps per difficulty setting
#endif
        {  1,  1,  1,  1,  1 },     // Ally CPU levels per difficulty setting
        {  9,  9,  9,  9,  9 }      // Ally handicaps per difficulty setting
    },

    // VS Metal Mario
    {
        TRUE,                       // Is team attack enabled?
        nSCBattleItemSwitchVeryLow, // Item Switch
#if defined(REGION_US)
        {  1,  3,  4,  6,  8 },     // Enemy CPU levels per difficulty setting
#else
        {  1,  2,  3,  4,  6 },     // Enemy CPU levels per difficulty setting
#endif
        { 30, 31, 32, 33, 34 },     // Enemy handicaps per difficulty setting
        {  1,  1,  1,  1,  1 },     // Ally CPU levels per difficulty setting
        {  9,  9,  9,  9,  9 }      // Ally handicaps per difficulty setting
    },

    // Bonus Stage: Race to the Finish!
    {
        TRUE,                       // Is team attack enabled?
        nSCBattleItemSwitchNone,    // Item Switch
#if defined(REGION_US)
        {  6,  8,  9,  9,  9 },     // Enemy CPU levels per difficulty setting
#else
        {  6,  8,  8,  9,  9 },     // Enemy CPU levels per difficulty setting
#endif
        {  1,  3,  5,  7,  9 },     // Enemy handicaps per difficulty setting
        {  1,  1,  1,  1,  1 },     // Ally CPU levels per difficulty setting
        {  9,  9,  9,  9,  9 }      // Ally handicaps per difficulty setting
    },

    // VS Fighting Polygon Team
    {
        TRUE,                       // Is team attack enabled?
        nSCBattleItemSwitchHigh,    // Item Switch
#if defined(REGION_US)
        {  2,  3,  4,  5,  7 },     // Enemy CPU levels per difficulty setting
#else
        {  2,  2,  3,  4,  5 },     // Enemy CPU levels per difficulty setting
#endif
        { 20, 21, 22, 23, 24 },     // Enemy handicaps per difficulty setting
        {  1,  1,  1,  1,  1 },     // Ally CPU levels per difficulty setting
        {  9,  9,  9,  9,  9 }      // Ally handicaps per difficulty setting
    },

    // Final Stage
    {
        TRUE,                       // Is team attack enabled?
        nSCBattleItemSwitchNone,    // Item Switch
        {  1,  2,  3,  4,  5 },     // Enemy CPU levels per difficulty setting
        { 35, 36, 37, 38, 39 },     // Enemy handicaps per difficulty setting
        {  1,  1,  1,  1,  1 },     // Ally CPU levels per difficulty setting
        {  9,  9,  9,  9,  9 }      // Ally handicaps per difficulty setting
    },

    // Challenger Approaching: Luigi
    {
        TRUE,                       // Is team attack enabled?
        nSCBattleItemSwitchNone,    // Item Switch
#if defined(REGION_US)
        {  6,  7,  7,  8,  9 },     // Enemy CPU levels per difficulty setting
#else
        {  6,  7,  7,  7,  8 },     // Enemy CPU levels per difficulty setting
#endif
        {  6,  6,  6,  6,  6 },     // Enemy handicaps per difficulty setting
        {  1,  1,  1,  1,  1 },     // Ally CPU levels per difficulty setting
        {  9,  9,  9,  9,  9 }      // Ally handicaps per difficulty setting
    },

    // Challenger Approaching: Ness
    {
        TRUE,                       // Is team attack enabled?
        nSCBattleItemSwitchNone,    // Item Switch
#if defined(REGION_US)
        {  6,  7,  7,  8,  9 },     // Enemy CPU levels per difficulty setting
#else
        {  6,  7,  7,  7,  8 },     // Enemy CPU levels per difficulty setting
#endif
        {  6,  6,  6,  6,  6 },     // Enemy handicaps per difficulty setting
        {  1,  1,  1,  1,  1 },     // Ally CPU levels per difficulty setting
        {  9,  9,  9,  9,  9 }      // Ally handicaps per difficulty setting
    },

    // Challenger Approaching: Jigglypuff
    {
        TRUE,                       // Is team attack enabled?
        nSCBattleItemSwitchNone,    // Item Switch
#if defined(REGION_US)
        {  6,  7,  7,  8,  9 },     // Enemy CPU levels per difficulty setting
#else
        {  6,  7,  7,  7,  8 },     // Enemy CPU levels per difficulty setting
#endif
        {  6,  6,  6,  6,  6 },     // Enemy handicaps per difficulty setting
        {  1,  1,  1,  1,  1 },     // Ally CPU levels per difficulty setting
        {  9,  9,  9,  9,  9 }      // Ally handicaps per difficulty setting
    },

    // Challenger Approaching: Captain Falcon
    {
        TRUE,                       // Is team attack enabled?
        nSCBattleItemSwitchNone,    // Item Switch
#if defined(REGION_US)
        {  6,  7,  7,  8,  9 },     // Enemy CPU levels per difficulty setting
#else
        {  6,  7,  7,  7,  8 },     // Enemy CPU levels per difficulty setting
#endif
        {  6,  6,  6,  6,  6 },     // Enemy handicaps per difficulty setting
        {  1,  1,  1,  1,  1 },     // Ally CPU levels per difficulty setting
        {  9,  9,  9,  9,  9 }      // Ally handicaps per difficulty setting
    }
};

// 0x801929BC
SC1PGameStage dSC1PGameStageDesc[/* */] =
{
    // VS Link
    {
        0xFF,                               // Screen flash alpha transparency
        nGRKindHyrule,                      // Stage ID
        0xFFFFFFFF,                         // Item toggles
        1,                                  // Number of enemies
        {                           
            nFTKindLink,                    // Enemy1 ID
            nFTKindNull                     // Enemy2 ID
        },
        nFTComputerTraitLink,               // Enemy trait
        0,                                  // Number of allies
        nFTComputerTraitDefault             // Ally trait
    },

    // VS Yoshi Team
    {
        0x80,                               // Screen flash alpha transparency
        nGRKindYosterSmall,                 // Stage ID
        0xFFFFFFFF,                         // Item toggles
        SC1PGAME_STAGE_YOSHI_TEAM_COUNT,    // Number of enemies
        {
            nFTKindYoshi,                   // Enemy1 ID
            nFTKindNull                     // Enemy2 ID
        },
        nFTComputerTraitYoshiTeam,          // Enemy trait
        0,                                  // Number of allies
        nFTComputerTraitDefault             // Ally trait
    },

    // VS Fox
    {
        0xFF,                               // Screen flash alpha transparency
        nGRKindSector,                      // Stage ID
        0xFFFFFFFF,                         // Item toggles
        1,                                  // Number of enemies
        {
            nFTKindFox,                     // Enemy Team Member 1 ID
            nFTKindNull                     // Enemy Team Member 2 ID
        },
        nFTComputerTraitDefault,            // Enemy trait
        0,                                  // Number of allies
        nFTComputerTraitDefault             // Ally trait
    },

    // Bonus Stage: Break the Targets!
    {
        0xFF,                               // Screen flash alpha transparency
        nGRKindCastle,                      // Stage ID
        0xFFFFFFFF,                         // Item toggles
        1,                                  // Number of enemies
        {
            nFTKindNull,                    // Enemy Team Member 1 ID
            nFTKindNull                     // Enemy Team Member 2 ID
        },
        nFTComputerTraitDefault,            // Enemy trait
        0,                                  // Number of allies
        nFTComputerTraitDefault             // Ally trait
    },

    // VS Mario Bros.
    {
        0xFF,                               // Screen flash alpha transparency
        nGRKindCastle,                      // Stage ID
        0xFFFFFFFF,                         // Item toggles
        2,                                  // Number of enemies
        {
            nFTKindMario,                   // Enemy Team Member 1 ID
            nFTKindLuigi                    // Enemy Team Member 2 ID
        },
        nFTComputerTraitMarioBros,          // Enemy trait
        1,                                  // Number of allies
        nFTComputerTraitAlly                // Ally trait
    },

    // VS Pikachu
    {
        0xFF,                               // Screen flash alpha transparency
        nGRKindYamabuki,                    // Stage ID
        0xFFFFFFFF,                         // Item toggles
        1,                                  // Number of enemies
        {
            nFTKindPikachu,                 // Enemy Team Member 1 ID
            nFTKindNull                     // Enemy Team Member 2 ID
        },
        nFTComputerTraitDefault,            // Enemy trait
        0,                                  // Number of allies
        nFTComputerTraitDefault             // Ally trait
    },

    // VS Giant Donkey Kong
    {
        0xFF,                               // Screen flash alpha transparency
        nGRKindJungle,                      // Stage ID
        0xFFFFFFFF,                         // Item toggles
        1,                                  // Number of enemies
        {
            nFTKindGDonkey,                 // Enemy Team Member 1 ID
            nFTKindNull                     // Enemy Team Member 2 ID
        },
        nFTComputerTraitGDonkey,            // Enemy trait
        2,                                  // Number of allies
        nFTComputerTraitAlly                // Ally trait
    },

    // Bonus Stage: Board the Platforms!
    {
        0xFF,                               // Screen flash alpha transparency
        nGRKindCastle,                      // Stage ID
        0xFFFFFFFF,                         // Item toggles
        1,                                  // Number of enemies
        {
            nFTKindNull,                    // Enemy Team Member 1 ID
            nFTKindNull                     // Enemy Team Member 2 ID
        },
        nFTComputerTraitDefault,            // Enemy trait
        0,                                  // Number of allies
        nFTComputerTraitDefault             // Ally trait
    },

    // VS Kirby Team
    {
        0x80,                               // Screen flash alpha transparency
        nGRKindPupupu,                      // Stage ID
        0xFFFFFFFF,                         // Item toggles
        SC1PGAME_STAGE_KIRBY_TEAM_COUNT,    // Number of enemies
        {
            nFTKindKirby,                   // Enemy Team Member 1 ID
            nFTKindNull                     // Enemy Team Member 2 ID
        },
        nFTComputerTraitKirbyTeam,          // Enemy trait
        0,                                  // Number of allies
        nFTComputerTraitDefault             // Ally trait
    },

    // VS Samus
    {
        0xFF,                               // Screen flash alpha transparency
        nGRKindZebes,                       // Stage ID
        0xFFFFFFFF,                         // Item toggles
        1,                                  // Number of enemies
        {
            nFTKindSamus,                   // Enemy Team Member 1 ID
            nFTKindNull                     // Enemy Team Member 2 ID
        },
        nFTComputerTraitDefault,            // Enemy trait
        0,                                  // Number of allies
        nFTComputerTraitDefault             // Ally trait
    },

    // VS Metal Mario
    {
        0xFF,                               // Screen flash alpha transparency
        nGRKindMetal,                       // Stage ID
        0xFFFFFFFF,                         // Item toggles
        1,                                  // Number of enemies
        {
            nFTKindMMario,                  // Enemy Team Member 1 ID
            nFTKindNull                     // Enemy Team Member 2 ID
        },
        nFTComputerTraitDefault,            // Enemy trait
        0,                                  // Number of allies
        nFTComputerTraitDefault             // Ally trait
    },

    // Bonus Stage: Race to the Finish!
    {
        0xFF,                               // Screen flash alpha transparency
        nGRKindBonus3,                      // Stage ID
        0xFFFFFFFF,                         // Item toggles
        3,                                  // Number of enemies
        {
            nFTKindNull,                    // Enemy Team Member 1 ID
            nFTKindNull                     // Enemy Team Member 2 ID
        },
        nFTComputerTraitBonus3,             // Enemy trait
        0,                                  // Number of allies
        nFTComputerTraitDefault             // Ally trait
    },

    // VS Fighting Polygon Team
    {
        0x80,                               // Screen flash alpha transparency
        nGRKindZako,                        // Stage ID
        0xFFFFFFFF,                         // Item toggles
        SC1PGAME_STAGE_MAX_TEAM_COUNT,      // Number of enemies
        {
            nFTKindNull,                    // Enemy Team Member 1 ID
            nFTKindNull                     // Enemy Team Member 2 ID
        },
        nFTComputerTraitPolyTeam,           // Enemy trait
        0,                                  // Number of allies
        nFTComputerTraitDefault             // Ally trait
    },

    // Final Stage
    {
        0xFF,                               // Screen flash alpha transparency
        nGRKindLast,                        // Stage ID
        0xFFFFFFFF,                         // Item toggles
        1,                                  // Number of enemies
        {
            nFTKindBoss,                    // Enemy Team Member 1 ID
            nFTKindNull                     // Enemy Team Member 2 ID
        },
        nFTComputerTraitDefault,            // Enemy trait
        0,                                  // Number of allies
        nFTComputerTraitDefault             // Ally trait
    },

    // Challenger Approaching: Luigi
    {
        0xFF,                               // Screen flash alpha transparency
        nGRKindCastle,                      // Stage ID
        0xFFFFFFFF,                         // Item toggles
        1,                                  // Number of enemies
        {
            nFTKindLuigi,                   // Enemy Team Member 1 ID
            nFTKindNull                     // Enemy Team Member 2 ID
        },
        nFTComputerTraitDefault,            // Enemy trait
        0,                                  // Number of allies
        nFTComputerTraitDefault             // Ally trait
    },

    // Challenger Approaching: Ness
    {
        0xFF,                               // Screen flash alpha transparency
        nGRKindPupupu,                      // Stage ID
        0xFFFFFFFF,                         // Item toggles
        1,                                  // Number of enemies
        {
            nFTKindNess,                    // Enemy Team Member 1 ID
            nFTKindNull                     // Enemy Team Member 2 ID
        },
        nFTComputerTraitDefault,            // Enemy trait
        0,                                  // Number of allies
        nFTComputerTraitDefault             // Ally trait
    },

    // Challenger Approaching: Jigglypuff
    {
        0xFF,                               // Screen flash alpha transparency
        nGRKindYamabuki,                    // Stage ID
        0xFFFFFFFF,                         // Item toggles
        1,                                  // Number of enemies
        {
            nFTKindPurin,                   // Enemy Team Member 1 ID
            nFTKindNull                     // Enemy Team Member 2 ID
        },
        nFTComputerTraitDefault,            // Enemy trait
        0,                                  // Number of allies
        nFTComputerTraitDefault             // Ally trait
    },

    // Challenger Approaching: Captain Falcon
    {
        0xFF,                               // Screen flash alpha transparency
        nGRKindZebes,                       // Stage ID
        0xFFFFFFFF,                         // Item toggles
        1,                                  // Number of enemies
        {
            nFTKindCaptain,                 // Enemy Team Member 1 ID
            nFTKindNull                     // Enemy Team Member 2 ID
        },
        nFTComputerTraitDefault,            // Enemy trait
        0,                                  // Number of allies
        nFTComputerTraitDefault             // Ally trait
    }
};

// 0x80192ADC
s32 dSC1PGameInterface4PlayersPositionsX[/* */] = {  55, 125, 195, 265 };

// 0x80192AEC
s32 dSC1PGameInterface3PlayersPositionsX[/* */] = {  90, 160, 230 };

// 0x80192AF8
s32 dSC1PGameInterface2PlayersPositionsX[/* */] = { 125, 195 };

// 0x80192B00 - Pointer to player HUD X-Position spacing setups for player counts of 2, 3 and 4
s32 *dSC1PGameInterfaceCountPositions[/* */] = 
{ 
    dSC1PGameInterface2PlayersPositionsX, 
    dSC1PGameInterface3PlayersPositionsX, 
    dSC1PGameInterface4PlayersPositionsX
};

// 0x80192B0C - Some kind of process ID list?
u16 dSC1PGameStageCommonSleepTics[/* */] = { 22, 15, 60 };

// 0x80192B14
SYColorRGBA dSC1PGameFadeColor = { 0x00, 0x00, 0x00, 0x00 };

// 0x80192B18
SYVideoSetup dSC1PGameVideoSetup = SYVIDEO_SETUP_DEFAULT();

// 0x80132B34
SYTaskmanSetup dSC1PGameTaskmanSetup =
{
    // Task Manager Buffer Setup
    {
        0,                          // ???
        sc1PGameFuncUpdate,         // Update function
        scManagerFuncDraw,          // Frame draw function
        &ovl65_BSS_END,             // Allocatable memory pool start
        0,                          // Allocatable memory pool size
        1,                          // ???
        2,                          // Number of contexts?
        sizeof(Gfx) * 7680,         // Display List Buffer 0 Size
        sizeof(Gfx) * 2560,         // Display List Buffer 1 Size
        0,                          // Display List Buffer 2 Size
        0,                          // Display List Buffer 3 Size
        0xD000,                     // Graphics Heap Size
        2,                          // ???
        0xC000,                     // RDP Output Buffer Size
        sc1PGameFuncLights,         // Pre-render function
        syControllerFuncRead,       // Controller I/O function
    },

    0,                              // Number of GObjThreads
    sizeof(u64) * 192,              // Thread stack size
    0,                              // Number of thread stacks
    0,                              // ???
    0,                              // Number of GObjProcesses
    0,                              // Number of GObjs
    sizeof(GObj),                   // GObj size
    0,                              // Number of XObjs
    dLBCommonFuncMatrixList,        // Matrix function list
    NULL,                           // DObjVec eject function
    0,                              // Number of AObjs
    0,                              // Number of MObjs
    0,                              // Number of DObjs
    sizeof(DObj),                   // DObj size
    0,                              // Number of SObjs
    sizeof(SObj),                   // SObj size
    0,                              // Number of CObjs
    sizeof(CObj),                 	// CObj size
    
    sc1PGameFuncStart               // Task start function
};

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x80192FA0 - Total number of enemies on current stage?
u8 sSC1PGameEnemyPlayerCount;

// 0x80192FA1 - Remaining stocks of enemy team
u8 sSC1PGameEnemyStocksRemaining;

// 0x80192FA4 - Flag to hide stock icons of defeated "VS *character* Team" members
u32 sSC1PGameEnemyStockSpriteFlags;

// 0x80192FA8 - "VS *character* Team" members defeated?
u8 sSC1PGameTeamPlayersRemaining;

// 0x80192FA9 - Current enemy variation?
u8 sSC1PGameCurrentEnemyVariation;

// 0x80192FAC
s32 sSC1PGamePad0x80192FAC;

// 0x80192FB0 - Number of unique enemy patterns in "VS *character* Team" battles (e.g. 6 randomly selected colors repeated 3 times for Yoshi Team)
u8 sSC1PGameEnemyVariations[SC1PGAME_STAGE_MAX_TEAM_COUNT];

// 0x80192FD0 - Camera target position when Master Hand is defeated
Vec3f sSC1PGameBossDefeatZoomPosition;

// 0x80192FDC
u16 sSC1PGameBossDefeatSoundTerminateTemp;

// 0x80192FE0
SC1PGameFighter sSC1PGamePlayerSetups[GMCOMMON_PLAYERS_MAX];

// 0x80193060 - Costume ID for all members of Kirby Team?
s32 sSC1PGameEnemyKirbyCostume;

// 0x80193064 - Base of Fighting Polygon Team stock sprite file?
void *sSC1PGameZakoStockFile;

// 0x80193068
LBFileNode sSC1PGameStatusBuffer[100];

// 0x80193388
LBFileNode sSC1PGameForceStatusBuffer[7];

// 0x801933C0
sb32 sSC1PGameIsEndStage;

// 0x801933C4 - Only works on Meta Crystal and Duel Zone?
sb32 sSC1PGameIsStartStage;

// 0x801933C8
s32 sSC1PGameBonusStatEndPlayerStatus;

// 0x801933CC
s32 sSC1PGameBonusStatInvincibleTimer;

// 0x801933D0 - Specific stats of all enemy players?
SC1PGameStats sSC1PGameBonusStatEnemyStats[SC1PGAME_STAGE_MAX_TEAM_COUNT];

// 0x801936A0 - Player's number of KOs scored on enemies
s32 sSC1PGameBonusStatPlayerKOsNum;

// 0x801936A4
sb32 gSC1PGameBonusBrosCalamity;

// 0x801936A8
u8 gSC1PGameBonusTomatoCount;

// 0x801936A9
u8 gSC1PGameBonusHeartCount;

#if defined(REGION_US)
// 0x801936AA
u8 gSC1PGameBonusStarCount;
#endif

// 0x801936AB
ub8 gSC1PGameBonusShieldBreaker;

// 0x801936AC
ub8 gSC1PGameBonusGiantImpact;

// 0x801936AD
ub8 gSC1PGameBonusMewCatcher;

// 0x801936B0
u32 gSC1PGameBonusAttackIDCount[nFTStatusAttackIDEnumCount];

// 0x80193798
u32 gSC1PGameBonusAttackIsSmashCount[2];

// 0x801937A0
u32 gSC1PGameBonusAttackGroundAirCount[2];

// 0x801937A8
u32 gSC1PGameBonusAttackIsProjectileCount[2];

// 0x801937B0
u32 gSC1PGameBonusDefendIDCount[nFTStatusAttackIDEnumCount];

// 0x80193898
u32 gSC1PGameBonusDefendIsSmashCount[2];

// 0x801938A0
u32 gSC1PGameBonusDefendGroundAirCount[2];

// 0x801938A8
u32 gSC1PGameBonusDefendIsProjectileCount[2];

// 0x801938B0
u32 sSC1PGameStageTimeSec;

// 0x801938B4
u32 sSC1PGameTotalTimeSec;

// 0x801938B8
s32 sSC1PGamePlayerInterfacePositionsX[GMCOMMON_PLAYERS_MAX];

// 0x801938C8 - Number of enemy stock icons to display?
u8 sSC1PGameEnemyStocksDisplay;

// 0x801938CC
FTSprites *sSC1PGameEnemyTeamSprites;

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8018D0C0
void sc1PGameSetupFiles(void)
{
    LBRelocSetup rl_setup;

    rl_setup.table_addr = (uintptr_t)&lLBRelocTableAddr;
    rl_setup.table_files_num = (u32)&llRelocFileCount;
    rl_setup.file_heap = NULL;
    rl_setup.file_heap_size = 0;
    rl_setup.status_buffer = sSC1PGameStatusBuffer;
    rl_setup.status_buffer_size = ARRAY_COUNT(sSC1PGameStatusBuffer);
    rl_setup.force_status_buffer = sSC1PGameForceStatusBuffer;
    rl_setup.force_status_buffer_size = ARRAY_COUNT(sSC1PGameForceStatusBuffer);

    lbRelocInitSetup(&rl_setup);
    lbRelocLoadFilesListed(dGMCommonFileIDs,  gGMCommonFiles);
}

// 0x8018D160
void sc1PGameSetGameStart(void)
{
    sSC1PGameIsStartStage = TRUE;

    if ((gSCManagerSceneData.spgame_stage == nSC1PGameStageMMario) || (gSCManagerSceneData.spgame_stage == nSC1PGameStageZako))
    {
        mpCollisionSetPlayBGM();
    }
}

// 0x8018D1A8
void sc1PGameSetGameEnd(void)
{
    GObj *fighter_gobj;
    FTStruct *fp;

    sSC1PGameIsEndStage = TRUE;

    fighter_gobj = gSCManagerBattleState->players[gSCManagerSceneData.player].fighter_gobj;
    fp = ftGetStruct(fighter_gobj);

    sSC1PGameBonusStatEndPlayerStatus = fp->status_id;
    sSC1PGameBonusStatInvincibleTimer = fp->star_invincible_tics;
}

// 0x8018D200
void sc1PGameFuncUpdate(void)
{
    ifCommonBattleUpdateInterfaceAll();

    if ((sSC1PGameIsStartStage == FALSE) && (gSCManagerBattleState->game_status == nSCBattleGameStatusGo))
    {
        sc1PGameSetGameStart();
    }
    if ((sSC1PGameIsEndStage == FALSE) && (gSCManagerBattleState->game_status == nSCBattleGameStatusSet))
    {
        sc1PGameSetGameEnd();
    }
}

// 0x8018D280
s32 sc1PGameGetNextFreePlayerPort(s32 player)
{
    return (player == (GMCOMMON_PLAYERS_MAX - 1)) ? 0 : player + 1;
}

// 0x8018D29C - Really weird match, gets next available costume for CPU player in 1P Game
s32 sc1PGameGetNextFreeCostume(s32 com)
{
    s32 player;
    s32 used_costume;
    s32 cp_costume;
    s32 cp_fkind;

    cp_fkind = gSCManagerBattleState->players[com].fkind;

    // WARNING: Newline memes. get_costume should be on the same line as used_costume = 0, player = 0;
    used_costume = 0, player = 0; get_costume:

    cp_costume = ftParamGetCostumeCommonID(cp_fkind, used_costume);

    while (player != ARRAY_COUNT(gSCManagerBattleState->players))
    {
        if (player == com) 
        {
            goto l_continue; // This goto is required
        }
        else if (gSCManagerBattleState->players[player].pkind == nFTPlayerKindNot) 
        {
            goto l_continue; // This one is not
        }
        else if
        (
            (gSCManagerBattleState->players[player].fkind == cp_fkind) && 
            (gSCManagerBattleState->players[player].costume == cp_costume)
        )
        {
            used_costume++, player = 0;

            goto get_costume;
        }
    l_continue:
        player++;
    }
    return cp_costume;
}

// 0x8018D38C - Empty function because of course
void func_ovl65_8018D38C(void)
{
    return;
}

// 0x8018D394 - Get random shuffled variable, used for Yoshi Team, Kirby Team (?), Race to the Finish and Fighting Polygon Team
s32 sc1PGameGetFighterKindsNum(u16 shuf, s32 max)
{
    s32 i = -1;

    max++;

    do
    {
        i++;

        if (!(shuf & (1 << i)))
        {
            max--;
        }
    } 
    while (max != 0);

    return i;
}

// 0x8018D3D0 - Yet ANOTHER empty function
void func_ovl65_8018D3D0(void)
{
    return;
}

// 0x8018D3D8
void sc1PGameGetStartPosition(Vec3f *mapobj_pos, s32 mapobj_kind)
{
    s32 mapobj;

    if (mpCollisionGetMapObjCountKind(mapobj_kind) != 1)
    {
        while (TRUE)
        {
            syDebugPrintf("getMapObjPos:mpGetMapObjNumId(%d) = %d\n", mapobj_kind, mpCollisionGetMapObjCountKind(mapobj_kind));
            scManagerRunPrintGObjStatus();
        }
    }
    mpCollisionGetMapObjIDsKind(mapobj_kind, &mapobj);
    mpCollisionGetMapObjPositionID(mapobj, mapobj_pos);
}

// 0x8018D45C
void sc1PGameGetRandomStartPosition(Vec3f *mapobj_pos, s32 mapobj_kind)
{
    s32 mapobj_count = mpCollisionGetMapObjCountKind(mapobj_kind);
    s32 mapobj[10];

    if ((mapobj_count == 0) || (mapobj_count > ARRAY_COUNT(mapobj)))
    {
        while (TRUE)
        {
            syDebugPrintf("getMapObjPosRandom:mpGetMapObjNumId(%d) = 0\n", mapobj_kind);
            scManagerRunPrintGObjStatus();
        }
    }
    mpCollisionGetMapObjIDsKind(mapobj_kind, mapobj);
    mpCollisionGetMapObjPositionID(mapobj[syUtilsRandIntRange(mapobj_count)], mapobj_pos);
}

// 0x8018D4EC
void sc1PGameSetupEnemyPlayer(SC1PGameStage *stagesetup, SC1PGameComputer *comsetup, s32 player, s32 enemy_player_num)
{
    s32 level = comsetup->enemy_level[gSCManagerBackupData.spgame_difficulty];

    level -= gSC1PManagerLevelDrop;

    if (level <= 0)
    {
        level = 1;
    }
    gSCManagerBattleState->players[player].level = level;
    gSCManagerBattleState->players[player].handicap = comsetup->enemy_handicap[gSCManagerBackupData.spgame_difficulty];
    gSCManagerBattleState->players[player].fkind = stagesetup->fkind[enemy_player_num];
    gSCManagerBattleState->players[player].team = nSCBattleTeamIDCom;
    gSCManagerBattleState->players[player].costume = 0;
    gSCManagerBattleState->players[player].shade = 0;
    gSCManagerBattleState->players[player].color = nSCBattlePlayerColorCP;
    gSCManagerBattleState->players[player].tag = nIFPlayerTagKindCP;
    gSCManagerBattleState->players[player].is_single_stockicon = TRUE;
    gSCManagerBattleState->players[player].stock_count = 0;
    gSCManagerBattleState->players[player].is_spgame_enemy = TRUE;
    gSCManagerBattleState->players[player].pkind = nFTPlayerKindCom;

    sSC1PGamePlayerSetups[player].cp_trait = stagesetup->opponent_behavior;

    sSC1PGameTeamPlayersRemaining--;
}

// 0x8018D60C
void sc1PGameSetupStageAll(void)
{
    SC1PGameStage *stagesetup = &dSC1PGameStageDesc[gSCManagerSceneData.spgame_stage];
    SC1PGameComputer *comsetup = &dSC1PGameComputerDesc[gSCManagerSceneData.spgame_stage];
    s32 player, com;
    u16 flags;
    s32 i;
    s32 variations;
    s32 level;
    s32 costume, kind;

    gSCManagerSceneData.is_reset = FALSE;

    gSCManagerBattleState->gkind = stagesetup->gkind;
    gSCManagerBattleState->is_team_attack = comsetup->is_team_attack;

    switch (gSCManagerSceneData.spgame_stage)
    {
    case nSC1PGameStageBonus3:
        gSCManagerBattleState->time_limit = 1;
        break;

    case nSC1PGameStageLuigi:
    case nSC1PGameStageNess:
    case nSC1PGameStagePurin:
    case nSC1PGameStageCaptain:
        gSCManagerBattleState->time_limit = SCBATTLE_TIMELIMIT_INFINITE;
        break;

    default:
        gSCManagerBattleState->time_limit = gSCManagerSceneData.spgame_time_limit;
        break;
    }
    gSCManagerBattleState->item_toggles = stagesetup->item_toggles;
    gSCManagerBattleState->item_appearance_rate = comsetup->item_appearance_rate;

    sSC1PGameEnemyPlayerCount = sSC1PGameEnemyStocksRemaining = sSC1PGameTeamPlayersRemaining = stagesetup->opponent_count;

    sSC1PGameEnemyStockSpriteFlags = 0;

    for (i = 0; i < (ARRAY_COUNT(gSCManagerBattleState->players) + ARRAY_COUNT(sSC1PGamePlayerSetups)) / 2; i++)
    {
        sSC1PGamePlayerSetups[i].figatree = NULL;
        sSC1PGamePlayerSetups[i].copy_kind = nFTKindKirby;
        sSC1PGamePlayerSetups[i].team_order = 0;
        sSC1PGamePlayerSetups[i].is_skip_entry = FALSE;
        sSC1PGamePlayerSetups[i].is_magnify_ignore = FALSE;
        sSC1PGamePlayerSetups[i].cp_trait = nFTComputerTraitDefault;
        sSC1PGamePlayerSetups[i].camera_frame_mul = 1.0F;

        if (i != gSCManagerSceneData.player)
        {
            gSCManagerBattleState->players[i].pkind = nFTPlayerKindNot;
        }
    }
    if (gSCManagerSceneData.spgame_stage <= nSC1PGameStageCommonEnd)
    {
        gSCManager1PGameBattleState.players[gSCManagerSceneData.player].is_single_stockicon = FALSE;
        sSC1PGamePlayerSetups[gSCManagerSceneData.player].mapobj_kind = nMPMapObjKind1PGamePlayer;
    }
    else
    {
        gSCManager1PGameBattleState.players[gSCManagerSceneData.player].is_single_stockicon = TRUE;
        sSC1PGamePlayerSetups[gSCManagerSceneData.player].mapobj_kind = nMPMapObjKind1PGameChallengerPlayer;
    }
    switch (gSCManagerSceneData.spgame_stage)
    {
    case nSC1PGameStageBoss:
    case nSC1PGameStageBonus3:
        sSC1PGamePlayerSetups[gSCManagerSceneData.player].is_skip_entry = TRUE;
        break;

    default:
        sSC1PGamePlayerSetups[gSCManagerSceneData.player].is_skip_entry = FALSE;
        break;
    }
    player = gSCManagerSceneData.player;

    switch (gSCManagerSceneData.spgame_stage)
    {
    case nSC1PGameStageLink:
    case nSC1PGameStageFox:
    case nSC1PGameStageMario:
    case nSC1PGameStagePikachu:
    case nSC1PGameStageDonkey:
    case nSC1PGameStageSamus:
    case nSC1PGameStageMMario:
    case nSC1PGameStageBoss:
    case nSC1PGameStageLuigi:
    case nSC1PGameStageNess:
    case nSC1PGameStagePurin:
    case nSC1PGameStageCaptain:
        for (i = 0; i < stagesetup->ally_count; i++)
        {
            gSCManagerBattleState->players[gSCManagerSceneData.ally_players[i]].level = comsetup->ally_level[gSCManagerBackupData.spgame_difficulty];
            gSCManagerBattleState->players[gSCManagerSceneData.ally_players[i]].handicap = comsetup->ally_handicap[gSCManagerBackupData.spgame_difficulty];
            gSCManagerBattleState->players[gSCManagerSceneData.ally_players[i]].team = 0;
            gSCManagerBattleState->players[gSCManagerSceneData.ally_players[i]].color = gSCManagerSceneData.player;
            gSCManagerBattleState->players[gSCManagerSceneData.ally_players[i]].tag = nIFPlayerTagKindHeart;
            gSCManagerBattleState->players[gSCManagerSceneData.ally_players[i]].is_single_stockicon = TRUE;
            gSCManagerBattleState->players[gSCManagerSceneData.ally_players[i]].stock_count = 0;
            gSCManagerBattleState->players[gSCManagerSceneData.ally_players[i]].is_spgame_enemy = FALSE;
            gSCManagerBattleState->players[gSCManagerSceneData.ally_players[i]].pkind = nFTPlayerKindCom;

            player = gSCManagerSceneData.ally_players[i];

            sSC1PGamePlayerSetups[gSCManagerSceneData.ally_players[i]].mapobj_kind = i + nMPMapObjKind1PGameAllyStart;
            sSC1PGamePlayerSetups[gSCManagerSceneData.ally_players[i]].cp_trait = stagesetup->ally_behavior;
        }
        for (i = 0; i < stagesetup->opponent_count; i++)
        {
            player = sc1PGameGetNextFreePlayerPort(player);

            sc1PGameSetupEnemyPlayer(stagesetup, comsetup, player, i);

            gSCManagerBattleState->players[player].costume = sc1PGameGetNextFreeCostume(player);

            if (gSCManagerSceneData.spgame_stage <= nSC1PGameStageCommonEnd)
            {
                sSC1PGamePlayerSetups[player].mapobj_kind = i + nMPMapObjKind1PGameEnemyStart;
            }
            else
            {
                sSC1PGamePlayerSetups[player].mapobj_kind = i + nMPMapObjKind1PGameChallengerEnemyStart;

                level = gSCManagerBattleState->players[player].level - gSCManagerSceneData.challenger_level_drop;

                gSCManagerBattleState->players[player].level = (level <= 0) ? 1 : level;
            }
            if (gSCManagerSceneData.spgame_stage == nSC1PGameStageBoss)
            {
                sSC1PGamePlayerSetups[player].is_magnify_ignore = TRUE;
            }
        }
        break;

    case nSC1PGameStageYoshi:
        variations = SC1PGAME_STAGE_YOSHI_VARIATIONS_COUNT;
        flags = 0;

        for (i = 0; i < SC1PGAME_STAGE_YOSHI_VARIATIONS_COUNT; i++)
        {
            sSC1PGameEnemyVariations[i] = sc1PGameGetFighterKindsNum(flags, syUtilsRandIntRange(variations));

            flags |= (1 << sSC1PGameEnemyVariations[i]);

            variations--;
        }
        for (costume = 0, i = SC1PGAME_STAGE_YOSHI_VARIATIONS_COUNT; i < stagesetup->opponent_count; i++)
        {
            sSC1PGameEnemyVariations[i] = sSC1PGameEnemyVariations[costume];

            if (costume == (SC1PGAME_STAGE_YOSHI_VARIATIONS_COUNT - 1))
            {
                costume = 0;
            }
            else costume++;
        }
        sSC1PGameCurrentEnemyVariation = 0;

        for (i = 0; i < SC1PGAME_STAGE_MAX_OPPONENT_COUNT; i++)
        {
            player = sc1PGameGetNextFreePlayerPort(player);

            sc1PGameSetupEnemyPlayer(stagesetup, comsetup, player, 0);

            gSCManagerBattleState->players[player].costume = sSC1PGameEnemyVariations[sSC1PGameCurrentEnemyVariation];

            if ((gSCManagerSceneData.fkind == nFTKindYoshi) && (gSCManagerSceneData.costume == gSCManagerBattleState->players[player].costume))
            {
                gSCManagerBattleState->players[player].shade = 1;
            }
            else gSCManagerBattleState->players[player].shade = 0;

            sSC1PGamePlayerSetups[player].mapobj_kind = i + nMPMapObjKind1PGameEnemyStart;
            sSC1PGamePlayerSetups[player].team_order = sSC1PGameCurrentEnemyVariation;
            sSC1PGamePlayerSetups[player].camera_frame_mul = 0.3F;
            sSC1PGamePlayerSetups[player].is_magnify_ignore = TRUE;

            sSC1PGameCurrentEnemyVariation++;
        }
        break;

    case nSC1PGameStageZako:
        variations = SC1PGAME_STAGE_MAX_VARIATIONS_COUNT;
        flags = 0;

        for (i = 0; i < SC1PGAME_STAGE_MAX_VARIATIONS_COUNT; i++)
        {
            sSC1PGameEnemyVariations[i] = sc1PGameGetFighterKindsNum(flags, syUtilsRandIntRange(variations));

            flags |= (1 << sSC1PGameEnemyVariations[i]);

            sSC1PGameEnemyVariations[i] += nFTKindNStart;

            variations--;
        }
        for (kind = 0, i = SC1PGAME_STAGE_MAX_VARIATIONS_COUNT; i < stagesetup->opponent_count; i++)
        {
            sSC1PGameEnemyVariations[i] = sSC1PGameEnemyVariations[kind];

            if (kind == (SC1PGAME_STAGE_MAX_VARIATIONS_COUNT - 1))
            {
                kind = 0;
            }
            else kind++;
        }
        sSC1PGameCurrentEnemyVariation = 0;

        for (i = 0; i < SC1PGAME_STAGE_MAX_OPPONENT_COUNT; i++)
        {
            player = sc1PGameGetNextFreePlayerPort(player);

            sc1PGameSetupEnemyPlayer(stagesetup, comsetup, player, 0);

            gSCManagerBattleState->players[player].fkind = sSC1PGameEnemyVariations[sSC1PGameCurrentEnemyVariation];

            sSC1PGamePlayerSetups[player].mapobj_kind = i + nMPMapObjKind1PGameEnemyStart;
            sSC1PGamePlayerSetups[player].team_order = sSC1PGameCurrentEnemyVariation;
            sSC1PGamePlayerSetups[player].camera_frame_mul = 0.3F;
            sSC1PGamePlayerSetups[player].is_magnify_ignore = TRUE;

            sSC1PGameCurrentEnemyVariation++;
        }
        break;

    case nSC1PGameStageKirby:
        sSC1PGameCurrentEnemyVariation = 0;

        for (i = 0; i < SC1PGAME_STAGE_KIRBY_SIM_COUNT; i++)
        {
            player = sc1PGameGetNextFreePlayerPort(player);

            sc1PGameSetupEnemyPlayer(stagesetup, comsetup, player, 0);

            sSC1PGameEnemyKirbyCostume = gSCManagerBattleState->players[player].costume =

            ((gSCManagerSceneData.fkind == nFTKindKirby) && (gSCManagerSceneData.costume == gSCManagerBattleState->players[player].costume)) ? 
                                                                          ftParamGetCostumeCommonID(nFTKindKirby, 1) : 
                                                                                                                  0 ;

            sSC1PGamePlayerSetups[player].team_order = sSC1PGameCurrentEnemyVariation;
            sSC1PGamePlayerSetups[player].copy_kind = dSC1PGameKirbyTeamCopyKinds[sSC1PGameCurrentEnemyVariation];
            sSC1PGamePlayerSetups[player].mapobj_kind = i + nMPMapObjKind1PGameEnemyStart;

            sSC1PGamePlayerSetups[player].camera_frame_mul = 0.3F;
            sSC1PGamePlayerSetups[player].is_magnify_ignore = TRUE;

            sSC1PGameCurrentEnemyVariation++;
        }
        break;

    case nSC1PGameStageBonus3:
        variations = SC1PGAME_STAGE_MAX_VARIATIONS_COUNT;
        flags = 0;

        for (i = 0; i < SC1PGAME_STAGE_MAX_OPPONENT_COUNT; i++)
        {
            sSC1PGameEnemyVariations[i] = sc1PGameGetFighterKindsNum(flags, syUtilsRandIntRange(variations));

            flags |= (1 << sSC1PGameEnemyVariations[i]);

            sSC1PGameEnemyVariations[i] += 14;

            variations--;
        }
        sSC1PGameCurrentEnemyVariation = 0;

        for (i = 0; i < SC1PGAME_STAGE_MAX_OPPONENT_COUNT; i++)
        {
            player = sc1PGameGetNextFreePlayerPort(player);

            sc1PGameSetupEnemyPlayer(stagesetup, comsetup, player, 0);

            gSCManagerBattleState->players[player].fkind = sSC1PGameEnemyVariations[sSC1PGameCurrentEnemyVariation];

            sSC1PGamePlayerSetups[player].mapobj_kind = i + nMPMapObjKind1PGameEnemyStart;
            sSC1PGamePlayerSetups[player].is_magnify_ignore = sSC1PGamePlayerSetups[player].is_skip_entry = TRUE;

            sSC1PGameCurrentEnemyVariation++;
        }
        break;

    default:
        break;
    }
    gSCManagerBattleState->pl_count = gSCManagerBattleState->cp_count = 0;

    for (i = 0; i < ARRAY_COUNT(gSCManagerBattleState->players); i++)
    {
        switch (gSCManagerBattleState->players[i].pkind)
        {
        case nFTPlayerKindMan:
            gSCManagerBattleState->pl_count++;
            break;

        case nFTPlayerKindCom:
            gSCManagerBattleState->cp_count++;
            break;
        }
    }
    gSC1PGameBonusTomatoCount = 0;
    gSC1PGameBonusHeartCount = 0;
#if defined(REGION_US)
    gSC1PGameBonusStarCount = 0;
#endif
    gSC1PGameBonusShieldBreaker = FALSE;
    gSC1PGameBonusGiantImpact = FALSE;
    gSC1PGameBonusMewCatcher = FALSE;

    sSC1PGameIsStartStage = FALSE;
    sSC1PGameIsEndStage = FALSE;

    sSC1PGameBonusStatPlayerKOsNum = 0;
    sSC1PGameBonusStatEndPlayerStatus = -1;
    sSC1PGameBonusStatInvincibleTimer = 0;
    gSC1PGameBonusBrosCalamity = FALSE;

    for (i = 0; i < ((ARRAY_COUNT(gSC1PGameBonusAttackIDCount) + ARRAY_COUNT(gSC1PGameBonusDefendIDCount)) / 2); i++)
    {
        gSC1PGameBonusAttackIDCount[i] = gSC1PGameBonusDefendIDCount[i] = 0;
    }
    for 
    (
        i = 0; 
        i < (ARRAY_COUNT(gSC1PGameBonusAttackIsSmashCount) + ARRAY_COUNT(gSC1PGameBonusAttackGroundAirCount) + ARRAY_COUNT(gSC1PGameBonusAttackIsProjectileCount) +
             ARRAY_COUNT(gSC1PGameBonusDefendIsSmashCount) + ARRAY_COUNT(gSC1PGameBonusDefendGroundAirCount) + ARRAY_COUNT(gSC1PGameBonusDefendIsProjectileCount)) / 6;
        i++
    )
    {
        gSC1PGameBonusAttackIsSmashCount[i] = gSC1PGameBonusDefendIsSmashCount[i] = 0;
        gSC1PGameBonusAttackGroundAirCount[i] = gSC1PGameBonusDefendGroundAirCount[i] = 0;
        gSC1PGameBonusAttackIsProjectileCount[i] = gSC1PGameBonusDefendIsProjectileCount[i] = 0;
    }
}

// 0x8018E18C - Spawn next member of enemy team
void sc1PGameSpawnEnemyTeamNext(GObj *player_gobj)
{
    FTStruct *fp;
    FTAttributes *attr;
    void *unused2;
    FTDesc desc;
    void **figatree;
    GObj *com_gobj;
    s32 player;
    void *unused1;

    fp = ftGetStruct(player_gobj);
    attr = fp->attr;
    player = fp->player;
    figatree = fp->figatree_heap;

    if (sSC1PGameTeamPlayersRemaining == 0)
    {
        ftCommonSleepSetStatus(player_gobj);
    }
    else
    {
        sSC1PGameTeamPlayersRemaining--;

        ftManagerDestroyFighter(player_gobj);

        gSCManagerBattleState->players[player].stock_count = 0;

        switch (gSCManagerSceneData.spgame_stage)
        {
        case nSC1PGameStageYoshi:
            gSCManagerBattleState->players[player].costume = sSC1PGameEnemyVariations[sSC1PGameCurrentEnemyVariation];

            gSCManagerBattleState->players[player].shade = ((gSCManagerSceneData.fkind == nFTKindYoshi) && (gSCManagerSceneData.costume == gSCManagerBattleState->players[player].costume)) ? 1 : 0;

            ifCommonPlayerStockSetLUT(player, gSCManagerBattleState->players[player].costume, attr);
            break;

        case nSC1PGameStageZako:
            gSCManagerBattleState->players[player].fkind = sSC1PGameEnemyVariations[sSC1PGameCurrentEnemyVariation];
            break;

        case nSC1PGameStageKirby:
            sSC1PGamePlayerSetups[player].copy_kind = 
            (sSC1PGameCurrentEnemyVariation == SC1PGAME_STAGE_KIRBY_VARIATIONS_COUNT)  ? 
                                                        gSC1PManagerKirbyTeamFinalCopy :
                            dSC1PGameKirbyTeamCopyKinds[sSC1PGameCurrentEnemyVariation];
            break;
        }
        sSC1PGamePlayerSetups[player].team_order = sSC1PGameCurrentEnemyVariation++;

        desc = dFTManagerDefaultFighterDesc;

        desc.fkind = gSCManagerBattleState->players[player].fkind;

        sc1PGameGetRandomStartPosition(&desc.pos, nMPMapObjKind1PGameEnemyTeam);

        desc.pos.y = (gMPCollisionGroundData->camera_bound_top + gMPCollisionGroundData->map_bound_top) * 0.5F;

        desc.lr = (desc.pos.x >= 0.0F) ? -1 : +1;

        desc.team = gSCManagerBattleState->players[player].team;

        desc.player = player;

        desc.detail = ((gSCManagerBattleState->pl_count + gSCManagerBattleState->cp_count) < 3) ? nFTPartsDetailHigh : nFTPartsDetailLow;

        desc.costume = gSCManagerBattleState->players[player].costume;

        desc.shade = gSCManagerBattleState->players[player].shade;

        desc.handicap = gSCManagerBattleState->players[player].handicap;

        desc.level = gSCManagerBattleState->players[player].level;

        desc.stock_count = gSCManagerBattleState->players[player].stock_count;

        desc.damage = 0;

        desc.pkind = gSCManagerBattleState->players[player].pkind;

        desc.controller = &gSYControllerDevices[player];

        desc.figatree_heap = figatree;

        desc.copy_kind = sSC1PGamePlayerSetups[player].copy_kind;

        desc.team_order = sSC1PGamePlayerSetups[player].team_order;

        desc.is_skip_entry = TRUE;
        desc.is_magnify_ignore = sSC1PGamePlayerSetups[player].is_magnify_ignore;
        desc.is_skip_shadow_setup = TRUE;

        com_gobj = ftManagerMakeFighter(&desc);

        fp = ftGetStruct(com_gobj);

        ftParamInitPlayerBattleStats(player, com_gobj);
        ftParamUnlockPlayerControl(com_gobj);

        fp->computer.trait = sSC1PGamePlayerSetups[player].cp_trait;

        ifCommonPlayerDamageStopBreakAnim(fp);

        fp->camera_zoom_frame *= sSC1PGamePlayerSetups[player].camera_frame_mul;
    }
}

// 0x8018E598
void sc1PGameSetPlayerInterfacePositions(void)
{
    s32 *pos = dSC1PGameInterfaceCountPositions[(gSCManagerBattleState->pl_count + gSCManagerBattleState->cp_count) - 2];
    s32 player = gSCManagerSceneData.player;
    s32 i;

    for (i = 0; i < (gSCManagerBattleState->pl_count + gSCManagerBattleState->cp_count); i++)
    {
        sSC1PGamePlayerInterfacePositionsX[player] = *pos;
        player = sc1PGameGetNextFreePlayerPort(player);

        pos++;
    }
    gIFCommonPlayerInterface.player_pos_x = sSC1PGamePlayerInterfacePositionsX;
    gIFCommonPlayerInterface.player_pos_y = 210;
}

// 0x8018E670
void sc1PGameWaitStageCommonUpdate(void)
{
    GObj *fighter_gobj;
    s32 random;
    s32 player_count;
    s32 sleep_tics;
    s32 player;
    s32 i;

    random = syUtilsRandIntRange(ARRAY_COUNT(dSC1PGameStageCommonSleepTics));
    gcSleepCurrentGObjThread(90);
    ifCommonCountdownMakeInterface();

    sleep_tics = dSC1PGameStageCommonSleepTics[random];

    if (random == 1)
    {
        gcSleepCurrentGObjThread(90);
    }
    player_count = gSCManagerBattleState->pl_count + gSCManagerBattleState->cp_count;

    if (player_count < 3)
    {
        gcSleepCurrentGObjThread(sleep_tics);
    }
    player = gSCManagerSceneData.player;

    for (i = 0; i < player_count; i++)
    {
        fighter_gobj = gSCManagerBattleState->players[player].fighter_gobj;

        ftCommonAppearSetStatus(fighter_gobj);

        if (random == 2)
        {
            gcSleepCurrentGObjThread(30);
            gmCameraSetStatusPlayerZoom(fighter_gobj, 0.0F, 0.0F, ftGetStruct(fighter_gobj)->attr->closeup_camera_zoom, 0.1F, 28.0F);
            gcSleepCurrentGObjThread(sleep_tics - 30);
        }
        else gcSleepCurrentGObjThread(sleep_tics);

        player = sc1PGameGetNextFreePlayerPort(player);
    }
    if (random == 2)
    {
        gcSleepCurrentGObjThread(30);
        gmCameraSetStatusDefault();
    }
}

// 0x8018E824
void sc1PGameWaitStageTeamUpdate(void)
{
    GObj *fighter_gobj;
    s32 player_count;
    s32 player;
    s32 i;

    gcSleepCurrentGObjThread(90);
    ifCommonCountdownMakeInterface();

    player_count = gSCManagerBattleState->pl_count + gSCManagerBattleState->cp_count;
    player = gSCManagerSceneData.player;

    for (i = 0; i < player_count; i++)
    {
        fighter_gobj = gSCManagerBattleState->players[player].fighter_gobj;

        if (player == gSCManagerSceneData.player)
        {
            ftCommonAppearSetStatus(fighter_gobj);
        }
        else ftCommonAppearSetPosition(fighter_gobj);

        gcSleepCurrentGObjThread(60);

        player = sc1PGameGetNextFreePlayerPort(player);
    }
}

// 0x8018E8F8
void sc1PGameWaitStageBonus3Update(void)
{
    gmCameraSetStatusPlayerFollow
    (
        gSCManagerBattleState->players[gSCManagerSceneData.player].fighter_gobj, 
        0.0F, 
        F_CST_DTOR32(-15.0F), 
        7000.0F, 
        0.3F, 
        31.5F
    );
    gcSleepCurrentGObjThread(60);
    ifCommonAnnounceGoMakeInterface();
    ifCommonPlayerDamageSetShowInterface();
    func_800269C0_275C0(nSYAudioVoiceAnnounceGo);
    ifCommonAnnounceGoSetStatus();
    gcEjectGObj(NULL);
    gcSleepCurrentGObjThread(1);
}

// 0x8018E9A4
void sc1PGameWaitStageBossUpdate(void)
{
    GObj *player_gobj;
    FTStruct *player_fp;
    FTStruct *com_fp;
    GObj *com_gobj;
    s32 player;
    Vec3f sp20;

    player_gobj = gSCManagerBattleState->players[gSCManagerSceneData.player].fighter_gobj;

    player_fp = ftGetStruct(player_gobj);

    sp20.x = 0.0F;
    sp20.y = 0.0F;
    sp20.z = 0.0F;

    gmCameraSetStatusAnim(lbRelocGetFileData(AObjEvent32*, ((uintptr_t)gMPCollisionGroundData->gr_desc[1].dobjdesc - (intptr_t)&llGRLastMapFileHead), &D_NF_00006010), 0.0F, &sp20);

    for (player = 0; TRUE; player++) // Wut da haeiyll
    {
        if (gSCManagerBattleState->players[player].pkind == nFTPlayerKindNot)
        {
            continue;
        }
        else if (gSCManagerBattleState->players[player].is_spgame_enemy == FALSE)
        {
            continue;
        }
        else
        {
            com_gobj = gSCManagerBattleState->players[player].fighter_gobj;

            break;
        }
    }
    com_fp = ftGetStruct(com_gobj);

    ftCommonAppearSetStatus(com_gobj);

    gcSleepCurrentGObjThread(600);

    ftParamUnlockPlayerControl(player_gobj);
    ftParamUnlockPlayerControl(com_gobj);

    player_fp->camera_mode = nFTCameraModeDefault;
    com_fp->camera_mode = nFTCameraModeDefault;

    gSCManagerBattleState->game_status = nSCBattleGameStatusGo;

    gIFCommonPlayerInterface.is_magnify_display = TRUE;

    ifCommonPlayerDamageSetShowInterface();

    gMPCollisionBGMDefault = nSYAudioBGMLast;

    ftParamTryUpdateItemMusic();
}

// 0x8018EADC
void sc1PGameWaitThreadUpdate(GObj *gobj)
{
    switch (gSCManagerSceneData.spgame_stage)
    {
    case nSC1PGameStageLink:
    case nSC1PGameStageFox:
    case nSC1PGameStageMario:
    case nSC1PGameStagePikachu:
    case nSC1PGameStageDonkey:
    case nSC1PGameStageSamus:
    case nSC1PGameStageMMario:
    case nSC1PGameStageLuigi:
    case nSC1PGameStageNess:
    case nSC1PGameStagePurin:
    case nSC1PGameStageCaptain:
        sc1PGameWaitStageCommonUpdate();
        break;

    case nSC1PGameStageYoshi:
    case nSC1PGameStageKirby:
    case nSC1PGameStageZako:
        sc1PGameWaitStageTeamUpdate();
        break;

    case nSC1PGameStageBonus3:
        sc1PGameWaitStageBonus3Update();
        break;

    case nSC1PGameStageBoss:
        sc1PGameWaitStageBossUpdate();
        break;
    }
    gcEjectGObj(NULL);
    gcSleepCurrentGObjThread(1);
}

// 0x8018EB68
void sc1PGameSetGameStatusWait(void)
{
    gcAddGObjProcess
    (
        gcMakeGObjSPAfter
        (
            nGCCommonKindInterface, 
            NULL, 
            nGCCommonLinkIDInterfaceActor, 
            GOBJ_PRIORITY_DEFAULT
        ),     
        sc1PGameWaitThreadUpdate, 
        nGCProcessKindThread, 
        5
    );
    gSCManagerBattleState->game_status = nSCBattleGameStatusWait;
}

// 0x8018EBB8
void sc1PGameTeamStockDisplayProcDisplay(GObj *interface_gobj)
{
    SObj *sobj;
    s32 stock_num;
    s32 ix, iy;

    if (sSC1PGameEnemyStocksRemaining != 0)
    {
        if (sSC1PGameEnemyStocksRemaining != sSC1PGameEnemyStocksDisplay)
        {
            sSC1PGameEnemyStocksDisplay = sSC1PGameEnemyStocksRemaining;

            sobj = SObjGetStruct(interface_gobj);

            for (stock_num = sSC1PGameEnemyPlayerCount - 1, ix = iy = 0; sobj != NULL; sobj = sobj->next, stock_num--)
            {
                if ((1 << stock_num) & sSC1PGameEnemyStockSpriteFlags)
                {
                    sobj->sprite.attr |= SP_HIDDEN;
                }
                else
                {
                    sobj->pos.x = (ix * 10) + 20;
                    sobj->pos.y = (iy * 10) + 20;

                    ix++;

                    if (ix >= 10)
                    {
                        ix = 0, iy++;
                    }
                    switch (gSCManagerSceneData.spgame_stage)
                    {
                    case nSC1PGameStageYoshi:
                        sobj->sprite = *sSC1PGameEnemyTeamSprites->stock_sprite;
                        sobj->sprite.LUT = sSC1PGameEnemyTeamSprites->stock_luts[sSC1PGameEnemyVariations[stock_num]];
                        break;

                    case nSC1PGameStageKirby:
                        sobj->sprite = *sSC1PGameEnemyTeamSprites->stock_sprite;
                        sobj->sprite.LUT = sSC1PGameEnemyTeamSprites->stock_luts[sSC1PGameEnemyKirbyCostume];
                        break;

                    case nSC1PGameStageZako:
                        sobj->sprite = *lbRelocGetFileData(Sprite*, sSC1PGameZakoStockFile, &llFTStocksZakoSprite);
                        break;
                    }
                    sobj->sprite.attr &= ~SP_HIDDEN;
                }
            }
        }
        lbCommonDrawSObjAttr(interface_gobj);
    }
}

// 0x8018EE44
void sc1PGameInitTeamStockDisplay(void)
{
    GObj *interface_gobj;
    FTStruct *fp;
    Sprite *sprite;
    s32 i;

    switch (gSCManagerSceneData.spgame_stage)
    {
    case nSC1PGameStageYoshi:
    case nSC1PGameStageKirby:
        for (i = 0; i < ARRAY_COUNT(gSCManagerBattleState->players); i++)
        {
            if (gSCManagerBattleState->players[i].pkind == nFTPlayerKindNot)
            {
                continue;
            }
            else if (gSCManagerBattleState->players[i].is_spgame_enemy == FALSE)
            {
                continue;
            }
            else
            {
                fp = ftGetStruct(gSCManagerBattleState->players[i].fighter_gobj);

                break;
            }
        }
        sSC1PGameEnemyTeamSprites = fp->attr->sprites;

        sprite = sSC1PGameEnemyTeamSprites->stock_sprite;
        sprite->attr = SP_TEXSHUF | SP_TRANSPARENT;

        goto make_gobj;

    case nSC1PGameStageZako:
        sSC1PGameZakoStockFile = lbRelocGetExternHeapFile
        (
            (u32)&llFTStocksZakoFileID,
            syTaskmanMalloc
            (
                lbRelocGetFileSize((u32)&llFTStocksZakoFileID),
                0x10
            )
        );
        sprite = lbRelocGetFileData(Sprite*, sSC1PGameZakoStockFile, &llFTStocksZakoSprite);

        sprite->attr = SP_TEXSHUF | SP_TRANSPARENT;

    make_gobj:
        interface_gobj = gcMakeGObjSPAfter(nGCCommonKindInterface, NULL, nGCCommonLinkIDInterface, GOBJ_PRIORITY_DEFAULT);

        gcAddGObjDisplay(interface_gobj, sc1PGameTeamStockDisplayProcDisplay, 23, GOBJ_PRIORITY_DEFAULT, ~0);

        for (i = 0; i < sSC1PGameEnemyStocksRemaining; i++)
        {
            lbCommonMakeSObjForGObj(interface_gobj, lbRelocGetFileData(Sprite*, gGMCommonFiles[4], &ll_104_FileID));
        }
        sSC1PGameEnemyStocksDisplay = sSC1PGameEnemyStocksRemaining + 1;

        break;
    }
}

// 0x8018EFFC
void sc1PGameSetPlayerDefeatStats(s32 player, s32 team_order)
{
    GObj *fighter_gobj = gSCManagerBattleState->players[player].fighter_gobj;
    FTStruct *fp = ftGetStruct(fighter_gobj);
    SC1PGameStats *enemy_stats;

    if 
    (
        (player == gSCManagerSceneData.player) && 
        (gSCManagerBattleState->players[player].stock_count == -1) && 
        (gSCManagerBattleState->game_status != nSCBattleGameStatusEnd)
    )
    {
        ifCommonAnnounceEndMessage();
    }
    else if (gSCManagerBattleState->players[player].is_spgame_enemy != FALSE)
    {
        sSC1PGameEnemyStocksRemaining--;
        sSC1PGameEnemyStockSpriteFlags |= (1 << team_order);

        enemy_stats = &sSC1PGameBonusStatEnemyStats[sSC1PGameBonusStatPlayerKOsNum];

        enemy_stats->team_order = fp->team_order;
        enemy_stats->damage_status_id = fp->status_id;
        enemy_stats->damage_player = fp->damage_player;
        enemy_stats->damage_object_class = fp->damage_object_class;
        enemy_stats->damage_object_kind = fp->damage_object_kind;
        enemy_stats->damage_stat_flags = fp->damage_stat_flags;
        enemy_stats->damage_stat_count = fp->damage_stat_count;

        sSC1PGameBonusStatPlayerKOsNum++;

        if ((gSCManagerSceneData.spgame_stage == nSC1PGameStageMario) && (fp->fkind == nFTKindLuigi) && (sSC1PGameEnemyStocksRemaining != 0) && (fp->damage_player == gSCManagerSceneData.player))
        {
            if (gSCManagerBattleState->players[(fp->player == 0) ? (GMCOMMON_PLAYERS_MAX - 1) : fp->player - 1].total_damage_players[gSCManagerSceneData.player] == 0)
            {
                gSC1PGameBonusBrosCalamity = TRUE; // Bros. Calamity bonus
            }
        }
        if (sSC1PGameEnemyStocksRemaining == 0)
        {
            ifCommonAnnounceEndMessage();
        }
    }
}

// 0x8018F1C0
void sc1PGameTryInitPlayerArrows(void)
{
    if (gSCManagerSceneData.spgame_stage != nSC1PGameStageBonus3)
    {
        gmCameraMakePlayerArrowsCamera();
        ifCommonPlayerArrowsInitInterface();
    }
}

// 0x8018F1F8
void sc1PGameInitTimeUpMessage(void)
{
    if (gSCManagerSceneData.spgame_stage != nSC1PGameStageBonus3)
    {
        ifCommonTimerMakeInterface(ifCommonAnnounceTimeUpInitInterface);
    }
    else ifCommonTimerMakeInterface(ifCommonAnnounceFailureInitInterface);
}

// 0x8018F240
s32 sc1PGameGetEnemyStartLR(s32 this_player)
{
    f32 lr_dist;
    f32 closest_dist;
    f32 current_dist;
    s32 current_player;
    Vec3f current_pos;
    Vec3f target_pos;
    s32 lr;

    closest_dist = 65536.0F;
    lr_dist = 0.0F;

    sc1PGameGetStartPosition(&target_pos, sSC1PGamePlayerSetups[this_player].mapobj_kind);

    for (current_player = 0; current_player < (ARRAY_COUNT(gSCManagerBattleState->players) + ARRAY_COUNT(sSC1PGamePlayerSetups)) / 2; current_player++)
    {
        if (current_player == this_player)
        {
            continue;
        }
        else if (gSCManagerBattleState->players[current_player].pkind == nFTPlayerKindNot)
        {
            continue;
        }
        else if (gSCManagerBattleState->players[current_player].team != gSCManagerBattleState->players[this_player].team)
        {
            sc1PGameGetStartPosition(&current_pos, sSC1PGamePlayerSetups[current_player].mapobj_kind);

            current_dist = (current_pos.x < target_pos.x) ? -(current_pos.x - target_pos.x) : (current_pos.x - target_pos.x);

            if (current_dist < closest_dist)
            {
                closest_dist = current_dist;

                lr_dist = (current_pos.x - target_pos.x);
            }
        }
    }
    lr = (lr_dist >= 0.0F) ? +1 : -1;

    return lr;
}

// 0x8018F3AC
void sc1PGameSetCameraZoom(void)
{
    GObj *fighter_gobj = gSCManagerBattleState->players[gSCManagerSceneData.player].fighter_gobj;
    FTStruct *fp = ftGetStruct(fighter_gobj);

    gmCameraSetStatusPlayerZoom
    (
        fighter_gobj,
        F_CLC_DTOR32(dSC1PGameZoomEyeX[syUtilsRandIntRange(ARRAY_COUNT(dSC1PGameZoomEyeX))]) * fp->lr,
        F_CLC_DTOR32(dSC1PGameZoomEyeY[syUtilsRandIntRange(ARRAY_COUNT(dSC1PGameZoomEyeY))]),
        ftGetStruct(fighter_gobj)->attr->closeup_camera_zoom,
        0.06F,
        28.0F
    );
    ftParamSetModelPartDetailAll(fighter_gobj, nFTPartsDetailHigh);
}

// 0x8018F4B0
void sc1PGameBossSetCameraZoom(FTStruct *fp)
{
    Vec3f world_pos;
    Vec3f zoom;

    world_pos.x = world_pos.y = world_pos.z = 0.0F;

    gmCollisionGetFighterPartsWorldPosition(fp->joints[fp->damage_joint_id], &world_pos);

    sSC1PGameBossDefeatZoomPosition = world_pos;

    zoom.x = world_pos.x;
    zoom.y = world_pos.y;
    zoom.z = world_pos.z + 3000.0F;

    gmCameraSetStatusMapZoom(&world_pos, &zoom);
}

// 0x8018F540
void sc1PGameBossHidePlayerTagAll(void)
{
    GObj *fighter_gobj = gGCCommonLinks[nGCCommonLinkIDFighter];

    while (fighter_gobj != NULL)
    {
        FTStruct *fp = ftGetStruct(fighter_gobj);

        fp->is_playertag_bossend = TRUE;

        fighter_gobj = fighter_gobj->link_next;
    }
}

// 0x8018F574
void sc1PGameBossAddBossInterface(GObj *fighter_gobj, u32 unused)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->fkind == nFTKindBoss)
    {
        ifCommonPlayerStockMakeStockSnap(fp);
    }
}

// 0x8018F5AC
void sc1PGameBossLockPlayerControl(GObj *fighter_gobj, u32 unused)
{
    ftParamLockPlayerControl(fighter_gobj);
}

// 0x8018F5CC
void sc1PGameBossSetIgnorePlayerMapBounds(GObj *fighter_gobj, u32 unused)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->is_limit_map_bounds = TRUE;
}

// 0x8018F5E4 - Unused?
void func_ovl65_8018F5E4(void)
{
    return;
}

// 0x8018F5EC
void sc1PGameBossDefeatInterfaceProcUpdate(void)
{
    gcFuncGObjByLink(nGCCommonLinkIDFighter, sc1PGameBossAddBossInterface, 0);
    gcFuncGObjByLink(nGCCommonLinkIDFighter, sc1PGameBossLockPlayerControl, 0);
    gcFuncGObjByLink(nGCCommonLinkIDFighter, sc1PGameBossSetIgnorePlayerMapBounds, 0);
    gcFuncGObjAll(ifCommonBattleInterfacePauseGObj, 0);

    gcFuncGObjByLink(nGCCommonLinkIDCamera, ifCommonBattleInterfaceResumeGObj, 0);
    gmRumbleResumeProcessAll();
    gcFuncGObjByLink(nGCCommonLinkIDInterface, ifCommonBattleInterfaceResumeGObj, 0);
    gcFuncGObjByLink(nGCCommonLinkIDEffect, ifCommonBattleInterfaceResumeGObj, 0);
    func_ovl65_801910B0();
    func_800266A0_272A0();
    syAudioStopBGMAll();
    ifCommonBattleEndPlaySoundQueue();
    func_800269C0_275C0(nSYAudioFGMExplodeL);
    func_800269C0_275C0(nSYAudioVoiceBossDead);
    func_800269C0_275C0(nSYAudioFGMBossDefeatL);

    sSC1PGameBossDefeatSoundTerminateTemp = D_8009EDD0_406D0.sfx_max;
    D_8009EDD0_406D0.sfx_max = 0;
}

// 0x8018F6DC
void func_ovl65_8018F6DC(void)
{
    D_8009EDD0_406D0.sfx_max = sSC1PGameBossDefeatSoundTerminateTemp;
}

// 0x8018F6F0
void sc1PGameBossDefeatInterfaceProcSet(void)
{
    gcFuncGObjAll(ifCommonBattleInterfaceResumeGObj, 0);
    sc1PGameBossSetChangeWallpaper();
    gmCameraSetStatusAnim((void*) (((uintptr_t)gMPCollisionGroundData->gr_desc[1].dobjdesc - (intptr_t)&llGRLastMapFileHead) + (intptr_t)&D_NF_00006450), 0.0F, &sSC1PGameBossDefeatZoomPosition);
    ifCommonBattleBossDefeatSetGameStatus();
}

// 0x8018F75C
void sc1PGameBossDefeatInitInterface(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    gIFCommonPlayerInterface.is_magnify_display = FALSE;

    sc1PGameBossHidePlayerTagAll();
    ifCommonPlayerDamageStartBreakAnim(fp);
    sc1PGameBossSetCameraZoom(fp);
    ifCommonBattleSetInterface(sc1PGameBossDefeatInterfaceProcUpdate, sc1PGameBossDefeatInterfaceProcSet, nSYAudioFGMVoiceEnd, 90);
}

// 0x8018F7B4
void sc1PGameFuncStart(void)
{
    GObj *fighter_gobj;
    FTData *plns;
    size_t largest_size;
    FTStruct *fp;
    sb32 (*func_sign)(void*);
    void *file;
    u8 signature[0x10];
    s32 i;
    FTDesc desc;
    SYColorRGBA color;

    sc1PGameSetupStageAll();
    sc1PGameSetupFiles();

    if (!(gSCManagerBackupData.error_flags & LBBACKUP_ERROR_VSBATTLECASTLE) && (gSCManagerBackupData.boot > 92))
    {
        syDmaReadRom(0xF10, signature, ARRAY_COUNT(signature));

        file = lbRelocGetExternHeapFile((u32)&llSYSignValidateFileID, syTaskmanMalloc(lbRelocGetFileSize((u32)&llSYSignValidateFileID), 0x10));
        func_sign = lbRelocGetFileData(sb32 (*)(void*), file, &llSYSignValidateFunc);

        osWritebackDCache(func_sign, *lbRelocGetFileData(s32*, file, &llSYSignValidateNBytes));
        osInvalICache(func_sign, *lbRelocGetFileData(s32*, file, &llSYSignValidateNBytes));

        if (func_sign(signature) == FALSE)
        {
            gSCManagerBackupData.error_flags |= LBBACKUP_ERROR_VSBATTLECASTLE;
        }
    }
    gcMakeDefaultCameraGObj(nGCCommonLinkIDCamera, GOBJ_PRIORITY_DEFAULT, 100, COBJ_FLAG_ZBUFFER, GPACK_RGBA8888(0x00, 0x00, 0x00, 0xFF));
    efParticleInitAll();
    ftParamInitGame();
    mpCollisionInitGroundData();
    gmCameraSetViewportDimensions(10, 10, 310, 230);
    gmCameraMakeWallpaperCamera();
    grWallpaperMakeDecideKind();
    gmCameraMakeBattleCamera();
    itManagerInitItems();
    grCommonSetupInitAll();
    ftManagerAllocFighter(FTDATA_FLAG_MAINMOTION, 4);
    wpManagerAllocWeapons();
    efManagerInitEffects();
    ifScreenFlashMakeInterface(dSC1PGameStageDesc[gSCManagerSceneData.spgame_stage].screenflash_alpha);
    gmRumbleMakeActor();
    ftPublicMakeActor();

    switch (gSCManagerSceneData.spgame_stage)
    {
    case nSC1PGameStageKirby:
        // Need to load PK Fire graphics from Ness' file
        plns = dFTManagerDataFiles[nFTKindNess];

        lbRelocGetExternHeapFile((uintptr_t)&ll_230_FileID, syTaskmanMalloc(lbRelocGetFileSize((uintptr_t)&ll_230_FileID), 0x10));
        efParticleGetLoadBankID
        (
            plns->particles_script_lo, 
            plns->particles_script_hi, 
            plns->particles_texture_lo, 
            plns->particles_texture_hi
        );
        break;

    case nSC1PGameStageZako:
        for (i = nFTKindNStart; i <= nFTKindNEnd; i++)
        {
            ftManagerSetupFilesAllKind(i);
        }
        largest_size = 0;

        for (i = nFTKindNStart; i <= nFTKindNEnd; i++)
        {
            if (largest_size < dFTManagerDataFiles[i]->file_anim_size)
            {
                largest_size = dFTManagerDataFiles[i]->file_anim_size;
            }
        }
        for (i = 0; i < (ARRAY_COUNT(gSCManagerBattleState->players) + ARRAY_COUNT(sSC1PGamePlayerSetups)) / 2; i++)
        {
            if (gSCManagerBattleState->players[i].pkind == nFTPlayerKindNot)
            {
                continue;
            }
            else if (gSCManagerBattleState->players[i].is_spgame_enemy == FALSE)
            {
                continue;
            }
            else sSC1PGamePlayerSetups[i].figatree = syTaskmanMalloc(largest_size, 0x10);
        }
        break;
    }
    for (i = 0; i < (ARRAY_COUNT(gSCManagerBattleState->players) + ARRAY_COUNT(sSC1PGamePlayerSetups)) / 2; i++)
    {
        desc = dFTManagerDefaultFighterDesc;

        if (gSCManagerBattleState->players[i].pkind == nFTPlayerKindNot)
        {
            continue;
        }
        ftManagerSetupFilesAllKind(gSCManagerBattleState->players[i].fkind);

        desc.fkind = gSCManagerBattleState->players[i].fkind;

        sc1PGameGetStartPosition(&desc.pos, sSC1PGamePlayerSetups[i].mapobj_kind);

        desc.lr = sc1PGameGetEnemyStartLR(i);

        desc.team = gSCManagerBattleState->players[i].team;

        desc.player = i;

        desc.detail = ((gSCManagerBattleState->pl_count + gSCManagerBattleState->cp_count) < 3) ? nFTPartsDetailHigh : nFTPartsDetailLow;

        desc.costume = gSCManagerBattleState->players[i].costume;

        desc.shade = gSCManagerBattleState->players[i].shade;

        desc.handicap = gSCManagerBattleState->players[i].handicap;

        desc.level = gSCManagerBattleState->players[i].level;

        desc.stock_count = gSCManagerBattleState->players[i].stock_count;

        desc.damage = 0;

        desc.pkind = gSCManagerBattleState->players[i].pkind;

        desc.controller = &gSYControllerDevices[i];

        desc.figatree_heap = (sSC1PGamePlayerSetups[i].figatree != NULL) ? sSC1PGamePlayerSetups[i].figatree : ftManagerAllocFigatreeHeapKind(gSCManagerBattleState->players[i].fkind);

        desc.copy_kind = sSC1PGamePlayerSetups[i].copy_kind;

        desc.team_order = sSC1PGamePlayerSetups[i].team_order;

        desc.is_skip_entry = sSC1PGamePlayerSetups[i].is_skip_entry;

        desc.is_magnify_ignore = sSC1PGamePlayerSetups[i].is_magnify_ignore;

        fighter_gobj = ftManagerMakeFighter(&desc), fp = ftGetStruct(fighter_gobj);

        ftParamInitPlayerBattleStats(i, fighter_gobj);

        fp->computer.trait = sSC1PGamePlayerSetups[i].cp_trait;

        fp->camera_zoom_frame *= sSC1PGamePlayerSetups[i].camera_frame_mul;
    }
    ftManagerSetupFilesPlayablesAll();
    ifCommonBattleSetGameStatusWait();
    sc1PGameTryInitPlayerArrows();
    gmCameraMakePlayerMagnifyCamera();
    ifCommonPlayerMagnifyMakeInterface();
    gmCameraScreenFlashMakeCamera();
    gmCameraMakeInterfaceCamera();
    gmCameraMakeEffectCamera();
    ifCommonPlayerTagMakeInterface();
    sc1PGameSetPlayerInterfacePositions();
    ifCommonPlayerDamageInitInterface();
    ifCommonPlayerStockInitInterface();
    sc1PGameInitTeamStockDisplay();
    sc1PGameSetGameStatusWait();

    if ((gSCManagerSceneData.spgame_stage == nSC1PGameStageMMario) || (gSCManagerSceneData.spgame_stage == nSC1PGameStageZako))
    {
        mpCollisionSetBGM();
        func_800269C0_275C0(nSYAudioFGMPublicPrologue);
    }
    else
    {
        mpCollisionSetPlayBGM();
        func_800269C0_275C0(nSYAudioVoicePublicExcited);
    }
    sc1PGameInitTimeUpMessage();
    ifCommonTimerMakeDigits();

    color = dSC1PGameFadeColor;
    lbFadeMakeActor(nGCCommonKindTransition, nGCCommonLinkIDTransition, 10, &color, 12, TRUE, NULL);
}

// 0x8018FD5C
void sc1PGameAppendBonusStats(void)
{
    sb32 is_thrown_item;
    sb32 is_jackpot;
    sb32 is_item_strike;
    sb32 is_ordered_variation;
    s32 damage_jackpot_div;
    s32 damage_jackpot_mod;
    s32 damage_jackpot_bak;
    s32 variation;
    s32 special_count;
    u32 attack_count;
    u8 variation_order[SC1PGAME_STAGE_MAX_VARIATIONS_COUNT]; // Full array of 12? Almost half of this goes unused, but stack suggests this is correct.
    s32 i;

    sSC1PGameStageTimeSec = I_TICS_TO_SEC(gSCManagerBattleState->time_passed);
    sSC1PGameTotalTimeSec = I_TICS_TO_SEC(gSC1PManagerTotalTimeTics);

    if (sSC1PGameBonusStatPlayerKOsNum != 0)
    {
        for (i = 0; i < sSC1PGameBonusStatPlayerKOsNum; i++)
        {
            if (TRUE);

            if
            (
                (sSC1PGameBonusStatEnemyStats[i].damage_player != gSCManagerSceneData.player)   ||
                (sSC1PGameBonusStatEnemyStats[i].damage_status_id != nFTCommonStatusDeadUpStar) &&
                (sSC1PGameBonusStatEnemyStats[i].damage_status_id != nFTCommonStatusDeadUpFall)
            )
            {
                goto loop_item_strike;
            }
        }
        switch (gSCManagerSceneData.spgame_stage)
        {
        case nSC1PGameStageYoshi:
        case nSC1PGameStageKirby:
        case nSC1PGameStageZako:
            gSCManagerSceneData.bonus_get_mask[0] |= SC1PGAME_BONUS_MASK0_TRICKSTER;
            break;

        default:
            gSCManagerSceneData.bonus_get_mask[0] |= SC1PGAME_BONUS_MASK0_STARFINISH;
            break;
        }
    }
loop_item_strike:
    is_item_strike = FALSE;

    for (i = nFTStatusAttackIDAttackStart; i < ARRAY_COUNT(gSC1PGameBonusDefendIDCount); i++)
    {
        if (gSC1PGameBonusDefendIDCount[i] != 0)
        {
            if ((i >= nFTStatusAttackIDItemStart) && (i <= nFTStatusAttackIDItemEnd) || (i == nFTStatusAttackIDNull))
            {
                is_item_strike = TRUE;
            }
            else goto check_item_strike;
        }
    }
    if (is_item_strike != FALSE)
    {
        // Item Strike
        gSCManagerSceneData.bonus_get_mask[0] |= SC1PGAME_BONUS_MASK0_ITEMSTRIKE;
    }
check_item_strike:
    if (!(gSCManagerSceneData.bonus_get_mask[0] & SC1PGAME_BONUS_MASK0_ITEMSTRIKE))
    {
        if (gSCManagerBattleState->item_appearance_rate != nSCBattleItemSwitchNone)
        {
#if defined(REGION_US)
            if ((gSC1PGameBonusTomatoCount == 0) && (gSC1PGameBonusHeartCount == 0) && (gSC1PGameBonusStarCount == 0))
#else
            if ((gSC1PGameBonusTomatoCount == 0) && (gSC1PGameBonusHeartCount == 0))
#endif
            {
                for (i = nFTMotionAttackIDItemStart; i <= nFTMotionAttackIDItemEnd; i++)
                {
                    if (gSC1PGameBonusAttackIDCount[i] != 0)
                    {
                        goto check_shieldbreaker;
                    }
                }
#if defined(REGION_US)
                if (gSC1PGameBonusDefendIDCount[nFTStatusAttackIDNull] != 0)
                {
                    goto check_shieldbreaker;
                }
                else
                {
#endif
                    // No Item
                    gSCManagerSceneData.bonus_get_mask[0] |= SC1PGAME_BONUS_MASK0_NOITEM;

#if defined(REGION_US)
                    if (sSC1PGameBonusStatPlayerKOsNum);
                }
#endif
            }
        }
    }
check_shieldbreaker:
    if (gSC1PGameBonusShieldBreaker != FALSE)
    {
        gSCManagerSceneData.bonus_get_mask[0] |= SC1PGAME_BONUS_MASK0_SHIELDBREAKER;
    }
    if ((gSC1PGameBonusDefendIDCount[nFTStatusAttackIDThrowF] != 0) || (gSC1PGameBonusDefendIDCount[nFTStatusAttackIDThrowB] != 0))
    {
        for (i = nFTStatusAttackIDAttackStart; i < ARRAY_COUNT(gSC1PGameBonusDefendIDCount); i++)
        {
            if ((i != nFTStatusAttackIDThrowF) && (i != nFTStatusAttackIDThrowB) && (gSC1PGameBonusDefendIDCount[i] != 0))
            {
                goto check_hawk;
            }
        }
        gSCManagerSceneData.bonus_get_mask[0] |= SC1PGAME_BONUS_MASK0_JUDOWARRIOR;
    }
check_hawk:
    if ((gSC1PGameBonusDefendGroundAirCount[nMPKineticsAir] != 0) && (gSC1PGameBonusDefendGroundAirCount[nMPKineticsGround] == 0))
    {
        // Hawk
        gSCManagerSceneData.bonus_get_mask[0] |= SC1PGAME_BONUS_MASK0_HAWK;
    }

    if ((gSC1PGameBonusDefendIsProjectileCount[TRUE] != 0) && (gSC1PGameBonusDefendIsProjectileCount[FALSE] == 0))
    {
        // Shooter
        gSCManagerSceneData.bonus_get_mask[0] |= SC1PGAME_BONUS_MASK0_SHOOTER;
    }
    for (i = nFTStatusAttackIDAttackStart; i < nFTStatusAttackIDJabStart; i++)
    {
        if (gSC1PGameBonusDefendIDCount[i] != 0)
        {
            continue;
        }
        else goto check_itemthrow;
    }
    switch (gSCManagerSceneData.fkind)
    {
    case nFTKindMario:
        if (gSC1PGameBonusDefendIDCount[nFTStatusAttackIDAttack12] == 0)
        {
            goto check_itemthrow;
        }
        else if (gSC1PGameBonusDefendIDCount[nFTStatusAttackIDAttack13] != 0)
        {
            break;
        }
        else goto check_itemthrow;

    case nFTKindFox:
        if (gSC1PGameBonusDefendIDCount[nFTStatusAttackIDAttack12] == 0)
        {
            goto check_itemthrow;
        }
        else if (gSC1PGameBonusDefendIDCount[nFTStatusAttackIDAttack100] != 0)
        {
            break;
        }
        else goto check_itemthrow;

    case nFTKindDonkey:
        if (gSC1PGameBonusDefendIDCount[nFTStatusAttackIDAttack12] != 0)
        {
            break;
        }
        else goto check_itemthrow;

    case nFTKindSamus:
        if (gSC1PGameBonusDefendIDCount[nFTStatusAttackIDAttack12] != 0)
        {
            break;
        }
        goto check_itemthrow;

    case nFTKindLuigi:
        if (gSC1PGameBonusDefendIDCount[nFTStatusAttackIDAttack12] == 0)
        {
            goto check_itemthrow;
        }
        else if (gSC1PGameBonusDefendIDCount[nFTStatusAttackIDAttack13] != 0)
        {
            break;
        }
        else goto check_itemthrow;

    case nFTKindLink:
        if (gSC1PGameBonusDefendIDCount[nFTStatusAttackIDAttack12] == 0)
        {
            goto check_itemthrow;
        }
        else if (gSC1PGameBonusDefendIDCount[nFTStatusAttackIDAttack13] == 0)
        {
            goto check_itemthrow;
        }
        else if (gSC1PGameBonusDefendIDCount[nFTStatusAttackIDAttack100] != 0)
        {
            break;
        }
        else goto check_itemthrow;

    case nFTKindYoshi:
        if (gSC1PGameBonusDefendIDCount[nFTStatusAttackIDAttack12] != 0)
        {
            break;
        }
        else goto check_itemthrow;

    case nFTKindCaptain:
        if (gSC1PGameBonusDefendIDCount[nFTStatusAttackIDAttack12] == 0)
        {
            goto check_itemthrow;
        }
        else if (gSC1PGameBonusDefendIDCount[nFTStatusAttackIDAttack13] == 0)
        {
            goto check_itemthrow;
        }
        else if (gSC1PGameBonusDefendIDCount[nFTStatusAttackIDAttack100] != 0)
        {
            break;
        }
        else goto check_itemthrow;

    case nFTKindKirby:
        if (gSC1PGameBonusDefendIDCount[nFTStatusAttackIDAttack12] == 0)
        {
            goto check_itemthrow;
        }
        else if (gSC1PGameBonusDefendIDCount[nFTStatusAttackIDAttack100] != 0)
        {
            break;
        }
        else goto check_itemthrow;

    case nFTKindPurin:
        if (gSC1PGameBonusDefendIDCount[nFTStatusAttackIDAttack12] != 0)
        {
            break;
        }
        goto check_itemthrow;

    case nFTKindNess:
        // This should also check Attack13, as Ness has a 3-hit jab.
        if (gSC1PGameBonusDefendIDCount[nFTStatusAttackIDAttack12] != 0)
        {
            break;
        }
        goto check_itemthrow;
    }
    // All Variations
    gSCManagerSceneData.bonus_get_mask[0] |= SC1PGAME_BONUS_MASK0_ALLVARIATIONS;

check_itemthrow:
    is_thrown_item = FALSE;

    for (i = nFTStatusAttackIDAttackStart; i < ARRAY_COUNT(gSC1PGameBonusDefendIDCount); i++)
    {
        if (gSC1PGameBonusDefendIDCount[i] != 0)
        {
            if (i == nFTMotionAttackIDItemThrow)
            {
                is_thrown_item = TRUE;
            }
            else goto check_smash;
        }
    }
    if (is_thrown_item != FALSE)
    {
        // Item Throw; why is this checking for Hammer usage?
        gSCManagerSceneData.bonus_get_mask[0] |= SC1PGAME_BONUS_MASK0_ITEMTHROW;
    }
check_smash:
    if ((gSC1PGameBonusDefendIsSmashCount[TRUE] != 0) && (gSC1PGameBonusDefendIsSmashCount[FALSE] == 0))
    {
        // Smash Mania
        gSCManagerSceneData.bonus_get_mask[1] |= SC1PGAME_BONUS_MASK1_SMASHMANIA;
    }

    if ((gSC1PGameBonusDefendIsSmashCount[TRUE] == 0) && (gSC1PGameBonusDefendIsSmashCount[FALSE] != 0))
    {
        // Smash-less
        gSCManagerSceneData.bonus_get_mask[1] |= SC1PGAME_BONUS_MASK1_SMASHLESS;
    }
    special_count = 0;

    for (i = nFTStatusAttackIDAttackStart; i < ARRAY_COUNT(gSC1PGameBonusDefendIDCount); i++)
    {
        if (gSC1PGameBonusDefendIDCount[i] != 0)
        {
            if ((i >= nFTStatusAttackIDSpecialStart) && (i <= nFTStatusAttackIDSpecialEnd))
            {
                special_count++;
            }
            else goto check_heavy_damage;
        }
    }
    if (special_count != 0)
    {
        if (special_count == 1)
        {
            // Single Move
            gSCManagerSceneData.bonus_get_mask[1] |= SC1PGAME_BONUS_MASK1_SINGLEMOVE;
        }
        else
        {
            // Special Move
            gSCManagerSceneData.bonus_get_mask[1] |= SC1PGAME_BONUS_MASK1_SPECIALMOVE;
        }
    }
check_heavy_damage:
#if defined(REGION_US)
    if (gSCManagerBattleState->players[gSCManagerSceneData.player].stock_damage_all >= 200)
#else
    if (gSCManagerBattleState->players[gSCManagerSceneData.player].stock_damage_all >= 300)
#endif
    {
        gSCManagerSceneData.bonus_get_mask[0] |= SC1PGAME_BONUS_MASK0_HEAVYDAMAGE;
    }
    if (sSC1PGameStageTimeSec <= 30)
    {
        // Speedster
        gSCManagerSceneData.bonus_get_mask[0] |= SC1PGAME_BONUS_MASK0_SPEEDSTER;
    }
    if (gSCManagerBattleState->players[gSCManagerSceneData.player].total_damage_given == 0)
    {
        // Pacifist
        gSCManagerSceneData.bonus_get_mask[0] |= SC1PGAME_BONUS_MASK0_PACIFIST;
    }
    if (gSC1PManagerTotalFalls == 0)
    {
        gSCManagerSceneData.bonus_get_mask[0] |= SC1PGAME_BONUS_MASK0_NOMISS;
    }
    if ((gSCManagerBattleState->players[gSCManagerSceneData.player].falls == 0) && (gSCManagerBattleState->players[gSCManagerSceneData.player].total_damage_all == 0))
    {
        // No Damage?
        gSCManagerSceneData.bonus_get_mask[0] |= SC1PGAME_BONUS_MASK0_NODAMAGE;
    }
    if (gSCManagerBattleState->players[gSCManagerSceneData.player].stock_damage_all == 0)
    {
        // Full Power?
        gSCManagerSceneData.bonus_get_mask[0] |= SC1PGAME_BONUS_MASK0_FULLPOWER;
    }
    if (sSC1PGameBonusStatInvincibleTimer != 0)
    {
        gSCManagerSceneData.bonus_get_mask[0] |= SC1PGAME_BONUS_MASK0_STARCLEAR;
    }
    if (gSC1PGameBonusTomatoCount >= 3)
    {
        // Vegetarian
        gSCManagerSceneData.bonus_get_mask[0] |= SC1PGAME_BONUS_MASK0_VEGETARIAN;
    }
    if (gSC1PGameBonusHeartCount >= 3)
    {
        // Heartthrob
        gSCManagerSceneData.bonus_get_mask[0] |= SC1PGAME_BONUS_MASK0_HEARTTHROB;
    }
    if (gSC1PGameBonusMewCatcher != FALSE)
    {
        // Mew Catcher
        gSCManagerSceneData.bonus_get_mask[0] |= SC1PGAME_BONUS_MASK0_MEWCATCHER;
    }
    if
    (
        (sSC1PGameBonusStatPlayerKOsNum != 0) &&
        (sSC1PGameBonusStatEnemyStats[sSC1PGameBonusStatPlayerKOsNum - 1].damage_player == gSCManagerSceneData.player) &&
        (
            (
                (sSC1PGameBonusStatEnemyStats[sSC1PGameBonusStatPlayerKOsNum - 1].damage_object_class == nFTHitLogObjectItem) &&
                (sSC1PGameBonusStatEnemyStats[sSC1PGameBonusStatPlayerKOsNum - 1].damage_object_kind >= nITKindMBallMonsterStart) &&
                (sSC1PGameBonusStatEnemyStats[sSC1PGameBonusStatPlayerKOsNum - 1].damage_object_kind <= nITKindMBallMonsterEnd)
            )
            ||
            (
                (sSC1PGameBonusStatEnemyStats[sSC1PGameBonusStatPlayerKOsNum - 1].damage_object_class == nFTHitLogObjectWeapon) &&
                (sSC1PGameBonusStatEnemyStats[sSC1PGameBonusStatPlayerKOsNum - 1].damage_object_kind >= nWPKindMonsterStart) &&
                (sSC1PGameBonusStatEnemyStats[sSC1PGameBonusStatPlayerKOsNum - 1].damage_object_kind <= nWPKindMonsterEnd)
            )
        )
    )
    {
        // Pokémon Finish
        gSCManagerSceneData.bonus_get_mask[1] |= SC1PGAME_BONUS_MASK1_POKEMONFINISH;
    }
    if
    (
        (sSC1PGameBonusStatPlayerKOsNum != 0) &&
        (sSC1PGameBonusStatEnemyStats[sSC1PGameBonusStatPlayerKOsNum - 1].damage_player == gSCManagerSceneData.player) &&
        (sSC1PGameBonusStatEnemyStats[sSC1PGameBonusStatPlayerKOsNum - 1].damage_object_class == nFTHitLogObjectItem) &&
        (sSC1PGameBonusStatEnemyStats[sSC1PGameBonusStatPlayerKOsNum - 1].damage_object_kind == nITKindMSBomb) &&
        (sSC1PGameBonusStatEnemyStats[sSC1PGameBonusStatPlayerKOsNum - 1].damage_stat_flags.attack_id == nFTStatusAttackIDNull)
    )
    {
        // Booby Trap
        gSCManagerSceneData.bonus_get_mask[1] |= SC1PGAME_BONUS_MASK1_BOOBYTRAP;
    }
    if
    (
        (sSC1PGameBonusStatPlayerKOsNum != 0) &&
        (sSC1PGameBonusStatEnemyStats[sSC1PGameBonusStatPlayerKOsNum - 1].damage_player == gSCManagerSceneData.player) &&
        (
            (sSC1PGameBonusStatEnemyStats[sSC1PGameBonusStatPlayerKOsNum - 1].damage_stat_flags.attack_id == nFTStatusAttackIDThrowF) ||
            (sSC1PGameBonusStatEnemyStats[sSC1PGameBonusStatPlayerKOsNum - 1].damage_stat_flags.attack_id == nFTStatusAttackIDThrowB)
        )
    )
    {
        // Throw Down
        gSCManagerSceneData.bonus_get_mask[0] |= SC1PGAME_BONUS_MASK0_THROWDOWN;
    }
    if (sSC1PGameBonusStatEndPlayerStatus == nFTCommonStatusAppeal)
    {
        // Fighter Stance
        gSCManagerSceneData.bonus_get_mask[1] |= SC1PGAME_BONUS_MASK1_FIGHTERSTANCE;
    }
    if ((sSC1PGameBonusStatEndPlayerStatus == nFTCommonStatusDeadDown) || (sSC1PGameBonusStatEndPlayerStatus == nFTCommonStatusDeadLeftRight))
    {
        // Mystic
        gSCManagerSceneData.bonus_get_mask[1] |= SC1PGAME_BONUS_MASK1_MYSTIC;
    }
    if (sSC1PGameBonusStatEndPlayerStatus == nFTCommonStatusDeadUpStar)
    {
        // Comet Mystic
        gSCManagerSceneData.bonus_get_mask[1] |= SC1PGAME_BONUS_MASK1_COMETMYSTIC;
    }
    if
    (
        (sSC1PGameBonusStatPlayerKOsNum != 0) &&
        (sSC1PGameBonusStatEnemyStats[sSC1PGameBonusStatPlayerKOsNum - 1].damage_player != -1) &&
        (sSC1PGameBonusStatEnemyStats[sSC1PGameBonusStatPlayerKOsNum - 1].damage_object_class == nFTHitLogObjectGround) &&
        (sSC1PGameBonusStatEnemyStats[sSC1PGameBonusStatPlayerKOsNum - 1].damage_object_kind == nGMHitEnvironmentAcid)
    )
    {
        // Acid Clear
        gSCManagerSceneData.bonus_get_mask[1] |= SC1PGAME_BONUS_MASK1_ACIDCLEAR;
    }
    if
    (
        (
            (sSC1PGameBonusStatPlayerKOsNum != 0) &&
            (sSC1PGameBonusStatEnemyStats[sSC1PGameBonusStatPlayerKOsNum - 1].damage_player != -1) &&
            (sSC1PGameBonusStatEnemyStats[sSC1PGameBonusStatPlayerKOsNum - 1].damage_object_class == nFTHitLogObjectItem) &&
            (sSC1PGameBonusStatEnemyStats[sSC1PGameBonusStatPlayerKOsNum - 1].damage_object_kind == nITKindGBumper)
        )
        ||
        (
            (sSC1PGameBonusStatEnemyStats[sSC1PGameBonusStatPlayerKOsNum - 1].damage_player == gSCManagerSceneData.player) &&
            (sSC1PGameBonusStatEnemyStats[sSC1PGameBonusStatPlayerKOsNum - 1].damage_object_class == nFTHitLogObjectItem) &&
            (sSC1PGameBonusStatEnemyStats[sSC1PGameBonusStatPlayerKOsNum - 1].damage_object_kind == nITKindNBumper)
        )
    )
    {
        // Bumper Clear
        gSCManagerSceneData.bonus_get_mask[1] |= SC1PGAME_BONUS_MASK1_BUMPERCLEAR;
    }

    if
    (
        (sSC1PGameBonusStatPlayerKOsNum != 0) &&
        (sSC1PGameBonusStatEnemyStats[sSC1PGameBonusStatPlayerKOsNum - 1].damage_player != -1) &&
        (sSC1PGameBonusStatEnemyStats[sSC1PGameBonusStatPlayerKOsNum - 1].damage_object_class == nFTHitLogObjectGround) &&
        (sSC1PGameBonusStatEnemyStats[sSC1PGameBonusStatPlayerKOsNum - 1].damage_object_kind == nGMHitEnvironmentTwister)
    )
    {
        // Twister Clear
        gSCManagerSceneData.bonus_get_mask[1] |= SC1PGAME_BONUS_MASK1_TORNADOCLEAR;
    }
    if
    (
        (sSC1PGameBonusStatPlayerKOsNum != 0) &&
        (sSC1PGameBonusStatEnemyStats[sSC1PGameBonusStatPlayerKOsNum - 1].damage_player != -1) &&
        (sSC1PGameBonusStatEnemyStats[sSC1PGameBonusStatPlayerKOsNum - 1].damage_object_class == nFTHitLogObjectWeapon) &&
        (
            (sSC1PGameBonusStatEnemyStats[sSC1PGameBonusStatPlayerKOsNum - 1].damage_object_kind == nWPKindArwingLaser2D) ||
            (sSC1PGameBonusStatEnemyStats[sSC1PGameBonusStatPlayerKOsNum - 1].damage_object_kind == nWPKindArwingLaser3D)
        )
    )
    {
        // Arwing Clear
        gSCManagerSceneData.bonus_get_mask[1] |= SC1PGAME_BONUS_MASK1_ARWINGCLEAR;
    }
    if ((gSCManagerBattleState->time_limit != SCBATTLE_TIMELIMIT_INFINITE) && (gSCManagerSceneData.spgame_time_remain == 1))
    {
        // Last Second
        gSCManagerSceneData.bonus_get_mask[1] |= SC1PGAME_BONUS_MASK1_LASTSECOND;
    }
    if ((gSCManagerBattleState->time_limit != SCBATTLE_TIMELIMIT_INFINITE) && (gSCManagerSceneData.spgame_time_remain == 213))
    {
        // Lucky 3
        gSCManagerSceneData.bonus_get_mask[1] |= SC1PGAME_BONUS_MASK1_LUCKY3;
    }
    damage_jackpot_bak = gSCManagerBattleState->players[gSCManagerSceneData.player].stock_damage_all;

    is_jackpot = FALSE;

    damage_jackpot_div = damage_jackpot_bak / 100;

    damage_jackpot_bak -= (damage_jackpot_div * 100);

    damage_jackpot_mod = damage_jackpot_bak;

    if (damage_jackpot_div != 0)
    {
        if (damage_jackpot_div == (damage_jackpot_mod / 10))
        {
            if ((damage_jackpot_mod - ((damage_jackpot_mod / 10) * 10)) == (damage_jackpot_mod / 10))
            {
                is_jackpot = TRUE;
            }
        }
    }
    else if ((damage_jackpot_mod / 10) != 0)
    {
        if ((damage_jackpot_mod - ((damage_jackpot_mod / 10) * 10)) == (damage_jackpot_mod / 10))
        {
            is_jackpot = TRUE;
        }
    }
    if (is_jackpot != FALSE)
    {
        // Jackpot
        gSCManagerSceneData.bonus_get_mask[1] |= SC1PGAME_BONUS_MASK1_JACKPOT;
    }
    if ((gSCManagerSceneData.spgame_stage == nSC1PGameStageYoshi) && (sSC1PGameEnemyStocksRemaining == 0))
    {
        is_ordered_variation = TRUE;

        for (i = 0; i < SC1PGAME_STAGE_YOSHI_VARIATIONS_COUNT; i++)
        {
            variation_order[i] = sSC1PGameEnemyVariations[sSC1PGameBonusStatEnemyStats[i].team_order];
        }
        for (variation = 0; i < sSC1PGameBonusStatPlayerKOsNum; i++, variation = (variation == (SC1PGAME_STAGE_YOSHI_VARIATIONS_COUNT - 1)) ? 0 : variation++)
        {
            if (sSC1PGameEnemyVariations[sSC1PGameBonusStatEnemyStats[i].team_order] != variation_order[variation])
            {
                is_ordered_variation = FALSE;
            }
        }
        if (is_ordered_variation != FALSE)
        {
            // Yoshi Rainbow
            gSCManagerSceneData.bonus_get_mask[1] |= SC1PGAME_BONUS_MASK1_YOSHIRAINBOW;
        }
    }
    if ((gSCManagerSceneData.spgame_stage == nSC1PGameStageKirby) && (sSC1PGameEnemyStocksRemaining == 0))
    {
        is_ordered_variation = TRUE;

        for (i = 0; i < SC1PGAME_STAGE_KIRBY_VARIATIONS_COUNT; i++)
        {
            if (dSC1PGameKirbyTeamCopyKinds[sSC1PGameBonusStatEnemyStats[i].team_order] != dSC1PGameKirbyTeamCopyKinds[i])
            {
                is_ordered_variation = FALSE;
            }
        }
        if (is_ordered_variation != FALSE)
        {
            // Kirby Ranks
            gSCManagerSceneData.bonus_get_mask[1] |= SC1PGAME_BONUS_MASK1_KIRBYRANKS;
        }
    }
    if (gSCManagerSceneData.spgame_stage == nSC1PGameStageMario)
    {
        if (gSCManagerBattleState->players[gSCManagerSceneData.ally_players[0]].falls == 0)
        {
            if (gSCManagerBattleState->players[gSCManagerSceneData.ally_players[0]].total_damage_all == 0)
            {
                // True Friend
                gSCManagerSceneData.bonus_get_mask[1] |= SC1PGAME_BONUS_MASK1_TRUEFRIEND;
            }
            else
            {
                // Good Friend
                gSCManagerSceneData.bonus_get_mask[1] |= SC1PGAME_BONUS_MASK1_GOODFRIEND;
            }
        }
        if (gSC1PGameBonusBrosCalamity != FALSE)
        {
            // Bros. Calamity
            gSCManagerSceneData.bonus_get_mask[1] |= SC1PGAME_BONUS_MASK1_BROSCALAMITY;
        }
    }
    if ((gSCManagerSceneData.spgame_stage == nSC1PGameStageDonkey) && (gSCManagerBattleState->players[gSCManagerSceneData.ally_players[0]].falls == 0) && (gSCManagerBattleState->players[gSCManagerSceneData.ally_players[1]].falls == 0))
    {
        if ((gSCManagerBattleState->players[gSCManagerSceneData.ally_players[0]].total_damage_all == 0) && (gSCManagerBattleState->players[gSCManagerSceneData.ally_players[1]].total_damage_all == 0))
        {
            // DK Perfect
            gSCManagerSceneData.bonus_get_mask[1] |= SC1PGAME_BONUS_MASK1_DKPERFECT;
        }
        else
        {
            // DK Defender
            gSCManagerSceneData.bonus_get_mask[1] |= SC1PGAME_BONUS_MASK1_DKDEFENDER;
        }
    }
    if (gSCManagerSceneData.spgame_stage == nSC1PGameStageBoss)
    {
        gSCManagerSceneData.bonus_get_mask[0] |= SC1PGAME_BONUS_MASK0_GAMECLEAR;

        if (gSC1PManagerTotalFalls == 0)
        {
            // No Miss Clear
            gSCManagerSceneData.bonus_get_mask[0] |= SC1PGAME_BONUS_MASK0_NOMISSCLEAR;
        }
        if (gSC1PManagerTotalDamage == 0)
        {
            // No Damage Clear
            gSCManagerSceneData.bonus_get_mask[0] |= SC1PGAME_BONUS_MASK0_NODAMAGECLEAR;
        }
        if (sSC1PGameTotalTimeSec <= I_MIN_TO_SEC(8))
        {
            // Speed Demon
            gSCManagerSceneData.bonus_get_mask[0] |= SC1PGAME_BONUS_MASK0_SPEEDDEMON;
        }
        else if (sSC1PGameTotalTimeSec <= I_MIN_TO_SEC(12)) // 12 minutes? Wiki claims this should be 20 minutes
        {
            // Speed King
            gSCManagerSceneData.bonus_get_mask[0] |= SC1PGAME_BONUS_MASK0_SPEEDKING;
        }
    }
    if
    (
        !(gSCManagerSceneData.bonus_get_mask[1] & SC1PGAME_BONUS_MASK1_SINGLEMOVE)   &&
        !(gSCManagerSceneData.bonus_get_mask[1] & SC1PGAME_BONUS_MASK1_SPECIALMOVE)  &&
        !(gSCManagerSceneData.bonus_get_mask[0] & SC1PGAME_BONUS_MASK0_HAWK)         &&
        !(gSCManagerSceneData.bonus_get_mask[0] & SC1PGAME_BONUS_MASK0_JUDOWARRIOR)  &&
        !(gSCManagerSceneData.bonus_get_mask[0] & SC1PGAME_BONUS_MASK0_SHOOTER)      &&
        !(gSCManagerSceneData.bonus_get_mask[0] & SC1PGAME_BONUS_MASK0_ALLVARIATIONS)&&
        !(gSCManagerSceneData.bonus_get_mask[0] & SC1PGAME_BONUS_MASK0_ITEMSTRIKE)   &&
        !(gSCManagerSceneData.bonus_get_mask[0] & SC1PGAME_BONUS_MASK0_TRICKSTER)    &&
        !(gSCManagerSceneData.bonus_get_mask[0] & SC1PGAME_BONUS_MASK0_ITEMTHROW)    &&
        !(gSCManagerSceneData.bonus_get_mask[0] & SC1PGAME_BONUS_MASK0_PACIFIST)     &&
        !(gSCManagerSceneData.bonus_get_mask[1] & SC1PGAME_BONUS_MASK1_SMASHMANIA)
    )
    {
        for (attack_count = 0, i = nFTStatusAttackIDAttackStart; i < ARRAY_COUNT(gSC1PGameBonusAttackIDCount); i++)
        {
            attack_count += gSC1PGameBonusAttackIDCount[i];
        }
        if (attack_count != 0)
        {
            for (i = nFTStatusAttackIDAttackStart; i < ARRAY_COUNT(gSC1PGameBonusAttackIDCount); i++)
            {
                if ((gSC1PGameBonusAttackIDCount[i] / (f32)attack_count) >= F_PCT_TO_DEC(35.0F))
                {
                    // Cheap Shot
                    gSCManagerSceneData.bonus_get_mask[0] |= SC1PGAME_BONUS_MASK0_CHEAPSHOT;

                    break;
                }
            }
        }
    }
}

// 0x80190E58
void sc1PGameInitBonusStats(void)
{
    gSCManagerSceneData.bonus_get_mask[0] = gSCManagerSceneData.bonus_get_mask[1] = gSCManagerSceneData.bonus_get_mask[2] = 0;

    gSCManagerSceneData.spgame_time_remain = (gSCManagerBattleState->time_limit == SCBATTLE_TIMELIMIT_INFINITE) ? 0 : I_TICS_TO_SEC(gSCManagerBattleState->time_remain + 59);

    gSC1PManagerTotalTimeTics += gSCManagerBattleState->time_passed;
    gSC1PManagerTotalFalls += gSCManagerBattleState->players[gSCManagerSceneData.player].falls;
    gSC1PManagerTotalDamage += gSCManagerBattleState->players[gSCManagerSceneData.player].total_damage_all;

    switch (gSCManagerSceneData.spgame_stage)
    {
    case nSC1PGameStageBonus3:
        if ((gSCManagerBattleState->players[gSCManagerSceneData.player].falls == 0) && (gSCManagerSceneData.spgame_time_remain != 0) && (gSCManagerBattleState->players[gSCManagerSceneData.player].total_damage_all == 0))
        {
            gSCManagerSceneData.bonus_get_mask[0] |= SC1PGAME_BONUS_MASK0_NODAMAGE;
        }
        break;

    default:
        gSCManagerSceneData.spgame_score += gSCManagerBattleState->players[gSCManagerSceneData.player].score * 1000;
        sc1PGameAppendBonusStats();
        break;
    }
}

// 0x80190F8C
void sc1PGameFuncLights(Gfx **dls)
{
    gSPSetGeometryMode(dls[0]++, G_LIGHTING);

    ftDisplayLightsDrawReflect(dls, gMPCollisionLightAngleX, gMPCollisionLightAngleY);
}

// 0x80190FD8
void sc1PGameStartScene(void)
{
    gSCManagerBattleState = &gSCManager1PGameBattleState;
    gSCManagerBattleState->game_type = nSCBattleGameType1PGame;

    dSC1PGameVideoSetup.zbuffer = SYVIDEO_ZBUFFER_START(320, 240, 0, 10, u16);
    syVideoInit(&dSC1PGameVideoSetup);

    dSC1PGameTaskmanSetup.scene_setup.arena_size = (size_t) ((uintptr_t)&gSYFramebufferSets - (uintptr_t)&ovl65_BSS_END);
    dSC1PGameTaskmanSetup.func_start = sc1PGameFuncStart;
    scManagerFuncUpdate(&dSC1PGameTaskmanSetup);
    
    sc1PGameInitBonusStats();
    syAudioStopBGMAll();

    while (syAudioCheckBGMPlaying(0) != FALSE)
    {
        continue;
    }
    syAudioSetBGMVolume(0, 0x7800);
    func_800266A0_272A0();
    gmRumbleInitPlayers();
}
