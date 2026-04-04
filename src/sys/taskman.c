#include "common.h"
#include <sys/taskman.h>

#include <sys/debug.h>
#include <sys/main.h>
#include <sys/malloc.h>
#include <sys/obj.h>
#include <sys/rdp.h>
#include <sys/video.h>
#include <sys/scheduler.h>
#include <sys/controller.h>

#include <macros.h>
#include <ssb_types.h>
#include <stddef.h>

#include <PR/mbi.h>
#include <PR/ucode.h>
#include <PR/ultratypes.h>

// externs
extern void syTaskmanCheckBufferLengths();

// structures
typedef struct SYTaskmanUcode
{
	/* 0x00 */ u64 *text;
	/* 0x04 */ u64 *data;

} SYTaskmanUcode;

typedef struct SYTaskFunction
{
	/* 0x00 */ u16 flags;
	/* 0x04 */ void (*scene_update)();
	/* 0x08 */ void (*task_update)(struct SYTaskFunction*);
	/* 0x0C */ void (*scene_draw)();
	/* 0x10 */ void (*task_draw)(struct SYTaskFunction*);

} SYTaskFunction; // size == 0x14 (sSYTaskmanDefaultFunction)

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x8003B6E0
s32 dSYTaskmanRdpOutputBufferKind = 0;

// 0x8003B6E4 - Total number of game updates
u32 dSYTaskmanUpdateCount = 0;

// 0x8003B6E8 - Total number of drawn frames
u32 dSYTaskmanFrameCount = 0;

// Ten total ucodes + a terminator?
// 0x8003B6EC
SYTaskmanUcode dSYTaskmanUcodes[/* */] =
{
	NewUcodeInfo(gspF3DEX2_fifo),
	NullUcodeInfo,
	NullUcodeInfo,
	NullUcodeInfo,
	NullUcodeInfo,
	NullUcodeInfo,
	NullUcodeInfo,
	NullUcodeInfo,
	NullUcodeInfo,
	NullUcodeInfo,
	EndUncodeInfoArray
};

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x80045480
s32 sSYTaskmanPad0x80045480[4];

// 0x80045490
OSMesg sSYTaskmanGameTicMesgs[4];

// 0x800454A0
OSMesgQueue sSYTaskmanGameTicMesgQueue;

// 0x800454B8
u16 sSYTaskmanUpdateInterval;

// 0x800454BA
u16 sSYTaskmanFrameInterval;

// 0x800454BC
u32 D_800454BC;

// 0x800454C0
OSMesg D_800454C0[1];

// 0x800454C8
OSMesgQueue D_800454C8;

// 0x800454E0
SYClient sSYTaskmanClient;

// 0x800454E8
unsigned int *sSYTaskmanSegmentFBase; // pointer to Gfx.w1 (segment base addr?)

// 0x800454F0
OSMesg sSYTaskmanContextMesgs[3];

// 0x80045500
OSMesgQueue sSYTaskmanContextMesgQueue; // sctask end? or for all tasks?

// 0x80045518
OSMesg sSYTaskmanResetMesgs[1];

// 0x80045520
OSMesgQueue sSYTaskmanResetMesgQueue;

// 0x80045538
u64 sSYTaskmanDramStack[SP_DRAM_STACK_SIZE64 + 1];

// 0x80045940
u64 sSYTaskmanYieldData[OS_YIELD_DATA_SIZE / sizeof(u64) + 1];

// 0x80046548
SYTaskGfx *sSYTaskmanGfxBuffersStart[2];

// 0x80046550
SYTaskGfx *sSYTaskmanGfxBuffersCurrent[2];

// 0x80046558
SYTaskGfx *sSYTaskmanGfxBuffersEnd[2];

// 0x80046560
SYTaskGfxEnd *sSYTaskmanGfxEndBuffers[2];

// 0x80046568
SYTaskVi *sSYTaskmanViBuffers[2];

// 0x80046570
SYTaskmanDLBuffer sSYTaskmanDLBuffers[2][4];

// 0x800465B0
Gfx *gSYTaskmanDLHeads[4];

// 0x800465C0
Gfx *sSYTaskmanDLBranches[4];

// 0x800465D0 - Writing 1 to this will load the screen at scene_curr (gSCManagerSceneData).
s32 sSYTaskmanStatus;

// 0x800465D4
s32 sSYTaskmanFramebufferID;

// 0x800465D8
SYMallocRegion gSYTaskmanGraphicsHeap;

// 0x800465E8
SYMallocRegion gSYTaskmanGeneralHeap;

// 0x800465F8
SYTaskFunction sSYTaskmanDefaultFunction;

// 0x8004660C
u32 sSYTaskmanTimeStart;

// 0x80046610
u32 sSYTaskmanUpdateTimeDelta;

// 0x80046614
u32 sSYTaskmanFrameTimeDelta;

// 0x80046618
void *sSYTaskmanRdpOutputBuffer; // u64 *?

// 0x8004661C
size_t sSYTaskmanRdpOutputBufferSize;   // size of sSYTaskmanRdpOutputBuffer

// 0x80046620
sb32 sSYTaskmanIsNoNearClipping;

// 0x80046624
u16 D_80046624;

// 0x80046626
u16 D_80046626; // ucode idx?

// 0x80046628
u16 D_80046628;

// 0x8004662C
Gfx *sSYTaskmanRdpResetDL;

// 0x80046630 - offset into sSYTaskmanDefaultGraphicsHeap and sSYTaskmanGfxBuffersStart
s32 gSYTaskmanTaskID;

// 0x80046634
s32 D_80046634;

// 0x80046638
s32 D_80046638[2];

// 0x80046640
s32 gSYTaskmanTaskCount;

// 0x80046644
UNUSED s32 unref80046644;

// 0x80046648
SYMallocRegion sSYTaskmanDefaultGraphicsHeap[2];

// 0x80046668
void (*sSYTaskmanFuncController)(void);

// 0x8004666C
SYTaskCallback D_8004666C;  // function pointer?

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x800048D0
void syTaskmanSetFuncSwapBuffer(SYTaskGfxCallback arg0)
{
	if (arg0 != NULL)
	{
		D_8004666C = (void*)arg0;
	}
	else D_8004666C = (void*)sySchedulerCheckReadyFramebuffer;
}

