#ifndef _OBJDEF_H_
#define _OBJDEF_H_

typedef enum GObjKind
{
    nGCCommonKindFighter = 1000,            // 1000
    nGCCommonKindFighterParts,              // 1001
    nGCCommonKindMainCamera = 1002,         // 1002
    nGCCommonKindWallpaperCamera,           // 1003 - Used to render stage backgrounds?
    nGCCommonKindScissorCamera,             // 1004 - Camera scissor?
    nGCCommonKindPlayerMagnifyCamera,                // 1005 - ???
    nGCCommonKindPlayerArrowsCamera,                // 1006 - ???
    nGCCommonKindUnkCamera3,                // 1007
    nGCCommonKindWallpaper,                 // 1008 - Stage background?
    nGCCommonKindGroundDisplay,             // 1009 - Stage renderer?
    nGCCommonKindGround,                    // 1010
    nGCCommonKindEffect,                    // 1011
    nGCCommonKindWeapon,                    // 1012
    nGCCommonKindItem,                      // 1013
    nGCCommonKindMovie = 1015,              // 1015 - Opening movie?
    nGCCommonKindInterface,                 // 1016
    nGCCommonKindRumble,                    // 1017
    nGCCommonKindPublic,                    // 1018 - Audience reaction
    nGCCommonKindPauseMenu,                 // 1019
    nGCCommonKindShadow,                    // 1020
    nGCCommonKindTransition,                // 1021 - Screen fadeout actor?
    nGCCommonKindBossWallpaper = 1023,      // 1023
    nGCCommonKindPlayerSelect,              // 1024
    nGCCommonKindSceneCamera                // 1025

} GObjKind;

typedef enum GObjProcessKind
{
    nGCProcessKindThread,
    nGCProcessKindFunc,
    nGCProcessKindEnumCount

} GObjProcessKind;

typedef enum GObjObjKind
{
    nGCCommonAppendNone,
    nGCCommonAppendDObj,
    nGCCommonAppendSObj,
    nGCCommonAppendCamera,
    nGCCommonAppendEnumCount

} GObjObjKind;

typedef enum GObjEventKind
{
    nGCCommonEventShow,
    nGCCommonEventHide,
    nGCCommonEventResume,
    nGCCommonEventPause,
    nGCCommonEventEnd

} GObjEventKind;

typedef enum GObjLinkID
{
    // These assume that the scene has ovl3 loaded.
    // Scenes without ovl3 seem to not follow any consistent link indexing convention.

    // GObj link indexes with ID 1
    nGCCommonLinkIDGround = 1,          // ID 1: Stage object

    nGCCommonLinkIDGroundDisplay = 2,   // ID 2: Stage renderer?
    nGCCommonLinkIDItemActor = 2,       // ID 2: Item spawn actor
    nGCCommonLinkID02 = 2,              // ID 2: ???

    // GObj link indexes with ID 3
    nGCCommonLinkIDFighter = 3,         // ID 3: Fighter object
    nGCCommonLinkIDCreditsName = 3,     // ID 3: Credits staff roll name

    // GObj link indexes with ID 4
    nGCCommonLinkIDItem = 4,            // ID 4: Item object
    nGCCommonLinkIDCreditsJob = 4,      // ID 4: Credits staff roll job

    nGCCommonLinkIDWeapon = 5,          // ID 5: Weapon (projectile) object
    nGCCommonLinkIDEffect,              // ID 6: Effect type 1 object (other ID is 8 but idk what it does)
    nGCCommonLinkIDSpecialEffect = 8,   // ID 8: Effect type 2 object
    nGCCommonLinkIDHighlight = 9,       // ID 9: Credits shootout rectangles that zoom in on crosshair
    nGCCommonLinkIDCamera = 9,          // ID 9: Cameras?
    nGCCommonLinkIDInterfaceActor = 10, // ID 10: Traffic lamp on match start
    nGCCommonLinkIDInterface = 11,      // ID 11: HUD elements object
    nGCCommonLinkIDMagnify = 12,        // ID 12: HUD magnifying glass

    nGCCommonLinkIDMovie = 13,          // ID 13: Movie?
    nGCCommonLinkIDRumble = 13,         // ID 13: Rumble object?
    nGCCommonLinkIDWallpaper = 13,      // ID 13: Stage background object?
    nGCCommonLinkIDWallpaperEffect=13,  // ID 13: Stage background effect
    nGCCommonLinkIDFighterParts = 13,   // ID 13: FighterParts
    nGCCommonLinkIDShadow = 13,         // ID 13: Fighter shadows
    nGCCommonLinkIDTransition = 13,     // ID 13: Screen fadeout transition

    nGCCommonLinkIDPauseMenu = 14,      // ID 14: Pause menu object?
    nGCCommonLinkIDPlayerSelect = 15,   // ID 15: Player Select menu?
    nGCCommonLinkIDSceneCamera = 16     // ID 16: Scene camera?

} GObjLinkID;

