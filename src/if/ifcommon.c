#include <if/interface.h>
#include <ef/effect.h>
#include <ft/fighter.h>
#include <it/item.h>
#include <gr/ground.h>
#include <sc/scene.h>
#include <sys/video.h>
#include <sys/rdp.h>
#include <reloc_data.h>

extern alSoundEffect* func_800269C0_275C0(u16);

extern void syAudioSetBGMVolume(u32, u32);

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x8012EBF0 - Width of each digit?
s32 dIFCommonPlayerDamageDigitWidths[/* */] =
{
    14,  9, 15, 14,
    15, 13, 15, 14,
    15, 15, 17, 20
};

// 0x8012EC20 -> 0x8012EC38 - Player HUD digit colors
u8 dIFCommonPlayerDamageDigitColorsR[/* */] = { 0xFF, 0xF0, 0xF0, 0xFF, 0xFF }; // Hope the padding works here

// 0x8012EC28
u8 dIFCommonPlayerDamageDigitColorsG[/* */] = { 0xF0, 0xFF, 0xF0, 0xFF, 0xFF };

// 0x8012EC30
u8 dIFCommonPlayerDamageDigitColorsB[/* */] = { 0xF0, 0xF0, 0xFF, 0xFF, 0xFF };

// 0x8012EC38 - Player HUD position X offsets
s32 dIFCommonPlayerDamagePositionOffsetsX[/* */] = { 55, 125, 195, 265 };

// 0x8012EC48
s32 dIFCommonPlayerScorePositionOffsetsX[/* */] = { 0, 0, 0, 0 }; // Bruh, what's the point then lol

// 0x8012EC58 - Player emblem position X offsets
s32 dIFCommonPlayerDamageEmblemOffsetsX[/* */] = { 3, 3, 3, 3 };

// 0x8012EC68 - Player emblem position Y offsets
s32 dIFCommonPlayerDamageEmblemOffsetsY[/* */] = { -3, -3, -3, -3 };

// 0x8012EC78 - Player emblem scale values
f32 dIFCommonPlayerDamageEmblemScales[/* */] = { 1.0F, 1.0F, 1.0F, 1.0F };

// 0x8012EC88 - Player stock icon X offsets (when stock count <= 6)
s32 dIFCommonPlayerStocksIconOffsetsX[/* */] = { -24, -24, -24 ,-24 };

// 0x8012EC98 - Player stock digit X offsets (when stock count > 6)
s32 dIFCommonPlayerStocksDigitOffsetsX[/* */] = { 4, 4, 4, 4 };

// 0x8012ECA8
u8 dIFCommonTrafficSpriteColorsR[/* */] = { 0xFE, 0xFF, 0x4B, 0xFF, 0xFF, 0x22, 0xFF, 0xFF, 0xFF };

// 0x8012ECB4
u8 dIFCommonTrafficSpriteColorsG[/* */] = { 0x0C, 0xA2, 0x64, 0x38, 0xA2, 0x66, 0xFF, 0xFF, 0xFF };

// 0x8012ECC0
u8 dIFCommonTrafficSpriteColorsB[/* */] = { 0x0C, 0x00, 0xFF, 0x38, 0x00, 0xFE, 0xFF, 0xFF, 0xFF };

// 0x8012ECCC
u8 dIFCommonTrafficGoBacklightR[/* */] = { 0x00, 0x6A };

// 0x8012ECD0
u8 dIFCommonTrafficGoBacklightG[/* */] = { 0x00, 0x6A };

// 0x8012ECD4
u8 dIFCommonTrafficGoBacklightB[/* */] = { 0x00, 0x95 };

// 0x8012ECD8
u8 dIFCommonTrafficGoShadowR[/* */] = { 0x00, 0x12 };

// 0x8012ECDC
u8 dIFCommonTrafficGoShadowG[/* */] = { 0x00, 0x12 };

// 0x8012ECE0
u8 dIFCommonTrafficGoShadowB[/* */] = { 0x00, 0x2E };

// 0x8012ECE4
IFTraffic dIFCommonTrafficSpriteData[/* */] =
{
    { 123, -13, 0x00 },
    { 140, -11, 0x01 },
    { 153, -11, 0x01 },
    { 166, -11, 0x01 },
    { 180, -15, 0x02 },
    { 107, -33, 0x03 },
    { 119,  21, 0x04 },
    { 132,  21, 0x04 },
    { 145,  21, 0x04 },
    { 162,  20, 0x05 },
    { 115, -26, 0x06 },
    { 131,  32, 0x07 },
    { 144,  32, 0x07 },
    { 157,  32, 0x07 },
    { 167,  23, 0x08 }
};

// 0x8012ED40
intptr_t dIFCommonTrafficSpriteOffsets[/* */] =
{
    &llIFCommonGameStatusLampRedDimSprite,     &llIFCommonGameStatusLampYellowDimSprite,     &llIFCommonGameStatusLampBlueDimSprite,
    &llIFCommonGameStatusLampRedContourSprite, &llIFCommonGameStatusLampYellowContourSprite, &llIFCommonGameStatusLampBlueContourSprite,
    &llIFCommonGameStatusLampRedLightSprite,   &llIFCommonGameStatusLampYellowLightSprite,   &llIFCommonGameStatusLampBlueLightSprite
};

// 0x8012ED64 - Announcer text: "GO!"
IFACharacter dIFCommonAnnounceGoSpriteData[/* */] =
{
    {  82, 93, &llIFCommonGameStatusOrangeLetterGSprite         },
    { 144, 93, &llIFCommonGameStatusOrangeLetterOSprite         },
    { 214, 93, &llIFCommonGameStatusOrangeExclamationMarkSprite }
};

// 0x8012ED7C
IFACharacter dIFCommonAnnounceSuddenDeathSpriteData[/* */] =
{
    {  74,  67, &llIFCommonAnnounceCommonLetterSSprite       },
    { 102,  67, &llIFCommonAnnounceCommonLetterUSprite       },
    { 132,  67, &llIFCommonAnnounceCommonLetterDSprite       },
    { 163,  67, &llIFCommonAnnounceCommonLetterDSprite       },
    { 193,  67, &llIFCommonAnnounceCommonLetterESprite       },
    { 217,  67, &llIFCommonAnnounceCommonLetterNSprite       },
    {  83, 113, &llIFCommonAnnounceCommonLetterDSprite       },
    { 113, 113, &llIFCommonAnnounceCommonLetterESprite       },
    { 135, 113, &llIFCommonAnnounceCommonLetterASprite       },
    { 165, 113, &llIFCommonAnnounceCommonLetterTSprite       },
    { 192, 113, &llIFCommonAnnounceCommonLetterHSprite       },
    { 227, 113, &llIFCommonAnnounceCommonSymbolExclaimSprite }
};

// 0x8012EDDC
SYColorRGBPair dIFCommonAnnounceSuddenDeathSpriteColors = { { 0xFF, 0xFF, 0xFF }, { 0x00, 0x00, 0x00 } };

// 0x8012EDE4
IFACharacter dIFCommonAnnounceTimeUpSpriteData[/* */] =
{
    {  45, 95, &llIFCommonGameStatusBlueLetterTSprite },
    {  82, 95, &llIFCommonGameStatusBlueLetterISprite },
    { 100, 95, &llIFCommonGameStatusBlueLetterMSprite },
    { 151, 95, &llIFCommonGameStatusBlueLetterESprite },
    { 195, 95, &llIFCommonGameStatusBlueLetterUSprite },
    { 238, 95, &llIFCommonGameStatusBlueLetterPSprite }
};

// 0x8012EE14
IFACharacter dIFCommonAnnounceGameSetSpriteData[/* */] =
{
    {  22, 95, &llIFCommonGameStatusBlueLetterGSprite },
    {  62, 95, &llIFCommonGameStatusBlueLetterASprite },
    { 104, 95, &llIFCommonGameStatusBlueLetterMSprite },
    { 154, 95, &llIFCommonGameStatusBlueLetterESprite },
    { 191, 95, &llIFCommonGameStatusBlueLetterSSprite },
    { 230, 95, &llIFCommonGameStatusBlueLetterESprite },
    { 262, 95, &llIFCommonGameStatusBlueLetterTSprite }
};

// 0x8012EE4C
u16 dIFCommonEntryFocusSleepTics[/* */] =
{
    22,
    15,
    60
};

// 0x8012EE54
s32 dIFCommonTimerDigitsSpritePositionsX[/* */] =
{
    232, 247, 273, 288
};

// 0x8012EE64 - Offset of twelve digits: numbers 0 through 9, % sign and H.P. text
intptr_t dIFCommonPlayerDamageDigitSpriteOffsets[/* */] =
{
    &llIFCommonPlayerDamageDigit0Sprite,
    &llIFCommonPlayerDamageDigit1Sprite,
    &llIFCommonPlayerDamageDigit2Sprite,
    &llIFCommonPlayerDamageDigit3Sprite,
    &llIFCommonPlayerDamageDigit4Sprite,
    &llIFCommonPlayerDamageDigit5Sprite,
    &llIFCommonPlayerDamageDigit6Sprite,
    &llIFCommonPlayerDamageDigit7Sprite,
    &llIFCommonPlayerDamageDigit8Sprite,
    &llIFCommonPlayerDamageDigit9Sprite,
    &llIFCommonPlayerDamageSymbolPercentSprite,
    &llIFCommonPlayerDamageSymbolHPSprite
};

// 0x8012EE94
intptr_t dIFCommonTimerDigitSpriteOffsets[/* */] =
{
    &llIFCommonTimerDigit0Sprite,
    &llIFCommonTimerDigit1Sprite,
    &llIFCommonTimerDigit2Sprite,
    &llIFCommonTimerDigit3Sprite,
    &llIFCommonTimerDigit4Sprite,
    &llIFCommonTimerDigit5Sprite,
    &llIFCommonTimerDigit6Sprite,
    &llIFCommonTimerDigit7Sprite,
    &llIFCommonTimerDigit8Sprite,
    &llIFCommonTimerDigit9Sprite,
    &llIFCommonTimerSymbolColonSprite,
    &llIFCommonTimerSymbolSecSprite,
    &llIFCommonTimerSymbolCSecSprite
};

// 0x8012EEC8
intptr_t dIFCommonPlayerStockDigitSpriteOffsets[/* */] =
{
    &llIFCommonDigits0Sprite,
    &llIFCommonDigits1Sprite,
    &llIFCommonDigits2Sprite,
    &llIFCommonDigits3Sprite,
    &llIFCommonDigits4Sprite,
    &llIFCommonDigits5Sprite,
    &llIFCommonDigits6Sprite,
    &llIFCommonDigits7Sprite,
    &llIFCommonDigits8Sprite,
    &llIFCommonDigits9Sprite,
    &llIFCommonDigitsCrossSprite
};

// 0x8012EEF8
Gfx dIFCommonPlayerArrowsDisplayList[/* */] =
{
    gsSPClearGeometryMode(G_ZBUFFER),
    gsDPPipeSync(),
    gsDPSetRenderMode(G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2),
    gsDPSetAlphaCompare(G_AC_THRESHOLD),
    gsDPSetBlendColor(0x00, 0x00, 0x00, 0x08),
    gsDPSetPrimColor(0, 0, 0xFF, 0x00, 0x00, 0x80),
    gsDPSetCombineMode(G_CC_PRIMITIVE, G_CC_PRIMITIVE),
    gsSPEndDisplayList()
};

// 0x8012EF38 - Length of time units in order: minute tens, minute ones, second tens, second ones (in frames)
u16 dIFCommonTimerDigitsUnitLengths[/* */] = 
{ 
    I_MIN_TO_TICS(10), 
    I_MIN_TO_TICS(1), 
    I_SEC_TO_TICS(10), 
    I_SEC_TO_TICS(1) 
};

// 0x8012EF40
u8 dIFCommonPlayerTeamColorIDs[/* */] = { 0, 1, 3, 4, 0 };

// 0x8012EF48
u16 dIFCommonAnnounceTimerVoiceIDs[/* */] = 
{
    nSYAudioVoiceAnnounceOne,
    nSYAudioVoiceAnnounceTwo, 
    nSYAudioVoiceAnnounceThree, 
    nSYAudioVoiceAnnounceFour,
    nSYAudioVoiceAnnounceFive
};

// 0x8012EF54
intptr_t dIFCommonBattlePausePlayerNumSpriteOffsets[/* */] =
{
    &llIFCommonBattlePausePlayerNum1PSprite,
    &llIFCommonBattlePausePlayerNum2PSprite,
    &llIFCommonBattlePausePlayerNum3PSprite,
    &llIFCommonBattlePausePlayerNum4PSprite
};

// 0x8012EF64
u8 dIFCommonPlayerMagnifyColorsR[/* */] = { 0xEF, 0x00, 0xFF, 0x00, 0xFF };

// 0x8012EF6C
u8 dIFCommonPlayerMagnifyColorsG[/* */] = { 0x0D, 0x00, 0xE1, 0xFF, 0xFF };

// 0x8012EF74
u8 dIFCommonPlayerMagnifyColorsB[/* */] = { 0x17, 0xFF, 0x00, 0x00, 0xFF };

// 0x8012EF7C
u8 dIFCommonPlayerTagPrimColorsR[/* */] = { 0xED, 0x4E, 0xFF, 0x4E, 0xAC };

// 0x8012EF84
u8 dIFCommonPlayerTagPrimColorsG[/* */] = { 0x36, 0x4E, 0xDF, 0xB9, 0xAC };

// 0x8012EF8C
u8 dIFCommonPlayerTagPrimColorsB[/* */] = { 0x36, 0xE9, 0x1A, 0x4E, 0xAC };

// 0x8012EF94
u8 dIFCommonPlayerTagEnvColorsR[/* */] = { 0x00, 0x00, 0x00, 0x00, 0x00 };

// 0x8012EF9C
u8 dIFCommonPlayerTagEnvColorsG[/* */] = { 0x00, 0x00, 0x00, 0x00, 0x00 };

// 0x8012EFA4
u8 dIFCommonPlayerTagEnvColorsB[/* */] = { 0x00, 0x00, 0x00, 0x00, 0x00 };

// 0x8012EFAC
intptr_t dIFCommonPlayerTagSpriteOffsets[/* */] =
{
    &llIFCommonPlayerTags1PSprite,
    &llIFCommonPlayerTags2PSprite,
    &llIFCommonPlayerTags3PSprite,
    &llIFCommonPlayerTags4PSprite,
    &llIFCommonPlayerTagsCPSprite,
    &llIFCommonPlayerTagsAllySprite
};

// 0x8012EFC4
IFPauseDecal dIFCommonBattlePauseDecalsSpriteData[/* */] =
{
    { &llIFCommonBattlePauseDecalPauseSprite,          { 232, 191 }, { { 0xFF, 0xFF, 0xFF }, { 0x00, 0x00, 0x00 } } },
    { &llIFCommonBattlePauseDecalAButtonSprite,        {  99, 203 }, { { 0x00, 0x95, 0xFF }, { 0x00, 0x05, 0xC7 } } },
    { &llIFCommonBattlePauseDecalBButtonSprite,        { 122, 203 }, { { 0x36, 0xBF, 0x00 }, { 0x00, 0x30, 0x00 } } },
    { &llIFCommonBattlePauseDecalZTriggerSprite,       { 145, 202 }, { { 0x80, 0x80, 0x80 }, { 0x21, 0x21, 0x21 } } },
    { &llIFCommonBattlePauseDecalRTriggerSprite,       { 164, 203 }, { { 0x80, 0x80, 0x80 }, { 0x21, 0x21, 0x21 } } },
    { &llIFCommonBattlePauseDecalPlusSprite,           { 113, 206 }, { { 0xFF, 0xFF, 0xFF }, { 0x00, 0x00, 0x00 } } },
    { &llIFCommonBattlePauseDecalPlusSprite,           { 136, 206 }, { { 0xFF, 0xFF, 0xFF }, { 0x00, 0x00, 0x00 } } },
    { &llIFCommonBattlePauseDecalPlusSprite,           { 155, 206 }, { { 0xFF, 0xFF, 0xFF }, { 0x00, 0x00, 0x00 } } },
    { &llIFCommonBattlePauseDecalResetSprite,          { 182, 205 }, { { 0xFF, 0xFF, 0xFF }, { 0x00, 0x00, 0x00 } } },
    { &llIFCommonBattlePauseDecalSmashBallSprite,      { 198, 191 }, { { 0xFF, 0xFF, 0xFF }, { 0x00, 0x00, 0x00 } } },
    { &llIFCommonBattlePauseDecalArrowsSprite,         {  21,  19 }, { { 0xFF, 0x00, 0x00 }, { 0x00, 0x00, 0x00 } } },
    { &llIFCommonBattlePauseDecalControlStickSprite,   {  31,  29 }, { { 0xFF, 0xFF, 0xFF }, { 0x14, 0x18, 0x11 } } },
    { &llIFCommonBattlePauseDecalLTriggerSprite,       {  34, 203 }, { { 0x80, 0x80, 0x80 }, { 0x21, 0x21, 0x21 } } },
    { &llIFCommonBattlePauseDecalRetrySprite,          {  51, 205 }, { { 0xFF, 0xFF, 0xFF }, { 0x00, 0x00, 0x00 } } }
};

