#include <sys/obj.h>

#include <sys/objtypes.h>
#include <sys/debug.h>
#include <sys/taskman.h>
#include <sys/objdisplay.h>
#include <sys/rdp.h>
#include <stddef.h>

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

GObjThread *sGCThreadHead;
u32 sGCThreadsActive;
u32 sGCStacksActiveNum;
u32 sGCThreadStackSize;
u32 sUnkUnusedSetup;
GObjThreadStack *sGCThreadStackHead;

void (*sGCProcessFunction)(GObjProcess*);
GObjProcess *sGCProcessHead;
GObjProcess *sGCProcessQueue[6];
u32 sGCProcessesActive;

GObj *gGCCommonLinks[GC_COMMON_MAX_LINKS];
s32 D_80046774_40794;
GObj *sGCCommonLinks[GC_COMMON_MAX_LINKS];
GObj *sGCCommonHead;
GObj *gGCCommonDLLinks[GC_COMMON_MAX_DLLINKS];
GObj *sGCCommonDLLinks[GC_COMMON_MAX_DLLINKS];
s32 sGCCommonsActiveNum;
u16 sGCCommonSize;
s16 sGCCommonsMaxNum;

XObj *sGCMatrixHead;
u32 sGCMatrixesActiveNum;

void (*sGCDrawFuncEject)(DObjVec*);

AObj *sGCAnimHead;
u32 sGCAnimsActiveNum;

MObj *sGCMaterialHead;
u32 sGCMaterialsActive;

DObj *sGCDrawHead;
u32 sGCDrawsActiveNum;
u16 sGCDrawSize;

SObj *sGCSpriteHead;
u32 sGCSpritesActiveNum;
u16 sGCSpriteSize;

CObj *sGCCameraHead;
u32 sGCCamerasActiveNum;
u16 sGCCameraSize;

GObj *gGCCurrentCommon;
GObj *gGCCurrentCamera; // Is this exclusively a camera GObj?
GObj *gGCCurrentDisplay;

GObjProcess *gGCCurrentProcess;
u32 sGCRunStatus;
OSMesg sGCMesgs[1];
OSMesgQueue gGCMesgQueue;

GCGfxLink gGCFrameQueueGfxLinks[64];
u8 D_80046F88[24];

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x80038B70
OSId dGCProcessThreadID = 10000000;

// 0x80038B74
s32 dGCCurrentStatus = nGCStatusSystem;

// 0x8003B878
GCPersp dGCPerspDefault = { NULL, 0, 30.0F, 4.0F / 3.0F, 100.0F, 12800.0F, 1.0F };

// 0x8003B984
GCOrtho dGCOrthoDefault = { NULL, -160.0F, 160.0F, -120.0F, 120.0F, 100.0F, 12800.0F, 1.0F };

// 0x8003B8B4
CObjVec dGCCObjVecDefault = { NULL, { 0.0F, 0.0F, 1500.0F }, { 0.0F, 0.0F, 0.0F }, { 0.0F, 1.0F, 0.0F } };

// 0x8003B8DC
GCTranslate dGCTranslateDefault = { NULL, { 0.0F, 0.0F, 0.0F } };

// 0x8003B8EC
GCRotate dGCRotateDefaultAXYZ = { NULL, 0.0F, { 0.0F, 0.0F, 1.0F } };

// 0x8003B900
GCRotate dGCRotateDefaultRpy = { NULL, 0.0F, { 0.0F, 0.0F, 0.0F } };

// 0x8003B914
GCScale dGCScaleDefault = { NULL, { 1.0F, 1.0F, 1.0F } };

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x800073E0
GObjThread* gcGetGObjThread(void)
{
	GObjThread* gobjthread;

	if (sGCThreadHead == NULL)
	{
		sGCThreadHead = syTaskmanMalloc(sizeof(GObjThread), 0x8);
		sGCThreadHead->next = NULL;
	}

	if (sGCThreadHead == NULL)
	{
		syDebugPrintf("om : couldn't get GObjThread\n");
		while (TRUE);
	}

	gobjthread = sGCThreadHead;
	sGCThreadHead = sGCThreadHead->next;
	sGCThreadsActive++;

	return gobjthread;
}

// 0x8000745C
void gcSetGObjThreadPrevAlloc(GObjThread *gobjthread)
{
	gobjthread->next = sGCThreadHead;
	sGCThreadHead = gobjthread;
	sGCThreadsActive--;
}

// 0x80007488
GObjStack* gcGetGObjStackOfSize(size_t size)
{
	GObjThreadStack *curr, *prev;
	GObjStack *ret;

	curr = sGCThreadStackHead;
	prev = NULL;

	while (curr != NULL)
	{
		if (curr->size == size)
		{
			break;
		}
		prev = curr;
		curr = curr->next;
	}
	if (curr == NULL)
	{
		curr = syTaskmanMalloc(sizeof(GObjThreadStack), 0x4);
		curr->next = NULL;
		curr->stack = NULL;
		curr->size = size;

		if (prev != NULL)
		{
			prev->next = curr;
		}
		else sGCThreadStackHead = curr;
	}

	if (curr->stack != NULL)
	{
		ret = curr->stack;
		curr->stack = curr->stack->next;
	}
	else
	{
		ret = syTaskmanMalloc(size + offsetof(GObjStack, stack), 0x8);
		ret->stack_size = size;
	}
	ret->next = NULL;
	sGCStacksActiveNum++;

	return ret;
}

// 0x80007564
GObjStack* gcGetDefaultGObjStack(void)
{
	return gcGetGObjStackOfSize(sGCThreadStackSize);
}

// 0x80007588
void gcEjectGObjStack(GObjStack *gobjstack)
{
	GObjThreadStack *gobjthreadstack = sGCThreadStackHead;

	while (gobjthreadstack != NULL)
	{
		if (gobjthreadstack->size == gobjstack->stack_size)
		{
			break;
		}
		gobjthreadstack = gobjthreadstack->next;
	}
	if (gobjthreadstack == NULL)
	{
		syDebugPrintf("om : Illegal GObjThreadStack Link\n");
		while (TRUE);
	}
	gobjstack->next = gobjthreadstack->stack;
	gobjthreadstack->stack = gobjstack;
	sGCStacksActiveNum--;
}

// 0x80007604
GObjProcess* gcGetGObjProcess(void)
{
	GObjProcess *gobjproc;

	if (sGCProcessHead == NULL)
	{
		sGCProcessHead = syTaskmanMalloc(sizeof(GObjProcess), 4);
		sGCProcessHead->link_next = NULL;
	}

	if (sGCProcessHead == NULL)
	{
		syDebugPrintf("om : couldn't get GObjProcess\n");
		while (TRUE);
	}

	gobjproc = sGCProcessHead;
	sGCProcessHead = sGCProcessHead->link_next;
	sGCProcessesActive++;

	return gobjproc;
}

// 0x80007680
void gcLinkGObjProcess(GObjProcess *gobjproc)
{
	GObj *parent_gobj = gobjproc->parent_gobj;
	s32 link_id = gobjproc->parent_gobj->link_id;
	GObj *prev_gobj = gobjproc->parent_gobj;

	while (TRUE)
	{
		while (prev_gobj != NULL)
		{
			GObjProcess *prev_gobjproc = prev_gobj->gobjproc_tail;

			while (prev_gobjproc != NULL)
			{
				if (prev_gobjproc->priority == gobjproc->priority)
				{
					gobjproc->priority_next = prev_gobjproc->priority_next;
					prev_gobjproc->priority_next = gobjproc;
					gobjproc->priority_prev = prev_gobjproc;

					goto loop_break;
				}
				prev_gobjproc = prev_gobjproc->link_prev;
			}
			prev_gobj = prev_gobj->link_prev;
		}
		if (link_id != 0)
		{
			prev_gobj = sGCCommonLinks[--link_id];
		}
		else
		{
			gobjproc->priority_next = sGCProcessQueue[gobjproc->priority];
			sGCProcessQueue[gobjproc->priority] = gobjproc;
			gobjproc->priority_prev = NULL;
			break;
		}
	}
loop_break:
	if (gobjproc->priority_next != NULL)
	{
		gobjproc->priority_next->priority_prev = gobjproc;
	}
	if (parent_gobj->gobjproc_tail != NULL)
	{
		parent_gobj->gobjproc_tail->link_next = gobjproc;
	}
	else parent_gobj->gobjproc_head = gobjproc;

	gobjproc->link_prev = parent_gobj->gobjproc_tail;
	gobjproc->link_next = NULL;
	parent_gobj->gobjproc_tail = gobjproc;
}

// 0x80007758
void gcSetGObjProcessPrevAlloc(GObjProcess *gobjproc)
{
	gobjproc->link_next = sGCProcessHead;
	sGCProcessHead = gobjproc;
	sGCProcessesActive--;
}

// 0x80007784
void func_80007784(GObjProcess *gobjproc)
{
	if (gobjproc->priority_prev != NULL)
	{
		gobjproc->priority_prev->priority_next = gobjproc->priority_next;
	}
	else sGCProcessQueue[gobjproc->priority] = gobjproc->priority_next;

	if (gobjproc->priority_next != NULL)
	{
		gobjproc->priority_next->priority_prev = gobjproc->priority_prev;
	}
}

