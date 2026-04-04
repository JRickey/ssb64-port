#include <ft/fighter.h>
#include <wp/weapon.h>
#include <it/item.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x800ECD80
void gmCollisionCopyMatrix(Mtx44f dst, Mtx44f src)
{
    dst[0][0] = src[0][0];
    dst[1][0] = src[1][0];
    dst[2][0] = src[2][0];
    dst[3][0] = src[3][0];
    dst[0][1] = src[0][1];
    dst[1][1] = src[1][1];
    dst[2][1] = src[2][1];
    dst[3][1] = src[3][1];
    dst[0][2] = src[0][2];
    dst[1][2] = src[1][2];
    dst[2][2] = src[2][2];
    dst[3][2] = src[3][2];
}

// 0x800ECDE4
void gmCollisionTransformMatrixAll(DObj *dobj, FTParts *parts, Mtx44f mtx)
{
    Vec3f *translate = &dobj->translate.vec.f;
    Vec3f *rotate = &dobj->rotate.vec.f;
    Vec3f *scale = &dobj->scale.vec.f;
    f32 sinx, siny, sinz;
    f32 cosx, cosy, cosz;

    sinx = lbCommonSin(rotate->x);
    cosx = lbCommonCos(rotate->x);

    siny = lbCommonSin(rotate->y);
    cosy = lbCommonCos(rotate->y);

    sinz = lbCommonSin(rotate->z);
    cosz = lbCommonCos(rotate->z);

    mtx[0][0] = cosy * cosz;
    mtx[0][1] = cosy * sinz;
    mtx[0][2] = -siny;

    mtx[1][0] = (sinx * siny * cosz) - (cosx * sinz);
    mtx[1][1] = (sinx * siny * sinz) + (cosx * cosz);
    mtx[1][2] = sinx * cosy;

    mtx[2][0] = (cosx * siny * cosz) + (sinx * sinz);
    mtx[2][1] = (cosx * siny * sinz) - (sinx * cosz);
    mtx[2][2] = cosx * cosy;

    if (scale->x != 1.0F)
    {
        mtx[0][0] *= scale->x;
        mtx[0][1] *= scale->x;
        mtx[0][2] *= scale->x;
    }
    if (scale->y != 1.0F)
    {
        mtx[1][0] *= scale->y;
        mtx[1][1] *= scale->y;
        mtx[1][2] *= scale->y;
    }
    if (scale->z != 1.0F)
    {
        mtx[2][0] *= scale->z;
        mtx[2][1] *= scale->z;
        mtx[2][2] *= scale->z;
    }
    mtx[3][0] = translate->x;
    mtx[3][1] = translate->y;
    mtx[3][2] = translate->z;
}

// 0x800ECFF4
void gmCollisionSetMatrixNcs(DObj *dobj, FTParts *parts, Mtx44f mtx, Vec3f *scale_mul)
{
    Vec3f *translate = &dobj->translate.vec.f;
    Vec3f *rotate = &dobj->rotate.vec.f;
    Vec3f *scale = &dobj->scale.vec.f;
    Vec3f *nscale = &parts->vec_scale;
    f32 sinx, siny, sinz;
    f32 cosx, cosy, cosz;
    f32 invx, invy, invz;

    if (scale_mul != NULL)
    {
        nscale->x = scale_mul->x * scale->x;
        nscale->y = scale_mul->y * scale->y;
        nscale->z = scale_mul->z * scale->z;
    }
    else *nscale = *scale;

    sinx = lbCommonSin(rotate->x);
    cosx = lbCommonCos(rotate->x);

    siny = lbCommonSin(rotate->y);
    cosy = lbCommonCos(rotate->y);

    sinz = lbCommonSin(rotate->z);
    cosz = lbCommonCos(rotate->z);

    mtx[0][0] = cosy * cosz;
    mtx[0][1] = cosy * sinz;
    mtx[0][2] = -siny;

    mtx[1][0] = (sinx * siny * cosz) - (cosx * sinz);
    mtx[1][1] = (sinx * siny * sinz) + (cosx * cosz);
    mtx[1][2] = sinx * cosy;

    mtx[2][0] = (cosx * siny * cosz) + (sinx * sinz);
    mtx[2][1] = (cosx * siny * sinz) - (sinx * cosz);
    mtx[2][2] = cosx * cosy;

    if (nscale->x != 1.0F)
    {
        mtx[0][0] *= nscale->x;
        mtx[0][1] *= nscale->x;
        mtx[0][2] *= nscale->x;
    }
    if (nscale->y != 1.0F)
    {
        mtx[1][0] *= nscale->y;
        mtx[1][1] *= nscale->y;
        mtx[1][2] *= nscale->y;
    }
    if (nscale->z != 1.0F)
    {
        mtx[2][0] *= nscale->z;
        mtx[2][1] *= nscale->z;
        mtx[2][2] *= nscale->z;
    }
    if (scale_mul != NULL)
    {
        if (scale_mul->x != 1.0F)
        {
            if (scale_mul->x == 0.0F)
            {
                while (TRUE)
                {
                    syDebugPrintf("zero div x in gcSetMatrixNcs()\n");
                    scManagerRunPrintGObjStatus();
                }
            }
            invx = 1.0F / scale_mul->x;

            mtx[0][0] *= invx;
            mtx[1][0] *= invx;
            mtx[2][0] *= invx;
        }
        if (scale_mul->y != 1.0F)
        {
            if (scale_mul->y == 0.0F)
            {
                while (TRUE)
                {
                    syDebugPrintf("zero div y in gcSetMatrixNcs()\n");
                    scManagerRunPrintGObjStatus();
                }
            }
            invy = 1.0F / scale_mul->y;

            mtx[0][1] *= invy;
            mtx[1][1] *= invy;
            mtx[2][1] *= invy;
        }
        if (scale_mul->z != 1.0F)
        {
            if (scale_mul->z == 0.0F)
            {
                while (TRUE)
                {
                    syDebugPrintf("zero div z in gcSetMatrixNcs()\n");
                    scManagerRunPrintGObjStatus();
                }
            }
            invz = 1.0F / scale_mul->z;

            mtx[0][2] *= invz;
            mtx[1][2] *= invz;
            mtx[2][2] *= invz;
        }
    }
    mtx[3][0] = translate->x;
    mtx[3][1] = translate->y;
    mtx[3][2] = translate->z;
}

// 0x800ED3C0
void gmCollisionGetWorldPosition(Mtx44f mtx, Vec3f *vec)
{
    Vec3f product;

    product.x = ((mtx[0][0] * vec->x) + (mtx[1][0] * vec->y) + (mtx[2][0] * vec->z)) + mtx[3][0];
    product.y = ((mtx[0][1] * vec->x) + (mtx[1][1] * vec->y) + (mtx[2][1] * vec->z)) + mtx[3][1];
    product.z = ((mtx[0][2] * vec->x) + (mtx[1][2] * vec->y) + (mtx[2][2] * vec->z)) + mtx[3][2];

    *vec = product;
}

// 0x800ED490
void func_ovl2_800ED490(Mtx44f dst, Mtx44f lhs, Mtx44f rhs)
{
    dst[0][0] = (lhs[0][0] * rhs[0][0]) + (lhs[1][0] * rhs[0][1]) + (lhs[2][0] * rhs[0][2]);
    dst[0][1] = (lhs[0][1] * rhs[0][0]) + (lhs[1][1] * rhs[0][1]) + (lhs[2][1] * rhs[0][2]);
    dst[0][2] = (lhs[0][2] * rhs[0][0]) + (lhs[1][2] * rhs[0][1]) + (lhs[2][2] * rhs[0][2]);

    dst[1][0] = (lhs[0][0] * rhs[1][0]) + (lhs[1][0] * rhs[1][1]) + (lhs[2][0] * rhs[1][2]);
    dst[1][1] = (lhs[0][1] * rhs[1][0]) + (lhs[1][1] * rhs[1][1]) + (lhs[2][1] * rhs[1][2]);
    dst[1][2] = (lhs[0][2] * rhs[1][0]) + (lhs[1][2] * rhs[1][1]) + (lhs[2][2] * rhs[1][2]);

    dst[2][0] = (lhs[0][0] * rhs[2][0]) + (lhs[1][0] * rhs[2][1]) + (lhs[2][0] * rhs[2][2]);
    dst[2][1] = (lhs[0][1] * rhs[2][0]) + (lhs[1][1] * rhs[2][1]) + (lhs[2][1] * rhs[2][2]);
    dst[2][2] = (lhs[0][2] * rhs[2][0]) + (lhs[1][2] * rhs[2][1]) + (lhs[2][2] * rhs[2][2]);

    dst[3][0] = ((lhs[0][0] * rhs[3][0]) + (lhs[1][0] * rhs[3][1]) + (lhs[2][0] * rhs[3][2])) + lhs[3][0];
    dst[3][1] = ((lhs[0][1] * rhs[3][0]) + (lhs[1][1] * rhs[3][1]) + (lhs[2][1] * rhs[3][2])) + lhs[3][1];
    dst[3][2] = ((lhs[0][2] * rhs[3][0]) + (lhs[1][2] * rhs[3][1]) + (lhs[2][2] * rhs[3][2])) + lhs[3][2];
}

