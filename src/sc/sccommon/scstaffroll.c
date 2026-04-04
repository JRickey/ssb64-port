#include <sc/scene.h>
#include <gm/gmsound.h>
#include <sys/video.h>
#include <sys/controller.h>
#include <sys/interp.h>
#include <sys/rdp.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x80135260
s32 dSCStaffrollNameCharacters[/* */] =
{
	#include "credits/staff.credits.encoded"
};

// 0x801364F4
SCStaffrollText dSCStaffrollNameTextInfo[/* */] =
{
	#include "credits/staff.credits.metadata"
};

// 0x80136794
s32 dSCStaffrollUnused0x80136794[/* */] =
{
#if defined(REGION_US)
	0x000004A5, 
#else
	0x0000046D, 
#endif
	0x00000000
};

// 0x8013679C
SCStaffrollJob dSCStaffrollJobDescriptions[/* */] =
{
	// Director
	{ 
		-1,			// Prefix ID
		0, 			// Job string ID
		1 			// Show next job from this staff member in order
	},
	
	// Chief Programmer
	{ 
		1,			// Prefix ID
#if defined(REGION_US)
		16, 		// Job string ID
#else
		2, 		    // Job string ID
#endif
		2 			// Show next job from this staff member in order
	},

	// Programmers
	{
		-1,			// Prefix ID
		2, 			// Job string ID
		7 			// Show next job from this staff member in order
	},
	
	// Chief Designer
	{
		1,			// Prefix ID
#if defined(REGION_US)
		17, 		// Job string ID
#else
		3, 		    // Job string ID
#endif
		8 			// Show next job from this staff member in order
	},

	// Designers
	{
		-1,			// Prefix ID
		3, 			// Job string ID
		27 			// Show next job from this staff member in order
	},

	// Sound Composer
	{
		-1,			// Prefix ID
		4, 			// Job string ID
		28 			// Show next job from this staff member in order
	},

	// Support
	{
		-1,			// Prefix ID
		5, 			// Job string ID
		29 			// Show next job from this staff member in order
	},

	// Voice Actors
	{
		-1,			// Prefix ID
		6, 			// Job string ID
		38 			// Show next job from this staff member in order
	},

	// Original Game Staff
	{
		-1,			// Prefix ID
		8, 			// Job string ID
		56 			// Show next job from this staff member in order
	},

#if defined(REGION_US)
	// NOA Staff
	{
		-1,			// Prefix ID
		15, 		// Job string ID
		59 			// Show next job from this staff member in order
	},
#endif

	// Special Thanks
	{
		-1,			// Prefix ID
		7, 			// Job string ID
#if defined(REGION_US)
		75 			// Show next job from this staff member in order
#else
		71 			// Show next job from this staff member in order
#endif
	},

	// Project Manager
	{
		9,			// Prefix ID
		10, 		// Job string ID
#if defined(REGION_US)
		76 			// Show next job from this staff member in order
#else
		72 			// Show next job from this staff member in order
#endif
	},

	// Progress Manager
	{
		11,			// Prefix ID
		10, 		// Job string ID
#if defined(REGION_US)
		77 			// Show next job from this staff member in order
#else
		73 			// Show next job from this staff member in order
#endif
	},

	// Producer
	{
		-1,			// Prefix ID
		12, 		// Job string ID
#if defined(REGION_US)
		81 			// Show next job from this staff member in order
#else
		77 			// Show next job from this staff member in order
#endif
	},

	// Executive Producer
	{
		13,			// Prefix ID
		12, 		// Job string ID
#if defined(REGION_US)
		82 			// Show next job from this staff member in order
#else
		78 			// Show next job from this staff member in order
#endif
	},

	// Presents
	{
		-1,			// Prefix ID
		14, 		// Job string ID
		-1 			// Show next job from this staff member in order
	}
};

// 0x8013685C
s32 dSCStaffrollJobCharacters[/* */] =
{
	#include "credits/titles.credits.encoded"
};

// 0x80136B10
SCStaffrollText dSCStaffrollJobTextInfo[/* */] =
{
	#include "credits/titles.credits.metadata"
};

// 0x80136BA0
s32 dSCStaffrollStaffRoleCharacters[/* */] =
{
	#include "credits/info.credits.encoded"
};

// 0x80139B68
SCStaffrollText dSCStaffrollStaffRoleTextInfo[/* */] =
{
	#include "credits/info.credits.metadata"
};

// 0x80139E08
s32 dSCStaffrollCompanyCharacters[/* */] =
{
	#include "credits/companies.credits.encoded"
};

// 0x80139FD4
SCStaffrollText dSCStaffrollCompanyTextInfo[/* */] =
{
	#include "credits/companies.credits.metadata"
};

// 0x8013A02C - Empty word in company text info
#if defined(REGION_US)
s32 dSCStaffrollUnused0x8013A02C[/* */] = { 0x00000073, 0x00000000 };
#else
s32 dSCStaffrollUnused0x8013A02C[/* */] = { 0x0000006E, 0x00000000 };
#endif

// 0x8013A034
s32 dSCStaffrollCompanyIDs[/* */] =
{
	nSCStaffrollCompanyHAL,
	nSCStaffrollCompanyHAL,
	nSCStaffrollCompanyHAL,
	nSCStaffrollCompanyHAL,
	nSCStaffrollCompanyHAL,
	nSCStaffrollCompanyHAL,
	nSCStaffrollCompanyHAL,
	nSCStaffrollCompanyHAL,
	nSCStaffrollCompanyHAL,
	nSCStaffrollCompanyHAL,
	nSCStaffrollCompanyHAL,
	nSCStaffrollCompanyHAL,
	nSCStaffrollCompanyHAL,
	nSCStaffrollCompanyHAL,
	nSCStaffrollCompanyHAL,
	nSCStaffrollCompanyHAL,
	nSCStaffrollCompanyHAL,
	nSCStaffrollCompanyHAL,
	nSCStaffrollCompanyHAL,
	nSCStaffrollCompanyNull,
	nSCStaffrollCompanyNull,
	nSCStaffrollCompanyNull,
	nSCStaffrollCompanyNull,
	nSCStaffrollCompanyNull,
	nSCStaffrollCompanyCreatures,
	nSCStaffrollCompanyCreatures,
	nSCStaffrollCompanyCreatures,
	nSCStaffrollCompanyHAL,
	nSCStaffrollCompanyHAL,
	nSCStaffrollCompanyNull,
	nSCStaffrollCompanyARTSVISION,
	nSCStaffrollCompanyNINTENDO,
	nSCStaffrollCompanyAONIProd,
	nSCStaffrollCompanyAONIProd,
	nSCStaffrollCompanyEZAKIProd,
	nSCStaffrollCompanyAONIProd,
#if defined(REGION_US)
	nSCStaffrollCompanyNull,
#else
	nSCStaffrollCompanyKENProd,
#endif
	nSCStaffrollCompanyMickeys,
	nSCStaffrollCompanyNINTENDO,
	nSCStaffrollCompanyNINTENDO,
	nSCStaffrollCompanyNINTENDO,
	nSCStaffrollCompanyNINTENDO,
#if defined(REGION_US)
	nSCStaffrollCompanyNull,
#else
	nSCStaffrollCompanyRare,
#endif
	nSCStaffrollCompanyNINTENDO,
	nSCStaffrollCompanyNINTENDO,
	nSCStaffrollCompanyNINTENDO,
	nSCStaffrollCompanyHAL,
	nSCStaffrollCompanyNINTENDO,
	nSCStaffrollCompanyNINTENDO,
	nSCStaffrollCompanyGAMEFREAK,
	nSCStaffrollCompanyCreatures,
	nSCStaffrollCompanyNINTENDO,
	nSCStaffrollCompanyHAL,
	nSCStaffrollCompanyNINTENDO,
	nSCStaffrollCompanyCreatures,
	nSCStaffrollCompanyNull,
#if defined(REGION_US)
	nSCStaffrollCompanyNOA,
	nSCStaffrollCompanyNOA,
	nSCStaffrollCompanyNOA,
#endif
	nSCStaffrollCompanyHAL,
	nSCStaffrollCompanyHAL,
	nSCStaffrollCompanyHAL,
	nSCStaffrollCompanyHAL,
	nSCStaffrollCompanyHAL,
	nSCStaffrollCompanyHAL,
	nSCStaffrollCompanyHAL,
	nSCStaffrollCompanyNINTENDO,
	nSCStaffrollCompanyNINTENDO,
	nSCStaffrollCompanyCreatures,
	nSCStaffrollCompanyNINTENDO,
	nSCStaffrollCompanyNINTENDO,
	nSCStaffrollCompanyNINTENDO,
	nSCStaffrollCompanyNull,
	nSCStaffrollCompanyNull,
#if defined(REGION_US)
	nSCStaffrollCompanyNull,
#endif
	nSCStaffrollCompanyHAL,
	nSCStaffrollCompanyNINTENDO,
	nSCStaffrollCompanyHAL,
	nSCStaffrollCompanyHAL,
	nSCStaffrollCompanyNINTENDO,
	nSCStaffrollCompanyNINTENDO,
	nSCStaffrollCompanyNINTENDO,
	nSCStaffrollCompanyNull,
	nSCStaffrollCompanyNull
};

// 0x8013A184
u32 dSCStaffrollFileIDs[/* */] = { &llSCStaffrollFileID };