// 0x8012F0A4
SYRectangle dIFCommonBattlePauseBorderRectangle[/* */] =
{
    {  26,  24, 294,  26 },
    {  26,  24,  28, 199 },
    {  26, 197, 190, 199 },
    { 292,  24, 294, 199 },
    { 279, 197, 294, 199 }
};

// 0x8012F0F4
IFACharacter dIFCommonAnnounceFailureSpriteData[/* */] =
{
    {  77, 101, &llIFCommonAnnounceCommonLetterFSprite },
    {  97, 101, &llIFCommonAnnounceCommonLetterASprite },
    { 130, 101, &llIFCommonAnnounceCommonLetterISprite },
    { 145, 101, &llIFCommonAnnounceCommonLetterLSprite },
    { 167, 101, &llIFCommonAnnounceCommonLetterUSprite },
    { 197, 101, &llIFCommonAnnounceCommonLetterRSprite },
    { 225, 101, &llIFCommonAnnounceCommonLetterESprite }
};

// 0x8012F12C
SYColorRGBPair dIFCommonAnnounceFailureSpriteColors = { { 0xFF, 0xFF, 0xFF }, { 0x00, 0x00, 0xFF } };

// 0x8012F134
IFACharacter dIFCommonAnnounceCompleteSpriteData[/* */] =
{
    {  46, 101, &llIFCommonAnnounceCommonLetterCSprite       },
    {  71, 101, &llIFCommonAnnounceCommonLetterOSprite       },
    { 104, 100, &llIFCommonAnnounceCommonLetterMSprite       },
    { 143, 101, &llIFCommonAnnounceCommonLetterPSprite       },
    { 168, 101, &llIFCommonAnnounceCommonLetterLSprite       },
    { 189, 101, &llIFCommonAnnounceCommonLetterESprite       },
    { 212, 101, &llIFCommonAnnounceCommonLetterTSprite       },
    { 237, 101, &llIFCommonAnnounceCommonLetterESprite       },
    { 267, 101, &llIFCommonAnnounceCommonSymbolExclaimSprite }
};

// 0x8012F17C
SYColorRGBPair dIFCommonAnnounceCompleteSpriteColors = { { 0xFF, 0xFF, 0xFF }, { 0xFF, 0x00, 0x00 } };

// 0x8012F184
u16 dIFCommonAnnounceDefeatedVoiceIDs[/* */] =
{
    nSYAudioVoiceAnnouncePlayer1,
    nSYAudioVoiceAnnouncePlayer2,
    nSYAudioVoiceAnnouncePlayer3,
    nSYAudioVoiceAnnouncePlayer4
};

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x80131570 - Padding?
s32 sIFCommonPad0x80131570[4];

// 0x80131580
IFPlayerCommon gIFCommonPlayerInterface;

// 0x80131598
IFPlayerDamage sIFCommonPlayerDamageInterface[GMCOMMON_PLAYERS_MAX];

// 0x80131748
IFPlayerMagnify sIFCommonPlayerMagnifyInterface[GMCOMMON_PLAYERS_MAX];

// 0x801317C8 - Values of digits displayed on the match timer
u8 sIFCommonTimerDigitsInterface[4];

// 0x801317CC - This might be part of another struct
s8 sIFCommonPlayerStocksNum[GMCOMMON_PLAYERS_MAX];

// 0x801317D0
GObj *sIFCommonPlayerStocksGObj[GMCOMMON_PLAYERS_MAX];

// 0x801317E0 - Identical to gSCManagerBattleState->time_remain; the countdown timer adds one second once it has begun decrementing; s32 or u32?
u32 sIFCommonTimerLimit;

// 0x801317E4 - ID of player who paused
u8 sIFCommonBattlePausePlayer;

// 0x801317E5 - Poly-mode of pausing player?
u8 sIFCommonBattlePausePlayerDetail;

// 0x801317E6 - Number of frames the camera takes to revert to its pre-pause position
u16 sIFCommonBattlePauseCameraRestoreWait;

// 0x801317E8 - Eye X angle of camera before and after pausing
f32 sIFCommonBattlePauseCameraEyeXOrigin;

// 0x801317EC - Eye Y angle of camera before and after pausing
f32 sIFCommonBattlePauseCameraEyeYOrigin;

// 0x801317F0 - Sprite of red arrow indicator for grabbable items
Sprite *sIFCommonItemArrowSprite;

// 0x801317F4 - Number of unique teams in-game minus one?
s32 sIFCommonBattlePlace;

// 0x801317F8 - Padding?
s32 sIFCommonPad0x801317F8;

// 0x801317FC
u32 sIFCommonTimerStamp;

// 0x80131800
u32 sIFCommonTimerIsStarted; // Confirmed u32 by ifCommonBattleUpdateInterfaceAll

// 0x80131804 - Padding?
s32 sIFCommonPad0x80131804;

// 0x80131808 - Array of sound effect IDs to play on game end
u16 sIFCommonBattleEndSoundsQueue[16];

// 0x80131828 - What kind of pause menu to display
u8 sIFCommonBattlePauseKindInterface;

// 0x80131829 - Number of sound effects queued to play on game end
u8 sIFCommonBattleEndSoundsNum;

// 0x8013182C
void (*sIFCommonBattleInterfaceProcUpdate)();

// 0x80131830
void (*sIFCommonBattleInterfaceProcSet)();

// 0x80131834 - Padding?
s32 sIFCommonPad0x80131834;

// 0x80131838
IFPlayerSteal sIFCommonPlayerStealInterface[GMCOMMON_PLAYERS_MAX];

// 0x80131858
u8 sIFCommonPlayerMagnifySoundWait;

// 0x80131859
u8 dIFCommonBattleBossUpdateInterval;

// 0x8013185A
u8 dIFCommonBattleBossUpdateWait;

// 0x8013185C - Whether each second in the 5-second countdown before time-up has been announced
ub8 sIFCommonIsAnnouncedSecond[5];

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8010E690
void ifCommonPlayerDamageSetShowInterface(void)
{
    s32 i;

    for (i = 0; i < ARRAY_COUNT(sIFCommonPlayerDamageInterface); i++)
    {
        if (sIFCommonPlayerDamageInterface[i].interface_gobj != NULL)
        {
            sIFCommonPlayerDamageInterface[i].is_show_interface = TRUE;
        }
    }
}

// 0x8010E700 - Gets position of special character (% or H.P.) in damage display character array?
s32 ifCommonPlayerDamageGetSpecialArrayID(s32 damage, u8 *digits)
{
    u8 *digits_start = digits;
    s32 digit_update = 1;

    if (damage >= 10)
    {
        do
        {
            digit_update *= 10;
        }
        while ((damage / digit_update) >= 10);
    }
    do
    {
        *digits++ = damage / digit_update;

        damage %= digit_update;

        digit_update /= 10;
    }
    while (digit_update != 0);

    return digits - digits_start;
}

// 0x8010E7D4
s32 ifCommonPlayerDamageGetPercentArrayID(s32 damage, u8 *digits)
{
    s32 id = ifCommonPlayerDamageGetSpecialArrayID(damage, digits);

    digits[id] = 0xA;

    return id + 1;
}

// 0x8010E808
s32 ifCommonPlayerDamageGetHitPointsArrayID(s32 damage, u8 *digits)
{
    s32 id = ifCommonPlayerDamageGetSpecialArrayID(damage, digits);

    digits[id] = 0xB;

    return id + 1;
}

// 0x8010E83C
s32 ifCommonPlayerDamageGetDigitOffset(s32 digit_count, u8 *digit_ids)
{
    s32 i, offset = 0;

    for (i = 0; i < digit_count; i++)
    {
        offset += dIFCommonPlayerDamageDigitWidths[digit_ids[i]];
    }
    return offset;
}

// 0x8010E8F4
void ifCommonPlayerDamageUpdateDigits(GObj *interface_gobj)
{
    s32 player;
    IFDCharacter *ifchar;
    SObj *sobj;
    s32 damage_scale;
    s32 start_damage;
    s32 char_count;
    s32 hitpoints;
    s32 digit_id;
    s32 color_id; // I don't know
    s32 pos_adjust_wait;
    s32 flash_reset_wait;
    s32 unused[2];
    f32 scale;
    u8 digits[4];
    s32 sprite_id;
    f32 offset;
    s32 damage;
    f32 pos_x;

    player = ifGetPlayer(interface_gobj);
    damage = start_damage = gSCManagerBattleState->players[player].stock_damage_all;

    if (damage > 999)
    {
        damage = 999;
    }
    scale = sIFCommonPlayerDamageInterface[player].scale;
    damage_scale = start_damage - sIFCommonPlayerDamageInterface[player].damage;

    if (damage_scale == 0)
    {
        if (scale == 1.0F)
        {
            return;
        }
    }
    else if (damage_scale < 0)
    {
        scale = 1.0F;
    }
    else
    {
        sIFCommonPlayerDamageInterface[player].pos_adjust_wait = 4;
        sIFCommonPlayerDamageInterface[player].flash_reset_wait = 1;

        scale = (damage_scale / 300.0F) + 1.0F;
    }
    pos_adjust_wait = sIFCommonPlayerDamageInterface[player].pos_adjust_wait;
    flash_reset_wait = sIFCommonPlayerDamageInterface[player].flash_reset_wait;

    if (flash_reset_wait != 0)
    {
        color_id = GMCOMMON_PLAYERS_MAX;
    }
    else color_id = player;

    if (gSCManagerBattleState->players[player].fkind == nFTKindBoss)
    {
        hitpoints = 300 - damage;

        if (hitpoints < 0)
        {
            hitpoints = 0;
        }
        sIFCommonPlayerDamageInterface[player].char_display_count = char_count = ifCommonPlayerDamageGetHitPointsArrayID(hitpoints, digits);
    }
    else sIFCommonPlayerDamageInterface[player].char_display_count = char_count = ifCommonPlayerDamageGetPercentArrayID(damage, digits);

    pos_x = (ifCommonPlayerDamageGetDigitOffset(char_count, digits) * scale * 0.5F);

    pos_x = gIFCommonPlayerInterface.player_pos_x[player] + pos_x;

    if ((scale > 1.0F) && (pos_adjust_wait == 0))
    {
        scale -= 0.05F;

        if (scale < 1.0F)
        {
            scale = 1.0F;
        }
    }
    digit_id = char_count - 1;

    sobj = SObjGetStruct(interface_gobj)->next;

    while (sobj != NULL)
    {
        if (digit_id < 0)
        {
            sobj->sprite.attr |= SP_HIDDEN;
        }
        else
        {
            sprite_id = digits[digit_id];
            ifchar = sobj->user_data.p;

            ifchar->image_id = sprite_id;

            offset = dIFCommonPlayerDamageDigitWidths[sprite_id] * scale;

            ifchar->pos.x = (pos_x - (offset * 0.5F));
            ifchar->pos.y = gIFCommonPlayerInterface.player_pos_y;

            pos_x -= offset;

            sobj->sprite.attr &= ~SP_HIDDEN;
        }
        sobj = sobj->next;

        digit_id--;
    }
    if (pos_adjust_wait > 0)
    {
        pos_adjust_wait--;
    }
    if (flash_reset_wait > 0)
    {
        flash_reset_wait--;
    }
    sIFCommonPlayerDamageInterface[player].damage = start_damage;
    sIFCommonPlayerDamageInterface[player].scale = scale;
    sIFCommonPlayerDamageInterface[player].color_id = color_id;

    sIFCommonPlayerDamageInterface[player].pos_adjust_wait = pos_adjust_wait;
    sIFCommonPlayerDamageInterface[player].flash_reset_wait = flash_reset_wait;
}

// 0x8010EC50
void ifCommonPlayerDamageUpdateAnim(GObj *interface_gobj)
{
    s32 player;
    s32 char_id;
    s32 random;
    s32 modulo;
    s32 i, j;
    IFDCharacter *ifchar;
    SObj *sobj;

    player = ifGetPlayer(interface_gobj);

    if (sIFCommonPlayerDamageInterface[player].break_anim_frame < 19)
    {
        modulo = sIFCommonPlayerDamageInterface[player].break_anim_frame / 6;

        if (!(sIFCommonPlayerDamageInterface[player].break_anim_frame - modulo * 6))
        {
            char_id = sIFCommonPlayerDamageInterface[player].char_display_count - modulo;

            if (char_id > 0)
            {
                random = syUtilsRandIntRange(char_id);

                for (i = j = 0; i < sIFCommonPlayerDamageInterface[player].char_display_count; i++)
                {
                    if (sIFCommonPlayerDamageInterface[player].chars[i].is_lock_movement == FALSE)
                    {
                        if (j == random)
                        {
                            break;
                        }
                        else j++;
                    }
                }
                sIFCommonPlayerDamageInterface[player].chars[i].is_lock_movement = TRUE;
            }
        }
        sIFCommonPlayerDamageInterface[player].break_anim_frame++;
    }
    sobj = SObjGetStruct(interface_gobj)->next;

    while (sobj != NULL)
    {
        if (!(sobj->sprite.attr & SP_HIDDEN))
        {
            ifchar = sobj->user_data.p;

            if (ifchar->is_lock_movement != FALSE)
            {
                ifchar->vel.y++;

                ifchar->pos.x += ifchar->vel.x;
                ifchar->pos.y += ifchar->vel.y;
            }
        }
        sobj = sobj->next;
    }
}

// 0x8010EE18
void ifCommonPlayerDamageProcUpdate(GObj *interface_gobj)
{
    s32 player = ifGetPlayer(interface_gobj);

    if (gSCManagerBattleState->players[player].stock_count == -1)
    {
        if (sIFCommonPlayerDamageInterface[player].dead_stopupdate_wait != 0)
        {
            if (sIFCommonPlayerDamageInterface[player].is_update_anim != FALSE)
            {
                ifCommonPlayerDamageUpdateAnim(interface_gobj);
            }
            sIFCommonPlayerDamageInterface[player].dead_stopupdate_wait--;
        }
    }
    else if (sIFCommonPlayerDamageInterface[player].is_update_anim != FALSE)
    {
        ifCommonPlayerDamageUpdateAnim(interface_gobj);
    }
    else ifCommonPlayerDamageUpdateDigits(interface_gobj);
}

// 0x8010EEFC
void ifCommonPlayerDamageProcDisplay(GObj *interface_gobj)
{
    f32 pos_x;
    f32 scale;
    f32 pos_y;
    f32 damage_scale;
    s32 player;
    s32 color_id;
    u8 color_r;
    u8 color_g;
    u8 color_b;
    SObj *sobj, *sub_sobj;
    IFDCharacter *ifchar;

    sobj = SObjGetStruct(interface_gobj);

    lbCommonPrepSObjAttr(gSYTaskmanDLHeads, sobj);
    lbCommonPrepSObjDraw(gSYTaskmanDLHeads, sobj);

    lbCommonSetExternSpriteParams(&sobj->sprite);

    player = ifGetPlayer(interface_gobj);

    if
    (
        (sIFCommonPlayerDamageInterface[player].is_show_interface != FALSE) &&
        (
            (gSCManagerBattleState->players[player].stock_count >= 0)       ||
            (sIFCommonPlayerDamageInterface[player].dead_stopupdate_wait != 0)
        )
    )
    {
        color_id = sIFCommonPlayerDamageInterface[player].color_id;
        scale = sIFCommonPlayerDamageInterface[player].scale;

        if (color_id == GMCOMMON_PLAYERS_MAX)
        {
            color_r = dIFCommonPlayerDamageDigitColorsR[color_id];
            color_g = dIFCommonPlayerDamageDigitColorsG[color_id];
            color_b = dIFCommonPlayerDamageDigitColorsB[color_id];
        }
        else
        {
            damage_scale = 1.0F - (sIFCommonPlayerDamageInterface[player].damage / 300.0F);

            if (damage_scale < 0.0F)
            {
                damage_scale = 0.0F;
            }
            color_r = (s32) ((dIFCommonPlayerDamageDigitColorsR[color_id] - 0x64) * damage_scale) + 0x64;
            color_g = (s32) ((dIFCommonPlayerDamageDigitColorsG[color_id] - 0x14) * damage_scale) + 0x14;
            color_b = (s32) ((dIFCommonPlayerDamageDigitColorsB[color_id] - 0x14) * damage_scale) + 0x14;
        }
        sobj = sobj->next;
        ifchar = sobj->user_data.p;

        sobj->sprite = *lbRelocGetFileData(Sprite*, gGMCommonFiles[2], dIFCommonPlayerDamageDigitSpriteOffsets[ifchar->image_id]);

        sobj->pos.x = (ifchar->pos.x - (sobj->sprite.width * 0.5F * scale));
        sobj->pos.y = (ifchar->pos.y - (sobj->sprite.height * 0.5F * scale));

        sobj->sprite.scalex = scale;
        sobj->sprite.scaley = scale;

        sobj->sprite.red = color_r;
        sobj->sprite.green = color_g;
        sobj->sprite.blue = color_b;

        lbCommonPrepSObjAttr(gSYTaskmanDLHeads, sobj);

        if (color_id == GMCOMMON_PLAYERS_MAX)
        {
            gDPSetCombineLERP(gSYTaskmanDLHeads[0]++, 0, 0, 0, PRIMITIVE, 0, 0, 0, TEXEL0, 0, 0, 0, PRIMITIVE, 0, 0, 0, TEXEL0);
        }
        lbCommonPrepSObjDraw(gSYTaskmanDLHeads, sobj);

        sobj = sobj->next;

        while (sobj != NULL)
        {
            if (!(sobj->sprite.attr & SP_HIDDEN))
            {
                ifchar = sobj->user_data.p;

                sobj->sprite = *lbRelocGetFileData(Sprite*, gGMCommonFiles[2], dIFCommonPlayerDamageDigitSpriteOffsets[ifchar->image_id]);

                pos_x = ifchar->pos.x - (sobj->sprite.width * 0.5F * scale);
                pos_y = ifchar->pos.y - (sobj->sprite.height * 0.5F * scale);

                if ((scale == 1.0F) && (sIFCommonPlayerDamageInterface[player].is_update_anim == FALSE))
                {
                    sobj->pos.x = (s32)pos_x;
                    sobj->pos.y = (s32)pos_y;
                }
                else
                {
                    sobj->pos.x = pos_x;
                    sobj->pos.y = pos_y;
                }
                sobj->sprite.scalex = sobj->sprite.scaley = scale;

                lbCommonPrepSObjDraw(gSYTaskmanDLHeads, sobj);
            }
            sobj = sobj->next;
        }
        lbCommonClearExternSpriteParams();
    }
}

