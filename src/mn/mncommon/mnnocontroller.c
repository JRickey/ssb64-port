#include <mn/menu.h>
#include <sc/scene.h>
#include <sys/video.h>
#include <sys/controller.h>
#include <sys/rdp.h>
#include <reloc_data.h>


// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x800D6700
SYVideoSetup dMNNoControllerVideoSetup = SYVIDEO_SETUP_DEFAULT();

// 0x800D671C
SYTaskmanSetup dMNNoControllerTaskmanSetup =
{
    // Task Manager Buffer Setup
    {
        0,                          // ???
        gcRunAll,          			// Update function
        gcDrawAll,                  // Frame draw function
        &ovl11_BSS_END,             // Allocatable memory pool start
        0,                          // Allocatable memory pool size
        1,                          // ???
        2,                          // Number of contexts?
        sizeof(Gfx) * 3584,         // Display List Buffer 0 Size
        sizeof(Gfx) * 1280,         // Display List Buffer 1 Size
        0,                          // Display List Buffer 2 Size
        0,                          // Display List Buffer 3 Size
        0x1000,                     // Graphics Heap Size
        2,                          // ???
        0x1000,                     // RDP Output Buffer Size
        NULL,         				// Pre-render function
        syControllerFuncRead,       // Controller I/O function
    },

    0,                              // Number of GObjThreads
    sizeof(u64) * 192,              // Thread stack size
    0,                              // Number of thread stacks
    0,                              // ???
    0,                              // Number of GObjProcesses
    0,                              // Number of GObjs
    sizeof(GObj),                   // GObj size
    0,                              // Number of XObjs
    NULL,                           // Matrix function list
    NULL,                           // DObjVec eject function
    0,                              // Number of AObjs
    0,                              // Number of MObjs
    0,                              // Number of DObjs
    sizeof(DObj),                   // DObj size
    0,                              // Number of SObjs
    sizeof(SObj),                   // SObj size
    0,                              // Number of CObjs
    sizeof(CObj),                 	// CObj size
    
    mnNoControllerFuncStart         // Task start function
};

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x800D67C0
s32 sMNNoControllerPad0x800D67C0;

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x800D6490
GObj* mnNoControllerMakeCamera(void)
{
	GObj *gobj = gcMakeCameraGObj
	(
		1000,
		NULL,
		0,
		GOBJ_PRIORITY_DEFAULT,
		lbCommonDrawSprite,
		100,
		COBJ_MASK_DLLINK(0),
		~0,
		FALSE,
		nGCProcessKindFunc,
		NULL,
		1,
		FALSE
	);
	CObj *cobj = CObjGetStruct(gobj);

	syRdpSetViewport(&cobj->viewport, 10.0F, 10.0F, 310.0F, 230.0F);

	return gobj;
}

// 0x800D6538
void mnNoControllerMakeImage(void)
{
	GObj *gobj;
	SObj *sobj;

	gobj = gcMakeGObjSPAfter(1001, NULL, 1, GOBJ_PRIORITY_DEFAULT);
	gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 0, GOBJ_PRIORITY_DEFAULT, ~0);

	sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, gMNNoControllerFiles[0], &llMNNoControllerSprite));

	sobj->pos.x = 10.0F;
	sobj->pos.y = 10.0F;
}

// 0x800D65B8
void mnNoControllerFuncStart(void)
{
	mnNoControllerSetupFiles();
	gcMakeDefaultCameraGObj(0, GOBJ_PRIORITY_DEFAULT, 100, COBJ_FLAG_NONE, GPACK_RGBA8888(0x00, 0x00, 0x00, 0xFF));
	mnNoControllerMakeCamera();
	mnNoControllerMakeImage();
}

// 0x800D6604
void mnNoControllerStartScene(void)
{
	dMNNoControllerVideoSetup.zbuffer = SYVIDEO_ZBUFFER_START(320, 240, 0, 10, u16);
	syVideoInit(&dMNNoControllerVideoSetup);

	dMNNoControllerTaskmanSetup.scene_setup.arena_size = (size_t) ((uintptr_t)&gSYFramebufferSets - (uintptr_t)&ovl11_BSS_END);
	syTaskmanStartTask(&dMNNoControllerTaskmanSetup);
}