// 0x800ED6EC
void gmCollisionSetInvertMatrix(Mtx44f dst, Mtx44f src)
{
    f32 scale;

    dst[0][0] = (src[1][1] * src[2][2]) - (src[1][2] * src[2][1]);
    dst[1][0] = (src[1][0] * src[2][2]) - (src[1][2] * src[2][0]);
    dst[2][0] = (src[1][0] * src[2][1]) - (src[1][1] * src[2][0]);
    dst[3][0] = (src[3][0] * dst[0][0]) - (src[3][1] * dst[1][0]) + (src[3][2] * dst[2][0]);

    dst[0][1] = (src[0][1] * src[2][2]) - (src[0][2] * src[2][1]);
    dst[1][1] = (src[0][0] * src[2][2]) - (src[0][2] * src[2][0]);
    dst[2][1] = (src[0][0] * src[2][1]) - (src[0][1] * src[2][0]);
    dst[3][1] = (src[3][0] * dst[0][1]) - (src[3][1] * dst[1][1]) + (src[3][2] * dst[2][1]);

    dst[0][2] = (src[0][1] * src[1][2]) - (src[0][2] * src[1][1]);
    dst[1][2] = (src[0][0] * src[1][2]) - (src[0][2] * src[1][0]);
    dst[2][2] = (src[0][0] * src[1][1]) - (src[0][1] * src[1][0]);
    dst[3][2] = (src[3][0] * dst[0][2]) - (src[3][1] * dst[1][2]) + (src[3][2] * dst[2][2]);

    scale = (src[0][0] * dst[0][0]) - (src[0][1] * dst[1][0]) + (src[0][2] * dst[2][0]);

    dst[1][0] = -dst[1][0];
    dst[3][0] = -dst[3][0];
    dst[0][1] = -dst[0][1];
    dst[2][1] = -dst[2][1];
    dst[1][2] = -dst[1][2];
    dst[3][2] = -dst[3][2];

    if (scale == 0.0F)
    {
        while (TRUE)
        {
            syDebugPrintf("zero div in gcSetInvMatrix()\n");
            scManagerRunPrintGObjStatus();
        }
    }
    scale = 1.0F / scale;

    dst[0][0] *= scale;
    dst[1][0] *= scale;
    dst[2][0] *= scale;
    dst[3][0] *= scale;
    dst[0][1] *= scale;
    dst[1][1] *= scale;
    dst[2][1] *= scale;
    dst[3][1] *= scale;
    dst[0][2] *= scale;
    dst[1][2] *= scale;
    dst[2][2] *= scale;
    dst[3][2] *= scale;
}

// 0x800EDA0C
void func_ovl2_800EDA0C(Mtx44f mtx, Vec3f *rotate)
{
    Mtx44f dst;
    f32 scale;
    s32 i;

    // i < 3
    for (i = 0; i < 3; i++)
    {
        scale = sqrtf(SQUARE(mtx[i][0]) + SQUARE(mtx[i][1]) + SQUARE(mtx[i][2]));

        if (scale != 0.0F)
        {
            scale = 1.0F / scale;
        }
        dst[i][0] = mtx[i][0] * scale;
        dst[i][1] = mtx[i][1] * scale;
        dst[i][2] = mtx[i][2] * scale;
    }
    if ((dst[0][2] == -1.0F) || (dst[0][2] == 1.0F))
    {
        if (dst[0][2] == -1.0F)
        {
            rotate->y = F_CLC_DTOR32(90.0F);
            rotate->x = syUtilsArcTan2(dst[1][0], dst[1][1]);

        }
        else
        {
            rotate->y = F_CLC_DTOR32(-90.0F);
            rotate->x = syUtilsArcTan2(-dst[1][0], dst[1][1]);
        }
        rotate->z = 0;
    }
    else
    {
        rotate->y = syUtilsArcSin(-dst[0][2]);
        rotate->x = syUtilsArcTan2(dst[1][2], dst[2][2]);
        rotate->z = syUtilsArcTan2(dst[0][1], dst[0][0]);
    }
}

// 0x800EDB88
void func_ovl2_800EDB88(Mtx44f mtx, Vec3f *vec)
{
    vec->x = mtx[3][0];
    vec->y = mtx[3][1];
    vec->z = mtx[3][2];
}

// 0x800EDBA4
void func_ovl2_800EDBA4(DObj *main_dobj)
{
    DObj *current_dobj;
    FTParts *parts;
    FTParts *current_dobjdata;
    DObj *setup_dobj[18];
    DObj *second_dobj;
    u32 flag = ftGetStruct(main_dobj->parent_gobj)->is_use_animlocks;
    s32 i;

    if (flag == FALSE)
    {
        second_dobj = main_dobj;

        i = 0;

        while (TRUE)
        {
            parts = ftGetParts(second_dobj);

            if (parts->unk_dobjtrans_0x5 != 0)
            {
                break;
            }
            else if (second_dobj->parent == DOBJ_PARENT_NULL)
            {
                if (parts->transform_update_mode == 0)
                {
                    gmCollisionTransformMatrixAll(second_dobj, parts, parts->unk_dobjtrans_0x10);

                    parts->transform_update_mode = 1;
                }
                gmCollisionCopyMatrix(parts->mtx_translate, parts->unk_dobjtrans_0x10);

                parts->unk_dobjtrans_0x5 = 1;

                break;
            }
            else
            {
                setup_dobj[i] = second_dobj;
                second_dobj = second_dobj->parent;

                i++;
            }
        }
        for (i = i - 1; i >= 0; i--)
        {
            second_dobj = setup_dobj[i];

            current_dobjdata = ftGetParts(second_dobj);

            if (current_dobjdata->transform_update_mode == 0)
            {
                gmCollisionTransformMatrixAll(second_dobj, current_dobjdata, current_dobjdata->unk_dobjtrans_0x10);

                current_dobjdata->transform_update_mode = 1;
            }
            func_ovl2_800ED490(current_dobjdata->mtx_translate, parts->mtx_translate, current_dobjdata->unk_dobjtrans_0x10);

            current_dobjdata->unk_dobjtrans_0x5 = 1;
            parts = current_dobjdata;
        }
    }
    else
    {
        second_dobj = main_dobj;

        i = 0;

        while (TRUE)
        {
            parts = ftGetParts(second_dobj);

            if (parts->unk_dobjtrans_0x5 != 0)
            {
                break;
            }
            else if (second_dobj->parent == DOBJ_PARENT_NULL)
            {
                if (parts->transform_update_mode == 0)
                {
                    gmCollisionSetMatrixNcs(second_dobj, parts, parts->unk_dobjtrans_0x10, NULL);

                    parts->transform_update_mode = 1;
                    parts->unk_dobjtrans_0x6 = 1;
                }
                gmCollisionCopyMatrix(parts->mtx_translate, parts->unk_dobjtrans_0x10);

                parts->unk_dobjtrans_0x5 = 1;

                break;
            }
            else
            {
                setup_dobj[i] = second_dobj;
                second_dobj = second_dobj->parent;

                i++;
            }
        }
        for (i = i - 1; i >= 0; i--)
        {
            second_dobj = setup_dobj[i];

            current_dobjdata = ftGetParts(second_dobj);

            if (current_dobjdata->transform_update_mode == 0)
            {
                gmCollisionSetMatrixNcs(second_dobj, current_dobjdata, current_dobjdata->unk_dobjtrans_0x10, &parts->vec_scale);

                current_dobjdata->transform_update_mode = 1;
                current_dobjdata->unk_dobjtrans_0x6 = 1;
            }
            func_ovl2_800ED490(current_dobjdata->mtx_translate, parts->mtx_translate, current_dobjdata->unk_dobjtrans_0x10);

            current_dobjdata->unk_dobjtrans_0x5 = 1;
            parts = current_dobjdata;
        }
    }
}

// 0x800EDE00
void func_ovl2_800EDE00(DObj *main_dobj)
{
    FTParts *parts = ftGetParts(main_dobj);

    if (parts->unk_dobjtrans_0x7 == 0)
    {
        if (parts->unk_dobjtrans_0x5 == 0)
        {
            func_ovl2_800EDBA4(main_dobj);
        }
        gmCollisionSetInvertMatrix(parts->unk_dobjtrans_0x9C, parts->mtx_translate);

        parts->unk_dobjtrans_0x7 = 1;
    }
}

// 0x800EDE5C
void func_ovl2_800EDE5C(DObj *main_dobj)
{
    FTParts *parts = ftGetParts(main_dobj);

    if (parts->unk_dobjtrans_0x6 == 0)
    {
        if (parts->unk_dobjtrans_0x5 == 0)
        {
            func_ovl2_800EDBA4(main_dobj);
        }
        parts->vec_scale.x = sqrtf(SQUARE(parts->mtx_translate[0][0]) + SQUARE(parts->mtx_translate[0][1]) + SQUARE(parts->mtx_translate[0][2]));
        parts->vec_scale.y = sqrtf(SQUARE(parts->mtx_translate[1][0]) + SQUARE(parts->mtx_translate[1][1]) + SQUARE(parts->mtx_translate[1][2]));
        parts->vec_scale.z = sqrtf(SQUARE(parts->mtx_translate[2][0]) + SQUARE(parts->mtx_translate[2][1]) + SQUARE(parts->mtx_translate[2][2]));

        parts->unk_dobjtrans_0x6 = 1;
    }
}

// 0x800EDF24
void gmCollisionGetFighterPartsWorldPosition(DObj *main_dobj, Vec3f *vec)
{
    FTParts *parts;
    u32 flag = ftGetStruct(main_dobj->parent_gobj)->is_use_animlocks;

    if (flag == FALSE)
    {
        while (main_dobj != DOBJ_PARENT_NULL)
        {
            parts = ftGetParts(main_dobj);

            if (parts->unk_dobjtrans_0x5 != 0)
            {
                gmCollisionGetWorldPosition(parts->mtx_translate, vec);

                return;
            }
            else if (parts->transform_update_mode == 0)
            {
                gmCollisionTransformMatrixAll(main_dobj, parts, parts->unk_dobjtrans_0x10);

                parts->transform_update_mode = 1;
            }
            gmCollisionGetWorldPosition(parts->unk_dobjtrans_0x10, vec);

            main_dobj = main_dobj->parent;
        }
    }
    else
    {
        parts = ftGetParts(main_dobj);

        if (parts->unk_dobjtrans_0x5 == 0)
        {
            func_ovl2_800EDBA4(main_dobj);
        }
        gmCollisionGetWorldPosition(parts->mtx_translate, vec);
    }
}

