#include "HiResHook.h"

#include "HiResPack.h"
#include "../port_log.h"

#include <libultraship/libultraship.h>
#include <fast/interpreter.h>

#include <cstdint>

namespace {

bool HiResHook(uint8_t fmt, uint8_t siz,
               const uint8_t* rgba8, uint16_t width, uint16_t height,
               const uint8_t** outBuf, uint16_t* outW, uint16_t* outH) {
    // Master enable lives in a CVar so the menu toggle takes effect
    // immediately (no relaunch). Defaults to on — Init() already logged
    // whether the mods/ folder exists, so a flat "no PNGs to substitute"
    // setup is silently a no-op.
    if (CVarGetInteger("gHiResTextures.Enabled", 1) == 0) {
        return false;
    }

    const ssb64::hires::DecodedTexture* dec =
        ssb64::hires::HiResPack::Get().Lookup(fmt, siz, rgba8, width, height);
    if (dec == nullptr) return false;

    *outBuf = dec->rgba.data();
    *outW = dec->w;
    *outH = dec->h;
    return true;
}

} // namespace

extern "C" void ssb64_hires_register(void) {
    gfx_register_hires_hook(&HiResHook);
    port_log("HiResPack: hook registered with libultraship Fast3D\n");
}
