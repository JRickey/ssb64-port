/*
 * reloc_extract — Standalone SSB64:RELOC file extractor.
 *
 * Reads baserom.us.z64 directly, locates a RELOC file by file_id using the
 * exact same logic as Torch's RelocFactory::parse, decompresses VPK0 if
 * needed, and writes the raw decompressed bytes to a file.
 *
 * Usage:
 *   reloc_extract <baserom.z64> <file_id> <output.bin>
 *
 * The output bytes are in N64 BIG-ENDIAN order — they are the same bytes that
 * Torch stores in the .o2r archive's mDecompressedData blob, BEFORE any
 * pass1 BSWAP32 or struct fixup is applied.  Compare this against the same
 * file extracted from the .o2r side to verify Torch's extraction is lossless.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../../torch/lib/libvpk0/vpk0.h"

/* Mirror RelocFactory.h constants */
#define RELOC_TABLE_ROM_ADDR    0x001AC870u
#define RELOC_FILE_COUNT        2132u
#define RELOC_TABLE_ENTRY_SIZE  12u
#define RELOC_TABLE_SIZE        ((RELOC_FILE_COUNT + 1u) * RELOC_TABLE_ENTRY_SIZE)
#define RELOC_DATA_START        (RELOC_TABLE_ROM_ADDR + RELOC_TABLE_SIZE)

static uint16_t read_be_u16(const uint8_t *p) {
    return (uint16_t)((p[0] << 8) | p[1]);
}

static uint32_t read_be_u32(const uint8_t *p) {
    return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) |
           ((uint32_t)p[2] << 8)  |  (uint32_t)p[3];
}

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "usage: %s <baserom.z64> <file_id> <output.bin>\n", argv[0]);
        return 2;
    }

    const char *rom_path = argv[1];
    uint32_t    file_id  = (uint32_t)strtoul(argv[2], NULL, 0);
    const char *out_path = argv[3];

    if (file_id >= RELOC_FILE_COUNT) {
        fprintf(stderr, "error: file_id %u out of range (max %u)\n",
                file_id, RELOC_FILE_COUNT - 1);
        return 1;
    }

    /* --- Load ROM --- */
    FILE *fp = fopen(rom_path, "rb");
    if (!fp) {
        fprintf(stderr, "error: cannot open ROM '%s'\n", rom_path);
        return 1;
    }
    fseek(fp, 0, SEEK_END);
    long sz = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    if (sz <= 0) {
        fprintf(stderr, "error: empty ROM\n");
        fclose(fp);
        return 1;
    }

    uint8_t *rom = (uint8_t *)malloc((size_t)sz);
    if (!rom) {
        fprintf(stderr, "error: oom\n");
        fclose(fp);
        return 1;
    }
    if (fread(rom, 1, (size_t)sz, fp) != (size_t)sz) {
        fprintf(stderr, "error: short read\n");
        free(rom);
        fclose(fp);
        return 1;
    }
    fclose(fp);

    /* z64 magic check */
    if (!(rom[0] == 0x80 && rom[1] == 0x37 && rom[2] == 0x12 && rom[3] == 0x40)) {
        fprintf(stderr, "error: not a z64 (big-endian) ROM (header: %02X %02X %02X %02X)\n",
                rom[0], rom[1], rom[2], rom[3]);
        free(rom);
        return 1;
    }

    /* --- Read this file's table entry and the next one --- */
    size_t table_off = RELOC_TABLE_ROM_ADDR + (size_t)file_id * RELOC_TABLE_ENTRY_SIZE;
    if (table_off + RELOC_TABLE_ENTRY_SIZE * 2 > (size_t)sz) {
        fprintf(stderr, "error: ROM too small for table entry %u\n", file_id);
        free(rom);
        return 1;
    }

    const uint8_t *te = rom + table_off;
    uint32_t first_word            = read_be_u32(te + 0);
    int      is_compressed         = (first_word >> 31) != 0;
    uint32_t data_offset           = first_word & 0x7FFFFFFFu;
    uint16_t reloc_intern          = read_be_u16(te + 4);
    uint16_t compressed_words      = read_be_u16(te + 6);
    uint16_t reloc_extern          = read_be_u16(te + 8);
    uint16_t decompressed_words    = read_be_u16(te + 10);

    uint32_t next_first_word       = read_be_u32(te + 12);
    uint32_t next_data_offset      = next_first_word & 0x7FFFFFFFu;

    uint32_t compressed_bytes      = (uint32_t)compressed_words   * 4u;
    uint32_t decompressed_bytes    = (uint32_t)decompressed_words * 4u;

    fprintf(stderr,
            "file_id=%u  compressed=%d  data_offset=0x%X  "
            "reloc_intern=0x%04X  reloc_extern=0x%04X  "
            "comp_bytes=%u  decomp_bytes=%u\n",
            file_id, is_compressed, data_offset,
            reloc_intern, reloc_extern,
            compressed_bytes, decompressed_bytes);

    /* --- Locate ROM data --- */
    size_t data_rom_addr = RELOC_DATA_START + data_offset;
    if (data_rom_addr + compressed_bytes > (size_t)sz) {
        fprintf(stderr, "error: ROM too small for file data\n");
        free(rom);
        return 1;
    }
    const uint8_t *file_data = rom + data_rom_addr;

    fprintf(stderr, "data_rom_addr=0x%zX (RELOC_DATA_START=0x%X + offset=0x%X)\n",
            data_rom_addr, (unsigned)RELOC_DATA_START, data_offset);

    /* --- Decompress / copy --- */
    uint8_t *out;
    uint32_t out_bytes;

    if (is_compressed) {
        out = (uint8_t *)malloc(decompressed_bytes);
        if (!out) {
            fprintf(stderr, "error: oom\n");
            free(rom);
            return 1;
        }
        uint32_t result = vpk0_decode(file_data, compressed_bytes,
                                      out, decompressed_bytes);
        if (result == 0) {
            fprintf(stderr, "error: VPK0 decode failed\n");
            free(out);
            free(rom);
            return 1;
        }
        out_bytes = decompressed_bytes;
    } else {
        out = (uint8_t *)malloc(decompressed_bytes);
        if (!out) {
            fprintf(stderr, "error: oom\n");
            free(rom);
            return 1;
        }
        memcpy(out, file_data, decompressed_bytes);
        out_bytes = decompressed_bytes;
    }

    /* --- Write output --- */
    FILE *of = fopen(out_path, "wb");
    if (!of) {
        fprintf(stderr, "error: cannot open output '%s'\n", out_path);
        free(out);
        free(rom);
        return 1;
    }
    if (fwrite(out, 1, out_bytes, of) != out_bytes) {
        fprintf(stderr, "error: short write\n");
        fclose(of);
        free(out);
        free(rom);
        return 1;
    }
    fclose(of);

    fprintf(stderr, "wrote %u bytes to %s\n", out_bytes, out_path);

    free(out);
    free(rom);
    return 0;
}