// 0x800EE018
void func_ovl2_800EE018(DObj *main_dobj, Vec3f *vec)
{
    FTParts *parts = ftGetParts(main_dobj);

    func_ovl2_800EDE00(main_dobj);
    gmCollisionGetWorldPosition(parts->unk_dobjtrans_0x9C, vec);
}

// 0x800EE050
void func_ovl2_800EE050(s32 arg0, Vec3f *arg1, Vec3f *arg2, sb32 *arg3, Mtx44f mtx, f32 *p_scale)
{
    Vec3f dist;
    f32 unused[2];
    f32 square;
    f32 scale;
    f32 temp;

    if ((arg0 == 3) && (*arg3 == FALSE))
    {
        dist.x = arg2->x - arg1->x;
        dist.y = arg2->y - arg1->y;
        dist.z = arg2->z - arg1->z;

        *p_scale = sqrtf(SQUARE(dist.x) + SQUARE(dist.y) + SQUARE(dist.z));

        if (*p_scale != 0.0F)
        {
            scale = 1.0F / *p_scale;

            dist.x *= scale;
            dist.y *= scale;
            dist.z *= scale;

            // JUST BARELY matches; otherwise 1.0F and square are swapped in the c.eq.s instruction operands; if(TRUE) necessary (for now)
            square = SQUARE(dist.x);

            if (TRUE && square == 1.0F)
            {
                if (dist.x >= 0.0F)
                {
                    mtx[0][0] = mtx[1][1] = mtx[2][2] = 1.0F;

                    mtx[0][1] =
                    mtx[0][2] =
                    mtx[1][0] =
                    mtx[1][2] =
                    mtx[2][0] =
                    mtx[2][1] = 0.0F;
                }
                else
                {
                    mtx[0][0] = mtx[1][1] = -1.0F;

                    mtx[2][2] = 1.0F;

                    mtx[0][1] =
                    mtx[0][2] =
                    mtx[1][0] =
                    mtx[1][2] =
                    mtx[2][0] =
                    mtx[2][1] = 0.0F;
                }
            }
            else
            {
                mtx[0][0] = temp = dist.x;
                mtx[1][2] = (dist.z * dist.y) / (1.0F + dist.x);
                mtx[2][1] = (dist.z * dist.y) / (1.0F + dist.x);

                scale = (1.0F / (1.0F - SQUARE(temp)));

                mtx[1][1] = ((1.0F - (SQUARE(dist.z) * scale)) * temp) + (SQUARE(dist.z) * scale);
                mtx[2][0] = dist.z;
                mtx[0][2] = -dist.z;

                mtx[2][2] = ((1.0F - (SQUARE(dist.y) * scale)) * temp) + (SQUARE(dist.y) * scale);
                mtx[0][1] = dist.y;
                mtx[1][0] = -dist.y;
            }
            mtx[3][0] = *p_scale *= 0.5F;
            mtx[3][1] = 0.0F;
            mtx[3][2] = 0.0F;

            *arg3 = TRUE;
        }
    }
}

// 0x800EE24C
u32 func_ovl2_800EE24C(Vec3f *lhs, Vec3f *rhs)
{
    u32 flags = 0;

    if (lhs->x < -rhs->x)
    {
        flags |= 1;
    }
    if (lhs->x > rhs->x)
    {
        flags |= 2;
    }
    if (lhs->y < -rhs->y)
    {
        flags |= 4;
    }
    if (lhs->y > rhs->y)
    {
        flags |= 8;
    }
    return flags;
}

// 0x800EE2C0
u32 func_ovl2_800EE2C0(Vec3f *lhs, Vec3f *rhs)
{
    u32 flags = 0;

    if (lhs->z < -rhs->z)
    {
        flags |= 1;
    }
    if (lhs->z > rhs->z)
    {
        flags |= 2;
    }
    return flags;
}

// 0x800EE300
sb32 gmCollisionTestRectangle(Vec3f *pos_curr, Vec3f *pos_prev, f32 radius, s32 opkind, Mtx44f mtx, Vec3f *offset, Vec3f *size, Vec3f *scale)
{
    // Not sure about the variable names; help from ChatGPT
    Vec3f center;
    Vec3f sp90;
    Vec3f sp84;
    Vec3f sp78;
    Vec3f sp6C;
    u32 flags_sp78;
    u32 flags_sp6C;
    u32 flags_main;
    f32 distx;
    f32 disty;
    f32 distz;

    center.x = size->x + (radius / scale->x);
    center.y = size->y + (radius / scale->y);
    center.z = size->z + (radius / scale->z);

    if (opkind == 2)
    {
        sp90 = *pos_curr;

        if (mtx != NULL)
        {
            gmCollisionGetWorldPosition(mtx, &sp90);
        }
        sp90.x -= offset->x;
        sp90.y -= offset->y;
        sp90.z -= offset->z;

        if ((-center.x <= sp90.x) && (sp90.x <= center.x) && (-center.y <= sp90.y) && (sp90.y <= center.y) && (-center.z <= sp90.z) && (sp90.z <= center.z))
        {
            return TRUE;
        }
        else return FALSE;
    }
    sp78 = *pos_curr;
    sp6C = *pos_prev;

    if (mtx != NULL)
    {
        gmCollisionGetWorldPosition(mtx, &sp78);
        gmCollisionGetWorldPosition(mtx, &sp6C);
    }
    sp78.x -= offset->x;
    sp78.y -= offset->y;
    sp78.z -= offset->z;
    sp6C.x -= offset->x;
    sp6C.y -= offset->y;
    sp6C.z -= offset->z;

    distx = sp6C.x - sp78.x;
    disty = sp6C.y - sp78.y;
    distz = sp6C.z - sp78.z;

    flags_sp78 = func_ovl2_800EE24C(&sp78, &center);
    flags_sp6C = func_ovl2_800EE24C(&sp6C, &center);

loop:
    if ((flags_sp78 != 0) || (flags_sp6C != 0))
    {
        if (flags_sp78 & flags_sp6C)
        {
            return FALSE;
        }
        else if (flags_sp78 != 0)
        {
            flags_main = flags_sp78;
        }
        else flags_main = flags_sp6C;

        if (flags_main & 1)
        {
            sp84.x = -center.x;
            sp84.y = (((sp84.x - sp78.x) / distx) * disty) + sp78.y;
            sp84.z = (((sp84.x - sp78.x) / distx) * distz) + sp78.z;
        }
        else if (flags_main & 2)
        {
            sp84.x = center.x;
            sp84.y = (((sp84.x - sp78.x) / distx) * disty) + sp78.y;
            sp84.z = (((sp84.x - sp78.x) / distx) * distz) + sp78.z;
        }
        else if (flags_main & 4)
        {
            sp84.y = -center.y;
            sp84.x = (((sp84.y - sp78.y) / disty) * distx) + sp78.x;
            sp84.z = (((sp84.y - sp78.y) / disty) * distz) + sp78.z;
        }
        else if (flags_main & 8)
        {
            sp84.y = center.y;
            sp84.x = (((sp84.y - sp78.y) / disty) * distx) + sp78.x;
            sp84.z = (((sp84.y - sp78.y) / disty) * distz) + sp78.z;
        }
        if (flags_main == flags_sp78)
        {
            sp78 = sp84;
            flags_sp78 = func_ovl2_800EE24C(&sp78, &center);
        }
        else
        {
            sp6C = sp84;
            flags_sp6C = func_ovl2_800EE24C(&sp6C, &center);
        }
        goto loop;
    }
    flags_sp78 = func_ovl2_800EE2C0(&sp78, &center);
    flags_sp6C = func_ovl2_800EE2C0(&sp6C, &center);

    if (flags_sp78 & flags_sp6C)
    {
        return FALSE;
    }
    else return TRUE;
}