// 0x800077D0
void func_800077D0(GObjProcess *gobjproc)
{
	GObj *gobj = gobjproc->parent_gobj;

	func_80007784(gobjproc);

	if (gobjproc->link_prev != NULL)
	{
		gobjproc->link_prev->link_next = gobjproc->link_next;
	}
	else gobj->gobjproc_head = gobjproc->link_next;

	if (gobjproc->link_next != NULL)
	{
		gobjproc->link_next->link_prev = gobjproc->link_prev;
	}
	else gobj->gobjproc_tail = gobjproc->link_prev;
}

// 0x80007840
GObjProcess* gcGetCurrentGObjProcess(void)
{
	return gGCCurrentProcess;
}

// 0x8000784C
u64* gcGetGObjProcessThreadStack(GObjProcess *gobjproc)
{
	if (gobjproc == NULL)
	{
		gobjproc = gGCCurrentProcess;
	}
	if ((gobjproc != NULL) && (gobjproc->kind == nGCProcessKindThread))
	{
		return gobjproc->exec.gobjthread->stack;
	}
	else return NULL;
}

// 0x800007884
size_t gcGetGObjProcessThreadStackSize(GObjProcess *gobjproc)
{
	if (gobjproc == NULL)
	{
		gobjproc = gGCCurrentProcess;
	}
	if ((gobjproc != NULL) && (gobjproc->kind == nGCProcessKindThread))
	{
		return gobjproc->exec.gobjthread->stack_size;
	}
	else return 0;
}

// 0x800078BC
void gcSetGObjProcessFunction(void (*proc)(GObjProcess*))
{
	sGCProcessFunction = proc;
}

// 0x800078C8
s32 gcGetGObjsActiveNum(void)
{
	GObj *gobj = sGCCommonHead;
	s32 i = 0;

	while (gobj != NULL)
	{
		i++;
		gobj = gobj->link_next;
	}
	return i + sGCCommonsActiveNum;
}

// 0x800078FC
GObj* gcGetGObjSetNextAlloc(void)
{
	GObj *gobj;

	if ((sGCCommonsMaxNum == -1) || (sGCCommonsActiveNum < sGCCommonsMaxNum))
	{
		gobj = sGCCommonHead;

		if (gobj == NULL)
		{
			sGCCommonHead = syTaskmanMalloc(sGCCommonSize, 0x8);
			sGCCommonHead->link_next = NULL;
			gobj = sGCCommonHead;
		}
	}
	else return NULL;

	if (gobj == NULL)
	{
		return NULL;
	}
	sGCCommonHead = gobj->link_next;
	sGCCommonsActiveNum++;

	return gobj;
}

// 0x800079A8
void gcSetGObjPrevAlloc(GObj *gobj)
{
	gobj->link_next = sGCCommonHead;
	sGCCommonHead = gobj;
	sGCCommonsActiveNum--;
}

// 0x800079D4
void gcLinkGObjAfter(GObj *this_gobj, GObj *link_gobj)
{
	this_gobj->link_prev = link_gobj;

	if (link_gobj != NULL)
	{
		this_gobj->link_next = link_gobj->link_next;
		link_gobj->link_next = this_gobj;
	}
	else
	{
		this_gobj->link_next = gGCCommonLinks[this_gobj->link_id];
		gGCCommonLinks[this_gobj->link_id] = this_gobj;
	}
	if (this_gobj->link_next != NULL)
		this_gobj->link_next->link_prev = this_gobj;
	else
		sGCCommonLinks[this_gobj->link_id] = this_gobj;
}

// 0x80007A3C
void gcLinkGObjSPAfter(GObj *this_gobj)
{
	GObj *current_gobj = sGCCommonLinks[this_gobj->link_id];

	while ((current_gobj != NULL) && (current_gobj->link_priority < this_gobj->link_priority))
	{
		current_gobj = current_gobj->link_prev;
	}
	gcLinkGObjAfter(this_gobj, current_gobj);
}

// 0x80007AA8
void gcLinkGObjSPBefore(GObj *this_gobj)
{
	GObj *current_gobj = gGCCommonLinks[this_gobj->link_id];
	GObj *found_gobj;

	while ((current_gobj != NULL) && (this_gobj->link_priority < current_gobj->link_priority))
	{
		current_gobj = current_gobj->link_next;
	}
	if (current_gobj != NULL)
	{
		found_gobj = current_gobj->link_prev;
	}
	else found_gobj = sGCCommonLinks[this_gobj->link_id];

	gcLinkGObjAfter(this_gobj, found_gobj);
}

// 0x80007B30
void gcRemoveGObjFromLinkedList(GObj *this_gobj)
{
	if (this_gobj->link_prev != NULL)
	{
		this_gobj->link_prev->link_next = this_gobj->link_next;
	}
	else gGCCommonLinks[this_gobj->link_id] = this_gobj->link_next;

	if (this_gobj->link_next != NULL)
	{
		this_gobj->link_next->link_prev = this_gobj->link_prev;
	}
	else sGCCommonLinks[this_gobj->link_id] = this_gobj->link_prev;
}

// 0x80007B98
void gcAppendGObjToDLLinkedList(GObj *this_gobj, GObj *dl_link_gobj)
{
	this_gobj->dl_link_prev = dl_link_gobj;

	if (dl_link_gobj != NULL)
	{
		this_gobj->dl_link_next = dl_link_gobj->dl_link_next;
		dl_link_gobj->dl_link_next = this_gobj;
	}
	else
	{
		this_gobj->dl_link_next = gGCCommonDLLinks[this_gobj->dl_link_id];
		gGCCommonDLLinks[this_gobj->dl_link_id] = this_gobj;
	}

	if (this_gobj->dl_link_next != NULL)
	{
		this_gobj->dl_link_next->dl_link_prev = this_gobj;
	}
	else sGCCommonDLLinks[this_gobj->dl_link_id] = this_gobj;
}

// 0x80007C00
void gcDLLinkGObjTail(GObj *this_gobj)
{
	GObj *current_gobj = sGCCommonDLLinks[this_gobj->dl_link_id];

	while ((current_gobj != NULL) && (current_gobj->dl_link_priority < this_gobj->dl_link_priority))
	{
		current_gobj = current_gobj->dl_link_prev;
	}
	gcAppendGObjToDLLinkedList(this_gobj, current_gobj);
}

// 0x80007C6C
void gcDLLinkGObjHead(GObj *this_gobj)
{
	GObj *current_gobj = gGCCommonDLLinks[this_gobj->dl_link_id];
	GObj *found_gobj;

	while ((current_gobj != NULL) && (this_gobj->dl_link_priority < current_gobj->dl_link_priority))
	{
		current_gobj = current_gobj->dl_link_next;
	}
	if (current_gobj != NULL)
	{
		found_gobj = current_gobj->dl_link_prev;
	}
	else found_gobj = sGCCommonDLLinks[this_gobj->dl_link_id];

	gcAppendGObjToDLLinkedList(this_gobj, found_gobj);
}

// 0x80007CF4
void gcRemoveGObjFromDLLinkedList(GObj *this_gobj)
{
	if (this_gobj->dl_link_prev != NULL)
	{
		this_gobj->dl_link_prev->dl_link_next = this_gobj->dl_link_next;
	}
	else gGCCommonDLLinks[this_gobj->dl_link_id] = this_gobj->dl_link_next;

	if (this_gobj->dl_link_next != NULL)
	{
		this_gobj->dl_link_next->dl_link_prev = this_gobj->dl_link_prev;
	}
	else sGCCommonDLLinks[this_gobj->dl_link_id] = this_gobj->dl_link_prev;
}

// 0x80007D5C
XObj* gcGetXObjSetNextAlloc(void)
{
	XObj *xobj;

	if (sGCMatrixHead == NULL)
	{
		sGCMatrixHead = syTaskmanMalloc(sizeof(XObj), 0x8);
		sGCMatrixHead->next = NULL;
	}
	if (sGCMatrixHead == NULL)
	{
		syDebugPrintf("om : couldn't get OMMtx\n");
		while (TRUE);
	}
	xobj = sGCMatrixHead;
	sGCMatrixHead = sGCMatrixHead->next;
	sGCMatrixesActiveNum++;

	return xobj;
}

// 0x80007DD8
void gcSetXObjPrevAlloc(XObj *xobj)
{
	xobj->next = sGCMatrixHead;
	sGCMatrixHead = xobj;
	sGCMatrixesActiveNum--;
}

// 0x80007E04
AObj* gcGetAObjSetNextAlloc(void)
{
	AObj *aobj;

	if (sGCAnimHead == NULL)
	{
		sGCAnimHead = syTaskmanMalloc(sizeof(AObj), 0x4);

		sGCAnimHead->next = NULL;
	}
	if (sGCAnimHead == NULL)
	{
		syDebugPrintf("om : couldn't get AObj\n");
		while (TRUE);
	}
	aobj = sGCAnimHead;
	sGCAnimHead = sGCAnimHead->next;
	sGCAnimsActiveNum++;

	return aobj;
}

// 0x80007E80
void gcAppendAObjToDObj(DObj *dobj, AObj *aobj)
{
	aobj->next = dobj->aobj;
	dobj->aobj = aobj;
}

// 0x80007E90
void gcAppendAObjToMObj(MObj *mobj, AObj *aobj)
{
	aobj->next = mobj->aobj;
	mobj->aobj = aobj;
}

