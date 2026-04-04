#ifndef _FTDEF_H_
#define _FTDEF_H_

#define FTCOMPUTER_LEVEL_MAX 9
#define FTCOMMON_HANDICAP_DEFAULT 9

#define FTDISPLAY_DLLINK_DEFAULT 9

#define FTPARTS_HURT_NUM_MAX 11
#define FTPARTS_JOINT_NUM_MAX 37
#define FTPARTS_FLAG_NOFOG 0x40
#define FTPARTS_FLAG_TOGGLEFOG 0x80

#define FTPHYSICS_AIRDRIFT_CLAMP_RANGE_MIN  8 // Default minimum stick range required to clamp air drift in respective physics routine

#define FTINPUT_STICKBUFFER_TICS_MAX      (U8_MAX - 1)
#define FTINPUT_ZTRIGLAST_TICS_MAX        (U16_MAX + 1)

#define FTSTAT_CHARDATA_START 0x20000
#define FTSTAT_OPENING1_START 0x1000F
#define FTSTAT_OPENING2_START 0x10000

#define FTDATA_FLAG_SUBMOTION               0x1
#define FTDATA_FLAG_MAINMOTION              0x2

#define FTMOTION_SCRIPT_NULL                0x80000000

#define FTANIM_FLAG_TRANSN_JOINT            0x80000000
#define FTANIM_FLAG_XROTN_JOINT             0x40000000
#define FTANIM_FLAG_YROTN_JOINT             0x20000000

#define FTANIM_FLAG_SUBMOTION_SCRIPT        0x00000010
#define FTANIM_FLAG_ANIMJOINT               0x00000008
#define FTANIM_FLAG_TRANSLATE_SCALES        0x00000004
#define FTANIM_FLAG_SHIELDPOSE              0x00000002
#define FTANIM_FLAG_ANIMLOCKS               0x00000001
#define FTANIM_FLAG_NONE                    0x00000000

#define FTSTATUS_PRESERVE_NONE              (0)                                     // 0x0 - Just zero
#define FTSTATUS_PRESERVE_HIT               (1 << nFTStatusPreserveHit)             // 0x1
#define FTSTATUS_PRESERVE_COLANIM           (1 << nFTStatusPreserveColAnim)         // 0x2
#define FTSTATUS_PRESERVE_EFFECT            (1 << nFTStatusPreserveEffect)          // 0x4
#define FTSTATUS_PRESERVE_FASTFALL          (1 << nFTStatusPreserveFastFall)        // 0x8
#define FTSTATUS_PRESERVE_HITSTATUS         (1 << nFTStatusPreserveHitStatus)       // 0x10
#define FTSTATUS_PRESERVE_MODELPART         (1 << nFTStatusPreserveModelPart)       // 0x20
#define FTSTATUS_PRESERVE_SLOPECONTOUR      (1 << nFTStatusPreserveSlopeContour)    // 0x40
#define FTSTATUS_PRESERVE_TEXTUREPART       (1 << nFTStatusPreserveTexturePart)     // 0x80
#define FTSTATUS_PRESERVE_PLAYERTAG         (1 << nFTStatusPreservePlayerTag)       // 0x100
#define FTSTATUS_PRESERVE_THROWPOINTER      (1 << nFTStatusPreserveThrowPointer)    // 0x200
#define FTSTATUS_PRESERVE_SHUFFLETIME       (1 << nFTStatusPreserveShuffleTime)     // 0x400
#define FTSTATUS_PRESERVE_LOOPSFX           (1 << nFTStatusPreserveLoopSFX)         // 0x800
#define FTSTATUS_PRESERVE_DAMAGEPLAYER      (1 << nFTStatusPreserveDamagePlayer)    // 0x1000
#define FTSTATUS_PRESERVE_AFTERIMAGE        (1 << nFTStatusPreserveAfterImage)      // 0x2000
#define FTSTATUS_PRESERVE_RUMBLE            (1 << nFTStatusPreserveRumble)          // 0x4000

#define FTSTATUS_CHARACTERS_DEMO(status_id) (0x20000 + (status_id))
#define FTSTATUS_CHARACTERS_NULL            0xA2C2A

#define FTCATCHKIND_MASK_YOSHISPECIALN      (1 << nFTCatchKindYoshiSpecialN)    // 0x1
#define FTCATCHKIND_MASK_KIRBYSPECIALN      (1 << nFTCatchKindKirbySpecialN)    // 0x2
#define FTCATCHKIND_MASK_TARUCANN           (1 << nFTCatchKindTaruCann)         // 0x4
#define FTCATCHKIND_MASK_TWISTER            (1 << nFTCatchKindTwister)          // 0x8
#define FTCATCHKIND_MASK_COMMON             (1 << nFTCatchKindCommon)           // 0x10
#define FTCATCHKIND_MASK_CAPTAINSPECIALHI   (1 << nFTCatchKindCaptainSpecialHi) // 0x20

#define FTCATCHKIND_MASK_NONE (0)
#define FTCATCHKIND_MASK_ALL            \
(                                       \
    FTCATCHKIND_MASK_CAPTAINSPECIALHI | \
    FTCATCHKIND_MASK_COMMON           | \
    FTCATCHKIND_MASK_TWISTER          | \
    FTCATCHKIND_MASK_TARUCANN         | \
    FTCATCHKIND_MASK_KIRBYSPECIALN    | \
    FTCATCHKIND_MASK_YOSHISPECIALN      \
)

#define FTSLOPECONTOUR_FLAG_LFOOT   (1 << nFTSlopeContourLFoot)
#define FTSLOPECONTOUR_FLAG_RFOOT   (1 << nFTSlopeContourRFoot)
#define FTSLOPECONTOUR_FLAG_FULL    (1 << nFTSlopeContourFull)

#define FTCOMPUTER_COMMAND_TIMER_BITS           (0x4)
#define FTCOMPUTER_COMMAND_OPCODE_BITS          (0x4)

#define FTCOMPUTER_COMMAND_TIMER_MASK           (0x0F)
#define FTCOMPUTER_COMMAND_OPCODE_MASK          (0xF0)

#define FTCOMPUTER_COMMAND_BUTTON_A_PRESS       (nFTComputerCommandButtonAPress        << FTCOMPUTER_COMMAND_TIMER_BITS)   // 0x0
#define FTCOMPUTER_COMMAND_BUTTON_A_RELEASE     (nFTComputerCommandButtonARelease      << FTCOMPUTER_COMMAND_TIMER_BITS)   // 0x10
#define FTCOMPUTER_COMMAND_BUTTON_B_PRESS       (nFTComputerCommandButtonBPress        << FTCOMPUTER_COMMAND_TIMER_BITS)   // 0x20
#define FTCOMPUTER_COMMAND_BUTTON_B_RELEASE     (nFTComputerCommandButtonBRelease      << FTCOMPUTER_COMMAND_TIMER_BITS)   // 0x30
#define FTCOMPUTER_COMMAND_BUTTON_Z_PRESS       (nFTComputerCommandButtonZPress        << FTCOMPUTER_COMMAND_TIMER_BITS)   // 0x40
#define FTCOMPUTER_COMMAND_BUTTON_Z_RELEASE     (nFTComputerCommandButtonZRelease      << FTCOMPUTER_COMMAND_TIMER_BITS)   // 0x50
#define FTCOMPUTER_COMMAND_BUTTON_L_PRESS       (nFTComputerCommandButtonLPress        << FTCOMPUTER_COMMAND_TIMER_BITS)   // 0x60
#define FTCOMPUTER_COMMAND_BUTTON_L_RELEASE     (nFTComputerCommandButtonLRelease      << FTCOMPUTER_COMMAND_TIMER_BITS)   // 0x70
#define FTCOMPUTER_COMMAND_BUTTON_START_PRESS   (nFTComputerCommandButtonStartPress    << FTCOMPUTER_COMMAND_TIMER_BITS)   // 0x80
#define FTCOMPUTER_COMMAND_BUTTON_START_RELEASE (nFTComputerCommandButtonStartRelease  << FTCOMPUTER_COMMAND_TIMER_BITS)   // 0x90
#define FTCOMPUTER_COMMAND_STICK_X_TILT         (nFTComputerCommandStickX              << FTCOMPUTER_COMMAND_TIMER_BITS)   // 0xA0
#define FTCOMPUTER_COMMAND_STICK_Y_TILT         (nFTComputerCommandStickY              << FTCOMPUTER_COMMAND_TIMER_BITS)   // 0xB0
#define FTCOMPUTER_COMMAND_MOVEAUTO             (nFTComputerCommandMoveAuto            << FTCOMPUTER_COMMAND_TIMER_BITS)   // 0xC0
#define FTCOMPUTER_COMMAND_STICK_X_VAR          (nFTComputerCommandStickXVar           << FTCOMPUTER_COMMAND_TIMER_BITS)   // 0xD0
#define FTCOMPUTER_COMMAND_STICK_Y_VAR          (nFTComputerCommandStickYVar           << FTCOMPUTER_COMMAND_TIMER_BITS)   // 0xE0
#define FTCOMPUTER_COMMAND_DEFAULT_MAX          (nFTComputerCommandEnumCount             << FTCOMPUTER_COMMAND_TIMER_BITS)   // 0xF0
#define FTCOMPUTER_COMMAND_PKTHUNDER            (0xF3)
#define FTCOMPUTER_COMMAND_END                  (0xFF)