// 0x800EE750
sb32 gmCollisionTestSphere(Vec3f *pos_curr, Vec3f *pos_prev, f32 hitsize, s32 attack_state, Mtx44f mtx, Vec3f *sphere_offset, Vec3f *sphere_size, Vec3f *arg7, s32 sphit_kind, f32 *p_angle, Vec3f *argA)
{
    // Might be very fake. Maybe not. Spent a whole day trying to match this and finally managed to find a solution using the permuter!
    Vec3f center;

    center.x = sphere_size->x + (hitsize / arg7->x);
    center.y = sphere_size->y + (hitsize / arg7->y);
    center.z = sphere_size->z + (hitsize / arg7->z);

    if ((attack_state == nGMAttackStateTransfer) || (pos_curr->x == pos_prev->x) && (pos_curr->y == pos_prev->y) && (pos_curr->z == pos_prev->z))
    {
        Vec3f copy1 = *pos_curr;

        gmCollisionGetWorldPosition(mtx, &copy1);

        copy1.x -= sphere_offset->x;
        copy1.y -= sphere_offset->y;
        copy1.z -= sphere_offset->z;

        copy1.x /= center.x;
        copy1.y /= center.y;
        copy1.z /= center.z;

        if (syVectorMag3D(&copy1) <= 1)
        {
            switch (sphit_kind)
            {
            default:
                break;

            case 0:
                *p_angle = F_CLC_DTOR32(180.0F);
                break;

            case 1:
                *p_angle = F_CLC_DTOR32(180.0F);

                argA->x = 1;
                argA->y = argA->z = 0.0F;
                break;

            case 2:
                break;
            }
            return TRUE;
        }
        else return FALSE;
    }
    else
    {
        Vec3f copy1;
        Vec3f copy2;
        Vec3f copysub;
        Vec3f sp58;
        f32 sp54;
        f32 sp50;
        f32 sp4C;
        f32 sp48;
        f32 sp44;
        f32 sp40;
        f32 sp3C;

        copy1 = *pos_curr;
        copy2 = *pos_prev;

        gmCollisionGetWorldPosition(mtx, &copy1);
        gmCollisionGetWorldPosition(mtx, &copy2);

        copy1.x -= sphere_offset->x;
        copy1.y -= sphere_offset->y;
        copy1.z -= sphere_offset->z;

        copy2.x -= sphere_offset->x;
        copy2.y -= sphere_offset->y;
        copy2.z -= sphere_offset->z;

        copy1.x /= center.x;
        copy1.y /= center.y;
        copy1.z /= center.z;

        copy2.x /= center.x;
        copy2.y /= center.y;
        copy2.z /= center.z;

        copysub.x = copy1.x - copy2.x;
        copysub.y = copy1.y - copy2.y;
        copysub.z = copy1.z - copy2.z;

        sp54 = (SQUARE(copysub.x) + SQUARE(copysub.y) + SQUARE(copysub.z));

        if (sp54 != 0.0F)
        {
            sp40 = SQUARE(copy2.z);

            if (sp54 != 0.0F)
            {

            }
            sp50 = (copysub.x * copy2.x) + (copysub.y * copy2.y) + (copysub.z * copy2.z);
            sp3C = ((SQUARE(copy2.x) + SQUARE(copy2.y) + sp40) - 1.0F);
            sp3C = sp54 * sp3C;

            sp48 = SQUARE(sp50) - sp3C;

            if (SQUARE(sp50) < sp3C)
            {
                return FALSE;
            }
            else if (sp48 == 0.0F)
            {
                if (sp54 == 0.0F)
                {
                    while (TRUE)
                    {
                        syDebugPrintf("zero div 1 in gcColSphere()\n");
                        scManagerRunPrintGObjStatus();
                    }
                }
                sp44 = sp40 = -sp50 / sp54;

                if ((sp44 >= 0.0F) && (sp44 <= 1.0F))
                {
                    goto next;
                }
                else return FALSE;
            }
            else
            {
                if (sp54 == 0.0F)
                {
                    while (TRUE)
                    {
                        syDebugPrintf("zero div 2 in gcColSphere()\n");
                        scManagerRunPrintGObjStatus();
                    }
                }
                sp44 = (sqrtf(sp48) + -sp50) / sp54;
                sp40 = (-sp50 - sqrtf(sp48)) / sp54;

                if ((sp44 >= 0.0F) && (sp44 <= 1.0F))
                {
                    goto next;
                }
                else if ((sp40 >= 0.0F) && (sp40 <= 1.0F))
                {
                    goto next;
                }
                else if ((sp44 * sp40) < 0.0F)
                {
                    goto next;
                }
                else return FALSE;
            }
        next:
            switch (sphit_kind)
            {
            default:
                break;

            case 0:
                sp3C = (sp44 < sp40) ? sp44 : sp40;

                sp58.x = (copysub.x * sp3C) + copy2.x;
                sp58.y = (copysub.y * sp3C) + copy2.y;
                sp58.z = (copysub.z * sp3C) + copy2.z;

                *p_angle = syVectorAngleDiff3D(&copysub, &sp58);
                break;

            case 1:
                sp3C = (sp44 < sp40) ? sp44 : sp40;

                copysub.x = 0.0F;
                sp58.x = 0.0F;

                sp58.y = (copysub.y * sp3C) + copy2.y;
                sp58.z = (copysub.z * sp3C) + copy2.z;

                *p_angle = ((sp58.y == 0.0F) && (sp58.z == 0.0F)) ? F_CLC_DTOR32(180.0F) : syVectorAngleDiff3D(&copysub, &sp58);

                if (*p_angle != F_CLC_DTOR32(180.0F))
                {
                    syVectorNormCross3D(&copysub, &sp58, argA);
                }
                else
                {
                    argA->x = 1.0F;
                    argA->y = argA->z = 0.0F;
                }
                break;

            case 2:
                break;
            }
            return TRUE;
        }
        else if (syVectorMag3D(&copy1) <= 1)
        {
            switch (sphit_kind)
            {
            default:
                break;

            case 0:
                *p_angle = F_CLC_DTOR32(180.0F);
                break;

            case 1:
                *p_angle = F_CLC_DTOR32(180.0F);

                argA->x = 1;
                argA->y = argA->z = 0.0F;
                break;

            case 2:
                break;
            }
            return TRUE;
        }
        else return FALSE;
    }
}

// 0x800EEEAC
sb32 func_ovl2_800EEEAC(Vec3f *arg0, Vec3f *arg1, f32 arg2, s32 opkind, Mtx44f mtx, s32 arg5, Vec3f *arg6, f32 arg7, s32 arg8, f32 arg9)
{
    Vec3f sp94;
    Vec3f sp88;
    Vec3f sp7C;
    Vec3f sp70;
    Vec3f sp64;
    u32 flags_sp70;
    u32 flags_sp64;
    u32 flags_main;
    f32 distx;
    f32 disty;
    f32 distz;

    sp94.x = arg9 + arg7 + arg2;
    sp94.y = sp94.z = arg7 + arg2;

    if (opkind == 2)
    {
        sp88.x = arg0->x - arg6->x;
        sp88.y = arg0->y - arg6->y;
        sp88.z = arg0->z - arg6->z;

        if ((arg8 == 3) && (arg5 == 1))
        {
            gmCollisionGetWorldPosition(mtx, &sp88);
        }
        if ((-sp94.x <= sp88.x) && (sp88.x <= sp94.x) && (-sp94.y <= sp88.y) && (sp88.y <= sp94.y) && (-sp94.z <= sp88.z) && (sp88.z <= sp94.z))
        {
            return TRUE;
        }
        else return FALSE;
    }
    sp70.x = arg0->x - arg6->x;
    sp70.y = arg0->y - arg6->y;
    sp70.z = arg0->z - arg6->z;

    sp64.x = arg1->x - arg6->x;
    sp64.y = arg1->y - arg6->y;
    sp64.z = arg1->z - arg6->z;

    if ((arg8 == 3) && (arg5 == 1))
    {
        gmCollisionGetWorldPosition(mtx, &sp70);
        gmCollisionGetWorldPosition(mtx, &sp64);
    }
    distx = sp64.x - sp70.x;
    disty = sp64.y - sp70.y;
    distz = sp64.z - sp70.z;

    flags_sp70 = func_ovl2_800EE24C(&sp70, &sp94);
    flags_sp64 = func_ovl2_800EE24C(&sp64, &sp94);

loop:
    if ((flags_sp70 != 0) || (flags_sp64 != 0))
    {
        if (flags_sp70 & flags_sp64)
        {
            return FALSE;
        }
        else if (flags_sp70 != 0)
        {
            flags_main = flags_sp70;
        }
        else flags_main = flags_sp64;

        if (flags_main & 1)
        {
            sp7C.x = -sp94.x;
            sp7C.y = (((sp7C.x - sp70.x) / distx) * disty) + sp70.y;
            sp7C.z = (((sp7C.x - sp70.x) / distx) * distz) + sp70.z;
        }
        else if (flags_main & 2)
        {
            sp7C.x = sp94.x;
            sp7C.y = (((sp7C.x - sp70.x) / distx) * disty) + sp70.y;
            sp7C.z = (((sp7C.x - sp70.x) / distx) * distz) + sp70.z;
        }
        else if (flags_main & 4)
        {
            sp7C.y = -sp94.y;
            sp7C.x = (((sp7C.y - sp70.y) / disty) * distx) + sp70.x;
            sp7C.z = (((sp7C.y - sp70.y) / disty) * distz) + sp70.z;
        }
        else if (flags_main & 8)
        {
            sp7C.y = sp94.y;
            sp7C.x = (((sp7C.y - sp70.y) / disty) * distx) + sp70.x;
            sp7C.z = (((sp7C.y - sp70.y) / disty) * distz) + sp70.z;
        }
        if (flags_main == flags_sp70)
        {
            sp70 = sp7C;

            flags_sp70 = func_ovl2_800EE24C(&sp70, &sp94);
        }
        else
        {
            sp64 = sp7C;

            flags_sp64 = func_ovl2_800EE24C(&sp64, &sp94);
        }
        goto loop;
    }
    flags_sp70 = func_ovl2_800EE2C0(&sp70, &sp94);
    flags_sp64 = func_ovl2_800EE2C0(&sp64, &sp94);

    if (flags_sp70 & flags_sp64)
    {
        return FALSE;
    }
    return TRUE;
}

// 0x800EF2D0
sb32 gmCollisionCheckAttackInFighterRange(Vec3f *attack_position, Vec3f *obj_position, Vec3f *range, f32 size)
{
    f32 distx = attack_position->x - obj_position->x;
    f32 disty = attack_position->y - obj_position->y;

    if ((distx < (-range->z - size)) || (distx > (range->z + size)) || (disty < (-range->y - size)) || (disty > (range->x + size)))
    {
        return FALSE;
    }
    else return TRUE;
}

// 0x800EF364
sb32 gmCollisionCheckFighterInFighterRange(FTAttackColl *attack_coll, GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    if (attack_coll->attack_state == nGMAttackStateTransfer)
    {
        return gmCollisionCheckAttackInFighterRange(&attack_coll->pos_curr, &DObjGetStruct(fighter_gobj)->translate.vec.f, &attr->hit_detect_range, attack_coll->size);
    }
    else if
    (
        (gmCollisionCheckAttackInFighterRange(&attack_coll->pos_curr, &DObjGetStruct(fighter_gobj)->translate.vec.f, &attr->hit_detect_range, attack_coll->size) != FALSE)      ||
        (gmCollisionCheckAttackInFighterRange(&attack_coll->pos_prev, &DObjGetStruct(fighter_gobj)->translate.vec.f, &attr->hit_detect_range, attack_coll->size) != FALSE)
    )
    {
        return TRUE;
    }
    else return FALSE;
}