// 0x800048F8
void syTaskmanInitSegmentF(Gfx **dl)
{
	sSYTaskmanSegmentFBase = &dl[0]->words.w1;
	gSPSegment(dl[0]++, G_MWO_SEGMENT_F, 0x00000000);
}

// 0x80004928
void unref_80004928(sb32 bool)
{
	sSYTaskmanIsNoNearClipping = bool;
}

// 0x80004934
void unref_80004934(u16 arg0, u16 arg1)
{
	D_80046624 = arg0;
	D_80046626 = arg1;
}

// 0x80004950
void syTaskmanInitGeneralHeap(void *start, u32 size)
{
	syMallocInit(&gSYTaskmanGeneralHeap, 0x10000, start, size);
}

// 0x80004980
void* syTaskmanMalloc(size_t size, u32 align) // alloc_with_alignment
{
	return syMallocSet(&gSYTaskmanGeneralHeap, size, align);
}

// 0x800049B0
void syTaskmanResetGraphicsHeap(void) // reset gSYTaskmanGraphicsHeap allocator
{
	gSYTaskmanGraphicsHeap.id    = sSYTaskmanDefaultGraphicsHeap[gSYTaskmanTaskID].id;
	gSYTaskmanGraphicsHeap.start = sSYTaskmanDefaultGraphicsHeap[gSYTaskmanTaskID].start;
	gSYTaskmanGraphicsHeap.end   = sSYTaskmanDefaultGraphicsHeap[gSYTaskmanTaskID].end;
	gSYTaskmanGraphicsHeap.ptr   = sSYTaskmanDefaultGraphicsHeap[gSYTaskmanTaskID].ptr;

	syMallocReset(&gSYTaskmanGraphicsHeap);
}

// 0x80004A0C
void syTaskmanSetDLBuffer(SYTaskmanDLBuffer (*src)[4])
{
	s32 i;

	for (i = 0; i < ARRAY_COUNT(sSYTaskmanDLBuffers); i++)
	{
		sSYTaskmanDLBuffers[i][0] = src[i][0];
		sSYTaskmanDLBuffers[i][1] = src[i][1];
		sSYTaskmanDLBuffers[i][2] = src[i][2];
		sSYTaskmanDLBuffers[i][3] = src[i][3];
	}
}

// 0x80004AB0
void func_80004AB0()
{
	s32 i;

	for (i = 0; i < (ARRAY_COUNT(gSYTaskmanDLHeads) + ARRAY_COUNT(sSYTaskmanDLBranches) + ARRAY_COUNT(sSYTaskmanDLBuffers[0])) / 3; i++)
	{
		gSYTaskmanDLHeads[i] = sSYTaskmanDLBranches[i] = sSYTaskmanDLBuffers[gSYTaskmanTaskID][i].start;
	}
	for (i = 0; i < (ARRAY_COUNT(gSYTaskmanDLHeads) + ARRAY_COUNT(sSYTaskmanDLBranches) + ARRAY_COUNT(sSYTaskmanDLBuffers[0])) / 3; i++)
	{
		if (sSYTaskmanDLBuffers[gSYTaskmanTaskID][i].length != 0)
		{
			sSYTaskmanRdpResetDL = gSYTaskmanDLHeads[i];
			syRdpResetSettings(&gSYTaskmanDLHeads[i]);
			gSPEndDisplayList(gSYTaskmanDLHeads[i]++);
			sSYTaskmanDLBranches[i] = gSYTaskmanDLHeads[i];
			break;
		}
	}
	D_80046628 = 0;
}

// 0x80004B9C
void syTaskmanCheckBufferLengths(void)
{
	s32 i;

	for (i = 0; i < (ARRAY_COUNT(gSYTaskmanDLHeads) + ARRAY_COUNT(sSYTaskmanDLBranches) + ARRAY_COUNT(sSYTaskmanDLBuffers[0])) / 3; i++)
	{
		if (sSYTaskmanDLBuffers[gSYTaskmanTaskID][i].length + (uintptr_t)sSYTaskmanDLBuffers[gSYTaskmanTaskID][i].start < (uintptr_t)gSYTaskmanDLHeads[i])
		{
			syDebugPrintf("gtl : DLBuffer over flow !  kind = %d  vol = %d byte\n", i, (uintptr_t)gSYTaskmanDLHeads[i] - (uintptr_t)sSYTaskmanDLBuffers[gSYTaskmanTaskID][i].start);
			while (TRUE);
		}
	}
	if ((uintptr_t)gSYTaskmanGraphicsHeap.end < (uintptr_t)gSYTaskmanGraphicsHeap.ptr)
	{
		syDebugPrintf("gtl : DynamicBuffer over flow !  %d byte\n", (uintptr_t)gSYTaskmanGraphicsHeap.ptr - (uintptr_t)gSYTaskmanGraphicsHeap.start);
		while (TRUE);
	}
}

// 0x80004C5C
void syTaskmanMakeRdpBufferTask(void *buffer, size_t buffer_size)
{
	SYTaskRdpBuffer t;

	t.info.type     = nSYTaskTypeRdpBuffer;
	t.info.priority = 50;
	t.buffer        = buffer;
	t.size          = buffer_size;
	func_80000970(&t.info);

	if ((uintptr_t)&sSYSchedulerRdpCache % 8)
	{
		syDebugPrintf("bad addr sc_rdp_output_len = %x\n", &sSYSchedulerRdpCache);
		while (TRUE);
	}
}

// 0x80004CB4
void syTaskmanSetRdpOutputBuffer(s32 kind, void *buffer, size_t buffer_size)
{
	dSYTaskmanRdpOutputBufferKind = kind;
	sSYTaskmanRdpOutputBuffer = buffer;
	sSYTaskmanRdpOutputBufferSize = buffer_size;

	if ((kind == 2) || (kind == 1))
	{
		if (buffer_size == 0)
		{
			syDebugPrintf("gtl : Buffer size for RDP is zero !!\n");
			while (TRUE);
		}
	}
	if (kind == 1)
	{
		syTaskmanMakeRdpBufferTask(buffer, buffer_size);
	}
}