// 0x80007EA0
void gcAppendAObjToCamera(CObj *cobj, AObj *aobj)
{
	aobj->next = cobj->aobj;
	cobj->aobj = aobj;
}

// 0x80007EB0
void gcSetAObjPrevAlloc(AObj *aobj)
{
	aobj->next = sGCAnimHead;
	sGCAnimsActiveNum--;
	sGCAnimHead = aobj;
}

// 0x80007EDC
MObj* gcGetMObjSetNextAlloc(void)
{
	MObj *mobj;

	if (sGCMaterialHead == NULL)
	{
		sGCMaterialHead = syTaskmanMalloc(sizeof(MObj), 0x4);
		sGCMaterialHead->next = NULL;
	}

	if (sGCMaterialHead == NULL)
	{
		syDebugPrintf("om : couldn't get MObj\n");
		while (TRUE);
	}

	mobj = sGCMaterialHead;
	sGCMaterialHead = sGCMaterialHead->next;
	sGCMaterialsActive++;

	return mobj;
}

// 0x80007F58
void gcSetMObjPrevAlloc(MObj *mobj)
{
	mobj->next = sGCMaterialHead;
	sGCMaterialsActive--;
	sGCMaterialHead = mobj;
}

// 0x80007F84
DObj* gcGetDObjSetNextAlloc(void)
{
	DObj *dobj;

	if (sGCDrawHead == NULL)
	{
		sGCDrawHead = syTaskmanMalloc(sGCDrawSize, 0x8);

		sGCDrawHead->alloc_free = NULL;
	}
	if (sGCDrawHead == NULL)
	{
		syDebugPrintf("om : couldn't get DObj\n");
		while (TRUE);
	}
	dobj = sGCDrawHead;
	sGCDrawHead = sGCDrawHead->alloc_free;
	sGCDrawsActiveNum++;

	return dobj;
}

// 0x80008004
void gcSetDObjPrevAlloc(DObj *dobj)
{
	dobj->alloc_free = sGCDrawHead;
	sGCDrawsActiveNum--;
	sGCDrawHead = dobj;
}

// 0x80008030
SObj* gcGetSObjSetNextAlloc(void)
{
	SObj *sobj;

	if (sGCSpriteHead == NULL)
	{
		sGCSpriteHead = syTaskmanMalloc(sGCSpriteSize, 0x8);
		sGCSpriteHead->alloc_free = NULL;
	}
	if (sGCSpriteHead == NULL)
	{
		syDebugPrintf("om : couldn't get SObj\n");
		while (TRUE);
	}
	sobj = sGCSpriteHead;
	sGCSpriteHead = sGCSpriteHead->alloc_free;
	sGCSpritesActiveNum++;

	return sobj;
}

// 0x800080B0
void gcSetSObjPrevAlloc(SObj *sobj)
{
	sobj->alloc_free = sGCSpriteHead;
	sGCSpritesActiveNum--;
	sGCSpriteHead = sobj;
}

// 0x800080DC
CObj *gcGetCObjSetNextAlloc(void)
{
	CObj *cobj;

	if (sGCCameraHead == NULL)
	{
		sGCCameraHead = syTaskmanMalloc(sGCCameraSize, 0x8);
		sGCCameraHead->next = NULL;
	}

	if (sGCCameraHead == NULL)
	{
		syDebugPrintf("om : couldn't get Camera\n");
		while (TRUE);
	}

	cobj = sGCCameraHead;
	sGCCameraHead = sGCCameraHead->next;
	sGCCamerasActiveNum++;

	return cobj;
}

// 0x8000815C
void gcSetCObjPrevAlloc(CObj *cobj)
{
	cobj->next = sGCCameraHead;
	sGCCamerasActiveNum--;
	sGCCameraHead = cobj;
}

// 0x80008188
GObjProcess* gcAddGObjProcess(GObj *gobj, void (*proc)(GObj*), u8 kind, u32 priority)
{
	GObjStack *gobjstack;
	GObjThread *gobjthread;
	GObjProcess *gobjproc;

	if (gobj == NULL)
	{
		gobj = gGCCurrentCommon;
	}
	gobjproc = gcGetGObjProcess();

	if (priority >= 6)
	{
		syDebugPrintf("om : GObjProcess's priority is bad value\n");
		while (TRUE);
	}
	gobjproc->priority = priority;
	gobjproc->kind = kind;
	gobjproc->is_paused = FALSE;
	gobjproc->parent_gobj = gobj;
	gobjproc->func_id = proc;

	switch (kind)
	{
	case nGCProcessKindThread:
		gobjthread = gcGetGObjThread();
		gobjproc->exec.gobjthread = gobjthread;

		gobjstack = gcGetDefaultGObjStack();
		gobjthread->stack = gobjstack->stack;
		gobjthread->stack_size = sGCThreadStackSize;

		osCreateThread
		(
			&gobjthread->thread,
			dGCProcessThreadID++,
			(void (*)(void*)) proc,
			gobj,
			&gobjthread->stack[sGCThreadStackSize / sizeof(u64)],
			51
		);
		gobjthread->stack[7] = 0xFEDCBA98;

		if (dGCProcessThreadID >= 20000000)
		{
			dGCProcessThreadID = 10000000;
		}
		break;

	case nGCProcessKindFunc:
		gobjproc->exec.func = proc;
		break;
	
	default:
		syDebugPrintf("om : GObjProcess's kind is bad value\n");
		while (TRUE);
	}
	gcLinkGObjProcess(gobjproc);

	return gobjproc;
}

// 0x80008304
GObjProcess* unref_80008304(GObj *gobj, void (*proc)(GObj*), u32 pri, s32 thread_id, u32 stack_size)
{
	GObjProcess *gobjproc;	// s0
	GObjThread* gobjthread; // v1 / sp28
	GObjStack *gobjstack;
	OSId tid;

	if (gobj == NULL)
	{
		gobj = gGCCurrentCommon;
	}
	gobjproc = gcGetGObjProcess();

	if (pri >= 6)
	{
		syDebugPrintf("om : GObjProcess's priority is bad value\n");
		while (TRUE);
	}

	gobjproc->priority = pri;
	gobjproc->is_paused = FALSE;
	gobjproc->parent_gobj = gobj;
	gobjproc->func_id = proc;

	gobjproc->exec.gobjthread = gobjthread = gcGetGObjThread();
	gobjproc->kind = nGCProcessKindThread;

	gobjstack = (stack_size == 0) ? gcGetDefaultGObjStack() : gcGetGObjStackOfSize(stack_size);
	gobjthread->stack = gobjstack->stack;
	gobjthread->stack_size = (stack_size == 0) ? sGCThreadStackSize : stack_size;
	tid = (thread_id != -1) ? thread_id : dGCProcessThreadID++;

	osCreateThread
	(
		&gobjthread->thread,
		tid,
		(void (*)(void*)) proc,
		gobj,
		&gobjthread->stack[gobjthread->stack_size / sizeof(u64)],
		51
	);
	gobjthread->stack[7] = 0xFEDCBA98;

	if (dGCProcessThreadID >= 20000000)
	{
		dGCProcessThreadID = 10000000;
	}
	gcLinkGObjProcess(gobjproc);

	return gobjproc;
}

// 0x8000848C
void gcEndGObjProcess(GObjProcess *gobjproc)
{
	GObjStack *gobjstack;

	if ((gobjproc == NULL) || (gobjproc == gGCCurrentProcess))
	{
		sGCRunStatus = 1;

		if (gGCCurrentProcess->kind == nGCProcessKindThread)
		{
			gcSleepCurrentGObjThread(1);
		}
		return;
	}

	if (sGCProcessFunction != NULL)
	{
		sGCProcessFunction(gobjproc);
	}
	switch (gobjproc->kind)
	{
	case nGCProcessKindThread:
		osDestroyThread(&gobjproc->exec.gobjthread->thread);
		// cast from stack pointer back to stack node
		gobjstack = (GObjStack*) ((uintptr_t)(gobjproc->exec.gobjthread->stack) - offsetof(GObjStack, stack));
		gcEjectGObjStack(gobjstack);
		gcSetGObjThreadPrevAlloc(gobjproc->exec.gobjthread);
		break;

	case nGCProcessKindFunc: break;
	}

	func_800077D0(gobjproc);
	gcSetGObjProcessPrevAlloc(gobjproc);
}

