#ifndef _SYS_ERROR_H_
#define _SYS_ERROR_H_

#include <ssb_types.h>
#include <macros.h>
#include <PR/gbi.h>
#include <PR/os.h>
#include <sys/objdef.h>
#include <stdarg.h>
#include <string.h>

extern void func_800210C0(s32 arg0, s32 arg1, s32 arg2, sb32 arg3);
extern void func_80021734(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void func_800218E0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern void func_80021908(s32 arg0, s32 arg1, f32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void unref_80021958(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern void syDebugFillRectangle(Gfx *dl, u32 ulx, u32 uly, u32 lrx, u32 lry);
extern void syDebugMeterProcDisplay(GObj *gobj);
extern GObj* syDebugMakeMeterCamera(s32 link, u32 link_priority, u32 dl_link_priority);
extern void syDebugFramebufferDrawBlackRectangle(s32 ulx, s32 uly, s32 width, s32 height);
extern void syDebugFramebufferWriteGlyph(s32 ulx, s32 uly, s32 char_index);
extern char* syDebugMemcpyAdvance(char *dst, const char *src, size_t count);
extern void syDebugFramebufferPrintf(s32 ulx, s32 ulr, char *fmt, ...);
extern void syDebugWaitMsec(s32 millisec);
extern void syDebugFramebufferPrintFloatReg(s32 x, s32 y, s32 reg_id, f32 *fp_reg);
extern void syDebugFramebufferPrintFCSR(u32 fcsr);
extern s32 syDebugWaitFramebufferOrController(u32 buttons, void *framebuf);
extern void syDebugFramebufferPrintThreadStatus(OSThread *t, sb32 is_show_summary);
extern OSThread* syDebugGetFaultThread(void);
extern void syDebugSetFuncPrint(void (*func_print)(void));
extern void syDebugResetMesgCursor(s32 x, s32 y);
extern void syDebugFramebufferVPrintfNewLine(const char *fmt, va_list args);
extern void syDebugDebugPrintf(const char *fmt, ...);
extern void syDebugReportCPUBreakFault(void *arg);
extern void syDebugStartRmonThread8(void);
extern void syDebugFileLoaderThread8(void *arg);
extern void syDebugStartRmonThread5Hang(void);
extern void syDebugPrintf(const char *fmt, ...);
extern void syDebugRunFuncPrint(void (*func_print)(void));

#endif /* SYS_CRASH_H */
