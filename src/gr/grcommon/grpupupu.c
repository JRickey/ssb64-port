#include <gr/ground.h>
#include <ft/fighter.h>
#include <ef/effect.h>
#include <sc/scene.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//          ENUMERATORS          //
//                               //
// // // // // // // // // // // //

enum grPupupuWhispyWindStatus
{
	nGRPupupuWhispyWindStatusSleep,
	nGRPupupuWhispyWindStatusWait,
	nGRPupupuWhispyWindStatusTurn,
	nGRPupupuWhispyWindStatusOpen,
	nGRPupupuWhispyWindStatusBlow,
	nGRPupupuWhispyWindStatusStop,
    nGRPupupuWhispyWindStatusEnumCount
};

enum grPupupuWhispyMouthStatus
{
	nGRPupupuWhispyMouthStatusStretch,
	nGRPupupuWhispyMouthStatusTurn,
	nGRPupupuWhispyMouthStatusOpen,
	nGRPupupuWhispyMouthStatusClose,
    nGRPupupuWhispyMouthStatusEnumCount
};

enum grPupupuWhispyMouthTexture
{
    nGRPupupuWhispyMouthTextureOpen,
    nGRPupupuWhispyMouthTextureBlow,
    nGRPupupuWhispyMouthTextureClose,
    nGRPupupuWhispyMouthTextureEnumCount
};

enum grPupupuWhispyEyesStatus
{
	nGRPupupuWhispyEyesStatusTurn,
	nGRPupupuWhispyEyesStatusBlink,
    nGRPupupuWhispyEyesStatusEnumCount
};

enum grPupupuWhispyEyesTexture
{
    nGRPupupuWhispyEyesTexture0,
    nGRPupupuWhispyEyesTexture1,
    nGRPupupuWhispyEyesTexture2,
    nGRPupupuWhispyEyesTextureEnumCount
};

enum grPupupuFlowerStatus
{
	nGRPupupuFlowerStatusDefault,
	nGRPupupuFlowerStatusWindStart,
	nGRPupupuFlowerStatusWindLoopStart,
	nGRPupupuFlowerStatusWindLoop,
	nGRPupupuFlowerStatusWindLoopEnd,
	nGRPupupuFlowerStatusWindStop,
    nGRPupupuFlowerStatusEnumCount
};

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x8012E870
intptr_t dGRPupupuWhispyEyesAnims[/* */][nGRPupupuWhispyEyesStatusEnumCount][2] =
{
    { { &llGRPupupuMapWhispyEyesLeftTurnAnimJoint, &llGRPupupuMapWhispyEyesLeftTurnMatAnimJoint }, { &llGRPupupuMapWhispyEyesLeftBlinkAnimJoint, 0x0 } },
    { { &llGRPupupuMapWhispyEyesRightTurnAnimJoint, &llGRPupupuMapWhispyEyesRightTurnMatAnimJoint }, { &llGRPupupuMapWhispyEyesRightBlinkAnimJoint, 0x0 } }
};

// 0x8012E890
intptr_t dGRPupupuWhispyMouthAnims[/* */][nGRPupupuWhispyMouthStatusEnumCount][2] =
{
    // Left-facing
    {
        { &llGRPupupuMapWhispyMouthLeftStretchAnimJoint, &llGRPupupuMapWhispyMouthLeftStretchMatAnimJoint },
        { &llGRPupupuMapWhispyMouthLeftTurnAnimJoint, &llGRPupupuMapWhispyMouthLeftTurnMatAnimJoint },
        { &llGRPupupuMapWhispyMouthLeftOpenAnimJoint, &llGRPupupuMapWhispyMouthLeftOpenMatAnimJoint },
        { &llGRPupupuMapWhispyMouthLeftCloseAnimJoint, &llGRPupupuMapWhispyMouthLeftCloseMatAnimJoint }
    },

    // Right-facing
    {
        { &llGRPupupuMapWhispyMouthRightStretchAnimJoint, &llGRPupupuMapWhispyMouthRightStretchMatAnimJoint },
        { &llGRPupupuMapWhispyMouthRightTurnAnimJoint, &llGRPupupuMapWhispyMouthRightTurnMatAnimJoint },
        { &llGRPupupuMapWhispyMouthRightOpenAnimJoint, &llGRPupupuMapWhispyMouthRightOpenMatAnimJoint },
        { &llGRPupupuMapWhispyMouthRightCloseAnimJoint, &llGRPupupuMapWhispyMouthRightCloseMatAnimJoint }
    }
};

