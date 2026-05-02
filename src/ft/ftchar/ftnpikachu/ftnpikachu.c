#include <ft/fighter.h>

// 0x801310D0
void *gFTDataNPikachuMain;

// 0x801310D4
void *gFTDataNPikachuSubMotion;

// 0x801310D8
void *gFTDataNPikachuModel;

// 0x801310DC
// PORT: misleading decomp name. This slot is the Special2 file pointer
// (dFTNPikachuData::p_file_special2 points here). On N64 a 4-byte s32
// happened to coincide with a 4-byte void*; on LP64 the 8-byte
// `*p_file_special2 = lbRelocGetStatusBufferFile(...)` write overran
// into the next BSS slot (gFTDataNPikachuSubMotion's low half) and
// corrupted the polygon-Pikachu submotion file pointer — fingerprint
// `0x400000004` from `0x4_xxxxxxxx_xxxxxxxx` Special2 ptr clobbering
// SubMotion's low 4 bytes. Widen to a proper void*.
void *gFTDataNPikachuParticleBankID;

// 0x801310E0
s32 gFTDataNPikachuPad0x801310E0;
