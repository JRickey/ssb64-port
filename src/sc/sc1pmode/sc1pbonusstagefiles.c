#include <sc/scene.h>
#include <reloc_data.h>

// 0x8018F3B0
LBFileNode sSC1PBonusStageStatusBuffer[100];

// 0x8018F6D0
LBFileNode sSC1PBonusStageForceStatusBuffer[7];

// 0x8018ED70
void sc1PBonusStageSetupFiles(void)
{
	LBRelocSetup rl_setup;

	rl_setup.table_addr = (uintptr_t)&lLBRelocTableAddr;
	rl_setup.table_files_num = (u32)&llRelocFileCount;
	rl_setup.file_heap = NULL;
	rl_setup.file_heap_size = 0;
	rl_setup.status_buffer = sSC1PBonusStageStatusBuffer;
	rl_setup.status_buffer_size = ARRAY_COUNT(sSC1PBonusStageStatusBuffer);
	rl_setup.force_status_buffer = sSC1PBonusStageForceStatusBuffer;
	rl_setup.force_status_buffer_size = ARRAY_COUNT(sSC1PBonusStageForceStatusBuffer);

	lbRelocInitSetup(&rl_setup);
	lbRelocLoadFilesListed(dGMCommonFileIDs, gGMCommonFiles);
}