#define FTCOMPUTER_STICK_AUTOFULL               (0x7F)
#define FTCOMPUTER_STICK_AUTOHALF               (0x80)

#define FTCOMPUTER_EVENT_INSTRUCTION(k, t)      ( ((((k) << FTCOMPUTER_COMMAND_TIMER_BITS) & FTCOMPUTER_COMMAND_OPCODE_MASK) | ((t) & FTCOMPUTER_COMMAND_TIMER_MASK)) & U8_MAX )
#define FTCOMPUTER_EVENT_STICK_X(x, t)          FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandStickX, t), (x)
#define FTCOMPUTER_EVENT_STICK_Y(y, t)          FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandStickY, t), (y)
#define FTCOMPUTER_EVENT_PKTHUNDER()            (FTCOMPUTER_COMMAND_PKTHUNDER)
#define FTCOMPUTER_EVENT_END()                  (FTCOMPUTER_COMMAND_END)

#define FTKEY_EVENT_INSTRUCTION(k, t)       ( ((((k) << 12) & 0xF000) | ((t) & 0xFFF)) & U16_MAX )
#define FTKEY_EVENT_STICK(x, y, t)          FTKEY_EVENT_INSTRUCTION(nFTKeyEventStick, t), (((((x) << 8) & 0xFF00) | (((y) << 0) & 0x00FF)) & U16_MAX)
#define FTKEY_EVENT_BUTTON(b, t)            FTKEY_EVENT_INSTRUCTION(nFTKeyEventButton, t), ((b) & U16_MAX)
#define FTKEY_EVENT_END()                   FTKEY_EVENT_INSTRUCTION(nFTKeyEventEnd, 0)

#define ftMotionEventAdvance(event, type) ((event)->p_script = (void*)((uintptr_t)(event)->p_script + (sizeof(type))))

#define ftMotionEventCast(event, type) ((type*)(event)->p_script)

// WARNING: Only advances 4 bytes at a time
#define ftMotionEventCastAdvance(event, type) ((type*)(event)->p_script++)

#define ftMotionCommandEnd() GC_FIELDSET(nFTMotionEventEnd, 26, 6)

#define ftMotionCommandWait(frames) (GC_FIELDSET(nFTMotionEventSyncWait, 26, 6) | GC_FIELDSET(frames, 0, 26))
#define ftMotionCommandWaitAsync(frames) (GC_FIELDSET(nFTMotionEventAsyncWait, 26, 6) | GC_FIELDSET(frames, 0, 26))

#define ftMotionPlayFGM(fgm_id) (GC_FIELDSET(nFTMotionEventPlayFGM, 26, 6) | GC_FIELDSET(fgm_id, 0, 26))

#define ftMotionPlayVoice(fgm_id) (GC_FIELDSET(nFTMotionEventPlayVoiceStoreInfo, 26, 6) | GC_FIELDSET(fgm_id, 0, 26))
#define ftMotionPlayInterruptableVoice(fgm_id) (GC_FIELDSET(nFTMotionEventPlayLoopVoiceStoreInfo, 26, 6) | GC_FIELDSET(fgm_id, 0, 26)) // stops playing on action change

#define ftMotionCommandSubroutineS1() GC_FIELDSET(nFTMotionEventSubroutine, 26, 6)
#define ftMotionCommandSubroutineS2(addr) ((uintptr_t)addr)

#define ftMotionCommandSubroutine(addr) ftMotionCommandSubroutineS1(), ftMotionCommandSubroutineS2(addr)

#define ftMotionCommandReturn() GC_FIELDSET(nFTMotionEventReturn, 26, 6)

#define ftMotionCommandGotoS1() GC_FIELDSET(nFTMotionEventGoto, 26, 6)
#define ftMotionCommandGotoS2(addr) ((uintptr_t)addr)

#define ftMotionCommandGoto(addr) ftMotionCommandGotoS1(), ftMotionCommandGotoS2(addr)

#define ftMotionCommandSetTexturePartID(flag) (GC_FIELDSET(nFTMotionEventSetTexturePartID, 26, 6) | GC_FIELDSET(flag, 0, 26)) // 0 = none, 3 = feet

#define ftMotionCommandSetSlopeContour(flag) (GC_FIELDSET(nFTMotionEventSetSlopeContour, 26, 6) | GC_FIELDSET(flag, 0, 26)) // 0 = none, 3 = feet


// Enums
typedef enum FTStatusUpdateFlags
{
    nFTStatusPreserveHit,               // Preserve hitboxes between action states
    nFTStatusPreserveColAnim,           // Preserve color animation between action states
    nFTStatusPreserveEffect,            // Preserve GFX between action states
    nFTStatusPreserveFastFall,          // Preserve fastfall between action states
    nFTStatusPreserveHitStatus,         // Preserve hurtbox collision state between action states
    nFTStatusPreserveModelPart,         // Preserve modified model part display state between action states
    nFTStatusPreserveSlopeContour,      // Preserve inverse kinematics mode between action states
    nFTStatusPreserveTexturePart,       // Preserve modified texture part display state between action states
    nFTStatusPreservePlayerTag,         // Preserve player indicator wait timer
    nFTStatusPreserveThrowPointer,      // Preserve thrower's GObj pointer between action states; used to tell the game not to check thrower's collateral hitbox on thrown victim?  
    nFTStatusPreserveShuffleTime,       // Preserve model vibration frames from hitlag between action states
    nFTStatusPreserveLoopSFX,           // Preserve looping SFX
    nFTStatusPreserveDamagePlayer,      // Preserve damaging player's port ID if grounded?
    nFTStatusPreserveAfterImage,        // Preserve sword trail display state
    nFTStatusPreserveRumble             // Preserve rumble

} FTStatusUpdateFlags;

