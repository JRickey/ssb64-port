#include <ft/fighter.h>
#include <mn/menu.h>
#include <sc/scene.h>
#include <gm/gmsound.h>
#include <sys/controller.h>
#include <sys/video.h>
#include <sys/rdp.h>
#include <reloc_data.h>


// // // // // // // // // // // //
//                               //
//             MACROS            //
//                               //
// // // // // // // // // // // //

#define mnVSOptionsCheckGetOptionButtonInput(is_button, mask) \
mnCommonCheckGetOptionButtonInput(sMNVSOptionsOptionChangeWait, is_button, mask)

#define mnVSOptionsCheckGetOptionStickInputUD(stick_range, min, b) \
mnCommonCheckGetOptionStickInputUD(sMNVSOptionsOptionChangeWait, stick_range, min, b)

#define mnVSOptionsCheckGetOptionStickInputLR(stick_range, min, b) \
mnCommonCheckGetOptionStickInputLR(sMNVSOptionsOptionChangeWait, stick_range, min, b)

#define mnVSOptionsSetOptionChangeWaitP(is_button, stick_range, div) \
mnCommonSetOptionChangeWaitP(sMNVSOptionsOptionChangeWait, is_button, stick_range, div)

#define mnVSOptionsSetOptionChangeWaitN(is_button, stick_range, div) \
mnCommonSetOptionChangeWaitN(sMNVSOptionsOptionChangeWait, is_button, stick_range, div)

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x801346C0
u32 dMNVSOptionsFileIDs[/* */] = { &llMNCommonFileID, &llMNVSOptionsFileID };

// 0x801346C8
Lights1 dMNVSOptionsLights1 = gdSPDefLights1(0x20, 0x20, 0x20, 0xFF, 0xFF, 0xFF, 0x3C, 0x3C, 0x3C);

// 0x801346E0
Gfx dMNVSOptionsDisplayList[/* */] =
{
    gsSPSetGeometryMode(G_LIGHTING),
    gsSPSetLights1(dMNVSOptionsLights1),
    gsSPEndDisplayList()
};

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x801348C0
GObj *sMNVSOptionsOptionGObjs[nMNVSOptionsOptionEnumCount];

// 0x801348D8
s32 sMNVSOptionsPad0x801348D8[2];

// 0x801348E0
s32 sMNVSOptionsOption;

// 0x801348E4
s32 sMNVSOptionsHandicapStatus;

// 0x801348E8
s32 sMNVSOptionsTeamAttackStatus;

// 0x801348EC
s32 sMNVSOptionsStageSelectStatus;

// 0x801348F0
s32 sMNVSOptionsDamage;

// 0x801348F4
GObj *sMNVSOptionsSubtitlesGObj;

// 0x801348F8
s32 sMNVSOptionsDamageGObj;

// 0x801348FC
s32 sMNVSOptionsFirstAvailableOption;

// 0x80134900
s32 sMNVSOptionsLastAvailableOption;

// 0x80134904
sb32 sMNVSOptionsIsHaveItemSwitch;

// 0x80134908
s32 sMNVSOptionsOptionChangeWait;

// 0x8013490C
s32 sMNVSOptionsTotalTimeTics;

// 0x80134910
s32 sMNVSOptionsReturnTic;

// 0x80134914
// s32 sMNVSOptionsPad0x80134914;

// 0x80134918
LBFileNode sMNVSOptionsStatusBuffer[24];

// 0x801349D8
void *sMNVSOptionsFiles[ARRAY_COUNT(dMNVSOptionsFileIDs)];

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80131B00
void mnVSOptionsFuncLights(Gfx **dls)
{
    gSPDisplayList(dls[0]++, dMNVSOptionsDisplayList);
}

// 0x80131B24
sb32 mnVSOptionsCheckHaveItemSwitch(void)
{
    if (gSCManagerBackupData.unlock_mask & LBBACKUP_UNLOCK_MASK_ITEMSWITCH)
    {
        return TRUE;
    }
    else return FALSE;
}

// 0x80131B4C
s32 mnVSOptionsGetPowerOf(s32 base, s32 exp)
{
	s32 raised = base;
	s32 i;

	if (exp == 0)
	{
		return 1;
	}
	i = exp;

	while (i > 1)
	{
		i--;
		raised *= base;
	}
	return raised;
}

// 0x80131BEC
void mnVSOptionsSetDamageDigitSpriteColors(SObj *sobj, u32 *colors)
{
    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;
    
    sobj->sprite.red = colors[0];
    sobj->sprite.green = colors[1];
    sobj->sprite.blue = colors[2];
}

// 0x80131C1C
s32 mnVSOptionsGetDamageDigitCount(s32 damage, s32 digit_count_max)
{
	s32 digit_count_curr = digit_count_max;

	while (digit_count_curr > 0)
	{
		s32 digit = (mnVSOptionsGetPowerOf(10, digit_count_curr - 1) != 0) ? damage / mnVSOptionsGetPowerOf(10, digit_count_curr - 1) : 0;

		if (digit != 0)
		{
			return digit_count_curr;
		}
		else digit_count_curr--;
	}
	return 0;
}

// 0x80131CC8
void mnVSOptionsMakeDamageDigitSObjs(GObj *gobj, s32 damage, f32 pos_x, f32 pos_y, u32 *colors, s32 digit_count_max, sb32 is_fixed_digit_count)
{    
    // 0x80134708
    intptr_t digit_offsets[/* */] =
    {
        &llMNCommonDigit0Sprite,
        &llMNCommonDigit1Sprite,
        &llMNCommonDigit2Sprite,
        &llMNCommonDigit3Sprite,
        &llMNCommonDigit4Sprite,
        &llMNCommonDigit5Sprite,
        &llMNCommonDigit6Sprite,
        &llMNCommonDigit7Sprite,
        &llMNCommonDigit8Sprite,
        &llMNCommonDigit9Sprite
    };

    // 0x80134730 - unused
    f32 digit_widths[/* */] =
    {
        10.0F,
         6.0F,
         9.0F,
         9.0F,
        10.0F,
         9.0F,
         9.0F,
        10.0F,
         9.0F,
        10.0F
    };

    s32 i;
    s32 digit;
    f32 calc_x;
    SObj *sobj;

    calc_x = pos_x;

    if (damage < 0)
    {
        damage = 0;
    }
    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNVSOptionsFiles[0], digit_offsets[damage % 10]));
    mnVSOptionsSetDamageDigitSpriteColors(sobj, colors);

    calc_x -= 11.0F;
    
    sobj->pos.x = calc_x;
    sobj->pos.y = pos_y;
    
    for (i = 1; i < ((is_fixed_digit_count != FALSE) ? digit_count_max : mnVSOptionsGetDamageDigitCount(damage, digit_count_max)); i++)
    {
        digit = (mnVSOptionsGetPowerOf(10, i) != 0) ? damage / mnVSOptionsGetPowerOf(10, i) : 0;

        sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNVSOptionsFiles[0], digit_offsets[digit % 10]));
        
        mnVSOptionsSetDamageDigitSpriteColors(sobj, colors);
        
        calc_x -= 11.0F;

        sobj->pos.x = calc_x;
        sobj->pos.y = pos_y;
    }
}

