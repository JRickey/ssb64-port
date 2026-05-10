#include "enhancements.h"

#include <libultraship/bridge/consolevariablebridge.h>
#include <ship/Context.h>
#include <ship/controller/controldeck/ControlDeck.h>
#include <ship/controller/controldevice/controller/Controller.h>
#include <ship/controller/controldevice/controller/ControllerStick.h>
#include <ship/controller/controldevice/controller/mapping/ControllerAxisDirectionMapping.h>

#include <algorithm>
#include <cmath>

// built-in updater
#include <thread>
#include <mutex>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <nlohmann/json.hpp>
#include "port_log.h"

#if defined(__linux__)
#include <sys/stat.h>
#endif

namespace {

constexpr const char* kTapJumpCVars[PORT_ENHANCEMENT_MAX_PLAYERS] = {
    "gEnhancements.TapJumpDisabled.P1",
    "gEnhancements.TapJumpDisabled.P2",
    "gEnhancements.TapJumpDisabled.P3",
    "gEnhancements.TapJumpDisabled.P4",
};

constexpr const char* kHitboxViewCVar = "gEnhancements.HitboxView";
constexpr const char* kStageClearFrozenWallpaperCVar = "gEnhancements.StageClearFrozenWallpaper";

constexpr const char* kCStickSmashCVars[PORT_ENHANCEMENT_MAX_PLAYERS] = {
    "gEnhancements.CStickSmash.P1",
    "gEnhancements.CStickSmash.P2",
    "gEnhancements.CStickSmash.P3",
    "gEnhancements.CStickSmash.P4",
};

constexpr const char* kDPadJumpCVars[PORT_ENHANCEMENT_MAX_PLAYERS] = {
    "gEnhancements.DPadJump.P1",
    "gEnhancements.DPadJump.P2",
    "gEnhancements.DPadJump.P3",
    "gEnhancements.DPadJump.P4",
};

// Per-player NRage-style analog-stick remap. When the enable cvar for a player
// is on, src/sys/controller.c discards the libultraship Process()'d int8_t
// stick output and recomputes the value from the raw per-direction axis
// magnitudes via the per-axis formula in
// usbh_xpad.c (Ownasaurus/USBtoN64v2). Disabled by default; the LUS pipeline
// (circular deadzone + octagonal gate + notch snap) runs unchanged when off.
// Enable key has to live UNDER PX (as a sibling of Deadzone/Range), not AT PX.
// The config layer stores cvars in flattened JSON-pointer form and unflattens
// on every save; if PX were both a scalar (the enable flag) and a parent of
// PX.Deadzone / PX.Range, nlohmann's unflatten throws type_error.313 on the
// next save and Config::Save truncates the file to empty before serializing,
// wiping the user's settings (issue #96).
constexpr const char* kAnalogRemapEnableCVars[PORT_ENHANCEMENT_MAX_PLAYERS] = {
    "gEnhancements.AnalogRemap.P1.Enabled",
    "gEnhancements.AnalogRemap.P2.Enabled",
    "gEnhancements.AnalogRemap.P3.Enabled",
    "gEnhancements.AnalogRemap.P4.Enabled",
};
constexpr const char* kAnalogRemapDeadzoneCVars[PORT_ENHANCEMENT_MAX_PLAYERS] = {
    "gEnhancements.AnalogRemap.P1.Deadzone",
    "gEnhancements.AnalogRemap.P2.Deadzone",
    "gEnhancements.AnalogRemap.P3.Deadzone",
    "gEnhancements.AnalogRemap.P4.Deadzone",
};
constexpr const char* kAnalogRemapRangeCVars[PORT_ENHANCEMENT_MAX_PLAYERS] = {
    "gEnhancements.AnalogRemap.P1.Range",
    "gEnhancements.AnalogRemap.P2.Range",
    "gEnhancements.AnalogRemap.P3.Range",
    "gEnhancements.AnalogRemap.P4.Range",
};

constexpr float kAnalogRemapDeadzoneDefault = 0.10f;
constexpr float kAnalogRemapRangeDefault    = 1.00f;

// N64 button bitmasks. Duplicated here so the C++ layer doesn't have to pull
// in PR/os.h. Kept narrow (only what these enhancements need); if more
// enhancements grow C++-side button decoding, hoist into a shared header.
constexpr unsigned short A_BUTTON  = 0x8000;
constexpr unsigned short U_CBUTTON = 0x0008;
constexpr unsigned short D_CBUTTON = 0x0004;
constexpr unsigned short L_CBUTTON = 0x0002;
constexpr unsigned short R_CBUTTON = 0x0001;
constexpr unsigned short U_JPAD    = 0x0800;
constexpr unsigned short D_JPAD    = 0x0400;
constexpr unsigned short L_JPAD    = 0x0200;
constexpr unsigned short R_JPAD    = 0x0100;

// Mirrors dbObjectDisplayMode in src/sys/develop.h. Duplicated here to keep the
// C ABI of port_enhancement_hitbox_display_override() free of game headers.
enum {
    kDisplayModeMaster = 0,
    kDisplayModeHitCollisionFill = 1,
    kDisplayModeHitAttackOutline = 2,
    kDisplayModeMapCollision = 3,
};

} // namespace