typedef enum FTCommonMotion
{
    nFTCommonMotionNull = -1,
    nFTCommonMotionEntryNull,
    nFTCommonMotionRebirthDown,
    nFTCommonMotionRebirthStand,
    nFTCommonMotionRebirthWait,
    nFTCommonMotionWait,
    nFTCommonMotionWalkSlow,
    nFTCommonMotionWalkMiddle,
    nFTCommonMotionWalkFast,
    nFTCommonMotionWalkEnd,
    nFTCommonMotionDash,
    nFTCommonMotionRun,
    nFTCommonMotionRunBrake,
    nFTCommonMotionTurn,
    nFTCommonMotionTurnRun,
    nFTCommonMotionKneeBend,
    nFTCommonMotionGuardKneeBend,
    nFTCommonMotionJumpF,
    nFTCommonMotionJumpB,
    nFTCommonMotionJumpAerialF,
    nFTCommonMotionJumpAerialB,
    nFTCommonMotionFall,
    nFTCommonMotionFallAerial,
    nFTCommonMotionSquat,
    nFTCommonMotionSquatWait,
    nFTCommonMotionSquatRv,
    nFTCommonMotionLandingLight,
    nFTCommonMotionLandingHeavy,
    nFTCommonMotionPass,
    nFTCommonMotionGuardPass,
    nFTCommonMotionOttottoWait,
    nFTCommonMotionOttotto,
    nFTCommonMotionDamageHi1,
    nFTCommonMotionDamageHi2,
    nFTCommonMotionDamageHi3,
    nFTCommonMotionDamageN1,
    nFTCommonMotionDamageN2,
    nFTCommonMotionDamageN3,
    nFTCommonMotionDamageLw1,
    nFTCommonMotionDamageLw2,
    nFTCommonMotionDamageLw3,
    nFTCommonMotionDamageAir1,
    nFTCommonMotionDamageAir2,
    nFTCommonMotionDamageAir3,
    nFTCommonMotionDamageE,
    nFTCommonMotionDamageFlyHi,
    nFTCommonMotionDamageFlyN,
    nFTCommonMotionDamageFlyLw,
    nFTCommonMotionDamageFlyTop,
    nFTCommonMotionDamageFlyRoll,
    nFTCommonMotionWallDamage,
    nFTCommonMotionDamageFall,
    nFTCommonMotionFallSpecial,
    nFTCommonMotionLandingFallSpecial,
    nFTCommonMotionTwister,
    nFTCommonMotionDokanStart,
    nFTCommonMotionDokanEnd,
    nFTCommonMotionDokanWalk,
    nFTCommonMotionStopCeil,
    nFTCommonMotionDownBounceD,
    nFTCommonMotionDownBounceU,
    nFTCommonMotionDownStandD,
    nFTCommonMotionDownStandU,
    nFTCommonMotionPassiveStandF,
    nFTCommonMotionPassiveStandB,
    nFTCommonMotionDownForwardD,
    nFTCommonMotionDownForwardU,
    nFTCommonMotionDownBackD,
    nFTCommonMotionDownBackU,
    nFTCommonMotionDownAttackD,
    nFTCommonMotionDownAttackU,
    nFTCommonMotionPassive,
    nFTCommonMotionRebound,
    nFTCommonMotionCliffCatch,
    nFTCommonMotionCliffWait,
    nFTCommonMotionCliffQuick,
    nFTCommonMotionCliffClimbQuick1,
    nFTCommonMotionCliffClimbQuick2,
    nFTCommonMotionCliffSlow,
    nFTCommonMotionCliffClimbSlow1,
    nFTCommonMotionCliffClimbSlow2,
    nFTCommonMotionCliffAttackQuick1,
    nFTCommonMotionCliffAttackQuick2,
    nFTCommonMotionCliffAttackSlow1,
    nFTCommonMotionCliffAttackSlow2,
    nFTCommonMotionCliffEscapeQuick1,
    nFTCommonMotionCliffEscapeQuick2,
    nFTCommonMotionCliffEscapeSlow1,
    nFTCommonMotionCliffEscapeSlow2,
    nFTCommonMotionLightGet,
    nFTCommonMotionHeavyGet,
    nFTCommonMotionLightThrowDrop,
    nFTCommonMotionLightThrowDash,
    nFTCommonMotionLightThrowF,
    nFTCommonMotionLightThrowB,
    nFTCommonMotionLightThrowHi,
    nFTCommonMotionLightThrowLw,
    nFTCommonMotionLightThrowF4,
    nFTCommonMotionLightThrowB4,
    nFTCommonMotionLightThrowHi4,
    nFTCommonMotionLightThrowLw4,
    nFTCommonMotionLightThrowAirF,
    nFTCommonMotionLightThrowAirB,
    nFTCommonMotionLightThrowAirHi,
    nFTCommonMotionLightThrowAirLw,
    nFTCommonMotionLightThrowAirF4,
    nFTCommonMotionLightThrowAirB4,
    nFTCommonMotionLightThrowAirHi4,
    nFTCommonMotionLightThrowAirLw4,
    nFTCommonMotionHeavyThrowF,
    nFTCommonMotionHeavyThrowB,
    nFTCommonMotionHeavyThrowF4,
    nFTCommonMotionHeavyThrowB4,
    nFTCommonMotionSwordSwing1,
    nFTCommonMotionSwordSwing3,
    nFTCommonMotionSwordSwing4,
    nFTCommonMotionSwordSwingDash,
    nFTCommonMotionBatSwing1,
    nFTCommonMotionBatSwing3,
    nFTCommonMotionBatSwing4,
    nFTCommonMotionBatSwingDash,
    nFTCommonMotionHarisenSwing1,
    nFTCommonMotionHarisenSwing3,
    nFTCommonMotionHarisenSwing4,
    nFTCommonMotionHarisenSwingDash,
    nFTCommonMotionStarRodSwing1,
    nFTCommonMotionStarRodSwing3,
    nFTCommonMotionStarRodSwing4,
    nFTCommonMotionStarRodSwingDash,
    nFTCommonMotionLGunShoot,
    nFTCommonMotionLGunShootAir,
    nFTCommonMotionFireFlowerShoot,
    nFTCommonMotionFireFlowerShootAir,
    nFTCommonMotionHammerWait,
    nFTCommonMotionHammerWalk,
    nFTCommonMotionGuardOn,
    nFTCommonMotionGuardOff,
    nFTCommonMotionEscapeF,
    nFTCommonMotionEscapeB,
    nFTCommonMotionShieldBreakFly,
    nFTCommonMotionShieldBreakFall,
    nFTCommonMotionShieldBreakDownD,
    nFTCommonMotionShieldBreakDownU,
    nFTCommonMotionShieldBreakStandD,
    nFTCommonMotionShieldBreakStandU,
    nFTCommonMotionFuraFura,
    nFTCommonMotionFuraSleep,
    nFTCommonMotionCatch,
    nFTCommonMotionCatchPull,
    nFTCommonMotionThrowF,
    nFTCommonMotionThrowB,
    nFTCommonMotionCapturePulled,
    nFTCommonMotionThrownKirbyStar,
    nFTCommonMotionThrownCopyStar,
    nFTCommonMotionYoshiEgg,
    nFTCommonMotionCaptureCaptain,
    nFTCommonMotionThrownDonkeyUnk,
    nFTCommonMotionThrownDonkeyF,
    nFTCommonMotionThrownMarioB1,
    nFTCommonMotionThrownUnk1,
    nFTCommonMotionShouldered,
    nFTCommonMotionThrownMarioB2,
    nFTCommonMotionThrownCommon,
    nFTCommonMotionThrownUnk2,
    nFTCommonMotionThrownUnk3,
    nFTCommonMotionAppeal,
    nFTCommonMotionAttack11,
    nFTCommonMotionAttack12,
    nFTCommonMotionAttackDash,
    nFTCommonMotionAttackS3Hi,
    nFTCommonMotionAttackS3HiS,
    nFTCommonMotionAttackS3,
    nFTCommonMotionAttackS3LwS,
    nFTCommonMotionAttackS3Lw,
    nFTCommonMotionAttackHi3F, // Forward-angled up tilt???
    nFTCommonMotionAttackHi3,
    nFTCommonMotionAttackHi3B, // Backward-angled up tilt???
    nFTCommonMotionAttackLw3,
    nFTCommonMotionAttackS4Hi,
    nFTCommonMotionAttackS4HiS,
    nFTCommonMotionAttackS4,
    nFTCommonMotionAttackS4LwS,
    nFTCommonMotionAttackS4Lw,
    nFTCommonMotionAttackHi4,
    nFTCommonMotionAttackLw4,

    nFTCommonMotionAttackAirStart,
    nFTCommonMotionAttackAirN = nFTCommonMotionAttackAirStart,
    nFTCommonMotionAttackAirF,
    nFTCommonMotionAttackAirB,
    nFTCommonMotionAttackAirHi,
    nFTCommonMotionAttackAirLw,
    nFTCommonMotionAttackAirEnd = nFTCommonMotionAttackAirLw,

    nFTCommonMotionLandingAirStart,
    nFTCommonMotionLandingAirN = nFTCommonMotionLandingAirStart,
    nFTCommonMotionLandingAirF,
    nFTCommonMotionLandingAirB,
    nFTCommonMotionLandingAirHi,
    nFTCommonMotionLandingAirLw,
    nFTCommonMotionLandingAirEnd = nFTCommonMotionLandingAirLw,

    nFTCommonMotionLandingAirNull,
    nFTCommonMotionSpecialStart

} FTCommonMotion;