// 0x80131F74
void mnVSOptionsSetOptionSpriteColors(GObj *gobj, s32 status)
{
    // 0x80134758
    SYColorRGBPair notcolors = { { 0x00, 0x00, 0x00 }, { 0x82, 0x82, 0xAA } };

    // 0x80134760
    SYColorRGBPair hicolors = { { 0xFA, 0x8C, 0x00 }, { 0xF4, 0xC8, 0x0A } };

    SYColorRGBPair *colors;
    
    SObj *sobj = SObjGetStruct(gobj);
    
    colors = (status != nMNOptionTabStatusNot) ? &hicolors : &notcolors;

    sobj->envcolor.r = colors->prim.r;
    sobj->envcolor.g = colors->prim.g;
    sobj->envcolor.b = colors->prim.b;
    
    sobj->sprite.red = colors->env.r;
    sobj->sprite.green = colors->env.g;
    sobj->sprite.blue = colors->env.b;
}

// 0x80131FFC
void mnVSOptionsSetToggleSpriteColors(GObj *gobj, s32 status)
{
    SObj *sobj = SObjGetStruct(gobj)->next->next;
    
    if (status != nMNOptionTabStatusOff)
    {
        sobj->sprite.red = 0xFF;
        sobj->sprite.green = 0x00;
        sobj->sprite.blue = 0x28;
        
        sobj->next->sprite.red = 0x32;
        sobj->next->sprite.green = 0x32;
        sobj->next->sprite.blue = 0x32;
    }
    else
    {
        sobj->sprite.red = 0x32;
        sobj->sprite.green = 0x32;
        sobj->sprite.blue = 0x32;
        
        sobj->next->sprite.red = 0xFF;
        sobj->next->sprite.green = 0x00;
        sobj->next->sprite.blue = 0x28;
    }
}

// 0x80132078
void mnVSOptionsMakeOnOffToggle(GObj *gobj, f32 pos_x, f32 pos_y)
{
    SObj *sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNVSOptionsFiles[0], &llMNCommonOnTextSprite));

    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;
    
    sobj->pos.x = pos_x;
    sobj->pos.y = pos_y;
    
    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNVSOptionsFiles[0], &llMNCommonOffTextSprite));
    
    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;

    sobj->pos.x = pos_x + 25.0F + 7.0F;
    sobj->pos.y = pos_y;

    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNVSOptionsFiles[0], &llMNCommonSlashSprite));
    
    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;
    
    sobj->pos.x = pos_x + 25.0F;
    sobj->pos.y = pos_y;
    
    sobj->sprite.red = 0x32;
    sobj->sprite.green = 0x32;
    sobj->sprite.blue = 0x32;
}

// 0x80132184
void mnVSOptionsSetHandicapSpriteColors(GObj *gobj, s32 setting)
{
    SObj *sobj = SObjGetStruct(gobj)->next->next;

    switch (setting)
    {
    case nSCBattleHandicapOn:
        sobj->sprite.red = 0xFF;
        sobj->sprite.green = 0x00;
        sobj->sprite.blue = 0x28;
        
        sobj->next->sprite.red = 0x32;
        sobj->next->sprite.green = 0x32;
        sobj->next->sprite.blue = 0x32;
        
        sobj->next->next->sprite.red = 0x32;
        sobj->next->next->sprite.green = 0x32;
        sobj->next->next->sprite.blue = 0x32;
        break;
        
    case nSCBattleHandicapAuto:
        sobj->sprite.red = 0x32;
        sobj->sprite.green = 0x32;
        sobj->sprite.blue = 0x32;
        
        sobj->next->sprite.red = 0xFF;
        sobj->next->sprite.green = 0x00;
        sobj->next->sprite.blue = 0x28;
        
        sobj->next->next->sprite.red = 0x32;
        sobj->next->next->sprite.green = 0x32;
        sobj->next->next->sprite.blue = 0x32;
        break;
        
    case nSCBattleHandicapOff:
        sobj->sprite.red = 0x32;
        sobj->sprite.green = 0x32;
        sobj->sprite.blue = 0x32;
        
        sobj->next->sprite.red = 0x32;
        sobj->next->sprite.green = 0x32;
        sobj->next->sprite.blue = 0x32;
        
        sobj->next->next->sprite.red = 0xFF;
        sobj->next->next->sprite.green = 0x00;
        sobj->next->next->sprite.blue = 0x28;
        break;
    }
}

// 0x801322B8
void mnVSOptionsMakeHandicapToggle(GObj *gobj, f32 pos_x, f32 pos_y)
{
    SObj *sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNVSOptionsFiles[0], &llMNCommonOnTextSprite));

    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;
    
    sobj->pos.x = pos_x;
    sobj->pos.y = pos_y;
    
    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNVSOptionsFiles[0], &llMNCommonAutoTextSprite));

    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;
    
    sobj->pos.x = pos_x + 25.0F + 5.0F;
    sobj->pos.y = pos_y + 1.0F;

    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNVSOptionsFiles[0], &llMNCommonOffTextSprite));
    
    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;
    
    sobj->pos.x = pos_x + 25.0F + 5.0F + 30.0F + 6.0F;
    sobj->pos.y = pos_y;

    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNVSOptionsFiles[0], &llMNCommonSlashSprite));
    
    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;

    sobj->pos.x = pos_x + 25.0F;
    sobj->pos.y = pos_y;
    
    sobj->sprite.red = 0x32;
    sobj->sprite.green = 0x32;
    sobj->sprite.blue = 0x32;
    
    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNVSOptionsFiles[0], &llMNCommonSlashSprite));
    
    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;
    
    sobj->pos.x = pos_x + 25.0F + 5.0F + 30.0F;
    sobj->pos.y = pos_y;

    sobj->sprite.red = 0x32;
    sobj->sprite.green = 0x32;
    sobj->sprite.blue = 0x32;
}

// 0x80132478
void mnVSOptionsMakeDamageDigits(void)
{
    GObj *gobj;
    s32 unused;
    
    // 0x80134768
    u32 colors[/* */] =
    {
        0xFF, 0x00, 0x28,
        0x00, 0x00, 0x00
    };
 
#if defined(REGION_US)
    s32 pos_y = (sMNVSOptionsIsHaveItemSwitch != FALSE) ? 151 : 164;
#else
    s32 pos_y = (sMNVSOptionsIsHaveItemSwitch != FALSE) ? 135 : 144;
#endif
    
    sMNVSOptionsDamageGObj = gobj = gcMakeGObjSPAfter(0, NULL, 4, GOBJ_PRIORITY_DEFAULT);

    gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 2, GOBJ_PRIORITY_DEFAULT, ~0);
    mnVSOptionsMakeDamageDigitSObjs(gobj, sMNVSOptionsDamage, 220.0F, pos_y, colors, 3, FALSE);
}