// 0x800EF414
sb32 gmCollisionCheckWeaponInFighterRange(WPAttackColl *attack_coll, s32 attack_id, GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    if (attack_coll->attack_state == nGMAttackStateTransfer)
    {
        return gmCollisionCheckAttackInFighterRange(&attack_coll->attack_pos[attack_id].pos_curr, &DObjGetStruct(fighter_gobj)->translate.vec.f, &attr->hit_detect_range, attack_coll->size);
    }
    else if
    (
        (gmCollisionCheckAttackInFighterRange(&attack_coll->attack_pos[attack_id].pos_curr, &DObjGetStruct(fighter_gobj)->translate.vec.f, &attr->hit_detect_range, attack_coll->size) != FALSE)      ||
        (gmCollisionCheckAttackInFighterRange(&attack_coll->attack_pos[attack_id].pos_prev, &DObjGetStruct(fighter_gobj)->translate.vec.f, &attr->hit_detect_range, attack_coll->size) != FALSE)
    )
    {
        return TRUE;
    }
    else return FALSE;
}

// 0x800EF4F4
sb32 gmCollisionCheckItemInFighterRange(ITAttackColl *attack_coll, s32 attack_id, GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    if (attack_coll->attack_state == nGMAttackStateTransfer)
    {
        return gmCollisionCheckAttackInFighterRange(&attack_coll->attack_pos[attack_id].pos_curr, &DObjGetStruct(fighter_gobj)->translate.vec.f, &attr->hit_detect_range, attack_coll->size);
    }
    else if
    (
        (gmCollisionCheckAttackInFighterRange(&attack_coll->attack_pos[attack_id].pos_curr, &DObjGetStruct(fighter_gobj)->translate.vec.f, &attr->hit_detect_range, attack_coll->size) != FALSE)      ||
        (gmCollisionCheckAttackInFighterRange(&attack_coll->attack_pos[attack_id].pos_prev, &DObjGetStruct(fighter_gobj)->translate.vec.f, &attr->hit_detect_range, attack_coll->size) != FALSE)
    )
    {
        return TRUE;
    }
    else return FALSE;
}

// 0x800EF5D4
sb32 func_ovl2_800EF5D4(Vec3f *arg0, Vec3f *arg1, f32 arg2, s32 arg3, Vec3f *arg4, Vec3f *arg5, f32 arg6, s32 arg7)
{
    f32 sp2C;
    f32 sp28;
    f32 sp24;
    f32 sp20;
    f32 var_f12;
    f32 temp;

    temp = arg6 + arg2;

    if (arg7 == 2)
    {
        if (arg3 == 2)
        {
            if ((arg0->x < (arg4->x - temp)) || ((arg4->x + temp) < arg0->x) || (arg0->y < (arg4->y - temp)) || ((arg4->y + temp) < arg0->y))
            {
                return FALSE;
            }
            else return TRUE;
        }
        if (arg0->x < arg1->x)
        {
            sp2C = arg0->x - temp;
            sp28 = arg1->x + temp;
        }
        else
        {
            sp2C = arg1->x - temp;
            sp28 = arg0->x + temp;
        }
        if ((arg4->x < sp2C) || (sp28 < arg4->x))
        {
            return FALSE;
        }
        if (arg0->y < arg1->y)
        {
            sp24 = arg0->y - temp;
            sp20 = arg1->y + temp;
        }
        else
        {
            sp24 = arg1->y - temp;
            sp20 = arg0->y + temp;
        }
        if ((arg4->y < sp24) || (sp20 < arg4->y))
        {
            return FALSE;
        }
        else return TRUE;
    }
    if (arg3 == 2)
    {
        if (arg4->x < arg5->x)
        {
            sp2C = arg4->x - temp;
            sp28 = arg5->x + temp;
        }
        else
        {
            sp2C = arg5->x - temp;
            sp28 = arg4->x + temp;
        }
        if ((arg0->x < sp2C) || (sp28 < arg0->x))
        {
            return FALSE;
        }
        if (arg4->y < arg5->y)
        {
            sp24 = arg4->y - temp;
            sp20 = arg5->y + temp;
        }
        else
        {
            sp24 = arg5->y - temp;
            sp20 = arg4->y + temp;
        }
        if ((arg0->y < sp24) || (sp20 < arg0->y))
        {
            return FALSE;
        }
        else return TRUE;
    }
    if (arg0->x < arg1->x)
    {
        var_f12 = -(arg0->x - arg1->x);
    }
    else
    {
        var_f12 = arg0->x - arg1->x;
    }
    if (arg4->x < arg5->x)
    {
        sp2C = arg4->x - ((var_f12 * 0.5F) + temp);
        sp28 = arg5->x + ((var_f12 * 0.5F) + temp);
    }
    else
    {
        sp2C = arg5->x - ((var_f12 * 0.5F) + temp);
        sp28 = arg4->x + ((var_f12 * 0.5F) + temp);

    }
    if ((arg0->x < sp2C) && (arg1->x < sp2C))
    {
        return FALSE;
    }
    else if ((sp28 < arg0->x) && (sp28 < arg1->x))
    {
        return FALSE;
    }
    if (arg0->y < arg1->y)
    {
        var_f12 = -(arg0->y - arg1->y);
    }
    else var_f12 = arg0->y - arg1->y;

    if (arg4->y < arg5->y)
    {
        sp24 = arg4->y - ((var_f12 * 0.5F) + temp);
        sp20 = arg5->y + ((var_f12 * 0.5F) + temp);
    }
    else
    {
        sp24 = arg5->y - ((var_f12 * 0.5F) + temp);
        sp20 = arg4->y + ((var_f12 * 0.5F) + temp);
    }
    if ((arg0->y < sp24) && (arg1->y < sp24))
    {
        return FALSE;
    }
    else if ((sp20 < arg0->y) && (sp20 < arg1->y))
    {
        return FALSE;
    }
    return TRUE;
}

// 0x800EFABC - Check if fighter hitbox intersects with other fighter hitbox
sb32 gmCollisionCheckFighterAttacksCollide(FTAttackColl *attack_coll1, FTAttackColl *attack_coll2)
{
    if 
    (
        func_ovl2_800EF5D4
        (
            &attack_coll1->pos_curr, 
            &attack_coll1->pos_prev, 
            attack_coll1->size, 
            attack_coll1->attack_state, 
            &attack_coll2->pos_curr, 
            &attack_coll2->pos_prev, 
            attack_coll2->size, 
            attack_coll2->attack_state

        ) == FALSE
    )
    {
        return FALSE;
    }
    else func_ovl2_800EE050
    (
        attack_coll2->attack_state, 
        &attack_coll2->pos_curr, 
        &attack_coll2->pos_prev, 
        &attack_coll2->attack_matrix.unk_fthitmtx_0x0, 
        attack_coll2->attack_matrix.mtx, 
        &attack_coll2->attack_matrix.unk_fthitmtx_0x44
    );

    return func_ovl2_800EEEAC
    (
        &attack_coll1->pos_curr,
        &attack_coll1->pos_prev,
        attack_coll1->size,
        attack_coll1->attack_state,
        attack_coll2->attack_matrix.mtx,
        attack_coll2->attack_matrix.unk_fthitmtx_0x0,
        &attack_coll2->pos_curr,
        attack_coll2->size,
        attack_coll2->attack_state,
        attack_coll2->attack_matrix.unk_fthitmtx_0x44
    );
}

// 0x800EFBA4 - Check if fighter hitbox intersects with fighter hurtbox
sb32 gmCollisionCheckFighterAttackDamageCollide(FTAttackColl *attack_coll, FTDamageColl *damage_coll)
{
    FTParts *parts;
    DObj *dobj;

    dobj = damage_coll->joint;
    parts = ftGetParts(dobj);

    func_ovl2_800EDE00(dobj);
    func_ovl2_800EDE5C(dobj);

    return gmCollisionTestRectangle
    (
        &attack_coll->pos_curr,
        &attack_coll->pos_prev,
        attack_coll->size,
        attack_coll->attack_state,
        parts->unk_dobjtrans_0x9C,
        &damage_coll->offset,
        &damage_coll->size,
        &parts->vec_scale
    );
}

// 0x800EFC20 - Check if item hurtbox intersects with fighter hitbox
sb32 gmCollisionCheckFighterAttackItemDamageCollide(FTAttackColl *attack_coll, ITDamageColl *damage_coll, GObj *item_gobj)
{
    Vec3f damage_coll_pos;

    damage_coll_pos.x = DObjGetStruct(item_gobj)->translate.vec.f.x + damage_coll->offset.x;
    damage_coll_pos.y = DObjGetStruct(item_gobj)->translate.vec.f.y + damage_coll->offset.y;
    damage_coll_pos.z = DObjGetStruct(item_gobj)->translate.vec.f.z + damage_coll->offset.z;

    return gmCollisionTestRectangle(&attack_coll->pos_curr, &attack_coll->pos_prev, attack_coll->size, attack_coll->attack_state, NULL, &damage_coll_pos, &damage_coll->size, &DObjGetStruct(item_gobj)->scale.vec.f);
}

// 0x800EFCC0 - Check if fighter hitbox intersects with shield
sb32 gmCollisionCheckFighterAttackShieldCollide(FTAttackColl *attack_coll, GObj *fighter_gobj, DObj *dobj, f32 *p_angle)
{
    FTParts *parts;
    Vec3f sp48;
    Vec3f sp3C;

    sp48.x = 0.0F;
    sp48.y = 0.0F;
    sp48.z = 0.0F;

    sp3C.x = 30.0F;
    sp3C.y = 30.0F;
    sp3C.z = 30.0F;

    parts = ftGetParts(dobj);

    func_ovl2_800EDE00(dobj);
    func_ovl2_800EDE5C(dobj);

    return gmCollisionTestSphere
    (
        &attack_coll->pos_curr, 
        &attack_coll->pos_prev, 
        attack_coll->size, 
        attack_coll->attack_state, 
        parts->unk_dobjtrans_0x9C,
        &sp48, 
        &sp3C, 
        &parts->vec_scale, 
        0,
        p_angle, 
        NULL
    );
}