typedef enum FTCommonStatus
{
    nFTCommonStatusDeadDown,
    nFTCommonStatusDeadLeftRight,
    nFTCommonStatusDeadUpStar,
    nFTCommonStatusDeadUpFall,
    nFTCommonStatusSleep,
    nFTCommonStatusEntry,

    nFTCommonStatusActionStart,    // Start of uh... I don't know a better name for this.
    nFTCommonStatusEntryNull = nFTCommonStatusActionStart,
    nFTCommonStatusRebirthDown,
    nFTCommonStatusRebirthStand,
    nFTCommonStatusRebirthWait,

    nFTCommonStatusControlStart,    // Start of (mostly) actionable (or rather player-influenced) moves
    nFTCommonStatusWait = nFTCommonStatusControlStart,
    nFTCommonStatusWalkSlow,
    nFTCommonStatusWalkMiddle,
    nFTCommonStatusWalkFast,
    nFTCommonStatusWalkEnd,
    nFTCommonStatusDash,
    nFTCommonStatusRun,
    nFTCommonStatusRunBrake,
    nFTCommonStatusTurn,
    nFTCommonStatusTurnRun,
    nFTCommonStatusKneeBend,
    nFTCommonStatusGuardKneeBend,
    nFTCommonStatusJumpF,
    nFTCommonStatusJumpB,
    nFTCommonStatusJumpAerialF,
    nFTCommonStatusJumpAerialB,
    nFTCommonStatusFall,
    nFTCommonStatusFallAerial,
    nFTCommonStatusSquat,
    nFTCommonStatusSquatWait,
    nFTCommonStatusSquatRv,
    nFTCommonStatusLandingLight,
    nFTCommonStatusLandingHeavy,
    nFTCommonStatusPass,
    nFTCommonStatusGuardPass,
    nFTCommonStatusOttottoWait,
    nFTCommonStatusOttotto,

    nFTCommonStatusDamageStart,
    nFTCommonStatusDamageHi1 = nFTCommonStatusDamageStart,
    nFTCommonStatusDamageHi2,
    nFTCommonStatusDamageHi3,
    nFTCommonStatusDamageN1,
    nFTCommonStatusDamageN2,
    nFTCommonStatusDamageN3,
    nFTCommonStatusDamageLw1,
    nFTCommonStatusDamageLw2,
    nFTCommonStatusDamageLw3,
    nFTCommonStatusDamageAir1,
    nFTCommonStatusDamageAir2,
    nFTCommonStatusDamageAir3,
    nFTCommonStatusDamageE1,       // Eletric damage weak
    nFTCommonStatusDamageE2,       // Eletric damage strong
    nFTCommonStatusDamageFlyHi,
    nFTCommonStatusDamageFlyN,
    nFTCommonStatusDamageFlyLw,
    nFTCommonStatusDamageFlyTop,
    nFTCommonStatusDamageFlyRoll,
    nFTCommonStatusWallDamage,
    nFTCommonStatusDamageEnd = nFTCommonStatusWallDamage,

    nFTCommonStatusDamageFall,
    nFTCommonStatusFallSpecial,
    nFTCommonStatusLandingFallSpecial,
    nFTCommonStatusTwister,        // Hyrule Tornado
    nFTCommonStatusTaruCann,       // DK Barrel
    nFTCommonStatusDokanStart,     // Enter pipe
    nFTCommonStatusDokanWait,      // In pipe
    nFTCommonStatusDokanEnd,       // Exit pipe
    nFTCommonStatusDokanWalk,      // Exit horizontal pipe
    nFTCommonStatusStopCeil,       // Bump head into ceiling
    nFTCommonStatusDownBounceD,
    nFTCommonStatusDownBounceU,
    nFTCommonStatusDownWaitD,
    nFTCommonStatusDownWaitU,
    nFTCommonStatusDownStandD,
    nFTCommonStatusDownStandU,
    nFTCommonStatusPassiveStandF,
    nFTCommonStatusPassiveStandB,
    nFTCommonStatusDownForwardD,
    nFTCommonStatusDownForwardU,
    nFTCommonStatusDownBackD,
    nFTCommonStatusDownBackU,
    nFTCommonStatusDownAttackD,
    nFTCommonStatusDownAttackU,
    nFTCommonStatusPassive,
    nFTCommonStatusReboundWait,
    nFTCommonStatusRebound,
    nFTCommonStatusCliffCatch,
    nFTCommonStatusCliffWait,
    nFTCommonStatusCliffQuick,
    nFTCommonStatusCliffClimbQuick1,
    nFTCommonStatusCliffClimbQuick2,
    nFTCommonStatusCliffSlow,
    nFTCommonStatusCliffClimbSlow1,
    nFTCommonStatusCliffClimbSlow2,
    nFTCommonStatusCliffAttackQuick1,
    nFTCommonStatusCliffAttackQuick2,
    nFTCommonStatusCliffAttackSlow1,
    nFTCommonStatusCliffAttackSlow2,
    nFTCommonStatusCliffEscapeQuick1,
    nFTCommonStatusCliffEscapeQuick2,
    nFTCommonStatusCliffEscapeSlow1,
    nFTCommonStatusCliffEscapeSlow2,
    nFTCommonStatusLightGet,
    nFTCommonStatusHeavyGet,
    nFTCommonStatusLiftWait,
    nFTCommonStatusLiftTurn,

    nFTCommonStatusLightThrowStart,                                    // Start of Light Throw action states
    nFTCommonStatusLightThrowDrop = nFTCommonStatusLightThrowStart,
    nFTCommonStatusLightThrowDash,
    nFTCommonStatusLightThrowF,
    nFTCommonStatusLightThrowB,
    nFTCommonStatusLightThrowHi,
    nFTCommonStatusLightThrowLw,
    nFTCommonStatusLightThrowF4,
    nFTCommonStatusLightThrowB4,
    nFTCommonStatusLightThrowHi4,
    nFTCommonStatusLightThrowLw4,
    nFTCommonStatusLightThrowAirF,
    nFTCommonStatusLightThrowAirB,
    nFTCommonStatusLightThrowAirHi,
    nFTCommonStatusLightThrowAirLw,
    nFTCommonStatusLightThrowAirF4,
    nFTCommonStatusLightThrowAirB4,
    nFTCommonStatusLightThrowAirHi4,
    nFTCommonStatusLightThrowAirLw4,
    nFTCommonStatusLightThrowEnd = nFTCommonStatusLightThrowAirLw4,   // End of Light Throw action states

    nFTCommonStatusHeavyThrowStart,                                    // Start of Heavy Throw action states
    nFTCommonStatusHeavyThrowF = nFTCommonStatusHeavyThrowStart,
    nFTCommonStatusHeavyThrowB,
    nFTCommonStatusHeavyThrow4Start,                                   // Start of Heavy Throw Smash action states
    nFTCommonStatusHeavyThrowF4 = nFTCommonStatusHeavyThrow4Start, 
    nFTCommonStatusHeavyThrowB4,
    nFTCommonStatusHeavyThrow4End = nFTCommonStatusHeavyThrowB4,      // End of Heavy Throw Smash action states
    nFTCommonStatusHeavyThrowEnd = nFTCommonStatusHeavyThrow4End,     // End of Heavy Throw action states

    nFTCommonStatusItemSwingStart,                                     // Start of Item Swing action states
    nFTCommonStatusSwordSwing1 = nFTCommonStatusItemSwingStart,
    nFTCommonStatusSwordSwing3,
    nFTCommonStatusSwordSwing4,
    nFTCommonStatusSwordSwingDash,
    nFTCommonStatusBatSwing1,
    nFTCommonStatusBatSwing3,
    nFTCommonStatusBatSwing4,
    nFTCommonStatusBatSwingDash,
    nFTCommonStatusHarisenSwing1,
    nFTCommonStatusHarisenSwing3,
    nFTCommonStatusHarisenSwing4,
    nFTCommonStatusHarisenSwingDash,
    nFTCommonStatusStarRodSwing1,
    nFTCommonStatusStarRodSwing3,
    nFTCommonStatusStarRodSwing4,
    nFTCommonStatusStarRodSwingDash,
    nFTCommonStatusItemSwingEnd = nFTCommonStatusStarRodSwingDash,    // End of Item Swing action states

    nFTCommonStatusLGunShoot,
    nFTCommonStatusLGunShootAir,
    nFTCommonStatusFireFlowerShoot,
    nFTCommonStatusFireFlowerShootAir,

    nFTCommonStatusHammerStart,                                        // Start of Hammer action states
    nFTCommonStatusHammerWait = nFTCommonStatusHammerStart,
    nFTCommonStatusHammerWalk,
    nFTCommonStatusHammerTurn,
    nFTCommonStatusHammerKneeBend,
    nFTCommonStatusHammerFall,                                         // Pretty much any aerial Hammer state
    nFTCommonStatusHammerLanding,
    nFTCommonStatusHammerEnd = nFTCommonStatusHammerLanding,          // End of Hammer action states

    nFTCommonStatusGuardStart,                                         // Start of Shield action states
    nFTCommonStatusGuardOn = nFTCommonStatusGuardStart,
    nFTCommonStatusGuard,
    nFTCommonStatusGuardOff,
    nFTCommonStatusGuardSetOff,
    nFTCommonStatusGuardEnd = nFTCommonStatusGuardSetOff,             // End of Shield action states  

    nFTCommonStatusEscapeF,
    nFTCommonStatusEscapeB,
    nFTCommonStatusShieldBreakFly,
    nFTCommonStatusShieldBreakFall,
    nFTCommonStatusShieldBreakDownD,
    nFTCommonStatusShieldBreakDownU,
    nFTCommonStatusShieldBreakStandD,
    nFTCommonStatusShieldBreakStandU,
    nFTCommonStatusFuraFura,
    nFTCommonStatusFuraSleep,
    nFTCommonStatusCatch,
    nFTCommonStatusCatchPull,
    nFTCommonStatusCatchWait,
    nFTCommonStatusThrowF,
    nFTCommonStatusThrowB,
    nFTCommonStatusCapturePulled,
    nFTCommonStatusCaptureWait,
    nFTCommonStatusCaptureKirby,
    nFTCommonStatusCaptureWaitKirby,
    nFTCommonStatusThrownKirbyStar,
    nFTCommonStatusThrownCopyStar,
    nFTCommonStatusCaptureYoshi,
    nFTCommonStatusYoshiEgg,
    nFTCommonStatusCaptureCaptain,
    nFTCommonStatusThrownDonkeyUnk, // Related to DK throws?

    nFTCommonStatusThrownStart,
    nFTCommonStatusThrownDonkeyF = nFTCommonStatusThrownStart,
    nFTCommonStatusThrownMarioBStart,
    nFTCommonStatusThrownFoxFStart,
    nFTCommonStatusShouldered,
    nFTCommonStatusThrownMarioB,
    nFTCommonStatusThrownCommon,
    nFTCommonStatusThrownFoxF,
    nFTCommonStatusThrownFoxB,
    nFTCommonStatusThrownEnd = nFTCommonStatusThrownFoxB,

    nFTCommonStatusAppeal,
    nFTCommonStatusAttack11,
    nFTCommonStatusAttack12,
    nFTCommonStatusAttackDash,
    nFTCommonStatusAttackS3Hi,
    nFTCommonStatusAttackS3HiS,
    nFTCommonStatusAttackS3,
    nFTCommonStatusAttackS3LwS,
    nFTCommonStatusAttackS3Lw,
    nFTCommonStatusAttackHi3F, // Forward-angled up tilt???
    nFTCommonStatusAttackHi3,
    nFTCommonStatusAttackHi3B, // Backward-angled up tilt???
    nFTCommonStatusAttackLw3,
    nFTCommonStatusAttackS4Hi,
    nFTCommonStatusAttackS4HiS,
    nFTCommonStatusAttackS4,
    nFTCommonStatusAttackS4LwS,
    nFTCommonStatusAttackS4Lw,
    nFTCommonStatusAttackHi4,
    nFTCommonStatusAttackLw4,

    nFTCommonStatusAttackAirStart,
    nFTCommonStatusAttackAirN = nFTCommonStatusAttackAirStart,
    nFTCommonStatusAttackAirF,
    nFTCommonStatusAttackAirB,
    nFTCommonStatusAttackAirHi,
    nFTCommonStatusAttackAirLw,
    nFTCommonStatusAttackAirEnd = nFTCommonStatusAttackAirLw,

    nFTCommonStatusLandingAirStart,
    nFTCommonStatusLandingAirN = nFTCommonStatusLandingAirStart,
    nFTCommonStatusLandingAirF,
    nFTCommonStatusLandingAirB,
    nFTCommonStatusLandingAirHi,
    nFTCommonStatusLandingAirLw,
    nFTCommonStatusLandingAirNull,
    nFTCommonStatusLandingAirEnd = nFTCommonStatusLandingAirNull,

    nFTCommonStatusSpecialStart // Start of special move table

} FTCommonStatus;