// 0x80132564
void mnVSOptionsMakeDamageOption(void)
{
    GObj *gobj;
    SObj *sobj;
    s32 pos_y;

#if defined(REGION_US)
    pos_y = (sMNVSOptionsIsHaveItemSwitch != FALSE) ? 148 : 161;
#else
    pos_y = (sMNVSOptionsIsHaveItemSwitch != FALSE) ? 132 : 141;
#endif

    sMNVSOptionsOptionGObjs[nMNVSOptionsOptionDamage] = gobj = gcMakeGObjSPAfter(0, NULL, 4, GOBJ_PRIORITY_DEFAULT);
    gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 2, GOBJ_PRIORITY_DEFAULT, ~0);
    
    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNVSOptionsFiles[1], &llMNVSOptionsBubbleSprite));
    
    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;
    
    sobj->pos.x = 90.0F;
    sobj->pos.y = pos_y;
    
    mnVSOptionsSetOptionSpriteColors(gobj, sMNVSOptionsOption == nMNVSOptionsOptionDamage);
    
    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNVSOptionsFiles[1], &llMNVSOptionsDamageTextSprite));
    
    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;
    
    sobj->pos.x = 116.0F;
    sobj->pos.y = pos_y + 1;

    sobj->sprite.red = 0x00;
    sobj->sprite.green = 0x00;
    sobj->sprite.blue = 0x00;
    
    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNVSOptionsFiles[0], &llMNCommonPercentageSprite));

    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;

    sobj->pos.x = 226.0F;
    sobj->pos.y = pos_y + 3;
    
    sobj->sprite.red = 0x00;
    sobj->sprite.green = 0x00;
    sobj->sprite.blue = 0x00;
}

// 0x801326F0
void mnVSOptionsMakeItemSwitchOption(void)
{
    GObj *gobj;
    SObj *sobj;
#if defined(REGION_US)
    s32 pos_y = 176;
#endif

    sMNVSOptionsOptionGObjs[nMNVSOptionsOptionItemSwitch] = gobj = gcMakeGObjSPAfter(0, NULL, 4, GOBJ_PRIORITY_DEFAULT);

    gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 2, GOBJ_PRIORITY_DEFAULT, ~0);
    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNVSOptionsFiles[1], &llMNVSOptionsBubbleSprite));
    
    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;

    sobj->pos.x = 82.0F;
#if defined(REGION_US)
    sobj->pos.y = pos_y + 1;
#else
    sobj->pos.y = 157.0F;
#endif
    
    mnVSOptionsSetOptionSpriteColors(gobj, sMNVSOptionsOption == nMNVSOptionsOptionItemSwitch);
    
    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNVSOptionsFiles[1], &llMNVSOptionsItemSwitchTextSprite));
    
    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;
    
    sobj->pos.x = 128.0F;
#if defined(REGION_US)
    sobj->pos.y = pos_y + 3;
#else
    sobj->pos.y = 159.0F;
#endif

    sobj->sprite.red = 0x00;
    sobj->sprite.green = 0x00;
    sobj->sprite.blue = 0x00;
}

// 0x80132804
void mnVSOptionsMakeStageSelectOption(void)
{
    GObj *gobj;
    SObj *sobj;
    s32 pos_y;

#if defined(REGION_US)
    pos_y = (sMNVSOptionsIsHaveItemSwitch != FALSE) ? 119 : 129;
#else
    pos_y = (sMNVSOptionsIsHaveItemSwitch != FALSE) ? 107 : 113;
#endif

    sMNVSOptionsOptionGObjs[nMNVSOptionsOptionStageSelect] = gobj = gcMakeGObjSPAfter(0, NULL, 4, GOBJ_PRIORITY_DEFAULT);
    gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 2, GOBJ_PRIORITY_DEFAULT, ~0);
    
    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNVSOptionsFiles[1], &llMNVSOptionsBubbleSprite));
    
    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;
    
    sobj->pos.x = 98.0F;
    sobj->pos.y = pos_y;
    
    mnVSOptionsSetOptionSpriteColors(gobj, sMNVSOptionsOption == nMNVSOptionsOptionStageSelect);
    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNVSOptionsFiles[1], &llMNVSOptionsStageSelectTextSprite));

    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;
    
    sobj->pos.x = 104.0F;
    sobj->pos.y = pos_y + 1;
    
    sobj->sprite.red = 0x00;
    sobj->sprite.green = 0x00;
    sobj->sprite.blue = 0x00;
    
    mnVSOptionsMakeOnOffToggle(gobj, 208.0F, pos_y + 1);
    mnVSOptionsSetToggleSpriteColors(gobj, sMNVSOptionsStageSelectStatus);
}

// 0x80132968
void mnVSOptionsMakeTeamAttackOption(void)
{
    GObj *gobj;
    SObj *sobj;
    s32 pos_y;

#if defined(REGION_US)
    pos_y = (sMNVSOptionsIsHaveItemSwitch != FALSE) ? 90 : 97;
#else
    pos_y = (sMNVSOptionsIsHaveItemSwitch != FALSE) ? 82 : 85;
#endif

    sMNVSOptionsOptionGObjs[nMNVSOptionsOptionTeamAttack] = gobj = gcMakeGObjSPAfter(0, NULL, 4, GOBJ_PRIORITY_DEFAULT);
    gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 2, GOBJ_PRIORITY_DEFAULT, ~0);
    
    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNVSOptionsFiles[1], &llMNVSOptionsBubbleSprite));
    
    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;
    
    sobj->pos.x = 106.0F;
    sobj->pos.y = pos_y;
    
    mnVSOptionsSetOptionSpriteColors(gobj, sMNVSOptionsOption == nMNVSOptionsOptionTeamAttack);

    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNVSOptionsFiles[1], &llMNVSOptionsTeamAttackTextSprite));
    
    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;
    
    sobj->pos.x = 116.0F;
    sobj->pos.y = pos_y + 2;
    
    sobj->sprite.red = 0x00;
    sobj->sprite.green = 0x00;
    sobj->sprite.blue = 0x00;
    
    mnVSOptionsMakeOnOffToggle(gobj, 212.0F, pos_y + 1);
    mnVSOptionsSetToggleSpriteColors(gobj, sMNVSOptionsTeamAttackStatus);
}

// 0x80132AC8
void mnVSOptionsMakeHandicapOption(void)
{
    GObj *gobj;
    SObj *sobj;
    s32 pos_y;

#if defined(REGION_US)
    pos_y = (sMNVSOptionsIsHaveItemSwitch != FALSE) ? 61 : 65;
#else
    pos_y = 57;
#endif

    sMNVSOptionsOptionGObjs[nMNVSOptionsOptionHandicap] = gobj = gcMakeGObjSPAfter(0, NULL, 4, GOBJ_PRIORITY_DEFAULT);
    gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 2, GOBJ_PRIORITY_DEFAULT, ~0);
    
    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNVSOptionsFiles[1], &llMNVSOptionsBubbleSprite));
    
    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;
    
    sobj->pos.x = 114.0F;
    sobj->pos.y = pos_y;
    
    mnVSOptionsSetOptionSpriteColors(gobj, sMNVSOptionsOption == nMNVSOptionsOptionHandicap);
    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNVSOptionsFiles[1], &llMNVSOptionsHandicapTextSprite));
    
    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;
    
    sobj->pos.x = 121.0F;
    sobj->pos.y = pos_y + 2;
    
    sobj->sprite.red = 0x00;
    sobj->sprite.green = 0x00;
    sobj->sprite.blue = 0x00;
    
    mnVSOptionsMakeHandicapToggle(gobj, 191.0F, pos_y + 1);
    mnVSOptionsSetHandicapSpriteColors(gobj, sMNVSOptionsHandicapStatus);
}

