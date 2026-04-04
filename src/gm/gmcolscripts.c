#include <ef/effect.h>
#include <gm/gmsound.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x8012CA90
u32 dGMColScriptsFighterComPlayer[/* */] =
{
	gmColCommandSetColor1(0xFF, 0xFF, 0xFF, 0x30),
	gmColCommandWait(65535),
	gmColCommandGoto(dGMColScriptsFighterComPlayer)
};

// 0x8012CAA4
u32 dGMColScriptsFighterHitStatusNormal[/* */] =
{
	gmColCommandClearColorAll(),
	gmColCommandEnd()
};

// 0x8012CAAC
u32 dGMColScriptsFighterHitStatusIntangible[/* */] =
{
	gmColCommandSetColor1(0xFF, 0xFF, 0xFF, 0x82),
	gmColCommandBlendColor1(3, 0xFF, 0xFF, 0xFF, 0x32),
	gmColCommandWait(3),
	gmColCommandClearColorAll(),
	gmColCommandWait(1),
	gmColCommandGoto(dGMColScriptsFighterHitStatusIntangible)
};

// 0x8012CAD0
u32 dGMColScriptsFighterHitStatusInvincible[/* */] =
{
	gmColCommandSetColor1(0x80, 0xFF, 0x80, 0x50),
	gmColCommandBlendColor1(3, 0x80, 0xFF, 0x80, 0x14),
	gmColCommandWait(3),
	gmColCommandClearColorAll(),
	gmColCommandWait(1),
	gmColCommandGoto(dGMColScriptsFighterHitStatusInvincible)
};

// 0x8012CAF4
u32 dGMColScriptsFighterDamageCommon[/* */] =
{
	gmColCommandSetLight(90, 0),
	gmColCommandClearColorAll(),
	gmColCommandWait(1),
	gmColCommandSetColor1(0xFF, 0xFF, 0xFF, 0xE6),
	gmColCommandWait(1),
	gmColCommandBlendColor1(6, 0xFF, 0xFF, 0xFF, 0x1E),
	gmColCommandWait(6),
	gmColCommandClearColorAll(),
	gmColCommandEnd()
};

