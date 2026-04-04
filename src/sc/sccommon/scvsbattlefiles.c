#include <sc/scene.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x8018E480
LBFileNode sSCVSBattleStatusBuffer[100];

// 0x8018E7A0
LBFileNode sSCVSBattleForceStatusBuffer[7];

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8018E330
void scVSBattleSetupFiles(void)
{
	LBRelocSetup rl_setup;

	rl_setup.table_addr = (uintptr_t)&lLBRelocTableAddr;
	rl_setup.table_files_num = (u32)&llRelocFileCount;
	rl_setup.file_heap = NULL;
	rl_setup.file_heap_size = 0;
	rl_setup.status_buffer = sSCVSBattleStatusBuffer;
	rl_setup.status_buffer_size = ARRAY_COUNT(sSCVSBattleStatusBuffer);
	rl_setup.force_status_buffer = sSCVSBattleForceStatusBuffer;
	rl_setup.force_status_buffer_size = ARRAY_COUNT(sSCVSBattleForceStatusBuffer);

	lbRelocInitSetup(&rl_setup);
	lbRelocLoadFilesListed(dGMCommonFileIDs, gGMCommonFiles);
}