// 0x80132C24
void mnVSOptionsLabelProcDisplay(GObj *gobj)
{
    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_1CYCLE);
    gDPSetCombineMode(gSYTaskmanDLHeads[0]++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
    gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0x80, 0x80, 0x80, 0xFF);
    gDPFillRectangle(gSYTaskmanDLHeads[0]++, 79, 34, 310, 39);
    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_1CYCLE);
    
    lbCommonClearExternSpriteParams();
    lbCommonDrawSObjAttr(gobj);
}

// 0x80132D68
void mnVSOptionsMakeLabel(void)
{
    GObj *gobj;
    SObj *sobj;
    
    gobj = gcMakeGObjSPAfter(0, NULL, 3, GOBJ_PRIORITY_DEFAULT);
    gcAddGObjDisplay(gobj, mnVSOptionsLabelProcDisplay, 1, GOBJ_PRIORITY_DEFAULT, ~0);
    
    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNVSOptionsFiles[1], &llMNVSOptionsVSOptionsTextSprite));

    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;
    
    sobj->envcolor.r = 0x00;
    sobj->envcolor.g = 0x00;
    sobj->envcolor.b = 0x00;
    
    sobj->sprite.red = 0xF2;
    sobj->sprite.green = 0xC7;
    sobj->sprite.blue = 0x0D;
    
    sobj->pos.x = 84.0F;
    sobj->pos.y = 24.0F;
}

// 0x80132E24
void mnVSOptionsSetSubtitleSpriteColors(SObj *sobj)
{
    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;
    
    sobj->envcolor.r = 0x00;
    sobj->envcolor.g = 0x00;
    sobj->envcolor.b = 0x00;
    
    sobj->sprite.red = 0xFF;
    sobj->sprite.green = 0xFF;
    sobj->sprite.blue = 0xFF;
}

// 0x80132E58
void mnVSOptionsMakeSubtitle(void)
{
    GObj *gobj;
    SObj *sobj;
    
    // 0x80134780
    intptr_t sp1C[/* */] =
    {
        &llMNCommonOffTextSprite,
        &llMNCommonOnTextSprite,
        &llMNCommonAutoTextJapSprite
    };
    
    sMNVSOptionsSubtitlesGObj = gobj = gcMakeGObjSPAfter(0, NULL, 3, GOBJ_PRIORITY_DEFAULT);

#if defined(REGION_JP)
    gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 1, GOBJ_PRIORITY_DEFAULT, ~0);

    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNVSOptionsFiles[0], &llMNCommonFrameSprite));

    sobj->pos.x = 93.0F;
    sobj->pos.y = 189.0F;
    
    mnVSOptionsSetSubtitleSpriteColors(sobj);
    
    switch (sMNVSOptionsOption)
    {
        case nMNVSOptionsOptionHandicap:
            sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNVSOptionsFiles[1], &llMNVSOptionsHandicapTextJapSprite));
            
            sobj->pos.x = 120.0F;
            sobj->pos.y = 195.0F;
            
            mnVSOptionsSetSubtitleSpriteColors(sobj);

            switch (sMNVSOptionsHandicapStatus)
            {
                case nSCBattleHandicapOff:

                    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNVSOptionsFiles[0], &llMNCommonOffTextJapSprite));
                    
                    sobj->pos.x = 177.0F;
                    sobj->pos.y = 196.0F;
                    break;
                case nSCBattleHandicapOn:
                    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNVSOptionsFiles[0], &llMNCommonOnTextJapSprite));
                    
                    sobj->pos.x = 177.0F;
                    sobj->pos.y = 196.0F;
                    break;
                case nSCBattleHandicapAuto:
                    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNVSOptionsFiles[0], &llMNCommonAutoTextJapSprite));
                    
                    sobj->pos.x = 177.0F;
                    sobj->pos.y = 195.0F;
                    break;
            }

            mnVSOptionsSetSubtitleSpriteColors(sobj);

            return;

        case nMNVSOptionsOptionTeamAttack:
            sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNVSOptionsFiles[1], &llMNVSOptionsTeamAttackTextJapSprite));
            
            sobj->pos.x = 96.0F;
            sobj->pos.y = 195.0F;
            
            mnVSOptionsSetSubtitleSpriteColors(sobj);

            if (sMNVSOptionsTeamAttackStatus == nMNOptionTabStatusOn)
            {
                sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNVSOptionsFiles[0], &llMNCommonOnTextJapSprite));
            }
            else
            {
                sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNVSOptionsFiles[0], &llMNCommonOffTextJapSprite));
            }
                    
            sobj->pos.x = 200.0F;
            sobj->pos.y = 196.0F;
            
            mnVSOptionsSetSubtitleSpriteColors(sobj);
            return;

        case nMNVSOptionsOptionStageSelect:
            sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNVSOptionsFiles[1], &llMNVSOptionsStageSelectTextJapSprite));
            
            sobj->pos.x = 104.0F;
            sobj->pos.y = 195.0F;
            
            mnVSOptionsSetSubtitleSpriteColors(sobj);

            if (sMNVSOptionsStageSelectStatus == nMNOptionTabStatusOn)
            {
                sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNVSOptionsFiles[0], &llMNCommonOnTextJapSprite));
            }
            else
            {
                sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNVSOptionsFiles[0], &llMNCommonOffTextJapSprite));
            }
            
            sobj->pos.x = 192.0F;
            sobj->pos.y = 196.0F;
            
            mnVSOptionsSetSubtitleSpriteColors(sobj);
            return;

        case nMNVSOptionsOptionItemSwitch:
            sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNVSOptionsFiles[1], &llMNVSOptionsItemSwitchTextJapSprite));
            
            sobj->pos.x = 109.0F;
            sobj->pos.y = 195.0F;
            
            mnVSOptionsSetSubtitleSpriteColors(sobj);
            return;

        case nMNVSOptionsOptionDamage:
            sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNVSOptionsFiles[1], &llMNVSOptionsDamageTextJapSprite));
            
            sobj->pos.x = 109.0F;
            sobj->pos.y = 194.0F;
            
            mnVSOptionsSetSubtitleSpriteColors(sobj);
            return;
    }
#endif
}

// 0x80132EAC
void mnVSOptionsMakeWallpaper(void)
{
    GObj *gobj;
    SObj *sobj;

    gobj = gcMakeGObjSPAfter(0, NULL, 2, GOBJ_PRIORITY_DEFAULT);
    gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 0, GOBJ_PRIORITY_DEFAULT, ~0);
    
    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNVSOptionsFiles[0], &llMNCommonSmashBrosCollageSprite));
    
    sobj->pos.x = 10.0F;
    sobj->pos.y = 10.0F;
}

