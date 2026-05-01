#include "port_save.h"
#include "port_log.h"

#include <libultraship/libultraship.h>
#include <SDL2/SDL.h>

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <mutex>
#include <string>

// Where the SRAM file lives, in priority order:
//
//   1. $SSB64_SAVE_PATH                  — explicit override (debug / CI).
//   2. Ship::Context::GetPathRelativeToAppDirectory("ssb64_save.bin")
//      — same convention as BattleShip.o2r and BattleShip.cfg.json:
//         * NON_PORTABLE=ON                                 → OS app-data dir
//             macOS    ~/Library/Application Support/BattleShip/
//             Linux    $XDG_DATA_HOME/BattleShip/  (or ~/.local/share/...)
//             Windows  %APPDATA%\BattleShip\
//         * NON_PORTABLE=OFF                                → cwd (portable)
//         * SHIP_HOME=<dir>                                 → that dir wins on macOS/Linux
//   3. SDL_GetPrefPath fallback                              — only if Ship::Context
//                                                             hasn't been constructed yet
//                                                             (shouldn't happen — SRAM
//                                                             I/O runs after PortInit —
//                                                             but defensive).
//   4. "ssb64_save.bin" in cwd                               — last-resort.

namespace {

std::once_flag gPathOnce;
std::string gSavePath;

void resolveSavePath()
{
    if (const char *override = std::getenv("SSB64_SAVE_PATH")) {
        gSavePath = override;
        return;
    }
    try {
        // Mirrors how BattleShip.o2r / BattleShip.cfg.json / logs/*.log
        // are located. Honors SHIP_HOME and the NON_PORTABLE build flag.
        gSavePath = Ship::Context::GetPathRelativeToAppDirectory("ssb64_save.bin");
        if (!gSavePath.empty()) {
            return;
        }
    } catch (...) {
        // Ship::Context not yet alive — fall through.
    }
    if (char *p = SDL_GetPrefPath(NULL, "BattleShip")) {
        gSavePath = std::string(p) + "ssb64_save.bin";
        SDL_free(p);
        return;
    }
    gSavePath = "ssb64_save.bin"; // last-resort cwd
}

const std::string &savePath()
{
    std::call_once(gPathOnce, resolveSavePath);
    return gSavePath;
}

} // namespace

extern "C" const char *port_save_get_path(void)
{
    return savePath().c_str();
}

extern "C" int port_save_read(uintptr_t offset, void *dst, size_t size)
{
    if (size == 0) return 0;
    std::memset(dst, 0, size);

    FILE *f = std::fopen(savePath().c_str(), "rb");
    if (f == NULL) {
        return 0; // missing file -> zero-filled, treated as fresh SRAM
    }
    if (std::fseek(f, (long)offset, SEEK_SET) != 0) {
        std::fclose(f);
        return 0; // short file -> zero-filled
    }
    (void)std::fread(dst, 1, size, f);
    std::fclose(f);
    return 0;
}

extern "C" int port_save_write(uintptr_t offset, const void *src, size_t size)
{
    if (size == 0) return 0;

    const char *path = savePath().c_str();

    FILE *f = std::fopen(path, "r+b");
    if (f == NULL) {
        f = std::fopen(path, "w+b");
        if (f == NULL) {
            port_log("SSB64 Save: open(%s) for write failed\n", path);
            return -1;
        }
    }

    std::fseek(f, 0, SEEK_END);
    long cur = std::ftell(f);
    if (cur < (long)(offset + size)) {
        // pad with zeros up to offset
        if (cur < (long)offset) {
            static const char zeros[256] = {0};
            long need = (long)offset - cur;
            while (need > 0) {
                size_t chunk = need > (long)sizeof(zeros) ? sizeof(zeros) : (size_t)need;
                std::fwrite(zeros, 1, chunk, f);
                need -= chunk;
            }
        }
    }
    std::fseek(f, (long)offset, SEEK_SET);
    size_t wrote = std::fwrite(src, 1, size, f);
    std::fflush(f);
    std::fclose(f);

    if (wrote != size) {
        port_log("SSB64 Save: short write (%zu/%zu) at offset 0x%lx\n",
                 wrote, size, (unsigned long)offset);
        return -1;
    }
    return 0;
}
