#include <mn/menu.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x80134420
u32 dMNTitleFileIDs[/* */] = { &llMNTitleFileID, &llMNTitleFireAnimFileID };

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x801344A0
LBFileNode sMNTitleStatusBuffer[32];

// 0x801345A0
void *sMNTitleFiles[ARRAY_COUNT(dMNTitleFileIDs)];

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80134140
void mnTitleLoadFiles(void)
{
	LBRelocSetup rl_setup;

	rl_setup.table_addr = (uintptr_t)&lLBRelocTableAddr;
	rl_setup.table_files_num = (u32)&llRelocFileCount;
	rl_setup.file_heap = NULL;
	rl_setup.file_heap_size = 0;
	rl_setup.status_buffer = sMNTitleStatusBuffer;
	rl_setup.status_buffer_size = ARRAY_COUNT(sMNTitleStatusBuffer);
	rl_setup.force_status_buffer = NULL;
	rl_setup.force_status_buffer_size = 0;

	lbRelocInitSetup(&rl_setup);
	lbRelocLoadFilesListed(dMNTitleFileIDs, sMNTitleFiles);
}