// 0x800EFD70 - Check if weapon hitbox intersects with fighter hitbox
sb32 gmCollisionCheckWeaponAttackFighterAttackCollide(WPAttackColl *wp_attack_coll, s32 attack_id, FTAttackColl *ft_attack_coll)
{
    if
    (
        func_ovl2_800EF5D4
        (
            &wp_attack_coll->attack_pos[attack_id].pos_curr,
            &wp_attack_coll->attack_pos[attack_id].pos_prev,
            wp_attack_coll->size,
            wp_attack_coll->attack_state,
            &ft_attack_coll->pos_curr,
            &ft_attack_coll->pos_prev,
            ft_attack_coll->size,
            ft_attack_coll->attack_state

        ) == FALSE
    )
    {
        return FALSE;
    }
    else func_ovl2_800EE050
    (
        ft_attack_coll->attack_state,
        &ft_attack_coll->pos_curr,
        &ft_attack_coll->pos_prev,
        &ft_attack_coll->attack_matrix.unk_fthitmtx_0x0,
        ft_attack_coll->attack_matrix.mtx,
        &ft_attack_coll->attack_matrix.unk_fthitmtx_0x44
    );

    return func_ovl2_800EEEAC
    (
        &wp_attack_coll->attack_pos[attack_id].pos_curr,
        &wp_attack_coll->attack_pos[attack_id].pos_prev,
        wp_attack_coll->size,
        wp_attack_coll->attack_state,
        ft_attack_coll->attack_matrix.mtx,
        ft_attack_coll->attack_matrix.unk_fthitmtx_0x0,
        &ft_attack_coll->pos_curr,
        ft_attack_coll->size,
        ft_attack_coll->attack_state,
        ft_attack_coll->attack_matrix.unk_fthitmtx_0x44
    );
}

// 0x800EFE6C
sb32 gmCollisionCheckWeaponAttackFighterDamageCollide(WPAttackColl *attack_coll, s32 attack_id, FTDamageColl *damage_coll)
{
    FTParts *parts;
    DObj *dobj;

    dobj = damage_coll->joint;
    parts = ftGetParts(dobj);

    func_ovl2_800EDE00(dobj);
    func_ovl2_800EDE5C(dobj);

    return gmCollisionTestRectangle
    (
        &attack_coll->attack_pos[attack_id].pos_curr,
        &attack_coll->attack_pos[attack_id].pos_prev,
        attack_coll->size,
        attack_coll->attack_state, 
        parts->unk_dobjtrans_0x9C, 
        &damage_coll->offset, 
        &damage_coll->size, 
        &parts->vec_scale
    );
}

// 0x800EFF00
sb32 gmCollisionCheckWeaponAttackShieldCollide(WPAttackColl *attack_coll, s32 attack_id, GObj *fighter_gobj, DObj *dobj, f32 *p_angle, Vec3f *vec)
{
    FTParts *parts;
    Vec3f sp58;
    Vec3f sp4C;
    Vec3f unused;

    sp58.x = 0.0F;
    sp58.y = 0.0F;
    sp58.z = 0.0F;

    sp4C.x = 30.0F;
    sp4C.y = 30.0F;
    sp4C.z = 30.0F;

    parts = ftGetParts(dobj);

    func_ovl2_800EDE00(dobj);
    func_ovl2_800EDE5C(dobj);

    return gmCollisionTestSphere
    (
        &attack_coll->attack_pos[attack_id].pos_curr,
        &attack_coll->attack_pos[attack_id].pos_prev,
        attack_coll->size,
        attack_coll->attack_state,
        parts->unk_dobjtrans_0x9C,
        &sp58,
        &sp4C,
        &parts->vec_scale,
        1,
        p_angle,
        vec
    );
}

// 0x800EFFCC
sb32 gmCollisionCheckWeaponAttackSpecialCollide(WPAttackColl *attack_coll, s32 attack_id, FTStruct *fp, FTSpecialColl *special_coll)
{
    DObj *dobj = fp->joints[special_coll->joint_id];
    FTParts *parts = ftGetParts(dobj);

    func_ovl2_800EDE00(dobj);
    func_ovl2_800EDE5C(dobj);

    return gmCollisionTestSphere
    (
        &attack_coll->attack_pos[attack_id].pos_curr,
        &attack_coll->attack_pos[attack_id].pos_prev,
        attack_coll->size,
        attack_coll->attack_state,
        parts->unk_dobjtrans_0x9C,
        &special_coll->offset,
        &special_coll->size,
        &parts->vec_scale,
        2,
        NULL,
        NULL
    );
}

// 0x800F007C
sb32 gmCollisionCheckWeaponAttacksCollide(WPAttackColl *attack_coll1, s32 atk1_id, WPAttackColl *attack_coll2, s32 atk2_id)
{
    if
    (
        func_ovl2_800EF5D4
        (
            &attack_coll1->attack_pos[atk1_id].pos_curr,
            &attack_coll1->attack_pos[atk1_id].pos_prev,
            attack_coll1->size,
            attack_coll1->attack_state,
            &attack_coll2->attack_pos[atk2_id].pos_curr,
            &attack_coll2->attack_pos[atk2_id].pos_prev,
            attack_coll2->size,
            attack_coll2->attack_state

        ) == FALSE
    )
    {
        return FALSE;
    }
    else func_ovl2_800EE050
    (
        attack_coll2->attack_state,
        &attack_coll2->attack_pos[atk2_id].pos_curr,
        &attack_coll2->attack_pos[atk2_id].pos_prev,
        &attack_coll2->attack_pos[atk2_id].unk_wphitpos_0x18,
        attack_coll2->attack_pos[atk2_id].mtx,
        &attack_coll2->attack_pos[atk2_id].unk_wphitpos_0x5C
    );

    return func_ovl2_800EEEAC
    (
        &attack_coll1->attack_pos[atk1_id].pos_curr,
        &attack_coll1->attack_pos[atk1_id].pos_prev,
        attack_coll1->size,
        attack_coll1->attack_state,
        attack_coll2->attack_pos[atk2_id].mtx,
        attack_coll2->attack_pos[atk2_id].unk_wphitpos_0x18,
        &attack_coll2->attack_pos[atk2_id].pos_curr,
        attack_coll2->size,
        attack_coll2->attack_state,
        attack_coll2->attack_pos[atk2_id].unk_wphitpos_0x5C
    );
}

// 0x800F019C
sb32 gmCollisionCheckWeaponAttackItemAttackCollide(WPAttackColl *wp_attack_coll, s32 wp_attack_id, ITAttackColl *it_attack_coll, s32 it_attack_id)
{
    if
    (
        func_ovl2_800EF5D4
        (
            &wp_attack_coll->attack_pos[wp_attack_id].pos_curr,
            &wp_attack_coll->attack_pos[wp_attack_id].pos_prev,
            wp_attack_coll->size,
            wp_attack_coll->attack_state,
            &it_attack_coll->attack_pos[it_attack_id].pos_curr,
            &it_attack_coll->attack_pos[it_attack_id].pos_prev,
            it_attack_coll->size,
            it_attack_coll->attack_state

        ) == FALSE
    )
    {
        return FALSE;
    }
    else func_ovl2_800EE050
    (
        it_attack_coll->attack_state,
        &it_attack_coll->attack_pos[it_attack_id].pos_curr,
        &it_attack_coll->attack_pos[it_attack_id].pos_prev,
        &it_attack_coll->attack_pos[it_attack_id].unk_ithitpos_0x18,
        it_attack_coll->attack_pos[it_attack_id].mtx,
        &it_attack_coll->attack_pos[it_attack_id].unk_ithitpos_0x5C
    );

    return func_ovl2_800EEEAC
    (
        &wp_attack_coll->attack_pos[wp_attack_id].pos_curr,
        &wp_attack_coll->attack_pos[wp_attack_id].pos_prev,
        wp_attack_coll->size,
        wp_attack_coll->attack_state,
        it_attack_coll->attack_pos[it_attack_id].mtx,
        it_attack_coll->attack_pos[it_attack_id].unk_ithitpos_0x18,
        &it_attack_coll->attack_pos[it_attack_id].pos_curr,
        it_attack_coll->size,
        it_attack_coll->attack_state,
        it_attack_coll->attack_pos[it_attack_id].unk_ithitpos_0x5C
    );
}

// 0x800F02BC
sb32 gmCollisionCheckItemAttackFighterAttackCollide(ITAttackColl *it_attack_coll, s32 attack_id, FTAttackColl *ft_attack_coll)
{
    if
    (
        func_ovl2_800EF5D4
        (
            &it_attack_coll->attack_pos[attack_id].pos_curr,
            &it_attack_coll->attack_pos[attack_id].pos_prev,
            it_attack_coll->size,
            it_attack_coll->attack_state,
            &ft_attack_coll->pos_curr,
            &ft_attack_coll->pos_prev,
            ft_attack_coll->size,
            ft_attack_coll->attack_state

        ) == FALSE
    )
    {
        return FALSE;
    }
    else func_ovl2_800EE050
    (
        ft_attack_coll->attack_state,
        &ft_attack_coll->pos_curr,
        &ft_attack_coll->pos_prev,
        &ft_attack_coll->attack_matrix.unk_fthitmtx_0x0,
        ft_attack_coll->attack_matrix.mtx,
        &ft_attack_coll->attack_matrix.unk_fthitmtx_0x44
    );

    return func_ovl2_800EEEAC
    (
        &it_attack_coll->attack_pos[attack_id].pos_curr,
        &it_attack_coll->attack_pos[attack_id].pos_prev,
        it_attack_coll->size,
        it_attack_coll->attack_state,
        ft_attack_coll->attack_matrix.mtx,
        ft_attack_coll->attack_matrix.unk_fthitmtx_0x0,
        &ft_attack_coll->pos_curr,
        ft_attack_coll->size,
        ft_attack_coll->attack_state,
        ft_attack_coll->attack_matrix.unk_fthitmtx_0x44
    );
}