typedef enum DObjVectorKind
{
    nGCDrawVectorKindNone,
    nGCDrawVectorKindTranslate,
    nGCDrawVectorKindRotate,
    nGCDrawVectorKindScale,
    nGCDrawVectorKindEnumCount

} DObjVectorKind;

typedef enum XObjTransformKind
{
    nGCMatrixKindNull,
    nGCMatrixKindPerspFastF = 3,     // Camera Fast FP perspective projection
    nGCMatrixKindPerspF,             // Camera FP perspective projection
    nGCMatrixKindOrtho,              // Camera Orthographic projection
    nGCMatrixKindLookAt,
    nGCMatrixKindTra = 18,           // DObj Translate
    nGCMatrixKindRotD,               // DObj Rotate degrees
    nGCMatrixKindTraRotD,            // DObj Translate + rotate degrees
    nGCMatrixKindRotRpyD,            // DObj Rotate Roll/Pitch/Yaw degrees
    nGCMatrixKindTraRotRpyD,         // DObj Translate + rotate Roll/Pitch/Yaw degrees
    nGCMatrixKindRotR,               // DObj Rotate radians (?)
    nGCMatrixKindTraRotR,            // DObj Translate + rotate radians
    nGCMatrixKindTraRotRSca,         // DObj Translate + rotate radians + scale
    nGCMatrixKindRotRpyR,            // DObj Rotate Roll/Pitch/Yaw radians
    nGCMatrixKindTraRotRpyR,         // DObj Translate + rotate Roll/Pitch/Yaw radians
    nGCMatrixKindTraRotRpyRSca,      // DObj Translate + rotate Roll/Pitch/Yaw radians + rowscale (?)
    nGCMatrixKindRotPyrR,            // DObj Rotate Pitch/Yaw/Roll radians (?)
    nGCMatrixKindTraRotPyrR,         // DObj Translate + rotate Pitch/Yaw/Roll radians (?)
    nGCMatrixKindTraRotPyrRSca,      // DObj Translate + rotate Pitch/Yaw/Roll radians (?) + scale
    nGCMatrixKindSca,                // DObj Scale
    nGCMatrixKindRecalcRotPyrR = 41, // MVP Recalc Pitch/Yaw/Roll radians (?)
    nGCMatrixKindRecalcRotRpyR,      // MVP Recalc Roll/Pitch/Yaw radians (?)
    nGCMatrixKindRecalcRotPyrRSca,   // MVP Recalc Pitch/Yaw/Roll radians + scale (?)
    nGCMatrixKindRecalcRotRpyRSca,   // MVP Recalc Roll/Pitch/Yaw radians + scale (?)
    nGCMatrixKind45,
    nGCMatrixKind46,
    nGCMatrixKind47,
    nGCMatrixKind48,
    nGCMatrixKind49,
    nGCMatrixKind50,
    nGCMatrixKindVecTra = 56,        // Standalone (?) vector translate
    nGCMatrixKindVecRotR,            // Standalone (?) vector rotate radians
    nGCMatrixKindVecRotRpyR,         // Standalone (?) vector rotate Roll/Pitch/Yaw radians
    nGCMatrixKindVecSca,             // Standalone (?) vector scale
    nGCMatrixKindVecTraRotR,         // Standalone (?) vector translate + rotate radians
    nGCMatrixKindVecTraRotRSca,      // Standalone (?) vector translate + rotate radians + rowscale (?)
    nGCMatrixKindVecTraRotRpyR,      // Standalone (?) vector translate + rotate Roll/Pitch/Yaw radians
    nGCMatrixKindVecTraRotRpyRSca    // Standalone (?) vector translate + rotate Roll/Pitch/Yaw radians + rowscale (?) 

} XObjTransformKind;