// 0x8013A188
SCStaffrollSprite dSCStaffrollNameAndJobSpriteInfo[/* */] =
{
	{ 20, 22, &llSCStaffrollNameAndJobAUpperImage },
	{ 15, 22, &llSCStaffrollNameAndJobBUpperImage },
	{ 15, 22, &llSCStaffrollNameAndJobCUpperImage },
	{ 18, 22, &llSCStaffrollNameAndJobDUpperImage },
	{ 13, 22, &llSCStaffrollNameAndJobEUpperImage },
	{ 13, 22, &llSCStaffrollNameAndJobFUpperImage },
	{ 19, 22, &llSCStaffrollNameAndJobGUpperImage },
	{ 18, 22, &llSCStaffrollNameAndJobHUpperImage },
	{  7, 22, &llSCStaffrollNameAndJobIUpperImage },
	{ 11, 22, &llSCStaffrollNameAndJobJUpperImage },
	{ 18, 22, &llSCStaffrollNameAndJobKUpperImage },
	{ 13, 22, &llSCStaffrollNameAndJobLUpperImage },
	{ 23, 22, &llSCStaffrollNameAndJobMUpperImage },
	{ 19, 22, &llSCStaffrollNameAndJobNUpperImage },
	{ 22, 22, &llSCStaffrollNameAndJobOUpperImage },
	{ 15, 22, &llSCStaffrollNameAndJobPUpperImage },
	{ 23, 22, &llSCStaffrollNameAndJobQUpperImage },
	{ 16, 22, &llSCStaffrollNameAndJobRUpperImage },
	{ 15, 22, &llSCStaffrollNameAndJobSUpperImage },
	{ 15, 22, &llSCStaffrollNameAndJobTUpperImage },
	{ 16, 22, &llSCStaffrollNameAndJobUUpperImage },
	{ 18, 22, &llSCStaffrollNameAndJobVUpperImage },
	{ 25, 22, &llSCStaffrollNameAndJobWUpperImage },
	{ 19, 22, &llSCStaffrollNameAndJobXUpperImage },
	{ 16, 22, &llSCStaffrollNameAndJobYUpperImage },
	{ 19, 22, &llSCStaffrollNameAndJobZUpperImage },
	{ 14, 18, &llSCStaffrollNameAndJobALowerImage },
	{ 16, 22, &llSCStaffrollNameAndJobBLowerImage },
	{ 14, 18, &llSCStaffrollNameAndJobCLowerImage },
	{ 16, 22, &llSCStaffrollNameAndJobDLowerImage },
	{ 16, 18, &llSCStaffrollNameAndJobELowerImage },
	{ 11, 22, &llSCStaffrollNameAndJobFLowerImage },
	{ 15, 22, &llSCStaffrollNameAndJobGLowerImage },
	{ 15, 22, &llSCStaffrollNameAndJobHLowerImage },
	{  6, 22, &llSCStaffrollNameAndJobILowerImage },
	{  8, 26, &llSCStaffrollNameAndJobJLowerImage },
	{ 16, 22, &llSCStaffrollNameAndJobKLowerImage },
	{  6, 22, &llSCStaffrollNameAndJobLLowerImage },
	{ 20, 18, &llSCStaffrollNameAndJobMLowerImage },
	{ 15, 18, &llSCStaffrollNameAndJobNLowerImage },
	{ 18, 18, &llSCStaffrollNameAndJobOLowerImage },
	{ 16, 22, &llSCStaffrollNameAndJobPLowerImage },
	{ 16, 22, &llSCStaffrollNameAndJobQLowerImage },
	{ 11, 18, &llSCStaffrollNameAndJobRLowerImage },
	{ 13, 18, &llSCStaffrollNameAndJobSLowerImage },
	{ 11, 22, &llSCStaffrollNameAndJobTLowerImage },
	{ 15, 18, &llSCStaffrollNameAndJobULowerImage },
	{ 14, 18, &llSCStaffrollNameAndJobVLowerImage },
	{ 21, 18, &llSCStaffrollNameAndJobWLowerImage },
	{ 15, 18, &llSCStaffrollNameAndJobXLowerImage },
	{ 16, 22, &llSCStaffrollNameAndJobYLowerImage },
	{ 14, 17, &llSCStaffrollNameAndJobZLowerImage },
	{  7,  7, &llSCStaffrollNameAndJobPeriodImage },
	{  8, 11, &llSCStaffrollNameAndJobCommaImage },
	{  8, 11, &llSCStaffrollNameAndJobApostropheImage },
#if defined(REGION_US)
	{ 16, 22, &llSCStaffrollNameAndJob4Image }
#endif
};

// 0x8013A348
SCStaffrollSprite dSCStaffrollTextBoxSpriteInfo[/* */] =
{
	{ 12, 14, &llSCStaffrollTextBoxAUpperSprite },
	{ 12, 14, &llSCStaffrollTextBoxBUpperSprite },
	{ 12, 14, &llSCStaffrollTextBoxCUpperSprite },
	{ 12, 14, &llSCStaffrollTextBoxDUpperSprite },
	{ 12, 14, &llSCStaffrollTextBoxEUpperSprite },
	{ 12, 14, &llSCStaffrollTextBoxFUpperSprite },
	{ 12, 14, &llSCStaffrollTextBoxGUpperSprite },
	{ 12, 14, &llSCStaffrollTextBoxHUpperSprite },
	{  5, 14, &llSCStaffrollTextBoxIUpperSprite },
	{ 12, 14, &llSCStaffrollTextBoxJUpperSprite },
	{ 12, 14, &llSCStaffrollTextBoxKUpperSprite },
	{ 12, 14, &llSCStaffrollTextBoxLUpperSprite },
	{ 14, 14, &llSCStaffrollTextBoxMUpperSprite },
	{ 12, 14, &llSCStaffrollTextBoxNUpperSprite },
	{ 12, 14, &llSCStaffrollTextBoxOUpperSprite },
	{ 12, 14, &llSCStaffrollTextBoxPUpperSprite },
	{ 13, 14, &llSCStaffrollTextBoxQUpperSprite },
	{ 12, 14, &llSCStaffrollTextBoxRUpperSprite },
	{ 12, 14, &llSCStaffrollTextBoxSUpperSprite },
	{ 13, 14, &llSCStaffrollTextBoxTUpperSprite },
	{ 12, 14, &llSCStaffrollTextBoxUUpperSprite },
	{ 14, 14, &llSCStaffrollTextBoxVUpperSprite },
	{ 14, 14, &llSCStaffrollTextBoxWUpperSprite },
	{ 12, 14, &llSCStaffrollTextBoxXUpperSprite },
	{ 13, 14, &llSCStaffrollTextBoxYUpperSprite },
	{ 14, 14, &llSCStaffrollTextBoxZUpperSprite },
	{ 10, 11, &llSCStaffrollTextBoxALowerSprite },
	{ 10, 13, &llSCStaffrollTextBoxBLowerSprite },
	{ 10, 11, &llSCStaffrollTextBoxCLowerSprite },
	{ 10, 13, &llSCStaffrollTextBoxDLowerSprite },
	{ 10, 11, &llSCStaffrollTextBoxELowerSprite },
	{  9, 13, &llSCStaffrollTextBoxFLowerSprite },
	{ 10, 12, &llSCStaffrollTextBoxGLowerSprite },
	{ 10, 13, &llSCStaffrollTextBoxHLowerSprite },
	{ 04, 13, &llSCStaffrollTextBoxILowerSprite },
	{ 06, 14, &llSCStaffrollTextBoxJLowerSprite },
	{ 10, 13, &llSCStaffrollTextBoxKLowerSprite },
	{ 04, 13, &llSCStaffrollTextBoxLLowerSprite },
	{ 12, 11, &llSCStaffrollTextBoxMLowerSprite },
	{ 10, 11, &llSCStaffrollTextBoxNLowerSprite },
	{ 10, 11, &llSCStaffrollTextBoxOLowerSprite },
	{ 10, 12, &llSCStaffrollTextBoxPLowerSprite },
	{ 10, 12, &llSCStaffrollTextBoxQLowerSprite },
	{  9, 11, &llSCStaffrollTextBoxRLowerSprite },
	{ 10, 11, &llSCStaffrollTextBoxSLowerSprite },
	{  9, 13, &llSCStaffrollTextBoxTLowerSprite },
	{ 10, 11, &llSCStaffrollTextBoxULowerSprite },
	{ 10, 11, &llSCStaffrollTextBoxVLowerSprite },
	{ 12, 11, &llSCStaffrollTextBoxWLowerSprite },
	{ 12, 11, &llSCStaffrollTextBoxXLowerSprite },
	{ 10, 12, &llSCStaffrollTextBoxYLowerSprite },
	{ 10, 11, &llSCStaffrollTextBoxZLowerSprite },
	{  5, 11, &llSCStaffrollTextBoxColonSprite },
	{ 13, 14, &llSCStaffrollTextBox9Sprite },
	{ 13, 14, &llSCStaffrollTextBox8Sprite },
	{ 13, 14, &llSCStaffrollTextBox7Sprite },
	{ 13, 14, &llSCStaffrollTextBox6Sprite },
	{ 13, 14, &llSCStaffrollTextBox5Sprite },
	{ 13, 14, &llSCStaffrollTextBox4Sprite },
	{ 13, 14, &llSCStaffrollTextBox3Sprite },
	{ 13, 14, &llSCStaffrollTextBox2Sprite },
	{  9, 14, &llSCStaffrollTextBox1Sprite },
	{ 13, 14, &llSCStaffrollTextBox0Sprite },
	{  5,  5, &llSCStaffrollTextBoxPeriodSprite },
	{  9, 04, &llSCStaffrollTextBoxDashSprite },
	{  5,  5, &llSCStaffrollTextBoxCommaSprite },
	{ 16, 14, &llSCStaffrollTextBoxAmpersSprite },
	{  5,  5, &llSCStaffrollTextBoxQuoteSprite },
	{ 06, 12, &llSCStaffrollTextBoxSlashSprite },
	{  5,  5, &llSCStaffrollTextBoxApostropheSprite },
	{ 12, 14, &llSCStaffrollTextBoxQuestionSprite },
	{  7, 14, &llSCStaffrollTextBoxBracketOpenSprite },
	{  7, 14, &llSCStaffrollTextBoxBracketCloseSprite },
#if defined(REGION_US)
	{ 10, 13, &llSCStaffrollTextBoxEAccentSprite }
#endif
};

// 0x8013A598
Gfx dSCStaffrollTextBoxDisplayList[/* */] =
{
	gsDPPipeSync(),
	gsDPSetCycleType(G_CYC_FILL),
	gsDPSetRenderMode(G_RM_NOOP, G_RM_NOOP2),
	gsDPSetFillColor(GPACK_FILL16(GPACK_RGBA5551(0x42, 0x3A, 0x31, 0x01))),
	gsDPFillRectangle(346, 35, 348, 164),
	gsDPFillRectangle(346, 35, 584,  37),
	gsDPFillRectangle(582, 35, 584, 164),
	gsDPFillRectangle(346, 162, 584, 164),
	gsSPEndDisplayList()
};

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x8013A7D0
s32 sSCStaffrollPad0x8013A7D0[2];

// 0x8013A7D8
#if defined(REGION_US)
Gfx *sSCStaffrollNameAndJobDisplayLists[56];
#else
Gfx *sSCStaffrollNameAndJobDisplayLists[55];
#endif

// 0x8013A8B8
s32 sSCStaffrollNameID;

// 0x8013A8BC
f32 sSCStaffrollRollSpeed;

// 0x8013A8C0
s32 sSCStaffrollStatus;

// 0x8013A8C4
SCStaffrollName *sSCStaffrollNameAllocFree;

// 0x8013A8C8
GObj *sSCStaffrollScrollGObj;

// 0x8013A8CC
GObj *sSCStaffrollCrosshairGObj;

// 0x8013A8D0
sb32 sSCStaffrollIsPaused;

// 0x8013A8D4
f32 sSCStaffrollCrosshairPositionX;

// 0x8013A8D8
f32 sSCStaffrollCrosshairPositionY;

// 0x8013A8DC
AObjEvent32 *sSCStaffrollNameAnimJoint;

// 0x8013A8E0
SYInterpDesc *sSCStaffrollNameInterpolation;

// 0x8013A8E4
CObj *sSCStaffrollCamera;

// 0x8013A8E8
DObjDesc *sSCStaffrollDObjDesc;

// 0x8013A8EC
s32 sSCStaffrollHighlightSize;

// 0x8013A8F0
f32 sSCStaffrollHighlightPositionX;

// 0x8013A8F4
f32 sSCStaffrollHighlightPositionY;

// 0x8013A8F8
GObj *sSCStaffrollStaffRoleTextGObj;

// 0x8013A8FC
GObj *sSCStaffrollCompanyTextGObj;

// 0x8013A900
s32 sSCStaffrollRollBeginWait;