// 0x800F03B8
sb32 gmCollisionCheckItemAttackFighterDamageCollide(ITAttackColl *attack_coll, s32 attack_id, FTDamageColl *damage_coll)
{
    FTParts *parts;
    DObj *dobj;

    dobj = damage_coll->joint;
    parts = ftGetParts(dobj);

    func_ovl2_800EDE00(dobj);
    func_ovl2_800EDE5C(dobj);

    return gmCollisionTestRectangle
    (
        &attack_coll->attack_pos[attack_id].pos_curr,
        &attack_coll->attack_pos[attack_id].pos_prev,
        attack_coll->size,
        attack_coll->attack_state, 
        parts->unk_dobjtrans_0x9C,
        &damage_coll->offset, 
        &damage_coll->size, 
        &parts->vec_scale
    );
}

// 0x800F044C
sb32 gmCollisionCheckItemAttackShieldCollide(ITAttackColl *attack_coll, s32 attack_id, GObj *fighter_gobj, DObj *dobj, f32 *p_angle, Vec3f *vec)
{
    FTParts *parts;
    Vec3f sp58;
    Vec3f sp4C;
    Vec3f unused;

    sp58.x = 0.0F;
    sp58.y = 0.0F;
    sp58.z = 0.0F;

    sp4C.x = 30.0F;
    sp4C.y = 30.0F;
    sp4C.z = 30.0F;

    parts = ftGetParts(dobj);

    func_ovl2_800EDE00(dobj);
    func_ovl2_800EDE5C(dobj);

    return gmCollisionTestSphere
    (
        &attack_coll->attack_pos[attack_id].pos_curr,
        &attack_coll->attack_pos[attack_id].pos_prev,
        attack_coll->size,
        attack_coll->attack_state,
        parts->unk_dobjtrans_0x9C,
        &sp58,
        &sp4C,
        &parts->vec_scale,
        1,
        p_angle,
        vec
    );
}

// 0x800F0518
sb32 gmCollisionCheckItemAttackSpecialCollide(ITAttackColl *attack_coll, s32 attack_id, FTStruct *fp, FTSpecialColl *special_coll)
{
    DObj *dobj = fp->joints[special_coll->joint_id];
    FTParts *parts = ftGetParts(dobj);

    func_ovl2_800EDE00(dobj);
    func_ovl2_800EDE5C(dobj);

    return gmCollisionTestSphere
    (
        &attack_coll->attack_pos[attack_id].pos_curr,
        &attack_coll->attack_pos[attack_id].pos_prev,
        attack_coll->size,
        attack_coll->attack_state,
        parts->unk_dobjtrans_0x9C,
        &special_coll->offset,
        &special_coll->size,
        &parts->vec_scale,
        2,
        NULL,
        NULL
    );
}

// 0x800F05C8
sb32 gmCollisionCheckItemAttacksCollide(ITAttackColl *attack_coll1, s32 atk1_id, ITAttackColl *attack_coll2, s32 atk2_id)
{
    if
    (
        func_ovl2_800EF5D4
        (
            &attack_coll1->attack_pos[atk1_id].pos_curr,
            &attack_coll1->attack_pos[atk1_id].pos_prev,
            attack_coll1->size,
            attack_coll1->attack_state,
            &attack_coll2->attack_pos[atk2_id].pos_curr,
            &attack_coll2->attack_pos[atk2_id].pos_prev,
            attack_coll2->size,
            attack_coll2->attack_state

        ) == FALSE
    )
    {
        return FALSE;
    }
    else func_ovl2_800EE050
    (
        attack_coll2->attack_state,
        &attack_coll2->attack_pos[atk2_id].pos_curr,
        &attack_coll2->attack_pos[atk2_id].pos_prev,
        &attack_coll2->attack_pos[atk2_id].unk_ithitpos_0x18,
        attack_coll2->attack_pos[atk2_id].mtx,
        &attack_coll2->attack_pos[atk2_id].unk_ithitpos_0x5C
    );

    return func_ovl2_800EEEAC
    (
        &attack_coll1->attack_pos[atk1_id].pos_curr,
        &attack_coll1->attack_pos[atk1_id].pos_prev,
        attack_coll1->size,
        attack_coll1->attack_state,
        attack_coll2->attack_pos[atk2_id].mtx,
        attack_coll2->attack_pos[atk2_id].unk_ithitpos_0x18,
        &attack_coll2->attack_pos[atk2_id].pos_curr,
        attack_coll2->size,
        attack_coll2->attack_state,
        attack_coll2->attack_pos[atk2_id].unk_ithitpos_0x5C
    );
}

// 0x800F06E8
sb32 gmCollisionCheckItemAttackDamageCollide(ITAttackColl *attack_coll, s32 attack_id, ITDamageColl *damage_coll, GObj *item_gobj)
{
    Vec3f damage_coll_pos;

    damage_coll_pos.x = DObjGetStruct(item_gobj)->translate.vec.f.x + damage_coll->offset.x;
    damage_coll_pos.y = DObjGetStruct(item_gobj)->translate.vec.f.y + damage_coll->offset.y;
    damage_coll_pos.z = DObjGetStruct(item_gobj)->translate.vec.f.z + damage_coll->offset.z;

    return gmCollisionTestRectangle
    (
        &attack_coll->attack_pos[attack_id].pos_curr,
        &attack_coll->attack_pos[attack_id].pos_prev,
        attack_coll->size,
        attack_coll->attack_state,
        NULL,
        &damage_coll_pos,
        &damage_coll->size,
        &DObjGetStruct(item_gobj)->scale.vec.f
    );
}

// 0x800F079C
sb32 gmCollisionCheckWeaponAttackItemDamageCollide(WPAttackColl *attack_coll, s32 attack_id, ITDamageColl *damage_coll, GObj *item_gobj)
{
    Vec3f damage_coll_pos;

    damage_coll_pos.x = DObjGetStruct(item_gobj)->translate.vec.f.x + damage_coll->offset.x;
    damage_coll_pos.y = DObjGetStruct(item_gobj)->translate.vec.f.y + damage_coll->offset.y;
    damage_coll_pos.z = DObjGetStruct(item_gobj)->translate.vec.f.z + damage_coll->offset.z;

    return gmCollisionTestRectangle
    (
        &attack_coll->attack_pos[attack_id].pos_curr,
        &attack_coll->attack_pos[attack_id].pos_prev,
        attack_coll->size,
        attack_coll->attack_state,
        NULL,
        &damage_coll_pos,
        &damage_coll->size,
        &DObjGetStruct(item_gobj)->scale.vec.f
    );
}

// 0x800F0850
void gmCollisionGetFighterAttackPosition(Vec3f *dst, FTAttackColl *attack_coll)
{
    if (attack_coll->attack_state == nGMAttackStateTransfer)
    {
        *dst = attack_coll->pos_curr;
    }
    else
    {
        dst->x = (attack_coll->pos_curr.x + attack_coll->pos_prev.x) * 0.5F;
        dst->y = (attack_coll->pos_curr.y + attack_coll->pos_prev.y) * 0.5F;
        dst->z = (attack_coll->pos_curr.z + attack_coll->pos_prev.z) * 0.5F;
    }
}

// 0x800F08C8
void gmCollisionGetWeaponAttackPosition(Vec3f *dst, WPAttackColl *attack_coll, s32 attack_id)
{
    if (attack_coll->attack_state == nGMAttackStateTransfer)
    {
        *dst = attack_coll->attack_pos[attack_id].pos_curr;
    }
    else
    {
        dst->x = (attack_coll->attack_pos[attack_id].pos_curr.x + attack_coll->attack_pos[attack_id].pos_prev.x) * 0.5F;
        dst->y = (attack_coll->attack_pos[attack_id].pos_curr.y + attack_coll->attack_pos[attack_id].pos_prev.y) * 0.5F;
        dst->z = (attack_coll->attack_pos[attack_id].pos_curr.z + attack_coll->attack_pos[attack_id].pos_prev.z) * 0.5F;
    }
}

// 0x800F095C
void gmCollisionGetItemAttackPosition(Vec3f *dst, ITAttackColl *attack_coll, s32 attack_id)
{
    if (attack_coll->attack_state == nGMAttackStateTransfer)
    {
        *dst = attack_coll->attack_pos[attack_id].pos_curr;
    }
    else
    {
        dst->x = (attack_coll->attack_pos[attack_id].pos_curr.x + attack_coll->attack_pos[attack_id].pos_prev.x) * 0.5F;
        dst->y = (attack_coll->attack_pos[attack_id].pos_curr.y + attack_coll->attack_pos[attack_id].pos_prev.y) * 0.5F;
        dst->z = (attack_coll->attack_pos[attack_id].pos_curr.z + attack_coll->attack_pos[attack_id].pos_prev.z) * 0.5F;
    }
}

// 0x800F09F0
void gmCollisionGetShieldPosition(Vec3f *dst, GObj *gobj, DObj *dobj)
{
    FTParts *parts = ftGetParts(dobj);

    dst->x = 0.0F;
    dst->y = 0.0F;
    dst->z = 0.0F;

    gmCollisionGetWorldPosition(parts->mtx_translate, dst);

    dst->z = DObjGetStruct(gobj)->translate.vec.f.z;
}

// 0x800F0A48
void gmCollisionGetCommonImpactPosition(Vec3f *dst, Vec3f *pos, Vec3f *offset)
{
    dst->x = (pos->x + offset->x) * 0.5F;
    dst->y = (pos->y + offset->y) * 0.5F;
    dst->z = (pos->z + offset->z) * 0.5F;
}