// 0x8010F334
void ifCommonPlayerDamageSetDigitAttr(void)
{
    s32 i;

    for (i = 0; i < ARRAY_COUNT(dIFCommonPlayerDamageDigitSpriteOffsets); i++)
    {
        lbRelocGetFileData(Sprite*, gGMCommonFiles[2], dIFCommonPlayerDamageDigitSpriteOffsets[i])->attr = SP_TEXSHUF | SP_TRANSPARENT;
    }
}

// 0x8010F3A0
void ifCommonPlayerDamageSetDigitPositions(void)
{
    gIFCommonPlayerInterface.player_pos_x = dIFCommonPlayerDamagePositionOffsetsX;
    gIFCommonPlayerInterface.player_pos_y = 210;
}

// 0x8010F3C0
void ifCommonPlayerDamageInitInterface(void)
{
    FTStruct *fp;
    FTSprites *ft_sprites;
    GObj *interface_gobj;
    SObj *sobj;
    s32 player;
    s32 emblem;

    ifCommonPlayerDamageSetDigitAttr();

    for (player = 0; player < ARRAY_COUNT(sIFCommonPlayerDamageInterface); player++)
    {
        if (gSCManagerBattleState->players[player].pkind == nFTPlayerKindNot)
        {
            sIFCommonPlayerDamageInterface[player].interface_gobj = NULL;
        }
        else
        {
            interface_gobj = gcMakeGObjSPAfter(nGCCommonKindInterface, NULL, nGCCommonLinkIDInterface, GOBJ_PRIORITY_DEFAULT);

            sIFCommonPlayerDamageInterface[player].interface_gobj = interface_gobj;

            gcAddGObjDisplay(interface_gobj, ifCommonPlayerDamageProcDisplay, 23, GOBJ_PRIORITY_DEFAULT, ~0);

            fp = ftGetStruct(gSCManagerBattleState->players[player].fighter_gobj);

            ft_sprites = fp->attr->sprites;

            if ((ft_sprites != NULL) && (ft_sprites->emblem != NULL))
            {
                sobj = lbCommonMakeSObjForGObj(interface_gobj, ft_sprites->emblem);

                sobj->pos.x = (s32)
                (
                    (gIFCommonPlayerInterface.player_pos_x[player] -
                    (sobj->sprite.width  * dIFCommonPlayerDamageEmblemScales[player] * 0.5F)) + dIFCommonPlayerDamageEmblemOffsetsX[player]
                );
                sobj->pos.y = (s32)
                (
                    (gIFCommonPlayerInterface.player_pos_y         -
                    (sobj->sprite.height * dIFCommonPlayerDamageEmblemScales[player] * 0.5F)) + dIFCommonPlayerDamageEmblemOffsetsY[player]
                );

                sobj->sprite.scalex = sobj->sprite.scaley = dIFCommonPlayerDamageEmblemScales[player];

                emblem = gSCManagerBattleState->players[player].color;

                sobj->sprite.red = gMPCollisionGroundData->emblem_colors[emblem].r;
                sobj->sprite.green = gMPCollisionGroundData->emblem_colors[emblem].g;
                sobj->sprite.blue = gMPCollisionGroundData->emblem_colors[emblem].b;

                sobj->sprite.attr = SP_TEXSHUF | SP_TRANSPARENT;
            }
            else
            {
                gcAddSObjForGObj(interface_gobj, NULL)->sprite.attr = SP_HIDDEN;
            }
            lbCommonMakeSObjForGObj(interface_gobj, lbRelocGetFileData(Sprite*, gGMCommonFiles[2], &llIFCommonPlayerDamageDigit0Sprite))->user_data.p = &sIFCommonPlayerDamageInterface[player].chars[0];
            lbCommonMakeSObjForGObj(interface_gobj, lbRelocGetFileData(Sprite*, gGMCommonFiles[2], &llIFCommonPlayerDamageDigit0Sprite))->user_data.p = &sIFCommonPlayerDamageInterface[player].chars[1];
            lbCommonMakeSObjForGObj(interface_gobj, lbRelocGetFileData(Sprite*, gGMCommonFiles[2], &llIFCommonPlayerDamageDigit0Sprite))->user_data.p = &sIFCommonPlayerDamageInterface[player].chars[2];
            lbCommonMakeSObjForGObj(interface_gobj, lbRelocGetFileData(Sprite*, gGMCommonFiles[2], &llIFCommonPlayerDamageDigit0Sprite))->user_data.p = &sIFCommonPlayerDamageInterface[player].chars[3];

            // The above functions should all return SObj*

            sIFCommonPlayerDamageInterface[player].damage = gSCManagerBattleState->players[player].stock_damage_all;
            sIFCommonPlayerDamageInterface[player].pos_adjust_wait = 0;
            sIFCommonPlayerDamageInterface[player].flash_reset_wait = 0;
            sIFCommonPlayerDamageInterface[player].scale = 1.04F;
            sIFCommonPlayerDamageInterface[player].is_update_anim = FALSE;
            sIFCommonPlayerDamageInterface[player].dead_stopupdate_wait = 180;
            sIFCommonPlayerDamageInterface[player].is_show_interface = FALSE;

            ifSetPlayer(interface_gobj, player); // Cast is probably redundant but I don't want any compilers screaming at me

            ifCommonPlayerDamageProcUpdate(interface_gobj);
            gcAddGObjProcess(interface_gobj, ifCommonPlayerDamageProcUpdate, nGCProcessKindFunc, 0);
        }
    }
}

// 0x8010F76C
void ifCommonPlayerDamageStartBreakAnim(FTStruct *fp)
{
    s32 player = fp->player;
    s32 i;

    for (i = 0; i < sIFCommonPlayerDamageInterface[player].char_display_count; i++)
    {
        sIFCommonPlayerDamageInterface[player].chars[i].vel.x = (syUtilsRandFloat() * 2) + (-1.0F);
        sIFCommonPlayerDamageInterface[player].chars[i].vel.y = -10.0F;

        sIFCommonPlayerDamageInterface[player].chars[i].is_lock_movement = FALSE;
    }
    sIFCommonPlayerDamageInterface[player].break_anim_frame = 0;
    sIFCommonPlayerDamageInterface[player].is_update_anim = TRUE;
}

// 0x8010F840
void ifCommonPlayerDamageStopBreakAnim(FTStruct *fp)
{
    s32 player = fp->player;

    sIFCommonPlayerDamageInterface[player].is_update_anim = FALSE;
    sIFCommonPlayerDamageInterface[player].scale = 1.04F;
}

// 0x8010F878
void ifCommonPlayerStockMultiProcDisplay(GObj *interface_gobj)
{
    s32 player;
    FTStruct *fp;
    s32 unused;
    s32 stock_count;
    s32 digit_count;
    SObj *gt_sobj;
    SObj *lt_sobj;
    s32 stock_order, digit_order;
    s32 trunc_pos_x;
    u8 digits[3];

    player = ifGetPlayer(interface_gobj);
    stock_count = gSCManagerBattleState->players[player].stock_count;
    fp = ftGetStruct(gSCManagerBattleState->players[player].fighter_gobj);

    if (stock_count >= 0)
    {
        stock_count++;

        if (stock_count != sIFCommonPlayerStocksNum[player])
        {
            if (TRUE)
            {
                /*
                 * ...well, at least it doesn't sound too unreasonable to have this here... I suppose?
                 * ...
                 * ...
                 * ...are you still reading this? 
                 * ...well then, on an unrelated note, please try to match some of those pesky nonmatchings!
                 */
            }
            if (stock_count <= 6)
            {
                stock_order = 0;

                lt_sobj = SObjGetStruct(interface_gobj);

                while (lt_sobj != NULL)
                {
                    if (stock_order < stock_count)
                    {
                        lt_sobj->sprite = *fp->attr->sprites->stock_sprite;

                        lt_sobj->sprite.LUT = fp->attr->sprites->stock_luts[fp->costume];

                        lt_sobj->pos.x = ((gIFCommonPlayerInterface.player_pos_x[player] + dIFCommonPlayerStocksIconOffsetsX[player] + (stock_order * 10)) - (lt_sobj->sprite.width * 0.5F));
                        lt_sobj->pos.y = ((gIFCommonPlayerInterface.player_pos_y - (s32)(lt_sobj->sprite.height * 0.5F)) - 20);

                        lt_sobj->sprite.attr &= ~SP_HIDDEN;
                    }
                    else lt_sobj->sprite.attr |= SP_HIDDEN;

                    lt_sobj = lt_sobj->next;

                    stock_order++;
                }
            }
            else
            {
                digit_count = ifCommonPlayerDamageGetSpecialArrayID(stock_count, digits);

                trunc_pos_x = gIFCommonPlayerInterface.player_pos_x[player] + dIFCommonPlayerStocksDigitOffsetsX[player];

                gt_sobj = SObjGetStruct(interface_gobj);

                gt_sobj->sprite = *fp->attr->sprites->stock_sprite;

                gt_sobj->sprite.LUT = fp->attr->sprites->stock_luts[fp->costume];

                gt_sobj->pos.x = ((trunc_pos_x - 22) - (gt_sobj->sprite.width * 0.5F));
                gt_sobj->pos.y = ((gIFCommonPlayerInterface.player_pos_y - (s32)(gt_sobj->sprite.height * 0.5F)) - 20);

                gt_sobj->sprite.attr &= ~SP_HIDDEN;

                gt_sobj = gt_sobj->next;

                gt_sobj->sprite = *lbRelocGetFileData(Sprite*, gGMCommonFiles[4], dIFCommonPlayerStockDigitSpriteOffsets[10]);

                gt_sobj->pos.x = ((trunc_pos_x + -10.5F) - (gt_sobj->sprite.width * 0.5F));
                gt_sobj->pos.y = ((gIFCommonPlayerInterface.player_pos_y - 20) - (gt_sobj->sprite.height * 0.5F));

                gt_sobj->sprite.attr &= ~SP_HIDDEN;

                gt_sobj = gt_sobj->next;

                digit_order = 0;

                while (gt_sobj != NULL)
                {
                    if (digit_order < digit_count)
                    {
                        gt_sobj->sprite = *lbRelocGetFileData(Sprite*, gGMCommonFiles[4], dIFCommonPlayerStockDigitSpriteOffsets[digits[digit_order]]);

                        gt_sobj->pos.x = ((trunc_pos_x + (digit_order * 8)) - (gt_sobj->sprite.width * 0.5F));
                        gt_sobj->pos.y = ((gIFCommonPlayerInterface.player_pos_y - 20) - (gt_sobj->sprite.height * 0.5F));

                        gt_sobj->sprite.attr &= ~SP_HIDDEN;
                    }
                    else gt_sobj->sprite.attr |= SP_HIDDEN;

                    gt_sobj = gt_sobj->next;

                    digit_order++;
                }
            }
            sIFCommonPlayerStocksNum[player] = stock_count;
        }
        lbCommonDrawSObjAttr(interface_gobj);
    }
}

// 0x8010FD2C
void ifCommonPlayerStockSetIconAttr(void)
{
    s32 i;

    for (i = 0; i < ARRAY_COUNT(dIFCommonPlayerStockDigitSpriteOffsets); i++)
    {
        lbRelocGetFileData(Sprite*, gGMCommonFiles[4], dIFCommonPlayerStockDigitSpriteOffsets[i])->attr = SP_TEXSHUF | SP_TRANSPARENT;
    }
}

// 0x8010FDD4
void ifCommonPlayerStockMultiMakeInterface(s32 player)
{
    FTStruct *fp = ftGetStruct(gSCManagerBattleState->players[player].fighter_gobj);
    Sprite *sprite;

    if ((fp->attr->sprites != NULL) && (fp->attr->sprites->stock_sprite != NULL))
    {
        GObj *interface_gobj = gcMakeGObjSPAfter(nGCCommonKindInterface, NULL, nGCCommonLinkIDInterface, GOBJ_PRIORITY_DEFAULT);
        gcAddGObjDisplay(interface_gobj, ifCommonPlayerStockMultiProcDisplay, 23, GOBJ_PRIORITY_DEFAULT, ~0);

        lbCommonMakeSObjForGObj(interface_gobj, lbRelocGetFileData(Sprite*, gGMCommonFiles[4], &llIFCommonDigits0Sprite));
        lbCommonMakeSObjForGObj(interface_gobj, lbRelocGetFileData(Sprite*, gGMCommonFiles[4], &llIFCommonDigits0Sprite));
        lbCommonMakeSObjForGObj(interface_gobj, lbRelocGetFileData(Sprite*, gGMCommonFiles[4], &llIFCommonDigits0Sprite));
        lbCommonMakeSObjForGObj(interface_gobj, lbRelocGetFileData(Sprite*, gGMCommonFiles[4], &llIFCommonDigits0Sprite));
        lbCommonMakeSObjForGObj(interface_gobj, lbRelocGetFileData(Sprite*, gGMCommonFiles[4], &llIFCommonDigits0Sprite));
        lbCommonMakeSObjForGObj(interface_gobj, lbRelocGetFileData(Sprite*, gGMCommonFiles[4], &llIFCommonDigits0Sprite));

        sIFCommonPlayerStocksNum[player] = S8_MAX;

        sprite = fp->attr->sprites->stock_sprite;
        sprite->attr = SP_TEXSHUF | SP_TRANSPARENT;

        ifSetPlayer(interface_gobj, player);
    }
}

// 0x8010FF24
void ifCommonPlayerStockSingleProcDisplay(GObj *interface_gobj)
{
    s32 player = ifGetPlayer(interface_gobj);
    s32 stocks = gSCManagerBattleState->players[player].stock_count;

    if (stocks != -1)
    {
        lbCommonDrawSObjAttr(interface_gobj);
    }
}

// 0x8010FF78
void ifCommonPlayerStockSetLUT(s32 player, s32 lut_id, FTAttributes *attr)
{
    SObjGetStruct(sIFCommonPlayerStocksGObj[player])->sprite.LUT = attr->sprites->stock_luts[lut_id];
}

// 0x8010FFA8
void ifCommonPlayerStockSingleMakeInterface(s32 player)
{
    FTStruct *fp = ftGetStruct(gSCManagerBattleState->players[player].fighter_gobj);
    GObj *interface_gobj;
    SObj *sobj;

    if ((fp->attr->sprites != NULL) && (fp->attr->sprites->stock_sprite != NULL))
    {
        sIFCommonPlayerStocksGObj[player] = interface_gobj = gcMakeGObjSPAfter(nGCCommonKindInterface, NULL, nGCCommonLinkIDInterface, GOBJ_PRIORITY_DEFAULT);

        gcAddGObjDisplay(interface_gobj, ifCommonPlayerStockSingleProcDisplay, 23, GOBJ_PRIORITY_DEFAULT, ~0);

        sobj = lbCommonMakeSObjForGObj(interface_gobj, fp->attr->sprites->stock_sprite);

        sobj->sprite.attr = SP_TEXSHUF | SP_TRANSPARENT;
        sobj->sprite.LUT = fp->attr->sprites->stock_luts[fp->costume];

        sobj->pos.x = ((gIFCommonPlayerInterface.player_pos_x[player] + dIFCommonPlayerStocksIconOffsetsX[player]) - (s32)(sobj->sprite.width * 0.5F));
        sobj->pos.y = ((gIFCommonPlayerInterface.player_pos_y - (s32)(sobj->sprite.height * 0.5F)) - 20);

        ifSetPlayer(interface_gobj, player);
    }
}