// 0x80132F2C
void mnVSOptionsUnderlineProcDisplay(GObj *gobj)
{   
    // 0x8013478C
#if defined(REGION_US)
    SYRectangle handicap_rect[/* */] =
    {
        { 255, 77, 283, 77 },
        { 190, 77, 216, 77 },
        { 219, 77, 251, 77 }
    };
    
    // 0x801347BC
    SYRectangle team_attack_rect[/* */] =
    {
        { 245, 106, 272, 106 },
        { 213, 106, 239, 106 }
    };

    // 0x801347DC
    SYRectangle stage_select_rect[/* */] =
    {
        { 241, 135, 269, 135 },
        { 208, 135, 234, 135 }
    };
#else
    SYRectangle handicap_rect[/* */] =
    {
        { 255, 73, 283, 73 },
        { 190, 73, 216, 73 },
        { 219, 73, 251, 73 }
    };
    
    // 0x801347BC
    SYRectangle team_attack_rect[/* */] =
    {
        { 245, 98, 272, 98 },
        { 213, 98, 239, 98 }
    };

    // 0x801347DC
    SYRectangle stage_select_rect[/* */] =
    {
        { 241, 123, 269, 123 },
        { 208, 123, 234, 123 }
    };
#endif

    s32 off_y;

    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_FILL);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_NOOP, G_RM_NOOP2);
    gDPSetFillColor(gSYTaskmanDLHeads[0]++, syVideoGetFillColor(GPACK_RGBA8888(0xFF, 0x00, 0x28, 0xFF)));

    switch (sMNVSOptionsOption)
    {
    case nMNVSOptionsOptionHandicap:
#if defined(REGION_US)
        off_y = (sMNVSOptionsIsHaveItemSwitch != FALSE) ? 0 : 4;
#else
        off_y = 0;
#endif
        gDPFillRectangle
        (
            gSYTaskmanDLHeads[0]++,
            handicap_rect[sMNVSOptionsHandicapStatus].ulx,
            handicap_rect[sMNVSOptionsHandicapStatus].uly + off_y,
            handicap_rect[sMNVSOptionsHandicapStatus].lrx,
            handicap_rect[sMNVSOptionsHandicapStatus].lry + off_y
        );
        break;
        
    case nMNVSOptionsOptionTeamAttack:
#if defined(REGION_US)
        off_y = (sMNVSOptionsIsHaveItemSwitch != FALSE) ? 0 : 7;
#else
        off_y = (sMNVSOptionsIsHaveItemSwitch != FALSE) ? 0 : 3;
#endif

        gDPFillRectangle
        (
            gSYTaskmanDLHeads[0]++,
            team_attack_rect[sMNVSOptionsTeamAttackStatus].ulx,
            team_attack_rect[sMNVSOptionsTeamAttackStatus].uly + off_y,
            team_attack_rect[sMNVSOptionsTeamAttackStatus].lrx,
            team_attack_rect[sMNVSOptionsTeamAttackStatus].lry + off_y
        );
        break;
        
    case nMNVSOptionsOptionStageSelect:
#if defined(REGION_US)
        off_y = (sMNVSOptionsIsHaveItemSwitch != FALSE) ? 0 : 10;
#else
        off_y = (sMNVSOptionsIsHaveItemSwitch != FALSE) ? 0 : 6;
#endif

        gDPFillRectangle
        (
            gSYTaskmanDLHeads[0]++,
            stage_select_rect[sMNVSOptionsStageSelectStatus].ulx,
            stage_select_rect[sMNVSOptionsStageSelectStatus].uly + off_y,
            stage_select_rect[sMNVSOptionsStageSelectStatus].lrx,
            stage_select_rect[sMNVSOptionsStageSelectStatus].lry + off_y
        );
        break;
    }
    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_1CYCLE);
    
    lbCommonClearExternSpriteParams();
}

// 0x80133300
void mnVSOptionsMakeUnderline(void)
{
    gcAddGObjDisplay(gcMakeGObjSPAfter(0, NULL, 3, GOBJ_PRIORITY_DEFAULT), mnVSOptionsUnderlineProcDisplay, 1, GOBJ_PRIORITY_DEFAULT, ~0);
}

// 0x8013334C
void mnVSOptionsTintProcDisplay(GObj *gobj)
{
    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_1CYCLE);
    gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0x0D, 0x00, 0x00, 0x99);
    gDPSetCombineMode(gSYTaskmanDLHeads[0]++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
    gDPFillRectangle(gSYTaskmanDLHeads[0]++, 10, 10, 310, 230);
    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    
    lbCommonClearExternSpriteParams();
}

// 0x80133464
void mnVSOptionsMakeTint(void)
{
    gcAddGObjDisplay(gcMakeGObjSPAfter(0, NULL, 5, GOBJ_PRIORITY_DEFAULT), mnVSOptionsTintProcDisplay, 3, GOBJ_PRIORITY_DEFAULT, ~0);
}

// 0x801334B0
void mnVSOptionsMakeDecal(void)
{
    GObj *gobj;
    SObj *sobj;
    
    gobj = gcMakeGObjSPAfter(0, NULL, 6, GOBJ_PRIORITY_DEFAULT);
    gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 4, GOBJ_PRIORITY_DEFAULT, ~0);
    
    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNVSOptionsFiles[1], &llMNVSOptionsConsoleIconDarkSprite));
    
    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;
    
    sobj->sprite.red = 0x4A;
    sobj->sprite.green = 0x2A;
    sobj->sprite.blue = 0x23;
    
    sobj->pos.x = 10.0F;
    sobj->pos.y = 10.0F;
}

// 0x80133558
void mnVSOptionsMakeTintCamera(void)
{
    CObj *cobj = CObjGetStruct
    (
        gcMakeCameraGObj
        (
            1,
            NULL,
            1,
            GOBJ_PRIORITY_DEFAULT,
            lbCommonDrawSprite,
            70,
            COBJ_MASK_DLLINK(3),
            ~0,
            FALSE,
            nGCProcessKindFunc,
            NULL,
            1,
            FALSE
        )
    );
    syRdpSetViewport(&cobj->viewport, 10.0F, 10.0F, 310.0F, 230.0F);
}

// 0x801335F8
void mnVSOptionsMakeOptionCamera(void)
{
    CObj *cobj = CObjGetStruct
    (
        gcMakeCameraGObj
        (
            1,
            NULL,
            1,
            GOBJ_PRIORITY_DEFAULT,
            lbCommonDrawSprite,
            50,
            COBJ_MASK_DLLINK(2),
            ~0,
            FALSE,
            nGCProcessKindFunc,
            NULL,
            1,
            FALSE
        )
    );
    syRdpSetViewport(&cobj->viewport, 10.0F, 10.0F, 310.0F, 230.0F);
}

// 0x80133698
void mnVSOptionsMakeLabelUnderlineCamera(void)
{
    CObj *cobj = CObjGetStruct
    (
        gcMakeCameraGObj
        (
            1,
            NULL,
            1,
            GOBJ_PRIORITY_DEFAULT,
            lbCommonDrawSprite,
            40,
            COBJ_MASK_DLLINK(1),
            ~0,
            FALSE,
            nGCProcessKindFunc,
            NULL,
            1,
            FALSE
        )
    );
    syRdpSetViewport(&cobj->viewport, 10.0F, 10.0F, 310.0F, 230.0F);
}

// 0x80133738
void mnVSOptionsMakeWallpaperCamera(void)
{
    CObj *cobj = CObjGetStruct
    (
        gcMakeCameraGObj
        (
            1,
            NULL,
            1,
            GOBJ_PRIORITY_DEFAULT,
            lbCommonDrawSprite,
            80,
            COBJ_MASK_DLLINK(0),
            ~0,
            FALSE,
            nGCProcessKindFunc,
            NULL,
            1,
            FALSE
        )
    );
    syRdpSetViewport(&cobj->viewport, 10.0F, 10.0F, 310.0F, 230.0F);
}

