#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x803929D0
s32 sSCSubsysPad0x803929D0[2];

// 0x803929D8
f32 sSCSubsysFighterLightAngleX;

// 0x803929DC
f32 sSCSubsysFighterLightAngleY;

// 0x803929E0
SYColorRGBA sSCSubsysLightsColor;

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x803904E0
void scSubsysFighterSetLightParams(f32 light_angle_x, f32 light_angle_y, u8 r, u8 g, u8 b, u8 a)
{
    sSCSubsysFighterLightAngleX = light_angle_x;
    sSCSubsysFighterLightAngleY = light_angle_y;

    sSCSubsysLightsColor.r = r;
    sSCSubsysLightsColor.g = g;
    sSCSubsysLightsColor.b = b;
    sSCSubsysLightsColor.a = a;
}

// 0x8039051C
f32 scSubsysFighterGetLightAngleX(void)
{
    return sSCSubsysFighterLightAngleX;
}

// 0x80390528
f32 scSubsysFighterGetLightAngleY(void)
{
    return sSCSubsysFighterLightAngleY;
}

// 0x80390534
u8 scSubsysFighterDrawLightColorGetAlpha(Gfx **dls)
{
    gDPSetEnvColor(dls[0]++, sSCSubsysLightsColor.r, sSCSubsysLightsColor.g, sSCSubsysLightsColor.b, sSCSubsysLightsColor.a);
    
    return sSCSubsysLightsColor.a;
}

// 0x80390584
void scSubsysFighterProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftMainRunUpdateColAnim(fighter_gobj);
        
    if (fp->proc_update != NULL)
    {
        fp->proc_update(fighter_gobj);
    }
}

// 0x803905CC
void scSubsysFighterSetStatus(GObj *fighter_gobj, s32 status_id)
{
    ftMainSetStatus(fighter_gobj, status_id, FTSTATUS_PRESERVE_NONE, 1.0F, 0.0F);
}

// 0x803905F4 - Is this to attach models to Master Hand in the opening movie?
#if defined(REGION_US)
void scSubsysFighterOpeningProcUpdate(GObj *this_gobj, GObj *other_gobj)
{
    FTStruct *fp = ftGetStruct(this_gobj);
    s32 unused;
    DObj *child_dobj = DObjGetStruct(other_gobj)->child;
    Mtx44f mtx_f;
    
    func_ovl0_800C9A38(mtx_f, fp->joints[fp->attr->joint_itemheavy_id]);
    func_ovl2_800EDA0C(mtx_f, &DObjGetStruct(other_gobj)->rotate.vec.f);
    
    DObjGetStruct(other_gobj)->translate.vec.f.x = -child_dobj->translate.vec.f.x;
    DObjGetStruct(other_gobj)->translate.vec.f.y = -child_dobj->translate.vec.f.y;
    DObjGetStruct(other_gobj)->translate.vec.f.z = -child_dobj->translate.vec.f.z;
    
    gmCollisionGetWorldPosition(mtx_f, &DObjGetStruct(other_gobj)->translate.vec.f);
}
#else
void scSubsysFighterOpeningProcUpdate(GObj *this_gobj, GObj *other_gobj)
{
    FTStruct *fp = ftGetStruct(this_gobj);
    s32 unused;
    FTParts *ftparts;
    DObj *child_dobj = DObjGetStruct(other_gobj)->child;
    
    func_ovl2_800EDBA4(fp->joints[fp->attr->joint_itemheavy_id]);
    
    ftparts = fp->joints[fp->attr->joint_itemheavy_id]->user_data.p;
    func_ovl2_800EDA0C(ftparts->mtx_translate, &DObjGetStruct(other_gobj)->rotate.vec.f);
    
    DObjGetStruct(other_gobj)->translate.vec.f.x = -child_dobj->translate.vec.f.x;
    DObjGetStruct(other_gobj)->translate.vec.f.y = -child_dobj->translate.vec.f.y;
    DObjGetStruct(other_gobj)->translate.vec.f.z = -child_dobj->translate.vec.f.z;
    
    gmCollisionGetWorldPosition(ftparts->mtx_translate, &DObjGetStruct(other_gobj)->translate.vec.f);
}
#endif

// 0x8039069C
void scSubsysFighterApplyVelTransN(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    DObj *transn_joint = fp->joints[nFTPartsJointTransN], *topn_joint = fp->joints[nFTPartsJointTopN];
    
    topn_joint->translate.vec.f.x += (transn_joint->translate.vec.f.x - fp->anim_vel.x);
    topn_joint->translate.vec.f.y += (transn_joint->translate.vec.f.y - fp->anim_vel.y);
    topn_joint->translate.vec.f.z += (transn_joint->translate.vec.f.z - fp->anim_vel.z);
}