// 0x8000855C
XObj* gcAddXObjForDObjVar(DObj *dobj, u8 kind, u8 arg2, s32 xobj_id)
{
	uintptr_t csr;
	GCTranslate* translate;
	GCRotate* rotate;
	GCScale* scale;
	XObj *xobj;
	s32 i;

	if (dobj->xobjs_num == ARRAY_COUNT(dobj->xobjs))
	{
		syDebugPrintf("om : couldn\'t add OMMtx for DObj\n");
		while (TRUE);
	}
	if (dobj->vec != NULL)
	{
		csr = (uintptr_t)dobj->vec->data;

		for (i = 0; i < ARRAY_COUNT(dobj->vec->kinds); i++)
		{
			switch (dobj->vec->kinds[i])
			{
			case nGCDrawVectorKindNone: break;

			case nGCDrawVectorKindTranslate:
				translate = (GCTranslate*)csr;
				csr += sizeof(GCTranslate);
				break;

			case nGCDrawVectorKindRotate:
				rotate = (GCRotate*)csr;
				csr += sizeof(GCRotate);
				break;

			case nGCDrawVectorKindScale:
				scale = (GCScale*)csr;
				csr += sizeof(GCScale);
				break;
			}
		}
	}
	for (i = dobj->xobjs_num; i > xobj_id; i--)
	{
		dobj->xobjs[i] = dobj->xobjs[i - 1];
	}
	dobj->xobjs_num++;

	dobj->xobjs[xobj_id] = xobj = gcGetXObjSetNextAlloc();
	xobj->kind = kind;

	switch (kind)
	{
	case nGCMatrixKindTra:
	case 34:
	case 36:
	case 38:
	case 40:
	case 55:
		dobj->translate = dGCTranslateDefault;
		dobj->translate.xobj = xobj;
		break;

	case nGCMatrixKindRotD:
	case nGCMatrixKindRotR:
		dobj->rotate = dGCRotateDefaultAXYZ;
		dobj->rotate.xobj = xobj;
		break;

	case nGCMatrixKindTraRotD:
	case nGCMatrixKindTraRotR:
		dobj->translate = dGCTranslateDefault;
		dobj->rotate = dGCRotateDefaultAXYZ;
		dobj->translate.xobj = xobj;
		dobj->rotate.xobj = xobj;
		break;

	case nGCMatrixKindRotRpyD:
	case nGCMatrixKindRotRpyR:
	case nGCMatrixKindRotPyrR:
		dobj->rotate = dGCRotateDefaultRpy;
		dobj->rotate.xobj = xobj;
		break;

	case nGCMatrixKindTraRotRpyD:
	case nGCMatrixKindTraRotRpyR:
	case nGCMatrixKindTraRotPyrR:
	case 51:
	case 52:
		dobj->translate = dGCTranslateDefault;
		dobj->rotate = dGCRotateDefaultRpy;
		dobj->translate.xobj = xobj;
		dobj->rotate.xobj = xobj;
		break;

	case nGCMatrixKindTraRotRSca:
		dobj->translate = dGCTranslateDefault;
		dobj->rotate = dGCRotateDefaultAXYZ;
		dobj->scale = dGCScaleDefault;
		dobj->translate.xobj = xobj;
		dobj->rotate.xobj = xobj;
		dobj->scale.xobj = xobj;
		break;

	case nGCMatrixKindTraRotRpyRSca:
	case nGCMatrixKindTraRotPyrRSca:
	case 54:
		dobj->translate = dGCTranslateDefault;
		dobj->rotate = dGCRotateDefaultRpy;
		dobj->scale = dGCScaleDefault;
		dobj->translate.xobj = xobj;
		dobj->rotate.xobj = xobj;
		dobj->scale.xobj = xobj;
		break;

	case nGCMatrixKindSca:
	case 43:
	case 44:
	case 47:
	case 48:
	case 49:
	case 50:
	case 53:
		dobj->scale = dGCScaleDefault;
		dobj->scale.xobj = xobj;
		break;

	case 45:
	case 46:
		dobj->rotate = dGCRotateDefaultAXYZ;
		dobj->scale = dGCScaleDefault;
		dobj->rotate.xobj = xobj;
		dobj->scale.xobj = xobj;
		break;

	case nGCMatrixKindVecTra:
		*translate = dGCTranslateDefault;
		translate->xobj = xobj;
		break;

	case nGCMatrixKindVecRotR:
		*rotate = dGCRotateDefaultAXYZ;
		rotate->xobj = xobj;
		break;

	case nGCMatrixKindVecRotRpyR:
		*rotate = dGCRotateDefaultRpy;
		rotate->xobj = xobj;
		break;

	case nGCMatrixKindVecSca:
		*scale = dGCScaleDefault;
		scale->xobj = xobj;
		break;

	case nGCMatrixKindVecTraRotR:
		*translate = dGCTranslateDefault;
		*rotate = dGCRotateDefaultAXYZ;

		translate->xobj = rotate->xobj = xobj;
		break;

	case nGCMatrixKindVecTraRotRSca:
		*translate = dGCTranslateDefault;
		*rotate = dGCRotateDefaultAXYZ;
		*scale = dGCScaleDefault;

		translate->xobj = rotate->xobj = scale->xobj = xobj;
		break;

	case nGCMatrixKindVecTraRotRpyR:
		*translate = dGCTranslateDefault;
		*rotate = dGCRotateDefaultRpy;

		translate->xobj = rotate->xobj = xobj;
		break;

	case nGCMatrixKindVecTraRotRpyRSca:
		*translate = dGCTranslateDefault;
		*rotate = dGCRotateDefaultRpy;
		*scale = dGCScaleDefault;

		translate->xobj = rotate->xobj = scale->xobj = xobj;
		break;

	case 1:
	case 17: break;
	}
	xobj->unk05 = arg2;

	return xobj;
}

// 0x80008CC0
// This actually returns gcAddXObjForDObjVar, see https://decomp.me/scratch/nIQ4X
XObj* gcAddXObjForDObjFixed(DObj *dobj, u8 kind, u8 arg2)
{
	return gcAddXObjForDObjVar(dobj, kind, arg2, dobj->xobjs_num);
}

// 0x80008CF0
XObj* gcAddXObjForCamera(CObj *cobj, u8 kind, u8 arg2)
{
	XObj *xobj;

	if (cobj->xobjs_num == ARRAY_COUNT(cobj->xobjs))
	{
		syDebugPrintf("om : couldn't add OMMtx for Camera\n");
		while (TRUE);
	}
	xobj = gcGetXObjSetNextAlloc();

	cobj->xobjs[cobj->xobjs_num] = xobj;
	cobj->xobjs_num++;

	xobj->kind = kind;

	switch (kind)
	{
	case nGCMatrixKindPerspFastF:
	case nGCMatrixKindPerspF:
		cobj->projection.persp = dGCPerspDefault;
		cobj->projection.persp.xobj = xobj;
		break;

	case nGCMatrixKindOrtho:
		cobj->projection.ortho = dGCOrthoDefault;
		cobj->projection.ortho.xobj = xobj;
		break;

	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
		cobj->vec = dGCCObjVecDefault;
		cobj->vec.xobj = xobj;
		break;

	case 1:
	case 2: break;
	}
	xobj->unk05 = arg2;

	return xobj;
}

// 0x80008E78
AObj* gcAddAObjForDObj(DObj *dobj, u8 track)
{
	AObj *aobj = gcGetAObjSetNextAlloc();

	aobj->track = track;
	aobj->kind = nGCAnimKindNone;
	aobj->interpolate = NULL;
	aobj->rate_target = 0.0F;
	aobj->rate_base = 0.0F;
	aobj->value_target = 0.0F;
	aobj->value_base = 0.0F;
	aobj->length = 0.0F;
	aobj->length_invert = 1.0F;

	gcAppendAObjToDObj(dobj, aobj);

	return aobj;
}

// 0x80008EE4
void gcRemoveAObjFromDObj(DObj *dobj)
{
	AObj *current_aobj;
	AObj *next_aobj;

	current_aobj = dobj->aobj;

	while (current_aobj != NULL)
	{
		next_aobj = current_aobj->next;
		gcSetAObjPrevAlloc(current_aobj);
		current_aobj = next_aobj;
	}
	dobj->aobj = NULL;
	dobj->anim_wait = AOBJ_ANIM_NULL;
}

// 0x80008F44
AObj* gcAddAObjForMObj(MObj *mobj, u8 track)
{
	AObj *aobj = gcGetAObjSetNextAlloc();

	aobj->track = track;
	aobj->kind = nGCAnimKindNone;
	aobj->interpolate = NULL;
	aobj->rate_target = 0.0F;
	aobj->rate_base = 0.0F;
	aobj->value_target = 0.0F;
	aobj->value_base = 0.0F;
	aobj->length = 0.0F;
	aobj->length_invert = 1.0F;

	gcAppendAObjToMObj(mobj, aobj);

	return aobj;
}

// 0x80008FB0
// free struct AObj list at unk90
void gcRemoveAObjFromMObj(MObj *mobj)
{
	AObj *current_aobj;
	AObj *next_aobj;

	current_aobj = mobj->aobj;

	while (current_aobj != NULL)
	{
		next_aobj = current_aobj->next;
		gcSetAObjPrevAlloc(current_aobj);
		current_aobj = next_aobj;
	}
	mobj->aobj = NULL;
	mobj->anim_wait = AOBJ_ANIM_NULL;
}

// 0x80009010
AObj* gcAddAObjForCamera(CObj *cobj, u8 track)
{
	AObj *aobj = gcGetAObjSetNextAlloc();

	aobj->track = track;
	aobj->kind = nGCAnimKindNone;
	aobj->interpolate = NULL;
	aobj->rate_target = 0.0F;
	aobj->rate_base = 0.0F;
	aobj->value_target = 0.0F;
	aobj->value_base = 0.0F;
	aobj->length = 0.0F;
	aobj->length_invert = 1.0F;

	gcAppendAObjToCamera(cobj, aobj);

	return aobj;
}