typedef enum FTDemoStatus
{
    nFTDemoStatusNull = 0x10000,
    nFTDemoStatusWin1,          // Win / selected 1
    nFTDemoStatusWin2,          // Win / selected 2
    nFTDemoStatusWin3,          // Win / selected 3
    nFTDemoStatusWin4,          // Win / selected 4
    nFTDemoStatusLose,          // Lose (results screen)
    nFTDemoStatusRun,           // Common status ID for opening movie running scene
    nFTDemoStatusJump,          // Link jump status ID for opening movie running scene
    nFTDemoStatusFigurePulled,  // Fighter plucked by Master Hand during opening movie
    nFTDemoStatusFigureDropped, // Fighter dropped in opening movie / ending movie / game over
    nFTDemoStatusFigureStand,   // Fighter standing up in opening movie / game over continue
    nFTDemoStatusClash,         // Common status ID for opening movie clashing scene
    nFTDemoStatusStance,        // Common status ID for opening movie sliding fighter poses
    nFTDemoStatusIntroL,        // Common status ID for 1P Game Stage Cards (Player / Allies)
    nFTDemoStatusIntroR,        // Common status ID for 1P Game Stage Cards (Enemies)
    nFTDemoStatusSpecialStart   // Start of opening movie character-specific statuses

} FTDemoStatus;

typedef enum FTMotionAttackIndex
{
    nFTMotionAttackIDNone,

    nFTMotionAttackIDBaseStart,                                 // Start of moves that are part of the character's base moveset
    nFTMotionAttackIDAttack11 = nFTMotionAttackIDBaseStart,
    nFTMotionAttackIDAttack12,
    nFTMotionAttackIDAttack13,
    nFTMotionAttackIDAttack100,
    nFTMotionAttackIDAttackDash,
    nFTMotionAttackIDAttackS3,
    nFTMotionAttackIDAttackHi3,
    nFTMotionAttackIDAttackLw3,
    nFTMotionAttackIDAttackS4,
    nFTMotionAttackIDAttackHi4,
    nFTMotionAttackIDAttackLw4,
    nFTMotionAttackIDAttackAirN,
    nFTMotionAttackIDAttackAirF,
    nFTMotionAttackIDAttackAirB,
    nFTMotionAttackIDAttackAirHi,
    nFTMotionAttackIDAttackAirLw,
    nFTMotionAttackIDSpecialHi,
    nFTMotionAttackIDSpecialN,
    nFTMotionAttackIDSpecialNCopyMario,
    nFTMotionAttackIDSpecialNCopyLuigi,
    nFTMotionAttackIDSpecialNCopyFox,
    nFTMotionAttackIDSpecialNCopySamus,
    nFTMotionAttackIDSpecialNCopyDonkey,
    nFTMotionAttackIDSpecialNCopyPikachu,
    nFTMotionAttackIDSpecialNCopyNess,
    nFTMotionAttackIDSpecialNCopyLink,
    nFTMotionAttackIDSpecialNCopyPurin,
    nFTMotionAttackIDSpecialNCopyCaptain,
    nFTMotionAttackIDSpecialNCopyYoshi,
    nFTMotionAttackIDSpecialLw,
    nFTMotionAttackIDDownAttackD,
    nFTMotionAttackIDDownAttackU,
    nFTMotionAttackIDCliffAttackQuick,
    nFTMotionAttackIDCliffAttackSlow,
    nFTMotionAttackIDThrowF,
    nFTMotionAttackIDThrowB,
    nFTMotionAttackIDBaseEnd = nFTMotionAttackIDThrowB,

    nFTMotionAttackIDItemStart,
    nFTMotionAttackIDSwordSwing1 = nFTMotionAttackIDItemStart,
    nFTMotionAttackIDSwordSwing3,
    nFTMotionAttackIDSwordSwing4,
    nFTMotionAttackIDSwordSwingDash,
    nFTMotionAttackIDBatSwing1,
    nFTMotionAttackIDBatSwing3,
    nFTMotionAttackIDBatSwing4,
    nFTMotionAttackIDBatSwingDash,
    nFTMotionAttackIDHarisenSwing1,
    nFTMotionAttackIDHarisenSwing3,
    nFTMotionAttackIDHarisenSwing4,
    nFTMotionAttackIDHarisenSwingDash,
    nFTMotionAttackIDStarRodSwing1,
    nFTMotionAttackIDStarRodSwing3,
    nFTMotionAttackIDStarRodSwing4,
    nFTMotionAttackIDStarRodSwingDash,
    nFTMotionAttackIDLGunShoot,
    nFTMotionAttackIDFireFlowerShoot,
    nFTMotionAttackIDHammer,
    nFTMotionAttackIDItemThrow,
    nFTMotionAttackIDItemEnd = nFTMotionAttackIDItemThrow,

    nFTMotionAttackIDNull,
    nFTMotionAttackIDEnumCount

} FTMotionAttackIndex;