// 0x80004D2C
SYTaskGfx* func_80004D2C(void)
{
	SYTaskGfx *t;

	if (sSYTaskmanGfxBuffersStart[gSYTaskmanTaskID] == NULL)
	{
		syDebugPrintf("gtl : not defined SCTaskGfx\n");
		while (TRUE);
	}
	if (sSYTaskmanGfxBuffersCurrent[gSYTaskmanTaskID] == sSYTaskmanGfxBuffersEnd[gSYTaskmanTaskID])
	{
		syDebugPrintf("gtl : couldn\'t get SCTaskGfx\n");
		while (TRUE);
	}
	t = sSYTaskmanGfxBuffersCurrent[gSYTaskmanTaskID]++;

	return t;
}

// 0x80004DB4
void func_80004DB4(SYTaskGfx *gfx, s32 gfx_num, SYTaskGfxEnd *gfxend, SYTaskVi *vi)
{
	s32 i;

	for (i = 0; i < gSYTaskmanTaskCount; i++)
	{
		sSYTaskmanGfxBuffersStart[i] = (SYTaskGfx*) ((uintptr_t)gfx + (gfx_num * sizeof(SYTaskGfx)) * i);
		sSYTaskmanGfxBuffersCurrent[i] = (SYTaskGfx*) ((uintptr_t)gfx + (gfx_num * sizeof(SYTaskGfx)) * i);
		sSYTaskmanGfxBuffersEnd[i] = (SYTaskGfx*) ((uintptr_t)gfx + (gfx_num * sizeof(SYTaskGfx)) * (i + 1));

		sSYTaskmanGfxEndBuffers[i] = (SYTaskGfxEnd*) ((uintptr_t)gfxend + (i * sizeof(SYTaskGfxEnd)));
		sSYTaskmanViBuffers[i] = (SYTaskVi*) ((uintptr_t)vi + (i * sizeof(SYTaskVi)));
	}
}

// 0x80004E90
void syTaskmanScheduleGfxEnd(SYTaskGfxEnd *mesg, void *framebuffer, u32 retVal, OSMesgQueue *mq)
{
	mesg->info.type 	= nSYTaskTypeGfxEnd;
	mesg->info.priority = 100;
	mesg->info.fnCheck  = NULL;
	mesg->info.mq       = mq;
	mesg->info.retVal   = retVal;
	mesg->fb            = framebuffer;
	mesg->task_id       = gSYTaskmanTaskID;

	osSendMesg(&gSYSchedulerTaskMesgQueue, (OSMesg)mesg, OS_MESG_NOBLOCK);
}

// 0x80004EFC
void func_80004EFC(void)
{
	SYTaskGfxEnd *mesg = sSYTaskmanGfxEndBuffers[gSYTaskmanTaskID];

	if (mesg == NULL)
	{
		syDebugPrintf("gtl : not defined SCTaskGfxEnd\n");
		while (TRUE);
	}
	syTaskmanScheduleGfxEnd(mesg, (void*)-1, gSYTaskmanTaskID, &sSYTaskmanContextMesgQueue);
	sSYTaskmanGfxBuffersCurrent[gSYTaskmanTaskID] = sSYTaskmanGfxBuffersStart[gSYTaskmanTaskID];
}

// 0x80004F78
void func_80004F78(void)
{
	OSMesg recv;
	SYTaskGfxEnd *mesg = sSYTaskmanGfxEndBuffers[gSYTaskmanTaskID];

	if (mesg == NULL)
	{
		syDebugPrintf("gtl : not defined SCTaskGfxEnd\n");
		while (TRUE);
	}
	syTaskmanScheduleGfxEnd(mesg, NULL, gSYTaskmanTaskID, &sSYTaskmanResetMesgQueue);
	osRecvMesg(&sSYTaskmanResetMesgQueue, &recv, OS_MESG_BLOCK);
	sSYTaskmanGfxBuffersCurrent[gSYTaskmanTaskID] = sSYTaskmanGfxBuffersStart[gSYTaskmanTaskID];
	syTaskmanResetGraphicsHeap();
	func_80004AB0();
}

// 0x80005018
void func_80005018(SYTaskGfx *t, void *framebuffer, u32 ucode_id, s32 arg3, u64 *arg4, u64 *arg5, u32 arg6)
{
	SYTaskmanUcode *ucode;

	t->info.type = nSYTaskTypeGfx;
	t->info.priority = 50;

	if (sSYTaskmanSegmentFBase != NULL)
	{
		t->info.fnCheck = D_8004666C;
		t->unk68 = (void*)sSYTaskmanSegmentFBase;
		sSYTaskmanSegmentFBase = NULL;
	}
	else
	{
		t->info.fnCheck = NULL;
		t->unk68 = NULL;
	}
	t->fb = framebuffer;
	t->framebuffer_id = sSYTaskmanFramebufferID;

	if (framebuffer != 0)
	{
		t->info.mq = &sSYTaskmanContextMesgQueue;
		t->info.retVal = arg3;
	}
	else
		t->info.mq = NULL;

	t->info.unk18 = 2;
	t->task_id    = gSYTaskmanTaskID;
	t->unk7C      = 0;

	t->task.t.type            = M_GFXTASK;
	t->task.t.flags           = OS_TASK_LOADABLE;
	t->task.t.ucode_boot      = gSYMainRspBootCode;
	t->task.t.ucode_boot_size = sizeof(gSYMainRspBootCode);

	ucode = &dSYTaskmanUcodes[ucode_id];

	if (ucode->text == NULL)
	{
		syDebugPrintf("gtl : ucode isn\'t included  kind = %d\n", ucode_id);
		while (TRUE);
	}
	t->task.t.ucode           = ucode->text;
	t->task.t.ucode_data      = ucode->data;
	t->task.t.ucode_size      = SP_UCODE_SIZE;
	t->task.t.ucode_data_size = SP_UCODE_DATA_SIZE;
	t->task.t.dram_stack      = OS_DCACHE_ROUNDUP_ADDR(&sSYTaskmanDramStack);
	t->task.t.dram_stack_size = SP_DRAM_STACK_SIZE8;

	switch (ucode_id)
	{
	case 0:
	case 2:
	case 4:
	case 6:
	case 8:
		// FIFO microcodes..?
		t->task.t.output_buffer = arg5;
		t->task.t.output_buffer_size = (u64*) ((uintptr_t)arg5 + arg6);
		t->unk74 = 2;
		break;

	case 1:
	case 3:
	case 5:
	case 7:
	case 9:
		t->task.t.output_buffer = NULL;
		t->task.t.output_buffer_size = NULL;
		t->unk74 = 0;
		break;
	}
	t->task.t.data_ptr        = arg4;
	t->task.t.data_size       = 0;
	t->task.t.yield_data_ptr  = OS_DCACHE_ROUNDUP_ADDR(&sSYTaskmanYieldData);
	t->task.t.yield_data_size = OS_YIELD_DATA_SIZE;
	osWritebackDCacheAll();
	osSendMesg(&gSYSchedulerTaskMesgQueue, (OSMesg)t, OS_MESG_NOBLOCK);
}

