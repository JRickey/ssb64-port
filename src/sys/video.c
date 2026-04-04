#include "video.h"
#include <PR/mbi.h>
#include <sys/scheduler.h>

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// Pointer to Z-Buffer
u16 *gSYVideoZBuffer;

// Color depth (16-bit or 32-bit)
u32 gSYVideoColorDepth;

// Resolution width
s32 gSYVideoResWidth;

// Resolution height
s32 gSYVideoResHeight;

// Flags for scheduler
u32 sSYVideoFlags;

// Whether video settings have been recently modified
sb32 sSYVideoIsSettingsChanged;

// Private framebuffer pointers
void *sSYVideoFramebuffers[3];

// Left offset
s16 gSYVideoOffsetLeft;

// Right offset
s16 gSYVideoOffsetRight;

// Top offset
s16 gSYVideoOffsetTop;

// Bottom offset
s16 gSYVideoOffsetBottom;

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

/*
 * Convert an RBGA32 color value into a packed set of RBGA5551
 * that can be used with gDPSetFillColor.
 * Depends on the state of `gSYVideoColorDepth`.
 * color is a 32-bit pack of color values (RRGGBBAA).
 */
u32 syVideoGetFillColor(u32 color)
{
    // GPACK_RGBA5551, but it doesn't seem to match if that macro is used
    u32 packed = ((color >> 16) & 0xF800) | ((color >> 13) & 0x07C0) | ((color >> 10) & 0x003E) | ((color >> 7) & 0x0001);

    return (gSYVideoColorDepth == G_IM_SIZ_32b) ? color : (packed << 16) | packed;
}

// Set new framebuffers
void syVideoSetFramebuffers(void *fb0, void *fb1, void *fb2)
{
    SYTaskFramebuffer mesg;

    mesg.info.type = nSYTaskTypeFramebuffers;
    mesg.info.priority = 100;

    sSYVideoFramebuffers[0] = mesg.framebuffers[0] = fb0;
    sSYVideoFramebuffers[1] = mesg.framebuffers[1] = fb1;
    sSYVideoFramebuffers[2] = mesg.framebuffers[2] = fb2;

    func_80000970(&mesg.info);
}

// Set video output flags
void syVideoSetFlags(u32 flags)
{
    sSYVideoFlags |= flags;

    if (flags & SYVIDEO_FLAG_COLORDEPTH32)
    {
        gSYVideoColorDepth = G_IM_SIZ_32b;
    }
    if (flags & SYVIDEO_FLAG_COLORDEPTH16)
    {
        gSYVideoColorDepth = G_IM_SIZ_16b;
    }
    sSYVideoIsSettingsChanged = TRUE;
}

// Set current screen resolution width
void syVideoSetResWidth(s32 width)
{
    gSYVideoResWidth = width;
    sSYVideoIsSettingsChanged = TRUE;
}

// Set current screen resolution height
void syVideoSetResHeight(s32 height)
{
    gSYVideoResHeight = height;
    sSYVideoIsSettingsChanged = TRUE;
}

// Set screen offsets from center
void syVideoSetCenterOffsets(s16 left, s16 right, s16 top, s16 bottom)
{
    gSYVideoOffsetLeft = left;
    gSYVideoOffsetRight = right;
    gSYVideoOffsetTop = top;
    gSYVideoOffsetBottom = bottom;

    sSYVideoIsSettingsChanged = TRUE;
}

// Initialize video task
void syVideoInitViTask(SYTaskVi *task)
{
    task->width = gSYVideoResWidth;
    task->height = gSYVideoResHeight;
    task->flags = sSYVideoFlags;
    task->edgeOffsetLeft = gSYVideoOffsetLeft;
    task->edgeOffsetRight = gSYVideoOffsetRight;
    task->edgeOffsetTop = gSYVideoOffsetTop;
    task->edgeOffsetBottom = gSYVideoOffsetBottom;

    sSYVideoFlags = SYVIDEO_FLAG_NONE;
    sSYVideoIsSettingsChanged = FALSE;
}

// Apply non-blocking settings
void syVideoApplySettingsNoBlock(SYTaskVi *task)
{
    if (sSYVideoIsSettingsChanged != FALSE) 
    {
        task->info.type     = nSYTaskTypeVi;
        task->info.priority = 50;
        task->info.fnCheck  = NULL;
        task->info.mq       = NULL;
        syVideoInitViTask(task);
        osSendMesg(&gSYSchedulerTaskMesgQueue, (OSMesg)task, OS_MESG_NOBLOCK);
    }
}

// Set various video output settings
void syVideoSetScreenSettings(s32 width, s32 height, u32 flags)
{
    SYTaskVi task;

    sSYVideoFlags = SYVIDEO_FLAG_NONE;
    gSYVideoColorDepth = G_IM_SIZ_16b;

    syVideoSetFlags(flags);
    syVideoSetResWidth(width);
    syVideoSetResHeight(height);

    task.info.type = nSYTaskTypeVi;
    task.info.priority = 100;

    syVideoInitViTask(&task);
    func_80000970(&task.info);
}

// Apply video setup
void syVideoInit(SYVideoSetup *video_setup)
{
    syVideoSetFramebuffers(video_setup->framebuffers[0], video_setup->framebuffers[1], video_setup->framebuffers[2]);
    gSYVideoZBuffer = video_setup->zbuffer;
    syVideoSetScreenSettings(video_setup->width, video_setup->height, video_setup->flags);
}