extern "C" int port_enhancement_tap_jump_disabled(int playerIndex) {
    if (playerIndex < 0 || playerIndex >= PORT_ENHANCEMENT_MAX_PLAYERS) {
        return 0;
    }
    return CVarGetInteger(kTapJumpCVars[playerIndex], 0) != 0;
}

extern "C" int port_enhancement_hitbox_display_override(int current_mode) {
    int setting = CVarGetInteger(kHitboxViewCVar, 0);
    switch (setting) {
        case 1:
            return kDisplayModeHitCollisionFill;
        case 2:
            return kDisplayModeHitAttackOutline;
        default:
            return current_mode;
    }
}

extern "C" int port_enhancement_stage_clear_frozen_wallpaper_enabled(void) {
    return CVarGetInteger(kStageClearFrozenWallpaperCVar, 1) != 0;
}

extern "C" void port_enhancement_c_stick_smash(int player_index, unsigned short* button_hold, unsigned short* button_tap, signed char* stick_x, signed char* stick_y, unsigned short tap_pre_remap) {
    if (player_index < 0 || player_index >= PORT_ENHANCEMENT_MAX_PLAYERS) return;
    if (!CVarGetInteger(kCStickSmashCVars[player_index], 0)) return;

    // Always strip C-bits from both hold/tap so the engine never sees the raw
    // C-button input on this player when the remap is on (otherwise the stock
    // jump assignments would also fire alongside the synthesised attack).
    const unsigned short cbits = U_CBUTTON | D_CBUTTON | L_CBUTTON | R_CBUTTON;
    *button_hold &= ~cbits;
    *button_tap  &= ~cbits;

    // Fire the smash/aerial only on the rising edge of the C-button. A real
    // C-stick is analog and self-centers; translating it from a digital button
    // means as long as the C-button is pressed, the engine would otherwise see
    // stick pinned at ±80 — which manifests as drift in the air / running on
    // the ground (issue #97 [1]). Holding A across multiple frames also isn't
    // useful in SSB64 (no smash-attack charging), so a single-frame edge is
    // enough to register the attack.
    //
    // Cross-axis is forced to 0 so the input lands as a pure-axis tilt rather
    // than a 45° diagonal when the L-stick is already held sideways — without
    // this, e.g. C-up while running right reads as (80, 80) and the engine's
    // dominant-axis logic picks fair/bair instead of upair (issue #97 [2]).
    if (tap_pre_remap & U_CBUTTON) {
        *stick_x = 0;
        *stick_y = 80;
        *button_hold |= A_BUTTON;
        *button_tap  |= A_BUTTON;
    } else if (tap_pre_remap & D_CBUTTON) {
        *stick_x = 0;
        *stick_y = -80;
        *button_hold |= A_BUTTON;
        *button_tap  |= A_BUTTON;
    } else if (tap_pre_remap & L_CBUTTON) {
        *stick_x = -80;
        *stick_y = 0;
        *button_hold |= A_BUTTON;
        *button_tap  |= A_BUTTON;
    } else if (tap_pre_remap & R_CBUTTON) {
        *stick_x = 80;
        *stick_y = 0;
        *button_hold |= A_BUTTON;
        *button_tap  |= A_BUTTON;
    }
}

