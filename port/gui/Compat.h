#pragma once

#include <cstdint>
#include <functional>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#ifndef CVAR_PREFIX_SETTING
#define CVAR_PREFIX_SETTING "gSettings"
#endif

#ifndef CVAR_PREFIX_WINDOW
#define CVAR_PREFIX_WINDOW "gOpenWindows"
#endif

#ifndef CVAR_VSYNC_ENABLED
#define CVAR_VSYNC_ENABLED "gVsyncEnabled"
#endif

#ifndef CVAR_INTERNAL_RESOLUTION
#define CVAR_INTERNAL_RESOLUTION "gInternalResolution"
#endif

#ifndef CVAR_MSAA_VALUE
#define CVAR_MSAA_VALUE "gMSAAValue"
#endif

#ifndef CVAR_SDL_WINDOWED_FULLSCREEN
#define CVAR_SDL_WINDOWED_FULLSCREEN "gSdlWindowedFullscreen"
#endif

#ifndef CVAR_TEXTURE_FILTER
#define CVAR_TEXTURE_FILTER "gTextureFilter"
#endif

#ifndef CVAR_IMGUI_CONTROLLER_NAV
#define CVAR_IMGUI_CONTROLLER_NAV "gControlNav"
#endif

#ifndef CVAR_CONSOLE_WINDOW_OPEN
#define CVAR_CONSOLE_WINDOW_OPEN "gConsoleEnabled"
#endif

#ifndef CVAR_CONTROLLER_CONFIGURATION_WINDOW_OPEN
#define CVAR_CONTROLLER_CONFIGURATION_WINDOW_OPEN "gControllerConfigurationEnabled"
#endif

#ifndef CVAR_GFX_DEBUGGER_WINDOW_OPEN
#define CVAR_GFX_DEBUGGER_WINDOW_OPEN "gGfxDebuggerEnabled"
#endif

#ifndef CVAR_STATS_WINDOW_OPEN
#define CVAR_STATS_WINDOW_OPEN "gStatsEnabled"
#endif

#ifndef CVAR_ENABLE_MULTI_VIEWPORTS
#define CVAR_ENABLE_MULTI_VIEWPORTS "gEnableMultiViewports"
#endif

#ifndef CVAR_LOW_RES_MODE
#define CVAR_LOW_RES_MODE "gLowResMode"
#endif

#ifndef CVAR_PREFIX_ADVANCED_RESOLUTION
#define CVAR_PREFIX_ADVANCED_RESOLUTION "gAdvancedResolution"
#endif

#ifndef CVAR_ALLOW_BACKGROUND_INPUTS
#define CVAR_ALLOW_BACKGROUND_INPUTS "gAllowBackgroundInputs"
#endif

#ifndef BTN_CUSTOM_MODIFIER1
#define BTN_CUSTOM_MODIFIER1 0
#endif

#ifndef BTN_CUSTOM_MODIFIER2
#define BTN_CUSTOM_MODIFIER2 0
#endif

#ifndef CVAR_SETTING
#define CVAR_SETTING(var) CVAR_PREFIX_SETTING "." var
#endif

#ifndef CVAR_WINDOW
#define CVAR_WINDOW(var) CVAR_PREFIX_WINDOW "." var
#endif

inline bool Ship_IsCStringEmpty(const char* str) {
    return str == nullptr || *str == '\0';
}

struct ShipInit {
    static std::unordered_map<std::string, std::vector<std::function<void()>>>& GetAll() {
        static std::unordered_map<std::string, std::vector<std::function<void()>>> shipInitFuncs;
        return shipInitFuncs;
    }

    static void InitAll() {
        Init("*");
    }

    static void Init(const std::string& path) {
        auto& shipInitFuncs = GetAll();
        for (const auto& initFunc : shipInitFuncs[path]) {
            initFunc();
        }
    }
};

struct RegisterShipInitFunc {
    RegisterShipInitFunc(std::function<void()> initFunc, const std::set<std::string>& updatePaths = {}) {
        auto& shipInitFuncs = ShipInit::GetAll();
        shipInitFuncs["*"].push_back(initFunc);
        for (const auto& path : updatePaths) {
            shipInitFuncs[path].push_back(initFunc);
        }
    }
};

namespace ShipUtils {
inline uint64_t& RandomState() {
    static uint64_t state = 0x9E3779B97F4A7C15ull;
    return state;
}

inline uint64_t NextU64(uint64_t* state = nullptr) {
    uint64_t& s = state != nullptr ? *state : RandomState();
    s ^= s >> 12;
    s ^= s << 25;
    s ^= s >> 27;
    return s * 2685821657736338717ull;
}

inline double RandomDouble(uint64_t* state = nullptr) {
    constexpr double scale = 1.0 / static_cast<double>(UINT64_MAX);
    return static_cast<double>(NextU64(state)) * scale;
}
} // namespace ShipUtils
