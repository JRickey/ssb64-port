#ifndef _WPMARIOFIREBALL_H_
#define _WPMARIOFIREBALL_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <wp/wpdef.h>

extern sb32 wpMarioFireballProcUpdate(GObj *weapon_gobj);
extern sb32 wpMarioFireballProcMap(GObj *weapon_gobj);
extern sb32 wpMarioFireballProcHit(GObj *weapon_gobj);
extern sb32 wpMarioFireballProcHop(GObj *weapon_gobj);
extern sb32 wpMarioFireballProcReflector(GObj *weapon_gobj);
extern GObj* wpMarioFireballMakeWeapon(GObj *fighter_gobj, Vec3f *pos, s32 index);

#endif