// 0x800F0A90
void gmCollisionGetFighterAttackDamagePosition(Vec3f *dst, FTAttackColl *attack_coll, FTDamageColl *damage_coll)
{
    FTParts *parts;
    Vec3f attack_pos;
    Vec3f damage_pos;

    gmCollisionGetFighterAttackPosition(&attack_pos, attack_coll);

    parts = ftGetParts(damage_coll->joint);
    damage_pos = damage_coll->offset;

    gmCollisionGetWorldPosition(parts->mtx_translate, &damage_pos);
    gmCollisionGetCommonImpactPosition(dst, &attack_pos, &damage_pos);
}

// 0x800F0AF8
void gmCollisionGetFighterAttackItemDamagePosition(Vec3f *dst, FTAttackColl *attack_coll, ITDamageColl *damage_coll, GObj *item_gobj)
{
    Vec3f attack_pos;
    Vec3f damage_pos;

    gmCollisionGetFighterAttackPosition(&attack_pos, attack_coll);

    damage_pos.x = DObjGetStruct(item_gobj)->translate.vec.f.x + damage_coll->offset.x;
    damage_pos.y = DObjGetStruct(item_gobj)->translate.vec.f.y + damage_coll->offset.y;
    damage_pos.z = DObjGetStruct(item_gobj)->translate.vec.f.z + damage_coll->offset.z;

    gmCollisionGetCommonImpactPosition(dst, &attack_pos, &damage_pos);
}

// 0x800F0B78
void gmCollisionGetFighterAttackShieldPosition(Vec3f *dst, FTAttackColl *attack_coll, GObj *gobj, DObj *dobj)
{
    Vec3f pos;
    Vec3f offset;

    gmCollisionGetFighterAttackPosition(&pos, attack_coll);
    gmCollisionGetShieldPosition(&offset, gobj, dobj);
    gmCollisionGetCommonImpactPosition(dst, &pos, &offset);
}

// 0x800F0BC4
void gmCollisionGetFighterAttacksPosition(Vec3f *dst, FTAttackColl *attack_coll1, FTAttackColl *attack_coll2)
{
    Vec3f pos1;
    Vec3f pos2;

    gmCollisionGetFighterAttackPosition(&pos1, attack_coll1);
    gmCollisionGetFighterAttackPosition(&pos2, attack_coll2);
    gmCollisionGetCommonImpactPosition(dst, &pos1, &pos2);
}

// 0x800F0C08
void gmCollisionGetWeaponAttackFighterAttackPosition(Vec3f *dst, WPAttackColl *wp_attack_coll, s32 attack_id, FTAttackColl *ft_attack_coll)
{
    Vec3f wp_pos;
    Vec3f ft_pos;

    gmCollisionGetWeaponAttackPosition(&wp_pos, wp_attack_coll, attack_id);
    gmCollisionGetFighterAttackPosition(&ft_pos, ft_attack_coll);
    gmCollisionGetCommonImpactPosition(dst, &wp_pos, &ft_pos);
}

// 0x800F0C4C
void gmCollisionGetWeaponAttackShieldPosition(Vec3f *dst, WPAttackColl *attack_coll, s32 attack_id, GObj *gobj, DObj *dobj)
{
    Vec3f wp_pos;
    Vec3f shield_pos;

    gmCollisionGetWeaponAttackPosition(&wp_pos, attack_coll, attack_id);
    gmCollisionGetShieldPosition(&shield_pos, gobj, dobj);
    gmCollisionGetCommonImpactPosition(dst, &wp_pos, &shield_pos);
}

// 0x800F0C94
void gmCollisionGetWeaponAttacksPosition(Vec3f *dst, WPAttackColl *attack_coll1, s32 atk1_id, WPAttackColl *attack_coll2, s32 atk2_id)
{
    Vec3f pos1;
    Vec3f pos2;

    gmCollisionGetWeaponAttackPosition(&pos1, attack_coll1, atk1_id);
    gmCollisionGetWeaponAttackPosition(&pos2, attack_coll2, atk2_id);
    gmCollisionGetCommonImpactPosition(dst, &pos1, &pos2);
}

// 0x800F0CDC
void gmCollisionGetWeaponAttackItemAttackPosition(Vec3f *dst, WPAttackColl *wp_attack_coll, s32 wp_attack_id, ITAttackColl *it_attack_coll, s32 it_attack_id)
{
    Vec3f wp_pos;
    Vec3f it_pos;

    gmCollisionGetWeaponAttackPosition(&wp_pos, wp_attack_coll, wp_attack_id);
    gmCollisionGetItemAttackPosition(&it_pos, it_attack_coll, it_attack_id);
    gmCollisionGetCommonImpactPosition(dst, &wp_pos, &it_pos);
}

// 0x800F0D24
void gmCollisionGetWeaponAttackFighterDamagePosition(Vec3f *dst, WPAttackColl *attack_coll, s32 attack_id, FTDamageColl *damage_coll)
{
    FTParts *parts;
    Vec3f attack_pos;
    Vec3f damage_pos;

    gmCollisionGetWeaponAttackPosition(&attack_pos, attack_coll, attack_id);

    parts = ftGetParts(damage_coll->joint);
    damage_pos = damage_coll->offset;

    gmCollisionGetWorldPosition(parts->mtx_translate, &damage_pos);
    gmCollisionGetCommonImpactPosition(dst, &attack_pos, &damage_pos);
}

// 0x800F0D8C
void gmCollisionGetWeaponAttackItemDamagePosition(Vec3f *dst, WPAttackColl *attack_coll, s32 attack_id, ITDamageColl *damage_coll, GObj *item_gobj)
{
    Vec3f attack_pos;
    Vec3f damage_pos;

    gmCollisionGetWeaponAttackPosition(&attack_pos, attack_coll, attack_id);

    damage_pos.x = DObjGetStruct(item_gobj)->translate.vec.f.x + damage_coll->offset.x;
    damage_pos.y = DObjGetStruct(item_gobj)->translate.vec.f.y + damage_coll->offset.y;
    damage_pos.z = DObjGetStruct(item_gobj)->translate.vec.f.z + damage_coll->offset.z;

    gmCollisionGetCommonImpactPosition(dst, &attack_pos, &damage_pos);
}

// 0x800F0E08
void gmCollisionGetItemAttackFighterDamagePosition(Vec3f *dst, ITAttackColl *attack_coll, s32 attack_id, FTDamageColl *damage_coll)
{
    FTParts *parts;
    Vec3f attack_pos;
    Vec3f damage_pos;

    gmCollisionGetItemAttackPosition(&attack_pos, attack_coll, attack_id);

    parts = ftGetParts(damage_coll->joint);
    damage_pos = damage_coll->offset;

    gmCollisionGetWorldPosition(parts->mtx_translate, &damage_pos);
    gmCollisionGetCommonImpactPosition(dst, &attack_pos, &damage_pos);
}

// 0x800F0E70
void gmCollisionGetItemAttackFighterAttackPosition(Vec3f *dst, ITAttackColl *it_attack_coll, s32 attack_id, FTAttackColl *ft_attack_coll)
{
    Vec3f it_pos;
    Vec3f ft_pos;

    gmCollisionGetItemAttackPosition(&it_pos, it_attack_coll, attack_id);
    gmCollisionGetFighterAttackPosition(&ft_pos, ft_attack_coll);
    gmCollisionGetCommonImpactPosition(dst, &it_pos, &ft_pos);
}

// 0x800F0EB4
void gmCollisionGetItemAttackShieldPosition(Vec3f *dst, ITAttackColl *attack_coll, s32 attack_id, GObj *gobj, DObj *dobj)
{
    Vec3f it_pos;
    Vec3f shield_pos;

    gmCollisionGetItemAttackPosition(&it_pos, attack_coll, attack_id);
    gmCollisionGetShieldPosition(&shield_pos, gobj, dobj);
    gmCollisionGetCommonImpactPosition(dst, &it_pos, &shield_pos);
}

// 0x800F0EFC
void gmCollisionGetItemAttackItemAttackPosition(Vec3f *dst, ITAttackColl *attack_coll1, s32 atk1_id, ITAttackColl *attack_coll2, s32 atk2_id)
{
    Vec3f pos1;
    Vec3f pos2;

    gmCollisionGetItemAttackPosition(&pos1, attack_coll1, atk1_id);
    gmCollisionGetItemAttackPosition(&pos2, attack_coll2, atk2_id);
    gmCollisionGetCommonImpactPosition(dst, &pos1, &pos2);
}

// 0x800F0F44
void gmCollisionGetItemAttackItemDamagePosition(Vec3f *dst, ITAttackColl *attack_coll, s32 attack_id, ITDamageColl *damage_coll, GObj *item_gobj)
{
    Vec3f attack_pos;
    Vec3f damage_pos;

    gmCollisionGetItemAttackPosition(&attack_pos, attack_coll, attack_id);

    damage_pos.x = DObjGetStruct(item_gobj)->translate.vec.f.x + damage_coll->offset.x;
    damage_pos.y = DObjGetStruct(item_gobj)->translate.vec.f.y + damage_coll->offset.y;
    damage_pos.z = DObjGetStruct(item_gobj)->translate.vec.f.z + damage_coll->offset.z;

    gmCollisionGetCommonImpactPosition(dst, &attack_pos, &damage_pos);
}

// 0x800F0FC0
f32 gmCollisionGetDamageSlashRotation(FTStruct *fp, FTAttackColl *attack_coll)
{
    Vec2f pos;

    if (attack_coll->attack_state == nGMAttackStateTransfer)
    {
        pos.x = fp->physics.vel_air.x;
        pos.y = fp->physics.vel_air.y;
    }
    else
    {
        pos.x = attack_coll->pos_curr.x - attack_coll->pos_prev.x;
        pos.y = attack_coll->pos_curr.y - attack_coll->pos_prev.y;
    }
    return syUtilsArcTan2(pos.y, pos.x);
}