typedef enum FTStatusAttackIndex
{
    nFTStatusAttackIDNone,

    nFTStatusAttackIDAttackStart,

    nFTStatusAttackIDNormalStart = nFTStatusAttackIDAttackStart,
    nFTStatusAttackIDAttack11 = nFTStatusAttackIDNormalStart,
    nFTStatusAttackIDAttackDash,
    nFTStatusAttackIDAttackS3,
    nFTStatusAttackIDAttackHi3,
    nFTStatusAttackIDAttackLw3,
    nFTStatusAttackIDAttackS4,
    nFTStatusAttackIDAttackHi4,
    nFTStatusAttackIDAttackLw4,
    nFTStatusAttackIDAttackAirN,
    nFTStatusAttackIDAttackAirF,
    nFTStatusAttackIDAttackAirB,
    nFTStatusAttackIDAttackAirHi,
    nFTStatusAttackIDAttackAirLw,
    nFTStatusAttackIDNormalEnd = nFTStatusAttackIDAttackAirLw,

    nFTStatusAttackIDJabStart,
    nFTStatusAttackIDAttack12 = nFTStatusAttackIDJabStart,
    nFTStatusAttackIDAttack13,
    nFTStatusAttackIDAttack100,
    nFTStatusAttackIDJabEnd = nFTStatusAttackIDAttack100,

    nFTStatusAttackIDSpecialStart,
    nFTStatusAttackIDSpecialHi = nFTStatusAttackIDSpecialStart,
    nFTStatusAttackIDSpecialN,
    nFTStatusAttackIDSpecialNCopyMario,
    nFTStatusAttackIDSpecialNCopyLuigi,
    nFTStatusAttackIDSpecialNCopyFox,
    nFTStatusAttackIDSpecialNCopySamus,
    nFTStatusAttackIDSpecialNCopyDonkey,
    nFTStatusAttackIDSpecialNCopyPikachu,
    nFTStatusAttackIDSpecialNCopyNess,
    nFTStatusAttackIDSpecialNCopyLink,
    nFTStatusAttackIDSpecialNCopyPurin,
    nFTStatusAttackIDSpecialNCopyCaptain,
    nFTStatusAttackIDSpecialNCopyYoshi,
    nFTStatusAttackIDSpecialLw,
    nFTStatusAttackIDSpecialEnd = nFTStatusAttackIDSpecialLw,

    nFTStatusAttackIDDownAttackD,
    nFTStatusAttackIDDownAttackU,
    nFTStatusAttackIDCliffAttackQuick,
    nFTStatusAttackIDCliffAttackSlow,
    nFTStatusAttackIDThrowF,
    nFTStatusAttackIDThrowB,

    nFTStatusAttackIDItemStart,
    nFTStatusAttackIDSwordSwing1 = nFTStatusAttackIDItemStart,
    nFTStatusAttackIDSwordSwing3,
    nFTStatusAttackIDSwordSwing4,
    nFTStatusAttackIDSwordSwingDash,
    nFTStatusAttackIDBatSwing1,
    nFTStatusAttackIDBatSwing3,
    nFTStatusAttackIDBatSwing4,
    nFTStatusAttackIDBatSwingDash,
    nFTStatusAttackIDHarisenSwing1,
    nFTStatusAttackIDHarisenSwing3,
    nFTStatusAttackIDHarisenSwing4,
    nFTStatusAttackIDHarisenSwingDash,
    nFTStatusAttackIDStarRodSwing1,
    nFTStatusAttackIDStarRodSwing3,
    nFTStatusAttackIDStarRodSwing4,
    nFTStatusAttackIDStarRodSwingDash,
    nFTStatusAttackIDLGunShoot,
    nFTStatusAttackIDFireFlowerShoot,
    nFTStatusAttackIDHammer,
    nFTStatusAttackIDItemThrow,
    nFTStatusAttackIDItemEnd = nFTStatusAttackIDItemThrow,

    nFTStatusAttackIDNull,
    nFTStatusAttackIDEnumCount

} FTStatusAttackIndex;

typedef enum FTMotionEvent
{
	nFTMotionEventEnd,
	nFTMotionEventSyncWait,
	nFTMotionEventAsyncWait,
	nFTMotionEventMakeAttackColl,
	nFTMotionEventMakeAttackCollScaled,
	nFTMotionEventClearAttackCollID,
	nFTMotionEventClearAttackCollAll,
	nFTMotionEventSetAttackCollOffset,
	nFTMotionEventSetAttackCollDamage,
	nFTMotionEventSetAttackCollSize,
	nFTMotionEventSetAttackCollSoundLevel,
	nFTMotionEventRefreshAttackCollID,
	nFTMotionEventSetThrow,
	nFTMotionEventSetDamageThrown,
	nFTMotionEventPlayFGM,
	nFTMotionEventPlayLoopSFXStoreInfo,
	nFTMotionEventStopLoopSFX,
	nFTMotionEventPlayVoiceStoreInfo,
	nFTMotionEventPlayLoopVoiceStoreInfo,
	nFTMotionEventPlayFGMStoreInfo,
	nFTMotionEventPlaySmashVoice,
	nFTMotionEventSetFlag0,
	nFTMotionEventSetFlag1,
	nFTMotionEventSetFlag2,
	nFTMotionEventSetFlag3,
	nFTMotionEventSetAirJumpAdd,
	nFTMotionEventSetAirJumpMax,
	nFTMotionEventSetHitStatusPartAll,
	nFTMotionEventSetHitStatusPartID,
	nFTMotionEventSetHitStatusAll,
	nFTMotionEventResetDamageCollPartAll,
	nFTMotionEventSetDamageCollPartID,
	nFTMotionEventLoopBegin,
	nFTMotionEventLoopEnd,
	nFTMotionEventSubroutine,
	nFTMotionEventReturn,
	nFTMotionEventGoto,
	nFTMotionEventPauseScript,
	nFTMotionEventEffect,
	nFTMotionEventEffectItemHold, // ???
	nFTMotionEventSetModelPartID,
	nFTMotionEventResetModelPartAll,
	nFTMotionEventHideModelPartAll,
	nFTMotionEventSetTexturePartID,
	nFTMotionEventSetColAnim,
	nFTMotionEventResetColAnim,
	nFTMotionEventSetParallelScript, // What
	nFTMotionEventSetSlopeContour,
	nFTMotionEventHideItem,
	nFTMotionEventMakeRumble,
	nFTMotionEventStopRumble,
	nFTMotionEventSetAfterImage // Sword Trail

} FTMotionEvent;

typedef enum FTCameraModes
{
    nFTCameraModeDefault,
    nFTCameraModeGhost,
    nFTCameraModeDeadUp,
    nFTCameraModeEntry,
    nFTCameraModeExplain

} FTCameraModes;

typedef enum FTSlopeContours
{
    nFTSlopeContourLFoot,
    nFTSlopeContourRFoot,
    nFTSlopeContourFull

} FTSlopeContours;

typedef enum FTHitLogObjectKind
{
	nFTHitLogObjectNone,
	nFTHitLogObjectFighter,
	nFTHitLogObjectWeapon,
	nFTHitLogObjectItem,
	nFTHitLogObjectGround

} FTHitLogObjectKind;

typedef enum FTItemSwingKind
{
    nFTItemSwingKindSword,
    nFTItemSwingKindBat,
    nFTItemSwingKindHarisen,
    nFTItemSwingKindStarRod,
    nFTItemSwingKindEnumCount

} FTItemSwingKind;

typedef enum FTItemSwingType
{
    nFTItemSwingTypeAttack1,
    nFTItemSwingTypeAttack3,
    nFTItemSwingTypeAttack4,
    nFTItemSwingTypeAttackDash,
    nFTItemSwingTypeEnumCount
	
} FTItemSwingType;

typedef enum FTSpecialCollKind
{
    nFTSpecialCollKindFoxReflector,
    nFTSpecialCollKindNessAbsorb,
    nFTSpecialCollKindNessReflector

} FTSpecialCollKind;

typedef enum FTCatchKind
{
    nFTCatchKindYoshiSpecialN,              // Yoshi's Egg Lay, also used for Kirby's copy ability version of it
    nFTCatchKindKirbySpecialN,              // Kirby's Inhale
    nFTCatchKindTaruCann,                   // Barrel Cannon
    nFTCatchKindTwister,                    // Tornado
    nFTCatchKindCommon,                     // Common grab
    nFTCatchKindCaptainSpecialHi            // Captain Falcon's Falcon Dive

} FTCatchKind;

typedef enum FTPartsPlacement
{
    nFTPartsPlacementLow,
    nFTPartsPlacementMiddle,
    nFTPartsPlacementHigh

} FTPartsPlacement;

typedef enum FTDamageKind
{
    nFTDamageKindDefault,                   // Default damage procedure
    nFTDamageKindStatus,                    // Set damage status
    nFTDamageKindColAnim,                   // Set damage ColAnim
    nFTDamageKindCatch,                     // Something with grabs?
    nFTDamageKindNone                       // No procedure

} FTDamageKind;

typedef enum FTPartsJointLabels
{
    nFTPartsJointTopN,
    nFTPartsJointTransN,
    nFTPartsJointXRotN,
    nFTPartsJointYRotN,
    nFTPartsJointCommonStart

} FTPartsJointLabels;

typedef enum FTPartsLevelDetail
{
    nFTPartsDetailNone,                         // Unused?
    nFTPartsDetailStart,                        // Start of actual detail values
    nFTPartsDetailHigh = nFTPartsDetailStart,   // Hi-poly
    nFTPartsDetailLow                           // Lo-poly
        
} FTPartsLevelDetail;