// 0x801337D8
void mnVSOptionsMakeDecalCamera(void)
{
    CObj *cobj = CObjGetStruct
    (
        gcMakeCameraGObj
        (
            1,
            NULL,
            1,
            GOBJ_PRIORITY_DEFAULT,
            lbCommonDrawSprite,
            60,
            COBJ_MASK_DLLINK(4),
            ~0,
            FALSE,
            nGCProcessKindFunc,
            NULL,
            1,
            FALSE
        )
    );
    syRdpSetViewport(&cobj->viewport, 10.0F, 10.0F, 310.0F, 230.0F);
}

// 0x80133878
void mnVSOptionsInitVars(void)
{
    sMNVSOptionsOption = (gSCManagerSceneData.scene_prev == nSCKindVSItemSwitch) ? nMNVSOptionsOptionItemSwitch : nMNVSOptionsOptionHandicap;

    sMNVSOptionsHandicapStatus = gSCManagerTransferBattleState.handicap;
    sMNVSOptionsTeamAttackStatus = gSCManagerTransferBattleState.is_team_attack;
    sMNVSOptionsStageSelectStatus = gSCManagerTransferBattleState.is_stage_select;
    sMNVSOptionsDamage = gSCManagerTransferBattleState.damage_ratio;
    
    sMNVSOptionsFirstAvailableOption = nMNVSOptionsOptionHandicap;
    
    if (mnVSOptionsCheckHaveItemSwitch() != FALSE)
    {
        sMNVSOptionsLastAvailableOption = nMNVSOptionsOptionItemSwitch;
        sMNVSOptionsIsHaveItemSwitch = TRUE;
    }
    else
    {
        sMNVSOptionsLastAvailableOption = nMNVSOptionsOptionDamage;
        sMNVSOptionsIsHaveItemSwitch = FALSE;
    }
    sMNVSOptionsOptionChangeWait = 0;
    
    sMNVSOptionsTotalTimeTics = 0;
    sMNVSOptionsReturnTic = sMNVSOptionsTotalTimeTics + I_MIN_TO_TICS(5);
}

// 0x8013394C
void mnVSOptionsSetAllSettings(void)
{
    gSCManagerTransferBattleState.handicap = sMNVSOptionsHandicapStatus;
    gSCManagerTransferBattleState.is_team_attack = sMNVSOptionsTeamAttackStatus;
    gSCManagerTransferBattleState.is_stage_select = sMNVSOptionsStageSelectStatus;
    gSCManagerTransferBattleState.damage_ratio = sMNVSOptionsDamage;
    
    if (gSCManagerTransferBattleState.handicap == nSCBattleHandicapOff)
    {
        s32 i;

        for (i = 0; i < ARRAY_COUNT(gSCManagerTransferBattleState.players); i++)
        {
            gSCManagerTransferBattleState.players[i].handicap = FTCOMMON_HANDICAP_DEFAULT;
        }
    }
}

// 0x801339C4
void mnVSOptionsSetHandicapSettings(void)
{
    s32 i;
    
    if (sMNVSOptionsHandicapStatus == nSCBattleHandicapAuto)
    {
        for (i = 0; i < ARRAY_COUNT(gSCManagerTransferBattleState.players); i++)
        {
            gSCManagerTransferBattleState.players[i].handicap = 5;
        }
    }
    else for (i = 0; i < ARRAY_COUNT(gSCManagerTransferBattleState.players); i++)
    {
        gSCManagerTransferBattleState.players[i].handicap = FTCOMMON_HANDICAP_DEFAULT;
    }
}