// 0x80110138
void ifCommonPlayerStockStealProcUpdate(GObj *interface_gobj)
{
    f32 dist_x;
    f32 vel_x;
    f32 vel_y;
    SObj *sobj;
    IFPlayerSteal *s_steal = &sIFCommonPlayerStealInterface[ifGetPlayer(interface_gobj)];

    s_steal->anim_frames--;

    if (s_steal->anim_frames == 0)
    {
        efManagerStockStealEndMakeEffect
        (
            gIFCommonPlayerInterface.player_pos_x[ifGetPlayer(interface_gobj)] + 
            dIFCommonPlayerStocksIconOffsetsX[ifGetPlayer(interface_gobj)], 
            gIFCommonPlayerInterface.player_pos_y - 20
        );
        gcEjectGObj(interface_gobj);

        return;
    }
    sobj = SObjGetStruct(interface_gobj);

    dist_x = (s_steal->steal_pos_x - s_steal->target_pos_x);

    vel_x = (s_steal->anim_frames * dist_x) / 30.0F;

    if (vel_x < (dist_x * 0.5F))
    {
        vel_y = -(vel_x - (dist_x * 0.5F));
    }
    else vel_y = (vel_x - (dist_x * 0.5F));

    sobj->pos.x = (s_steal->target_pos_x + vel_x);
    sobj->pos.y = ((s_steal->steal_pos_y + ((15.0F / SQUARE(dist_x * 0.5F)) * vel_y * vel_y)) - 15.0F);
}

// 0x801102B0
void ifCommonPlayerStockStealMakeInterface(s32 thief, s32 stolen)
{
    FTStruct *fp = ftGetStruct(gSCManagerBattleState->players[stolen].fighter_gobj);
    GObj *interface_gobj = gcMakeGObjSPAfter(nGCCommonKindInterface, NULL, nGCCommonLinkIDInterface, GOBJ_PRIORITY_DEFAULT);

    if (interface_gobj != NULL)
    {
        SObj *check_sobj, *sobj;

        gcAddGObjDisplay(interface_gobj, lbCommonDrawSObjAttr, 23, GOBJ_PRIORITY_DEFAULT, ~0);
        gcAddGObjProcess(interface_gobj, ifCommonPlayerStockStealProcUpdate, nGCProcessKindFunc, 0);

        check_sobj = lbCommonMakeSObjForGObj(interface_gobj, fp->attr->sprites->stock_sprite);

        if (check_sobj == NULL)
        {
            gcEjectGObj(interface_gobj);

            return;
        }
        else
        {
            sobj = check_sobj;

            sobj->sprite.attr = SP_TEXSHUF | SP_TRANSPARENT;
            sobj->sprite.LUT = fp->attr->sprites->stock_luts[fp->costume];

            sIFCommonPlayerStealInterface[thief].steal_pos_x = ((gIFCommonPlayerInterface.player_pos_x[stolen] + dIFCommonPlayerStocksIconOffsetsX[stolen]) - (s32)(sobj->sprite.width * 0.5F));
            sIFCommonPlayerStealInterface[thief].steal_pos_y = ((gIFCommonPlayerInterface.player_pos_y - (s32)(sobj->sprite.height * 0.5F)) - 20);

            sIFCommonPlayerStealInterface[thief].target_pos_x = ((gIFCommonPlayerInterface.player_pos_x[thief] + dIFCommonPlayerStocksIconOffsetsX[thief]) - (s32)(sobj->sprite.width * 0.5F));

            sobj->pos.x = sIFCommonPlayerStealInterface[thief].steal_pos_x;
            sobj->pos.y = sIFCommonPlayerStealInterface[thief].steal_pos_y;

            sIFCommonPlayerStealInterface[thief].anim_frames = 30;

            ifSetPlayer(interface_gobj, thief);

            efManagerStockStealStartMakeEffect(gIFCommonPlayerInterface.player_pos_x[stolen] + dIFCommonPlayerStocksIconOffsetsX[stolen], gIFCommonPlayerInterface.player_pos_y - 20);
        }
    }
}

// 0x80110514
void ifCommonPlayerStockInitInterface(void)
{
    s32 player;

    ifCommonPlayerStockSetIconAttr();

    for (player = 0; player < ARRAY_COUNT(gSCManagerBattleState->players); player++)
    {
        if (gSCManagerBattleState->players[player].pkind != nFTPlayerKindNot)
        {
            switch (gSCManagerBattleState->players[player].is_single_stockicon)
            {
            case FALSE:
                ifCommonPlayerStockMultiMakeInterface(player);
                break;

            case TRUE:
                ifCommonPlayerStockSingleMakeInterface(player);
                break;
            }
        }
    }
}

// 0x801105CC - Get magnifying glass position / arrow point direction?
void ifCommonPlayerMagnifyGetPosition(f32 player_pos_x, f32 player_pos_y, Vec2f *magnify_pos)
{
    f32 left;
    f32 right;
    f32 up;
    f32 down;
    f32 bak_right;
    f32 bak_up;
    f32 diag_hz;
    f32 diag_vt;
    f32 div_xy;

    left = (-gGMCameraStruct.viewport_width / 2) + (20 * gIFCommonPlayerInterface.magnify_scale) + 5;
    bak_right = right = (+gGMCameraStruct.viewport_width / 2) - (20 * gIFCommonPlayerInterface.magnify_scale) - 5;
    bak_up = up = (+gGMCameraStruct.viewport_height / 2) - (20 * gIFCommonPlayerInterface.magnify_scale);
    down = (-gGMCameraStruct.viewport_height / 2) + (20 * gIFCommonPlayerInterface.magnify_scale);

    if (player_pos_x == 0.0F)
    {
        up = bak_up;

        if (player_pos_y > 0.0F)
        {
            magnify_pos->y = up;
        }
        else magnify_pos->y = down;

        magnify_pos->x = 0.0F;
    }
    else
    {
        div_xy = player_pos_y / player_pos_x;

        if (((up / right) < div_xy) || (-(up / right) > div_xy))
        {
            up = bak_up;

            if (player_pos_y > 0.0F)
            {
                magnify_pos->y = up;
            }
            else magnify_pos->y = down;

            diag_hz = (magnify_pos->y * player_pos_x) / player_pos_y;

            right = bak_right;

            if (diag_hz < left)
            {
                magnify_pos->x = left;
            }
            else if (diag_hz > right)
            {
                magnify_pos->x = right;
            }
            else magnify_pos->x = diag_hz;
        }
        else
        {
            right = bak_right;

            if (player_pos_x > 0.0F)
            {
                magnify_pos->x = right;
            }
            else magnify_pos->x = left;

            diag_vt = (magnify_pos->x * player_pos_y) / player_pos_x;

            up = bak_up;

            if (diag_vt < down)
            {
                magnify_pos->y = down;
            }
            else if (diag_vt > up)
            {
                magnify_pos->y = up;
            }
            else magnify_pos->y = diag_vt;
        }
    }
}