// 0x8012E8D0
intptr_t dGRPupupuWhispyMouthTextures[/* */][nGRPupupuWhispyMouthTextureEnumCount] =
{
    { &llGRPupupuMapWhispyMouthLeftOpenTexture, &llGRPupupuMapWhispyMouthLeftBlowTexture, &llGRPupupuMapWhispyMouthLeftCloseTexture },
    { &llGRPupupuMapWhispyMouthRightOpenTexture, &llGRPupupuMapWhispyMouthRightBlowTexture, &llGRPupupuMapWhispyMouthRightCloseTexture }
};

// 0x8012E8E8
intptr_t dGRPupupuWhispyEyesTextures[/* */][nGRPupupuWhispyEyesTextureEnumCount] =
{
    { &llGRPupupuMapWhispyEyesLeft0Texture, &llGRPupupuMapWhispyEyesLeft1Texture, &llGRPupupuMapWhispyEyesLeft2Texture },
    { &llGRPupupuMapWhispyEyesRight0Texture, &llGRPupupuMapWhispyEyesRight1Texture, &llGRPupupuMapWhispyEyesRight2Texture }
};

// 0x8012E900
GRPupupuEffect dGRPupupuWhispyLeavesEffectAttributes[/* */] =
{
    { { -715.0F, 450.0F, -696.0F }, F_CLC_DTOR32(-157.0F) },
    { { -205.0F, 450.0F, -762.0F }, F_CLC_DTOR32( -13.0F) }
};