// 0x800051E4
s32 syTaskmanGetUcodeID(void)
{
	s32 ucode_id = (D_80046628 != 0) ? D_80046626 : D_80046624;

	switch (ucode_id)
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 9:
		ucode_id = 9;
		break;

	default:
		ucode_id = 8;
		break;
	}
	return ucode_id;
}

// 0x80005240
void func_80005240(s32 arg0, u64 *arg1)
{
	s32 ucode_id;

	if (arg0 == 0)
	{
		ucode_id = D_80046624;

		if (sSYTaskmanIsNoNearClipping == TRUE)
		{
			switch (ucode_id)
			{
			case 0:
				ucode_id = 2;
				break;
			case 1:
				ucode_id = 3;
				break;
			}
		}
	}
	else ucode_id = syTaskmanGetUcodeID();

	switch (ucode_id)
	{
		case 1:
		case 3:
		case 5:
		case 7:
		case 9:
			func_80005018((void*)func_80004D2C(), 0, ucode_id, gSYTaskmanTaskID, arg1, NULL, 0);
			break;

		case 0:
		case 2:
		case 4:
		case 6:
		case 8:
			func_80005018((void*)func_80004D2C(), 0, ucode_id, gSYTaskmanTaskID, arg1, sSYTaskmanRdpOutputBuffer, sSYTaskmanRdpOutputBufferSize);
			break;
	}
}

// 0x80005344
void syTaskmanAppendGfxUcodeLoad(Gfx **dl, u32 ucode_id)
{
	switch (ucode_id)
	{
	case 0:
		gSPLoadUcodeL(dl[0]++, gspF3DEX2_fifo);
		// intentional fall-thru
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	default:
		gSPDisplayList(dl[0]++, sSYTaskmanRdpResetDL);
		break;
	}
}

// 0x800053CC
void func_800053CC()
{
	s32 a0;
	s32 diffs;
	s32 i;
	s32 dl_id;
	Gfx *cmdPtr;

	// calculate bitset of differences
	// bit 1 -> 0
	// 2 -> 1
	// 4 -> 2
	// 8 -> 3
	diffs = 0;

	for (i = 0; i < (ARRAY_COUNT(gSYTaskmanDLHeads) + ARRAY_COUNT(sSYTaskmanDLBranches)) / 2; i++)
	{
		diffs >>= 1;

		if (gSYTaskmanDLHeads[i] != sSYTaskmanDLBranches[i])
		{
			diffs |= 0x8;
		}
	}

	if (diffs != 0)
	{
		if (diffs & 0x1)
		{
			if (diffs & 0x4)
			{
				syTaskmanAppendGfxUcodeLoad(&gSYTaskmanDLHeads[0], syTaskmanGetUcodeID());
				gSPBranchList(gSYTaskmanDLHeads[0]++, sSYTaskmanDLBranches[2]);
			}
			else if (diffs & 0x2)
			{
				if (D_80046628 != 0)
				{
					syTaskmanAppendGfxUcodeLoad(&gSYTaskmanDLHeads[0], D_80046624);
				}
				gSPBranchList(gSYTaskmanDLHeads[0]++, sSYTaskmanDLBranches[1]);
			}
			else if (diffs & 0x8)
			{
				syTaskmanAppendGfxUcodeLoad(&gSYTaskmanDLHeads[0], syTaskmanGetUcodeID());
				gSPBranchList(gSYTaskmanDLHeads[0]++, sSYTaskmanDLBranches[3]);
			}
			else
			{
				gDPFullSync(gSYTaskmanDLHeads[0]++);
				gSPEndDisplayList(gSYTaskmanDLHeads[0]++);
			}
		}
		if (diffs & 0x4)
		{
			if (diffs & 0x2)
			{
				syTaskmanAppendGfxUcodeLoad(&gSYTaskmanDLHeads[2], D_80046624);
				gSPBranchList(gSYTaskmanDLHeads[2]++, sSYTaskmanDLBranches[1]);
			}
			else if (diffs & 0x8)
			{
				gSPBranchList(gSYTaskmanDLHeads[2]++, sSYTaskmanDLBranches[3]);
			}
			else
			{
				gDPFullSync(gSYTaskmanDLHeads[2]++);
				gSPEndDisplayList(gSYTaskmanDLHeads[2]++);
			}
		}
		if (diffs & 0x2)
		{
			if (diffs & 0x8)
			{
				syTaskmanAppendGfxUcodeLoad(&gSYTaskmanDLHeads[0], syTaskmanGetUcodeID());
				gSPBranchList(gSYTaskmanDLHeads[1]++, sSYTaskmanDLBranches[3]);
			}
			else
			{
				gDPFullSync(gSYTaskmanDLHeads[1]++);
				gSPEndDisplayList(gSYTaskmanDLHeads[1]++);
			}
		}
		if (diffs & 0x8)
		{
			gDPFullSync(gSYTaskmanDLHeads[3]++);
			gSPEndDisplayList(gSYTaskmanDLHeads[3]++);
		}
		if (diffs & 0x1)
		{
			dl_id = 0;
			a0    = 0;
		}
		else if (diffs & 0x4)
		{
			dl_id = 2;
			a0    = 1;
		}
		else if (diffs & 0x2)
		{
			dl_id = 1;
			a0    = 0;
		}
		else
		{
			dl_id = 3;
			a0    = 1;
		}
		cmdPtr = gSYTaskmanDLHeads[dl_id];
		gSPDisplayList(gSYTaskmanDLHeads[dl_id]++, sSYTaskmanRdpResetDL);
		gSPBranchList(gSYTaskmanDLHeads[dl_id]++, sSYTaskmanDLBranches[dl_id]);
		func_80005240(a0, (u64*)cmdPtr);

		sSYTaskmanDLBranches[0] = gSYTaskmanDLHeads[0];
		sSYTaskmanDLBranches[2] = gSYTaskmanDLHeads[2];
		sSYTaskmanDLBranches[1] = gSYTaskmanDLHeads[1];
		sSYTaskmanDLBranches[3] = gSYTaskmanDLHeads[3];
	}
	syTaskmanCheckBufferLengths();
}

