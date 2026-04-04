#include <ft/fttypes.h>
#include <sc/scsubsys/scsubsys.h>
#include <reloc_data.h>

s32 D_ovl1_803928E0[] =
{
#if defined(REGION_US)
	0xB110012C,
#else
	0xB10C012C,
#endif
	ftMotionCommandEnd()
};

FTMotionDesc dFTBossSubMotionDescs[] =
{
	&ll_2098_FileID, 0x80000000, 0x80000000,
	&ll_2098_FileID, 0x80000000, 0x80000000,
	&ll_2098_FileID, 0x80000000, 0x80000000,
	&ll_2098_FileID, 0x80000000, 0x80000000,
	&ll_2098_FileID, 0x80000000, 0x80000000,
	&ll_2098_FileID, 0x80000000, 0x80000000,
	&ll_2098_FileID, 0x80000000, 0x80000000,
	&ll_2098_FileID, 0x80000000, 0x80000000,
	&ll_2098_FileID, 0x80000000, 0x80000000,
	&ll_2098_FileID, 0x80000000, 0x80000000,
	&ll_2098_FileID, 0x80000000, 0x80000000,
	0x00000000, 0x80000000, 0x00000000,
	0x00000000, 0x80000000, 0x00000000,
	0x00000000, 0x80000000, 0x00000000,
	&ll_461_FileID, 0x80000000, 0x80000000,
	&ll_458_FileID, 0x80000000, 0x80000000,
	&ll_459_FileID, D_ovl1_803928E0, 0x80000000,
	&ll_460_FileID, 0x80000000, 0x80000000
};
s32 dFTBossSubMotionDescsCount = sizeof(dFTBossSubMotionDescs)/sizeof(FTMotionDesc); // 0x00000012