// 0x8013A904 - controller port to use for credits sequence
u8 sSCStaffrollPlayer;

// 0x8013A908
s32 sSCStaffrollRollEndWait;

// 0x8013A910
LBFileNode sSCStaffrollStatusBuffer[32];

// 0x8013AA10
void *sSCStaffrollFiles[ARRAY_COUNT(dSCStaffrollFileIDs)];

// 0x8013AA18
Mtx44f sSCStaffrollMatrix;

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80131B00 - Resume staff roll on button press, return bool for if paused or not
sb32 scStaffrollGetPauseStatusResume(void)
{
	sb32 is_paused = TRUE;
	u16 button_tap = gSYControllerDevices[sSCStaffrollPlayer].button_tap;

	if (button_tap & (A_BUTTON | B_BUTTON | Z_TRIG | START_BUTTON))
	{
		GObj *name_gobj;
		GObj *job_gobj;

		if (sSCStaffrollScrollGObj != NULL)
		{
			gcResumeGObjProcessAll(sSCStaffrollScrollGObj);
		}
		name_gobj = gGCCommonLinks[3];

		while (name_gobj != NULL)
		{
			gcResumeGObjProcessAll(name_gobj);

			name_gobj = name_gobj->link_next;
		}
		job_gobj = gGCCommonLinks[4];

		while (job_gobj != NULL)
		{
			gcResumeGObjProcessAll(job_gobj);

			job_gobj = job_gobj->link_next;
		}
		is_paused = FALSE;
	}
	return is_paused;
}

// 0x80131BB0
void func_ovl59_80131BB0(Mtx44f mtx, Vec3f *vec, f32 *width, f32 *height)
{
	f32 x = vec->x;
	f32 y = vec->y;
	f32 z = vec->z;

	f32 w = (mtx[0][0] * x) + (mtx[1][0] * y) + (mtx[2][0] * z) + mtx[3][0];
	f32 h = (mtx[0][1] * x) + (mtx[1][1] * y) + (mtx[2][1] * z) + mtx[3][1];
	f32 i = (mtx[0][3] * x) + (mtx[1][3] * y) + (mtx[2][3] * z) + mtx[3][3];

	i = (1.0F / i);

	*width = w * i * 640.0F * 0.5F;
	*height = h * i * 480.0F * 0.5F;
}

// 0x80131C88
void func_ovl59_80131C88(CObj *cobj)
{
	Mtx44f m, n;

	syMatrixPerspFastF(n, &cobj->projection.persp.norm, cobj->projection.persp.fovy, cobj->projection.persp.aspect, cobj->projection.persp.near, cobj->projection.persp.far, cobj->projection.persp.scale);
	syMatrixLookAtF(&m, cobj->vec.eye.x, cobj->vec.eye.y, cobj->vec.eye.z, cobj->vec.at.x, cobj->vec.at.y, cobj->vec.at.z, cobj->vec.up.x, cobj->vec.up.y, cobj->vec.up.z);
	guMtxCatF(m, n, sSCStaffrollMatrix);
}

// 0x80131D30
void func_ovl59_80131D30(DObj *dobj, Vec3f *vec, f32 *width, f32 *height)
{
	Mtx44f m, r;

	syMatrixTraRotRpyRScaF
	(
		&m,
		dobj->translate.vec.f.x,
		dobj->translate.vec.f.y,
		dobj->translate.vec.f.z,
		dobj->rotate.vec.f.x,
		dobj->rotate.vec.f.y,
		dobj->rotate.vec.f.z,
		dobj->scale.vec.f.x,
		dobj->scale.vec.f.y,
		dobj->scale.vec.f.z
	);
	guMtxCatF(m, sSCStaffrollMatrix, r);
	func_ovl59_80131BB0(r, vec, width, height);
}

// 0x80131DD0
void func_ovl59_80131DD0(GObj *gobj, SCStaffrollProjection *proj)
{
	SCStaffrollMatrix *credits = gobj->user_data.p;

	proj->pv0.z = proj->pv1.z = proj->pv2.z = proj->pv3.z = 0.0F;
	proj->pv0.y = proj->pv2.y = 28.0F;

	proj->pv1.y = proj->pv3.y = -(credits->unk_gmcreditsmtx_0x10 + 4.0F);
	proj->pv0.x = proj->pv1.x = -22.0F;

	proj->pv2.x = proj->pv3.x = (ABS(credits->unk_gmcreditsmtx_0xC) * 2) + 18.0F;
}

// 0x80131E70
void func_ovl59_80131E70(Vec3f *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4)
{
	arg0->x = arg2 - arg4;
	arg0->y = arg3 - arg1;
	arg0->z = (arg1 * arg4) - (arg3 * arg2);
}

// 0x80131EB8
sb32 scStaffrollCheckCursorNameOverlap(Vec3f *vec)
{
	SObj *sobj = SObjGetStruct(sSCStaffrollCrosshairGObj);
	f32 x = (sobj->pos.x + 29.0F) - 320.0F;
	f32 y = 240.0F - (sobj->pos.y + 29.0F);
	f32 v = (vec->x * x) + (vec->y * y) + vec->z;
	sb32 ret = TRUE;

	if (v < 0.0F)
	{
		ret = FALSE;
	}
	return ret;
}

// 0x80131F34
void func_ovl59_80131F34(GObj *arg0)
{
	GObj *ugobj = arg0->user_data.p;
	SCStaffrollMatrix *credits = ugobj->user_data.p;

	if ((credits->unk_gmcreditsmtx_0x14 + sSCStaffrollRollSpeed) >= 1.0F)
	{
		gcEjectGObj(NULL);
		gcEjectGObj(sSCStaffrollStaffRoleTextGObj);
		gcEjectGObj(sSCStaffrollCompanyTextGObj);
	}
	else
	{
		DObj *dobj = DObjGetStruct(arg0)->child;

		DObjGetStruct(arg0)->translate.vec.f = DObjGetStruct(ugobj)->translate.vec.f;
		DObjGetStruct(arg0)->rotate.vec.f = DObjGetStruct(ugobj)->rotate.vec.f;

		dobj->translate.vec.f.x = (ABS(credits->unk_gmcreditsmtx_0xC) * 2) + 18.0F;
	}
}

// 0x8013202C
void func_ovl59_8013202C(GObj *arg0)
{
	GObj *gobj = gGCCommonLinks[nGCCommonLinkID02];
	GObj *ugobj = arg0->user_data.p;

	if (gobj == NULL)
	{
		gobj = gcMakeGObjSPAfter(8, NULL, nGCCommonLinkID02, GOBJ_PRIORITY_DEFAULT);
		gcAddGObjDisplay(gobj, gcDrawDObjTreeForGObj, 3, GOBJ_PRIORITY_DEFAULT, ~0);
		gcSetupCustomDObjs(gobj, sSCStaffrollDObjDesc, NULL, nGCMatrixKindTraRotRpyRSca, nGCMatrixKindNull, nGCMatrixKindNull);
		gcAddGObjProcess(gobj, func_ovl59_80131F34, nGCProcessKindFunc, 1);

		gobj->user_data.p = arg0;
		ugobj->unk_gobj_0x1C = gobj;
	}
	else
	{
		gobj->user_data.p = arg0;
		ugobj->unk_gobj_0x1C = gobj;
	}
}

// 0x801320F4
s32 scStaffrollGetLockOnPositionX(s32 pos_x)
{
	s32 bound_x = pos_x;

	if (pos_x < 20)
	{
		bound_x = 20;
	}
	if (pos_x > 620)
	{
		bound_x = 620;
	}
	return bound_x;
}

// 0x8013211C
s32 scStaffrollGetLockOnPositionY(s32 pos_y)
{
	s32 bound_y = pos_y;

	if (pos_y < 20)
	{
		bound_y = 20;
	}
	if (pos_y > 460)
	{
		bound_y = 460;
	}
	return bound_y;
}

// 0x80132144
void scStaffrollHighlightProcDisplay(GObj *gobj)
{
	s32 unused;

	gDPPipeSync(gSYTaskmanDLHeads[0]++);
	gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_FILL);
	gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_NOOP, G_RM_NOOP2);
	gDPSetFillColor(gSYTaskmanDLHeads[0]++, GPACK_FILL16(GPACK_RGBA5551(0x80, 0x00, 0x00, 0x01)));
	gDPFillRectangle
	(
		gSYTaskmanDLHeads[0]++,
		scStaffrollGetLockOnPositionX((sSCStaffrollHighlightSize * -30) + sSCStaffrollHighlightPositionX),
		scStaffrollGetLockOnPositionY((sSCStaffrollHighlightSize * -25) + sSCStaffrollHighlightPositionY),
		scStaffrollGetLockOnPositionX(((sSCStaffrollHighlightSize * -30) + 2) + sSCStaffrollHighlightPositionX),
		scStaffrollGetLockOnPositionY(((sSCStaffrollHighlightSize * 45) + 2) + sSCStaffrollHighlightPositionY)
	);
	gDPFillRectangle
	(
		gSYTaskmanDLHeads[0]++,
		scStaffrollGetLockOnPositionX((sSCStaffrollHighlightSize * -30) + sSCStaffrollHighlightPositionX),
		scStaffrollGetLockOnPositionY((sSCStaffrollHighlightSize * -25) + sSCStaffrollHighlightPositionY),
		scStaffrollGetLockOnPositionX(((sSCStaffrollHighlightSize * 65) + 2) + sSCStaffrollHighlightPositionX),
		scStaffrollGetLockOnPositionY(((sSCStaffrollHighlightSize * -25) + 2) + sSCStaffrollHighlightPositionY)
	);
	gDPFillRectangle
	(
		gSYTaskmanDLHeads[0]++,
		scStaffrollGetLockOnPositionX((sSCStaffrollHighlightSize * -30) + sSCStaffrollHighlightPositionX),
		scStaffrollGetLockOnPositionY((sSCStaffrollHighlightSize * 45) + sSCStaffrollHighlightPositionY),
		scStaffrollGetLockOnPositionX(((sSCStaffrollHighlightSize * 65) + 2) + sSCStaffrollHighlightPositionX),
		scStaffrollGetLockOnPositionY(((sSCStaffrollHighlightSize * 45) + 2) + sSCStaffrollHighlightPositionY)
	);
	gDPFillRectangle
	(
		gSYTaskmanDLHeads[0]++,
		scStaffrollGetLockOnPositionX((sSCStaffrollHighlightSize * 65) + sSCStaffrollHighlightPositionX),
		scStaffrollGetLockOnPositionY((sSCStaffrollHighlightSize * -25) + sSCStaffrollHighlightPositionY),
		scStaffrollGetLockOnPositionX(((sSCStaffrollHighlightSize * 65) + 2) + sSCStaffrollHighlightPositionX),
		scStaffrollGetLockOnPositionY(((sSCStaffrollHighlightSize * 45) + 2) + sSCStaffrollHighlightPositionY)
	);
}

