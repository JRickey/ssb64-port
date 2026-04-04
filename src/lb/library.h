#ifndef _LIBRARY_H_
#define _LIBRARY_H_

#include "lbtypes.h"
#include "lbfunctions.h"

#define lbRelocGetFileData(type, file, offset) \
((type) ((uintptr_t)(file) + (intptr_t)(offset)))

extern intptr_t lSYKseg1ValidateFunc;    // 0x00000000
extern intptr_t lSYKseg1ValidateNBytes;  // 0x00000030

extern intptr_t lSYSignValidateFunc;    // 0x00000000
extern intptr_t lSYSignValidateNBytes;  // 0x00000040

#endif