// 0x8000907C
void gcRemoveAObjFromCamera(CObj *cobj)
{
	AObj *current_aobj;
	AObj *next_aobj;

	current_aobj = cobj->aobj;

	while (current_aobj != NULL)
	{
		next_aobj = current_aobj->next;
		gcSetAObjPrevAlloc(current_aobj);
		current_aobj = next_aobj;
	}
	cobj->aobj = NULL;
	cobj->anim_wait = AOBJ_ANIM_NULL;
}

// 0x800090DC
MObj* gcAddMObjForDObj(DObj *dobj, MObjSub* mobjsub)
{
	MObj *mobj = gcGetMObjSetNextAlloc();

	if (dobj->mobj != NULL)
	{
		MObj *current_mobj = dobj->mobj->next;
		MObj *prior_mobj = dobj->mobj;

		while (current_mobj != NULL)
		{
			prior_mobj = current_mobj;
			current_mobj = current_mobj->next;
		}
		prior_mobj->next = mobj;
	}
	else dobj->mobj = mobj;

	mobj->next = NULL;
	mobj->lfrac = mobjsub->prim_l / 255.0F;
	mobj->sub = *mobjsub;

	mobj->sub.unk24 = mobjsub->trau;
	mobj->sub.unk28 = mobjsub->scau;
	mobj->texture_id_curr = 0;
	mobj->texture_id_next = 0;
	mobj->palette_id = 0;
	mobj->aobj = NULL;
	mobj->matanim_joint.event32 = NULL;
	mobj->anim_wait = AOBJ_ANIM_NULL;
	mobj->anim_speed = 1.0F;
	mobj->anim_frame = 0.0F;

	return mobj;
}

// 0x800091F4
void gcRemoveMObjAll(DObj *dobj)
{
	MObj *current_mobj;
	MObj *next_mobj;
	AObj *current_aobj;
	AObj *next_aobj;

	current_mobj = dobj->mobj;

	while (current_mobj != NULL)
	{
		current_aobj = current_mobj->aobj;

		while (current_aobj != NULL)
		{
			next_aobj = current_aobj->next;
			gcSetAObjPrevAlloc(current_aobj);
			current_aobj = next_aobj;
		}
		next_mobj = current_mobj->next;
		gcSetMObjPrevAlloc(current_mobj);
		current_mobj = next_mobj;
	}
	dobj->mobj = NULL;
}

// 0x8000926C
void gcInitDObj(DObj *dobj)
{
	s32 i;

	dobj->vec = NULL;
	dobj->flags = DOBJ_FLAG_NONE;
	dobj->is_anim_root = FALSE;
	dobj->xobjs_num = 0;

	for (i = 0; i < ARRAY_COUNT(dobj->xobjs); i++)
	{
		dobj->xobjs[i] = NULL;
	}
	dobj->aobj = NULL;
	dobj->anim_joint.event32 = NULL;
	dobj->anim_wait = AOBJ_ANIM_NULL;
	dobj->anim_speed = 1.0F;
	dobj->anim_frame = 0.0F;
	dobj->mobj = NULL;
	dobj->user_data.p = NULL;
}

// 0x800092D0
DObj* gcAddDObjForGObj(GObj *gobj, void *dvar)
{
	DObj *new_dobj, *current_dobj;

	if (gobj == NULL)
	{
		gobj = gGCCurrentCommon;
	}
	new_dobj = gcGetDObjSetNextAlloc();

	if (gobj->obj != NULL)
	{
		current_dobj = DObjGetStruct(gobj);

		while (current_dobj->sib_next != NULL)
		{
			current_dobj = current_dobj->sib_next;
		}
		current_dobj->sib_next = new_dobj;
		new_dobj->sib_prev = current_dobj;
	}
	else
	{
		gobj->obj_kind = nGCCommonAppendDObj;
		gobj->obj = new_dobj;
		new_dobj->sib_prev = NULL;
	}
	new_dobj->parent_gobj = gobj;
	new_dobj->parent = DOBJ_PARENT_NULL;
	new_dobj->sib_next = NULL;
	new_dobj->child = NULL;
	new_dobj->dv = dvar;

	gcInitDObj(new_dobj);

	return new_dobj;
}

// 0x80009380
DObj* gcAddSiblingForDObj(DObj *dobj, void *dvar)
{
	DObj *new_dobj = gcGetDObjSetNextAlloc();

	if (dobj->sib_next != NULL)
	{
		dobj->sib_next->sib_prev = new_dobj;
	}
	new_dobj->sib_prev = dobj;
	new_dobj->sib_next = dobj->sib_next;
	dobj->sib_next = new_dobj;
	new_dobj->parent_gobj = dobj->parent_gobj;
	new_dobj->parent = dobj->parent;

	new_dobj->child = NULL;
	new_dobj->dv = dvar;

	gcInitDObj(new_dobj);

	return new_dobj;
}

// 0x800093F4
DObj* gcAddChildForDObj(DObj *dobj, void *dvar)
{
	DObj *new_dobj = gcGetDObjSetNextAlloc();
	DObj *current_dobj;

	if (dobj->child != NULL)
	{
		current_dobj = dobj->child;

		while (current_dobj->sib_next != NULL)
		{
			current_dobj = current_dobj->sib_next;
		}
		current_dobj->sib_next = new_dobj;
		new_dobj->sib_prev = current_dobj;
	}
	else
	{
		dobj->child = new_dobj;
		new_dobj->sib_prev = NULL;
	}
	new_dobj->parent_gobj = dobj->parent_gobj;
	new_dobj->parent = dobj;
	new_dobj->child = NULL;
	new_dobj->sib_next = NULL;
	new_dobj->dv = dvar;

	gcInitDObj(new_dobj);

	return new_dobj;
}

// 0x8000948C
// drop_dobj, cleanup_dobj, gcSetDObjPrevAlloc?
void gcEjectDObj(DObj *dobj)
{
	s32 i;
	AObj *current_aobj, *next_aobj;
	MObj *current_mobj, *next_mobj;

	while (dobj->child != NULL)
	{
		gcEjectDObj(dobj->child);
	}
	if (dobj->parent == (DObj*)1)
	{
		if (dobj == DObjGetStruct(dobj->parent_gobj))
		{
			dobj->parent_gobj->obj = dobj->sib_next;

			if (DObjGetStruct(dobj->parent_gobj) == NULL)
			{
				dobj->parent_gobj->obj_kind = nGCCommonAppendNone;
			}
		}
	}
	else if (dobj == dobj->parent->child)
	{
		dobj->parent->child = dobj->sib_next;
	}
	if (dobj->sib_prev != NULL)
	{
		dobj->sib_prev->sib_next = dobj->sib_next;
	}
	if (dobj->sib_next != NULL)
	{
		dobj->sib_next->sib_prev = dobj->sib_prev;
	}
	for (i = 0; i < ARRAY_COUNT(dobj->xobjs); i++)
	{
		if (dobj->xobjs[i] != NULL)
		{
			gcSetXObjPrevAlloc(dobj->xobjs[i]);
		}
	}
	if ((dobj->vec != NULL) && (sGCDrawFuncEject != NULL))
	{
		sGCDrawFuncEject(dobj->vec);
	}
	current_aobj = dobj->aobj;

	while (current_aobj != NULL)
	{
		next_aobj = current_aobj->next;
		gcSetAObjPrevAlloc(current_aobj);
		current_aobj = next_aobj;
	}
	current_mobj = dobj->mobj;

	while (current_mobj != NULL)
	{
		current_aobj = current_mobj->aobj;

		while (current_aobj != NULL)
		{
			next_aobj = current_aobj->next;
			gcSetAObjPrevAlloc(current_aobj);
			current_aobj = next_aobj;
		}
		next_mobj = current_mobj->next;
		gcSetMObjPrevAlloc(current_mobj);
		current_mobj = next_mobj;
	}

	gcSetDObjPrevAlloc(dobj);
}

// 0x80009614
SObj* gcAddSObjForGObj(GObj *gobj, Sprite *sprite)
{
	SObj *new_sobj;

	if (gobj == NULL)
	{
		gobj = gGCCurrentCommon;
	}
	new_sobj = gcGetSObjSetNextAlloc();

	if (SObjGetStruct(gobj) != NULL)
	{
		SObj *tail_sobj = SObjGetStruct(gobj);

		while (tail_sobj->next != NULL)
		{
			tail_sobj = tail_sobj->next;
		}
		tail_sobj->next = new_sobj;
		new_sobj->prev = tail_sobj;
	}
	else
	{
		gobj->obj_kind = nGCCommonAppendSObj;
		gobj->obj = new_sobj;
		new_sobj->prev = NULL;
	}
	new_sobj->parent_gobj = gobj;
	new_sobj->next = NULL;

	if (sprite != NULL)
	{
		new_sobj->sprite = *sprite;
	}
	new_sobj->user_data.p = NULL;

	return new_sobj;
}

// 0x800096EC
void gcEjectSObj(SObj *sobj)
{
	if (sobj == SObjGetStruct(sobj->parent_gobj))
	{
		sobj->parent_gobj->obj = sobj->next;

		if (SObjGetStruct(sobj->parent_gobj) == NULL)
		{
			sobj->parent_gobj->obj_kind = nGCCommonAppendNone;
		}
	}
	if (sobj->prev != NULL)
	{
		sobj->prev->next = sobj->next;
	}
	if (sobj->next != NULL)
	{
		sobj->next->prev = sobj->prev;
	}
	gcSetSObjPrevAlloc(sobj);
}