typedef enum FTKind
{
    nFTKindPlayableStart,
    nFTKindMario = nFTKindPlayableStart,
    nFTKindFox,
    nFTKindDonkey,
    nFTKindSamus,
    nFTKindLuigi,
    nFTKindLink,
    nFTKindYoshi,
    nFTKindCaptain,
    nFTKindKirby,
    nFTKindPikachu,
    nFTKindPurin,
    nFTKindNess,
    nFTKindPlayableEnd = nFTKindNess,

    nFTKindBoss,
    nFTKindMMario,

    nFTKindNStart,
    nFTKindNMario = nFTKindNStart,
    nFTKindNFox,
    nFTKindNDonkey,
    nFTKindNSamus,
    nFTKindNLuigi,
    nFTKindNLink,
    nFTKindNYoshi,
    nFTKindNCaptain,
    nFTKindNKirby,
    nFTKindNPikachu,
    nFTKindNPurin,
    nFTKindNNess,
    nFTKindNEnd = nFTKindNNess,

    nFTKindGDonkey,
    nFTKindEnumCount,
    nFTKindNull        // NONE

} FTKind;

typedef enum FTPlayerKind
{
    nFTPlayerKindMan,        // Player-controlled
    nFTPlayerKindCom,        // CPU-controlled
    nFTPlayerKindNot,        // N/A
    nFTPlayerKindDemo,       // Results Screen / Character Select Screen?
    nFTPlayerKindKey,        // Intro movie?
    nFTPlayerKindGameKey     // How to Play?

} FTPlayerKind;

typedef enum FTComputerCommandKind
{
    nFTComputerCommandButtonAPress,
    nFTComputerCommandButtonARelease,
    nFTComputerCommandButtonBPress,
    nFTComputerCommandButtonBRelease,
    nFTComputerCommandButtonZPress,
    nFTComputerCommandButtonZRelease,
    nFTComputerCommandButtonLPress,
    nFTComputerCommandButtonLRelease,
    nFTComputerCommandButtonStartPress,
    nFTComputerCommandButtonStartRelease,
    nFTComputerCommandStickX,
    nFTComputerCommandStickY,
    nFTComputerCommandMoveAuto,
    nFTComputerCommandStickXVar,
    nFTComputerCommandStickYVar,
    nFTComputerCommandEnumCount

} FTComputerCommandKind;

typedef enum FTComputerInputKind
{
    nFTComputerInputStickN,                                                // Reset stick to neutral
    nFTComputerInputMoveAuto,                                              // Move towards target (all directions)
    nFTComputerInputStickTiltAutoX,                                        // Move towards target (horizontally)
    nFTComputerInputStickNMoveAuto,                                        // Reset stick to neutral for one frame, then more towards target (all directions)
    nFTComputerInputMoveAutoStickTiltHiReleaseZ,                           // Used for many moves that involve slightly tilting the stick?
    nFTComputerInputStickTiltAutoXButtonA,                                 // Used for Forward Tilt
    nFTComputerInputStickTiltAutoXD5NButtonA,                              // Move towards target (horiztonally) for a duration of 5 frames then reset stick to neutral and press A
    nFTComputerInputMoveAutoButtonA,                                       // Move towards target (all directions) and press A
    nFTComputerInputStickSmashAutoXButtonA,                                // Move towards target's direction and press A (no Stick Y-Axis change)
    nFTComputerInputStickSmashAutoXButtonB,                                // Use Neutral Special in target's direction
    nFTComputerInputStickTiltAutoXNYD5SmashAutoXButtonB,                   // Move towards target for a duration of 5 frames then smash stick in target's direction and press B
    nFTComputerInputStickTiltAutoXNYD1ButtonB,                             // Move towards target for a duration of 1 frame then reset X-Axis to neutral and press B
    nFTComputerInputButtonZ1,                                              // Used for ledge roll (and more I suppose)
    nFTComputerInputStickSmashHiButtonB,                                   // Used for Up Special
    nFTComputerInputStickTiltAutoXD5SmashSButtonB,                         // Move towards target for a duration of 5 frames at half range, then do full range + B button
    nFTComputerInputStickNButtonL,                                         // Used for taunt
    nFTComputerInputButtonZ2,                                              // Why is this here again?
    nFTComputerInputStickTiltAutoXD5,                                      // Move towards target for a duration of 5 frames then end script 
    nFTComputerInputStickTiltAutoXD1,                                      // Move towards target for a duration of 1 frame then end script
    nFTComputerInputStickNButtonA,                                         // Used for Jab and Neutral Aerial
    nFTComputerInputStickTiltAutoXD5ButtonA,                               // Move towards target for a duration of 5 frames then press A
    nFTComputerInputStickSmashAutoXNYButtonA,                              // Used for Forward Smash, Forward Aerial and Back Aerial
    nFTComputerInputStickTiltAutoXD1SmashSButtonA,                         // Move towards target at half stick range for a duration of 1 frame, then do full range + A button and end script
    nFTComputerInputStickTiltHiButtonA,                                    // Used for Up Tilt
    nFTComputerInputStickTiltAutoXD5TiltAutoYButtonA,                      // Move towards target for a duration of 5 frames then do tilt attack in their Y direction
    nFTComputerInputStickSmashHiButtonA,                                   // Used for Up Smash and Up Aerial
    nFTComputerInputStickTiltAutoXD5SmashAutoYButtonA,                     // Move towards target for a duration of 5 frames then do smash attack in their Y direction
    nFTComputerInputStickSmashLwButtonB,                                   // Used for Down Special
    nFTComputerInputStickNButtonZButtonA,                                  // Used for grab
    nFTComputerInputStickTiltAutoXD5ButtonZButtonA,                        // Move towards target for a duration of 5 frames then use grab
    nFTComputerInputStickSmashL,                                           // Used for Throws
    nFTComputerInputStickSmashR,                                           // Used for Throws
    nFTComputerInputStickTiltLwButtonA,                                    // Used for Down Tilt
    nFTComputerInputStickTiltAutoXD5TiltLwButtonA,                         // Move towards target for a duration of 5 frames then use Down Tilt
    nFTComputerInputStickSmashLwButtonA,                                   // Used for Down Smash and Down Aerial
    nFTComputerInputStickNButtonZHold,                                     // Used to shield
    nFTComputerInputButtonZRelease,                                        // Used to let go of shield
    nFTComputerInputStickNXSmashLwButtonBReleaseBHold,                     // Used to repeatedly tap Down Special, is this Donkey Kong's Hand Slap move?
    nFTComputerInputStickNButtonBRelease,                                  // Let go of B button
    nFTComputerInputStickND1MoveAutoSmashLw,                               // Let go of control stick for a duration of 1 frame then do full range down; fast fall?
    nFTComputerInputStickNButtonBZReleaseAPress,                           // Used for ledge attack?
    nFTComputerInputStickTiltAutoXButtonBZReleaseAPress,                   // ???
    nFTComputerInputThrowItemImmediate,                                    // Throw item after a short wait?
    nFTComputerInputThrowItemWait,                                         // Throw item after a long wait?
    nFTComputerInputWiggle,                                                // Wiggle out of grab type moves (Cargo Throw, Inhale, Egg Lay)
    nFTComputerInputEscapeL,                                               // Roll left
    nFTComputerInputEscapeR,                                               // Roll right
    nFTComputerInputYoshiSpecialHiAim,                                     // Yoshi Up Special
    nFTComputerInputNessSpecialHiAim                                       // Ness Up Special

} FTComputerInputKind;

// The CPUs' "personality traits," overriding behavior if present (mostly 1P Game?)
typedef enum FTComputerTraitKind
{
    nFTComputerTraitDefault,           // "? Used in VS and for some 1P Game characters"
    nFTComputerTraitLink,              // Wait around for a while (1P Game Link)
    nFTComputerTraitYoshiTeam,         // Yoshi Team
    nFTComputerTraitKirbyTeam,         // Kirby (Is this specifically 1P Game Kirby Team?)
    nFTComputerTraitPolyTeam,          // Fighting Polygon Team (1P Game)
    nFTComputerTraitMarioBros,         // Mario & Luigi (1P Game)
    nFTComputerTraitGDonkey,       // Giant Donkey Kong (1P Game)
    nFTComputerTraitUnk1,              // Unknown
    nFTComputerTraitBonus3,            // Race to the Finish Fighting Polygons
    nFTComputerTraitAlly,              // Ally fighter in 1P Game
    nFTComputerTraitNone               // No overriding behavior (Training Mode)

} FTComputerTraitKind;

typedef enum FTComputerBehaviorKind
{
    nFTComputerBehaviorDefault,
    nFTComputerBehaviorUnk1,
    nFTComputerBehaviorUnk2,
    nFTComputerBehaviorAlly,
    nFTComputerBehaviorCaptain,
    nFTComputerBehaviorUnk3,
    nFTComputerBehaviorUnk4,
    nFTComputerBehaviorYoshiTeam,
    nFTComputerBehaviorKirbyTeam,
    nFTComputerBehaviorPolyTeam,
    nFTComputerBehaviorUnused1,
    nFTComputerBehaviorUnused2,
    nFTComputerBehaviorUnused3,
    nFTComputerBehaviorBonus3,
    nFTComputerBehaviorUnused4,
    nFTComputerBehaviorStand,
    nFTComputerBehaviorWalk,
    nFTComputerBehaviorEvade,
    nFTComputerBehaviorJump,
    nFTComputerBehaviorUnk5

} FTComputerBehaviorKind;