// 0x80133A40
void mnVSOptionsFuncRun(GObj *gobj)
{
    s32 unused;

    // 0x801347FC
    GObj **option_gobjss[/* */] =
    {
        &sMNVSOptionsOptionGObjs[nMNVSOptionsOptionHandicap],
        &sMNVSOptionsOptionGObjs[nMNVSOptionsOptionTeamAttack],
        &sMNVSOptionsOptionGObjs[nMNVSOptionsOptionStageSelect],
        &sMNVSOptionsOptionGObjs[nMNVSOptionsOptionDamage],
        &sMNVSOptionsOptionGObjs[nMNVSOptionsOptionItemSwitch]
    };
    
    s32 stick_range;
    sb32 is_button;
    
    sMNVSOptionsTotalTimeTics++;
    
    if (sMNVSOptionsTotalTimeTics >= 10)
    {
        if (sMNVSOptionsTotalTimeTics == sMNVSOptionsReturnTic)
        {
            gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
            gSCManagerSceneData.scene_curr = nSCKindTitle;
            
            mnVSOptionsSetAllSettings();
            syTaskmanSetLoadScene();
            return;
        }
        if (scSubsysControllerCheckNoInputAll() == FALSE)
        {
            sMNVSOptionsReturnTic = sMNVSOptionsTotalTimeTics + I_MIN_TO_TICS(5);
        }
        if (sMNVSOptionsOptionChangeWait != 0)
        {
            sMNVSOptionsOptionChangeWait--;
        }
        if
        (
            (scSubsysControllerGetPlayerStickInRangeLR(-20, 20) != FALSE)                                         &&
            (scSubsysControllerGetPlayerStickInRangeUD(-20, 20) != FALSE)                                         &&
            (scSubsysControllerGetPlayerHoldButtons(U_JPAD | R_JPAD | R_TRIG | U_CBUTTONS | R_CBUTTONS) == FALSE) &&
            (scSubsysControllerGetPlayerHoldButtons(D_JPAD | L_JPAD | L_TRIG | D_CBUTTONS | L_CBUTTONS) == FALSE)
        )
        {
            sMNVSOptionsOptionChangeWait = 0;
        }
        if ((scSubsysControllerGetPlayerTapButtons(A_BUTTON | START_BUTTON) != FALSE) && (sMNVSOptionsOption == nMNVSOptionsOptionItemSwitch))
        {
            func_800269C0_275C0(nSYAudioFGMMenuSelect);
            
            gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
            gSCManagerSceneData.scene_curr = nSCKindVSItemSwitch;

            mnVSOptionsSetAllSettings();
            syTaskmanSetLoadScene();
        }
        if (scSubsysControllerGetPlayerTapButtons(B_BUTTON) != FALSE)
        {
            gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
            gSCManagerSceneData.scene_curr = nSCKindVSMode;

            mnVSOptionsSetAllSettings();
            syTaskmanSetLoadScene();
        }
        if
        (
            mnVSOptionsCheckGetOptionButtonInput(is_button, U_JPAD | U_CBUTTONS) ||
            mnVSOptionsCheckGetOptionStickInputUD(stick_range, 20, 1)
        )
        {
            func_800269C0_275C0(nSYAudioFGMMenuScroll2);
        
            mnVSOptionsSetOptionChangeWaitP(is_button, stick_range, 7);
                
            mnVSOptionsSetOptionSpriteColors(*option_gobjss[sMNVSOptionsOption], nMNOptionTabStatusNot);
        
            if (sMNVSOptionsOption == sMNVSOptionsFirstAvailableOption)
            {
                sMNVSOptionsOption = sMNVSOptionsLastAvailableOption;
            }
            else sMNVSOptionsOption--;
                
            mnVSOptionsSetOptionSpriteColors(*option_gobjss[sMNVSOptionsOption], nMNOptionTabStatusHighlight);
        
            if (sMNVSOptionsOption == sMNVSOptionsFirstAvailableOption)
            {
                sMNVSOptionsOptionChangeWait += 8;
            }
            gcEjectGObj(sMNVSOptionsSubtitlesGObj);
            mnVSOptionsMakeSubtitle();
        }
        if
        (
            mnVSOptionsCheckGetOptionButtonInput(is_button, D_JPAD | D_CBUTTONS) ||
            mnVSOptionsCheckGetOptionStickInputUD(stick_range, -20, 0)
        )
        {
            func_800269C0_275C0(nSYAudioFGMMenuScroll2);
        
            mnVSOptionsSetOptionChangeWaitN(is_button, stick_range, 7);
                
            mnVSOptionsSetOptionSpriteColors(*option_gobjss[sMNVSOptionsOption], nMNOptionTabStatusNot);
        
            if (sMNVSOptionsOption == sMNVSOptionsLastAvailableOption)
            {
                sMNVSOptionsOption = sMNVSOptionsFirstAvailableOption;
            }
            else sMNVSOptionsOption++;
                
            mnVSOptionsSetOptionSpriteColors(*option_gobjss[sMNVSOptionsOption], nMNOptionTabStatusHighlight);
        
            if (sMNVSOptionsOption == sMNVSOptionsLastAvailableOption)
            {
                sMNVSOptionsOptionChangeWait += 8;
            }
            gcEjectGObj(sMNVSOptionsSubtitlesGObj);
            mnVSOptionsMakeSubtitle();
        }
        if
        (
            mnVSOptionsCheckGetOptionButtonInput(is_button, L_JPAD | L_TRIG | L_CBUTTONS) ||
            mnVSOptionsCheckGetOptionStickInputLR(stick_range, -20, 0)
        )
        {
            switch (sMNVSOptionsOption)
            {
            case nMNVSOptionsOptionHandicap:
                if (sMNVSOptionsHandicapStatus != nSCBattleHandicapOn)
                {
                    sMNVSOptionsHandicapStatus = (sMNVSOptionsHandicapStatus == nSCBattleHandicapOff) ? nSCBattleHandicapAuto : nSCBattleHandicapOn;

                    func_800269C0_275C0(nSYAudioFGMMenuScroll1);
                    gcEjectGObj(sMNVSOptionsSubtitlesGObj);
                    mnVSOptionsMakeSubtitle();
                    mnVSOptionsSetHandicapSettings();
                }
                mnVSOptionsSetHandicapSpriteColors(sMNVSOptionsOptionGObjs[nMNVSOptionsOptionHandicap], sMNVSOptionsHandicapStatus);
                mnVSOptionsSetOptionChangeWaitN(is_button, stick_range, 7);
                break;
                
            case nMNVSOptionsOptionTeamAttack:
                if (sMNVSOptionsTeamAttackStatus == nMNOptionTabStatusOff)
                {
                    sMNVSOptionsTeamAttackStatus = nMNOptionTabStatusOn;
                
                    func_800269C0_275C0(nSYAudioFGMMenuScroll1);
                    gcEjectGObj(sMNVSOptionsSubtitlesGObj);
                    mnVSOptionsMakeSubtitle();
                }
                mnVSOptionsSetToggleSpriteColors(sMNVSOptionsOptionGObjs[nMNVSOptionsOptionTeamAttack], sMNVSOptionsTeamAttackStatus);
                mnVSOptionsSetOptionChangeWaitN(is_button, stick_range, 7);
                break;
                
            case nMNVSOptionsOptionStageSelect:
                if (sMNVSOptionsStageSelectStatus == nMNOptionTabStatusOff)
                {
                    sMNVSOptionsStageSelectStatus = nMNOptionTabStatusOn;
                    
                    func_800269C0_275C0(nSYAudioFGMMenuScroll1);
                    gcEjectGObj(sMNVSOptionsSubtitlesGObj);
                    mnVSOptionsMakeSubtitle();
                }
                mnVSOptionsSetToggleSpriteColors(sMNVSOptionsOptionGObjs[nMNVSOptionsOptionStageSelect], sMNVSOptionsStageSelectStatus);
                mnVSOptionsSetOptionChangeWaitN(is_button, stick_range, 7);
                break;
                
            case nMNVSOptionsOptionDamage:
                func_800269C0_275C0(nSYAudioFGMMenuScroll1);
                
                if (sMNVSOptionsDamage == 50)
                {
                    sMNVSOptionsDamage = 200;
                }
                else sMNVSOptionsDamage--;
                    
                mnVSOptionsSetOptionChangeWaitN(is_button, stick_range, 14);
                
                if (sMNVSOptionsDamage == 50)
                {
                    sMNVSOptionsOptionChangeWait += 8;
                }
                gcEjectGObj(sMNVSOptionsDamageGObj);
                mnVSOptionsMakeDamageDigits();
                break;
            }
        }
        if
        (
            mnVSOptionsCheckGetOptionButtonInput(is_button, R_JPAD | R_TRIG | R_CBUTTONS) ||
            mnVSOptionsCheckGetOptionStickInputLR(stick_range, 20, 1)
        )
        {
            switch (sMNVSOptionsOption)
            {
            case nMNVSOptionsOptionHandicap:
                if (sMNVSOptionsHandicapStatus != nSCBattleHandicapOff)
                {
                    sMNVSOptionsHandicapStatus = (sMNVSOptionsHandicapStatus == nSCBattleHandicapOn) ? nSCBattleHandicapAuto : nSCBattleHandicapOff;

                    func_800269C0_275C0(nSYAudioFGMMenuScroll1);
                    gcEjectGObj(sMNVSOptionsSubtitlesGObj);
                    mnVSOptionsMakeSubtitle();
                    mnVSOptionsSetHandicapSettings();
                }
                mnVSOptionsSetHandicapSpriteColors(sMNVSOptionsOptionGObjs[nMNVSOptionsOptionHandicap], sMNVSOptionsHandicapStatus);
                mnVSOptionsSetOptionChangeWaitP(is_button, stick_range, 7);
                break;
                
            case nMNVSOptionsOptionTeamAttack:
                if (sMNVSOptionsTeamAttackStatus == nMNOptionTabStatusOn)
                {
                    sMNVSOptionsTeamAttackStatus = nMNOptionTabStatusOff;
                
                    func_800269C0_275C0(nSYAudioFGMMenuScroll1);
                    gcEjectGObj(sMNVSOptionsSubtitlesGObj);
                    mnVSOptionsMakeSubtitle();
                }
                mnVSOptionsSetToggleSpriteColors(sMNVSOptionsOptionGObjs[nMNVSOptionsOptionTeamAttack], sMNVSOptionsTeamAttackStatus);
                mnVSOptionsSetOptionChangeWaitP(is_button, stick_range, 7);
                break;
                
            case nMNVSOptionsOptionStageSelect:
                if (sMNVSOptionsStageSelectStatus == nMNOptionTabStatusOn)
                {
                    sMNVSOptionsStageSelectStatus = nMNOptionTabStatusOff;
                    
                    func_800269C0_275C0(nSYAudioFGMMenuScroll1);
                    gcEjectGObj(sMNVSOptionsSubtitlesGObj);
                    mnVSOptionsMakeSubtitle();
                }
                mnVSOptionsSetToggleSpriteColors(sMNVSOptionsOptionGObjs[nMNVSOptionsOptionStageSelect], sMNVSOptionsStageSelectStatus);
                mnVSOptionsSetOptionChangeWaitP(is_button, stick_range, 7);
                break;
                
            case nMNVSOptionsOptionDamage:
                if (sMNVSOptionsDamage == 200)
                {
                    sMNVSOptionsDamage = 50;
                }
                else sMNVSOptionsDamage++;
                    
                mnVSOptionsSetOptionChangeWaitP(is_button, stick_range, 14);
                
                if (sMNVSOptionsDamage == 200)
                {
                    sMNVSOptionsOptionChangeWait += 8;
                }
                func_800269C0_275C0(nSYAudioFGMMenuScroll1);
                gcEjectGObj(sMNVSOptionsDamageGObj);
                mnVSOptionsMakeDamageDigits();
                break;
            }
        }
        if (scSubsysControllerGetPlayerTapButtons(A_BUTTON) != FALSE)
        {
            switch (sMNVSOptionsOption)
            {
            case nMNVSOptionsOptionHandicap:
                func_800269C0_275C0(nSYAudioFGMMenuScroll1);
                
                if (sMNVSOptionsHandicapStatus == nSCBattleHandicapOff)
                {
                    sMNVSOptionsHandicapStatus = nSCBattleHandicapOn;
                } 
                else if (sMNVSOptionsHandicapStatus == nSCBattleHandicapAuto)
                {
                    sMNVSOptionsHandicapStatus = nSCBattleHandicapOff;
                }
                else sMNVSOptionsHandicapStatus = nSCBattleHandicapAuto;
                
                mnVSOptionsSetHandicapSpriteColors(sMNVSOptionsOptionGObjs[nMNVSOptionsOptionHandicap], sMNVSOptionsHandicapStatus);
                gcEjectGObj(sMNVSOptionsSubtitlesGObj);
                mnVSOptionsMakeSubtitle();
                mnVSOptionsSetHandicapSettings();
                break;
                
            case nMNVSOptionsOptionTeamAttack:
                func_800269C0_275C0(nSYAudioFGMMenuScroll1);
                sMNVSOptionsTeamAttackStatus = (sMNVSOptionsTeamAttackStatus == nMNOptionTabStatusOn) ? nMNOptionTabStatusOff : nMNOptionTabStatusOn;
                
                mnVSOptionsSetToggleSpriteColors(sMNVSOptionsOptionGObjs[nMNVSOptionsOptionTeamAttack], sMNVSOptionsTeamAttackStatus);
                gcEjectGObj(sMNVSOptionsSubtitlesGObj);
                mnVSOptionsMakeSubtitle();
                break;
                
            case nMNVSOptionsOptionStageSelect:
                func_800269C0_275C0(nSYAudioFGMMenuScroll1);
                sMNVSOptionsStageSelectStatus = (sMNVSOptionsStageSelectStatus == nMNOptionTabStatusOn) ? nMNOptionTabStatusOff : nMNOptionTabStatusOn;

                mnVSOptionsSetToggleSpriteColors(sMNVSOptionsOptionGObjs[nMNVSOptionsOptionStageSelect], sMNVSOptionsStageSelectStatus);
                gcEjectGObj(sMNVSOptionsSubtitlesGObj);
                mnVSOptionsMakeSubtitle();
                break;
            }
        }
    }
}