// NRage-style per-axis stick remap. See header comment for the design.
//
// Reads the raw per-direction normalized magnitudes from libultraship
// (0..MAX_AXIS_RANGE per direction) and reconstructs a signed per-axis value,
// then applies the verbatim usbh_xpad.c formula independently to X and Y:
//   - linear deadzone subtraction with rescale gain
//   - asymmetric negative branch (`-(in+1)`), faithful to the source
//   - linear range scale capped at 127
// No octagonal gate, no radial deadzone, no notch snap.
//
// When the enable cvar is off, this is a no-op so libultraship's Process()
// output flows through to the game unchanged.
extern "C" void port_enhancement_analog_remap(int player_index, signed char* stick_x, signed char* stick_y) {
    if (player_index < 0 || player_index >= PORT_ENHANCEMENT_MAX_PLAYERS) return;
    if (!CVarGetInteger(kAnalogRemapEnableCVars[player_index], 0)) return;

    auto ctx = Ship::Context::GetInstance();
    if (!ctx) return;
    auto deck = ctx->GetControlDeck();
    if (!deck) return;
    auto controller = deck->GetControllerByPort(static_cast<uint8_t>(player_index));
    if (!controller) return;
    auto stick = controller->GetLeftStick();
    if (!stick) return;

    const float right = stick->GetAxisDirectionValue(Ship::RIGHT);
    const float left  = stick->GetAxisDirectionValue(Ship::LEFT);
    const float up    = stick->GetAxisDirectionValue(Ship::UP);
    const float down  = stick->GetAxisDirectionValue(Ship::DOWN);

    // libultraship hands us per-direction magnitudes in [0, RAW_MAX] where
    // RAW_MAX is MAX_AXIS_RANGE (85.0f). The Ownasaurus/USBtoN64v2 firmware,
    // by contrast, runs the formula on the int16 USB stick value directly
    // (XPAD_MAX = 32767). Run the formula in the firmware's domain so it
    // is byte-faithful to usbh_xpad.c — including the `-(in+1)` asymmetry,
    // which is meaningful at int16 scale (one LSB of 32767, present to
    // protect against negating INT16_MIN) but would explode to ~1% of full
    // scale if applied to LUS's [-85, +85] floats directly. Output naturally
    // lands in [-127, +127] like the firmware.
    const float in_x = (right - left) * (32767.0f / 85.0f);
    const float in_y = (up - down)    * (32767.0f / 85.0f);

    constexpr float XPAD_MAX = 32767.0f;
    const float dz_pct =
        std::clamp(CVarGetFloat(kAnalogRemapDeadzoneCVars[player_index], kAnalogRemapDeadzoneDefault), 0.0f, 0.99f);
    const float rng_pct =
        std::clamp(CVarGetFloat(kAnalogRemapRangeCVars[player_index], kAnalogRemapRangeDefault), 0.50f, 1.50f);

    const float n64_max     = 127.0f * rng_pct;
    const float dz_val      = dz_pct * XPAD_MAX;
    const float dz_relation = (XPAD_MAX > dz_val) ? XPAD_MAX / (XPAD_MAX - dz_val) : 1.0f;

    // Verbatim usbh_xpad.c L260-L303 formula. The firmware truncates with a
    // (uint8_t) cast then unary-negates; at range > ~100% that truncation
    // wraps a >127 result into a small negative value (firmware bug). We
    // clamp to s8 instead so the user's range slider stays monotonic past
    // 100%.
    auto remap_axis = [&](float stick) -> signed char {
        float out;
        if (stick >= dz_val) {
            float unscaled = (stick - dz_val) * dz_relation;
            out = unscaled * (n64_max / XPAD_MAX);
        } else if (stick <= -dz_val) {
            float temp     = -(stick + 1.0f);
            float unscaled = (temp - dz_val) * dz_relation;
            out            = -(unscaled * (n64_max / XPAD_MAX));
        } else {
            return 0;
        }
        if (out >  127.0f) out =  127.0f;
        if (out < -127.0f) out = -127.0f;
        return static_cast<signed char>(out);
    };

    *stick_x = remap_axis(in_x);
    *stick_y = remap_axis(in_y);
}

extern "C" void port_enhancement_dpad_jump(int player_index, unsigned short* button_hold, unsigned short* button_tap, unsigned short tap_pre_remap) {
    if (player_index < 0 || player_index >= PORT_ENHANCEMENT_MAX_PLAYERS) return;
    if (!CVarGetInteger(kDPadJumpCVars[player_index], 0)) return;

    // If any D-Pad direction is pressed, inject a C-Up (Jump) into the engine.
    if (*button_hold & (U_JPAD | D_JPAD | L_JPAD | R_JPAD)) {
        *button_hold |= U_CBUTTON;
        if (tap_pre_remap & (U_JPAD | D_JPAD | L_JPAD | R_JPAD)) {
            *button_tap |= U_CBUTTON;
        }
    }
}