typedef enum FTComputerObjectiveKind
{
    nFTComputerObjectiveStand,         // CPU player will idle (?)
    nFTComputerObjectiveWalk,          // CPU player will walk around (?)
    nFTComputerObjectiveAttack,        // CPU player will try to attack its target position
    nFTComputerObjectiveEvade,         // CPU player will try to avoid contact with its target
    nFTComputerObjectiveRecover,       // CPU player will try to recover when offstage
    nFTComputerObjectiveTrackItem,     // CPU player will try to navigate to its target item and pick it up
    nFTComputerObjectiveUseItem,       // CPU player will try to use shootable, throwable or ground (?) item
    nFTComputerObjectiveCounterAttack, // CPU player will try to act just before landing from DamageFall? According to Smash Remix, this causes them to spam shield?
    nFTComputerObjectiveUnknown1,      // ???
    nFTComputerObjectiveAlly,          // ???
    nFTComputerObjectivePatrol,        // CPU player will walk around and attack opposing bystanders (not ally behavior, used in Kirby Team)
    nFTComputerObjectiveRush           // CPU player will try to relentlessly target opponents (Race to the Finish)

} FTComputerObjectiveKind;

typedef enum FTKeyEventKind
{
    nFTKeyEventEnd,
    nFTKeyEventButton,
    nFTKeyEventStick

} FTKeyEventKind;

// Structs
typedef struct FTStruct					            FTStruct;
typedef struct FTSpecialColl				        FTSpecialColl;
typedef struct FTItemPickup				            FTItemPickup;
typedef struct FTThrownStatus			            FTThrownStatus;
typedef struct FTThrownStatusArray		            FTThrownStatusArray;
typedef struct FTMotionDesc				            FTMotionDesc;
typedef struct FTMotionDescArray		            FTMotionDescArray;
typedef struct FTFileSize                           FTFileSize;
typedef struct FTData					            FTData;
typedef struct FTCommonPart                         FTCommonPart;
typedef struct FTCommonPartContainer                FTCommonPartContainer;
typedef struct FTModelPart				            FTModelPart;
typedef struct FTModelPartDesc			            FTModelPartDesc;
typedef struct FTModelPartContainer		            FTModelPartContainer;
typedef struct FTModelPartStatus	                FTModelPartStatus;
typedef struct FTTexturePart		                FTTexturePart;
typedef struct FTTexturePartContainer	            FTTexturePartContainer;
typedef struct FTTexturePartStatus                  FTTexturePartStatus;
typedef union  FTAnimDesc				            FTAnimDesc;
typedef struct FTMotionFlags		                FTMotionFlags;
typedef struct FTMotionScript                       FTMotionScript;
typedef struct FTMotionEventDefault                 FTMotionEventDefault;
typedef struct FTMotionEventDouble                  FTMotionEventDouble;
typedef struct FTMotionEventMakeAttack1             FTMotionEventMakeAttack1;
typedef struct FTMotionEventMakeAttack2             FTMotionEventMakeAttack2;
typedef struct FTMotionEventMakeAttack3             FTMotionEventMakeAttack3;
typedef struct FTMotionEventMakeAttack4             FTMotionEventMakeAttack4;
typedef struct FTMotionEventMakeAttack5             FTMotionEventMakeAttack5;
typedef struct FTMotionEventMakeAttack              FTMotionEventMakeAttack;
typedef struct FTMotionEventSetAttackOffset1        FTMotionEventSetAttackOffset1;
typedef struct FTMotionEventSetAttackOffset2        FTMotionEventSetAttackOffset2;
typedef struct FTMotionEventSetAttackOffset         FTMotionEventSetAttackOffset;
typedef struct FTMotionEventSetAttackCollDamage     FTMotionEventSetAttackCollDamage;
typedef struct FTMotionEventSetAttackCollSize       FTMotionEventSetAttackCollSize;
typedef struct FTMotionEventSetAttackCollSound      FTMotionEventSetAttackCollSound;
typedef struct FTMotionEventSetThrow1               FTMotionEventSetThrow1;
typedef struct FTMotionEventSetThrow2               FTMotionEventSetThrow2;
typedef struct FTMotionEventSetThrow                FTMotionEventSetThrow;
typedef struct FTMotionEventMakeEffect1             FTMotionEventMakeEffect1;
typedef struct FTMotionEventMakeEffect2             FTMotionEventMakeEffect2;
typedef struct FTMotionEventMakeEffect3             FTMotionEventMakeEffect3;
typedef struct FTMotionEventMakeEffect4             FTMotionEventMakeEffect4;
typedef struct FTMotionEventMakeEffect              FTMotionEventMakeEffect;
typedef struct FTMotionEventSetHitStatusPartID      FTMotionEventSetHitStatusPartID;
typedef struct FTMotionEventSetDamageCollPartID1    FTMotionEventSetDamageCollPartID1;
typedef struct FTMotionEventSetDamageCollPartID2    FTMotionEventSetDamageCollPartID2;
typedef struct FTMotionEventSetDamageCollPartID3    FTMotionEventSetDamageCollPartID3;
typedef struct FTMotionEventSetDamageCollPartID4    FTMotionEventSetDamageCollPartID4;
typedef struct FTMotionEventSetDamageCollPartID     FTMotionEventSetDamageCollPartID;
typedef struct FTMotionEventSubroutine1             FTMotionEventSubroutine1;
typedef struct FTMotionEventSubroutine2             FTMotionEventSubroutine2;
typedef struct FTMotionEventSubroutine              FTMotionEventSubroutine;
typedef struct FTMotionEventSetDamageThrown1        FTMotionEventSetDamageThrown1;
typedef struct FTMotionEventSetDamageThrown2        FTMotionEventSetDamageThrown2;
typedef struct FTMotionEventSetDamageThrown         FTMotionEventSetDamageThrown;
typedef struct FTMotionDamageScript                 FTMotionDamageScript;
typedef struct FTMotionEventGoto1                   FTMotionEventGoto1;
typedef struct FTMotionEventGoto2                   FTMotionEventGoto2;
typedef struct FTMotionEventGoto                    FTMotionEventGoto;
typedef struct FTMotionEventParallel1               FTMotionEventParallel1;
typedef struct FTMotionEventParallel2               FTMotionEventParallel2;
typedef struct FTMotionEventParallel                FTMotionEventParallel;
typedef struct FTMotionEventSetModelPartID          FTMotionEventSetModelPartID;
typedef struct FTMotionEventSetTexturePartID        FTMotionEventSetTexturePartID;
typedef struct FTMotionEventSetColAnimID            FTMotionEventSetColAnimID;
typedef struct FTMotionEventSetSlopeContour         FTMotionEventSetSlopeContour;
typedef struct FTMotionEventSetAfterImage           FTMotionEventSetAfterImage;
typedef struct FTMotionEventMakeRumble              FTMotionEventMakeRumble;
typedef struct FTMotionEventStopRumble              FTMotionEventStopRumble;
typedef struct FTStatusDesc				            FTStatusDesc;
typedef struct FTOpeningDesc		                FTOpeningDesc;
typedef struct FTThrowHitDesc			            FTThrowHitDesc;
typedef struct FTThrowReleaseDesc		            FTThrowReleaseDesc;
typedef struct FTDesc				                FTDesc;
typedef struct FTAttackMatrix                       FTAttackMatrix;
typedef struct FTAttackColl					        FTAttackColl;
typedef struct FTDamageCollDesc			            FTDamageCollDesc;
typedef struct FTDamageColl				            FTDamageColl;
typedef struct FTHitLog		                        FTHitLog;
typedef struct FTItemThrow				            FTItemThrow;
typedef struct FTItemSwing				            FTItemSwing;
typedef struct FTParts                              FTParts;
typedef struct FTHiddenPart			                FTHiddenPart;
typedef struct FTShadow                             FTShadow;
typedef struct FTCostume                            FTCostume;
typedef struct FTDemoDesc                           FTDemoDesc;
typedef struct FTSkeleton                           FTSkeleton;
typedef struct FTAfterImage				            FTAfterImage;
typedef struct FTCamera                             FTCamera;
typedef struct FTSprites                            FTSprites;
typedef struct FTComputer				            FTComputer;
typedef struct FTPlayerInput                        FTPlayerInput;
typedef struct FTComputerInput                      FTComputerInput;
typedef struct FTComputerAttack                     FTComputerAttack;
typedef union  FTKeyEvent                           FTKeyEvent;
typedef struct FTKey                                FTKey;
typedef struct FTAttributes				            FTAttributes;
typedef struct FTAccessPart		                    FTAccessPart;

#endif