// 0x800057C8
void syTaskmanUpdateDLBuffers(void)
{
	s32 i;
	s32 diffs;

	syTaskmanCheckBufferLengths();

	diffs = 0;

	for (i = 0; i < (ARRAY_COUNT(gSYTaskmanDLHeads) + ARRAY_COUNT(sSYTaskmanDLBranches)) / 2; i++)
	{
		diffs >>= 1;

		if (gSYTaskmanDLHeads[i] != sSYTaskmanDLBranches[i])
		{
			diffs |= 8;
		}
	}
	if (diffs != 0)
	{
		if (diffs & 1)
		{
			if (diffs & 4)
			{
				syTaskmanAppendGfxUcodeLoad(&gSYTaskmanDLHeads[0], syTaskmanGetUcodeID());
				gSPBranchList(gSYTaskmanDLHeads[0]++, sSYTaskmanDLBranches[2]);
			}
			else if (diffs & 2)
			{
				if (D_80046628 != 0)
				{
					syTaskmanAppendGfxUcodeLoad(&gSYTaskmanDLHeads[0], D_80046624);
				}
				gSPBranchList(gSYTaskmanDLHeads[0]++, sSYTaskmanDLBranches[1]);
			}
			else if (diffs & 8)
			{
				syTaskmanAppendGfxUcodeLoad(&gSYTaskmanDLHeads[0], syTaskmanGetUcodeID());
				gSPBranchList(gSYTaskmanDLHeads[0]++, sSYTaskmanDLBranches[3]);
			}
		}
		if (diffs & 4)
		{
			if (diffs & 2)
			{
				syTaskmanAppendGfxUcodeLoad(&gSYTaskmanDLHeads[2], D_80046624);
				gSPBranchList(gSYTaskmanDLHeads[2]++, sSYTaskmanDLBranches[1]);
			}
			else if (diffs & 8)
			{
				gSPBranchList(gSYTaskmanDLHeads[2]++, sSYTaskmanDLBranches[3]);
			}
			else
			{
				syTaskmanAppendGfxUcodeLoad(&gSYTaskmanDLHeads[2], D_80046624);
				gSPBranchList(gSYTaskmanDLHeads[2]++, gSYTaskmanDLHeads[0]);
			}
			sSYTaskmanDLBranches[2] = gSYTaskmanDLHeads[2];
		}
		if (diffs & 2)
		{
			if (diffs & 8)
			{
				syTaskmanAppendGfxUcodeLoad(&gSYTaskmanDLHeads[0], syTaskmanGetUcodeID());
				gSPBranchList(gSYTaskmanDLHeads[1]++, sSYTaskmanDLBranches[3]);
			}
			else
			{
				if (D_80046628 != 0)
				{
					syTaskmanAppendGfxUcodeLoad(&gSYTaskmanDLHeads[1], D_80046624);
				}
				gSPBranchList(gSYTaskmanDLHeads[1]++, gSYTaskmanDLHeads[0]);
			}
			sSYTaskmanDLBranches[1] = gSYTaskmanDLHeads[1];
		}
		if (diffs & 8)
		{
			syTaskmanAppendGfxUcodeLoad(&gSYTaskmanDLHeads[3], D_80046624);
			gSPBranchList(gSYTaskmanDLHeads[3]++, gSYTaskmanDLHeads[0]);
			sSYTaskmanDLBranches[3] = gSYTaskmanDLHeads[3];
		}
	}
	D_80046628 = 0;
	syTaskmanCheckBufferLengths();
}

// 0x80005AE4
sb32 syTaskmanSwitchContext(s32 arg0)
{
	s32 msg;
	s32 i;

	while (osRecvMesg(&sSYTaskmanContextMesgQueue, (OSMesg*)&msg, OS_MESG_NOBLOCK) != -1)
	{
		D_80046638[msg] = 0;
	}
	do
	{
		for (i = 0; i < gSYTaskmanTaskCount; i++)
		{
			if (D_80046638[i] == 0)
			{
				gSYTaskmanTaskID = i;
				D_80046638[i] = 1;
				return 1;
			}
		}
		if (arg0 == 0)
		{
			osRecvMesg(&sSYTaskmanContextMesgQueue, (OSMesg*)&msg, OS_MESG_BLOCK);
			D_80046638[msg] = 0;
		}
	}
	while (arg0 == 0);

	return 0;
}

// 0x80005BFC
void func_80005BFC(void)
{
	SYTaskInfo info;
	OSMesg msgs[1];
	OSMesgQueue mq;

	info.type = nSYTaskTypeNoOp;
	info.priority = 50;
	osCreateMesgQueue(&mq, msgs, ARRAY_COUNT(msgs));
	info.fnCheck  = func_80000B54;
	info.retVal = 1;
	info.mq = &mq;

	osSendMesg(&gSYSchedulerTaskMesgQueue, (OSMesg)&info, OS_MESG_NOBLOCK);
	osRecvMesg(&mq, NULL, OS_MESG_BLOCK);
}

// 0x80005C74
void syTaskmanSetLoadScene(void)
{
	sSYTaskmanStatus = nSYTaskmanStatusLoadScene;
}

