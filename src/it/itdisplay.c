#include <it/item.h>
#include <ft/fighter.h>
#include <sys/develop.h>
#include <sys/malloc.h>
#include <sys/matrix.h>

extern SYMallocRegion gSYTaskmanGraphicsHeap;

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80171410
void itDisplayHitCollisions(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    s32 i;
    ITAttackColl *it_attack_coll;
    ITDamageColl *damage_coll;
    Mtx *m;
    Vec3f *translate;

    it_attack_coll = &ip->attack_coll;

    for (i = 0; i < it_attack_coll->attack_count; i++)
    {
        if ((it_attack_coll->attack_state != nGMAttackStateOff) && (it_attack_coll->attack_state != nGMAttackStateNew))
        {
            gDPPipeSync(gSYTaskmanDLHeads[0]++);

            if (ip->display_mode == nDBDisplayModeHitAttackOutline)
            {
                gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0xB0, 0x00, 0x00, 0xFF);
                gDPSetEnvColor(gSYTaskmanDLHeads[0]++, 0xB0, 0x00, 0x00, 0xFF);
                gDPSetBlendColor(gSYTaskmanDLHeads[0]++, 0x00, 0x00, 0x00, 0xE0);
            }
            else
            {
                gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
                gDPSetEnvColor(gSYTaskmanDLHeads[0]++, 0xB0, 0x00, 0x00, 0xFF);
                gDPSetBlendColor(gSYTaskmanDLHeads[0]++, 0x00, 0x00, 0x00, 0x00);
            }
            if (it_attack_coll->attack_state == nGMAttackStateInterpolate)
            {
                syMatrixAdvanceW(m, gSYTaskmanGraphicsHeap);

                syMatrixTra(m, it_attack_coll->attack_pos[i].pos_prev.x, it_attack_coll->attack_pos[i].pos_prev.y, it_attack_coll->attack_pos[i].pos_prev.z);

                gSPMatrix(gSYTaskmanDLHeads[0]++, m, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);

                syMatrixAdvanceW(m, gSYTaskmanGraphicsHeap);

                syMatrixSca(m, it_attack_coll->size / 15.0F, it_attack_coll->size / 15.0F, it_attack_coll->size / 15.0F);

                gSPMatrix(gSYTaskmanDLHeads[0]++, m, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
                gSPDisplayList(gSYTaskmanDLHeads[0]++, dFTDisplayMainHitCollisionEdgeDL);
                gSPPopMatrix(gSYTaskmanDLHeads[0]++, G_MTX_MODELVIEW);
            }
            syMatrixAdvanceW(m, gSYTaskmanGraphicsHeap);

            syMatrixTra(m, it_attack_coll->attack_pos[i].pos_curr.x, it_attack_coll->attack_pos[i].pos_curr.y, it_attack_coll->attack_pos[i].pos_curr.z);

            gSPMatrix(gSYTaskmanDLHeads[0]++, m, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);

            syMatrixAdvanceW(m, gSYTaskmanGraphicsHeap);

            syMatrixSca(m, it_attack_coll->size / 15.0F, it_attack_coll->size / 15.0F, it_attack_coll->size / 15.0F);

            gSPMatrix(gSYTaskmanDLHeads[0]++, m, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);

            if (it_attack_coll->attack_state == nGMAttackStateInterpolate)
            {
                gSPDisplayList(gSYTaskmanDLHeads[0]++, dFTDisplayMainHitCollisionBlendDL);
            }
            gSPDisplayList(gSYTaskmanDLHeads[0]++, dFTDisplayMainHitCollisionCubeDL);
            gSPPopMatrix(gSYTaskmanDLHeads[0]++, G_MTX_MODELVIEW);
        }
    }
    damage_coll = &ip->damage_coll;

    if (damage_coll->hitstatus != nGMHitStatusNone)
    {
        translate = &DObjGetStruct(item_gobj)->translate.vec.f;

        syMatrixAdvanceW(m, gSYTaskmanGraphicsHeap);

        syMatrixTra(m, damage_coll->offset.x + translate->x, damage_coll->offset.y + translate->y, damage_coll->offset.z + translate->z);

        gSPMatrix(gSYTaskmanDLHeads[0]++, m, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);

        syMatrixAdvanceW(m, gSYTaskmanGraphicsHeap);

        syMatrixSca(m, damage_coll->size.x / 15.0F, damage_coll->size.y / 15.0F, damage_coll->size.z / 15.0F);

        gSPMatrix(gSYTaskmanDLHeads[0]++, m, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gDPPipeSync(gSYTaskmanDLHeads[0]++);

        switch (damage_coll->hitstatus)
        {
        case nGMHitStatusNormal:
            gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
            gDPSetEnvColor(gSYTaskmanDLHeads[0]++, 0xD0, 0xD0, 0x00, 0xFF);
            break;

        case nGMHitStatusInvincible:
            gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
            gDPSetEnvColor(gSYTaskmanDLHeads[0]++, 0x00, 0xD0, 0x00, 0xFF);
            break;

        case nGMHitStatusIntangible:
            gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
            gDPSetEnvColor(gSYTaskmanDLHeads[0]++, 0x00, 0x00, 0xD0, 0xFF);
            break;
        }
        gSPDisplayList(gSYTaskmanDLHeads[0]++, dFTDisplayMainHurtCollisionCuboidDL);
        gSPPopMatrix(gSYTaskmanDLHeads[0]++, G_MTX_MODELVIEW);
    }
}

// 0x801719AC
void itDisplayMapCollisions(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    Vec3f *translate = &DObjGetStruct(item_gobj)->translate.vec.f;
    MPObjectColl *map_coll = &ip->coll_data.map_coll;
    Mtx *m;

    gDPPipeSync(gSYTaskmanDLHeads[1]++);

    syMatrixAdvanceW(m, gSYTaskmanGraphicsHeap);

    syMatrixTra(m, translate->x, translate->y + map_coll->bottom, translate->z);

    gSPMatrix(gSYTaskmanDLHeads[1]++, m, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);

    syMatrixAdvanceW(m, gSYTaskmanGraphicsHeap);

    syMatrixSca(m, map_coll->width / 30.0F, (map_coll->center - map_coll->bottom) / 30.0F, 1.0F);

    gSPMatrix(gSYTaskmanDLHeads[1]++, m, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPDisplayList(gSYTaskmanDLHeads[1]++, dFTDisplayMainMapCollisionBottomDL);
    gSPPopMatrix(gSYTaskmanDLHeads[1]++, G_MTX_MODELVIEW);

    syMatrixAdvanceW(m, gSYTaskmanGraphicsHeap);

    syMatrixTra(m, translate->x, translate->y + map_coll->center, translate->z);

    gSPMatrix(gSYTaskmanDLHeads[1]++, m, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);

    syMatrixAdvanceW(m, gSYTaskmanGraphicsHeap);

    syMatrixSca(m, map_coll->width / 30.0F, (map_coll->top - map_coll->center) / 30.0F, 1.0F);

    gSPMatrix(gSYTaskmanDLHeads[1]++, m, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPDisplayList(gSYTaskmanDLHeads[1]++, dFTDisplayMainMapCollisionTopDL);
    gSPPopMatrix(gSYTaskmanDLHeads[1]++, G_MTX_MODELVIEW);
}

// 0x80171C10
sb32 itDisplayCheckItemVisible(ITStruct *ip)
{
    FTStruct *fp;

    if (ip->owner_gobj == NULL)
    {
        return TRUE;
    }
    else if (!(ip->is_hold))
    {
        return TRUE;
    }

    fp = ftGetStruct(ip->owner_gobj);

    if (!(fp->is_item_show))
    {
        return FALSE;
    }
    else if (fp->is_invisible)
    {
        return FALSE;
    }
    else return TRUE;
}

// 0x80171C7C
void itDisplayOPAProcDisplay(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (itDisplayCheckItemVisible(ip) != FALSE)
    {
        if ((ip->display_mode == nDBDisplayModeMaster) || (ip->is_hold))
        {
            gcDrawDObjTreeForGObj(item_gobj);
        }
        else if (ip->display_mode == nDBDisplayModeMapCollision)
        {
            gcDrawDObjTreeForGObj(item_gobj);
            itDisplayMapCollisions(item_gobj);
        }
        else if ((ip->damage_coll.hitstatus == nGMHitStatusNone) && (ip->attack_coll.attack_state == nGMAttackStateOff))
        {
            gcDrawDObjTreeForGObj(item_gobj);
        }
        else itDisplayHitCollisions(item_gobj);
    }
}

// 0x80171D38
void itDisplayXLUProcDisplay(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (itDisplayCheckItemVisible(ip) != FALSE)
    {
        if ((ip->display_mode == nDBDisplayModeMaster) || (ip->is_hold))
        {
            gcDrawDObjTreeDLLinksForGObj(item_gobj);
        }
        else if (ip->display_mode == nDBDisplayModeMapCollision)
        {
            gcDrawDObjTreeDLLinksForGObj(item_gobj);
            itDisplayMapCollisions(item_gobj);
        }
        else if ((ip->damage_coll.hitstatus == nGMHitStatusNone) && (ip->attack_coll.attack_state == nGMAttackStateOff))
        {
            gcDrawDObjTreeDLLinksForGObj(item_gobj);
        }
        else itDisplayHitCollisions(item_gobj);
    }
}

// 0x80171DF4
void itDisplayColAnimOPA(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_2CYCLE);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_PASS, G_RM_AA_ZB_OPA_SURF2);

    if (ip->colanim.is_use_color1)
    {
        gDPSetEnvColor(gSYTaskmanDLHeads[0]++, ip->colanim.color1.r, ip->colanim.color1.g, ip->colanim.color1.b, ip->colanim.color1.a);
    }
    else gDPSetEnvColor(gSYTaskmanDLHeads[0]++, 0x00, 0x00, 0x00, 0x00);
    
    gcDrawDObjTreeForGObj(item_gobj);

    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_1CYCLE);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
}