typedef enum AObjAnimKind
{
    nGCAnimKindNone, 
    nGCAnimKindStep,
    nGCAnimKindLinear, 
    nGCAnimKindCubic,
    nGCAnimKindSpecial

} AObjAnimKind;

typedef enum AObjEvent16Kind
{
    nGCAnimEvent16End,
    nGCAnimEvent16Block,
    nGCAnimEvent16SetValBlock,
    nGCAnimEvent16SetVal,
    nGCAnimEvent16SetValRateBlock,
    nGCAnimEvent16SetValRate,
    nGCAnimEvent16SetTargetRate,
    nGCAnimEvent16SetVal0RateBlock,
    nGCAnimEvent16SetVal0Rate,
    nGCAnimEvent16SetValAfterBlock,
    nGCAnimEvent16SetValAfter,
    nGCAnimEvent1611,
    nGCAnimEvent16SetTranslateInterp,
    nGCAnimEvent16Loop,
    nGCAnimEvent16SetFlags

} AObjEvent16Kind;

typedef enum AObjEvent32Kind
{
    nGCAnimEvent32End,
    nGCAnimEvent32Jump,
    nGCAnimEvent32Wait,
    nGCAnimEvent32SetValBlock,
    nGCAnimEvent32SetVal,
    nGCAnimEvent32SetValRateBlock,
    nGCAnimEvent32SetValRate,
    nGCAnimEvent32SetTargetRate,
    nGCAnimEvent32SetVal0RateBlock,
    nGCAnimEvent32SetVal0Rate,
    nGCAnimEvent32SetValAfterBlock,
    nGCAnimEvent32SetValAfter,
    ANIM_CMD_12 = 12,
    nGCAnimEvent32SetInterp = 13,
    nGCAnimEvent32SetAnim,
    nGCAnimEvent32SetFlags,
    ANIM_CMD_16 = 16,
    ANIM_CMD_17 = 17,
    nGCAnimEvent32SetExtValAfterBlock,
    nGCAnimEvent32SetExtValAfter,
    nGCAnimEvent32SetExtValBlock,
    nGCAnimEvent32SetExtVal,
    ANIM_CMD_22 = 22,
    ANIM_CMD_23 = 23

} AObjEvent32Kind;