// 0x80005C84
void unref_80005C84(s32 framebuffer_id)
{
	sSYTaskmanStatus = nSYTaskmanStatusUnk2;
	sSYTaskmanFramebufferID = framebuffer_id;
}

// 0x80005C9C
sb32 syTaskmanCheckBreakLoop(void)
{
	SYTaskInfo info;

	switch (sSYTaskmanStatus)
	{
	case nSYTaskmanStatusLoadScene:
		return TRUE;

	case nSYTaskmanStatusUnk2:
		if (gSYSchedulerIsCustomFramebuffer != FALSE)
		{
			info.type = SC_TASK_TYPE_11;
			info.priority = 100;
			func_80000970(&info);

			return TRUE;
		}
		else return FALSE;

	default:
		return FALSE;
	}
}

// 0x80005D10
void func_80005D10()
{
	if (D_800454BC == 1)
	{
		D_800454BC = 2;

		while (osRecvMesg(&D_800454C8, NULL, OS_MESG_NOBLOCK) != -1)
			continue;

		osRecvMesg(&D_800454C8, NULL, OS_MESG_BLOCK);
		D_800454BC = 0;
	}
}

// 0x80005DA0
void syTaskmanRunTask(SYTaskFunction *tfunc)
{
	s32 i;
	D_800454BC = 0;

	while (osRecvMesg(&sSYTaskmanContextMesgQueue, NULL, OS_MESG_NOBLOCK) != -1)
	{
		continue;
	}
	while (osRecvMesg(&sSYTaskmanResetMesgQueue, NULL, OS_MESG_NOBLOCK) != -1)
	{
		continue;
	}
	while (osRecvMesg(&sSYTaskmanGameTicMesgQueue, NULL, OS_MESG_NOBLOCK) != -1)
	{
		continue;
	}
	sSYTaskmanStatus = nSYTaskmanStatusDefault;
	sSYTaskmanFramebufferID = -1;
	gSYTaskmanTaskID = 1;
	gSYSchedulerIsCustomFramebuffer = 0;

	for (i = 0; i < ARRAY_COUNT(D_80046638); i++)
	{
		D_80046638[i] = 0;
	}
	if (tfunc->flags & 1)
	{
		while (TRUE)
		{
			func_80005D10();
			syMainVerifyStackProbes();

			for (i = 0; i < sSYTaskmanUpdateInterval; i++)
			{
				osRecvMesg(&sSYTaskmanGameTicMesgQueue, NULL, OS_MESG_BLOCK);
			}
			while (osRecvMesg(&sSYTaskmanGameTicMesgQueue, NULL, OS_MESG_NOBLOCK) != -1)
			{
				continue;
			}
			sSYTaskmanTimeStart = osGetCount();

			tfunc->task_update(tfunc);

			dSYTaskmanUpdateCount++;
			sSYTaskmanUpdateTimeDelta = (osGetCount() - sSYTaskmanTimeStart) / 2971; // what is this constant?

			if (syTaskmanCheckBreakLoop() != FALSE)
			{
				break;
			}
			if (dSYTaskmanUpdateCount % sSYTaskmanFrameInterval == 0)
			{
				syTaskmanSwitchContext(0);
				sSYTaskmanTimeStart = osGetCount();

				tfunc->task_draw(tfunc);

				dSYTaskmanFrameCount++;
				sSYTaskmanFrameTimeDelta = (osGetCount() - sSYTaskmanTimeStart) / 2971;

				if (syTaskmanCheckBreakLoop() != FALSE)
				{
					break;
				}
			}
		}
	}
	else while (TRUE)
	{
		func_80005D10();
		syMainVerifyStackProbes();

		for (i = 0; i < sSYTaskmanUpdateInterval; i++)
		{
			osRecvMesg(&sSYTaskmanGameTicMesgQueue, NULL, OS_MESG_BLOCK);
		}
		while (osRecvMesg(&sSYTaskmanGameTicMesgQueue, NULL, OS_MESG_NOBLOCK) != -1)
		{
			continue;
		}
		sSYTaskmanTimeStart = osGetCount();

		tfunc->task_update(tfunc);

		dSYTaskmanUpdateCount++;
		sSYTaskmanUpdateTimeDelta = (osGetCount() - sSYTaskmanTimeStart) / 2971;

		if (syTaskmanCheckBreakLoop() != FALSE)
		{
			break;
		}
		if ((dSYTaskmanUpdateCount % sSYTaskmanFrameInterval == 0) && (syTaskmanSwitchContext(1) != FALSE))
		{
			sSYTaskmanTimeStart = osGetCount();

			tfunc->task_draw(tfunc);

			dSYTaskmanFrameCount++;
			sSYTaskmanFrameTimeDelta = (osGetCount() - sSYTaskmanTimeStart) / 2971;

			if (syTaskmanCheckBreakLoop() != FALSE)
			{
				break;
			}
		}
	}
	func_80005BFC();

	while (osRecvMesg(&sSYTaskmanContextMesgQueue, NULL, OS_MESG_NOBLOCK) != -1)
	{
		continue;
	}
	while (osRecvMesg(&sSYTaskmanResetMesgQueue, NULL, OS_MESG_NOBLOCK) != -1)
	{
		continue;
	}
	while (osRecvMesg(&sSYTaskmanGameTicMesgQueue, NULL, OS_MESG_NOBLOCK) != -1)
	{
		continue;
	}
	syRdpSetFuncLights(NULL);
	D_800454BC = 2;
}

// 0x800062B4
void func_800062B4(SYTaskFunction *tfunc)
{
	sSYTaskmanFuncController();
	tfunc->scene_update();
}

// 0x800062EC
void func_800062EC(SYTaskFunction *tfunc)
{
	syTaskmanResetGraphicsHeap();
	func_80004AB0();

	tfunc->scene_draw();

	func_800053CC();
	syVideoApplySettingsNoBlock(sSYTaskmanViBuffers[gSYTaskmanTaskID]);
	func_80004EFC();
}

// 0x80006350
void syTaskmanCommonTaskUpdate(SYTaskFunction *tfunc)
{
	sSYTaskmanFuncController();
	tfunc->scene_update();

	if (syTaskmanCheckBreakLoop() != FALSE)
	{
		gcEjectAll();
	}
}