// 0x8013270C
void scStaffrollHighlightThreadUpdate(GObj *gobj)
{
	s32 i;

	for (i = 0; i < 3; i++)
	{
		sSCStaffrollHighlightSize = 6;

		while (sSCStaffrollHighlightSize != 0)
		{
			sSCStaffrollHighlightSize--;
			gcSleepCurrentGObjThread(1);
		}
	}
	gcEjectGObj(NULL);
	gcSleepCurrentGObjThread(1);
}

// 0x801327A4
void scStaffrollMakeHighlightGObj(GObj *gobj)
{
	GObj *highlight_gobj = gGCCommonLinks[nGCCommonLinkIDHighlight];
	s32 unused;
	SObj *sobj = SObjGetStruct(sSCStaffrollCrosshairGObj);

	if (highlight_gobj == NULL)
	{
		highlight_gobj = gcMakeGObjSPAfter(9, NULL, 9, GOBJ_PRIORITY_DEFAULT);

		gcAddGObjDisplay(highlight_gobj, scStaffrollHighlightProcDisplay, 8, GOBJ_PRIORITY_DEFAULT, ~0);
		gcAddGObjProcess(highlight_gobj, scStaffrollHighlightThreadUpdate, nGCProcessKindThread, 1);

		sSCStaffrollHighlightPositionX = sobj->pos.x + 8.0F;
		sSCStaffrollHighlightPositionY = sobj->pos.y + 20.0F;
	}
}

// 0x80132860
void scStaffrollSetTextQuetions(s32 *characters, s32 character_count)
{
	s32 i, j, k;
	s32 *cadd, *cbase = &dSCStaffrollStaffRoleCharacters[0];

	for (i = 0; i < ARRAY_COUNT(dSCStaffrollStaffRoleTextInfo) - 1; i++)
	{
		for (cadd = cbase, j = 0, k = 0; j < dSCStaffrollStaffRoleTextInfo[i].character_count; j++, cbase++)
		{
			if (*cbase == characters[k])
			{
				if (k == 0)
				{
					cadd = cbase;
				}
				k++;
			}
			else k = 0;

			if (k == character_count)
			{
				while (k != 0)
				{
					*cadd++ = GMSTAFFROLL_QUESTION_MARK_PARA_FONT_INDEX;

					k--;

					continue;
				}
				cadd = cbase;
				k = 0;
			}
		}
	}
}

