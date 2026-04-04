#include <ft/fighter.h>

// 0x800D9480
sb32 ftAnimEndCheckSetStatus(GObj *fighter_gobj, void (*proc_status)(GObj*))
{
    if (fighter_gobj->anim_frame <= 0.0F)
    {
        proc_status(fighter_gobj);

        return TRUE;
    }
    else return FALSE;
}

// 0x800D94C4
void ftAnimEndSetWait(GObj *fighter_gobj)
{
    ftAnimEndCheckSetStatus(fighter_gobj, ftCommonWaitSetStatus);
}

// 0x800D94E8
void ftAnimEndSetFall(GObj *fighter_gobj)
{
    ftAnimEndCheckSetStatus(fighter_gobj, ftCommonFallSetStatus);
}