typedef enum AObjTrackKind
{
    // model
    nGCAnimTrackNone,
    nGCAnimTrackJointStart,                     // Start of joint tracks
    nGCAnimTrackRotX = nGCAnimTrackJointStart,  // Rotation X (Roll)
    nGCAnimTrackRotY,                           // Rotation Y (Pitch)
    nGCAnimTrackRotZ,                           // Rotation Z (Yaw)
    nGCAnimTrackTraI,                           // Translation cubic interpolation?
    nGCAnimTrackTraX,                           // Translation X 
    nGCAnimTrackTraY,                           // Translation Y
    nGCAnimTrackTraZ,                           // Translation Z
    nGCAnimTrackScaX,                           // Scale X
    nGCAnimTrackScaY,                           // Scale Y
    nGCAnimTrackScaZ,                           // Scale Z
    nGCAnimTrackJointEnd = nGCAnimTrackScaZ,    // End of joint tracks
    // texture
    nGCAnimTrackMaterialStart = 13,
    nGCAnimTrackTextureIDCurrent = nGCAnimTrackMaterialStart,
    nGCAnimTrackTraU,
    nGCAnimTrackTraV,
    nGCAnimTrackScaU,
    nGCAnimTrackScaV,
    nGCAnimTrackTextureIDNext,
    nGCAnimTrackScrU,
    nGCAnimTrackScrV,
    nGCAnimTrackSetLFrac,
    nGCAnimTrackPaletteID,
    nGCAnimTrackMaterialEnd = nGCAnimTrackPaletteID,
    // camera
    nGCAnimTrackCameraStart = 25,
    nGCAnimTrackEyeX = nGCAnimTrackCameraStart,
    nGCAnimTrackEyeY,
    nGCAnimTrackEyeZ,
    nGCAnimTrackEyeI,
    nGCAnimTrackAtX,
    nGCAnimTrackAtY,
    nGCAnimTrackAtZ,
    nGCAnimTrackAtI,
    nGCAnimTrackUpX,
    nGCAnimTrackFovY,
    nGCAnimTrackCameraEnd = nGCAnimTrackFovY,
    // texture extra
    nGCAnimTrackMaterialSubStart = 37,
    nGCAnimTrackPrimColor = nGCAnimTrackMaterialSubStart,
    nGCAnimTrackEnvColor,
    nGCAnimTrackBlendColor,
    nGCAnimTrackLight1Color,
    nGCAnimTrackLight2Color,
    nGCAnimTrackMaterialSubEnd = nGCAnimTrackLight2Color

} AObjTrackKind;

typedef enum GCStatus
{
    nGCStatusSystem,
    nGCStatusRunning,               // Processing current GObj's func_run
    nGCStatusProcessing,            // Processing current GObjProcess
    nGCStatusCapturing,             // Processing current CObj's viewport?
    nGCStatusDisplaying             // Processing current CObj and DObj's rendering?

} GCStatus;

typedef enum GCRunStatus
{
    nGCRunStatusDefault,
    nGCRunStatusEnd,
    nGCRunStatusEject
    
} GCRunStatus;

typedef union  AObjEvent16          AObjEvent16;
typedef union  AObjEvent32          AObjEvent32;
typedef union  AObjScript           AObjScript;
typedef struct GObj                 GObj;
typedef struct GObjScript           GObjScript;
typedef struct GObjProcess          GObjProcess;
typedef struct GObjThread           GObjThread;
typedef struct GObjThreadStack      GObjThreadStack;
typedef struct GObjStack            GObjStack;
typedef struct DObj                 DObj;
typedef struct DObjDesc             DObjDesc;
typedef struct DObjTraDesc          DObjTraDesc;
typedef struct DObjTransformTypes   DObjTransformTypes;
typedef struct DObjVec              DObjVec;
typedef struct DObjDLLink           DObjDLLink;
typedef struct DObjDistDL           DObjDistDL;
typedef struct DObjDistDLLink       DObjDistDLLink;
typedef struct DObjMultiList        DObjMultiList;
typedef struct AObj                 AObj;
typedef struct MObj                 MObj;
typedef struct MObjSub              MObjSub;
typedef struct SObj                 SObj;
typedef struct CObj                 CObj;
typedef struct CObjVec              CObjVec;
typedef struct CObjDesc             CObjDesc;
typedef struct XObj                 XObj;
typedef struct GCGfxLink            GCGfxLink;
typedef struct GCPersp              GCPersp;
typedef struct GCFrustum            GCFrustum;
typedef struct GCOrtho              GCOrtho;
typedef struct GCTranslate          GCTranslate;
typedef struct GCRotate             GCRotate;
typedef struct GCScale              GCScale;
typedef union  GCUserData           GCUserData;
typedef struct GCSetup              GCSetup;

#endif