// 0x801107F0
void ifCommonPlayerMagnifyUpdateRender(Gfx **dls, s32 color_id, f32 ulx, f32 uly)
{
    Gfx *dl = dls[0];
    SYColorRGB *color;
    f32 temp_f0;
    s32 var_uly;
    s32 var_lrx;
    s32 var_ulx;
    s32 var_lry;
    s32 temp_t0;
    s32 temp_t1;

    gDPPipeSync(dl++);
    gDPSetColorImage(dl++, G_IM_FMT_RGBA, G_IM_SIZ_16b, gSYVideoResWidth, gSYVideoZBuffer);
    gDPSetRenderMode(dl++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    gDPSetCombineMode(dl++, G_CC_DECALRGBA, G_CC_DECALRGBA);
    gDPSetAlphaCompare(dl++, G_AC_NONE);
    gDPSetTexturePersp(dl++, G_TP_NONE);
    gSPClearGeometryMode(dl++, G_ZBUFFER);

    // This is a compound macro but I cannot find anything that would correspond to this
    gDPSetTextureImage(dl++, G_IM_FMT_IA, G_IM_SIZ_16b, 1, lbRelocGetFileData(Sprite*, gGMCommonFiles[0], &llIFCommonPlayerMagnifyFrameImage));

    // NEEDS TO BE ALL ON THE SAME LINE OR GLUED, OTHERWISE IT DOESN'T MATCH
    gDPSetTile(dl++, G_IM_FMT_IA, G_IM_SIZ_16b, 0, 0x0000, G_TX_LOADTILE, 0, G_TX_MIRROR | G_TX_WRAP, 4, G_TX_NOLOD, G_TX_MIRROR | G_TX_WRAP, 4, G_TX_NOLOD);\
    gDPLoadSync(dl++);\
    gDPLoadBlock(dl++, G_TX_LOADTILE, 0, 0, 127, 1024);\
    gDPPipeSync(dl++);

    // SAME HERE
    gDPSetTile(dl++, G_IM_FMT_IA, G_IM_SIZ_8b, 2, 0x0000, G_TX_RENDERTILE, 0, G_TX_MIRROR | G_TX_WRAP, 4, G_TX_NOLOD, G_TX_MIRROR | G_TX_WRAP, 4, G_TX_NOLOD);\
    gDPSetTileSize(dl++, G_TX_RENDERTILE, 0, 0, 0x003C, 0x003C);

    temp_f0 = (s32)((1024.0F / gIFCommonPlayerInterface.magnify_scale) + 0.5F);

    var_ulx = ulx;
    var_uly = uly;

    var_lrx = ((ulx == var_ulx) ? 0 : 1) + (s32)(ulx + (32.0F * gIFCommonPlayerInterface.magnify_scale));

    var_lry = ((uly == var_uly) ? 0 : 1) + (s32)(uly + (32.0F * gIFCommonPlayerInterface.magnify_scale));

    if (var_ulx < gGMCameraStruct.viewport_ulx)
    {
        var_ulx = gGMCameraStruct.viewport_ulx;
    }
    else if (var_lrx >= gGMCameraStruct.viewport_lrx)
    {
        var_lrx = gGMCameraStruct.viewport_lrx - 1;
    }
    if (var_uly < gGMCameraStruct.viewport_uly)
    {
        var_uly = gGMCameraStruct.viewport_uly;
    }
    else if (var_lry >= gGMCameraStruct.viewport_lry)
    {
        var_lry = gGMCameraStruct.viewport_lry - 1;
    }
    temp_t0 = ((s32)((var_ulx - ulx) * temp_f0) + 16) >> 5;
    temp_t1 = ((s32)((var_uly - uly) * temp_f0) + 16) >> 5;

    gSPTextureRectangle
    (
        dl++,
        var_ulx * 4, var_uly * 4, var_lrx * 4, var_lry * 4,
        G_TX_RENDERTILE,
        temp_t0, temp_t1,
        temp_f0, temp_f0
    );

    gDPPipeSync(dl++);
    gDPSetColorImage(dl++, G_IM_FMT_RGBA, gSYVideoColorDepth, gSYVideoResWidth, 0x0F000000);
    gDPSetRenderMode(dl++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
    gDPSetAlphaCompare(dl++, G_AC_THRESHOLD);
    gDPSetBlendColor(dl++, 0, 0, 0, 0x8);

    color = &gMPCollisionGroundData->fog_color;

    gDPSetPrimColor(dl++, 0, 0, color->r, color->g, color->b, 0xFF);
    gDPSetEnvColor(dl++, dIFCommonPlayerMagnifyColorsR[color_id], dIFCommonPlayerMagnifyColorsG[color_id], dIFCommonPlayerMagnifyColorsB[color_id], 0xFF);
    gDPSetCombineMode(dl++, G_CC_BLENDPEDECALA, G_CC_BLENDPEDECALA);
    gSPTextureRectangle
    (
        dl++,
        var_ulx * 4, var_uly * 4, var_lrx * 4, var_lry * 4,
        G_TX_RENDERTILE,
        temp_t0, temp_t1,
        temp_f0, temp_f0
    );
    gDPPipeSync(dl++);
    gDPSetAlphaCompare(dl++, G_AC_NONE);
    gDPSetTexturePersp(dl++, G_TP_PERSP);
    gSPSetGeometryMode(dl++, G_ZBUFFER);

    dls[0] = dl;
}

// 0x80110DD4
void ifCommonPlayerMagnifyUpdateViewport(Gfx **dls, FTStruct *fp)
{
    Gfx *dl;
    f32 magnify_x;
    f32 magnify_y;
    IFPlayerMagnify *ifmag;
    CObj *cobj;
    f32 scale;
    s32 ulx;
    s32 uly;
    s32 lrx;
    s32 lry;

    if (gIFCommonPlayerInterface.is_magnify_display != FALSE)
    {
        ifmag = &sIFCommonPlayerMagnifyInterface[fp->player];

        magnify_x = fp->magnify_pos.x;
        magnify_y = fp->magnify_pos.y;

        ifCommonPlayerMagnifyGetPosition(magnify_x, magnify_y, &ifmag->pos);

        magnify_x = ifmag->pos.x + gGMCameraStruct.viewport_center_x;
        magnify_y = gGMCameraStruct.viewport_center_y - ifmag->pos.y;

        gSPMatrix(dls[0]++, &CObjGetStruct(gGCCurrentCamera)->xobjs[0]->mtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);

        if (gIFCommonPlayerInterface.magnify_mode != 1)
        {
            cobj = CObjGetStruct(gGMCameraGObj);

            gSPViewport(dls[0]++, &cobj->viewport);

            gDPSetScissor(dls[0]++, G_SC_NON_INTERLACE, gGMCameraStruct.viewport_ulx, gGMCameraStruct.viewport_uly, gGMCameraStruct.viewport_lrx, gGMCameraStruct.viewport_lry);
        }
        else gIFCommonPlayerInterface.magnify_mode = 2;

        scale = (16.0F * gIFCommonPlayerInterface.magnify_scale);

        ifCommonPlayerMagnifyUpdateRender(dls, ifmag->color_id, magnify_x - scale, magnify_y - scale);

        dl = dls[0];

        scale = (18.0F * gIFCommonPlayerInterface.magnify_scale);

        magnify_x -= (scale / 2);
        magnify_y -= (scale / 2);

        syRdpSetViewport(&ifmag->viewport, magnify_x, magnify_y, scale + magnify_x, scale + magnify_y);

        gSPViewport(dl++, &ifmag->viewport);

        ulx = (ifmag->viewport.vp.vtrans[0] / 4) - (ifmag->viewport.vp.vscale[0] / 4);
        uly = (ifmag->viewport.vp.vtrans[1] / 4) - (ifmag->viewport.vp.vscale[1] / 4);
        lrx = (ifmag->viewport.vp.vtrans[0] / 4) + (ifmag->viewport.vp.vscale[0] / 4);
        lry = (ifmag->viewport.vp.vtrans[1] / 4) + (ifmag->viewport.vp.vscale[1] / 4);

        if (ulx < gGMCameraStruct.viewport_ulx)
        {
            ulx = gGMCameraStruct.viewport_ulx;
        }
        if (lrx > gGMCameraStruct.viewport_lrx)
        {
            lrx = gGMCameraStruct.viewport_lrx;
        }
        if (uly < gGMCameraStruct.viewport_uly)
        {
            uly = gGMCameraStruct.viewport_uly;
        }
        else if (lry > gGMCameraStruct.viewport_lry)
        {
            lry = gGMCameraStruct.viewport_lry;
        }
        gDPSetScissor(dl++, G_SC_NON_INTERLACE, ulx, uly, lrx, lry);

        dls[0] = dl;
    }
}

// 0x801111A0
void ifCommonPlayerMagnifyProcDisplay(FTStruct *fp)
{
    GObj *interface_gobj;
    DObj *dobj;
    IFPlayerMagnify *ifmag;
    CObj *cobj;

    if (gIFCommonPlayerInterface.is_magnify_display != FALSE)
    {
        ifmag = &sIFCommonPlayerMagnifyInterface[fp->player];

        interface_gobj = ifmag->interface_gobj;

        dobj = DObjGetStruct(interface_gobj);

        dobj->translate.vec.f.x = ifmag->pos.x;
        dobj->translate.vec.f.y = ifmag->pos.y;

        dobj->rotate.vec.f.z = syUtilsArcTan2(fp->magnify_pos.y, fp->magnify_pos.x) - F_CST_DTOR32(90.0F);

        dobj->scale.vec.f.x = dobj->scale.vec.f.y = gIFCommonPlayerInterface.magnify_scale * 0.5F;

        cobj = CObjGetStruct(gGMCameraGObj);

        gSPViewport(gSYTaskmanDLHeads[0]++, &cobj->viewport);
        gDPSetScissor(gSYTaskmanDLHeads[0]++, G_SC_NON_INTERLACE, gGMCameraStruct.viewport_ulx, gGMCameraStruct.viewport_uly, gGMCameraStruct.viewport_lrx, gGMCameraStruct.viewport_lry);
        gSPMatrix(gSYTaskmanDLHeads[0]++, &CObjGetStruct(gGCCurrentCamera)->xobjs[1]->mtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
        gSPClearGeometryMode(gSYTaskmanDLHeads[0]++, G_ZBUFFER);
        gDPPipeSync(gSYTaskmanDLHeads[0]++);
        gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_OPA_SURF, G_RM_AA_OPA_SURF2);
        gDPSetAlphaCompare(gSYTaskmanDLHeads[0]++, G_AC_NONE);
        gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, dIFCommonPlayerMagnifyColorsR[ifmag->color_id], dIFCommonPlayerMagnifyColorsG[ifmag->color_id], dIFCommonPlayerMagnifyColorsB[ifmag->color_id], 0xFF);
        gcDrawDObjDLHead0(interface_gobj);
    }
}

// 0x80111440
void ifCommonPlayerMagnifyMakeInterface(void)
{
    GObj *fighter_gobj = gGCCommonLinks[nGCCommonLinkIDFighter];

    while (fighter_gobj != NULL)
    {
        FTStruct *fp = ftGetStruct(fighter_gobj);
        GObj *interface_gobj = gcMakeGObjSPAfter(nGCCommonKindInterface, NULL, nGCCommonLinkIDMagnify, GOBJ_PRIORITY_DEFAULT);

        gcAddXObjForDObjFixed(gcAddDObjForGObj(interface_gobj, lbRelocGetFileData(void*, gGMCommonFiles[0], &llIFCommonPlayerMagnifyDisplayList)), nGCMatrixKindTraRotRpyRSca, 0);

        sIFCommonPlayerMagnifyInterface[fp->player].interface_gobj = interface_gobj;
        sIFCommonPlayerMagnifyInterface[fp->player].color_id = gSCManagerBattleState->players[fp->player].color;

        fighter_gobj = fighter_gobj->link_next;
    }
    gIFCommonPlayerInterface.is_magnify_display = FALSE;
}

// 0x80111554
void ifCommonPlayerArrowsLeftProcDisplay(GObj *interface_gobj)
{
    if (gIFCommonPlayerInterface.arrows_flags & 1)
    {
        gcDrawDObjTreeForGObj(interface_gobj);
    }
}

// 0x80111588
void ifCommonPlayerArrowsRightProcDisplay(GObj *interface_gobj)
{
    if (gIFCommonPlayerInterface.arrows_flags & 2)
    {
        gcDrawDObjTreeForGObj(interface_gobj);
    }
}

// 0x801115BC
void ifCommonPlayerArrowsAddAnim(GObj *interface_gobj)
{
    gcAddAnimJointAll(interface_gobj, lbRelocGetFileData(AObjEvent32**, gGMCommonFiles[0], &llIFCommonPlayerArrowsAnimJoint), 0.0F);
    gcPlayAnimAll(interface_gobj);
}

// 0x801115FC
void ifCommonPlayerArrowsLeftProcUpdate(GObj *interface_gobj)
{
    switch (gIFCommonPlayerInterface.arrows_left_status)
    {
    case 0:
        break;

    case 1:
        ifCommonPlayerArrowsAddAnim(interface_gobj);
        // Fallthrough
    default:
        gcPlayAnimAll(interface_gobj);
        break;
    }
}

// 0x80111640
void ifCommonPlayerArrowsRightProcUpdate(GObj *interface_gobj)
{
    switch (gIFCommonPlayerInterface.arrows_right_status)
    {
    case 0:
        break;

    case 1:
        ifCommonPlayerArrowsAddAnim(interface_gobj);
        // Fallthrough
    default:
        gcPlayAnimAll(interface_gobj);
        break;
    }
}

// 0x80111684
GObj* ifCommonPlayerArrowsMakeInterface(void (*proc_display)(GObj*), void (*proc_update)(GObj*))
{
    GObj *interface_gobj = gcMakeGObjSPAfter(nGCCommonKindInterface, NULL, nGCCommonLinkIDInterface, GOBJ_PRIORITY_DEFAULT);

    gcAddGObjDisplay(interface_gobj, proc_display, 8, GOBJ_PRIORITY_DEFAULT, ~0);
    gcSetupCustomDObjs
    (
        interface_gobj,
        lbRelocGetFileData(DObjDesc*, gGMCommonFiles[0], &llIFCommonPlayerArrowsDObjDesc),
        NULL,
        nGCMatrixKindTraRotRpyR,
        nGCMatrixKindNull,
        nGCMatrixKindNull
    );
    gcAddGObjProcess(interface_gobj, proc_update, nGCProcessKindFunc, 5);

    return interface_gobj;
}

// 0x8011171C
void ifCommonPlayerArrowsFuncRun(GObj *interface_gobj)
{
    sb32 lr_right = FALSE;
    sb32 lr_left = FALSE;

    if (gIFCommonPlayerInterface.is_magnify_display != FALSE)
    {
        GObj *fighter_gobj = gGCCommonLinks[nGCCommonLinkIDFighter];

        while (fighter_gobj != NULL)
        {
            FTStruct *fp = ftGetStruct(fighter_gobj);

            if (!(fp->is_magnify_ignore) && !(fp->is_rebirth) && (fp->is_magnify_show))
            {
                if (ABSF(fp->magnify_pos.x) > ABSF(fp->magnify_pos.y))
                {
                    if (fp->magnify_pos.x < 0.0F)
                    {
                        lr_left = TRUE;
                    }
                    else lr_right = TRUE;
                }
            }
            fighter_gobj = fighter_gobj->link_next;
        }
        if (lr_left == FALSE)
        {
            gIFCommonPlayerInterface.arrows_left_status = 0;
        }
        else if (gIFCommonPlayerInterface.arrows_left_status == 0)
        {
            gIFCommonPlayerInterface.arrows_left_status = 1;
        }
        else gIFCommonPlayerInterface.arrows_left_status = 2;

        if (lr_right == FALSE)
        {
            gIFCommonPlayerInterface.arrows_right_status = 0;
        }
        else if (gIFCommonPlayerInterface.arrows_right_status == 0)
        {
            gIFCommonPlayerInterface.arrows_right_status = 1;
        }
        else gIFCommonPlayerInterface.arrows_right_status = 2;
    }
    if ((lr_left != FALSE) || (lr_right != FALSE))
    {
        if (sIFCommonPlayerMagnifySoundWait == 0)
        {
            func_800269C0_275C0(nSYAudioFGMMagnify);

            sIFCommonPlayerMagnifySoundWait = 30;
        }
        sIFCommonPlayerMagnifySoundWait--;
    }
    else sIFCommonPlayerMagnifySoundWait = 0;
}

// 0x801118B4
void ifCommonPlayerArrowsMainProcDisplay(GObj *interface_gobj)
{
    gSPDisplayList(gSYTaskmanDLHeads[0]++, &dIFCommonPlayerArrowsDisplayList);
}

// 0x801118E4
void ifCommonPlayerArrowsInitInterface(void)
{
    DObj *dobj;

    gcAddGObjDisplay
    (
        gcMakeGObjSPAfter(nGCCommonKindInterface, ifCommonPlayerArrowsFuncRun, nGCCommonLinkIDInterface, GOBJ_PRIORITY_DEFAULT),
        ifCommonPlayerArrowsMainProcDisplay,
        8,
        GOBJ_PRIORITY_DEFAULT,
        ~0
    );
    dobj = DObjGetStruct(ifCommonPlayerArrowsMakeInterface(ifCommonPlayerArrowsLeftProcDisplay, ifCommonPlayerArrowsLeftProcUpdate));

    dobj->translate.vec.f.x = -134.0F;
    dobj->translate.vec.f.y = 0.0F;

    gIFCommonPlayerInterface.arrows_left_status = 0;

    dobj = DObjGetStruct(ifCommonPlayerArrowsMakeInterface(ifCommonPlayerArrowsRightProcDisplay, ifCommonPlayerArrowsRightProcUpdate));

    dobj->translate.vec.f.x = 134.0F;
    dobj->translate.vec.f.y = 0.0F;
    dobj->rotate.vec.f.z = F_CST_DTOR32(180.0F);

    gIFCommonPlayerInterface.arrows_right_status = 0;

    sIFCommonPlayerMagnifySoundWait = 0;
}

// 0x801119AC
void ifCommonPlayerArrowsUpdateFlags(f32 x, f32 y)
{
    if (ABSF(x) > ABSF(y))
    {
        if (x > 0.0F)
        {
            gIFCommonPlayerInterface.arrows_flags |= IFCOMMON_PLAYERARROWS_MASK_RIGHT;
        }
        else gIFCommonPlayerInterface.arrows_flags |= IFCOMMON_PLAYERARROWS_MASK_LEFT;
    }
}

// 0x80111A3C
void ifCommonPlayerTagProcDisplay(GObj *interface_gobj)
{
    s32 player = ifGetPlayer(interface_gobj);
    FTStruct *fp;
    f32 x;
    f32 y;
    Vec3f pos;

    fp = ftGetStruct(gSCManagerBattleState->players[player].fighter_gobj);

    if (!(fp->is_playertag_bossend) && !(fp->is_playertag_hide))
    {
        if ((fp->playertag_wait == 1) || (CObjGetStruct(gGMCameraGObj)->vec.eye.z > 6000.0F))
        {
            pos = fp->joints[nFTPartsJointTopN]->translate.vec.f;

            pos.y += fp->attr->camera_zoom_base;

            func_ovl2_800EB924(CObjGetStruct(gGMCameraGObj), gGMCameraMatrix, &pos, &x, &y);

            if (gmCameraCheckTargetInBounds(x, y) != FALSE)
            {
                SObjGetStruct(interface_gobj)->pos.x = (s32) ((gGMCameraStruct.viewport_center_x + x) - (SObjGetStruct(interface_gobj)->sprite.width * 0.5F));
                SObjGetStruct(interface_gobj)->pos.y = (s32) ((gGMCameraStruct.viewport_center_y - y) - SObjGetStruct(interface_gobj)->sprite.height);

                lbCommonDrawSObjAttr(interface_gobj);
            }
        }
    }
}

// 0x80111BE4
void ifCommonPlayerTagMakeInterface(void)
{
    GObj *interface_gobj;
    SObj *sobj;
    s32 player;
    u8 color_id;

    for (player = 0; player < ARRAY_COUNT(gSCManagerBattleState->players); player++)
    {
        if (gSCManagerBattleState->players[player].pkind != nFTPlayerKindNot)
        {
            interface_gobj = gcMakeGObjSPAfter(nGCCommonKindInterface, NULL, nGCCommonLinkIDInterface, GOBJ_PRIORITY_DEFAULT);

            gcAddGObjDisplay(interface_gobj, ifCommonPlayerTagProcDisplay, 23, GOBJ_PRIORITY_DEFAULT, ~0);

            sobj = lbCommonMakeSObjForGObj(interface_gobj, lbRelocGetFileData(Sprite*, gGMCommonFiles[6], dIFCommonPlayerTagSpriteOffsets[gSCManagerBattleState->players[player].tag]));

            sobj->sprite.attr = SP_TEXSHUF | SP_TRANSPARENT;

            color_id = gSCManagerBattleState->players[player].color;

            sobj->sprite.red   = dIFCommonPlayerTagPrimColorsR[color_id];
            sobj->sprite.green = dIFCommonPlayerTagPrimColorsG[color_id];
            sobj->sprite.blue  = dIFCommonPlayerTagPrimColorsB[color_id];

            sobj->envcolor.r = dIFCommonPlayerTagEnvColorsR[color_id];
            sobj->envcolor.g = dIFCommonPlayerTagEnvColorsG[color_id];
            sobj->envcolor.b = dIFCommonPlayerTagEnvColorsB[color_id];

            ifSetPlayer(interface_gobj, player);
        }
    }
}

// 0x80111D64
void ifCommonItemArrowProcDisplay(GObj *interface_gobj)
{
    ITStruct *ip = itGetStruct(interface_gobj); // So I'm guessing this copies the corresponding item's user_data? Its classifier is 0x3F8.
    SObj *sobj;
    f32 x;
    f32 y;
    Vec3f pos;

    if ((ip->is_allow_pickup) && (ip->arrow_timer >= 15))
    {
        sobj = SObjGetStruct(interface_gobj);

        pos = DObjGetStruct(ip->item_gobj)->translate.vec.f;

        pos.y += ip->coll_data.map_coll.top + 100.0F;

        func_ovl2_800EB924(CObjGetStruct(gGMCameraGObj), gGMCameraMatrix, &pos, &x, &y);

        if (gmCameraCheckTargetInBounds(x, y) != FALSE)
        {
            sobj->pos.x = (s32) ((gGMCameraStruct.viewport_center_x + x) - (sobj->sprite.width * 0.5F));
            sobj->pos.y = (s32) ((gGMCameraStruct.viewport_center_y - y) - sobj->sprite.height);

            lbCommonDrawSObjAttr(interface_gobj);
        }
    }
}

// 0x80111EC0
GObj* ifCommonItemArrowMakeInterface(ITStruct *ip)
{
    GObj *interface_gobj = gcMakeGObjSPAfter(nGCCommonKindInterface, NULL, nGCCommonLinkIDInterface, GOBJ_PRIORITY_DEFAULT);

    if (interface_gobj != NULL)
    {
        gcAddGObjDisplay(interface_gobj, ifCommonItemArrowProcDisplay, 23, GOBJ_PRIORITY_DEFAULT, ~0);

        if (lbCommonMakeSObjForGObj(interface_gobj, sIFCommonItemArrowSprite) != NULL)
        {
            interface_gobj->user_data.p = ip; // Give it up for... the GObj with the most flexible user_data assignments ever?

            if ((gSCManagerSceneData.scene_curr == nSCKind1PTrainingMode) && (gSCManagerBattleState->game_status == nSCBattleGameStatusPause))
            {
                interface_gobj->flags = GOBJ_FLAG_HIDDEN;
            }
            return interface_gobj;
        }
        else gcEjectGObj(interface_gobj);
    }
    return NULL;
}

// 0x80111F80
void ifCommonItemArrowSetAttr(void)
{
    Sprite *sprite = sIFCommonItemArrowSprite =

    lbRelocGetFileData(Sprite*, lbRelocGetExternHeapFile((intptr_t)&llIFCommonItemFileID, syTaskmanMalloc(lbRelocGetFileSize((intptr_t)&llIFCommonItemFileID), 0x10)), &llIFCommonItemArrowSprite);

    sprite->attr = SP_TEXSHUF | SP_TRANSPARENT;

    sprite->red   = 0xFF;
    sprite->green = 0x00;
    sprite->blue  = 0x00;
}

// 0x80111FF0
void ifCommonAnnounceThread(GObj *interface_gobj)
{
    gcSleepCurrentGObjThread(60);

    gcEjectGObj(NULL);

    gcSleepCurrentGObjThread(1);
}

// 0x80112024
void ifCommonAnnounceSetAttr(GObj *interface_gobj, s32 file_id, IFACharacter *character, s32 sprite_count)
{
    SObj *sobj;
    void *sprite_head = gGMCommonFiles[file_id];
    s32 i;

    for (i = 0; i < sprite_count; i++)
    {
        sobj = lbCommonMakeSObjForGObj(interface_gobj, lbRelocGetFileData(Sprite*, sprite_head, character[i].offset));

        sobj->sprite.attr = SP_TEXSHUF | SP_TRANSPARENT;

        sobj->pos.x = character[i].pos.x;
        sobj->pos.y = character[i].pos.y;
    }
}

// 0x801120D4
void ifCommonAnnounceGoMakeInterface(void)
{
    void *sprite_head = gGMCommonFiles[1];
    GObj *interface_gobj = gcMakeGObjSPAfter(nGCCommonKindInterface, NULL, nGCCommonLinkIDInterface, GOBJ_PRIORITY_DEFAULT);
    s32 i;

    gcAddGObjDisplay(interface_gobj, lbCommonDrawSObjAttr, 23, GOBJ_PRIORITY_DEFAULT, ~0);
    gcAddGObjProcess(interface_gobj, ifCommonAnnounceThread, nGCProcessKindThread, 5);

    for (i = 0; i < ARRAY_COUNT(dIFCommonAnnounceGoSpriteData); i++)
    {
        SObj *sobj = lbCommonMakeSObjForGObj(interface_gobj, lbRelocGetFileData(Sprite*, sprite_head, dIFCommonAnnounceGoSpriteData[i].offset));

        sobj->sprite.attr = SP_CLOUD | SP_TEXSHUF; // 0x1000 doesn't exist in base sp.h though?

        sobj->pos.x = dIFCommonAnnounceGoSpriteData[i].pos.x;
        sobj->pos.y = dIFCommonAnnounceGoSpriteData[i].pos.y;
    }
}

// 0x801121C4
void ifCommonAnnounceGoSetStatus(void)
{
    GObj *fighter_gobj = gGCCommonLinks[nGCCommonLinkIDFighter];

    while (fighter_gobj != NULL)
    {
        FTStruct *fp = ftGetStruct(fighter_gobj);

        ftParamUnlockPlayerControl(fighter_gobj);

        fp->camera_mode = nFTCameraModeDefault;

        fighter_gobj = fighter_gobj->link_next;
    }
    gSCManagerBattleState->game_status = nSCBattleGameStatusGo;

    gIFCommonPlayerInterface.is_magnify_display = TRUE;
}

// 0x80112234
SObj* ifCommonTrafficMakeSObj(GObj *interface_gobj, s32 id)
{
    SObj *sobj;
    s32 color_id;

    color_id = dIFCommonTrafficSpriteData[id].color_id;

    sobj = lbCommonMakeSObjForGObj(interface_gobj, lbRelocGetFileData(Sprite*, gGMCommonFiles[1], dIFCommonTrafficSpriteOffsets[color_id]));

    sobj->sprite.attr = SP_CLOUD | SP_TEXSHUF;

    sobj->pos.x = dIFCommonTrafficSpriteData[id].pos.x;
    sobj->pos.y = dIFCommonTrafficSpriteData[id].pos.y;

    sobj->sprite.red   = dIFCommonTrafficSpriteColorsR[color_id];
    sobj->sprite.green = dIFCommonTrafficSpriteColorsG[color_id];
    sobj->sprite.blue  = dIFCommonTrafficSpriteColorsB[color_id];

    return sobj;
}

// 0x801122F4
void ifCommonCountdownThread(GObj *interface_gobj)
{
    SObj *sobj;
    SObj *other_sobj;
    SObj *new_sobj;
    s32 timer;
    f32 rscale;
    f32 scale;
    s32 main_status;
    s32 lamp_status;
    SObj *child_sobj;

    for (timer = 0; timer < 60; timer++)
    {
        sobj = SObjGetStruct(interface_gobj);

        while (sobj != NULL)
        {
            sobj->pos.y += 0.8833333F;

            sobj = sobj->next;
        }
        gcSleepCurrentGObjThread(1);
    }
    sobj = ifGetSObj(interface_gobj);

    main_status = lamp_status = -1;

    child_sobj = sobj->next->next;

    while (TRUE)
    {
        switch (timer)
        {
        case I_SEC_TO_TICS(2):
            main_status = lamp_status = 6;

            func_800269C0_275C0(nSYAudioVoiceAnnounceThree);
            break;

        case I_SEC_TO_TICS(3):
            main_status = lamp_status = 7;

            func_800269C0_275C0(nSYAudioVoiceAnnounceTwo);
            break;

        case I_SEC_TO_TICS(4):
            main_status = lamp_status = 8;

            func_800269C0_275C0(nSYAudioVoiceAnnounceOne);
            break;

        case I_SEC_TO_TICS(5):
            ifCommonAnnounceGoMakeInterface();
            ifCommonAnnounceGoSetStatus();
            ifCommonPlayerDamageSetShowInterface();

            main_status = lamp_status = 9;

            func_800269C0_275C0(nSYAudioVoiceAnnounceGo);

            break;

        case I_SEC_TO_TICS(6):
            goto finish;
        }
        if (lamp_status != -1)
        {
            if (main_status != -1)
            {
                gcEjectSObj(sobj->next);
                gcEjectSObj(sobj);

                sobj = ifCommonTrafficMakeSObj(interface_gobj, lamp_status);

                sobj->sprite.scalex = sobj->sprite.scaley = scale = 3.0F;

                new_sobj = ifCommonTrafficMakeSObj(interface_gobj, lamp_status + 5);

                if (lamp_status == 9)
                {
                    other_sobj = child_sobj->prev;

                    child_sobj->sprite.red = dIFCommonTrafficGoBacklightR[1];
                    child_sobj->sprite.green = dIFCommonTrafficGoBacklightG[1];
                    child_sobj->sprite.blue = dIFCommonTrafficGoBacklightB[1];

                    child_sobj->envcolor.r = dIFCommonTrafficGoShadowR[1];
                    child_sobj->envcolor.g = dIFCommonTrafficGoShadowG[1];
                    child_sobj->envcolor.b = dIFCommonTrafficGoShadowB[1];

                    if (other_sobj != NULL)
                    {
                        other_sobj->next = child_sobj->next;
                    }
                    other_sobj = child_sobj->next;

                    if (other_sobj != NULL)
                    {
                        other_sobj->prev = child_sobj->prev;
                    }
                    new_sobj->next = child_sobj;

                    child_sobj->prev = new_sobj;
                    child_sobj->next = NULL;
                }
            }
            if (scale != 1.0F)
            {
                scale -= 0.2F;

                if (scale < 1.0F)
                {
                    scale = 1;
                }
                sobj->sprite.scalex = sobj->sprite.scaley = scale;

                rscale = (scale - 1.0F) * 0.5F;

                sobj->pos.x = dIFCommonTrafficSpriteData[lamp_status].pos.x - (rscale * sobj->sprite.width);
                sobj->pos.y = dIFCommonTrafficSpriteData[lamp_status].pos.y - (rscale * sobj->sprite.height);
            }
        }
        timer++;

        gcSleepCurrentGObjThread(1);

        main_status = -1;
    }
finish:
    for (timer = timer; timer < I_SEC_TO_TICS(7); timer++)
    {
        sobj = SObjGetStruct(interface_gobj);

        while (sobj != NULL)
        {
            sobj->pos.y += (-0.8833333F);

            sobj = sobj->next;
        }
        gcSleepCurrentGObjThread(1);
    }
    gcEjectGObj(NULL);
    gcSleepCurrentGObjThread(1);
}

// 0x80112668
SObj* ifCommonCountdownMakeInterface(void)
{
    GObj *interface_gobj;
    SObj *sobj;
    s32 i;

    interface_gobj = gcMakeGObjSPAfter(nGCCommonKindInterface, NULL, nGCCommonLinkIDInterface, GOBJ_PRIORITY_DEFAULT);

    gcAddGObjDisplay(interface_gobj, lbCommonDrawSObjAttr, 23, GOBJ_PRIORITY_DEFAULT, ~0);
    gcAddGObjProcess(interface_gobj, ifCommonCountdownThread, nGCProcessKindThread, 5);

    sobj = lbCommonMakeSObjForGObj(interface_gobj, lbRelocGetFileData(Sprite*, gGMCommonFiles[1], &llIFCommonGameStatusRodSprite));

    sobj->sprite.attr = SP_TEXSHUF | SP_TRANSPARENT;

    sobj->pos.x = 103.0F;
    sobj->pos.y = -57.0F;

    sobj = lbCommonMakeSObjForGObj(interface_gobj, lbRelocGetFileData(Sprite*, gGMCommonFiles[1], &llIFCommonGameStatusFrameSprite));

    sobj->sprite.attr = SP_TEXSHUF | SP_TRANSPARENT;

    sobj->pos.x = 111.0F;
    sobj->pos.y = -23.0F;

    for (i = 0; i < 6; i++)
    {
        sobj = ifCommonTrafficMakeSObj(interface_gobj, i);
    }
    ifSetSObj(interface_gobj, sobj);

    ifCommonTrafficMakeSObj(interface_gobj, 10);

    sobj = lbCommonMakeSObjForGObj(interface_gobj, lbRelocGetFileData(Sprite*, gGMCommonFiles[1], &llIFCommonGameStatusRodShadowSprite));

    sobj->sprite.attr = SP_TEXSHUF | SP_TRANSPARENT;

    sobj->pos.x = 182.0F;
    sobj->pos.y = -11.0F;

    sobj->sprite.red   = dIFCommonTrafficGoBacklightR[0];
    sobj->sprite.green = dIFCommonTrafficGoBacklightG[0];
    sobj->sprite.blue  = dIFCommonTrafficGoBacklightB[0];
    sobj->envcolor.r = dIFCommonTrafficGoShadowR[0];
    sobj->envcolor.g = dIFCommonTrafficGoShadowG[0];
    sobj->envcolor.b = dIFCommonTrafficGoShadowB[0];

    return sobj;
}

// 0x80112814
GObj* ifCommonAnnounceTimeUpMakeInterface(void)
{
    GObj *interface_gobj = gcMakeGObjSPAfter(nGCCommonKindInterface, NULL, nGCCommonLinkIDInterface, GOBJ_PRIORITY_DEFAULT);

    gcAddGObjDisplay(interface_gobj, lbCommonDrawSObjAttr, 23, GOBJ_PRIORITY_DEFAULT, ~0);

    ifCommonAnnounceSetAttr(interface_gobj, 1, dIFCommonAnnounceTimeUpSpriteData, ARRAY_COUNT(dIFCommonAnnounceTimeUpSpriteData));

    return interface_gobj;
}

// 0x80112880
void ifCommonEntryFocusThread(GObj *interface_gobj)
{
    GObj *fighter_gobj;
    s32 id = interface_gobj->user_data.s;
    s32 sleep_tics = dIFCommonEntryFocusSleepTics[id];
    s32 count;

    if (id == 1)
    {
        gcSleepCurrentGObjThread(90);
    }
    count = gSCManagerBattleState->pl_count + gSCManagerBattleState->cp_count;

    if (count < 3)
    {
        gcSleepCurrentGObjThread(sleep_tics);
    }
    fighter_gobj = gGCCommonLinks[nGCCommonLinkIDFighter];

    while (fighter_gobj != NULL)
    {
        ftCommonAppearSetStatus(fighter_gobj);

        if (id == 2)
        {
            gcSleepCurrentGObjThread(30);
            gmCameraSetStatusPlayerZoom(fighter_gobj, 0.0F, 0.0F, ftGetStruct(fighter_gobj)->attr->closeup_camera_zoom, 0.1F, 28.0F);
            gcSleepCurrentGObjThread(sleep_tics - 30);
        }
        else gcSleepCurrentGObjThread(sleep_tics);

        fighter_gobj = fighter_gobj->link_next;
    }
    if (id == 2)
    {
        gcSleepCurrentGObjThread(30);
        gmCameraSetStatusDefault();
    }
    gcEjectGObj(NULL);
    gcSleepCurrentGObjThread(1);
}

// 0x801129DC
void ifCommonEntryFocusMakeInterface(s32 id)
{
    GObj *interface_gobj = gcMakeGObjSPAfter(nGCCommonKindInterface, NULL, nGCCommonLinkIDInterfaceActor, GOBJ_PRIORITY_DEFAULT);

    gcAddGObjProcess(interface_gobj, ifCommonEntryFocusThread, nGCProcessKindThread, 5);

    interface_gobj->user_data.s = id;
}

// 0x80112A34
void ifCommonEntryAllThread(GObj *interface_gobj)
{
    gcSleepCurrentGObjThread(90);
    ifCommonCountdownMakeInterface();
    ifCommonEntryFocusMakeInterface(syUtilsRandIntRange(3));
    gcEjectGObj(NULL);
    gcSleepCurrentGObjThread(1);
}

// 0x80112A80
void ifCommonEntryAllMakeInterface(void)
{
    gcAddGObjProcess(gcMakeGObjSPAfter(nGCCommonKindInterface, NULL, nGCCommonLinkIDInterfaceActor, GOBJ_PRIORITY_DEFAULT), ifCommonEntryAllThread, nGCProcessKindThread, 5);

    gSCManagerBattleState->game_status = nSCBattleGameStatusWait;
}

// 0x80112AD0
void ifCommonSuddenDeathThread(GObj *interface_gobj)
{
    gcSleepCurrentGObjThread(90);
    ifCommonAnnounceGoMakeInterface();
    ifCommonPlayerDamageSetShowInterface();
    ifCommonAnnounceGoSetStatus();
    func_800269C0_275C0(nSYAudioVoiceAnnounceGo);
    gcEjectGObj(NULL);
    gcSleepCurrentGObjThread(1);
}

// 0x80112B24
void ifCommonAnnounceSetColors(GObj *interface_gobj, SYColorRGBPair *colors)
{
    SObj *sobj = SObjGetStruct(interface_gobj);

    while (sobj != NULL)
    {
        sobj->sprite.red   = colors->prim.r;
        sobj->sprite.green = colors->prim.g;
        sobj->sprite.blue  = colors->prim.b;
        sobj->envcolor.r = colors->env.r;
        sobj->envcolor.g = colors->env.g;
        sobj->envcolor.b = colors->env.b;

        sobj = sobj->next;
    }
}

// 0x80112B74
void ifCommonSuddenDeathMakeInterface(void)
{
    GObj *interface_gobj = gcMakeGObjSPAfter(nGCCommonKindInterface, NULL, nGCCommonLinkIDInterface, GOBJ_PRIORITY_DEFAULT);

    gcAddGObjDisplay(interface_gobj, lbCommonDrawSObjAttr, 23, GOBJ_PRIORITY_DEFAULT, ~0);
    gcAddGObjProcess(interface_gobj, ifCommonSuddenDeathThread, nGCProcessKindThread, 5);
    ifCommonAnnounceSetAttr(interface_gobj, 7, dIFCommonAnnounceSuddenDeathSpriteData, ARRAY_COUNT(dIFCommonAnnounceSuddenDeathSpriteData));
    ifCommonAnnounceSetColors(interface_gobj, &dIFCommonAnnounceSuddenDeathSpriteColors);
    func_800269C0_275C0(nSYAudioVoiceAnnounceSuddenDeath);

    gSCManagerBattleState->game_status = nSCBattleGameStatusWait;
}

// 0x80112C18
void ifCommonTimerProcDisplay(GObj *interface_gobj)
{
    s32 digit;
    s32 i;
    s32 time;
    SObj *sobj;

    sobj = SObjGetStruct(interface_gobj);

    if (gSCManagerBattleState->time_remain == 0)
    {
        sobj = sobj->next->next->next;

        sobj->sprite = *lbRelocGetFileData(Sprite*, gGMCommonFiles[3], dIFCommonTimerDigitSpriteOffsets[0]);

        sobj->pos.x = (s32)(dIFCommonTimerDigitsSpritePositionsX[3] - (sobj->sprite.width * 0.5F));
        sobj->pos.y = (s32)(30.0F - (sobj->sprite.height * 0.5F));
    }
    else
    {
        if (gSCManagerBattleState->time_remain == sIFCommonTimerLimit)
        {
            time = gSCManagerBattleState->time_remain;
        }
        else time = gSCManagerBattleState->time_remain + 59;

        for (i = 0; i < ARRAY_COUNT(sIFCommonTimerDigitsInterface); i++, sobj = sobj->next)
        {
            digit = time / dIFCommonTimerDigitsUnitLengths[i];

            time -= (digit * dIFCommonTimerDigitsUnitLengths[i]);

            if (sIFCommonTimerDigitsInterface[i] != digit)
            {
                sobj->sprite = *lbRelocGetFileData(Sprite*, gGMCommonFiles[3], dIFCommonTimerDigitSpriteOffsets[digit]);

                sobj->pos.x = (s32)(dIFCommonTimerDigitsSpritePositionsX[i] - (sobj->sprite.width * 0.5F));
                sobj->pos.y = (s32)(30.0F - (sobj->sprite.height * 0.5F));

                sIFCommonTimerDigitsInterface[i] = digit;
            }
        }
    }
    lbCommonDrawSObjAttr(interface_gobj);
}

// 0x80112EBC
void ifCommonTimerSetAttr(void)
{
    s32 i;

    for (i = 0; i < ARRAY_COUNT(dIFCommonTimerDigitSpriteOffsets); i++)
    {
        lbRelocGetFileData(Sprite*, gGMCommonFiles[3], dIFCommonTimerDigitSpriteOffsets[i])->attr = SP_TEXSHUF | SP_TRANSPARENT;
    }
}

// 0x80112F3C
void ifCommonTimerInitAnnouncedSeconds(void)
{
    s32 i;

    for (i = 0; i < ARRAY_COUNT(sIFCommonIsAnnouncedSecond); i++)
    {
        sIFCommonIsAnnouncedSecond[i] = FALSE;
    }
}

// 0x80112F68
SObj* ifCommonTimerMakeDigits(void)
{
    GObj *interface_gobj;
    SObj *sobj;

    if (!(gSCManagerBattleState->game_rules & SCBATTLE_GAMERULE_TIME) || (gSCManagerBattleState->time_limit == SCBATTLE_TIMELIMIT_INFINITE))
    {
        return NULL;
    }

    ifCommonTimerSetAttr();

    interface_gobj = gcMakeGObjSPAfter(nGCCommonKindInterface, NULL, nGCCommonLinkIDInterface, GOBJ_PRIORITY_DEFAULT);

    gcAddGObjDisplay(interface_gobj, ifCommonTimerProcDisplay, 23, GOBJ_PRIORITY_DEFAULT, ~0);

    lbCommonMakeSObjForGObj(interface_gobj, lbRelocGetFileData(Sprite*, gGMCommonFiles[3], (intptr_t)&llIFCommonTimerDigit0Sprite));
    lbCommonMakeSObjForGObj(interface_gobj, lbRelocGetFileData(Sprite*, gGMCommonFiles[3], (intptr_t)&llIFCommonTimerDigit0Sprite));
    lbCommonMakeSObjForGObj(interface_gobj, lbRelocGetFileData(Sprite*, gGMCommonFiles[3], (intptr_t)&llIFCommonTimerDigit0Sprite));
    lbCommonMakeSObjForGObj(interface_gobj, lbRelocGetFileData(Sprite*, gGMCommonFiles[3], (intptr_t)&llIFCommonTimerDigit0Sprite));

    sobj = lbCommonMakeSObjForGObj(interface_gobj, lbRelocGetFileData(Sprite*, gGMCommonFiles[3], &llIFCommonTimerSymbolColonSprite));

    sobj->pos.x = (s32)(260.0F - (sobj->sprite.width * 0.5F));
    sobj->pos.y = (s32)(30.0F - (sobj->sprite.height * 0.5F));

    sIFCommonTimerDigitsInterface[0] = sIFCommonTimerDigitsInterface[1] = sIFCommonTimerDigitsInterface[2] = sIFCommonTimerDigitsInterface[3] = 10;

    return sobj;
}

// 0x80113104
void ifCommonTimerFuncRun(GObj *interface_gobj)
{
    u32 time_delta;
    u32 time_update;
    s32 i;

    if (sIFCommonTimerIsStarted != FALSE)
    {
        time_update = sySchedulerGetTicCount();
        time_delta = time_update - sIFCommonTimerStamp;

        if (time_delta != 0)
        {
            sIFCommonTimerStamp = time_update;
            gSCManagerBattleState->time_passed += time_delta;

            if ((gSCManagerBattleState->game_rules & SCBATTLE_GAMERULE_TIME) && (gSCManagerBattleState->time_limit != SCBATTLE_TIMELIMIT_INFINITE))
            {
                if (gSCManagerBattleState->time_remain != 0)
                {
                    if (gSCManagerBattleState->time_remain < time_delta)
                    {
                        gSCManagerBattleState->time_remain = 0;
                    }
                    else gSCManagerBattleState->time_remain -= time_delta;

                    if
                    (
                        (gSCManagerBattleState->gkind == nGRKindInishie)          &&
                        (gSCManagerBattleState->time_remain <= I_SEC_TO_TICS(30)) &&
                        (gMPCollisionBGMDefault != nSYAudioBGMInishieHurry)
                    )
                    {
                        gMPCollisionBGMDefault = nSYAudioBGMInishieHurry;
                        ftParamTryUpdateItemMusic();
                    }
                    if (gSCManagerBattleState->time_remain <= I_SEC_TO_TICS(5))
                    {
                        if (gSCManagerBattleState->time_remain == 0)
                        {
                            ifGetProc(interface_gobj)();

                            gcEjectGObj(NULL);
                        }
                        else for (i = 0; i < ARRAY_COUNT(sIFCommonIsAnnouncedSecond); i++)
                        {
                            if ((sIFCommonIsAnnouncedSecond[i] == FALSE) && (gSCManagerBattleState->time_remain <= (I_SEC_TO_TICS(i) + I_SEC_TO_TICS(1))))
                            {
                                func_800269C0_275C0(dIFCommonAnnounceTimerVoiceIDs[i]);

                                sIFCommonIsAnnouncedSecond[i] = TRUE;
                            }
                        }
                        syAudioSetBGMVolume(0, ((gSCManagerBattleState->time_remain / F_SEC_TO_TICS(5)) * 20480.0F) + 10240.0F);
                    }
                }
            }
        }
    }
}

// 0x80113398
void ifCommonTimerMakeInterface(void (*proc)(void))
{
    gSCManagerBattleState->time_remain = sIFCommonTimerLimit = I_MIN_TO_TICS(gSCManagerBattleState->time_limit);
    gSCManagerBattleState->time_passed = 0;

    sIFCommonTimerIsStarted = FALSE;

    ifCommonTimerInitAnnouncedSeconds();
    ifSetProc(gcMakeGObjSPAfter(nGCCommonKindInterface, ifCommonTimerFuncRun, nGCCommonLinkIDInterfaceActor, GOBJ_PRIORITY_DEFAULT), proc);
}

// 0x8011341C
GObj* ifCommonAnnounceGameSetMakeInterface(void)
{
    GObj *interface_gobj = gcMakeGObjSPAfter(nGCCommonKindInterface, NULL, nGCCommonLinkIDInterface, GOBJ_PRIORITY_DEFAULT);

    gcAddGObjDisplay(interface_gobj, lbCommonDrawSObjAttr, 23, GOBJ_PRIORITY_DEFAULT, ~0);

    ifCommonAnnounceSetAttr(interface_gobj, 1, dIFCommonAnnounceGameSetSpriteData, ARRAY_COUNT(dIFCommonAnnounceGameSetSpriteData));

    return interface_gobj;
}

// 0x80113488
void ifCommonBattleInitPlacement(void)
{
    s32 i;
    s32 members[5]; // HAL goof?
    s32 teams;

    for (i = 0; i < ARRAY_COUNT(members); i++)
    {
        members[i] = 0;
    }
    switch (gSCManagerBattleState->is_team_battle)
    {
    case FALSE:
        for (i = 0; i < ARRAY_COUNT(gSCManagerBattleState->players); i++)
        {
            if (gSCManagerBattleState->players[i].pkind != nFTPlayerKindNot)
            {
                members[i]++;
            }
        }
        break;

    case TRUE:
        for (i = 0; i < ARRAY_COUNT(gSCManagerBattleState->players); i++)
        {
            if (gSCManagerBattleState->players[i].pkind != nFTPlayerKindNot)
            {
                members[gSCManagerBattleState->players[i].team]++;
            }
        }
        break;
    }
    for (i = teams = 0; i < ARRAY_COUNT(members); i++)
    {
        if (members[i] != 0)
        {
            teams++;
        }
    }
    sIFCommonBattlePlace = teams - 1;
}

// 0x80113638
void ifCommonBattleInterfacePauseGObj(GObj *interface_gobj, u32 unused)
{
    gcPauseGObjProcessAll(interface_gobj);

    interface_gobj->flags |= GOBJ_FLAG_NORUN;
}

// 0x8011366C
void ifCommonBattleInterfaceResumeGObj(GObj *interface_gobj, u32 unused)
{
    gcResumeGObjProcessAll(interface_gobj);

    interface_gobj->flags &= ~GOBJ_FLAG_NORUN;
}

// 0x801136A4
void ifCommonBattleInterfaceProcUpdate(void)
{
    gcFuncGObjAll(ifCommonBattleInterfacePauseGObj, 0);

    gcFuncGObjByLink(nGCCommonLinkIDSpecialEffect, ifCommonBattleInterfaceResumeGObj, 0);
    gcFuncGObjByLink(nGCCommonLinkIDInterface, ifCommonBattleInterfaceResumeGObj, 0);
    gmRumbleResumeProcessAll();
    ifCommonBattleInterfaceResumeGObj(gEFParticleStructsGObj, 0);
    ifCommonBattleInterfaceResumeGObj(gEFParticleGeneratorsGObj, 0);
    efParticleGObjSetSkipAll();
    efParticleGObjClearSkipID(2);
    efParticleGObjClearSkipID(3);
    func_800266A0_272A0();
    ifCommonBattleEndPlaySoundQueue();
}

// 0x80113744 - Unused?
void func_ovl2_80113744(GObj *fighter_gobj, u32 unused) 
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    
    if (fp->fkind == nFTKindBoss) 
    {
        gcResumeGObjProcessAll(fighter_gobj);
        
        fighter_gobj->flags &= ~GOBJ_FLAG_NORUN;
    }
}

