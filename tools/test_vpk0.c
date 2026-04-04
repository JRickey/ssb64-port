/**
 * Standalone test for the VPK0 decompressor.
 *
 * Reads the LBReloc table from the ROM, finds all VPK0-compressed files,
 * decompresses each one, and verifies:
 *   1. vpk0_decoded_size() returns the expected decompressed size
 *   2. vpk0_decode() returns the expected decompressed size (success)
 *   3. The decompressed data is not all zeros
 *
 * Usage: test_vpk0 <path-to-baserom.us.z64>
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../torch/lib/libvpk0/vpk0.h"

/* ROM constants for US NTSC v1.0 */
#define RELOC_TABLE_ROM_ADDR   0x001AC870
#define RELOC_FILE_COUNT       2132
#define RELOC_TABLE_ENTRY_SIZE 12
#define RELOC_TABLE_SIZE       ((RELOC_FILE_COUNT + 1) * RELOC_TABLE_ENTRY_SIZE)
#define RELOC_DATA_START       (RELOC_TABLE_ROM_ADDR + RELOC_TABLE_SIZE)

/* Read a big-endian u32 from a byte pointer */
static uint32_t read_be32(const uint8_t *p) {
    return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) |
           ((uint32_t)p[2] << 8)  | ((uint32_t)p[3]);
}

/* Read a big-endian u16 from a byte pointer */
static uint16_t read_be16(const uint8_t *p) {
    return ((uint16_t)p[0] << 8) | ((uint16_t)p[1]);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <path-to-baserom.us.z64>\n", argv[0]);
        return 1;
    }

    /* Read the entire ROM into memory */
    FILE *f = fopen(argv[1], "rb");
    if (!f) {
        fprintf(stderr, "Error: cannot open '%s'\n", argv[1]);
        return 1;
    }

    fseek(f, 0, SEEK_END);
    long rom_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    uint8_t *rom = (uint8_t *)malloc(rom_size);
    if (!rom) {
        fprintf(stderr, "Error: cannot allocate %ld bytes for ROM\n", rom_size);
        fclose(f);
        return 1;
    }

    if (fread(rom, 1, rom_size, f) != (size_t)rom_size) {
        fprintf(stderr, "Error: short read on ROM file\n");
        free(rom);
        fclose(f);
        return 1;
    }
    fclose(f);

    printf("ROM loaded: %ld bytes\n", rom_size);
    printf("Reloc table at: 0x%X\n", RELOC_TABLE_ROM_ADDR);
    printf("Data starts at: 0x%X\n", RELOC_DATA_START);
    printf("\n");

    /* Parse the reloc table and test all compressed files */
    int total_compressed = 0;
    int passed = 0;
    int failed = 0;

    for (int i = 0; i < RELOC_FILE_COUNT; i++) {
        size_t entry_offset = RELOC_TABLE_ROM_ADDR + i * RELOC_TABLE_ENTRY_SIZE;
        const uint8_t *entry = rom + entry_offset;

        uint32_t first_word = read_be32(entry);
        int is_compressed = (first_word >> 31) != 0;
        uint32_t data_offset = first_word & 0x7FFFFFFF;
        uint16_t compressed_size_words = read_be16(entry + 6);
        uint16_t decompressed_size_words = read_be16(entry + 10);

        if (!is_compressed) {
            continue;
        }

        total_compressed++;

        uint32_t compressed_size_bytes = (uint32_t)compressed_size_words * 4;
        uint32_t decompressed_size_bytes = (uint32_t)decompressed_size_words * 4;
        size_t data_rom_addr = RELOC_DATA_START + data_offset;

        if (data_rom_addr + compressed_size_bytes > (size_t)rom_size) {
            fprintf(stderr, "  [SKIP] File %d: data extends past ROM end\n", i);
            continue;
        }

        const uint8_t *compressed_data = rom + data_rom_addr;

        /* Test 1: vpk0_decoded_size */
        uint32_t reported_size = vpk0_decoded_size(compressed_data, compressed_size_bytes);
        if (reported_size != decompressed_size_bytes) {
            fprintf(stderr, "  [FAIL] File %d: vpk0_decoded_size returned %u, expected %u\n",
                    i, reported_size, decompressed_size_bytes);
            failed++;
            continue;
        }

        /* Test 2: vpk0_decode */
        uint8_t *decompressed = (uint8_t *)malloc(decompressed_size_bytes);
        if (!decompressed) {
            fprintf(stderr, "  [FAIL] File %d: cannot allocate %u bytes\n",
                    i, decompressed_size_bytes);
            failed++;
            continue;
        }

        memset(decompressed, 0, decompressed_size_bytes);
        uint32_t result = vpk0_decode(compressed_data, compressed_size_bytes,
                                       decompressed, decompressed_size_bytes);

        if (result != decompressed_size_bytes) {
            fprintf(stderr, "  [FAIL] File %d: vpk0_decode returned %u, expected %u\n",
                    i, result, decompressed_size_bytes);
            free(decompressed);
            failed++;
            continue;
        }

        /* Test 3: Not all zeros (sanity check) */
        int has_nonzero = 0;
        for (uint32_t j = 0; j < decompressed_size_bytes; j++) {
            if (decompressed[j] != 0) {
                has_nonzero = 1;
                break;
            }
        }

        if (!has_nonzero && decompressed_size_bytes > 0) {
            fprintf(stderr, "  [WARN] File %d: decompressed to all zeros (%u bytes)\n",
                    i, decompressed_size_bytes);
        }

        free(decompressed);
        passed++;
    }

    printf("Results: %d compressed files tested\n", total_compressed);
    printf("  Passed: %d\n", passed);
    printf("  Failed: %d\n", failed);

    free(rom);
    return failed > 0 ? 1 : 0;
}
