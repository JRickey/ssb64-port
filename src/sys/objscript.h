#ifndef _OBJSCRIPT_H_
#define _OBJSCRIPT_H_

#include <ssb_types.h>
#include <sys/objdef.h>

extern s32 gcSetupGObjScript(GObj *this_gobj, s32 id, GObj *next_gobj);
extern GObj* gcAddGObjScript(GObj *gobj, GObjScript *gobjscript);
extern void gcAddGObjScriptByLink(s32 link, s32 id, GObj *gobj);
extern sb32 gcParseGObjScript(void (*func)(GObjScript));

#endif
