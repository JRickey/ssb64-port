#ifndef PORT_SAVE_H
#define PORT_SAVE_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// SRAM-backed save file. The N64 cart had 32 KiB of battery-backed SRAM at
// PI domain 2; the game uses two redundant LBBackupData slots inside it
// (offsets 0x000 and 0x5F0 — see lbBackupWrite). On PC we emulate that
// region as a flat binary file in the user's app-data directory.

#define PORT_SAVE_SIZE 0x8000

// Resolve the absolute path to the SRAM file (stable across calls).
const char *port_save_get_path(void);

// Read `size` bytes starting at `offset` into `dst`. Missing file or
// short reads zero-fill the remainder. Returns 0 on success, -1 on hard
// I/O error.
int port_save_read(uintptr_t offset, void *dst, size_t size);

// Write `size` bytes from `src` to `offset`. Creates the file if it
// does not exist, padding with zeros up to `offset+size` if needed.
// Returns 0 on success, -1 on I/O error.
int port_save_write(uintptr_t offset, const void *src, size_t size);

#ifdef __cplusplus
}
#endif

#endif