// 0x80134504
void mnVSOptionsFuncStart(void)
{
    LBRelocSetup rl_setup;

    rl_setup.table_addr = (uintptr_t)&lLBRelocTableAddr;
    rl_setup.table_files_num = (u32)&llRelocFileCount;
    rl_setup.file_heap = NULL;
    rl_setup.file_heap_size = 0;
    rl_setup.status_buffer = sMNVSOptionsStatusBuffer;
    rl_setup.status_buffer_size = ARRAY_COUNT(sMNVSOptionsStatusBuffer);
    rl_setup.force_status_buffer = NULL;
    rl_setup.force_status_buffer_size = 0;
    
    lbRelocInitSetup(&rl_setup);
    lbRelocLoadFilesListed(dMNVSOptionsFileIDs, sMNVSOptionsFiles);
    gcMakeGObjSPAfter(0, mnVSOptionsFuncRun, 0, GOBJ_PRIORITY_DEFAULT);
    gcMakeDefaultCameraGObj(0, GOBJ_PRIORITY_DEFAULT, 100, 0, GPACK_RGBA8888(0x00, 0x00, 0x00, 0x00));
    
    mnVSOptionsInitVars();
    mnVSOptionsMakeWallpaperCamera();
    mnVSOptionsMakeLabelUnderlineCamera();
    mnVSOptionsMakeOptionCamera();
    mnVSOptionsMakeTintCamera();
    mnVSOptionsMakeDecalCamera();
    mnVSOptionsMakeWallpaper();
    mnVSOptionsMakeTint();
    mnVSOptionsMakeDecal();
    mnVSOptionsMakeLabel();
    mnVSOptionsMakeHandicapOption();
    mnVSOptionsMakeTeamAttackOption();
    mnVSOptionsMakeStageSelectOption();
    mnVSOptionsMakeDamageOption();
    mnVSOptionsMakeDamageDigits();
    
    if (sMNVSOptionsIsHaveItemSwitch != FALSE)
    {
        mnVSOptionsMakeItemSwitchOption();
    }
    mnVSOptionsMakeUnderline();
    mnVSOptionsMakeSubtitle();
}

// 0x80134810
SYVideoSetup dMNVSOptionsVideoSetup = SYVIDEO_SETUP_DEFAULT();

// 0x8013482C
SYTaskmanSetup dMNVSOptionsTaskmanSetup =
{
    // Task Manager Buffer Setup
    {
        0,                          // ???
        gcRunAll,              		// Update function
        gcDrawAll,                  // Frame draw function
        &ovl20_BSS_END,             // Allocatable memory pool start
        0,                          // Allocatable memory pool size
        1,                          // ???
        2,                          // Number of contexts?
        sizeof(Gfx) * 7500,         // Display List Buffer 0 Size
        0,                          // Display List Buffer 1 Size
        0,                          // Display List Buffer 2 Size
        0,                          // Display List Buffer 3 Size
        0x8000,                     // Graphics Heap Size
        2,                          // ???
        0xC000,                     // RDP Output Buffer Size
        mnVSOptionsFuncLights,      // Pre-render function
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
    NULL,                           // Matrix function list
    NULL,                           // DObjVec eject function
    0,                              // Number of AObjs
    0,                              // Number of MObjs
    0,                              // Number of DObjs
    sizeof(DObj),                   // DObj size
    0,                              // Number of SObjs
    sizeof(SObj),                   // SObj size
    0,                              // Number of CObjs
    sizeof(CObj),                 	// CObj size
    
    mnVSOptionsFuncStart            // Task start function
};

// 0x80134668
void mnVSOptionsStartScene(void)
{
    dMNVSOptionsVideoSetup.zbuffer = SYVIDEO_ZBUFFER_START(320, 240, 0, 10, u16);
    syVideoInit(&dMNVSOptionsVideoSetup);
    
    dMNVSOptionsTaskmanSetup.scene_setup.arena_size = (size_t) ((uintptr_t)&ovl1_VRAM - (uintptr_t)&ovl20_BSS_END);
    syTaskmanStartTask(&dMNVSOptionsTaskmanSetup);
}