// 0x80113790
void ifCommonBattleEndInitSoundNum(void)
{
    sIFCommonBattleEndSoundsNum = 0;
}

// 0x8011379C
void ifCommonBattleEndPlaySoundQueue(void)
{
    s32 i;

    for (i = 0; i < sIFCommonBattleEndSoundsNum; i++)
    {
        func_800269C0_275C0(sIFCommonBattleEndSoundsQueue[i]);
    }
}

// 0x80113804
void ifCommonBattleEndAddSoundQueueID(u16 sfx_id)
{
    if ((gSCManagerBattleState->game_status == nSCBattleGameStatusEnd) && (sIFCommonBattleEndSoundsNum < ARRAY_COUNT(sIFCommonBattleEndSoundsQueue)))
    {
        sIFCommonBattleEndSoundsQueue[sIFCommonBattleEndSoundsNum] = sfx_id;
        sIFCommonBattleEndSoundsNum++;
    }
}

// 0x80113854
void ifCommonBattleEndSetBossDefeat(void)
{
    func_ovl65_8018F6DC();

    gSCManagerBattleState->game_status = nSCBattleGameStatusBossDefeat;
    sIFCommonBattlePauseCameraRestoreWait = 0;
}

// 0x8011388C
void ifCommonBattleUpdateScoreStocks(FTStruct *fp)
{
    s32 teammates_remain; // Live teammates remaining
    s32 current_team; // Current team being checked
    s32 team; // Input player's team
    s32 i;

    team = fp->team;

    for (i = teammates_remain = 0; i < ARRAY_COUNT(gSCManagerBattleState->players); i++)
    {
        if (gSCManagerBattleState->players[i].pkind == nFTPlayerKindNot)
        {
            continue;
        }
        switch (gSCManagerBattleState->is_team_battle)
        {
        case FALSE:
            current_team = i;
            break;

        case TRUE:
            current_team = gSCManagerBattleState->players[i].team;
            break;
        }
        if ((current_team == team) && (gSCManagerBattleState->players[i].stock_count != -1))
        {
            teammates_remain++;
        }
    }
    if (teammates_remain == 0) // No players left on this team
    {
        switch (gSCManagerBattleState->is_team_battle)
        {
        case FALSE:
            gSCManagerBattleState->players[team].place = sIFCommonBattlePlace;
            break;

        case TRUE:
            for (i = 0; i < ARRAY_COUNT(gSCManagerBattleState->players); i++)
            {
                if (gSCManagerBattleState->players[i].pkind == nFTPlayerKindNot)
                {
                    continue;
                }
                if (gSCManagerBattleState->players[i].team == team)
                {
                    gSCManagerBattleState->players[i].place = sIFCommonBattlePlace;
                }
            }
            break;
        }
        sIFCommonBattlePlace--;

        if (sIFCommonBattlePlace == 0)
        {
            ifCommonAnnounceEndMessage();
        }
    }
    if ((sIFCommonBattlePlace != 0) && (fp->stock_count == -1))
    {
        if (fp->pkind == nFTPlayerKindMan)
        {
            ftPublicDefeatedAddID(dIFCommonAnnounceDefeatedVoiceIDs[fp->player]);
        }
        else ftPublicDefeatedAddID(nSYAudioVoiceAnnounceComputerPlayer);

        ftPublicDefeatedAddID(nSYAudioVoiceAnnounceDefeated);
    }
}