// 0x8012CB20
u32 dGMColScriptsFighterCommonSpecialNCharge[/* */] =
{
	gmColCommandEffect(0, nEFKindChargeSparkle, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandLoopBegin(2),
	gmColCommandSetColor1(0xFF, 0xFF, 0xFF, 0x30),
	gmColCommandWait(1),
	gmColCommandBlendColor1(4, 0xFF, 0xFF, 0xFF, 0x00),
	gmColCommandWait(4),
	gmColCommandClearColorAll(),
	gmColCommandWait(1),
	gmColCommandLoopEnd(),
	gmColCommandGoto(dGMColScriptsFighterCommonSpecialNCharge)
};

// 0x8012CB60
u32 dGMColScriptsFighterFallSpecial[/* */] =
{
	gmColCommandSetColor1(0x00, 0x00, 0x00, 0x5A),
	gmColCommandWait(2),
	gmColCommandClearColorAll(),
	gmColCommandWait(2),
	gmColCommandGoto(dGMColScriptsFighterFallSpecial)
};

// 0x8012CB7C
u32 dGMColScriptsFighterFastFall[/* */] =
{
	gmColCommandEffect(0, nEFKindSparkleWhiteScale, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandSetColor1(0x00, 0x00, 0x00, 0x80),
	gmColCommandWait(2),
	gmColCommandEnd()
};

// 0x8012CB9C
u32 dGMColScriptsFighterHeal[/* */] =
{
	gmColCommandSetColor1(0xFF, 0xFF, 0x40, 0x96),
	gmColCommandEffect(0, nEFKindHealSparkles, 0, 0, 0, 0, 300, 300, 300),
	gmColCommandWait(2),
	gmColCommandSetColor1(0xFF, 0xFF, 0x40, 0x14),
	gmColCommandEffect(0, nEFKindHealSparkles, 0, 0, 0, 0, 300, 300, 300),
	gmColCommandWait(2),
	gmColCommandGoto(dGMColScriptsFighterHeal)
};

// 0x8012CBDC
u32 dGMColScriptsFighterNoDamage[/* */] =
{
	gmColCommandSetColor1(0xFF, 0xFF, 0xFF, 0x80),
	gmColCommandWait(1),
	gmColCommandBlendColor1(6, 0xFF, 0xC8, 0xA0, 0x1E),
	gmColCommandWait(6),
	gmColCommandClearColorAll(),
	gmColCommandWait(1),
	gmColCommandBlendColor1(3, 0xFF, 0xFF, 0xFF, 0xA0),
	gmColCommandWait(3),
	gmColCommandGoto(dGMColScriptsFighterNoDamage)
};

// 0x8012CC10
u32 dGMColScriptsFighterRebirth[/* */] =
{
	gmColCommandSetLight(0, -70),
	gmColCommandSetColor1(0xFF, 0xFF, 0xFF, 0xFF),
	gmColCommandWait(2),
	gmColCommandBlendColor1(36, 0xFF, 0xFF, 0xFF, 0x0A),
	gmColCommandWait(36),
	gmColCommandBlendColor1(18, 0xFF, 0xFF, 0xFF, 0xB4),
	gmColCommandWait(18),
	gmColCommandGoto(dGMColScriptsFighterRebirth)
};

// 0x8012CC40
u32 dGMColScriptsFighterDamageFireSub1[/* */] =
{
	gmColCommandSetColor1(0xFF, 0xF0, 0x78, 0xAA),
	gmColCommandWait(1),
	gmColCommandSetColor1(0xDC, 0x6E, 0x1E, 0x96),
	gmColCommandWait(1),
	gmColCommandReturn()
};

// 0x8012CC5C
u32 dGMColScriptsFighterDamageFireSub2[/* */] =
{
	gmColCommandLoopBegin(2),
	gmColCommandSetColor1(0xB4, 0x64, 0x00, 0x64),
	gmColCommandWait(1),
	gmColCommandSetColor1(0x3C, 0x00, 0x00, 0xD2),
	gmColCommandWait(1),
	gmColCommandLoopEnd(),
	gmColCommandReturn()
};

// 0x8012CC80
u32 dGMColScriptsFighterDamageFireWeak[/* */] =
{
	gmColCommandLoopBegin(4),
	gmColCommandEffect(-1, nEFKindFlameLR, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandSubroutine(dGMColScriptsFighterDamageFireSub1),
	gmColCommandLoopEnd(),
	gmColCommandLoopBegin(4),
	gmColCommandEffect(-1, nEFKindFlameRandom, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandSubroutine(dGMColScriptsFighterDamageFireSub2),
	gmColCommandLoopEnd(),
	gmColCommandEnd()
};

// 0x8012CCC4
u32 dGMColScriptsFighterDamageFireMid[/* */] =
{
	gmColCommandLoopBegin(8),
	gmColCommandEffect(-1, nEFKindFlameLR, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandSubroutine(dGMColScriptsFighterDamageFireSub1),
	gmColCommandLoopEnd(),
	gmColCommandLoopBegin(8),
	gmColCommandEffect(-1, nEFKindFlameRandom, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandSubroutine(dGMColScriptsFighterDamageFireSub2),
	gmColCommandLoopEnd(),
	gmColCommandEnd()
};

// 0x8012CD08
u32 dGMColScriptsFighterDamageFireStrong[/* */] =
{
	gmColCommandLoopBegin(16),
	gmColCommandEffect(-1, nEFKindFlameLR, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandSubroutine(dGMColScriptsFighterDamageFireSub1),
	gmColCommandLoopEnd(),
	gmColCommandLoopBegin(16),
	gmColCommandEffect(-1, nEFKindFlameRandom, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandSubroutine(dGMColScriptsFighterDamageFireSub2),
	gmColCommandLoopEnd(),
	gmColCommandEnd()
};

// 0x8012CD4C
u32 dGMColScriptsFighterDamageFireFly[/* */] =
{
	gmColCommandLoopBegin(24),
	gmColCommandEffect(-1, nEFKindFlameLR, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandSubroutine(dGMColScriptsFighterDamageFireSub1),
	gmColCommandLoopEnd(),
	gmColCommandLoopBegin(20),
	gmColCommandEffect(-1, nEFKindFlameRandom, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandSubroutine(dGMColScriptsFighterDamageFireSub2),
	gmColCommandLoopEnd(),
	gmColCommandEnd()
};

// 0x8012CD90
u32 dGMColScriptsFighterDamageElectricCommonSub[/* */] =
{
	gmColCommandLoopBegin(2),
	gmColCommandSetColor1(0x00, 0x00, 0x94, 0x5A),
	gmColCommandWait(1),
	gmColCommandSetColor1(0xFF, 0xFF, 0xFF, 0x96),
	gmColCommandWait(1),
	gmColCommandLoopEnd(),
	gmColCommandReturn()
};

// 0x8012CDB4
u32 dGMColScriptsFighterDamageElectricCommonWeak[/* */] =
{
	gmColCommandLoopBegin(4),
	gmColCommandEffect(-1, nEFKindShockSmall, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandSubroutine(dGMColScriptsFighterDamageElectricCommonSub),
	gmColCommandLoopEnd(),
	gmColCommandEnd()
};

// 0x8012CDD8
u32 dGMColScriptsFighterDamageElectricCommonMid[/* */] =
{
	gmColCommandLoopBegin(8),
	gmColCommandEffect(-1, nEFKindShockSmall, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandSubroutine(dGMColScriptsFighterDamageElectricCommonSub),
	gmColCommandLoopEnd(),
	gmColCommandEnd()
};

// 0x8012CDFC
u32 dGMColScriptsFighterDamageElectricCommonStrong[/* */] =
{
	gmColCommandLoopBegin(12),
	gmColCommandEffect(-1, nEFKindShockSmall, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandSubroutine(dGMColScriptsFighterDamageElectricCommonSub),
	gmColCommandLoopEnd(),
	gmColCommandEnd()
};

// 0x8012CE20
u32 dGMColScriptsFighterDamageElectricCommonFly[/* */] =
{
	gmColCommandLoopBegin(16),
	gmColCommandEffect(-1, nEFKindShockSmall, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandSubroutine(dGMColScriptsFighterDamageElectricCommonSub),
	gmColCommandLoopEnd(),
	gmColCommandEnd()
};

// 0x8012CE44
u32 dGMColScriptsFighterDamageElectricSkeletonSub[/* */] =
{
	gmColCommandLoopBegin(1),
	gmColCommandSetColor1(0x14, 0x14, 0x14, 0xFF),
	gmColCommandWait(2),
	gmColCommandClearColorAll(),
	gmColCommandSetSkeletonID(1),
	gmColCommandWait(2),
	gmColCommandClearColorAll(),
	gmColCommandWait(1),
	gmColCommandLoopEnd(),
	gmColCommandReturn()
};

// 0x8012CE70
u32 dGMColScriptsFighterDamageElectricSkeletonWeak[/* */] =
{
	gmColCommandLoopBegin(2),
	gmColCommandEffect(-1, nEFKindShockSmall, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandSubroutine(dGMColScriptsFighterDamageElectricSkeletonSub),
	gmColCommandLoopEnd(),
	gmColCommandLoopBegin(2),
	gmColCommandEffect(-1, nEFKindShockSmall, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandSubroutine(dGMColScriptsFighterDamageElectricCommonSub),
	gmColCommandLoopEnd(),
	gmColCommandEnd()
};

// 0x8012CEB4
u32 dGMColScriptsFighterDamageElectricSkeletonMid[/* */] =
{
	gmColCommandLoopBegin(3),
	gmColCommandEffect(-1, nEFKindShockSmall, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandSubroutine(dGMColScriptsFighterDamageElectricSkeletonSub),
	gmColCommandLoopEnd(),
	gmColCommandLoopBegin(5),
	gmColCommandEffect(-1, nEFKindShockSmall, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandSubroutine(dGMColScriptsFighterDamageElectricCommonSub),
	gmColCommandLoopEnd(),
	gmColCommandEnd()
};

// 0x8012CEF8
u32 dGMColScriptsFighterDamageElectricSkeletonStrong[/* */] =
{
	gmColCommandLoopBegin(4),
	gmColCommandEffect(-1, nEFKindShockSmall, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandSubroutine(dGMColScriptsFighterDamageElectricSkeletonSub),
	gmColCommandLoopEnd(),
	gmColCommandLoopBegin(8),
	gmColCommandEffect(-1, nEFKindShockSmall, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandSubroutine(dGMColScriptsFighterDamageElectricCommonSub),
	gmColCommandLoopEnd(),
	gmColCommandEnd()
};

// 0x8012CF3C
u32 dGMColScriptsFighterDamageElectricSkeletonFly[/* */] =
{
	gmColCommandLoopBegin(5),
	gmColCommandEffect(-1, nEFKindShockSmall, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandSubroutine(dGMColScriptsFighterDamageElectricSkeletonSub),
	gmColCommandLoopEnd(),
	gmColCommandLoopBegin(11),
	gmColCommandEffect(-1, nEFKindShockSmall, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandSubroutine(dGMColScriptsFighterDamageElectricCommonSub),
	gmColCommandLoopEnd(),
	gmColCommandEnd()
};

// 0x8012CF80
u32 dGMColScriptsFighterDamageElectricBalloonSub[/* */] =
{
	gmColCommandLoopBegin(1),
	gmColCommandSetColor1(0x14, 0x14, 0x14, 0xFF),
	gmColCommandWait(2),
	gmColCommandClearColorAll(),
	gmColCommandSetSkeletonID(1),
	gmColCommandWait(3),
	gmColCommandClearColorAll(),
	gmColCommandWait(1),
	gmColCommandLoopEnd(),
	gmColCommandReturn()
};

// 0x8012CFAC
u32 dGMColScriptsFighterDamageElectricBalloonWeak[/* */] =
{
	gmColCommandLoopBegin(2),
	gmColCommandEffect(-1, nEFKindShockSmall, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandSubroutine(dGMColScriptsFighterDamageElectricBalloonSub),
	gmColCommandLoopEnd(),
	gmColCommandLoopBegin(2),
	gmColCommandEffect(-1, nEFKindShockSmall, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandSubroutine(dGMColScriptsFighterDamageElectricCommonSub),
	gmColCommandLoopEnd(),
	gmColCommandEnd()
};

// 0x8012CFF0
u32 dGMColScriptsFighterDamageElectricBalloonMid[/* */] =
{
	gmColCommandLoopBegin(3),
	gmColCommandEffect(-1, nEFKindShockSmall, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandSubroutine(dGMColScriptsFighterDamageElectricBalloonSub),
	gmColCommandLoopEnd(),
	gmColCommandLoopBegin(5),
	gmColCommandEffect(-1, nEFKindShockSmall, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandSubroutine(dGMColScriptsFighterDamageElectricCommonSub),
	gmColCommandLoopEnd(),
	gmColCommandEnd()
};

// 0x8012D034
u32 dGMColScriptsFighterDamageElectricBalloonStrong[/* */] =
{
	gmColCommandLoopBegin(4),
	gmColCommandEffect(-1, nEFKindShockSmall, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandSubroutine(dGMColScriptsFighterDamageElectricBalloonSub),
	gmColCommandLoopEnd(),
	gmColCommandLoopBegin(8),
	gmColCommandEffect(-1, nEFKindShockSmall, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandSubroutine(dGMColScriptsFighterDamageElectricCommonSub),
	gmColCommandLoopEnd(),
	gmColCommandEnd()
};

// 0x8012D078
u32 dGMColScriptsFighterDamageElectricBalloonFly[/* */] =
{
	gmColCommandLoopBegin(5),
	gmColCommandEffect(-1, nEFKindShockSmall, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandSubroutine(dGMColScriptsFighterDamageElectricBalloonSub),
	gmColCommandLoopEnd(),
	gmColCommandLoopBegin(11),
	gmColCommandEffect(-1, nEFKindShockSmall, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandSubroutine(dGMColScriptsFighterDamageElectricCommonSub),
	gmColCommandLoopEnd(),
	gmColCommandEnd()
};

// 0x8012D0BC
u32 dGMColScriptsFighterDamageElectricSamusSub[/* */] =
{
	gmColCommandLoopBegin(1),
	gmColCommandSetColor1(0x14, 0x14, 0x14, 0xFF),
	gmColCommandSetSkeletonID(2),
	gmColCommandWait(2),
	gmColCommandClearColorAll(),
	gmColCommandSetSkeletonID(1),
	gmColCommandWait(2),
	gmColCommandClearColorAll(),
	gmColCommandWait(1),
	gmColCommandLoopEnd(),
	gmColCommandReturn()
};

// 0x8012D0EC
u32 dGMColScriptsFighterDamageElectricSamusWeak[/* */] =
{
	gmColCommandLoopBegin(2),
	gmColCommandEffect(-1, nEFKindShockSmall, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandSubroutine(dGMColScriptsFighterDamageElectricSamusSub),
	gmColCommandLoopEnd(),
	gmColCommandLoopBegin(2),
	gmColCommandEffect(-1, nEFKindShockSmall, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandSubroutine(dGMColScriptsFighterDamageElectricCommonSub),
	gmColCommandLoopEnd(),
	gmColCommandEnd()
};

// 0x8012D130
u32 dGMColScriptsFighterDamageElectricSamusMid[/* */] =
{
	gmColCommandLoopBegin(3),
	gmColCommandEffect(-1, nEFKindShockSmall, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandSubroutine(dGMColScriptsFighterDamageElectricSamusSub),
	gmColCommandLoopEnd(),
	gmColCommandLoopBegin(5),
	gmColCommandEffect(-1, nEFKindShockSmall, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandSubroutine(dGMColScriptsFighterDamageElectricCommonSub),
	gmColCommandLoopEnd(),
	gmColCommandEnd()
};

// 0x8012D174
u32 dGMColScriptsFighterDamageElectricSamusStrong[/* */] =
{
	gmColCommandLoopBegin(4),
	gmColCommandEffect(-1, nEFKindShockSmall, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandSubroutine(dGMColScriptsFighterDamageElectricSamusSub),
	gmColCommandLoopEnd(),
	gmColCommandLoopBegin(8),
	gmColCommandEffect(-1, nEFKindShockSmall, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandSubroutine(dGMColScriptsFighterDamageElectricCommonSub),
	gmColCommandLoopEnd(),
	gmColCommandEnd()
};

// 0x8012D1B8
u32 dGMColScriptsFighterDamageElectricSamusFly[/* */] =
{
	gmColCommandLoopBegin(5),
	gmColCommandEffect(-1, nEFKindShockSmall, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandSubroutine(dGMColScriptsFighterDamageElectricSamusSub),
	gmColCommandLoopEnd(),
	gmColCommandLoopBegin(11),
	gmColCommandEffect(-1, nEFKindShockSmall, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandSubroutine(dGMColScriptsFighterDamageElectricCommonSub),
	gmColCommandLoopEnd(),
	gmColCommandEnd()
};

// 0x8012D1FC
u32 dGMColScriptsFighterDamageIceWeak[/* */] =
{
	gmColCommandSetColor1(0x00, 0x00, 0xFF, 0x80),
	gmColCommandWait(1),
	gmColCommandSetColor1(0xFF, 0xFF, 0xFF, 0x80),
	gmColCommandWait(1),
	gmColCommandEnd()
};

// 0x8012D1FC
u32 dGMColScriptsFighterDamageIceMid[/* */] =
{
	gmColCommandEnd()
};

// 0x8012D1FC
u32 dGMColScriptsFighterDamageIceStrong[/* */] =
{
	gmColCommandEnd()
};

// 0x8012D1FC
u32 dGMColScriptsFighterDamageIceFly[/* */] =
{
	gmColCommandEnd()
};

// 0x8012D224
u32 dGMColScriptsFighterShieldBreakFly[/* */] =
{
	gmColCommandSetColor1(0x80, 0x00, 0x00, 0x80),
	gmColCommandWait(2),
	gmColCommandBlendColor1(10, 0x80, 0x00, 0x00, 0x14),
	gmColCommandWait(10),
	gmColCommandClearColorAll(),
	gmColCommandWait(2),
	gmColCommandGoto(dGMColScriptsFighterShieldBreakFly)
};

// 0x8012D24C
u32 dGMColScriptsFighterFuraFura[/* */] =
{
	gmColCommandSetColor1(0x80, 0x00, 0x00, 0x80),
	gmColCommandWait(2),
	gmColCommandBlendColor1(12, 0x80, 0x00, 0x00, 0x14),
	gmColCommandWait(12),
	gmColCommandBlendColor1(6, 0x80, 0x00, 0x00, 0x80),
	gmColCommandWait(6),
	gmColCommandGoto(dGMColScriptsFighterFuraFura)
};

// 0x8012D278
u32 dGMColScriptsFighterFuraSleep[/* */] =
{
	gmColCommandSetColor1(0x80, 0x50, 0x64, 0x46),
	gmColCommandBlendColor1(12, 0xF0, 0x3C, 0x8C, 0x82),
	gmColCommandWait(12),
	gmColCommandBlendColor1(12, 0xF0, 0x3C, 0x8C, 0x3C),
	gmColCommandWait(12),
	gmColCommandClearColorAll(),
	gmColCommandWait(6),
	gmColCommandGoto(dGMColScriptsFighterFuraSleep)
};

// 0x8012D2A8
u32 dGMColScriptsFighterMarioSpecialN[/* */] =
{
	gmColCommandSetLight(75, -10),
	gmColCommandSetColor1(0xFF, 0x00, 0x00, 0xFF),
	gmColCommandWait(1),
	gmColCommandSetColor1(0xFF, 0x00, 0x00, 0x80),
	gmColCommandWait(1),
	gmColCommandClearColorAll(),
	gmColCommandWait(20),
	gmColCommandClearLight(),
	gmColCommandEnd()
};

// 0x8012D2D4
u32 dGMColScriptsFighterMarioAppeal[/* */] =
{
	gmColCommandSetColor1(0xFF, 0xFF, 0x80, 0xAA),
	gmColCommandWait(1),
	gmColCommandBlendColor1(3, 0x50, 0x50, 0x50, 0x5A),
	gmColCommandWait(3),
	gmColCommandBlendColor1(3, 0x50, 0x50, 0x50, 0x00),
	gmColCommandWait(3),
	gmColCommandBlendColor1(4, 0xFF, 0xFF, 0x80, 0xAA),
	gmColCommandWait(3),
	gmColCommandGoto(dGMColScriptsFighterMarioAppeal),
	gmColCommandEnd()
};

// 0x8012D310
u32 dGMColScriptsFighterDonkeySpecialNLoop[/* */] =
{
	gmColCommandSetColor1(0xFF, 0xFF, 0x80, 0x46),
	gmColCommandWait(2),
	gmColCommandSetColor1(0xFF, 0xFF, 0x80, 0x28),
	gmColCommandWait(2),
	gmColCommandSetColor1(0x3C, 0x3C, 0x00, 0x64),
	gmColCommandWait(2),
	gmColCommandEnd()
};

// 0x8012D338
u32 dGMColScriptsFighterDonkeySpecialNEnd[/* */] =
{
	gmColCommandSetColor1(0xFF, 0xFF, 0x80, 0xB4),
	gmColCommandWait(1),
	gmColCommandBlendColor1(5, 0xFF, 0xFF, 0x80, 0x00),
	gmColCommandWait(5),
	gmColCommandClearColorAll(),
	gmColCommandWait(2),
	gmColCommandEnd()
};

// 0x8012D35C
u32 dGMColScriptsFighterUnknown1[/* */] =
{
	gmColCommandEnd()
};

// 0x8012D360
u32 dGMColScriptsFighterSamusSpecialNEnd[/* */] =
{
	gmColCommandSetLight(75, 0),
	gmColCommandSetColor1(0xFF, 0xFF, 0xFF, 0x80),
	gmColCommandWait(1),
	gmColCommandClearColorAll(),
	gmColCommandWait(8),
	gmColCommandClearLight(),
	gmColCommandEnd()
};

// 0x8012D380
u32 dGMColScriptsFighterSamusSpecialHi[/* */] =
{
	gmColCommandSetColor1(0x80, 0xFF, 0xAA, 0xBE),
	gmColCommandWait(1),
	gmColCommandSetColor1(0x96, 0xFF, 0xC8, 0x8C),
	gmColCommandWait(1),
	gmColCommandSetColor1(0xFF, 0xFF, 0xFF, 0x64),
	gmColCommandWait(2),
	gmColCommandClearColorAll(),
	gmColCommandWait(1),
	gmColCommandGoto(dGMColScriptsFighterSamusSpecialHi),
	gmColCommandEnd()
};

// 0x8012D3B8
u32 dGMColScriptsFighterFoxSpecialLw[/* */] =
{
	gmColCommandSetLight(0, -80),
	gmColCommandSetColor1(0x00, 0xFF, 0xFF, 0x50),
	gmColCommandWait(1),
	gmColCommandClearColorAll(),
	gmColCommandWait(1),
	gmColCommandGoto(dGMColScriptsFighterFoxSpecialLw)
};

// 0x8012D3D8
u32 dGMColScriptsFighterFoxSpecialHiStart[/* */] =
{
	gmColCommandSetLight(0, -80),
	gmColCommandLoopBegin(4),
	gmColCommandSetColor1(0xFF, 0xFF, 0xFF, 0x1E),
	gmColCommandWait(1),
	gmColCommandSetColor1(0xFF, 0xFF, 0x00, 0x1E),
	gmColCommandWait(1),
	gmColCommandClearColorAll(),
	gmColCommandWait(1),
	gmColCommandLoopEnd(),
	gmColCommandLoopBegin(3),
	gmColCommandSetColor1(0xFF, 0xFF, 0xFF, 0x64),
	gmColCommandWait(1),
	gmColCommandSetColor1(0xFF, 0xFF, 0x00, 0x64),
	gmColCommandWait(1),
	gmColCommandClearColorAll(),
	gmColCommandWait(1),
	gmColCommandLoopEnd()
};

// 0x8012D42C - part of FoxSpecialHiStart, as that script does not have an End command
u32 dGMColScriptsFighterFoxSpecialHiHold[/* */] =
{
	gmColCommandSetColor1(0xFF, 0xFF, 0xFF, 0xB4),
	gmColCommandWait(1),
	gmColCommandSetColor1(0xFF, 0xFF, 0x00, 0xB4),
	gmColCommandWait(1),
	gmColCommandClearColorAll(),
	gmColCommandWait(1),
	gmColCommandGoto(dGMColScriptsFighterFoxSpecialHiHold)
};

// 0x8012D454
u32 dGMColScriptsFighterFoxSpecialHi[/* */] =
{
	gmColCommandSetColor1(0xFF, 0xFF, 0x00, 0xDC),
	gmColCommandWait(1),
	gmColCommandBlendColor1(30, 0xA0, 0x3C, 0x3C, 0x8C),
	gmColCommandWait(30),
	gmColCommandBlendColor1(20, 0x80, 0x00, 0x00, 0x00),
	gmColCommandWait(20),
	gmColCommandClearColorAll(),
	gmColCommandEnd()
};

// 0x8012D480
u32 dGMColScriptsFighterLinkSpecialHi[/* */] =
{
	gmColCommandSetLight(0, -80),
	gmColCommandSetColor1(0xFF, 0xAA, 0x00, 0x46),
	gmColCommandWait(4),
	gmColCommandSetColor1(0xD2, 0x8C, 0x00, 0x46),
	gmColCommandWait(4),
	gmColCommandEffect(0, nEFKindDustHeavyDouble, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandGoto(dGMColScriptsFighterLinkSpecialHi)
};

// 0x8012D4B4
u32 dGMColScriptsFighterCaptainSpecialN[/* */] =
{
	gmColCommandSetLight(0, -80),
	gmColCommandLoopBegin(20),
	gmColCommandSetColor1(0xFF, 0xF0, 0x78, 0x78),
	gmColCommandWait(1),
	gmColCommandSetColor1(0xDC, 0x6E, 0x1E, 0x50),
	gmColCommandWait(1),
	gmColCommandLoopEnd(),
	gmColCommandClearColorAll(),
	gmColCommandSetLight(75, -10),
	gmColCommandLoopBegin(6),
	gmColCommandEffect(-1, nEFKindFlameLR, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandWait(4),
	gmColCommandLoopEnd(),
	gmColCommandClearLight(),
	gmColCommandEnd()
};

// 0x8012D504
u32 dGMColScriptsFighterKirbySpecialLwHigh[/* */] =
{
	gmColCommandSetColor1(0xFF, 0xFF, 0xFF, 0x28),
	gmColCommandBlendColor1(8, 0xFF, 0xFF, 0xFF, 0x00),
	gmColCommandWait(8),
	gmColCommandBlendColor1(8, 0xFF, 0xFF, 0xFF, 0x28),
	gmColCommandWait(8),
	gmColCommandGoto(dGMColScriptsFighterKirbySpecialLwHigh)
};

// 0x8012D52C
u32 dGMColScriptsFighterKirbySpecialLwMid[/* */] =
{
	gmColCommandSetColor1(0xFF, 0xFF, 0xFF, 0x3C),
	gmColCommandBlendColor1(6, 0xFF, 0xFF, 0xFF, 0x0A),
	gmColCommandWait(6),
	gmColCommandBlendColor1(6, 0xFF, 0xFF, 0xFF, 0x3C),
	gmColCommandWait(6),
	gmColCommandGoto(dGMColScriptsFighterKirbySpecialLwMid)
};

// 0x8012D554
u32 dGMColScriptsFighterKirbySpecialLwLow[/* */] =
{
	gmColCommandSetColor1(0xFF, 0xFF, 0xFF, 0x5A),
	gmColCommandBlendColor1(4, 0xFF, 0xFF, 0xFF, 0x0A),
	gmColCommandWait(4),
	gmColCommandBlendColor1(4, 0xFF, 0xFF, 0xFF, 0x5A),
	gmColCommandWait(4),
	gmColCommandGoto(dGMColScriptsFighterKirbySpecialLwLow)
};

// 0x8012D57C
u32 dGMColScriptsFighterKirbySpecialLwStart[/* */] =
{
	gmColCommandWait(2),
	gmColCommandSetColor1(0xB4, 0x80, 0x64, 0x0A),
	gmColCommandWait(5),
	gmColCommandSetColor1(0x50, 0x3C, 0x28, 0x14),
	gmColCommandWait(2),
	gmColCommandSetColor1(0xB4, 0x80, 0x64, 0x28),
	gmColCommandWait(5),
	gmColCommandSetColor1(0x50, 0x3C, 0x28, 0x3C),
	gmColCommandWait(2),
	gmColCommandSetColor1(0xB4, 0x80, 0x64, 0x50),
	gmColCommandWait(5),
	gmColCommandSetColor1(0x50, 0x3C, 0x28, 0x64),
	gmColCommandWait(2),
	gmColCommandSetColor1(0xB4, 0x80, 0x64, 0x78),
	gmColCommandWait(5),
	gmColCommandSetColor1(0x50, 0x3C, 0x28, 0x8C),
	gmColCommandWait(2),
	gmColCommandSetColor1(0xB4, 0x80, 0x64, 0xA0),
	gmColCommandWait(5),
	gmColCommandSetColor1(0x50, 0x3C, 0x28, 0xB4),
	gmColCommandEnd()
};

// 0x8012D5F8
u32 dGMColScriptsFighterKirbySpecialLwEnd[/* */] =
{
	gmColCommandWait(2),
	gmColCommandSetColor1(0xB4, 0x80, 0x64, 0xB4),
	gmColCommandWait(5),
	gmColCommandSetColor1(0x50, 0x3C, 0x28, 0xA0),
	gmColCommandWait(2),
	gmColCommandSetColor1(0xB4, 0x80, 0x64, 0x8C),
	gmColCommandWait(5),
	gmColCommandSetColor1(0x50, 0x3C, 0x28, 0x78),
	gmColCommandWait(2),
	gmColCommandSetColor1(0xB4, 0x80, 0x64, 0x64),
	gmColCommandWait(5),
	gmColCommandSetColor1(0x50, 0x3C, 0x28, 0x50),
	gmColCommandWait(2),
	gmColCommandSetColor1(0xB4, 0x80, 0x64, 0x3C),
	gmColCommandWait(5),
	gmColCommandSetColor1(0x50, 0x3C, 0x28, 0x28),
	gmColCommandWait(2),
	gmColCommandSetColor1(0xB4, 0x80, 0x64, 0x14),
	gmColCommandWait(5),
	gmColCommandSetColor1(0x50, 0x3C, 0x28, 0x0A),
	gmColCommandEnd()
};

// 0x8012D674
u32 dGMColScriptsFighterPikachuAttackS4[/* */] =
{
	gmColCommandLoopBegin(4),
	gmColCommandSetColor1(0xFF, 0xFF, 0xFF, 0x64),
	gmColCommandWait(1),
	gmColCommandBlendColor1(6, 0x00, 0x00, 0xFF, 0x00),
	gmColCommandWait(6),
	gmColCommandEffect(-1, nEFKindShockSmall, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandLoopEnd(),
	gmColCommandGoto(dGMColScriptsFighterPikachuAttackS4)
};

// 0x8012D6AC
u32 dGMColScriptsFighterPikachuSpecialHiStart[/* */] =
{
	gmColCommandPlayFGM(nSYAudioFGMPikachuSpecialHiStart)
};

// 0x8012D6B0 - part of PikachuSpecialHiStart, as that script does not have an End command
u32 dGMColScriptsFighterPikachuSpecialHiStartLoop[/* */] =
{
	gmColCommandSetColor1(0xFF, 0xFF, 0xFF, 0x40),
	gmColCommandWait(1),
	gmColCommandSetColor1(0x00, 0xFF, 0xFF, 0x40),
	gmColCommandWait(1),
	gmColCommandGoto(dGMColScriptsFighterPikachuSpecialHiStartLoop)
};

#if defined(REGION_US)
// 0x8012D6D0
u32 dGMColScriptsFighterPikachuSpecialHi[/* */] =
{
	gmColCommandEffect(-1, nEFKindShockSmall, 0, 0, 0, 0, 150, 150, 150),
	gmColCommandWait(2),
	gmColCommandGoto(dGMColScriptsFighterPikachuSpecialHi)
};
#endif

// 0x8012D6EC
u32 dGMColScriptsFighterPikachuSpecialN[/* */] =
{
	gmColCommandLoopBegin(4),
	gmColCommandLoopBegin(3),
	gmColCommandSetColor1(0x00, 0x00, 0xFF, 0x32),
	gmColCommandWait(1),
	gmColCommandSetColor1(0xFF, 0xFF, 0xFF, 0x5A),
	gmColCommandWait(1),
	gmColCommandClearColorAll(),
	gmColCommandWait(1),
	gmColCommandLoopEnd(),
	gmColCommandEffect(-1, nEFKindShockSmall, 0, 0, 0, 0, 0, 0, 0),
	gmColCommandLoopEnd(),
	gmColCommandGoto(dGMColScriptsFighterPikachuSpecialN)
};

// 0x8012D734
u32 dGMColScriptsFighterPikachuSpecialLwHit[/* */] =
{
	gmColCommandLoopBegin(4),
	gmColCommandLoopBegin(2),
	gmColCommandSetColor1(0x00, 0x00, 0xFF, 0x5A),
	gmColCommandWait(1),
	gmColCommandSetColor1(0xFF, 0xFF, 0xFF, 0x64),
	gmColCommandWait(1),
	gmColCommandClearColorAll(),
	gmColCommandWait(1),
	gmColCommandLoopEnd(),
	gmColCommandEffect(-1, nEFKindShockSmall, 0, 0, 0, 0, 100, 100, 100),
	gmColCommandEffect(0, nEFKindSparkleWhiteScale, 0, 0, 150, 0, 400, 400, 400),
	gmColCommandLoopEnd(),
	gmColCommandGoto(dGMColScriptsFighterPikachuSpecialLwHit)
};

// 0x8012D78C
u32 dGMColScriptsFighterPikachuSpecialLwEnd[/* */] =
{
	gmColCommandSetColor1(0xFF, 0xFF, 0xFF, 0x50),
	gmColCommandWait(1),
	gmColCommandSetColor1(0x00, 0x00, 0x00, 0x50),
	gmColCommandWait(1),
	gmColCommandClearColorAll(),
	gmColCommandWait(2),
	gmColCommandGoto(dGMColScriptsFighterPikachuSpecialLwEnd)
};

// 0x8012D7B4
u32 dGMColScriptsFighterCaptainSpecialHi[/* */] =
{
	gmColCommandLoopBegin(11),
	gmColCommandSetColor1(0xFF, 0xFF, 0x64, 0x64),
	gmColCommandWait(1),
	gmColCommandSetColor1(0xFF, 0x64, 0x00, 0x5A),
	gmColCommandWait(1),
	gmColCommandClearColorAll(),
	gmColCommandWait(1),
	gmColCommandLoopEnd(),
	gmColCommandEnd()
};

// 0x8012D7E0
u32 dGMColScriptsFighterNessSpecialLwHold[/* */] =
{
	gmColCommandSetColor1(0xFF, 0xFF, 0xFF, 0x8C),
	gmColCommandWait(1),
	gmColCommandSetColor1(0x00, 0xFF, 0xFF, 0x8C),
	gmColCommandWait(1),
	gmColCommandGoto(dGMColScriptsFighterNessSpecialLwHold)
};

// 0x8012D800
u32 dGMColScriptsFighterNessSpecialLwHit[/* */] =
{
	gmColCommandSetLight(90, 0),
	gmColCommandSetColor1(0xFF, 0xFF, 0xFF, 0xFF),
	gmColCommandWait(1),
	gmColCommandSetColor1(0x00, 0xFF, 0xFF, 0xFF),
	gmColCommandWait(1),
	gmColCommandSetColor1(0x00, 0xFF, 0xFF, 0x8C),
	gmColCommandWait(1),
	gmColCommandClearColorAll(),
	gmColCommandWait(4),
	gmColCommandEnd()
};

// 0x8012D834
u32 dGMColScriptsFighterNessSpecialHiHold[/* */] =
{
	gmColCommandLoopBegin(2),
	gmColCommandSetColor1(0xFF, 0xFF, 0xFF, 0x30),
	gmColCommandWait(2),
	gmColCommandSetColor1(0x00, 0xFF, 0xFF, 0x30),
	gmColCommandWait(2),
	gmColCommandLoopEnd(),
	gmColCommandEffect(0, nEFKindShockSmall, 0, 0, 90, 0, 0, 0, 0),
	gmColCommandGoto(dGMColScriptsFighterNessSpecialHiHold)
};

// 0x8012D86C
u32 dGMColScriptsFighterNessSpecialHiJibaku[/* */] =
{
	gmColCommandSetColor1(0x7F, 0x7F, 0x7F, 0x50),
	gmColCommandWait(2),
	gmColCommandEffect(0, nEFKindSparkleWhiteScale, 0, 0, 90, 0, 180, 180, 180),
	gmColCommandEffect(0, nEFKindShockSmall, 0, 0, 90, 0, 0, 0, 0),
	gmColCommandSetColor1(0x00, 0xFF, 0x00, 0x50),
	gmColCommandWait(2),
	gmColCommandEffect(0, nEFKindSparkleWhiteScale, 0, 0, 90, 0, 180, 180, 180),
	gmColCommandEffect(0, nEFKindShockSmall, 0, 0, 90, 0, 0, 0, 0),
	gmColCommandGoto(dGMColScriptsFighterNessSpecialHiJibaku)
};

// 0x8012D8CC
u32 dGMColScriptsFighterNessAppear[/* */] =
{
	gmColCommandSetColor1(0x00, 0x00, 0x00, 0xFF),
	gmColCommandWait(30),
	gmColCommandBlendColor1(40, 0x00, 0x00, 0x00, 0x00),
	gmColCommandWait(40),
	gmColCommandClearColorAll(),
	gmColCommandEnd()
};

// 0x8012D8EC
u32 dGMColScriptsFighterBossDeskArrange[/* */] =
{
	gmColCommandSetColor1(0x00, 0x00, 0x00, 0x96),
	gmColCommandWait(1),
	gmColCommandGoto(dGMColScriptsFighterBossDeskArrange),
	gmColCommandEnd()
};

// 0x8012D904
u32 dGMColScriptsFighterBossOkuhikouki[/* */] =
{
	gmColCommandEffect(0, nEFKindSparkleWhite, 0, 0, 0, 0, 180, 180, 180),
	gmColCommandWait(2),
	gmColCommandGoto(dGMColScriptsFighterBossOkuhikouki)
};

// 0x8012D920
u32 dGMColScriptsFighterBossOkupunch[/* */] =
{
	gmColCommandEffect(0, nEFKindDustExpandLarge, 0, 0, 400, 0, 0, 0, 0),
	gmColCommandWait(1),
	gmColCommandGoto(dGMColScriptsFighterBossOkupunch),
	gmColCommandEnd()
};

// 0x8012D940
u32 dGMColScriptsFighterBossYubideppou2[/* */] =
{
	gmColCommandSetLight(90, 0),
	gmColCommandSetColor1(0x5A, 0x00, 0x00, 0x14),
	gmColCommandBlendColor1(60, 0x5A, 0x00, 0x00, 0xA0),
	gmColCommandWait(60),
	gmColCommandSetColor1(0x5A, 0x00, 0x00, 0xA0),
	gmColCommandWait(500),
	gmColCommandEnd()
};

// 0x8012D968
u32 dGMColScriptsFighterBossYubideppou3[/* */] =
{
	gmColCommandSetLight(90, 0),
	gmColCommandSetColor1(0xFF, 0xFF, 0xFF, 0x28),
	gmColCommandSetColor2(0xFF, 0xFF, 0xFF, 0xFF),
	gmColCommandWait(2),
	gmColCommandSetColor1(0x5A, 0x00, 0x00, 0xA0),
	gmColCommandBlendColor1(40, 0x5A, 0x00, 0x00, 0x0A),
	gmColCommandWait(40),
	gmColCommandClearColorAll(),
	gmColCommandWait(500),
	gmColCommandEnd()
};

// 0x8012D9A0
u32 dGMColScriptsFighterChallenger[/* */] =
{
	gmColCommandSetColor1(0x00, 0x00, 0x00, 0xFF),
	gmColCommandWait(1),
	gmColCommandGoto(dGMColScriptsFighterChallenger),
	gmColCommandEnd()
};

// 0x8012D9B8 - Hammer
u32 dGMColScriptsFighterHammer[/* */] =
{
	gmColCommandSetColor1(0xFF, 0x00, 0x00, 0x80),
	gmColCommandWait(2),
	gmColCommandSetColor1(0x80, 0xFF, 0x00, 0x64),
	gmColCommandWait(2),
	gmColCommandClearColorAll(),
	gmColCommandWait(2),
	gmColCommandGoto(dGMColScriptsFighterHammer)
};

// 0x8012D9E0 - Star
u32 dGMColScriptsFighterStar[/* */] =
{
	gmColCommandSetColor1(0xFF, 0xFF, 0xFF, 0x64),
	gmColCommandWait(1),
	gmColCommandEffect(0, nEFKindPsionic, 0, 0, 100, 0, 180, 180, 180),
	gmColCommandEffect(0, nEFKindHealSparkles, 0, 0, 0, 0, 300, 300, 300),
	gmColCommandBlendColor1(3, 0xFF, 0xC8, 0xA0, 0x8C),
	gmColCommandWait(3),
	gmColCommandEffect(0, nEFKindHealSparkles, 0, 0, 0, 0, 300, 300, 300),
	gmColCommandBlendColor1(3, 0x64, 0xC8, 0x64, 0x6E),
	gmColCommandWait(3),
	gmColCommandEffect(0, nEFKindHealSparkles, 0, 0, 0, 0, 300, 300, 300),
	gmColCommandBlendColor1(3, 0x00, 0x00, 0x28, 0x8C),
	gmColCommandWait(3),
	gmColCommandEffect(0, nEFKindHealSparkles, 0, 0, 0, 0, 300, 300, 300),
	gmColCommandClearColorAll(),
	gmColCommandWait(3),
	gmColCommandGoto(dGMColScriptsFighterStar)
};

// 0x8012DA70 - Star Rod
u32 dGMColScriptsFighterStarRod[/* */] =
{
	gmColCommandEffectItemHold(-2, nEFKindStarRodSpark, 0, 0, 250, 0, 100, 100, 100),
	gmColCommandWait(2),
	gmColCommandGoto(dGMColScriptsFighterStarRod)
};

// 0x8012DA8C - Home Run Bat
u32 dGMColScriptsFighterBat[/* */] =
{
	gmColCommandSetLight(75, -10),
	gmColCommandEffectItemHold(-2, nEFKindSparkleWhiteScale, 0, 0, 200, 0, 90, 90, 90),
	gmColCommandSetColor1(0xFF, 0xFF, 0x00, 0x78),
	gmColCommandWait(1),
	gmColCommandSetColor1(0x80, 0x00, 0x00, 0x78),
	gmColCommandWait(1),
	gmColCommandClearColorAll(),
	gmColCommandWait(2),
	gmColCommandGoto(dGMColScriptsFighterBat)
};

// 0x8012DAC8
u32 dGMColScriptsItemBombHeiCritical[/* */] =
{
	gmColCommandSetColor1(0xFF, 0xFF, 0x00, 0x8C),
	gmColCommandBlendColor1(8, 0x80, 0x00, 0x00, 0x8C),
	gmColCommandWait(8),
	gmColCommandSetColor1(0x80, 0x00, 0x00, 0x8C),
	gmColCommandBlendColor1(8, 0xFF, 0xFF, 0x00, 0x8C),
	gmColCommandWait(8),
	gmColCommandGoto(dGMColScriptsItemBombHeiCritical)
};

// 0x8012DAF8
u32 dGMColScriptsItemHammerEnd[/* */] =
{
	gmColCommandSetColor1(0xFF, 0xFF, 0x00, 0x8C),
	gmColCommandWait(1),
	gmColCommandSetColor1(0x80, 0x00, 0x00, 0x8C),
	gmColCommandWait(1),
	gmColCommandGoto(dGMColScriptsItemHammerEnd)
};

// 0x8012DB18
u32 dGMColScriptsItemLinkBombCritical[/* */] =
{
	gmColCommandSetColor1(0xFF, 0xFF, 0x00, 0x8C),
	gmColCommandBlendColor1(4, 0x80, 0x00, 0x00, 0x8C),
	gmColCommandWait(4),
	gmColCommandSetColor1(0x80, 0x00, 0x00, 0x8C),
	gmColCommandBlendColor1(12, 0xFF, 0xFF, 0x00, 0x8C),
	gmColCommandWait(12),
	gmColCommandGoto(dGMColScriptsItemLinkBombCritical)
};

// 0x8012DB48
u32 dGMColScriptsScreenFlashDeadExplode[/* */] =
{
	gmColCommandSetColor1(0xFF, 0xFF, 0xFF, 0x00),
	gmColCommandBlendColor1(6, 0xFF, 0xFF, 0xFF, 0x6E),
	gmColCommandWait(6),
	gmColCommandBlendColor1(30, 0xFF, 0xFF, 0xFF, 0x00),
	gmColCommandWait(30),
	gmColCommandClearColorAll(),
	gmColCommandEnd()
};

// 0x8012DB70
u32 dGMColScriptsScreenFlashDamageNormal[/* */] =
{
	gmColCommandSetColor1(0xFF, 0xFF, 0xFF, 0x46),
	gmColCommandBlendColor1(6, 0xFF, 0xFF, 0xFF, 0x00),
	gmColCommandWait(6),
	gmColCommandEnd()
};

// 0x8012DB88
u32 dGMColScriptsScreenFlashDamageFire[/* */] =
{
	gmColCommandSetColor1(0xFF, 0x8C, 0x78, 0x50),
	gmColCommandBlendColor1(8, 0xFF, 0x8C, 0x78, 0x00),
	gmColCommandWait(8),
	gmColCommandEnd()
};

// 0x8012DBA0
u32 dGMColScriptsScreenFlashDamageElectric[/* */] =
{
	gmColCommandSetColor1(0x8C, 0x8C, 0xFF, 0x50),
	gmColCommandBlendColor1(8, 0x8C, 0x8C, 0xFF, 0x00),
	gmColCommandWait(8),
	gmColCommandEnd()
};

// 0x8012DBB8
u32 dGMColScriptsScreenFlashDamageIce[/* */] =
{
	gmColCommandSetColor1(0x00, 0x80, 0xFF, 0x8C),
	gmColCommandBlendColor1(6, 0x00, 0x80, 0xFF, 0x00),
	gmColCommandWait(6),
	gmColCommandEnd()
};

// 0x8012DBD0
GMColDesc dGMColScriptsDescs[/* */] =
{
	{ NULL, 			    					 	  	  0, FALSE },
	{ dGMColScriptsFighterComPlayer,    		 	  	  1, FALSE },
	{ dGMColScriptsFighterHitStatusNormal,   		 	 30, FALSE },
	{ dGMColScriptsFighterHitStatusIntangible,  	 	 30, FALSE },
	{ dGMColScriptsFighterHitStatusInvincible,  	 	 30, FALSE },
	{ dGMColScriptsFighterDamageCommon,  	   			100, FALSE },
	{ dGMColScriptsFighterCommonSpecialNCharge, 	 	 10, FALSE },
	{ dGMColScriptsFighterFallSpecial,   			 	 60,  TRUE },
	{ dGMColScriptsFighterFastFall,   				 	 60,  TRUE },
	{ dGMColScriptsFighterHeal,   					 	 15, FALSE },
	{ dGMColScriptsFighterNoDamage,  				 	 11, FALSE },
	{ dGMColScriptsFighterRebirth,  				 	 10,  TRUE },
	{ dGMColScriptsFighterDamageFireWeak, 	   			100, FALSE },
	{ dGMColScriptsFighterDamageFireMid, 	   			100, FALSE },
	{ dGMColScriptsFighterDamageFireStrong, 	   		100, FALSE },
	{ dGMColScriptsFighterDamageFireFly, 	   			100, FALSE },
	{ dGMColScriptsFighterDamageElectricCommonWeak, 	100, FALSE },
	{ dGMColScriptsFighterDamageElectricCommonMid,  	100, FALSE },
	{ dGMColScriptsFighterDamageElectricCommonStrong, 	100, FALSE },
	{ dGMColScriptsFighterDamageElectricCommonFly,  	100, FALSE },
	{ dGMColScriptsFighterDamageElectricSkeletonWeak,	100, FALSE },
	{ dGMColScriptsFighterDamageElectricSkeletonMid,    100, FALSE },
	{ dGMColScriptsFighterDamageElectricSkeletonStrong,	100, FALSE },
	{ dGMColScriptsFighterDamageElectricSkeletonFly,    100, FALSE },
	{ dGMColScriptsFighterDamageElectricSamusWeak,		100, FALSE },
	{ dGMColScriptsFighterDamageElectricSamusMid,		100, FALSE },
	{ dGMColScriptsFighterDamageElectricSamusStrong,	100, FALSE },
	{ dGMColScriptsFighterDamageElectricSamusFly,		100, FALSE },
	{ dGMColScriptsFighterDamageElectricBalloonWeak,    100, FALSE },
	{ dGMColScriptsFighterDamageElectricBalloonMid,		100, FALSE },
	{ dGMColScriptsFighterDamageElectricBalloonStrong,	100, FALSE },
	{ dGMColScriptsFighterDamageElectricBalloonFly,		100, FALSE },
	{ dGMColScriptsFighterDamageIceWeak, 	   			100, FALSE },
	{ dGMColScriptsFighterDamageIceMid, 	   			100, FALSE },
	{ dGMColScriptsFighterDamageIceStrong, 	   			100, FALSE },
	{ dGMColScriptsFighterDamageIceFly, 	   			100, FALSE },
	{ dGMColScriptsFighterShieldBreakFly,  	    		 60,  TRUE },
	{ dGMColScriptsFighterFuraFura,  		    		 60,  TRUE },
	{ dGMColScriptsFighterFuraSleep,  		    		 60,  TRUE },
	{ dGMColScriptsFighterMarioSpecialN,  	    		 60,  TRUE },
	{ dGMColScriptsFighterMarioAppeal,  	    		 60,  TRUE },
	{ dGMColScriptsFighterFoxSpecialLw,  	    		 60,  TRUE },
	{ dGMColScriptsFighterFoxSpecialHiStart,			 60,  TRUE },
	{ dGMColScriptsFighterFoxSpecialHi,  				 60,  TRUE },
	{ dGMColScriptsFighterLinkSpecialHi,  	 			 60,  TRUE },
	{ dGMColScriptsFighterSamusSpecialHi,  				 60,  TRUE },
	{ dGMColScriptsFighterUnknown1,  					 60,  TRUE },
	{ dGMColScriptsFighterSamusSpecialNEnd,     		 60,  TRUE },
	{ dGMColScriptsFighterDonkeySpecialNLoop,  			 60,  TRUE },
	{ dGMColScriptsFighterDonkeySpecialNEnd,  			 60,  TRUE },
	{ dGMColScriptsFighterCaptainSpecialN,  			 60,  TRUE },
	{ dGMColScriptsFighterKirbySpecialLwHigh,  			 60,  TRUE },
	{ dGMColScriptsFighterKirbySpecialLwMid,  			 60,  TRUE },
	{ dGMColScriptsFighterKirbySpecialLwLow,  			 60,  TRUE },
	{ dGMColScriptsFighterKirbySpecialLwStart,  		 60,  TRUE },
	{ dGMColScriptsFighterKirbySpecialLwEnd,  			 60,  TRUE },
	{ dGMColScriptsFighterPikachuAttackS4,  			 60,  TRUE },
	{ dGMColScriptsFighterPikachuSpecialHiStart,		 60,  TRUE },
#if defined(REGION_US)
	{ dGMColScriptsFighterPikachuSpecialHi,  			 60,  TRUE },
#endif
	{ dGMColScriptsFighterPikachuSpecialN,  			 60,  TRUE },
	{ dGMColScriptsFighterPikachuSpecialLwHit,  		 60,  TRUE },
	{ dGMColScriptsFighterPikachuSpecialLwEnd,  		 60,  TRUE },
	{ dGMColScriptsFighterCaptainSpecialHi,  			 60,  TRUE },
	{ dGMColScriptsFighterNessSpecialLwHold,  			 60,  TRUE },
	{ dGMColScriptsFighterNessSpecialLwHit,  			 60,  TRUE },
	{ dGMColScriptsFighterNessSpecialHiHold,  			 60,  TRUE },
	{ dGMColScriptsFighterNessSpecialHiJibaku,  		 60,  TRUE },
	{ dGMColScriptsFighterNessAppear,  					 60,  TRUE },
	{ dGMColScriptsFighterBossDeskArrange,  			 60,  TRUE },
	{ dGMColScriptsFighterBossOkuhikouki,  				 60,  TRUE },
	{ dGMColScriptsFighterBossOkupunch,  				 60,  TRUE },
	{ dGMColScriptsFighterBossYubideppou2,  			 60,  TRUE },
	{ dGMColScriptsFighterBossYubideppou3,  			 60,  TRUE },
	{ dGMColScriptsFighterHammer,  						 12, FALSE },
	{ dGMColScriptsFighterStar, 			   			100, FALSE },
	{ dGMColScriptsFighterStarRod,  					 10,  TRUE },
	{ dGMColScriptsFighterBat,  						 60,  TRUE },
	{ dGMColScriptsItemBombHeiCritical,  				 60,  TRUE },
	{ dGMColScriptsItemHammerEnd,  						 60,  TRUE },
	{ dGMColScriptsItemLinkBombCritical,  				 60,  TRUE },
	{ dGMColScriptsFighterChallenger,  					 60,  TRUE },
	{ dGMColScriptsScreenFlashDeadExplode,  			 60,  TRUE },
	{ dGMColScriptsScreenFlashDamageNormal,  			 60,  TRUE },
	{ dGMColScriptsScreenFlashDamageFire,  				 60,  TRUE },
	{ dGMColScriptsScreenFlashDamageElectric,  			 60,  TRUE },
	{ dGMColScriptsScreenFlashDamageIce,  				 60,  TRUE }
};
