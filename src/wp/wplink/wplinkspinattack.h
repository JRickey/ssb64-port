#ifndef _WPLINKSPINATTACK_H_
#define _WPLINKSPINATTACK_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <wp/wpdef.h>

extern void func_ovl3_8016C9A0(void);
extern sb32 wpLinkSpinAttackProcDead(GObj *weapon_gobj);
extern sb32 wpLinkSpinAttackProcUpdate(GObj *weapon_gobj);
extern sb32 wpLinkSpinAttackProcMap(GObj *weapon_gobj);
extern sb32 wpLinkSpinAttackProcHit(GObj *weapon_gobj);
extern GObj* wpLinkSpinAttackMakeWeapon(GObj *fighter_gobj, Vec3f *pos);

#endif
