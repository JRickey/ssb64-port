#ifndef _DBMENU_H_
#define _DBMENU_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <db/dbdef.h>

extern sb32 gDBMenuIsMenuOpen;

extern void dbMenuUpdateMenuInputs(void);
extern void dbMenuDrawString(DBMenuPosition *menu_pos, const char *str, ...);
extern void dbMenuDrawBorder(s32 arg0, u32 color);
extern void dbMenuDrawBackground(s32 arg0, u32 color);
extern void dbMenuDrawMenuOptionLabel(void *arg0, DBMenuOption *menu_option);
extern void dbMenuDrawMenuOptions(void *arg0, DBMenuOption *menu_option, s32 menu_options_num);
extern void dbMenuDrawCursor(void* arg0, s32 cursor_id);
extern void dbMenuCommonProcDisplay(GObj *gobj);
extern void dbMenuActorProcUpdate(GObj *gobj);
extern void dbMenuMakeMenu(s32 x, s32 y, s32 w, DBMenuOption *menu_options, s32 menu_options_num);
extern void dbMenuDestroyMenu(void);
extern void dbMenuInitMenu(void);

#endif