extern "C" {
    int port_cheat_unlock_all()        { return CVarGetInteger("gCheats.UnlockAll", 0); }
    int port_cheat_unlock_luigi()      { return CVarGetInteger("gCheats.UnlockLuigi", 0); }
    int port_cheat_unlock_ness()       { return CVarGetInteger("gCheats.UnlockNess", 0); }
    int port_cheat_unlock_captain()    { return CVarGetInteger("gCheats.UnlockCaptain", 0); }
    int port_cheat_unlock_purin()      { return CVarGetInteger("gCheats.UnlockPurin", 0); }
    int port_cheat_unlock_inishie()    { return CVarGetInteger("gCheats.UnlockInishie", 0); }
    int port_cheat_unlock_soundtest()  { return CVarGetInteger("gCheats.UnlockSoundTest", 0); }
    int port_cheat_unlock_itemswitch() { return CVarGetInteger("gCheats.UnlockItemSwitch", 0); }
}

namespace ssb64 {
namespace enhancements {

// We use atomics for the state flags so the UI thread doesn't have to lock a mutex 60 times a second!
static std::atomic<bool> s_updateChecked{false};
static std::atomic<bool> s_updateAvailable{false};
static std::atomic<bool> s_isDownloading{false};
static std::atomic<bool> s_downloadComplete{false};
static std::atomic<bool> s_isCheckingForUpdates{false};

static std::string s_latestVersion = "";
static std::string s_downloadUrl = "";
static std::string s_downloadStatus = "";
static std::mutex s_stringMutex; // Only locks when reading/writing the actual text

#ifndef BATTLESHIP_CURRENT_VERSION
#define BATTLESHIP_CURRENT_VERSION "v1.0.0"
#endif

void CheckForUpdatesAsync(bool force) {
    // Check our atomic flags (no locks required)
    if (s_isCheckingForUpdates.load() || s_isDownloading.load()) return;
    if (!force && s_updateChecked.load()) return;

    s_updateChecked.store(true);
    s_isCheckingForUpdates.store(true);

    std::thread([]() {
        #if defined(_WIN32)
        FILE* pipe = _popen("curl -s -m 10 -H \"User-Agent: BattleShip-Updater\" https://api.github.com/repos/JRickey/BattleShip/tags", "r");
        #else
        FILE* pipe = popen("curl -s -m 10 -H \"User-Agent: BattleShip-Updater\" https://api.github.com/repos/JRickey/BattleShip/tags", "r");
        #endif
        if (!pipe) {
            s_isCheckingForUpdates.store(false);
            return;
        }

        std::string response = "";
        char buffer[128];
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            response += buffer;
        }

        #if defined(_WIN32)
        _pclose(pipe);
        #else
        pclose(pipe);
        #endif

        if (!response.empty()) {
            try {
                auto json = nlohmann::json::parse(response);

                if (json.is_array() && !json.empty() && json[0].contains("name")) {
                    std::string latest_tag = json[0]["name"];

                    {
                        std::lock_guard<std::mutex> lock(s_stringMutex);
                        s_latestVersion = latest_tag;
                    }

                    if (latest_tag != BATTLESHIP_CURRENT_VERSION) {
                        s_updateAvailable.store(true);

                        #if defined(__linux__)
                        std::lock_guard<std::mutex> lock(s_stringMutex);
                        s_downloadUrl = "https://github.com/JRickey/BattleShip/releases/download/" + latest_tag + "/BattleShip-x86_64.AppImage";
                        #elif defined(_WIN32)
                        std::lock_guard<std::mutex> lock(s_stringMutex);
                        s_downloadUrl = "https://github.com/JRickey/BattleShip/releases/download/" + latest_tag + "/BattleShip-windows.zip";
                        #elif defined(__APPLE__)
                        std::lock_guard<std::mutex> lock(s_stringMutex);
                        s_downloadUrl = "https://github.com/JRickey/BattleShip/releases/download/" + latest_tag + "/BattleShip.dmg";
                        #endif
                    } else {
                        s_updateAvailable.store(false);
                    }
                }
            } catch (...) {}
        }

        s_isCheckingForUpdates.store(false);
    }).detach();
}

void StartGameUpdate() {
    if (s_isDownloading.load()) return;
    s_isDownloading.store(true);

    {
        std::lock_guard<std::mutex> lock(s_stringMutex);
        s_downloadStatus = "Initializing download...";
    }

    std::thread([]() {
        std::string url;
        {
            std::lock_guard<std::mutex> lock(s_stringMutex);
            url = s_downloadUrl;
        }

        #if defined(__linux__)
        const char* appImagePath = getenv("APPIMAGE");
        if (!appImagePath) {
            {
                std::lock_guard<std::mutex> lock(s_stringMutex);
                s_downloadStatus = "Error: Not running as AppImage";
            }
            s_isDownloading.store(false);
            return;
        }

        std::string tempPath = std::string(appImagePath) + ".part";
        std::string cmd = "curl -L -# -o \"" + tempPath + "\" \"" + url + "\" 2>&1";
        FILE* pipe = popen(cmd.c_str(), "r");

        #elif defined(_WIN32)
        std::string tempZip = "update_temp.zip";
        std::string cmd = "curl -L -# -o \"" + tempZip + "\" \"" + url + "\" 2>&1";
        FILE* pipe = _popen(cmd.c_str(), "r");

        #elif defined(__APPLE__)
        std::string tempDmg = "/tmp/BattleShip_Update.dmg";
        std::string cmd = "curl -L -# -o \"" + tempDmg + "\" \"" + url + "\" 2>&1";
        FILE* pipe = popen(cmd.c_str(), "r");
        #endif

        if (!pipe) {
            {
                std::lock_guard<std::mutex> lock(s_stringMutex);
                s_downloadStatus = "Error: Failed to launch download.";
            }
            s_isDownloading.store(false);
            return;
        }

        // Universal, freeze-proof read loop
        int c;
        std::string currentLine = "";
        while ((c = fgetc(pipe)) != EOF) {
            if (c == '\r' || c == '\n') {
                if (!currentLine.empty() && currentLine.find('%') != std::string::npos) {

                    // Filter out ALL terminal escape codes. Only keep numbers and decimals!
                    std::string pctStr = "";
                    for (char ch : currentLine) {
                        if ((ch >= '0' && ch <= '9') || ch == '.') {
                            pctStr += ch;
                        }
                    }

                    if (!pctStr.empty()) {
                        std::lock_guard<std::mutex> lock(s_stringMutex);
                        s_downloadStatus = "Downloading... " + pctStr + "%";
                    }
                }
                currentLine.clear();
            } else {
                currentLine += (char)c;
            }
        }

        #if defined(_WIN32)
        int exitCode = _pclose(pipe);
        #else
        int exitCode = pclose(pipe);
        #endif

        if (exitCode == 0) {
            #if defined(__linux__)
            chmod(tempPath.c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
            if (std::rename(tempPath.c_str(), appImagePath) == 0) {
                {
                    std::lock_guard<std::mutex> lock(s_stringMutex);
                    s_downloadStatus = "Update complete! Please restart the game.";
                }
                s_downloadComplete.store(true);
            } else {
                std::lock_guard<std::mutex> lock(s_stringMutex);
                s_downloadStatus = "Error: Failed to replace AppImage.";
            }
            #elif defined(_WIN32)
            FILE* bat = fopen("update_game.bat", "w");
            if (bat) {
                fprintf(bat, "@echo off\n");
                fprintf(bat, "echo Update downloaded! Waiting for BattleShip to close before applying...\n");
                fprintf(bat, ":wait\n");
                fprintf(bat, "tasklist /FI \"IMAGENAME eq BattleShip.exe\" 2>NUL | find /I /N \"BattleShip.exe\">NUL\n");
                fprintf(bat, "if \"%%ERRORLEVEL%%\"==\"0\" (\n");
                fprintf(bat, "    timeout /t 1 /nobreak > NUL\n");
                fprintf(bat, "    goto wait\n");
                fprintf(bat, ")\n");
                fprintf(bat, "echo Installing update...\n");
                fprintf(bat, "tar -xf update_temp.zip\n");
                fprintf(bat, "del update_temp.zip\n");
                fprintf(bat, "start BattleShip.exe\n");
                fprintf(bat, "(goto) 2>nul & del \"%%~f0\"\n");
                fclose(bat);

                {
                    std::lock_guard<std::mutex> lock(s_stringMutex);
                    s_downloadStatus = "Update ready! Close the game to apply.";
                }
                s_downloadComplete.store(true);
                system("start \"BattleShip Updater\" \"update_game.bat\"");
            } else {
                std::lock_guard<std::mutex> lock(s_stringMutex);
                s_downloadStatus = "Error: Failed to create updater script.";
            }
            #elif defined(__APPLE__)
            {
                std::lock_guard<std::mutex> lock(s_stringMutex);
                s_downloadStatus = "Download complete! Opening DMG...";
            }
            s_downloadComplete.store(true);
            std::string openCmd = "open \"" + tempDmg + "\"";
            system(openCmd.c_str());
            #endif
        } else {
            {
                std::lock_guard<std::mutex> lock(s_stringMutex);
                s_downloadStatus = "Error: Download failed.";
            }
            #if defined(__linux__)
            std::remove(tempPath.c_str());
            #elif defined(_WIN32)
            std::remove(tempZip.c_str());
            #elif defined(__APPLE__)
            std::remove(tempDmg.c_str());
            #endif
        }

        s_isDownloading.store(false);
    }).detach();
}

// OS-aware URL opener for Windows and Mac fallback
void OpenReleasePage() {
    const char* url = "https://github.com/JRickey/BattleShip/releases/latest";

    #if defined(_WIN32)
    std::string cmd = std::string("start \"\" \"") + url + "\"";
    system(cmd.c_str());
    #elif defined(__APPLE__)
    std::string cmd = std::string("open ") + url;
    system(cmd.c_str());
    #else
    std::string cmd = std::string("xdg-open ") + url + " &";
    system(cmd.c_str());
    #endif
}

// Atomic reads - no locks required for the UI thread!
bool IsCheckingForUpdates() { return s_isCheckingForUpdates.load(); }
bool IsUpdateAvailable() { return s_updateAvailable.load(); }
bool IsDownloading() { return s_isDownloading.load(); }
bool IsDownloadComplete() { return s_downloadComplete.load(); }

// String getters still need the lock, but they are incredibly fast now
std::string GetDownloadStatus() { std::lock_guard<std::mutex> lock(s_stringMutex); return s_downloadStatus; }
std::string GetLatestVersion() { std::lock_guard<std::mutex> lock(s_stringMutex); return s_latestVersion; }

const char* TapJumpCVarName(int playerIndex) {
    if (playerIndex < 0 || playerIndex >= PORT_ENHANCEMENT_MAX_PLAYERS) {
        return kTapJumpCVars[0];
    }
    return kTapJumpCVars[playerIndex];
}

const char* HitboxViewCVarName() {
    return kHitboxViewCVar;
}

const char* StageClearFrozenWallpaperCVarName() {
    return kStageClearFrozenWallpaperCVar;
}

const char* CStickSmashCVarName(int playerIndex) {
    if (playerIndex < 0 || playerIndex >= PORT_ENHANCEMENT_MAX_PLAYERS) {
        return kCStickSmashCVars[0];
    }
    return kCStickSmashCVars[playerIndex];
}

const char* DPadJumpCVarName(int playerIndex) {
    if (playerIndex < 0 || playerIndex >= PORT_ENHANCEMENT_MAX_PLAYERS) {
        return kDPadJumpCVars[0];
    }
    return kDPadJumpCVars[playerIndex];
}

const char* AnalogRemapCVarName(int playerIndex) {
    if (playerIndex < 0 || playerIndex >= PORT_ENHANCEMENT_MAX_PLAYERS) {
        return kAnalogRemapEnableCVars[0];
    }
    return kAnalogRemapEnableCVars[playerIndex];
}

const char* AnalogRemapDeadzoneCVarName(int playerIndex) {
    if (playerIndex < 0 || playerIndex >= PORT_ENHANCEMENT_MAX_PLAYERS) {
        return kAnalogRemapDeadzoneCVars[0];
    }
    return kAnalogRemapDeadzoneCVars[playerIndex];
}

const char* AnalogRemapRangeCVarName(int playerIndex) {
    if (playerIndex < 0 || playerIndex >= PORT_ENHANCEMENT_MAX_PLAYERS) {
        return kAnalogRemapRangeCVars[0];
    }
    return kAnalogRemapRangeCVars[playerIndex];
}

}
} // namespace ssb64::enhancements
