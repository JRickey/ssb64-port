#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8015A3C0
void ftBossYubideppou3ProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if ((fp->passive_vars.boss.p->wait_div == 1.5F) && (fighter_gobj->anim_frame >= 20.0F) && (fp->status_vars.boss.yubideppou.bullet_count != 3))
    {
        ftBossYubideppou3SetStatus(fighter_gobj);
    }
    else if (fighter_gobj->anim_frame <= 0.0F)
    {
        ftBossWaitSetStatus(fighter_gobj);
    }
}

// 0x8015A45C
void ftBossYubideppou3ProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    Vec3f pos;

    if (fp->status_vars.boss.yubideppou.shoot_timer != 0)
    {
        fp->status_vars.boss.yubideppou.shoot_timer--;

        if (fp->status_vars.boss.yubideppou.shoot_timer == 0)
        {
            pos.x = pos.y = pos.z = 0.0F;

            gmCollisionGetFighterPartsWorldPosition(fp->joints[19], &pos);

            if ((fp->passive_vars.boss.p->wait_div == 1.5F) && (fp->status_vars.boss.yubideppou.bullet_count != 3))
            {
                wpBossBulletHardMakeWeapon(fighter_gobj, &pos);
            }
            else wpBossBulletNormalMakeWeapon(fighter_gobj, &pos); 
        }
    }
    if (fp->passive_vars.boss.p->wait_div == 1.5F)
    {
        ftBossYubideppou2UpdatePosition(fighter_gobj);
    }
}

// 0x8015A538
void ftBossYubideppou3SetStatus(GObj *fighter_gobj)
{
    FTStruct *fp;
    Vec3f pos;

    ftMainSetStatus(fighter_gobj, nFTBossStatusYubideppou3, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);

    fp = ftGetStruct(fighter_gobj);

    fp->status_vars.boss.yubideppou.bullet_count++;

    pos.x = 0.0F;
    pos.y = 0.0F;
    pos.z = 0.0F;

    gmCollisionGetFighterPartsWorldPosition(fp->joints[15], &pos);

    if ((fp->passive_vars.boss.p->wait_div == 1.5F) && (fp->status_vars.boss.yubideppou.bullet_count != 3))
    {
        wpBossBulletHardMakeWeapon(fighter_gobj, &pos);
    }
    else wpBossBulletNormalMakeWeapon(fighter_gobj, &pos);
    
    fp->status_vars.boss.yubideppou.shoot_timer = 4;
}
