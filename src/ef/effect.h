#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "eftypes.h"
#include "effunctions.h"

#define efGetStruct(effect_gobj) \
((EFStruct*)(effect_gobj)->user_data.p) \

extern void *gEFManagerFiles[3];
extern s32 gEFManagerParticleBankID;

#endif
