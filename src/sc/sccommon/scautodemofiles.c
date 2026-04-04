#include <sc/scene.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x8018E500
LBFileNode sSCAutoDemoStatusBuffer[100];

// 0x8018E820
LBFileNode sSCAutoDemoForceStatusBuffer[7];

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8018E0C0
void scAutoDemoSetupFiles(void)
{
	LBRelocSetup rl_setup;

	rl_setup.table_addr = (uintptr_t)&lLBRelocTableAddr;
	rl_setup.table_files_num = (u32)&llRelocFileCount;
	rl_setup.file_heap = NULL;
	rl_setup.file_heap_size = 0;
	rl_setup.status_buffer = sSCAutoDemoStatusBuffer;
	rl_setup.status_buffer_size = ARRAY_COUNT(sSCAutoDemoStatusBuffer);
	rl_setup.force_status_buffer = sSCAutoDemoForceStatusBuffer;
	rl_setup.force_status_buffer_size = ARRAY_COUNT(sSCAutoDemoForceStatusBuffer);

	lbRelocInitSetup(&rl_setup);
	lbRelocLoadFilesListed(dGMCommonFileIDs, gGMCommonFiles);
}