// 0x800063A0
void syTaskmanCommonTaskDraw(SYTaskFunction *tfunc)
{
	syTaskmanResetGraphicsHeap();
	func_80004AB0();

	tfunc->scene_draw();

	func_800053CC();
	syVideoApplySettingsNoBlock(sSYTaskmanViBuffers[gSYTaskmanTaskID]);
	func_80004EFC();

	if (syTaskmanCheckBreakLoop() != FALSE)
	{
		gcEjectAll();
	}
}

// 0x8000641C
void unref_8000641C(GObj *gobj)
{
	s32 id;
	SYTaskGfxEnd *task;

	syTaskmanSwitchContext(0);
	syTaskmanResetGraphicsHeap();
	func_80004AB0();

	gobj->proc_display(gobj);

	func_800053CC();

	task = sSYTaskmanGfxEndBuffers[gSYTaskmanTaskID];

	if (task == NULL)
	{
		syDebugPrintf("gtl : not defined SCTaskGfxEnd\n");
		while (TRUE);
	}
	syTaskmanScheduleGfxEnd(task, NULL, gSYTaskmanTaskID, &sSYTaskmanContextMesgQueue);
	sSYTaskmanGfxBuffersCurrent[gSYTaskmanTaskID] = sSYTaskmanGfxBuffersStart[gSYTaskmanTaskID];

	do
	{
		osRecvMesg(&sSYTaskmanContextMesgQueue, (OSMesg*)&id, OS_MESG_BLOCK);
		D_80046638[id] = 0;
	}
	while (D_80046638[gSYTaskmanTaskID] != 0);

#if defined(REGION_US)
	dSYTaskmanFrameCount++;
#endif
}

// 0x80006548
void syTaskmanLoadScene(SYTaskmanSceneSetup *tscene, void (*func_start)(void))
{
	s32 i;
	SYTaskmanDLBuffer dl_buffers[2][4];

	gSYTaskmanTaskCount = tscene->contexts_num;
	sSYTaskmanDefaultFunction.flags = tscene->flags;
	sSYTaskmanDefaultFunction.scene_update = tscene->func_update;
	sSYTaskmanDefaultFunction.scene_draw = tscene->func_draw;

	func_80004DB4
	(
		syTaskmanMalloc(tscene->taskgfx_num * sizeof(SYTaskGfx) * gSYTaskmanTaskCount, 0x8),
		tscene->taskgfx_num,
		syTaskmanMalloc(sizeof(SYTaskGfxEnd) * gSYTaskmanTaskCount, 0x8),
		syTaskmanMalloc(sizeof(SYTaskVi) * gSYTaskmanTaskCount, 0x8)
	);
	for (i = 0; i < gSYTaskmanTaskCount; i++)
	{
		dl_buffers[i][0].start = syTaskmanMalloc(tscene->dl_buffer0_size, 0x8);
		dl_buffers[i][0].length = tscene->dl_buffer0_size;
		dl_buffers[i][1].start = syTaskmanMalloc(tscene->dl_buffer1_size, 0x8);
		dl_buffers[i][1].length = tscene->dl_buffer1_size;
		dl_buffers[i][2].start = syTaskmanMalloc(tscene->dl_buffer2_size, 0x8);
		dl_buffers[i][2].length = tscene->dl_buffer2_size;
		dl_buffers[i][3].start = syTaskmanMalloc(tscene->dl_buffer3_size, 0x8);
		dl_buffers[i][3].length = tscene->dl_buffer3_size;
	}
	syTaskmanSetDLBuffer(dl_buffers);

	for (i = 0; i < gSYTaskmanTaskCount; i++)
	{
		syMallocInit(&gSYTaskmanGraphicsHeap, 0x10002, syTaskmanMalloc(tscene->graphics_arena_size, 0x8), tscene->graphics_arena_size);
		sSYTaskmanDefaultGraphicsHeap[i].id    = gSYTaskmanGraphicsHeap.id;
		sSYTaskmanDefaultGraphicsHeap[i].start = gSYTaskmanGraphicsHeap.start;
		sSYTaskmanDefaultGraphicsHeap[i].end   = gSYTaskmanGraphicsHeap.end;
		sSYTaskmanDefaultGraphicsHeap[i].ptr   = gSYTaskmanGraphicsHeap.ptr;
	}
	tscene->rdp_output_buffer_kind = 2;

	if (tscene->rdp_output_buffer_size == 0)
	{
		tscene->rdp_output_buffer_size = 0x1000;
	}
	syTaskmanSetRdpOutputBuffer(tscene->rdp_output_buffer_kind, syTaskmanMalloc(tscene->rdp_output_buffer_size, 0x10), tscene->rdp_output_buffer_size);
	syRdpSetFuncLights(tscene->func_lights);
	sSYTaskmanFuncController = tscene->func_controller;
	syControllerSetAutoRead((syControllerScheduleRead != sSYTaskmanFuncController) ? TRUE : FALSE);

	dSYTaskmanUpdateCount = dSYTaskmanFrameCount = 0;

	if (func_start != NULL)
	{
		func_start();
	}
	syTaskmanRunTask(&sSYTaskmanDefaultFunction);
}

// 0x800067E4
void unref_800067E4(SYTaskmanSceneSetup *tscene)
{
	syTaskmanInitGeneralHeap(tscene->arena_start, tscene->arena_size);
	sSYTaskmanDefaultFunction.task_update = func_800062B4;
	sSYTaskmanDefaultFunction.task_draw = func_800062EC;
	syTaskmanLoadScene(tscene, NULL);
}

