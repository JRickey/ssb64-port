#ifndef _WPDISPLAY_H_
#define _WPDISPLAY_H_

#include "wptypes.h"

// Display weapon hitboxes
void wpDisplayHitCollisions(GObj *weapon_gobj);

// Display map collision diamond
void wpDisplayMapCollisions(GObj *weapon_gobj);

// Unknown
void wpDisplayDrawNormal();

// Unknown
void wpDisplayDrawZBuffer();

// Unknown
void wpDisplayMain(GObj *weapon_gobj, void (*proc_display)(GObj*));

// Unknown
void wpDisplayDLHead1(GObj *weapon_gobj);

// Unknown
void wpDisplayDObjDLLinks(GObj *weapon_gobj);

// Unknown
void func_ovl3_80167618(GObj *weapon_gobj);

// Unknown
void wpDisplayDObjTreeDLLinks(GObj *weapon_gobj);

// Render PK Thunder specifically because life is short
void wpDisplayPKThunderProcDisplay(GObj *weapon_gobj);

#endif