CObj* gcAddCameraForGObj(GObj *gobj)
{
	s32 i;
	CObj *new_cobj;

	if (gobj == NULL)
	{
		gobj = gGCCurrentCommon;
	}
	gobj->obj_kind = nGCCommonAppendCamera;

	new_cobj = gcGetCObjSetNextAlloc();
	gobj->obj = new_cobj;
	new_cobj->parent_gobj = gobj;

	syRdpSetDefaultViewport(&new_cobj->viewport);

	new_cobj->xobjs_num = 0;

	for (i = 0; i < ARRAY_COUNT(new_cobj->xobjs); i++)
	{
		new_cobj->xobjs[i] = NULL;
	}
	new_cobj->flags = COBJ_FLAG_NONE;
	new_cobj->color = GPACK_RGBA8888(0x00, 0x00, 0x00, 0x00);
	new_cobj->func_camera = NULL;
	new_cobj->unk_camera_0x8C = 0;

	new_cobj->aobj = NULL;
	new_cobj->camanim_joint.event32 = NULL;

	new_cobj->anim_wait = AOBJ_ANIM_NULL;
	new_cobj->anim_speed = 1.0F;
	new_cobj->anim_frame = 0.0F;

	return new_cobj;
}

// 0x80009810
void gcEjectCamera(CObj *cobj)
{
	GObj *gobj;
	s32 i;
	AObj *current_aobj;
	AObj *next_aobj;

	gobj = cobj->parent_gobj;
	gobj->obj_kind = nGCCommonAppendNone;
	gobj->obj = NULL;

	for (i = 0; i < ARRAY_COUNT(cobj->xobjs); i++)
	{
		if (cobj->xobjs[i] != NULL)
			gcSetXObjPrevAlloc(cobj->xobjs[i]);
	}
	current_aobj = cobj->aobj;

	while (current_aobj != NULL)
	{
		next_aobj = current_aobj->next;
		gcSetAObjPrevAlloc(current_aobj);
		current_aobj = next_aobj;
	}
	gcSetCObjPrevAlloc(cobj);
}

// 0x800098A4
GObj* gcInitGObjCommon(u32 id, void (*func_run)(GObj*), u8 link, u32 priority)
{
	GObj *new_gobj;

	if (link >= ARRAY_COUNT(gGCCommonLinks))
	{
		syDebugPrintf("omGAddCommon() : link num over : link = %d : id = %d\n", link, id);
		while (TRUE);
	}

	new_gobj = gcGetGObjSetNextAlloc();

	if (new_gobj == NULL)
		return NULL;

	new_gobj->id = id;
	new_gobj->link_id = link;
	new_gobj->link_priority = priority;
	new_gobj->func_run = func_run;
	new_gobj->gobjproc_head = NULL;
	new_gobj->gobjproc_tail = NULL;
	new_gobj->gobjscripts_num = 0;
	new_gobj->flags = GOBJ_FLAG_NONE;

	new_gobj->obj_kind = nGCCommonAppendNone;
	new_gobj->obj = NULL;

	new_gobj->dl_link_id = ARRAY_COUNT(gGCCommonDLLinks);
	new_gobj->anim_frame = 0.0F;
	new_gobj->func_anim = NULL;
	new_gobj->user_data.p = NULL;

	return new_gobj;
}

// 0x80009968
GObj* gcMakeGObjSPAfter(u32 id, void (*func_run)(GObj*), u8 link, u32 priority)
{
	GObj *new_gobj = gcInitGObjCommon(id, func_run, link, priority);

	if (new_gobj == NULL)
	{
		return NULL;
	}
	gcLinkGObjSPAfter(new_gobj);

	return new_gobj;
}

// 0x800099A8
GObj* gcMakeGObjSPBefore(u32 id, void (*func_run)(GObj*), u8 link, u32 priority)
{
	GObj *new_gobj = gcInitGObjCommon(id, func_run, link, priority);

	if (new_gobj == NULL)
	{
		return NULL;
	}
	gcLinkGObjSPBefore(new_gobj);

	return new_gobj;
}

// 0x800099E8
GObj* gcMakeGObjAfter(u32 id, void (*func_run)(GObj*), GObj *link_gobj)
{
	GObj *new_gobj = gcInitGObjCommon(id, func_run, link_gobj->link_id, link_gobj->link_priority);

	if (new_gobj == NULL)
	{
		return NULL;
	}
	gcLinkGObjAfter(new_gobj, link_gobj);

	return new_gobj;
}

// 0x80009A34
GObj* gcMakeGObjBefore(u32 id, void (*func_run)(GObj*), GObj *link_gobj)
{
	GObj *new_gobj = gcInitGObjCommon(id, func_run, link_gobj->link_id, link_gobj->link_priority);

	if (new_gobj == NULL)
	{
		return NULL;
	}
	gcLinkGObjAfter(new_gobj, link_gobj->link_prev);

	return new_gobj;
}

// 0x80009A84
void gcEjectGObj(GObj *gobj)
{
	if ((gobj == NULL) || (gobj == gGCCurrentCommon))
	{
		sGCRunStatus = 2;
		return;
	}

	gcEndProcessAll(gobj);

	switch (gobj->obj_kind)
	{
	case nGCCommonAppendDObj: gcRemoveDObjAll(gobj); break;

	case nGCCommonAppendSObj: gcRemoveSObjAll(gobj); break;

	case nGCCommonAppendCamera: gcEjectCamera(CObjGetStruct(gobj)); break;
	}

	if (gobj->dl_link_id != ARRAY_COUNT(gGCCommonDLLinks))
		gcRemoveGObjFromDLLinkedList(gobj);

	gcRemoveGObjFromLinkedList(gobj);
	gcSetGObjPrevAlloc(gobj);
}

// 0x80009B48
void gcMoveGObjCommon(s32 sw, GObj *this_gobj, u8 link, u32 priority, GObj *other_gobj)
{
	GObjProcess *current_gobjproc;
	GObjProcess *orig_gobjproc;
	GObjProcess *next_gobjproc;

	if (link >= ARRAY_COUNT(gGCCommonLinks))
	{
		syDebugPrintf("omGMoveCommon() : link num over : link = %d : id = %d\n", link, this_gobj->id);

		while (TRUE);
	}

	if (this_gobj == NULL)
		this_gobj = gGCCurrentCommon;

	orig_gobjproc = this_gobj->gobjproc_head;

	this_gobj->gobjproc_head = NULL;
	this_gobj->gobjproc_tail = NULL;

	current_gobjproc = orig_gobjproc;

	while (current_gobjproc != NULL)
	{
		func_80007784(current_gobjproc);
		current_gobjproc = current_gobjproc->link_next;
	}
	gcRemoveGObjFromLinkedList(this_gobj);

	this_gobj->link_id = link;
	this_gobj->link_priority = priority;

	switch (sw)
	{
	case 0: gcLinkGObjSPAfter(this_gobj); break;

	case 1: gcLinkGObjSPBefore(this_gobj); break;

	case 2: gcLinkGObjAfter(this_gobj, other_gobj); break;

	case 3: gcLinkGObjAfter(this_gobj, other_gobj->link_prev); break;
	}

	current_gobjproc = orig_gobjproc;

	while (current_gobjproc != NULL)
	{
		next_gobjproc = current_gobjproc->link_next;
		gcLinkGObjProcess(current_gobjproc);
		current_gobjproc = next_gobjproc;
	}
}

// 0x80009C90
void func_80009C90(GObj *gobj, u8 link, u32 priority) { gcMoveGObjCommon(0, gobj, link, priority, NULL); }

// 0x80009CC8
void func_80009CC8(GObj *gobj, u8 link, u32 priority) { gcMoveGObjCommon(1, gobj, link, priority, NULL); }

// 0x80009D00
void unref_80009D00(GObj *this_gobj, GObj *other_gobj)
{
	gcMoveGObjCommon(2, this_gobj, other_gobj->link_id, other_gobj->link_priority, other_gobj);
}

// 0x80009D3C
void unref_80009D3C(GObj *this_gobj, GObj *other_gobj)
{
	gcMoveGObjCommon(3, this_gobj, other_gobj->link_id, other_gobj->link_priority, other_gobj);
}

// 0x80009D78
void gcLinkGObjDLCommon(GObj *gobj, void (*proc_display)(GObj*), u8 dl_link, u32 dl_order, u32 camera_tag)
{
	if (dl_link >= ARRAY_COUNT(gGCCommonDLLinks) - 1)
	{
		syDebugPrintf("omGLinkObjDLCommon() : dl_link num over : dl_link = %d : id = %d\n", dl_link, gobj->id);
		while (TRUE);
	}

	gobj->dl_link_id = dl_link;
	gobj->dl_link_priority = dl_order;
	gobj->proc_display = proc_display;
	gobj->camera_tag = camera_tag;
	gobj->frame_draw_last = dSYTaskmanFrameCount - 1;
}

// 0x80009DF4
void gcAddGObjDisplay(GObj *gobj, void (*proc_display)(GObj*), u8 dl_link, u32 priority, u32 camera_tag)
{
	if (gobj == NULL)
	{
		gobj = gGCCurrentCommon;
	}
	gcLinkGObjDLCommon(gobj, proc_display, dl_link, priority, camera_tag);
	gcDLLinkGObjTail(gobj);
}