// 0x80171F4C
void itDisplayColAnimOPAProcDisplay(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (itDisplayCheckItemVisible(ip) != FALSE)
    {
        if ((ip->display_mode == nDBDisplayModeMaster) || (ip->is_hold))
        {
            itDisplayColAnimOPA(item_gobj);
        }
        else if (ip->display_mode == nDBDisplayModeMapCollision)
        {
            itDisplayColAnimOPA(item_gobj);
            itDisplayMapCollisions(item_gobj);
        }
        else if ((ip->damage_coll.hitstatus == nGMHitStatusNone) && (ip->attack_coll.attack_state == nGMAttackStateOff))
        {
            itDisplayColAnimOPA(item_gobj);
        }
        else itDisplayHitCollisions(item_gobj);
    }
}

// 0x80172008
void itDisplayColAnimXLU(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_2CYCLE);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_PASS, G_RM_AA_ZB_OPA_SURF2);
    gDPPipeSync(gSYTaskmanDLHeads[1]++);
    gDPSetCycleType(gSYTaskmanDLHeads[1]++, G_CYC_2CYCLE);
    gDPSetRenderMode(gSYTaskmanDLHeads[1]++, G_RM_PASS, G_RM_AA_ZB_XLU_SURF2);

    if (ip->colanim.is_use_color1)
    {
        gDPSetEnvColor(gSYTaskmanDLHeads[0]++, ip->colanim.color1.r, ip->colanim.color1.g, ip->colanim.color1.b, ip->colanim.color1.a);
        gDPSetEnvColor(gSYTaskmanDLHeads[1]++, ip->colanim.color1.r, ip->colanim.color1.g, ip->colanim.color1.b, ip->colanim.color1.a);
    }
    else
    {
        gDPSetEnvColor(gSYTaskmanDLHeads[0]++, 0x00, 0x00, 0x00, 0x00);
        gDPSetEnvColor(gSYTaskmanDLHeads[1]++, 0x00, 0x00, 0x00, 0x00);
    }
    gcDrawDObjTreeDLLinksForGObj(item_gobj);

    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_1CYCLE);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gDPPipeSync(gSYTaskmanDLHeads[1]++);
    gDPSetCycleType(gSYTaskmanDLHeads[1]++, G_CYC_1CYCLE);
    gDPSetRenderMode(gSYTaskmanDLHeads[1]++, G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2);
}

// 0x8017224C
void itDisplayColAnimXLUProcDisplay(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (itDisplayCheckItemVisible(ip) != FALSE)
    {
        if ((ip->display_mode == nDBDisplayModeMaster) || (ip->is_hold))
        {
            itDisplayColAnimXLU(item_gobj);
        }
        else if (ip->display_mode == nDBDisplayModeMapCollision)
        {
            itDisplayColAnimXLU(item_gobj);
            itDisplayMapCollisions(item_gobj);
        }
        else if ((ip->damage_coll.hitstatus == nGMHitStatusNone) && (ip->attack_coll.attack_state == nGMAttackStateOff))
        {
            itDisplayColAnimXLU(item_gobj);
        }
        else itDisplayHitCollisions(item_gobj);
    }
}
