#pragma once

#define RELOC_FILE_COUNT 2132

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Maps file_id (0–2131) to the resource path in the .o2r archive.
 * Auto-generated from yamls/us/reloc_*.yml by tools/generate_reloc_table.py.
 */
extern const char* const gRelocFileTable[RELOC_FILE_COUNT];

#ifdef __cplusplus
}
#endif