// 0x80009E38
void unref_80009E38(GObj *gobj, void (*proc_display)(GObj*), u8 dl_link, u32 priority, u32 camera_tag)
{
	if (gobj == NULL)
		gobj = gGCCurrentCommon;

	gcLinkGObjDLCommon(gobj, proc_display, dl_link, priority, camera_tag);
	gcDLLinkGObjHead(gobj);
}

// 0x80009E7C
void unref_80009E7C(GObj *this_gobj, void (*proc_display)(GObj*), s32 arg2, GObj *other_gobj)
{
	if (this_gobj == NULL)
		this_gobj = gGCCurrentCommon;

	gcLinkGObjDLCommon(this_gobj, proc_display, other_gobj->dl_link_id, other_gobj->dl_link_priority, arg2);
	gcAppendGObjToDLLinkedList(this_gobj, other_gobj);
}

// 0x80009ED0
void unref_80009ED0(GObj *this_gobj, void (*proc_display)(GObj*), s32 arg2, GObj *other_gobj)
{
	if (this_gobj == NULL)
		this_gobj = gGCCurrentCommon;

	gcLinkGObjDLCommon(this_gobj, proc_display, other_gobj->dl_link_id, other_gobj->dl_link_priority, arg2);
	gcAppendGObjToDLLinkedList(this_gobj, other_gobj->link_prev);
}

// 0x80009F28
void func_80009F28(GObj *gobj, void (*proc_display)(GObj*), u32 priority, u64 arg3, u32 camera_tag)
{
	gobj->dl_link_id = ARRAY_COUNT(gGCCommonDLLinks) - 1;
	gobj->dl_link_priority = priority;
	gobj->proc_display = proc_display;
	gobj->camera_mask = arg3;
	gobj->camera_tag = camera_tag;
	gobj->buffer_mask = 0;
	gobj->frame_draw_last = dSYTaskmanFrameCount - 1;
}

// 0x80009F74
void func_80009F74(GObj *gobj, void (*proc_display)(GObj*), u32 priority, u64 arg3, u32 camera_tag)
{
	if (gobj == NULL)
	{
		gobj = gGCCurrentCommon;
	}
	func_80009F28(gobj, proc_display, priority, arg3, camera_tag);
	gcDLLinkGObjTail(gobj);
}

// 0x80009FC0
void unref_80009FC0(GObj *gobj, void (*proc_display)(GObj*), u32 priority, u64 arg3, u32 camera_tag)
{
	if (gobj == NULL)
	{
		gobj = gGCCurrentCommon;
	}
	func_80009F28(gobj, proc_display, priority, arg3, camera_tag);
	gcDLLinkGObjHead(gobj);
}

// 0x8000A00C
void unref_8000A00C(GObj *this_gobj, void (*proc_display)(GObj*), u64 arg2, s32 arg3, GObj *other_gobj)
{
	if (this_gobj == NULL)
	{
		this_gobj = gGCCurrentCommon;
	}
	func_80009F28(this_gobj, proc_display, other_gobj->dl_link_priority, arg2, arg3);
	gcAppendGObjToDLLinkedList(this_gobj, other_gobj);
}

// 0x8000A06C
void unref_8000A06C(GObj *this_gobj, void (*proc_display)(GObj*), u64 arg2, s32 arg3, GObj *other_gobj)
{
	if (this_gobj == NULL)
	{
		this_gobj = gGCCurrentCommon;
	}
	func_80009F28(this_gobj, proc_display, other_gobj->dl_link_priority, arg2, arg3);
	gcAppendGObjToDLLinkedList(this_gobj, other_gobj->link_prev);
}

// 0x8000A0D0
void gcMoveGObjDL(GObj *gobj, u8 dl_link, u32 priority)
{
	if (dl_link >= ARRAY_COUNT(gGCCommonDLLinks) - 1)
	{
		syDebugPrintf("omGMoveObjDL() : dl_link num over : dl_link = %d : id = %d\n", dl_link, gobj->id);
		while (TRUE);
	}
	gcRemoveGObjFromDLLinkedList(gobj);

	gobj->dl_link_id = dl_link;
	gobj->dl_link_priority = priority;

	gcDLLinkGObjTail(gobj);
}

// 0x8000A14C
void gcMoveGObjDLHead(GObj *gobj, u8 dl_link, u32 priority)
{
	if (dl_link >= ARRAY_COUNT(gGCCommonDLLinks) - 1)
	{
		syDebugPrintf("omGMoveObjDLHead() : dl_link num over : dl_link = %d : id = %d\n", dl_link, gobj->id);
		while (TRUE);
	}
	gcRemoveGObjFromDLLinkedList(gobj);
	
	gobj->dl_link_id = dl_link;
	gobj->dl_link_priority = priority;

	gcDLLinkGObjHead(gobj);
}

// 0x8000A1C8
void unref_8000A1C8(GObj *this_gobj, GObj *other_gobj)
{
	gcRemoveGObjFromDLLinkedList(this_gobj);
	this_gobj->dl_link_id = other_gobj->dl_link_id;
	this_gobj->dl_link_priority = other_gobj->dl_link_priority;
	gcAppendGObjToDLLinkedList(this_gobj, other_gobj);
}

// 0x8000A208
void unref_8000A208(GObj *this_gobj, GObj *other_gobj)
{
	gcRemoveGObjFromDLLinkedList(this_gobj);
	this_gobj->dl_link_id = other_gobj->dl_link_id;
	this_gobj->dl_link_priority = other_gobj->dl_link_priority;
	gcAppendGObjToDLLinkedList(this_gobj, other_gobj->dl_link_prev);
}

// 0x8000A24C
void func_8000A24C(GObj *gobj, u32 priority)
{
	gcRemoveGObjFromDLLinkedList(gobj);
	gobj->dl_link_priority = priority;
	gcDLLinkGObjTail(gobj);
}

// 0x8000A280
void unref_8000A280(GObj *gobj, u32 priority)
{
	gcRemoveGObjFromDLLinkedList(gobj);
	gobj->dl_link_priority = priority;
	gcDLLinkGObjHead(gobj);
}

// 0x8000A2B4
void func_8000A2B4(GObj *this_gobj, GObj *other_gobj)
{
	gcRemoveGObjFromDLLinkedList(this_gobj);
	this_gobj->dl_link_priority = other_gobj->dl_link_priority;
	gcAppendGObjToDLLinkedList(this_gobj, other_gobj);
}

// 0x8000A2EC
void unref_8000A2EC(GObj *this_gobj, GObj *other_gobj)
{
	gcRemoveGObjFromDLLinkedList(this_gobj);
	this_gobj->dl_link_priority = other_gobj->dl_link_priority;
	gcAppendGObjToDLLinkedList(this_gobj, other_gobj->link_prev);
}

// 0x8000A328
void gcSetMaxNumGObj(s32 num)
{
	sGCCommonsMaxNum = num;
}

// 0x8000A334
s16 gcGetMaxNumGObj(void)
{
	return sGCCommonsMaxNum;
}

// 0x8000A340
void gcDrawAll(void)
{
	s32 i;
	s32 frame_count;
	GObj *gobj;

	gGCCurrentCamera = NULL;
	gGCCurrentDisplay = NULL;

	for (i = 0, frame_count = dSYTaskmanFrameCount - 1; i < ARRAY_COUNT(gGCFrameQueueGfxLinks); i++)
	{
		gGCFrameQueueGfxLinks[i].frame = frame_count;
	}
	gobj = gGCCommonDLLinks[ARRAY_COUNT(gGCCommonDLLinks) - 1];

	while (gobj != NULL)
	{
		if (!(gobj->flags & GOBJ_FLAG_HIDDEN))
		{
			dGCCurrentStatus = nGCStatusCapturing;
			gGCCurrentCamera = gobj;

			gobj->proc_display(gobj);

			dGCCurrentStatus = nGCStatusSystem;
		}
		gobj = gobj->dl_link_next;
	}
}

// 0x8000A40C
GObj* gcRunGObj(GObj *gobj)
{
	GObj *next_gobj;

	dGCCurrentStatus = nGCStatusRunning;
	gGCCurrentCommon = gobj;

	gobj->func_run(gobj);

	next_gobj = gobj->link_next;

	gGCCurrentCommon = NULL;
	dGCCurrentStatus = nGCStatusSystem;

	switch (sGCRunStatus)
	{
	case nGCRunStatusDefault:
		break;

	case nGCRunStatusEject:
		sGCRunStatus = nGCRunStatusDefault;
		gcEjectGObj(gobj);
		break;

	default:
		sGCRunStatus = nGCRunStatusDefault;
		break;
	}
	return next_gobj;
}