// 0x80113AA8
void ifCommonBattlePauseProcDisplay(GObj *interface_gobj)
{
    s32 i;

    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_FILL);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_NOOP, G_RM_NOOP2);
    gDPSetFillColor(gSYTaskmanDLHeads[0]++, GPACK_FILL16(GPACK_RGBA5551(0xFF, 0xFF, 0xFF, 0x01)));

    for (i = 0; i < ARRAY_COUNT(dIFCommonBattlePauseBorderRectangle); i++)
    {
        gDPFillRectangle
        (
            gSYTaskmanDLHeads[0]++, 
            dIFCommonBattlePauseBorderRectangle[i].ulx, 
            dIFCommonBattlePauseBorderRectangle[i].uly, 
            dIFCommonBattlePauseBorderRectangle[i].lrx, 
            dIFCommonBattlePauseBorderRectangle[i].lry
        );
    }
    lbCommonClearExternSpriteParams();
}

// 0x80113CF8
void ifCommonBattlePausePlayerNumMakeSObj(GObj *interface_gobj, s32 player)
{
    SObj *sobj = lbCommonMakeSObjForGObj(interface_gobj, lbRelocGetFileData(Sprite*, gGMCommonFiles[5], dIFCommonBattlePausePlayerNumSpriteOffsets[player]));

    sobj->sprite.red   = 0xFF;
    sobj->sprite.green = 0xFF;
    sobj->sprite.blue  = 0xFF;

    sobj->sprite.attr = SP_TEXSHUF | SP_TRANSPARENT;

    sobj->pos.x = 213.0F;
    sobj->pos.y = 191.0F;
}

// 0x80113D60
void ifCommonBattlePauseDecalMakeSObjID(GObj *interface_gobj, s32 id)
{
    SObj *sobj = lbCommonMakeSObjForGObj(interface_gobj, lbRelocGetFileData(Sprite*, gGMCommonFiles[5], dIFCommonBattlePauseDecalsSpriteData[id].offset));

    sobj->pos.x = dIFCommonBattlePauseDecalsSpriteData[id].pos.x;
    sobj->pos.y = dIFCommonBattlePauseDecalsSpriteData[id].pos.y;

    sobj->sprite.red   = dIFCommonBattlePauseDecalsSpriteData[id].colors.prim.r;
    sobj->sprite.green = dIFCommonBattlePauseDecalsSpriteData[id].colors.prim.g;
    sobj->sprite.blue  = dIFCommonBattlePauseDecalsSpriteData[id].colors.prim.b;

    sobj->envcolor.r = dIFCommonBattlePauseDecalsSpriteData[id].colors.env.r;
    sobj->envcolor.g = dIFCommonBattlePauseDecalsSpriteData[id].colors.env.g;
    sobj->envcolor.b = dIFCommonBattlePauseDecalsSpriteData[id].colors.env.b;

    sobj->sprite.attr = SP_TEXSHUF | SP_TRANSPARENT;
}

// 0x80113E04
void ifCommonBattlePauseMakeSObjsAll(GObj *interface_gobj)
{
    // TO DO: make an enum for pause menu icon indexes
    s32 draw_count = (sIFCommonBattlePauseKindInterface != nIFPauseKindDefault) ? 10 : 12;
    s32 i;

    for (i = 0; i < draw_count; i++)
    {
        ifCommonBattlePauseDecalMakeSObjID(interface_gobj, i);
    }
    // If we're in Bonus Practice, display "L: RETRY" in the bottom left corner
    if ((gSCManagerSceneData.scene_curr == nSCKind1PBonusStage) && (gSCManagerSceneData.scene_prev != nSCKind1PGame)) 
    {
        // WARNING: This needs to be updated in case the pause menu icon array is expanded
        for (i = 12; i < ARRAY_COUNT(dIFCommonBattlePauseDecalsSpriteData); i++)
        {
            ifCommonBattlePauseDecalMakeSObjID(interface_gobj, i);
        }
    }
}

// 0x80113EB4
void ifCommonBattlePauseMakeInterface(s32 player)
{
    GObj *interface_gobj = gcMakeGObjSPAfter(nGCCommonKindPauseMenu, NULL, nGCCommonLinkIDPauseMenu, GOBJ_PRIORITY_DEFAULT);

    gcAddGObjDisplay(interface_gobj, ifCommonBattlePauseProcDisplay, 24, GOBJ_PRIORITY_DEFAULT, ~0);

    interface_gobj = gcMakeGObjSPAfter(nGCCommonKindPauseMenu, NULL, nGCCommonLinkIDPauseMenu, GOBJ_PRIORITY_DEFAULT);

    gcAddGObjDisplay(interface_gobj, lbCommonDrawSObjAttr, 24, GOBJ_PRIORITY_DEFAULT, ~0);

    ifCommonBattlePausePlayerNumMakeSObj(interface_gobj, player);
    ifCommonBattlePauseMakeSObjsAll(interface_gobj);
}

// 0x80113F50
void ifCommonBattlePauseEjectGObjs(void)
{
    lbCommonEjectGObjLinkedList(gGCCommonLinks[nGCCommonLinkIDPauseMenu]);
}

// 0x80113F74
void ifCommonInterfaceSetGObjFlagsAll(u32 flags)
{
    GObj *interface_gobj = gGCCommonLinks[nGCCommonLinkIDInterface];

    while (interface_gobj != NULL)
    {
        interface_gobj->flags = flags;

        interface_gobj = interface_gobj->link_next;
    }
}

// 0x80113F9C
void ifCommonBattlePauseSetGObjFlagsAll(u32 flags)
{
    GObj *gobj = gGCCommonLinks[nGCCommonLinkIDPauseMenu];

    while (gobj != NULL)
    {
        gobj->flags = flags;

        gobj = gobj->link_next;
    }
}

// 0x80113FC4
void ifCommonBattlePauseInitInterface(s32 player)
{
    ifCommonInterfaceSetGObjFlagsAll(GOBJ_FLAG_HIDDEN);
    gmRumbleInitPlayers();
    grWallpaperPausePerspUpdate();

    gIFCommonPlayerInterface.is_magnify_display = FALSE;
    gSCManagerBattleState->game_status = nSCBattleGameStatusPause;

    sIFCommonBattlePausePlayer = player;

    func_80026594_27194();
    func_800269C0_275C0(nSYAudioFGMGamePause);
    syAudioSetBGMVolume(0, 0x3C00);
    ifCommonBattlePauseMakeInterface(player);
}