// 0x8000683C
void syTaskmanStartTask(SYTaskmanSetup *tsetup)
{
	GCSetup gcsetup;

	syTaskmanInitGeneralHeap(tsetup->scene_setup.arena_start, tsetup->scene_setup.arena_size);

	gcsetup.gobjthreads = syTaskmanMalloc(sizeof(GObjThread) * tsetup->gobjthreads_num, 0x8);
	gcsetup.gobjthreads_num = tsetup->gobjthreads_num;
	gcsetup.gobjthreadstack_size = tsetup->gobjthreadstack_size;

	if (tsetup->gobjthreadstack_size != 0)
	{
		gcsetup.gobjthreadstacks = syTaskmanMalloc((tsetup->gobjthreadstack_size + offsetof(GObjStack, stack)) * tsetup->gobjthreadstacks_num, 0x8);
	}
	else gcsetup.gobjthreadstacks = NULL;
	
	gcsetup.gobjthreadstacks_num = tsetup->gobjthreadstacks_num;
	gcsetup.unk_gcsetup_0x14 = tsetup->unk4C;

	gcsetup.gobjprocs = syTaskmanMalloc(sizeof(GObjProcess) * tsetup->gobjprocs_num, 0x4);
	gcsetup.gobjprocs_num = tsetup->gobjprocs_num;

	gcsetup.gobjs = syTaskmanMalloc(tsetup->gobj_size * tsetup->gobjs_num, 0x8);
	gcsetup.gobjs_num = tsetup->gobjs_num;
	gcsetup.gobj_size = tsetup->gobj_size;

	gcsetup.xobjs = syTaskmanMalloc(sizeof(XObj) * tsetup->xobjs_num, 0x8);
	gcsetup.xobjs_num = tsetup->xobjs_num;

	gcSetMatrixFuncList(tsetup->matrix_func_list);
	gcsetup.func_eject = tsetup->func_eject;

	gcsetup.aobjs = syTaskmanMalloc(sizeof(AObj) * tsetup->aobjs_num, 0x4);
	gcsetup.aobjs_num = tsetup->aobjs_num;

	gcsetup.mobjs = syTaskmanMalloc(sizeof(MObj) * tsetup->mobjs_num, 0x4);
	gcsetup.mobjs_num = tsetup->mobjs_num;

	gcsetup.dobjs = syTaskmanMalloc(tsetup->dobj_size * tsetup->dobjs_num, 0x8);
	gcsetup.dobjs_num = tsetup->dobjs_num;
	gcsetup.dobj_size = tsetup->dobj_size;

	gcsetup.sobjs = syTaskmanMalloc(tsetup->sobj_size * tsetup->sobjs_num, 0x8);
	gcsetup.sobjs_num = tsetup->sobjs_num;
	gcsetup.sobj_size = tsetup->sobj_size;

	gcsetup.cameras = syTaskmanMalloc(tsetup->cobj_size * tsetup->cobjs_num, 0x8);
	gcsetup.cobjs_num = tsetup->cobjs_num;
	gcsetup.cobj_size = tsetup->cobj_size;

	gcSetupObjman(&gcsetup);

	sSYTaskmanDefaultFunction.task_update = syTaskmanCommonTaskUpdate;
	sSYTaskmanDefaultFunction.task_draw = syTaskmanCommonTaskDraw;

	syTaskmanLoadScene(&tsetup->scene_setup, tsetup->func_start);
}

// 0x80006A8C
void syTaskmanSetIntervals(u16 update, u16 framedraw)
{
	sSYTaskmanUpdateInterval = update;
	sSYTaskmanFrameInterval = framedraw;
}

// 0x80006AA8
void unref_80006AA8()
{
	if (D_800454BC != 2)
		D_800454BC = 1;
}

// 0x80006AD0
s32 unref_80006AD0()
{
	if (D_800454BC == 2)
		return 1;
	else
		return 0;
}

// 0x80006AF8
void unref_80006AF8()
{
	osSendMesg(&D_800454C8, NULL, OS_MESG_NOBLOCK);
}

// 0x80006B24
void unref_80006B24(s32 arg0)
{
	if ((arg0 == 1) || (arg0 == 2))
#if defined(REGION_US)
		gSYTaskmanTaskCount = arg0;
#else
		gSYTaskmanTaskID = arg0;
#endif
}

// 0x80006B44
s32 unref_80006B44(s32 arg0)
{
	if ((arg0 == 1) || (arg0 == 2))
	{
		if ((&D_80046634)[arg0] == 0)
			return 1;
	}
	return 0;
}

// 0x80006B80
void func_80006B80(void)
{
	s32 i, j;

	for
	(
		i = 0;
		i <
		(
			ARRAY_COUNT(sSYTaskmanGfxEndBuffers) + ARRAY_COUNT(sSYTaskmanGfxBuffersStart) +
			ARRAY_COUNT(sSYTaskmanGfxBuffersCurrent) + ARRAY_COUNT(sSYTaskmanGfxBuffersEnd)
		) / 4;
		i++
	)
	{
		sSYTaskmanGfxBuffersEnd[i] = NULL;
		sSYTaskmanGfxBuffersCurrent[i] = NULL;
		sSYTaskmanGfxBuffersStart[i] = NULL;
		sSYTaskmanGfxEndBuffers[i] = NULL;
	}
	sSYTaskmanIsNoNearClipping = FALSE;
	D_80046624 = D_80046626 = 0;

	for (i = 0; i < ARRAY_COUNT(sSYTaskmanDLBuffers); i++)
	{
		for (j = 0; j < ARRAY_COUNT(sSYTaskmanDLBuffers[0]); j++)
		{
			sSYTaskmanDLBuffers[i][j].start  = NULL;
			sSYTaskmanDLBuffers[i][j].length = 0;
		}
	}
	sSYTaskmanSegmentFBase = NULL;
	syTaskmanSetFuncSwapBuffer(NULL);

	sySchedulerAddClient(&sSYTaskmanClient, &sSYTaskmanGameTicMesgQueue, sSYTaskmanGameTicMesgs, ARRAY_COUNT(sSYTaskmanGameTicMesgs));
	osCreateMesgQueue(&sSYTaskmanContextMesgQueue, sSYTaskmanContextMesgs, ARRAY_COUNT(sSYTaskmanContextMesgs));
	osCreateMesgQueue(&sSYTaskmanResetMesgQueue, sSYTaskmanResetMesgs, ARRAY_COUNT(sSYTaskmanResetMesgs));
	sSYTaskmanUpdateInterval = sSYTaskmanFrameInterval = 1;
	osCreateMesgQueue(&D_800454C8, D_800454C0, ARRAY_COUNT(D_800454C0));
	D_800454BC = 2;
}