// 0x80132958
void scStaffrollTryHideUnlocks(void)
{
	s32 unused[5];

	// 0x8013A5E0
	s32 luigi[/* */] =
	{	
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('L'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('u'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('i'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('g'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('i')
	};

	// 0x8013A5F4
	s32 purin[/* */] =
	{
#if defined(REGION_US)
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('J'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('i'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('g'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('g'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('l'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('y'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('p'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('u'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('f'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('f')
#else
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('P'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('u'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('r'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('i'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('n')
#endif
	};

	// 0x8013A61C
	s32 captain[/* */] =
	{
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('C'),
		GMSTAFFROLL_PERIOD_PARA_FONT_INDEX,
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('F'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('a'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('l'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('c'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('o'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('n')
	};

	// 0x8013A63C
	s32 ness[/* */] =
	{
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('N'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('e'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('s'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('s')
	};

	// 0x8013A64C
	s32 earthbound[/* */] =
	{
#if defined(REGION_US)
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('E'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('a'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('r'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('t'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('h'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('B'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('o'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('u'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('n'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('d')
#else
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('M'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('o'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('t'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('h'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('e'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('r'),
		GMSTAFFROLL_ASCII_NUMBER_TO_PARA_FONT_INDEX('2'),
#endif
	};

	// 0x8013A674
	s32 fzero[/* */] =
	{
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('F'),
		GMSTAFFROLL_DASH_PARA_FONT_INDEX,
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('Z'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('E'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('R'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('O'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX(' '),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('X')
	};

	// 0x8013A694
	s32 classicmario[/* */] =
	{
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('C'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('l'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('a'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('s'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('s'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('i'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('c'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('M'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('a'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('r'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('i'),
		GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('o')
	};

	if (!(gSCManagerBackupData.unlock_mask & LBBACKUP_UNLOCK_MASK_LUIGI))
	{
		scStaffrollSetTextQuetions(luigi, ARRAY_COUNT(luigi));
	}
	if (!(gSCManagerBackupData.unlock_mask & LBBACKUP_UNLOCK_MASK_PURIN))
	{
		scStaffrollSetTextQuetions(purin, ARRAY_COUNT(purin));
	}
	if (!(gSCManagerBackupData.unlock_mask & LBBACKUP_UNLOCK_MASK_CAPTAIN))
	{
		scStaffrollSetTextQuetions(captain, ARRAY_COUNT(captain));
		scStaffrollSetTextQuetions(fzero, ARRAY_COUNT(fzero));
	}
	if (!(gSCManagerBackupData.unlock_mask & LBBACKUP_UNLOCK_MASK_NESS))
	{
		scStaffrollSetTextQuetions(ness, ARRAY_COUNT(ness));
		scStaffrollSetTextQuetions(earthbound, ARRAY_COUNT(earthbound));
	}
	if (!(gSCManagerBackupData.unlock_mask & LBBACKUP_UNLOCK_MASK_INISHIE))
	{
		scStaffrollSetTextQuetions(classicmario, ARRAY_COUNT(classicmario));
	}
}

// 0x80132BB8
void scStaffrollMakeStaffRoleTextSObjs(GObj *text_gobj, GObj *staff_gobj)
{
	s32 character_id;
	SObj *sobj;
	s32 i;
	f32 hvar;
	s32 character_count;
	f32 wbase;
	f32 hbase;
	SCStaffrollName *staff = staff_gobj->user_data.p;

	wbase = 350.0F;
	hbase = 40.0F;

	character_count = dSCStaffrollStaffRoleTextInfo[staff->name_id].character_count;

	for (i = 0, character_id = dSCStaffrollStaffRoleTextInfo[staff->name_id].character_start; i < character_count; i++, character_id++)
	{
		if
		(
			(dSCStaffrollStaffRoleCharacters[character_id] != GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX(' ')) &&
			(dSCStaffrollStaffRoleCharacters[character_id] != GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('\n'))
		)
		{
			hvar = 0.0F;

			sobj = lbCommonMakeSObjForGObj(text_gobj, lbRelocGetFileData(Sprite*, sSCStaffrollFiles[0], dSCStaffrollTextBoxSpriteInfo[dSCStaffrollStaffRoleCharacters[character_id]].offset));

			sobj->sprite.attr = SP_TRANSPARENT;

			sobj->sprite.red   = 0xB7;
			sobj->sprite.green = 0xBC;
			sobj->sprite.blue  = 0xEC;

			sobj->pos.x = wbase;

			sobj->sprite.scalex = sobj->sprite.scaley = 1;

			if (dSCStaffrollStaffRoleCharacters[character_id] >= GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('a'))
			{
				hvar = 3.0F;

				if
				(
					dSCStaffrollStaffRoleCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('b')      ||
					dSCStaffrollStaffRoleCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('d')      ||
					dSCStaffrollStaffRoleCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('f')      ||
					dSCStaffrollStaffRoleCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('h')      ||
					dSCStaffrollStaffRoleCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('i')      ||
					dSCStaffrollStaffRoleCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('j')      ||
					dSCStaffrollStaffRoleCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('k')      ||
					dSCStaffrollStaffRoleCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('l')      ||
					dSCStaffrollStaffRoleCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('t')      ||
					dSCStaffrollStaffRoleCharacters[character_id] == GMSTAFFROLL_COLON_PARA_FONT_INDEX                ||
					dSCStaffrollStaffRoleCharacters[character_id] == GMSTAFFROLL_ASCII_NUMBER_TO_PARA_FONT_INDEX('9') ||
					dSCStaffrollStaffRoleCharacters[character_id] == GMSTAFFROLL_ASCII_NUMBER_TO_PARA_FONT_INDEX('8') ||
					dSCStaffrollStaffRoleCharacters[character_id] == GMSTAFFROLL_ASCII_NUMBER_TO_PARA_FONT_INDEX('7') ||
					dSCStaffrollStaffRoleCharacters[character_id] == GMSTAFFROLL_ASCII_NUMBER_TO_PARA_FONT_INDEX('6') ||
					dSCStaffrollStaffRoleCharacters[character_id] == GMSTAFFROLL_ASCII_NUMBER_TO_PARA_FONT_INDEX('5') ||
					dSCStaffrollStaffRoleCharacters[character_id] == GMSTAFFROLL_ASCII_NUMBER_TO_PARA_FONT_INDEX('4') ||
					dSCStaffrollStaffRoleCharacters[character_id] == GMSTAFFROLL_ASCII_NUMBER_TO_PARA_FONT_INDEX('3') ||
					dSCStaffrollStaffRoleCharacters[character_id] == GMSTAFFROLL_ASCII_NUMBER_TO_PARA_FONT_INDEX('2') ||
					dSCStaffrollStaffRoleCharacters[character_id] == GMSTAFFROLL_ASCII_NUMBER_TO_PARA_FONT_INDEX('1') ||
					dSCStaffrollStaffRoleCharacters[character_id] == GMSTAFFROLL_ASCII_NUMBER_TO_PARA_FONT_INDEX('0') ||
					dSCStaffrollStaffRoleCharacters[character_id] == GMSTAFFROLL_AMPERSAND_PARA_FONT_INDEX            ||
					dSCStaffrollStaffRoleCharacters[character_id] == GMSTAFFROLL_QUESTION_MARK_PARA_FONT_INDEX        ||
#if defined(REGION_US)
					dSCStaffrollStaffRoleCharacters[character_id] == GMSTAFFROLL_E_ACCENT_PARA_FONT_INDEX             ||
#endif
					dSCStaffrollStaffRoleCharacters[character_id] == GMSTAFFROLL_DOUBLE_QUOTES_PARA_FONT_INDEX
				)
				{
					hvar = 1.0F;
				}
			}
			if (dSCStaffrollStaffRoleCharacters[character_id] == GMSTAFFROLL_PERIOD_PARA_FONT_INDEX)
			{
				hvar += 6.0F;
			}
			if (dSCStaffrollStaffRoleCharacters[character_id] == GMSTAFFROLL_DASH_PARA_FONT_INDEX)
			{
				hvar += 2.0F;
			}
			if (dSCStaffrollStaffRoleCharacters[character_id] == GMSTAFFROLL_COMMA_PARA_FONT_INDEX)
			{
				hvar += 7.0F;
			}
			sobj->pos.y = hbase + hvar;

			wbase += dSCStaffrollTextBoxSpriteInfo[dSCStaffrollStaffRoleCharacters[character_id]].width;
		}
		else if (dSCStaffrollStaffRoleCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX(' '))
		{
			wbase += 3.0F;
		}
		else
		{
			wbase = 350.0F;
			hbase += 20.0F;
		}
	}
}

// 0x80132E74
void scStaffrollMakeStaffRoleTextGObj(GObj *staff_gobj)
{
	GObj *text_gobj;

	if (gGCCommonLinks[10] != NULL)
	{
		gcEjectGObj(gGCCommonLinks[10]);
	}
	text_gobj = gcMakeGObjSPAfter(6, NULL, 10, GOBJ_PRIORITY_DEFAULT);

	gcAddGObjDisplay(text_gobj, lbCommonDrawSObjAttr, 5, GOBJ_PRIORITY_DEFAULT, ~0);

	sSCStaffrollStaffRoleTextGObj = text_gobj;

	scStaffrollMakeStaffRoleTextSObjs(text_gobj, staff_gobj);
}

// 0x80132EF0
void scStaffrollMakeCompanyTextSObjs(GObj *text_gobj, GObj *staff_gobj)
{
	SObj *sobj;
	f32 hvar;
	f32 wbase;
	s32 character_id;
	s32 character_count;
	SCStaffrollName *staff = staff_gobj->user_data.p;
	s32 i;

	if (dSCStaffrollCompanyIDs[staff->name_id] != nSCStaffrollCompanyNull)
	{
		wbase = 350.0F;

		character_count = dSCStaffrollCompanyTextInfo[dSCStaffrollCompanyIDs[staff->name_id]].character_count;
		character_id = dSCStaffrollCompanyTextInfo[dSCStaffrollCompanyIDs[staff->name_id]].character_start;

		for (i = 0; i < character_count; i++, character_id++)
		{
			if (dSCStaffrollCompanyCharacters[character_id] != GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX(' '))
			{
				sobj = lbCommonMakeSObjForGObj(text_gobj, lbRelocGetFileData(Sprite*, sSCStaffrollFiles[0], dSCStaffrollTextBoxSpriteInfo[dSCStaffrollCompanyCharacters[character_id]].offset));

				hvar = 0.0F;

				sobj->sprite.attr = SP_TRANSPARENT;

				sobj->sprite.scalex = sobj->sprite.scaley = 1;

				sobj->sprite.red   = 0x80;
				sobj->sprite.green = 0x40;
				sobj->sprite.blue  = 0x80;

				sobj->pos.x = wbase;

				if (dSCStaffrollCompanyCharacters[character_id] >= GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('a'))
				{
					hvar = 3.0F;

					if
					(
						(dSCStaffrollCompanyCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('b')) ||
						(dSCStaffrollCompanyCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('d')) ||
						(dSCStaffrollCompanyCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('f')) ||
						(dSCStaffrollCompanyCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('h')) ||
						(dSCStaffrollCompanyCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('i')) ||
						(dSCStaffrollCompanyCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('j')) ||
						(dSCStaffrollCompanyCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('k')) ||
						(dSCStaffrollCompanyCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('l')) ||
						(dSCStaffrollCompanyCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('t')) ||
						(dSCStaffrollCompanyCharacters[character_id] == GMSTAFFROLL_COLON_PARA_FONT_INDEX) ||
						(dSCStaffrollCompanyCharacters[character_id] == GMSTAFFROLL_AMPERSAND_PARA_FONT_INDEX) ||
						(dSCStaffrollCompanyCharacters[character_id] == GMSTAFFROLL_DOUBLE_QUOTES_PARA_FONT_INDEX)
					)
					{
						hvar = 1.0F;
					}
				}
				if ((dSCStaffrollCompanyCharacters[character_id] == GMSTAFFROLL_OPEN_PARENTHESIS_PARA_FONT_INDEX) ||
					(dSCStaffrollCompanyCharacters[character_id] == GMSTAFFROLL_CLOSE_PARENTHESIS_PARA_FONT_INDEX))
				{
					hvar = 0.0F;
				}
				if (dSCStaffrollCompanyCharacters[character_id] == GMSTAFFROLL_PERIOD_PARA_FONT_INDEX)
				{
					hvar += 6.0F;
				}
				if (dSCStaffrollCompanyCharacters[character_id] == GMSTAFFROLL_DASH_PARA_FONT_INDEX)
				{
					hvar += 2.0F;
				}
				if (dSCStaffrollCompanyCharacters[character_id] == GMSTAFFROLL_COMMA_PARA_FONT_INDEX)
				{
					hvar += 7.0F;
				}
				sobj->pos.y = 140.0F + hvar;

				wbase += dSCStaffrollTextBoxSpriteInfo[dSCStaffrollCompanyCharacters[character_id]].width;
			}
			else wbase += 3.0F;
		}
	}
	if (text_gobj); // idk just take it
}

// 0x80133184
void scStaffrollMakeCompanyTextGObj(GObj *staff_gobj)
{
	GObj *text_gobj;

	if (gGCCommonLinks[11] != NULL)
	{
		gcEjectGObj(gGCCommonLinks[11]);
	}
	text_gobj = gcMakeGObjSPAfter(7, NULL, 0xB, GOBJ_PRIORITY_DEFAULT);

	gcAddGObjDisplay(text_gobj, lbCommonDrawSObjAttr, 6, GOBJ_PRIORITY_DEFAULT, ~0);

	sSCStaffrollCompanyTextGObj = text_gobj;

	scStaffrollMakeCompanyTextSObjs(text_gobj, staff_gobj);
}

// 0x80133200
sb32 scStaffrollCheckCursorHighlightPrompt(GObj *gobj, SCStaffrollProjection *proj)
{
	sb32 b;
	s32 unused;
	Vec3f sp4C;
	Vec3f sp40;
	Vec3f sp34;
	Vec3f sp28;

	b = TRUE;

	func_ovl59_80131E70(&sp4C, proj->px0, proj->py0, proj->px2, proj->py2);
	func_ovl59_80131E70(&sp40, proj->px1, proj->py1, proj->px3, proj->py3);
	func_ovl59_80131E70(&sp34, proj->px0, proj->py0, proj->px1, proj->py1);
	func_ovl59_80131E70(&sp28, proj->px2, proj->py2, proj->px3, proj->py3);

	if 
	(
		(scStaffrollCheckCursorNameOverlap(&sp4C) == FALSE) && 
		(scStaffrollCheckCursorNameOverlap(&sp40) != FALSE) && 
		(scStaffrollCheckCursorNameOverlap(&sp34) != FALSE) && 
		(scStaffrollCheckCursorNameOverlap(&sp28) == FALSE)
	)
	{
		func_800269C0_275C0(nSYAudioFGMTrainingSel);

		b = FALSE;

		func_ovl59_8013202C(gobj);
		scStaffrollMakeHighlightGObj(gobj);
		scStaffrollMakeStaffRoleTextGObj(gobj);
		scStaffrollMakeCompanyTextGObj(gobj);
	}
	return b;
}

// 0x8013330C
void func_ovl59_8013330C(void)
{
	GObj *gobj;
	DObj *dobj;
	SCStaffrollProjection proj;
	sb32 b;

	func_ovl59_80131C88(sSCStaffrollCamera);

	gobj = gGCCommonLinks[3];

	if (gobj != NULL)
	{
		do
		{
			dobj = DObjGetStruct(gobj);

			func_ovl59_80131DD0(gobj, &proj);
			func_ovl59_80131D30(dobj, &proj.pv0, &proj.px0, &proj.py0);
			func_ovl59_80131D30(dobj, &proj.pv1, &proj.px1, &proj.py1);
			func_ovl59_80131D30(dobj, &proj.pv2, &proj.px2, &proj.py2);
			func_ovl59_80131D30(dobj, &proj.pv3, &proj.px3, &proj.py3);

			b = scStaffrollCheckCursorHighlightPrompt(gobj, &proj);

			gobj = gobj->link_next;
		} 
		while ((gobj != NULL) && (b != FALSE));
	}
}

// 0x8013341C - Highlight staff member if A is pressed and/or pause if B is pressed, get bool for if paused or not
sb32 scStaffrollGetPauseStatusHighlight(void)
{
	GObj *gobj;
	u16 button_tap = gSYControllerDevices[sSCStaffrollPlayer].button_tap;
	sb32 is_paused = FALSE;

	if (button_tap & (A_BUTTON | B_BUTTON))
	{
		func_ovl59_8013330C();

		if (button_tap & B_BUTTON)
		{
			if (sSCStaffrollScrollGObj != NULL)
			{
				gcPauseGObjProcessAll(sSCStaffrollScrollGObj);
			}
			gobj = gGCCommonLinks[3];

			while (gobj != NULL)
			{
				gcPauseGObjProcessAll(gobj);

				gobj = gobj->link_next;
			}
			gobj = gGCCommonLinks[4];

			while (gobj != NULL)
			{
				gcPauseGObjProcessAll(gobj);

				gobj = gobj->link_next;
			}
			is_paused = TRUE;
		}
	}
	return is_paused;
}

// 0x801334E4
void scStaffrollFuncRun(GObj *gobj)
{
	sb32 is_paused;
	u16 button_tap;

	if ((sSCStaffrollRollEndWait == 0) || (sSCStaffrollStatus != -1) && (sSCStaffrollStatus != -2))
	{
		button_tap = gSYControllerDevices[sSCStaffrollPlayer].button_tap;

		if (sSCStaffrollStatus == 1)
		{
			if (sSCStaffrollRollBeginWait < 120)
			{
				sSCStaffrollRollBeginWait++;
			}
			else
			{
				scStaffrollMakeTextBoxBracketSObjs();
				scStaffrollMakeTextBoxGObj();
				sSCStaffrollStatus = 0;
			}
		}
		is_paused = sSCStaffrollIsPaused;

		if (sSCStaffrollIsPaused == FALSE)
		{
			is_paused = scStaffrollGetPauseStatusHighlight();
		}
		if (sSCStaffrollIsPaused == TRUE)
		{
			is_paused = scStaffrollGetPauseStatusResume();
		}
		sSCStaffrollIsPaused = is_paused;

		if (button_tap & START_BUTTON)
		{
			if (sSCStaffrollRollSpeed == 0.0037500001F)
			{
				sSCStaffrollRollSpeed = 0.049999997F;
			}
			else sSCStaffrollRollSpeed = 0.0037500001F;
		}
	}
}

// 0x80133618
SCStaffrollName* SCStaffrollNameUpdateAlloc(GObj *gobj)
{
	SCStaffrollName *cn;

	if (sSCStaffrollNameAllocFree == NULL)
	{
		cn = syTaskmanMalloc(sizeof(SCStaffrollName), 0x4);
	}
	else
	{
		cn = sSCStaffrollNameAllocFree;
		sSCStaffrollNameAllocFree = cn->next;
	}
	cn->offset_x = cn->unkgmcreditsstruct0x10 = cn->interpolation = cn->status = 0;

	gobj->user_data.p = cn;

	return cn;
}

// 0x80133684
void SCStaffrollNameSetPrevAlloc(SCStaffrollName *cn)
{
	cn->next = sSCStaffrollNameAllocFree;
	sSCStaffrollNameAllocFree = cn;
}

// 0x8013369C
void scStaffrollJobAndNameThreadUpdate(GObj *gobj)
{
	SCStaffrollName *cn;
	Vec3f pos;
	DObj *dobj;
	s32 unused[3];

	cn = gobj->user_data.p;
	dobj = gobj->obj;

	cn->interpolation = 0.0F;

	gobj->flags = GOBJ_FLAG_HIDDEN;

	while (sSCStaffrollStatus != 0)
	{
		gcSleepCurrentGObjThread(1);
	}
	gobj->flags = GOBJ_FLAG_NONE;

	while (cn->interpolation != 1.0F)
	{
		gcAddDObjAnimJoint(dobj, sSCStaffrollNameAnimJoint, cn->interpolation * 99.0F);

		syInterpCubic(&pos, sSCStaffrollNameInterpolation, cn->interpolation);

		dobj->translate.vec.f.x = pos.x + cn->offset_x;
		dobj->translate.vec.f.y = pos.y + 12.0F;
		dobj->translate.vec.f.z = pos.z;

		cn->interpolation += sSCStaffrollRollSpeed;

		if (cn->interpolation > 1.0F)
		{
			cn->interpolation = 1.0F;
		}
		gcPlayAnimAll(gobj);
		gcSleepCurrentGObjThread(1);
	}
	if (cn->status == -1)
	{
		sSCStaffrollStatus = -1;
	}
	SCStaffrollNameSetPrevAlloc(cn);
	gcEjectGObj(NULL);
	gcSleepCurrentGObjThread(1);
}

// 0x80133854
void scStaffrollJobProcDisplay(GObj *gobj)
{
	if (gobj == gGCCommonLinks[4])
	{
		gSPTexture(gSYTaskmanDLHeads[0]++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
		gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
		gSPClearGeometryMode(gSYTaskmanDLHeads[0]++, G_ZBUFFER);
		gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0x7F, 0x7F, 0x89, 0xFF);
		gDPSetCombineLERP(gSYTaskmanDLHeads[0]++, 0, 0, 0, PRIMITIVE, 0, 0, 0, TEXEL0, 0, 0, 0, PRIMITIVE, 0, 0, 0, TEXEL0);
	}
	gcDrawDObjTreeForGObj(gobj);
}

// 0x80133930
void scStaffrollNameProcDisplay(GObj *gobj)
{
	if (gobj == gGCCommonLinks[3])
	{
		gSPTexture(gSYTaskmanDLHeads[0]++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
		gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
		gSPClearGeometryMode(gSYTaskmanDLHeads[0]++, G_ZBUFFER);
		gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0x88, 0x93, 0xFF, 0xFF);
		gDPSetCombineLERP(gSYTaskmanDLHeads[0]++, 0, 0, 0, PRIMITIVE, 0, 0, 0, TEXEL0, 0, 0, 0, PRIMITIVE, 0, 0, 0, TEXEL0);
	}
	gcDrawDObjTreeForGObj(gobj);
}

// 0x80133A0C
void scStaffrollJobAndNameInitStruct(GObj *gobj, DObj *first_dobj, DObj *second_dobj, sb32 job_or_name)
{
	SCStaffrollName *cn = SCStaffrollNameUpdateAlloc(gobj);

	cn->offset_x = (first_dobj->translate.vec.f.x - second_dobj->translate.vec.f.x) * 0.5F;

	cn->unkgmcreditsstruct0x10 = 26.0F;

	cn->name_id = sSCStaffrollNameID;
	cn->job_or_name = job_or_name;
}

// 0x80133A78
SCStaffrollSetup* scStaffrollMakeJobDObjs(SCStaffrollSetup *name_setup, DObj *dobj, s32 name_id, f32 wbase)
{
	SCStaffrollSetup local_setup;
	DObj *new_dobj;
	f32 width;
	f32 height;
	s32 job_character_id;
	s32 character_id;
	s32 i;

	for
	(
		i = 0, character_id = dSCStaffrollJobTextInfo[name_id].character_start, job_character_id = -1;
		i < dSCStaffrollJobTextInfo[name_id].character_count;
		job_character_id = character_id, i++, character_id++
	)
	{
		if (dSCStaffrollJobCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX(' '))
		{
			wbase += 16.0F;

			continue;
		}
		else
		{
			Gfx *dl = sSCStaffrollNameAndJobDisplayLists[dSCStaffrollJobCharacters[character_id]];

			width = dSCStaffrollNameAndJobSpriteInfo[dSCStaffrollJobCharacters[character_id]].width;
			height = dSCStaffrollNameAndJobSpriteInfo[dSCStaffrollJobCharacters[character_id]].height;

			new_dobj = gcAddChildForDObj(dobj, dl);

			gcAddXObjForDObjFixed(new_dobj, nGCMatrixKindTra, 1);

			if (job_character_id != -1)
			{
				if
				(
					(
						(dSCStaffrollJobCharacters[job_character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('K')) ||
						(dSCStaffrollJobCharacters[job_character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('T')) ||
						(dSCStaffrollJobCharacters[job_character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('V')) ||
						(dSCStaffrollJobCharacters[job_character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('W')) ||
						(dSCStaffrollJobCharacters[job_character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('Y'))
					)
					&&
					(
						(dSCStaffrollJobCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('a')) ||
						(dSCStaffrollJobCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('c')) ||
						(dSCStaffrollJobCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('e')) ||
						(dSCStaffrollJobCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('g')) ||
						(dSCStaffrollJobCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('m')) ||
						(dSCStaffrollJobCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('n')) ||
						(dSCStaffrollJobCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('o')) ||
						(dSCStaffrollJobCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('p')) ||
						(dSCStaffrollJobCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('q')) ||
						(dSCStaffrollJobCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('r')) ||
						(dSCStaffrollJobCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('s')) ||
						(dSCStaffrollJobCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('u')) ||
						(dSCStaffrollJobCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('v')) ||
						(dSCStaffrollJobCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('w')) ||
						(dSCStaffrollJobCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('x')) ||
						(dSCStaffrollJobCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('y')) ||
						(dSCStaffrollJobCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('z'))
					)
				)
				{
					wbase -= 6.0F;
				}
				else if
				(
					(
						(dSCStaffrollJobCharacters[job_character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('k')) ||
						(dSCStaffrollJobCharacters[job_character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('r')) ||
						(dSCStaffrollJobCharacters[job_character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('y'))
					)
					&&
					(
						(dSCStaffrollJobCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('a')) ||
						(dSCStaffrollJobCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('e')) ||
						(dSCStaffrollJobCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('o'))
					)
				)
				{
					wbase -= 6.0F;
				}
				else if
				(
					(dSCStaffrollJobCharacters[job_character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('o')) &&
				    (dSCStaffrollJobCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('s'))
				)
				{
					wbase -= 4.0F;
				}
				else if
				(
					(dSCStaffrollJobCharacters[job_character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('S')) &&
					(dSCStaffrollJobCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('u'))
				)
				{
					wbase -= 4.0F;
				}
			}
			new_dobj->translate.vec.f.x = wbase + width;
			new_dobj->translate.vec.f.y = height - 22.0F;

			wbase = new_dobj->translate.vec.f.x + width;

			if (dSCStaffrollJobCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('z'))
			{
				new_dobj->translate.vec.f.y += 1.0F;
			}
			if (dSCStaffrollJobCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('j'))
			{
				new_dobj->translate.vec.f.y = 22.0F - height;
			}
			if (dSCStaffrollJobCharacters[character_id] == GMSTAFFROLL_ASCII_NUMBER_TO_PARA_FONT_INDEX('8'))
			{
				new_dobj->translate.vec.f.y += 22.0F;
			}
			if
			(
				(dSCStaffrollJobCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('g')) ||
				(dSCStaffrollJobCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('p')) ||
				(dSCStaffrollJobCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('q')) ||
				(dSCStaffrollJobCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('y'))
			)
			{
				new_dobj->translate.vec.f.y = -8.0F;
			}
		}
	}
	local_setup.spacing = wbase;
	local_setup.dobj = new_dobj;

	*name_setup = local_setup;

	return name_setup;
}

// 0x80133E68
GObj* scStaffrollMakeJobGObj(SCStaffrollJob *job)
{
	SCStaffrollSetup job_setup;
	s32 unused;
	GObj *gobj;
	DObj *dobj;
	f32 wbase;

	wbase = 0.0F;

	gobj = gcMakeGObjSPAfter(1, NULL, 4, GOBJ_PRIORITY_DEFAULT);

	gcAddGObjDisplay(gobj, scStaffrollJobProcDisplay, 2, GOBJ_PRIORITY_DEFAULT, ~0);

	dobj = gcAddDObjForGObj(gobj, NULL);

	gcAddXObjForDObjFixed(dobj, nGCMatrixKindTraRotRpyRSca, 0);

	if (job->prefix_id != -1)
	{
		scStaffrollMakeJobDObjs(&job_setup, dobj, job->prefix_id, 0.0F);
		wbase = 16.0F + job_setup.spacing;
	}
	scStaffrollMakeJobDObjs(&job_setup, dobj, job->job_id, wbase);
	scStaffrollJobAndNameInitStruct(gobj, dobj, job_setup.dobj, 0);
	gcAddGObjProcess(gobj, scStaffrollJobAndNameThreadUpdate, nGCProcessKindThread, 1);

	return gobj;
}

// 0x80133F68
GObj* scStaffrollMakeNameGObjAndDObjs(void)
{
	s32 unused;
	GObj *gobj;
	Gfx *dl;
	DObj *dobj;
	DObj *new_dobj;
	f32 width;
	f32 wbase;
	f32 height;
	s32 name_character_id;
	s32 character_id;
	s32 i;

	name_character_id = -1;

	gobj = gcMakeGObjSPAfter(1, NULL, 3, GOBJ_PRIORITY_DEFAULT);

	gcAddGObjDisplay(gobj, scStaffrollNameProcDisplay, 1, GOBJ_PRIORITY_DEFAULT, ~0);

	new_dobj = dobj = gcAddDObjForGObj(gobj, NULL);

	gcAddXObjForDObjFixed(dobj, nGCMatrixKindTraRotRpyRSca, 0);

	wbase = 0.0F;

	for
	(
		i = 0, character_id = dSCStaffrollNameTextInfo[sSCStaffrollNameID].character_start;
		i < dSCStaffrollNameTextInfo[sSCStaffrollNameID].character_count;
		name_character_id = character_id, i++, character_id++
	)
	{
		if (dSCStaffrollNameCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX(' '))
		{
			wbase += 16.0F;

			continue;
		}
		else
		{
			dl = sSCStaffrollNameAndJobDisplayLists[dSCStaffrollNameCharacters[character_id]];

			width = dSCStaffrollNameAndJobSpriteInfo[dSCStaffrollNameCharacters[character_id]].width;
			height = dSCStaffrollNameAndJobSpriteInfo[dSCStaffrollNameCharacters[character_id]].height;

			new_dobj = gcAddChildForDObj(dobj, dl);

			gcAddXObjForDObjFixed(new_dobj, nGCMatrixKindTra, 1);

			if (name_character_id != -1)
			{
				if
				(
					(
						(dSCStaffrollNameCharacters[name_character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('K')) ||
						(dSCStaffrollNameCharacters[name_character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('T')) ||
						(dSCStaffrollNameCharacters[name_character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('V')) ||
						(dSCStaffrollNameCharacters[name_character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('W')) ||
						(dSCStaffrollNameCharacters[name_character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('Y'))
					)
					&&
					(
						(dSCStaffrollNameCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('a')) ||
						(dSCStaffrollNameCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('c')) ||
						(dSCStaffrollNameCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('e')) ||
						(dSCStaffrollNameCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('g')) ||
						(dSCStaffrollNameCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('m')) ||
						(dSCStaffrollNameCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('n')) ||
						(dSCStaffrollNameCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('o')) ||
						(dSCStaffrollNameCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('p')) ||
						(dSCStaffrollNameCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('q')) ||
						(dSCStaffrollNameCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('r')) ||
						(dSCStaffrollNameCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('s')) ||
						(dSCStaffrollNameCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('u')) ||
						(dSCStaffrollNameCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('v')) ||
						(dSCStaffrollNameCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('w')) ||
						(dSCStaffrollNameCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('x')) ||
						(dSCStaffrollNameCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('y')) ||
						(dSCStaffrollNameCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('z'))
					)
				)
				{
					wbase -= 6.0F;
				}
				else if
				(
					(
						(dSCStaffrollNameCharacters[name_character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('k')) ||
						(dSCStaffrollNameCharacters[name_character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('r')) ||
						(dSCStaffrollNameCharacters[name_character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('y'))
					)
					&&
					(
						(dSCStaffrollNameCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('a')) ||
						(dSCStaffrollNameCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('e')) ||
						(dSCStaffrollNameCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('o'))
					)
				)
				{
					wbase -= 6.0F;
				}
				else if 
				(
					(dSCStaffrollNameCharacters[name_character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('o')) &&
					(dSCStaffrollNameCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('s'))
				)
				{
					wbase -= 4.0F;
				}
				else if
				(
					(dSCStaffrollNameCharacters[name_character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('S')) &&
					(dSCStaffrollNameCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('u'))
				)
				{
					wbase -= 4.0F;
				}
			}
			new_dobj->translate.vec.f.x = wbase + width;
			new_dobj->translate.vec.f.y = height - 22.0F;

			wbase = new_dobj->translate.vec.f.x + width;

			if (dSCStaffrollNameCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('z'))
			{
				new_dobj->translate.vec.f.y += 1.0F;
			}
			if (dSCStaffrollNameCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('j'))
			{
				new_dobj->translate.vec.f.y = 22.0F - height;
			}
			if
			(
				(dSCStaffrollNameCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('g')) ||
				(dSCStaffrollNameCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('p')) ||
				(dSCStaffrollNameCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('q')) ||
				(dSCStaffrollNameCharacters[character_id] == GMSTAFFROLL_ASCII_LETTER_TO_FONT_INDEX('y'))
			)
			{
				new_dobj->translate.vec.f.y = -8.0F;
			}
			if (dSCStaffrollNameCharacters[character_id] == GMSTAFFROLL_ASCII_NUMBER_TO_PARA_FONT_INDEX('9'))
			{
				new_dobj->translate.vec.f.y -= 4.0F;
			}
		}

	}
	scStaffrollJobAndNameInitStruct(gobj, dobj, new_dobj, 1);
	gcAddGObjProcess(gobj, scStaffrollJobAndNameThreadUpdate, 0, 1);

	return gobj;
}

// 0x801343AC
void scStaffrollCrosshairThreadUpdate(GObj *gobj)
{
	SObj *sobj = SObjGetStruct(gobj);
	s32 crosshair_center_wait = 19;

	sobj->pos.x = 291.0F;
	sobj->pos.y = 0.0F;

	do
	{
		sobj->pos.y += 10.5F;

		gcSleepCurrentGObjThread(1);
	} 
	while (crosshair_center_wait--);

	sSCStaffrollStatus = 1;

	while (TRUE)
	{
		s32 stick_x = gSYControllerDevices[sSCStaffrollPlayer].stick_range.x;
		s32 stick_y = gSYControllerDevices[sSCStaffrollPlayer].stick_range.y;

		f32 base_x = sobj->pos.x;
		f32 base_y = sobj->pos.y;

		sobj->pos.x += (ABS(stick_x) > 16) ? stick_x * 0.125F : 0.0F;
		sobj->pos.y -= (ABS(stick_y) > 16) ? stick_y * 0.125F : 0.0F;

		sobj->pos.x = (sobj->pos.x < 32.0F) ? 32.0F : (sobj->pos.x > 540.0F) ? 540.0F : sobj->pos.x;
		sobj->pos.y = (sobj->pos.y < 36.0F) ? 36.0F : (sobj->pos.y > 400.0F) ? 400.0F : sobj->pos.y;

		sSCStaffrollCrosshairPositionX = sobj->pos.x - base_x;
		sSCStaffrollCrosshairPositionY = sobj->pos.y - base_y;

		gcSleepCurrentGObjThread(1);
	}
}

// 0x801345FC
void scStaffrollMakeCrosshairGObj(void)
{
	GObj *gobj;
	SObj *sobj;

	gobj = gcMakeGObjSPAfter(3, NULL, 6, GOBJ_PRIORITY_DEFAULT);
	gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 4, GOBJ_PRIORITY_DEFAULT, ~0);
	gcAddGObjProcess(gobj, scStaffrollCrosshairThreadUpdate, nGCProcessKindThread, 1);

	sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sSCStaffrollFiles[0], &llSCStaffrollCrosshairSprite));

	sSCStaffrollCrosshairGObj = gobj;

	sobj->sprite.attr = SP_TRANSPARENT;

	sobj->sprite.red   = 0xFF;
	sobj->sprite.green = 0x00;
	sobj->sprite.blue  = 0x00;

	sobj->sprite.scalex = 2.0F;
	sobj->sprite.scaley = 2.0F;
}

// 0x801346B4
void scStaffrollMakeTextBoxBracketSObjs(void)
{
	GObj *gobj;
	SObj *left_sobj;
	SObj *right_sobj;

	gobj = gcMakeGObjSPAfter(3, NULL, 8, GOBJ_PRIORITY_DEFAULT);

	gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 7, GOBJ_PRIORITY_DEFAULT, ~0);

	left_sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sSCStaffrollFiles[0], &llSCStaffrollTextBoxBracketLeftSprite));

	gobj = gcMakeGObjSPAfter(3, NULL, 8, GOBJ_PRIORITY_DEFAULT);

	gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 7, GOBJ_PRIORITY_DEFAULT, ~0);

	right_sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sSCStaffrollFiles[0], &llSCStaffrollTextBoxBracketRightSprite));

	left_sobj->sprite.attr = right_sobj->sprite.attr = SP_TRANSPARENT;

	left_sobj->sprite.scalex = right_sobj->sprite.scalex = 2.0F;
	left_sobj->sprite.scaley = right_sobj->sprite.scaley = 2.4F;

	left_sobj->sprite.red   = right_sobj->sprite.red   = 0x78;
	left_sobj->sprite.green = right_sobj->sprite.green = 0x6E;
	left_sobj->sprite.blue  = right_sobj->sprite.blue  = 0x40;

	left_sobj->pos.y = right_sobj->pos.y = 30.0F;

	left_sobj->pos.x = 328.0F;
	right_sobj->pos.x = 588.0F;
}

// 0x801347F4
void scStaffrollMakeTextBoxGObj(void)
{
	GObj *gobj = gcMakeGObjSPAfter(4, NULL, 7, GOBJ_PRIORITY_DEFAULT);

	gcAddGObjDisplay(gobj, gcDrawDObjTreeForGObj, 9, GOBJ_PRIORITY_DEFAULT, ~0);
	gcAddDObjForGObj(gobj, dSCStaffrollTextBoxDisplayList);
}

// 0x80134854
void scStaffrollScrollThreadUpdate(GObj *gobj)
{
	GObj *name_gobj;
	SCStaffrollJob *job;
	SCStaffrollName *name;
	sb32 is_queued_name;    // Whether next block of rolling text is job or name
	f32 interpolation;

	is_queued_name = TRUE;
	job = dSCStaffrollJobDescriptions;
	name = scStaffrollMakeJobGObj(job)->user_data.p;

	while (sSCStaffrollNameID < ARRAY_COUNT(dSCStaffrollStaffRoleTextInfo))
	{
		interpolation = (is_queued_name != FALSE) ? 0.15F : 0.3F;

		if (name->interpolation > interpolation)
		{
			if (is_queued_name != FALSE)
			{
				name_gobj = scStaffrollMakeNameGObjAndDObjs();

				name = name_gobj->user_data.p;

				if (++sSCStaffrollNameID == job->staff_count)
				{
					is_queued_name = FALSE;
				}
			}
			else
			{
				job++;

				name = scStaffrollMakeJobGObj(job)->user_data.p;

				is_queued_name = TRUE;
			}
		}
		gcSleepCurrentGObjThread(1);
	}
	name = name_gobj->user_data.p;
	name->status = -1;

	sSCStaffrollScrollGObj = NULL;

	gcEjectGObj(NULL);
	gcSleepCurrentGObjThread(1);
}

// 0x8013498C
void scStaffrollMakeScrollGObj(void)
{
	GObj *gobj = gcMakeGObjSPAfter(0, NULL, 1, GOBJ_PRIORITY_DEFAULT);

	gcAddGObjProcess(gobj, scStaffrollScrollThreadUpdate, nGCProcessKindThread, 1);

	sSCStaffrollScrollGObj = gobj;
}

// 0x801349DC
void scStaffrollSetupFiles(void)
{
	LBRelocSetup rl_setup;

	rl_setup.table_addr = (uintptr_t)&lLBRelocTableAddr;
	rl_setup.table_files_num = (u32)&llRelocFileCount;
	rl_setup.file_heap = NULL;
	rl_setup.file_heap_size = 0;
	rl_setup.status_buffer = sSCStaffrollStatusBuffer;
	rl_setup.status_buffer_size = ARRAY_COUNT(sSCStaffrollStatusBuffer);
	rl_setup.force_status_buffer = NULL;
	rl_setup.force_status_buffer_size = 0;

	lbRelocInitSetup(&rl_setup);
	lbRelocLoadFilesListed(dSCStaffrollFileIDs, sSCStaffrollFiles);
}

// 0x80134A70
void scStaffrollInitNameAndJobDisplayLists(void)
{
	Gfx *dl;
	Vtx *vtx_base, *vtx_curr;
	s32 i, j;

	for (i = 0; i < (ARRAY_COUNT(sSCStaffrollNameAndJobDisplayLists) + ARRAY_COUNT(dSCStaffrollNameAndJobSpriteInfo)) / 2; i++)
	{
		vtx_base = vtx_curr = syTaskmanMalloc(sizeof(Vtx) * 4, 0x8);

		for (j = 0; j < 4; j++, vtx_curr++)
		{
			vtx_curr->v.ob[0] = (j & 2) ? -dSCStaffrollNameAndJobSpriteInfo[i].width : dSCStaffrollNameAndJobSpriteInfo[i].width;
			vtx_curr->v.ob[1] = (j == 0) ? dSCStaffrollNameAndJobSpriteInfo[i].height : (j < 3) ? -dSCStaffrollNameAndJobSpriteInfo[i].height : dSCStaffrollNameAndJobSpriteInfo[i].height;
			vtx_curr->v.ob[2] = 0;

			vtx_curr->v.flag = 0;

			vtx_curr->v.tc[0] = (j & 2) ? 0 : dSCStaffrollNameAndJobSpriteInfo[i].width * 32;
			vtx_curr->v.tc[1] = (j == 0) ? 0 : (j < 3) ? dSCStaffrollNameAndJobSpriteInfo[i].height * 32 : 0;

			vtx_curr->v.cn[0] = 0x00;
			vtx_curr->v.cn[1] = 0x00;
			vtx_curr->v.cn[2] = 0x7F;
			vtx_curr->v.cn[3] = 0x00;
		}
		sSCStaffrollNameAndJobDisplayLists[i] = dl = syTaskmanMalloc(sizeof(Gfx) * 12, 0x8);

		gDPPipeSync(dl++);
		gDPLoadTextureBlock_4b
		(
			dl++, // pkt
			lbRelocGetFileData(Sprite*, sSCStaffrollFiles[0], dSCStaffrollNameAndJobSpriteInfo[i].offset), // timg
			G_IM_FMT_I, // fmt
			((dSCStaffrollNameAndJobSpriteInfo[i].width + 15) / 16) * 16, // width
			dSCStaffrollNameAndJobSpriteInfo[i].height, // height
			0, // pal
			G_TX_NOMIRROR | G_TX_CLAMP, // cms
			G_TX_NOMIRROR | G_TX_CLAMP, // cmt
			5, // masks
			5, // maskt
			G_TX_NOLOD, // shifts
			G_TX_NOLOD // shiftt
		);
		gDPPipeSync(dl++);
		gSPVertex(dl++, vtx_base, 4, 0);
		gSP2Triangles(dl++, 3, 2, 1, 0, 0, 3, 1, 0);
		gSPEndDisplayList(dl++);
	}
}

// 0x80134E08
void scStaffrollInitVars(void)
{
	sSCStaffrollStatus = 2;
	sSCStaffrollNameID = 0;
	sSCStaffrollRollSpeed = 0.0037500001F;
	sSCStaffrollNameAllocFree = NULL;
	sSCStaffrollIsPaused = FALSE;
	sSCStaffrollNameInterpolation = lbRelocGetFileData(SYInterpDesc*, sSCStaffrollFiles[0], &llSCStaffrollInterpolation);
	sSCStaffrollNameAnimJoint = lbRelocGetFileData(AObjEvent32*, sSCStaffrollFiles[0], &llSCStaffrollAnimJoint);
	sSCStaffrollDObjDesc = lbRelocGetFileData(DObjDesc*, sSCStaffrollFiles[0], &llSCStaffrollDObjDesc);
	sSCStaffrollRollBeginWait = 0;
	sSCStaffrollPlayer = gSCManagerSceneData.player;
	sSCStaffrollRollEndWait = 60;
}

// 0x80134EA8
void scStaffrollUpdateCameraAt(GObj *gobj)
{
	CObj *cobj = CObjGetStruct(gobj);

	cobj->vec.at.x += (sSCStaffrollCrosshairPositionX * 0.25F);
	cobj->vec.at.y -= (sSCStaffrollCrosshairPositionY * 0.25F);
}

// 0x80134EE8
void scStaffrollMakeCamera(void)
{
	CObj *cobj = CObjGetStruct
	(
		gcMakeCameraGObj
		(
			5,
			NULL,
			12,
			GOBJ_PRIORITY_DEFAULT,
			lbCommonDrawSprite,
			30,
			COBJ_MASK_DLLINK(7) | COBJ_MASK_DLLINK(6) | 
			COBJ_MASK_DLLINK(5) | COBJ_MASK_DLLINK(4),
			-1,
			0,
			1,
			NULL,
			1,
			0
		)
	);
	syRdpSetViewport(&cobj->viewport, 20.0F, 20.0F, 620.0F, 460.0F);

	sSCStaffrollCamera = cobj = CObjGetStruct
	(
		gcMakeCameraGObj
		(
			5,
			NULL,
			12,
			GOBJ_PRIORITY_DEFAULT,
			func_80017EC0,
			50,
			COBJ_MASK_DLLINK(9) | COBJ_MASK_DLLINK(8) | 
			COBJ_MASK_DLLINK(3) | COBJ_MASK_DLLINK(2) |
			COBJ_MASK_DLLINK(1),
			-1,
			1,
			1,
			scStaffrollUpdateCameraAt,
			1,
			0
		)
	);
	syRdpSetViewport(&cobj->viewport, 20.0F, 20.0F, 620.0F, 460.0F);

	cobj->vec.eye.y = cobj->vec.at.x = cobj->vec.at.y = cobj->vec.at.z = 0.0F;

	cobj->vec.eye.x = 0.0F;
	cobj->vec.eye.z = 580.0F;

	cobj->projection.persp.fovy = 50.0F;
}

// 0x8013505C
void scStaffrollFuncStart(void)
{
	gcMakeGObjSPAfter(0, scStaffrollFuncRun, 1, GOBJ_PRIORITY_DEFAULT);
	gcMakeDefaultCameraGObj(12, GOBJ_PRIORITY_DEFAULT, 100, COBJ_FLAG_FILLCOLOR, GPACK_RGBA8888(0x00, 0x00, 0x00, 0xFF));

	scStaffrollSetupFiles();
	scStaffrollInitNameAndJobDisplayLists();
	scStaffrollTryHideUnlocks();
	scStaffrollInitVars();
	scStaffrollMakeCrosshairGObj();
	scStaffrollMakeScrollGObj();
	scStaffrollMakeCamera();

	syAudioStopBGMAll();
	syAudioPlayBGM(0, nSYAudioBGMStaffroll);
}

// 0x8013A6C8
Lights1 dSCStaffrollLights1 = gdSPDefLights1(0x20, 0x20, 0x20, 0xFF, 0xFF, 0xFF, 0x0A, 0x32, 0x32);

// 0x8013A6E0
Gfx dSCStaffrollDisplayList[/* */] =
{
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPSetLights1(dSCStaffrollLights1),
	gsSPEndDisplayList()
};

// 0x801350F4
void scStaffrollFuncLights(Gfx **dls)
{
	gSPDisplayList(dls[0]++, dSCStaffrollDisplayList);
}

// 0x80135118
void scStaffrollFuncDraw(void)
{
	gcDrawAll();

	if (sSCStaffrollRollEndWait != 0)
	{
		if ((sSCStaffrollStatus == -1) || (sSCStaffrollStatus == -2))
		{
			sSCStaffrollRollEndWait--;
		}
	}
	if (sSCStaffrollRollEndWait == 0)
	{
		syTaskmanSetLoadScene();
	}
	if (sSCStaffrollStatus == -1)
	{
#if defined(REGION_US)        
    	gSCManagerSceneData.scene_curr = nSCKindStartup;
#else
    	gSCManagerSceneData.scene_curr = nSCKindOpeningRoom;
#endif

		syAudioStopBGMAll();
		syVideoSetFlags(SYVIDEO_FLAG_BLACKOUT);

		sSCStaffrollStatus = -2;
	}
}

// 0x8013A708
SYVideoSetup dSCStaffrollVideoSetup =
{
	SYVIDEO_DEFINE_FRAMEBUFFER_ADDR(640, 480, 0, 0, u16, 0),
	SYVIDEO_DEFINE_FRAMEBUFFER_ADDR(640, 480, 0, 0, u16, 1),
	SYVIDEO_DEFINE_FRAMEBUFFER_ADDR(640, 480, 0, 0, u16, 2),
	NULL,
	640,
	480,
	SYVIDEO_FLAG_DIVOT        | SYVIDEO_FLAG_DITHERFILTER | SYVIDEO_FLAG_GAMMADITHER | 
	0x400                     | SYVIDEO_FLAG_NOBLACKOUT   | SYVIDEO_FLAG_NOGAMMA     |
    SYVIDEO_FLAG_COLORDEPTH16 | SYVIDEO_FLAG_SERRATE      | 0x2
};

// 0x8013A724
SYTaskmanSetup dSCStaffrollTaskmanSetup =
{
    // Task Manager Buffer Setup
    {
        0,                          // ???
        gcRunAll,              		// Update function
        scStaffrollFuncDraw,        // Frame draw function
        &ovl59_BSS_END,             // Allocatable memory pool start
        0,                          // Allocatable memory pool size
        1,                          // ???
        2,                          // Number of contexts?
        sizeof(Gfx) * 2048,         // Display List Buffer 0 Size
        sizeof(Gfx) * 128,          // Display List Buffer 1 Size
        0,                          // Display List Buffer 2 Size
        0,                          // Display List Buffer 3 Size
        0x1000,                     // Graphics Heap Size
        2,                          // ???
        0x1000,                     // RDP Output Buffer Size
        scStaffrollFuncLights,     	// Pre-render function
        syControllerFuncRead,       // Controller I/O function
    },

    16,                             // Number of GObjThreads
    sizeof(u64) * 192,              // Thread stack size
    16,                             // Number of thread stacks
    0,                              // ???
    64,                             // Number of GObjProcesses
    64,                             // Number of GObjs
    sizeof(GObj),                   // GObj size
    256,                            // Number of XObjs
    NULL,                           // Matrix function list
    NULL,                           // DObjVec eject function
    32,                             // Number of AObjs
    16,                             // Number of MObjs
    1024,                           // Number of DObjs
    sizeof(DObj),                   // DObj size
    256,                            // Number of SObjs
    sizeof(SObj),                   // SObj size
    8,                              // Number of CObjs
    sizeof(CObj),                 	// CObj size
    
    scStaffrollFuncStart            // Task start function
};

// 0x801351B8
void scStaffrollStartScene(void)
{
	/* 
	 * This is really weird. The function will only match if fb32 is assigned a hardcoded constant value.
	 * One would assume they did this for the default 320x230 framebuffers as well, but that is not the case.
	 * The default framebuffers effectively start at ovl1_BSS_END, at 0x80392A00, which is 0x1900 bytes behind
	 * where it would start if they had calculated the address with SYVIDEO_DEFINE_FRAMEBUFFER_ADDR.
	 * Does this and the fact that the default framebuffer start only match when used with its extern variable
	 * suggest that they truly start at ovl1_BSS_END and have a definitive location in VRAM, rather than
	 * being only a concept as seen here with the custom 640x480 framebuffer?
	 */

	u16 *fb16;
	u32 *fb32;

	fb32 = (u32*)SYVIDEO_DEFINE_FRAMEBUFFER_ADDR(640, 480, 0, 0, u16, 0);

	while ((uintptr_t)fb32 < 0x80400000) { *fb32++ = 0x00000000; }

	dSCStaffrollVideoSetup.zbuffer = SYVIDEO_ZBUFFER_START(640, 480, 0, 10, u16);
	syVideoInit(&dSCStaffrollVideoSetup);

	dSCStaffrollTaskmanSetup.scene_setup.arena_size = (size_t) ((uintptr_t)SYVIDEO_DEFINE_FRAMEBUFFER_ADDR(640, 480, 0, 0, u16, 0) - (uintptr_t)&ovl59_BSS_END);
	syTaskmanStartTask(&dSCStaffrollTaskmanSetup);

	fb16 = (u16*) gSYFramebufferSets;

	while ((uintptr_t)fb16 < 0x80400000) { *fb16++ = GPACK_RGBA5551(0x00, 0x00, 0x00, 0x01); }
}