// 0x8011403C
void ifCommonBattleGoUpdateInterface(void)
{
    GObj *fighter_gobj;
    FTStruct *fp;
    s32 player;
    Vec3f sp68;
    Vec3f sp5C;

    for (player = 0; player < (ARRAY_COUNT(gSCManagerBattleState->players) + ARRAY_COUNT(gSYControllerDevices)) / 2; player++) // WARNING: GMCOMMON_PLAYERS_MAX and MAX_CONTROLLERS should be identical
    {
        if (gSYControllerDevices[player].button_tap & START_BUTTON)
        {
            if (gSCManagerBattleState->players[player].pkind != nFTPlayerKindNot)
            {
                if ((gSCManagerBattleState->gkind != nGRKindBonus3) || (gSCManagerBattleState->players[player].pkind != nFTPlayerKindCom))
                {
                    fighter_gobj = gSCManagerBattleState->players[player].fighter_gobj;

                    fp = ftGetStruct(fighter_gobj);

                    if ((fp->status_id == nFTCommonStatusSleep) && (ftCommonSleepCheckIgnorePauseMenu(fighter_gobj) != FALSE)) 
                    {
                        continue;
                    }
                    if (!(fp->is_menu_ignore))
                    {
                        if (gSCManagerBattleState->game_type == nSCBattleGameTypeBonus)
                        {
                            sp68.x = gMPCollisionGroundData->zoom_start.x;
                            sp68.y = gMPCollisionGroundData->zoom_start.y;
                            sp68.z = gMPCollisionGroundData->zoom_start.z;

                            sp5C.x = gMPCollisionGroundData->zoom_end.x;
                            sp5C.y = gMPCollisionGroundData->zoom_end.y;
                            sp5C.z = gMPCollisionGroundData->zoom_end.z;

                            gmCameraSetStatusMapZoom(&sp68, &sp5C);

                            sIFCommonBattlePauseKindInterface = nIFPauseKindBonus;
                        }
                        else if (gmCameraCheckPausePlayerOutBounds(&DObjGetStruct(fighter_gobj)->translate.vec.f) != FALSE)
                        {
                            sIFCommonBattlePauseKindInterface = nIFPauseKindPlayerNA;
                        }
                        else
                        {
                            gmCameraSetStatusPlayerZoom(fighter_gobj, 0.0F, 0.0F, ftGetStruct(fighter_gobj)->attr->closeup_camera_zoom, 0.1F, 29.0F);

                            sIFCommonBattlePauseCameraEyeXOrigin = gGMCameraPauseCameraEyeX;
                            sIFCommonBattlePauseCameraEyeYOrigin = gGMCameraPauseCameraEyeY;

                            sIFCommonBattlePauseKindInterface = nIFPauseKindDefault;

                            sIFCommonBattlePausePlayerDetail = fp->detail_curr;

                            ftParamSetModelPartDetailAll(fighter_gobj, nFTPartsDetailHigh);
                        }
                        ifCommonBattlePauseInitInterface(player);

                        return;
                    }
                }
            }
            sIFCommonBattlePauseKindInterface = 0;

            ifCommonBattlePauseInitInterface(player);

            return;
        }
    }
    gcRunAll();
}

// 0x801142B4
void ifCommonBattleInterfaceProcSet(void)
{
    ifCommonInterfaceSetGObjFlagsAll(GOBJ_FLAG_HIDDEN);

    gSCManagerBattleState->game_status = nSCBattleGameStatusSet;

    sIFCommonBattlePauseCameraRestoreWait = 3;
}

// 0x801142EC
void ifCommonBattlePauseUpdateInterface(void)
{
    u16 button_tap = gSYControllerDevices[sIFCommonBattlePausePlayer].button_tap;
    u16 button_hold = gSYControllerDevices[sIFCommonBattlePausePlayer].button_hold;

    if (sIFCommonBattlePauseKindInterface == nIFPauseKindDefault)
    {
        s32 stick_x = gSYControllerDevices[sIFCommonBattlePausePlayer].stick_range.x;
        s32 stick_y = gSYControllerDevices[sIFCommonBattlePausePlayer].stick_range.y;

        if (ABS(stick_x) > 8.0F)
        {
            gGMCameraPauseCameraEyeX += (stick_x * 0.000333F);

            if (gGMCameraPauseCameraEyeX > F_CLC_DTOR32(50.0F))
            {
                gGMCameraPauseCameraEyeX = F_CLC_DTOR32(50.0F);
            }
            else if (gGMCameraPauseCameraEyeX < F_CLC_DTOR32(-50.0F))
            {
                gGMCameraPauseCameraEyeX = F_CLC_DTOR32(-50.0F);
            }
        }
        if (ABS(stick_y) > 8.0F)
        {
            gGMCameraPauseCameraEyeY -= (stick_y * 0.000333F);

            if (gGMCameraPauseCameraEyeY > F_CLC_DTOR32(20.0F))
            {
                gGMCameraPauseCameraEyeY = F_CLC_DTOR32(20.0F);
            }
            else if (gGMCameraPauseCameraEyeY < F_CLC_DTOR32(-20.0F))
            {
                gGMCameraPauseCameraEyeY = F_CLC_DTOR32(-20.0F);
            }
        }
    }
    if (button_tap)
    {
        if (button_tap & START_BUTTON)
        {
            if (sIFCommonBattlePauseKindInterface != nIFPauseKindPlayerNA)
            {
                gmCameraSetStatusPrev();

                sIFCommonBattlePauseCameraRestoreWait = 20;
            }
            else sIFCommonBattlePauseCameraRestoreWait = 0;

            gSCManagerBattleState->game_status = nSCBattleGameStatusUnpause;

            return;
        }
        else button_hold = gSYControllerDevices[sIFCommonBattlePausePlayer].button_hold;

        if
        (
            (button_hold & A_BUTTON) &&
            (button_hold & B_BUTTON) &&
            (button_hold & R_TRIG) &&
            (button_hold & Z_TRIG)
        )
        {
            gSCManagerSceneData.is_reset = TRUE;

            gcFuncGObjAll(ifCommonBattleInterfacePauseGObj, 0);
            func_800266A0_272A0();
            gmRumbleInitPlayers();
            ifCommonBattlePauseSetGObjFlagsAll(GOBJ_FLAG_HIDDEN);
            ifCommonBattleInterfaceProcSet();

            return;
        }
        if ((button_tap & L_TRIG) && (gSCManagerSceneData.scene_curr == nSCKind1PBonusStage) && (gSCManagerSceneData.scene_prev != nSCKind1PGame))
        {
            func_800266A0_272A0();
            gmRumbleInitPlayers();
            syTaskmanSetLoadScene();

            return;
        }
    }
    if (sIFCommonBattlePauseKindInterface != nIFPauseKindPlayerNA)
    {
        gmCameraRunFuncCamera(gGMCameraGObj);
        grWallpaperRunProcessAll();
    }
}

// 0x80114588
void ifCommonBattlePauseRestoreInterfaceAll(void)
{
    if (sIFCommonBattlePauseCameraRestoreWait != 0)
    {
        sIFCommonBattlePauseCameraRestoreWait--;

        gGMCameraPauseCameraEyeX += (sIFCommonBattlePauseCameraEyeXOrigin - gGMCameraPauseCameraEyeX) * 0.1F;
        gGMCameraPauseCameraEyeY += (sIFCommonBattlePauseCameraEyeYOrigin - gGMCameraPauseCameraEyeY) * 0.1F;

        gmCameraRunFuncCamera(gGMCameraGObj);
        grWallpaperRunProcessAll();

        return;
    }
    ifCommonBattlePauseEjectGObjs();
    ifCommonInterfaceSetGObjFlagsAll(0);
    grWallpaperResumePerspUpdate();

    gIFCommonPlayerInterface.is_magnify_display = TRUE;

    gSCManagerBattleState->game_status = nSCBattleGameStatusGo;

    gGMCameraPauseCameraEyeX = sIFCommonBattlePauseCameraEyeXOrigin;
    gGMCameraPauseCameraEyeY = sIFCommonBattlePauseCameraEyeYOrigin;

    func_800264A4_270A4();
    syAudioSetBGMVolume(0, 0x7800);

    if (sIFCommonBattlePauseKindInterface == nIFPauseKindDefault)
    {
        ftParamSetModelPartDetailAll(gSCManagerBattleState->players[sIFCommonBattlePausePlayer].fighter_gobj, sIFCommonBattlePausePlayerDetail);
    }
    gcRunAll();
}

// 0x801146DC
void ifCommonBattleEndUpdateInterface(void)
{
    sIFCommonBattleInterfaceProcUpdate();

    gSCManagerBattleState->game_status = nSCBattleGameStatusBossDefeat;

    dIFCommonBattleBossUpdateInterval = dIFCommonBattleBossUpdateWait = 0;
}

// 0x80114724
void ifCommonBattleBossDefeatUpdateInterface(void)
{
    if (sIFCommonBattlePauseCameraRestoreWait != 0)
    {
        sIFCommonBattlePauseCameraRestoreWait--;
    }
    else sIFCommonBattleInterfaceProcSet();

    if (dIFCommonBattleBossUpdateWait == 0)
    {
        gcRunAll();

        dIFCommonBattleBossUpdateWait = dIFCommonBattleBossUpdateInterval;
    }
    else
    {
        gmCameraRunFuncCamera(gGMCameraGObj);
        grWallpaperRunProcessAll();

        dIFCommonBattleBossUpdateWait--;
    }
}

// 0x801147BC
void ifCommonBattleSetUpdateInterface(void)
{
    if (sIFCommonBattlePauseCameraRestoreWait != 0)
    {
        sIFCommonBattlePauseCameraRestoreWait--;
    }
    else syTaskmanSetLoadScene();

    gcRunAll();
}

// 0x80114800
void ifCommonSetMaxNumGObj(void)
{
    size_t free_space = (size_t) ((uintptr_t)gSYTaskmanGeneralHeap.end - (uintptr_t)gSYTaskmanGeneralHeap.ptr);

    if ((gcGetMaxNumGObj() == -1) && (free_space < 25 * 1024))
    {
        gcSetMaxNumGObj(gcGetGObjsActiveNum());
    }
}

// 0x8011485C
void ifCommonBattleUpdateInterfaceAll(void)
{
    if (gSCManagerBattleState->game_status != nSCBattleGameStatusGo)
    {
        sIFCommonTimerIsStarted = FALSE;
    }
    else if (sIFCommonTimerIsStarted == FALSE)
    {
        sIFCommonTimerIsStarted = TRUE;
        sIFCommonTimerStamp = 0;

        sySchedulerSetTicCount(0);
    }
    switch (gSCManagerBattleState->game_status)
    {
    case nSCBattleGameStatusWait:
        gcRunAll();
        break;

    case nSCBattleGameStatusGo:
        ifCommonBattleGoUpdateInterface();
        break;

    case nSCBattleGameStatusPause:
        ifCommonBattlePauseUpdateInterface();
        break;

    case nSCBattleGameStatusUnpause:
        ifCommonBattlePauseRestoreInterfaceAll();
        break;

    case nSCBattleGameStatusEnd:
        ifCommonBattleEndUpdateInterface();
        /* fallthrough */

    case nSCBattleGameStatusBossDefeat:
        ifCommonBattleBossDefeatUpdateInterface();
        break;

    case nSCBattleGameStatusSet:
        ifCommonBattleSetUpdateInterface();
        break;
    }
    ifCommonSetMaxNumGObj();
}

// 0x80114958
void ifCommonBattleSetGameStatusWait(void)
{
    gSCManagerBattleState->game_status = nSCBattleGameStatusWait;
}

// 0x80114968
void ifCommonPlayerStockMakeStockSnap(FTStruct *fp)
{
    efManagerStockSnapMakeEffect(gIFCommonPlayerInterface.player_pos_x[fp->player], gIFCommonPlayerInterface.player_pos_y);
}

// 0x801149CC
void ifCommonPlayerScoreMakeEffect(FTStruct *fp, s32 score)
{
    Vec3f pos;

    pos.x = ((gIFCommonPlayerInterface.player_pos_x[fp->player] + dIFCommonPlayerScorePositionOffsetsX[fp->player]) << 2);
    pos.y = ((gIFCommonPlayerInterface.player_pos_y + 13) << 2); // ??? Can't get this one to match unless we do bitwise instead of literal multiplication
    pos.z = 0.0F;

    efManagerBattleScoreMakeEffect(&pos, score);
}

// 0x80114A48
GObj* ifCommonAnnounceFailureMakeInterface(void)
{
    GObj *interface_gobj = gcMakeGObjSPAfter(nGCCommonKindInterface, NULL, nGCCommonLinkIDInterface, GOBJ_PRIORITY_DEFAULT);

    gcAddGObjDisplay(interface_gobj, lbCommonDrawSObjAttr, 23, GOBJ_PRIORITY_DEFAULT, ~0);
    ifCommonAnnounceSetAttr(interface_gobj, 7, dIFCommonAnnounceFailureSpriteData, ARRAY_COUNT(dIFCommonAnnounceFailureSpriteData));
    ifCommonAnnounceSetColors(interface_gobj, &dIFCommonAnnounceFailureSpriteColors);

    return interface_gobj;
}

// 0x8014AC4
GObj* ifCommonAnnounceCompleteMakeInterface(void)
{
    GObj *interface_gobj = gcMakeGObjSPAfter(nGCCommonKindInterface, NULL, nGCCommonLinkIDInterface, GOBJ_PRIORITY_DEFAULT);

    gcAddGObjDisplay(interface_gobj, lbCommonDrawSObjAttr, 23, GOBJ_PRIORITY_DEFAULT, ~0);
    ifCommonAnnounceSetAttr(interface_gobj, 7, dIFCommonAnnounceCompleteSpriteData, ARRAY_COUNT(dIFCommonAnnounceCompleteSpriteData));
    ifCommonAnnounceSetColors(interface_gobj, &dIFCommonAnnounceCompleteSpriteColors);

    return interface_gobj;
}

// 0x80114B40
void ifCommonBonusInterfaceProcUpdate(void)
{
    gcFuncGObjAll(ifCommonBattleInterfacePauseGObj, 0);
    gmRumbleInitPlayers();
    func_800266A0_272A0();
    ifCommonBattleEndPlaySoundQueue();
}

// 0x80114B80
void ifCommonBattleSetInterface(void (*proc_update)(void), void (*proc_set)(void), u16 sfx_id, u16 restore_wait)
{
    gSCManagerBattleState->game_status = nSCBattleGameStatusEnd;
    sIFCommonBattlePauseCameraRestoreWait = restore_wait;

    sIFCommonBattleInterfaceProcUpdate = proc_update;
    sIFCommonBattleInterfaceProcSet = proc_set;

    ifCommonBattleEndInitSoundNum();

    if (sfx_id != nSYAudioFGMVoiceEnd)
    {
        ifCommonBattleEndAddSoundQueueID(sfx_id);
    }
}

// 0x80114BE4
void ifCommonBattleBossDefeatSetGameStatus(void)
{
    gSCManagerBattleState->game_status = nSCBattleGameStatusBossDefeat;

    sIFCommonBattlePauseCameraRestoreWait = U16_MAX;

    sIFCommonBattleInterfaceProcSet = ifCommonBattleInterfaceProcSet;
    dIFCommonBattleBossUpdateInterval = 2;
}

// 0x80114C20
void ifCommon1PGameInterfaceProcSet(void)
{
    gcFuncGObjByLink(nGCCommonLinkIDCamera, ifCommonBattleInterfaceResumeGObj, 0);
    grWallpaperResumeProcessAll();
    ifCommonInterfaceSetGObjFlagsAll(GOBJ_FLAG_HIDDEN);

    gSCManagerBattleState->game_status = nSCBattleGameStatusSet;

    sIFCommonBattlePauseCameraRestoreWait = 45;

    sc1PGameSetCameraZoom();

    gIFCommonPlayerInterface.is_magnify_display = FALSE;
}

// 0x80114C80
void ifCommonAnnounceEndMessage(void)
{
    if (gSCManagerBattleState->gkind >= nGRKindBonusStageStart)
    {
        ifCommonBattleSetInterface(ifCommonBattleInterfaceProcUpdate, ifCommonBattleInterfaceProcSet, nSYAudioVoiceAnnounceFailure, 90);
        ifCommonAnnounceFailureMakeInterface();
    }
    else
    {
        if ((gSCManagerBattleState->game_type == nSCBattleGameType1PGame) && (gSCManagerBattleState->players[gSCManagerSceneData.player].stock_count != -1))
        {
            ifCommonBattleSetInterface(ifCommonBattleInterfaceProcUpdate, ifCommon1PGameInterfaceProcSet, nSYAudioVoiceAnnounceGameSet, 90);
        }
        else ifCommonBattleSetInterface(ifCommonBattleInterfaceProcUpdate, ifCommonBattleInterfaceProcSet, nSYAudioVoiceAnnounceGameSet, 90);

        ifCommonAnnounceGameSetMakeInterface();
    }
}

// 0x80114D58
void ifCommonAnnounceCompleteInitInterface(u16 sfx_id)
{
    ifCommonBattleSetInterface(ifCommonBonusInterfaceProcUpdate, ifCommonBattleInterfaceProcSet, sfx_id, 90);
    ifCommonAnnounceCompleteMakeInterface();
}

// 0x80114D98
void ifCommonAnnounceTimeUpInitInterface(void)
{
    ifCommonBattleSetInterface(ifCommonBonusInterfaceProcUpdate, ifCommonBattleInterfaceProcSet, nSYAudioVoiceAnnounceTimeUp, 90);
    ifCommonAnnounceTimeUpMakeInterface();
}

// 0x80114DD4
void ifCommonAnnounceFailureInitInterface(void)
{
#if defined(REGION_US)
    if (gSCManagerBattleState->game_status != nSCBattleGameStatusEnd)
    {
#endif
        ifCommonBattleSetInterface(ifCommonBonusInterfaceProcUpdate, ifCommonBattleInterfaceProcSet, nSYAudioVoiceAnnounceFailure, 90);
        ifCommonAnnounceFailureMakeInterface();
#if defined(REGION_US)
    }
#endif
}