// 0x8012E920
Vec3f dGRPupupuWhispyDustEffectPositions[/* */] =
{
    { -715.0F, 100.0F, 0.0F },
    { -205.0F, 100.0F, 0.0F }
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x801058E0
s32 grPupupuWhispyGetLR(GObj *ground_gobj)
{
    s32 players_rside = 0;
    s32 players_lside = 0;
    GObj *fighter_gobj = gGCCommonLinks[nGCCommonLinkIDFighter];

    while (fighter_gobj != NULL)
    {
        FTStruct *fp = ftGetStruct(fighter_gobj);

        if (fp->joints[nFTPartsJointTopN]->translate.vec.f.x > GRPUPUPU_WHISPY_POS_X)
        {
            players_rside++;
        }
        else players_lside++;

        fighter_gobj = fighter_gobj->link_next;
    }
    if (players_rside == players_lside)
    {
        return -1;
    }
    else if (players_rside < players_lside)
    {
        return 0;
    }
    else return 1;
}

// 0x8010595C
void grPupupuWhispySetWindPush(void)
{
    GObj *fighter_gobj = gGCCommonLinks[nGCCommonLinkIDFighter];
    sb32 lr_wind = gGRCommonStruct.pupupu.lr_players;

    while (fighter_gobj != NULL)
    {
        FTStruct *fp = ftGetStruct(fighter_gobj);
        DObj *joint = fp->joints[nFTPartsJointTopN];
        f32 dist_x;
        Vec3f push;
        f32 pos_x = joint->translate.vec.f.x, pos_y = joint->translate.vec.f.y;

        if ((pos_y <= GRPUPUPU_WHISPY_WINDBOX_TOP) && (pos_y >= GRPUPUPU_WHISPY_WINDBOX_BOTTOM))
        {
            if
            (
                ((lr_wind == 0) && (pos_x >= GRPUPUPU_WHISPY_WINDBOX_EDGELEFT)  && (pos_x <= GRPUPUPU_WHISPY_POS_X)) ||
                ((lr_wind != 0) && (pos_x <= GRPUPUPU_WHISPY_WINDBOX_EDGERIGHT) && (pos_x >= GRPUPUPU_WHISPY_POS_X))
            )
            {
                dist_x = ((pos_x < GRPUPUPU_WHISPY_POS_X) ? -(pos_x - GRPUPUPU_WHISPY_POS_X) : (pos_x - GRPUPUPU_WHISPY_POS_X));

                push.x = GRPUPUPU_WHISPY_WIND_VEL_BASE - (dist_x * GRPUPUPU_WHISPY_WIND_DIST_DECAY);

                if (push.x > 0.0F)
                {
                    push.z = 0.0F;
                    push.y = 0.0F;

                    push.x = (lr_wind == 0) ? -push.x : push.x;

                    ftParamSetVelPush(fighter_gobj, &push);
                }
            }
        }
        fighter_gobj = fighter_gobj->link_next;
    }
}

// 0x80105AF0
void grPupupuWhispyUpdateSleep(void)
{
    if (gSCManagerBattleState->game_status != nSCBattleGameStatusWait)
    {
        gGRCommonStruct.pupupu.whispy_status = nGRPupupuWhispyWindStatusWait;
    }
}

// 0x80105B18
void grPupupuWhispyLeavesMakeEffect(void)
{
    LBParticle *pc;
    LBTransform *xf;

    xf = NULL;
    pc = lbParticleMakeScriptID(gGRCommonStruct.pupupu.particle_bank_id | LBPARTICLE_MASK_GENLINK(0), 0);

    if (pc != NULL)
    {
        xf = lbParticleAddTransformForStruct(pc, nLBTransformStatusReady);

        if (xf == NULL)
        {
            lbParticleEjectStruct(pc);
        }
        else
        {
            LBParticleProcessStruct(pc);

            if (xf->users_num == 0)
            {
                xf = NULL;
            }
            else
            {
                xf->translate = dGRPupupuWhispyLeavesEffectAttributes[gGRCommonStruct.pupupu.lr_players].pos;
                xf->rotate.y = dGRPupupuWhispyLeavesEffectAttributes[gGRCommonStruct.pupupu.lr_players].rotate;
            }
        }
    }
    gGRCommonStruct.pupupu.leaves_xf = xf;
}

// 0x80105BE8
void grPupupuWhispyUpdateWait(void)
{
    if (gGRCommonStruct.pupupu.whispy_wind_wait != 0)
    {
        gGRCommonStruct.pupupu.whispy_wind_wait--;
    }
    else
    {
        s32 lr = grPupupuWhispyGetLR(gGRCommonStruct.pupupu.map_gobj[1]);

        if (lr == -1) // -1 = Both sides of the map have an equal number of players
        {
            lr = gGRCommonStruct.pupupu.lr_players;
        }
        if (lr != gGRCommonStruct.pupupu.lr_players)
        {
            gGRCommonStruct.pupupu.whispy_eyes_status = nGRPupupuWhispyEyesStatusTurn;
            gGRCommonStruct.pupupu.whispy_mouth_status = nGRPupupuWhispyMouthStatusTurn;
            gGRCommonStruct.pupupu.whispy_status = nGRPupupuWhispyWindStatusTurn;
            gGRCommonStruct.pupupu.lr_players = lr;
        }
        else
        {
            gGRCommonStruct.pupupu.whispy_mouth_status = nGRPupupuWhispyMouthStatusOpen;
            gGRCommonStruct.pupupu.whispy_status = nGRPupupuWhispyWindStatusOpen;
        }
    }
}

// 0x80105C70
void grPupupuWhispyUpdateTurn(void)
{
    if (gGRCommonStruct.pupupu.map_gobj[1]->anim_frame <= 0.0F)
    {
        gGRCommonStruct.pupupu.whispy_mouth_status = nGRPupupuWhispyMouthStatusOpen;
        gGRCommonStruct.pupupu.whispy_status = nGRPupupuWhispyWindStatusOpen;
    }
}

// 0x80105CAC
void grPupupuWhispyUpdateOpen(void)
{
    if (gGRCommonStruct.pupupu.map_gobj[1]->anim_frame <= 0.0F)
    {
        gGRCommonStruct.pupupu.whispy_status = nGRPupupuWhispyWindStatusBlow;

        gGRCommonStruct.pupupu.flowers_back_status = gGRCommonStruct.pupupu.flowers_front_status = nGRPupupuFlowerStatusWindStart;

        gGRCommonStruct.pupupu.whispy_wind_duration = syUtilsRandIntRange(GRPUPUPU_WHISPY_WIND_DURATION_RANDOM) + GRPUPUPU_WHISPY_WIND_DURATION_BASE;

        gGRCommonStruct.pupupu.rumble_wait = 0;

        grPupupuWhispyLeavesMakeEffect();

        func_800269C0_275C0(nSYAudioFGMPupupuWhispyWind);
    }
}

// 0x80105D20
void grPupupuWhispyUpdateWindRumble(void)
{
    if (gGRCommonStruct.pupupu.rumble_wait == 0)
    {
        efManagerQuakeMakeEffect(0);

        gGRCommonStruct.pupupu.rumble_wait = GRPUPUPU_WHISPY_WIND_RUMBLE_WAIT;
    }
    gGRCommonStruct.pupupu.rumble_wait--;
}

// 0x80105D6C
void grPupupuWhispyUpdateBlow(void)
{
    gGRCommonStruct.pupupu.whispy_wind_duration--;

    if (gGRCommonStruct.pupupu.whispy_wind_duration == 0)
    {
        gGRCommonStruct.pupupu.whispy_mouth_status = nGRPupupuWhispyMouthStatusClose;

        gGRCommonStruct.pupupu.flowers_back_status = gGRCommonStruct.pupupu.flowers_front_status = nGRPupupuFlowerStatusWindLoopEnd;

        gGRCommonStruct.pupupu.whispy_status = nGRPupupuWhispyWindStatusStop;

        if (gGRCommonStruct.pupupu.leaves_xf != NULL)
        {
            lbParticleEjectStructID(gGRCommonStruct.pupupu.leaves_xf->generator_id, 1);
        }
    }
    grPupupuWhispyUpdateWindRumble();
}

// 0x80105DD8
void grPupupuWhispyUpdateStop(void)
{
    if (gGRCommonStruct.pupupu.map_gobj[1]->anim_frame <= 0.0F)
    {
        gGRCommonStruct.pupupu.whispy_wind_wait = syUtilsRandIntRange(GRPUPUPU_WHISPY_WAIT_DURATION_RANDOM) + GRPUPUPU_WHISPY_WAIT_DURATION_BASE;
        gGRCommonStruct.pupupu.whispy_status = nGRPupupuWhispyWindStatusWait;
    }
}

// 0x80105E34
void grPupupuWhispyUpdateBlink(void)
{
    if ((gGRCommonStruct.pupupu.whispy_eyes_status == -1) && (gGRCommonStruct.pupupu.map_gobj[0]->anim_frame <= 0.0F))
    {
        gGRCommonStruct.pupupu.whispy_blink_wait--;

        if ((gGRCommonStruct.pupupu.whispy_blink_wait == 0) || (gGRCommonStruct.pupupu.whispy_blink_wait == -10))
        {
            gGRCommonStruct.pupupu.whispy_eyes_status = nGRPupupuWhispyEyesStatusBlink;

            if ((gGRCommonStruct.pupupu.map_gobj[1]->anim_frame <= 0.0F) && (gGRCommonStruct.pupupu.whispy_status != nGRPupupuWhispyWindStatusBlow))
            {
                gGRCommonStruct.pupupu.whispy_mouth_status = nGRPupupuWhispyMouthStatusStretch;
            }
            if (gGRCommonStruct.pupupu.whispy_blink_wait != 0)
            {
                gGRCommonStruct.pupupu.whispy_blink_wait = syUtilsRandIntRange(GRPUPUPU_WHISPY_BLINK_WAIT_RANDOM) + GRPUPUPU_WHISPY_BLINK_WAIT_BASE;
            }
        }
    }
}

// 0x80105EF4
void grPupupuUpdateWhispyStatus(void)
{
    switch (gGRCommonStruct.pupupu.whispy_status)
    {
    case nGRPupupuWhispyWindStatusSleep:
        grPupupuWhispyUpdateSleep();
        break;

    case nGRPupupuWhispyWindStatusWait:
        grPupupuWhispyUpdateWait();
        break;

    case nGRPupupuWhispyWindStatusTurn:
        grPupupuWhispyUpdateTurn();
        break;

    case nGRPupupuWhispyWindStatusOpen:
        grPupupuWhispyUpdateOpen();
        break;

    case nGRPupupuWhispyWindStatusBlow:
        grPupupuWhispyUpdateBlow();
        break;

    case nGRPupupuWhispyWindStatusStop:
        grPupupuWhispyUpdateStop();
        break;
    }
    grPupupuWhispyUpdateBlink();
}

// 0x80105F94
void grPupupuFlowersBackWindStart(void)
{
    gGRCommonStruct.pupupu.flowers_back_wait--;

    if (gGRCommonStruct.pupupu.flowers_back_wait == 0)
    {
        gGRCommonStruct.pupupu.whispy_mouth_texture = 0;
        gGRCommonStruct.pupupu.flowers_back_status = nGRPupupuFlowerStatusWindLoopStart;
    }
}

// 0x80105FC4
void grPupupuFlowersBackLoopStart(void)
{
    if (gGRCommonStruct.pupupu.map_gobj[2]->anim_frame <= 0.0F)
    {
        gGRCommonStruct.pupupu.whispy_mouth_texture = 1;
        gGRCommonStruct.pupupu.flowers_back_status = nGRPupupuFlowerStatusWindLoop;
        gGRCommonStruct.pupupu.flowers_back_wait = 15;
    }
}

// 0x80106008
void grPupupuFlowersBackLoopEnd(void)
{
    gGRCommonStruct.pupupu.flowers_back_wait--;

    if (gGRCommonStruct.pupupu.flowers_back_wait == 0)
    {
        gGRCommonStruct.pupupu.whispy_mouth_texture = 2;
        gGRCommonStruct.pupupu.flowers_back_status = nGRPupupuFlowerStatusWindStop;
        gGRCommonStruct.pupupu.flowers_back_wait = 15;
    }
}

// 0x80106044
void grPupupuFlowersBackUpdateAll(void)
{
    switch (gGRCommonStruct.pupupu.flowers_back_status)
    {
    case nGRPupupuFlowerStatusWindStart:
        grPupupuFlowersBackWindStart();
        break;

    case nGRPupupuFlowerStatusWindLoopStart:
        grPupupuFlowersBackLoopStart();
        break;

    case nGRPupupuFlowerStatusWindLoopEnd:
        grPupupuFlowersBackLoopEnd();
        break;
    }
}

// 0x801060B0
void grPupupuFlowersFrontWindStart(void)
{
    gGRCommonStruct.pupupu.flowers_front_wait--;

    if (gGRCommonStruct.pupupu.flowers_front_wait == 0)
    {
        gGRCommonStruct.pupupu.whispy_eyes_texture = 0;
        gGRCommonStruct.pupupu.flowers_front_status = nGRPupupuFlowerStatusWindLoopStart;
    }
}

// 0x801060E0
void grPupupuWhispyDustMakeEffect(void)
{
    LBParticle *pc;
    LBTransform *xf;

    xf = NULL;
    pc = lbParticleMakeScriptID(gGRCommonStruct.pupupu.particle_bank_id | LBPARTICLE_MASK_GENLINK(0), 1);

    if (pc != NULL)
    {
        xf = lbParticleAddTransformForStruct(pc, nLBTransformStatusReady);

        if (xf == NULL)
        {
            lbParticleEjectStruct(pc);
        }
        else
        {
            LBParticleProcessStruct(pc);

            if (xf->users_num == 0)
            {
                xf = NULL;
            }
            else
            {
                xf->translate = dGRPupupuWhispyDustEffectPositions[gGRCommonStruct.pupupu.lr_players];

                xf->rotate.y = (gGRCommonStruct.pupupu.lr_players == 1) ? 0.0F : F_CST_DTOR32(180.0F);
            }
        }
    }
    gGRCommonStruct.pupupu.dust_xf = xf;
}

// 0x801061CC
void grPupupuFlowersFrontLoopStart(void)
{
    if (gGRCommonStruct.pupupu.map_gobj[3]->anim_frame <= 0.0F)
    {
        gGRCommonStruct.pupupu.whispy_eyes_texture = 1;
        gGRCommonStruct.pupupu.flowers_front_status = nGRPupupuFlowerStatusWindLoop;
        gGRCommonStruct.pupupu.flowers_front_wait = 22;

        grPupupuWhispyDustMakeEffect();
    }
}

// 0x80106220
void grPupupuFlowersFrontLoopEnd(void)
{
    gGRCommonStruct.pupupu.flowers_front_wait--;

    if (gGRCommonStruct.pupupu.flowers_front_wait == 0)
    {
        gGRCommonStruct.pupupu.whispy_eyes_texture = 2;
        gGRCommonStruct.pupupu.flowers_front_status = nGRPupupuFlowerStatusWindStop;
        gGRCommonStruct.pupupu.flowers_front_wait = 22;

        if (gGRCommonStruct.pupupu.dust_xf != NULL)
        {
            lbParticleEjectStructID(gGRCommonStruct.pupupu.dust_xf->generator_id, 1);
        }
    }
    else grPupupuWhispySetWindPush();
}

// 0x80106290
void grPupupuFlowersFrontUpdateAll(void)
{
    switch (gGRCommonStruct.pupupu.flowers_front_status)
    {
    case nGRPupupuFlowerStatusWindStart:
        grPupupuFlowersFrontWindStart();
        break;

    case nGRPupupuFlowerStatusWindLoopStart:
        grPupupuFlowersFrontLoopStart();
        break;

    case nGRPupupuFlowerStatusWindLoop:
        grPupupuWhispySetWindPush();
        break;

    case nGRPupupuFlowerStatusWindLoopEnd:
        grPupupuFlowersFrontLoopEnd();
        break;
    }
}

// 0x80106314
void grPupupuUpdateGObjAnims(void)
{
    if (gGRCommonStruct.pupupu.whispy_eyes_status != -1)
    {
        intptr_t offset = dGRPupupuWhispyEyesAnims[gGRCommonStruct.pupupu.lr_players][gGRCommonStruct.pupupu.whispy_eyes_status][1];

        gcAddAnimAll
        (
            gGRCommonStruct.pupupu.map_gobj[0],
            (AObjEvent32**)
            (dGRPupupuWhispyEyesAnims[gGRCommonStruct.pupupu.lr_players][gGRCommonStruct.pupupu.whispy_eyes_status][0] + (uintptr_t)gGRCommonStruct.pupupu.map_head),
            (AObjEvent32***)
            ((offset != 0) ? (void*) ((uintptr_t)gGRCommonStruct.pupupu.map_head + offset) : NULL),
            0.0F
        );
        gcPlayAnimAll(gGRCommonStruct.pupupu.map_gobj[0]);

        gGRCommonStruct.pupupu.whispy_eyes_status = -1;
    }
    if (gGRCommonStruct.pupupu.whispy_mouth_status != -1)
    {
        gcAddAnimAll
        (
            gGRCommonStruct.pupupu.map_gobj[1],
            (AObjEvent32**)
            (dGRPupupuWhispyMouthAnims[gGRCommonStruct.pupupu.lr_players][gGRCommonStruct.pupupu.whispy_mouth_status][0] + (uintptr_t)gGRCommonStruct.pupupu.map_head),
            (AObjEvent32***)
            (dGRPupupuWhispyMouthAnims[gGRCommonStruct.pupupu.lr_players][gGRCommonStruct.pupupu.whispy_mouth_status][1] + (uintptr_t)gGRCommonStruct.pupupu.map_head),
            0.0F
        );
        gcPlayAnimAll(gGRCommonStruct.pupupu.map_gobj[1]);

        gGRCommonStruct.pupupu.whispy_mouth_status = -1;
    }
    if (gGRCommonStruct.pupupu.whispy_mouth_texture != -1)
    {
        gcAddAnimJointAll
        (
            gGRCommonStruct.pupupu.map_gobj[2],
            (AObjEvent32**)
            (dGRPupupuWhispyMouthTextures[gGRCommonStruct.pupupu.lr_players][gGRCommonStruct.pupupu.whispy_mouth_texture] + (uintptr_t)gGRCommonStruct.pupupu.map_head),
            0.0F
        );
        gcPlayAnimAll(gGRCommonStruct.pupupu.map_gobj[2]);

        gGRCommonStruct.pupupu.whispy_mouth_texture = -1;
    }
    if (gGRCommonStruct.pupupu.whispy_eyes_texture != -1)
    {
        gcAddAnimJointAll
        (
            gGRCommonStruct.pupupu.map_gobj[3],
            (AObjEvent32**)
            (dGRPupupuWhispyEyesTextures[gGRCommonStruct.pupupu.lr_players][gGRCommonStruct.pupupu.whispy_eyes_texture] + (uintptr_t)gGRCommonStruct.pupupu.map_head),
            0.0F
        );
        gcPlayAnimAll(gGRCommonStruct.pupupu.map_gobj[3]);

        gGRCommonStruct.pupupu.whispy_eyes_texture = -1;
    }
}

// 0x80106490
void grPupupuProcUpdate(GObj *ground_gobj)
{
    grPupupuUpdateWhispyStatus();
    grPupupuFlowersBackUpdateAll();
    grPupupuFlowersFrontUpdateAll();
    grPupupuUpdateGObjAnims();
}

// 0x801064C8
GObj* grPupupuMakeMapGObj(intptr_t o_dobjdesc, intptr_t o_mobjsub, void (*proc_display)(GObj*), u8 dl_link)
{
    GObj *ground_gobj = gcMakeGObjSPAfter(nGCCommonKindGround, NULL, nGCCommonLinkIDGround, GOBJ_PRIORITY_DEFAULT);

    gcAddGObjDisplay(ground_gobj, proc_display, dl_link, GOBJ_PRIORITY_DEFAULT, ~0);

    gcSetupCustomDObjs
    (
        ground_gobj,
        (DObjDesc*)
        ((uintptr_t)gGRCommonStruct.pupupu.map_head + o_dobjdesc),
        NULL,
        nGCMatrixKindTraRotRpyRSca,
        nGCMatrixKindNull,
        nGCMatrixKindNull
    );
    if (o_mobjsub != 0)
    {
        gcAddMObjAll(ground_gobj, lbRelocGetFileData(MObjSub***, gGRCommonStruct.pupupu.map_head, o_mobjsub));
    }
    gcAddGObjProcess(ground_gobj, gcPlayAnimAll, nGCProcessKindFunc, 5);

    return ground_gobj;
}

// 0x8010658C
void grPupupuInitAll(void)
{
    gGRCommonStruct.pupupu.map_head = (void*) ((uintptr_t)gMPCollisionGroundData->map_nodes - (intptr_t)&llGRPupupuMapMapHead);
    gGRCommonStruct.pupupu.map_gobj[0] = grPupupuMakeMapGObj(&llGRPupupuMapMapHead, &llGRPupupuMapWhispyEyesTransformKindsMObjSub, grDisplayLayer0PriProcDisplay, 4);
    gGRCommonStruct.pupupu.map_gobj[1] = grPupupuMakeMapGObj(&llGRPupupuMapWhispyMouthTransformKindsDObjDesc, &llGRPupupuMapWhispyMouthTransformKindsMObjSub, grDisplayLayer0PriProcDisplay, 4);
    gGRCommonStruct.pupupu.map_gobj[2] = grPupupuMakeMapGObj(&llGRPupupuMapFlowersBackTransformKindsDObjDesc, 0x0, grDisplayLayer0PriProcDisplay, 4);
    gGRCommonStruct.pupupu.map_gobj[3] = grPupupuMakeMapGObj(&llGRPupupuMapFlowersFrontTransformKindsDObjDesc, 0x0, grDisplayLayer3PriProcDisplay, 16);

    gGRCommonStruct.pupupu.whispy_eyes_status   =
    gGRCommonStruct.pupupu.whispy_mouth_status  =
    gGRCommonStruct.pupupu.whispy_mouth_texture =
    gGRCommonStruct.pupupu.whispy_eyes_texture  = -1;

    gGRCommonStruct.pupupu.whispy_status        = 0;

    gGRCommonStruct.pupupu.lr_players           = 1;

    gGRCommonStruct.pupupu.whispy_wind_wait     = syUtilsRandIntRange(GRPUPUPU_WHISPY_WAIT_DURATION_RANDOM) + GRPUPUPU_WHISPY_WAIT_DURATION_BASE;
    gGRCommonStruct.pupupu.whispy_blink_wait    = syUtilsRandIntRange(GRPUPUPU_WHISPY_BLINK_WAIT_RANDOM)    + GRPUPUPU_WHISPY_BLINK_WAIT_BASE;

    gGRCommonStruct.pupupu.flowers_back_status  =
    gGRCommonStruct.pupupu.flowers_front_status = 0;

    gGRCommonStruct.pupupu.flowers_back_wait    = 15;
    gGRCommonStruct.pupupu.flowers_front_wait   = 22;

    gGRCommonStruct.pupupu.particle_bank_id = efParticleGetLoadBankID(&lGRPupupuParticleScriptBankLo, &lGRPupupuParticleScriptBankHi, &lGRPupupuParticleTextureBankLo, &lGRPupupuParticleTextureBankHi);
}

// 0x801066D4
GObj* grPupupuMakeGround(void)
{
    GObj *ground_gobj = gcMakeGObjSPAfter(nGCCommonKindGround, NULL, nGCCommonLinkIDGround, GOBJ_PRIORITY_DEFAULT);

    gcAddGObjProcess(ground_gobj, grPupupuProcUpdate, nGCProcessKindFunc, 4);
    grPupupuInitAll();

    return ground_gobj;
}