// 0x8000A49C
GObjProcess* gcRunGObjProcess(GObjProcess *gobjproc)
{
	GObjProcess *next_gobjproc;

	dGCCurrentStatus = nGCStatusProcessing;
	gGCCurrentCommon = gobjproc->parent_gobj;
	gGCCurrentProcess = gobjproc;

	switch (gobjproc->kind)
	{
	case nGCProcessKindThread:
		osStartThread(&gobjproc->exec.gobjthread->thread);
		osRecvMesg(&gGCMesgQueue, NULL, OS_MESG_BLOCK);
		break;

	case nGCProcessKindFunc:
		gobjproc->exec.func(gobjproc->parent_gobj);
		break;
	}
	next_gobjproc = gobjproc->priority_next;

	gGCCurrentCommon = NULL;
	gGCCurrentProcess = NULL;
	dGCCurrentStatus = nGCStatusSystem;

	switch (sGCRunStatus)
	{
	case nGCRunStatusEject:
		sGCRunStatus = nGCRunStatusDefault;

		while ((next_gobjproc != NULL) && (next_gobjproc->parent_gobj == gobjproc->parent_gobj))
		{
			next_gobjproc = next_gobjproc->priority_next;
		}
		gcEjectGObj(gobjproc->parent_gobj);
		break;

	case nGCRunStatusEnd:
		sGCRunStatus = nGCRunStatusDefault;
		gcEndGObjProcess(gobjproc);
		break;

	case nGCRunStatusDefault:
		break;

	default:
		sGCRunStatus = nGCRunStatusDefault;
		break;
	}
	return next_gobjproc;
}

// 0x8000A5E4
void gcRunAll(void)
{
	s32 i;
	GObj *gobj;
	GObjProcess *gobjproc;

	sGCRunStatus = nGCRunStatusDefault;
	gGCCurrentCommon = NULL;
	gGCCurrentProcess = NULL;

	for (i = 0; i < ARRAY_COUNT(gGCCommonLinks); i++)
	{
		gobj = gGCCommonLinks[i];

		while (gobj != NULL)
		{
			if (!(gobj->flags & GOBJ_FLAG_NORUN) && (gobj->func_run != NULL))
			{
				gobj = gcRunGObj(gobj);
			}
			else gobj = gobj->link_next;
		}
	}
	for (i = ARRAY_COUNT(sGCProcessQueue) - 1; i >= 0; i--)
	{
		gobjproc = sGCProcessQueue[i];

		while (gobjproc != NULL)
		{
			if (gobjproc->is_paused == FALSE)
			{
				gobjproc = gcRunGObjProcess(gobjproc);
			}
			else gobjproc = gobjproc->priority_next;
		}
	}
}

// 0x8000A6E0
void gcSetupObjman(GCSetup *setup)
{
	s32 i;

	sGCThreadStackSize = setup->gobjthreadstack_size;
	sUnkUnusedSetup = setup->unk_gcsetup_0x14;

	if (setup->gobjthreads_num != 0)
	{
		GObjThread *current_gobjthread;
		sGCThreadHead = current_gobjthread = setup->gobjthreads;

		for (i = 0; i < setup->gobjthreads_num - 1; i++)
		{
			GObjThread *next_gobjthread = current_gobjthread + 1;

			current_gobjthread->next = next_gobjthread;
			current_gobjthread = next_gobjthread;
		}
		current_gobjthread->next = NULL;
	}
	else sGCThreadHead = NULL;

	if ((setup->gobjthreadstacks_num != 0) && (setup->gobjthreadstack_size != 0))
	{
		GObjStack *current_gobjstack;

		sGCThreadStackHead = syTaskmanMalloc(sizeof(GObjThreadStack), 0x4);
		sGCThreadStackHead->next = NULL;
		sGCThreadStackHead->size = sGCThreadStackSize;
		sGCThreadStackHead->stack = current_gobjstack = setup->gobjthreadstacks;

		for (i = 0; i < setup->gobjthreadstacks_num - 1; i++)
		{
			current_gobjstack->next = (GObjStack*) ((uintptr_t)current_gobjstack + sGCThreadStackSize + offsetof(GObjStack, stack));
			current_gobjstack->stack_size = sGCThreadStackSize;
			current_gobjstack = (GObjStack*) ((uintptr_t)current_gobjstack + sGCThreadStackSize + offsetof(GObjStack, stack));
		}
		current_gobjstack->stack_size = sGCThreadStackSize;
		current_gobjstack->next = NULL;
	}
	else sGCThreadStackHead = NULL;

	if (setup->gobjprocs_num != 0)
	{
		GObjProcess *current_gobjproc;
		sGCProcessHead = current_gobjproc = setup->gobjprocs;

		for (i = 0; i < setup->gobjprocs_num - 1; i++)
		{
			GObjProcess *next_gobjproc = current_gobjproc + 1;

			current_gobjproc->link_next = next_gobjproc;
			current_gobjproc = next_gobjproc;
		}
		current_gobjproc->link_next = NULL;
	}
	else sGCProcessHead = NULL;

	for (i = 0; i < ARRAY_COUNT(sGCProcessQueue); i++)
	{
		sGCProcessQueue[i] = NULL;
	}
	if (setup->gobjs_num != 0)
	{
		GObj *current_gobj;
		sGCCommonHead = current_gobj = setup->gobjs;

		for (i = 0; i < setup->gobjs_num - 1; i++)
		{
			current_gobj->link_next = (GObj*) ((uintptr_t)current_gobj + setup->gobj_size);
			current_gobj = current_gobj->link_next;
		}
		current_gobj->link_next = NULL;
	}
	else sGCCommonHead = NULL;

	sGCCommonSize = setup->gobj_size;
	sGCCommonsMaxNum = -1;
	sGCDrawFuncEject = setup->func_eject;

	if (setup->xobjs_num != 0)
	{
		XObj *current_xobj;
		sGCMatrixHead = current_xobj = setup->xobjs;

		for (i = 0; i < setup->xobjs_num - 1; i++)
		{
			XObj *next_xobj = current_xobj + 1;
			current_xobj->next = next_xobj;
			current_xobj = next_xobj;
		}
		current_xobj->next = NULL;
	}
	else sGCMatrixHead = NULL;

	if (setup->aobjs_num != 0)
	{
		AObj *current_aobj;
		sGCAnimHead = current_aobj = setup->aobjs;

		for (i = 0; i < setup->aobjs_num - 1; i++)
		{
			AObj *next_aobj = current_aobj + 1;
			current_aobj->next = next_aobj;
			current_aobj = next_aobj;
		}
		current_aobj->next = NULL;
	}
	else sGCAnimHead = NULL;

	if (setup->mobjs_num != 0)
	{
		MObj *current_mobj;
		sGCMaterialHead = current_mobj = setup->mobjs;

		for (i = 0; i < setup->mobjs_num - 1; i++)
		{
			MObj *mobj_next = current_mobj + 1;
			current_mobj->next = mobj_next;
			current_mobj = mobj_next;
		}
		current_mobj->next = NULL;
	}
	else sGCMaterialHead = NULL;

	if (setup->dobjs_num != 0)
	{
		DObj *current_dobj;
		sGCDrawHead = current_dobj = setup->dobjs;

		for (i = 0; i < setup->dobjs_num - 1; i++)
		{
			current_dobj->alloc_free = (DObj*) ((uintptr_t)current_dobj + setup->dobj_size);
			current_dobj = current_dobj->alloc_free;
		}
		current_dobj->alloc_free = NULL;
	}
	else sGCDrawHead = NULL;

	sGCDrawSize = setup->dobj_size;

	if (setup->sobjs_num != 0)
	{
		SObj *current_sobj;
		sGCSpriteHead = current_sobj = setup->sobjs;

		for (i = 0; i < setup->sobjs_num - 1; i++)
		{
			current_sobj->alloc_free = (SObj*) ((uintptr_t)current_sobj + setup->sobj_size);
			current_sobj = current_sobj->alloc_free;
		}
		current_sobj->alloc_free = NULL;
	}
	else sGCSpriteHead = NULL;

	sGCSpriteSize = setup->sobj_size;

	if (setup->cobjs_num != 0)
	{
		CObj *current_cobj;
		sGCCameraHead = current_cobj = setup->cameras;

		for (i = 0; i < setup->cobjs_num - 1; i++)
		{
			current_cobj->next = (CObj*) ((uintptr_t)current_cobj + setup->cobj_size);
			current_cobj = current_cobj->next;
		}
		current_cobj->next = NULL;
	}
	else sGCCameraHead = NULL;

	sGCCameraSize = setup->cobj_size;

	for (i = 0; i < (ARRAY_COUNT(sGCCommonLinks) + ARRAY_COUNT(gGCCommonLinks)) / 2; i++)
	{
		gGCCommonLinks[i] = sGCCommonLinks[i] = NULL;
	}
	for (i = 0; i < (ARRAY_COUNT(sGCCommonDLLinks) + ARRAY_COUNT(gGCCommonDLLinks)) / 2; i++)
	{
		gGCCommonDLLinks[i] = sGCCommonDLLinks[i] = NULL;
	}
	gcInitDLs();
	osCreateMesgQueue(&gGCMesgQueue, sGCMesgs, ARRAY_COUNT(sGCMesgs));

	sGCStacksActiveNum 	=
	sGCThreadsActive 	=
	sGCProcessesActive 	=
	sGCCommonsActiveNum =
	sGCMatrixesActiveNum=
	sGCAnimsActiveNum 	=
	sGCDrawsActiveNum 	=	
	sGCSpritesActiveNum =
	sGCCamerasActiveNum = 0;

	sGCProcessFunction = NULL;

	gcSetCameraMatrixMode(0);

	dGCCurrentStatus = nGCStatusSystem;
}
