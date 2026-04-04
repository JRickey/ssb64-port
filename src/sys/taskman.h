#ifndef _SYTASKMAN_H_
#define _SYTASKMAN_H_

#include <sys/malloc.h>
#include <sys/scheduler.h>
#include <sys/objdef.h>

#include <PR/mbi.h>
#include <PR/ultratypes.h>

// match Nintendo's name to make the text and data symbols
#define NewUcodeInfo(ucode) \
	{ (u64*)ucode##TextStart, (u64*)ucode##DataStart }
#define NullUcodeInfo \
	{ NULL, NULL }
#define EndUncodeInfoArray NullUcodeInfo

typedef enum SYTaskmanStatus
{
	nSYTaskmanStatusDefault,
	nSYTaskmanStatusLoadScene,
	nSYTaskmanStatusUnk2

} SYTaskmanStatus;

typedef struct SYTaskmanDLBuffer
{
	Gfx *start;
	u32 length;

} SYTaskmanDLBuffer; // size = 0x08

typedef struct SYTaskmanSceneSetup
{
	u16 flags;
	void (*func_update)(void);
	void (*func_draw)(void);
	void *arena_start;          	// Start of allocatable memory pool
	size_t arena_size;          	// Size of allocatable memory pool for heap; usually from end of last overlay's .bss section to start of subsys (ovl1)
	s32 taskgfx_num; 				// Number of SYTaskGfx structs?
	s32 contexts_num;           	// Number of contexts? (what even is a task?)
	size_t dl_buffer0_size;
	size_t dl_buffer1_size;
	size_t dl_buffer2_size;
	size_t dl_buffer3_size;
	size_t graphics_arena_size;		// Graphics memory pool size
	u16 rdp_output_buffer_kind;
	s32 rdp_output_buffer_size;
	void (*func_lights)(Gfx**); 	// Lighting callback?
	void (*func_controller)(void); 	// Controller read callback?

} SYTaskmanSceneSetup; // size == 0x40

typedef struct SYTaskmanSetup
{
	SYTaskmanSceneSetup scene_setup;
	u32 gobjthreads_num;
	size_t gobjthreadstack_size;
	u32 gobjthreadstacks_num;
	s32 unk4C;
	u32 gobjprocs_num;
	u32 gobjs_num;
	size_t gobj_size;
	u32 xobjs_num;
	void *matrix_func_list;
	void (*func_eject)(DObjVec*); 	// fn pointer void(*)(struct DObjVec *)
	u32 aobjs_num;
	u32 mobjs_num;
	u32 dobjs_num;
	size_t dobj_size;
	u32 sobjs_num;
	size_t sobj_size;
	u32 cobjs_num;
	size_t cobj_size;
	void (*func_start)(void);	// Scene start function

} SYTaskmanSetup; // size >= 0x8C

extern s32 gSYTaskmanTaskCount;
extern u32 dSYTaskmanUpdateCount;
extern u32 dSYTaskmanFrameCount;
extern Gfx *gSYTaskmanDLHeads[4];
extern Gfx *sSYTaskmanDLBranches[4];
extern SYMallocRegion gSYTaskmanGraphicsHeap;
extern SYMallocRegion gSYTaskmanGeneralHeap;
// Gfx *? Gfx
extern u32 sSYTaskmanUpdateTimeDelta;
// Gfx *? Gfx
extern u32 sSYTaskmanFrameTimeDelta;
extern u16 D_80046626;
extern u16 D_80046628;
extern s32 gSYTaskmanTaskID;

extern void syTaskmanSetFuncSwapBuffer(SYTaskGfxCallback arg0);
extern void syTaskmanInitSegmentF(Gfx **dl);
extern void* syTaskmanMalloc(size_t size, u32 align);
extern void func_80004F78(void);
extern void syTaskmanAppendGfxUcodeLoad(Gfx **dlist, u32 ucodeIdx);
extern void func_800053CC(void);
extern void syTaskmanUpdateDLBuffers(void);
extern void syTaskmanSetLoadScene(void);
extern void syTaskmanStartTask(SYTaskmanSetup *arg);
extern void func_80006B80(void);

#endif
