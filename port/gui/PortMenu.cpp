#include "PortMenu.h"

#include "Compat.h"
#include "../bridge/framebuffer_capture.h"
#include "../enhancements/enhancements.h"

#include <fast/backends/gfx_rendering_api.h>
#include <ship/Context.h>
#include <ship/window/Window.h>
#include <ship/window/gui/Gui.h>

#include <SDL2/SDL.h>
#include <spdlog/fmt/fmt.h>

#include <algorithm>
#include <cassert>
#include <filesystem>

namespace fs = std::filesystem;

namespace ssb64 {

using namespace UIWidgets;

namespace {
static const std::map<int32_t, const char*> kMenuThemeOptions = {
    { UIWidgets::Colors::Red, "Red" },
    { UIWidgets::Colors::DarkRed, "Dark Red" },
    { UIWidgets::Colors::Orange, "Orange" },
    { UIWidgets::Colors::Green, "Green" },
    { UIWidgets::Colors::DarkGreen, "Dark Green" },
    { UIWidgets::Colors::LightBlue, "Light Blue" },
    { UIWidgets::Colors::Blue, "Blue" },
    { UIWidgets::Colors::DarkBlue, "Dark Blue" },
    { UIWidgets::Colors::Indigo, "Indigo" },
    { UIWidgets::Colors::Violet, "Violet" },
    { UIWidgets::Colors::Purple, "Purple" },
    { UIWidgets::Colors::Brown, "Brown" },
    { UIWidgets::Colors::Gray, "Gray" },
    { UIWidgets::Colors::DarkGray, "Dark Gray" },
};

static const std::map<int32_t, const char*> kTextureFilteringMap = {
    { Fast::FILTER_THREE_POINT, "Three-Point" },
    { Fast::FILTER_LINEAR, "Linear" },
    { Fast::FILTER_NONE, "None" },
};

// Mirrors dbObjectDisplayMode (src/sys/develop.h). 0 disables the override and
// returns the engine's normal rendering.
static const std::map<int32_t, const char*> kHitboxViewMap = {
    { 0, "Off" },
    { 1, "Filled (red cubes)" },
    { 2, "Outline + opaque hurtboxes" },
};
} // namespace

void PortMenu::AddSidebarEntry(std::string sectionName, std::string sidebarName, uint32_t columnCount) {
    assert(!sectionName.empty());
    assert(!sidebarName.empty());
    menuEntries.at(sectionName).sidebars.emplace(sidebarName, SidebarEntry{ .columnCount = columnCount });
    menuEntries.at(sectionName).sidebarOrder.push_back(sidebarName);
}

WidgetInfo& PortMenu::AddWidget(WidgetPath& pathInfo, std::string widgetName, WidgetType widgetType) {
    assert(!widgetName.empty());
    assert(menuEntries.contains(pathInfo.sectionName));
    assert(menuEntries.at(pathInfo.sectionName).sidebars.contains(pathInfo.sidebarName));

    auto& sidebars = menuEntries.at(pathInfo.sectionName).sidebars;
    const uint8_t column = pathInfo.column;
    while (sidebars.at(pathInfo.sidebarName).columnWidgets.size() < column + 1) {
        sidebars.at(pathInfo.sidebarName).columnWidgets.push_back({});
    }

    SidebarEntry& entry = sidebars.at(pathInfo.sidebarName);
    entry.columnWidgets.at(column).push_back({ .name = std::move(widgetName), .type = widgetType });
    WidgetInfo& widget = entry.columnWidgets.at(column).back();

    switch (widgetType) {
        case WIDGET_CHECKBOX:
        case WIDGET_CVAR_CHECKBOX:
            widget.options = std::make_shared<CheckboxOptions>();
            break;
        case WIDGET_SLIDER_FLOAT:
        case WIDGET_CVAR_SLIDER_FLOAT:
            widget.options = std::make_shared<FloatSliderOptions>();
            break;
        case WIDGET_CVAR_BTN_SELECTOR:
            widget.options = std::make_shared<BtnSelectorOptions>();
            break;
        case WIDGET_SLIDER_INT:
        case WIDGET_CVAR_SLIDER_INT:
            widget.options = std::make_shared<IntSliderOptions>();
            break;
        case WIDGET_COMBOBOX:
        case WIDGET_CVAR_COMBOBOX:
        case WIDGET_AUDIO_BACKEND:
        case WIDGET_VIDEO_BACKEND:
            widget.options = std::make_shared<ComboboxOptions>();
            break;
        case WIDGET_BUTTON:
            widget.options = std::make_shared<ButtonOptions>();
            break;
        case WIDGET_WINDOW_BUTTON:
            widget.options = std::make_shared<WindowButtonOptions>();
            break;
        case WIDGET_CVAR_COLOR_PICKER:
        case WIDGET_COLOR_PICKER:
            widget.options = std::make_shared<ColorPickerOptions>();
            break;
        case WIDGET_SEPARATOR_TEXT:
        case WIDGET_TEXT:
            widget.options = std::make_shared<TextOptions>();
            break;
        case WIDGET_SEARCH:
        case WIDGET_SEPARATOR:
        default:
            widget.options = std::make_shared<WidgetOptions>();
            break;
    }

    return widget;
}

PortMenu::PortMenu()
    : Menu("gOpenPortMenu", "Port Menu", 0, UIWidgets::Colors::LightBlue) {
}

void PortMenu::AddMenuSettings() {
    AddMenuEntry("Settings", CVAR_SETTING("Menu.SettingsSidebarSection"));
    AddSidebarEntry("Settings", "General", 2);

    WidgetPath path = { "Settings", "General", SECTION_COLUMN_1 };
    AddWidget(path, "Menu Settings", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "Menu Theme", WIDGET_CVAR_COMBOBOX)
        .CVar(CVAR_SETTING("Menu.Theme"))
        .RaceDisable(false)
        .Options(ComboboxOptions().Tooltip("Changes the theme of the menu widgets.")
                     .ComboMap(kMenuThemeOptions)
                     .DefaultIndex(Colors::LightBlue));

    AddWidget(path, "Menu Controller Navigation", WIDGET_CVAR_CHECKBOX)
        .CVar(CVAR_IMGUI_CONTROLLER_NAV)
        .RaceDisable(false)
        .Options(CheckboxOptions().Tooltip(
            "Allows controller navigation of the port menu.\nCAUTION: this disables game inputs while the menu is visible."));

    AddWidget(path, "Allow background inputs", WIDGET_CVAR_CHECKBOX)
        .CVar(CVAR_ALLOW_BACKGROUND_INPUTS)
        .RaceDisable(false)
        .Callback([](WidgetInfo&) {
            SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS,
                        CVarGetInteger(CVAR_ALLOW_BACKGROUND_INPUTS, 1) ? "1" : "0");
        })
        .Options(CheckboxOptions().Tooltip(
                     "Allows controller input to be picked up by the game even when the window is not focused.")
                     .DefaultValue(true));

    AddWidget(path, "Menu Background Opacity", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar(CVAR_SETTING("Menu.BackgroundOpacity"))
        .RaceDisable(false)
        .Options(FloatSliderOptions().DefaultValue(0.85f).IsPercentage().Tooltip(
            "Sets the opacity of the background behind the port menu."));

    AddWidget(path, "General Settings", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "Cursor Always Visible", WIDGET_CVAR_CHECKBOX)
        .CVar(CVAR_SETTING("CursorVisibility"))
        .RaceDisable(false)
        .Callback([](WidgetInfo&) {
            Ship::Context::GetInstance()->GetWindow()->SetForceCursorVisibility(
                CVarGetInteger(CVAR_SETTING("CursorVisibility"), 0));
        })
        .Options(CheckboxOptions().Tooltip("Makes the cursor always visible, even in fullscreen."));

    AddWidget(path, "Search In Sidebar", WIDGET_CVAR_CHECKBOX)
        .CVar(CVAR_SETTING("Menu.SidebarSearch"))
        .RaceDisable(false)
        .Callback([this](WidgetInfo&) {
            if (CVarGetInteger(CVAR_SETTING("Menu.SidebarSearch"), 0)) {
                InsertSidebarSearch();
            } else {
                RemoveSidebarSearch();
            }
        })
        .Options(CheckboxOptions().Tooltip(
            "Displays the Search page as a sidebar entry in Settings instead of in the header."));

    AddWidget(path, "Search Input Autofocus", WIDGET_CVAR_CHECKBOX)
        .CVar(CVAR_SETTING("Menu.SearchAutofocus"))
        .RaceDisable(false)
        .Options(CheckboxOptions().Tooltip("Gives the search input focus when it becomes visible."));

    AddWidget(path, "Open App Files Folder", WIDGET_BUTTON)
        .RaceDisable(false)
        .Callback([](WidgetInfo&) {
            std::string filesPath = Ship::Context::GetInstance()->GetAppDirectoryPath();
            SDL_OpenURL(std::string("file:///" + fs::absolute(filesPath).string()).c_str());
        })
        .Options(ButtonOptions().Tooltip("Opens the folder that contains the save, config, and asset files."));

    path.column = SECTION_COLUMN_2;
    AddWidget(path, "About", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "BattleShip", WIDGET_TEXT);
    AddWidget(path, "Native Smash 64 PC port powered by libultraship", WIDGET_TEXT);

    path.sidebarName = "Graphics";
    path.column = SECTION_COLUMN_1;
    AddSidebarEntry("Settings", "Graphics", 2);

    AddWidget(path, "Graphics Options", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "Toggle Fullscreen", WIDGET_BUTTON)
        .RaceDisable(false)
        .Callback([](WidgetInfo&) { Ship::Context::GetInstance()->GetWindow()->ToggleFullscreen(); })
        .Options(ButtonOptions().Tooltip("Toggles fullscreen on or off."));

    AddWidget(path, "Internal Resolution", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar(CVAR_INTERNAL_RESOLUTION)
        .RaceDisable(false)
        .Callback([](WidgetInfo&) {
            Ship::Context::GetInstance()->GetWindow()->SetResolutionMultiplier(
                CVarGetFloat(CVAR_INTERNAL_RESOLUTION, 1.0f));
        })
        .PreFunc([this](WidgetInfo& info) {
            if (disabledMap.at(DISABLE_FOR_ADVANCED_RESOLUTION_ON).active &&
                disabledMap.at(DISABLE_FOR_VERTICAL_RES_TOGGLE_ON).active) {
                info.activeDisables.push_back(DISABLE_FOR_ADVANCED_RESOLUTION_ON);
                info.activeDisables.push_back(DISABLE_FOR_VERTICAL_RES_TOGGLE_ON);
            } else if (disabledMap.at(DISABLE_FOR_LOW_RES_MODE_ON).active) {
                info.activeDisables.push_back(DISABLE_FOR_LOW_RES_MODE_ON);
            }
        })
        .Options(FloatSliderOptions()
                     .Tooltip("Multiplies the internal rendering resolution.")
                     .ShowButtons(false)
                     .IsPercentage()
                     .Min(0.5f)
                     .Max(2.0f));

#ifndef __WIIU__
    AddWidget(path, "Anti-aliasing (MSAA)", WIDGET_CVAR_SLIDER_INT)
        .CVar(CVAR_MSAA_VALUE)
        .RaceDisable(false)
        .Callback([](WidgetInfo&) {
            Ship::Context::GetInstance()->GetWindow()->SetMsaaLevel(CVarGetInteger(CVAR_MSAA_VALUE, 1));
        })
        .Options(IntSliderOptions()
                     .Tooltip("Sets the MSAA level for rendered geometry.")
                     .Min(1)
                     .Max(8)
                     .DefaultValue(1));
#endif

    AddWidget(path, "Renderer API (Needs reload)", WIDGET_VIDEO_BACKEND).RaceDisable(false);
    AddWidget(path, "Enable Vsync", WIDGET_CVAR_CHECKBOX)
        .CVar(CVAR_VSYNC_ENABLED)
        .RaceDisable(false)
        .PreFunc([this](WidgetInfo& info) { info.isHidden = disabledMap.at(DISABLE_FOR_NO_VSYNC).active; })
        .Options(CheckboxOptions().Tooltip("Removes tearing, but can cap the game to the display refresh rate.")
                     .DefaultValue(true));

    AddWidget(path, "Windowed Fullscreen", WIDGET_CVAR_CHECKBOX)
        .CVar(CVAR_SDL_WINDOWED_FULLSCREEN)
        .RaceDisable(false)
        .PreFunc([this](WidgetInfo& info) {
            info.isHidden = disabledMap.at(DISABLE_FOR_NO_WINDOWED_FULLSCREEN).active;
        })
        .Options(CheckboxOptions().Tooltip("Enables borderless windowed fullscreen mode."));

    AddWidget(path, "Allow multi-windows", WIDGET_CVAR_CHECKBOX)
        .CVar(CVAR_ENABLE_MULTI_VIEWPORTS)
        .RaceDisable(false)
        .PreFunc([this](WidgetInfo& info) {
            info.isHidden = disabledMap.at(DISABLE_FOR_NO_MULTI_VIEWPORT).active;
        })
        .Options(CheckboxOptions()
                     .Tooltip("Allows multiple ImGui windows to be opened at once. Requires reload to take effect.")
                     .DefaultValue(true));

    AddWidget(path, "Texture Filter (Needs reload)", WIDGET_CVAR_COMBOBOX)
        .CVar(CVAR_TEXTURE_FILTER)
        .RaceDisable(false)
        .Options(ComboboxOptions().Tooltip("Sets the active texture filtering mode.").ComboMap(kTextureFilteringMap));

    path.sidebarName = "Gameplay";
    path.column = SECTION_COLUMN_1;
    AddSidebarEntry("Settings", "Gameplay", 1);

    /*
    AddWidget(path, "Per-Port Enhancements", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "Disable Tap Jump (P1)", WIDGET_CVAR_CHECKBOX)
        .CVar(enhancements::TapJumpCVarName(0))
        .RaceDisable(false)
        .Options(CheckboxOptions().Tooltip(
            "When enabled, pushing up on the analog stick no longer triggers a jump for player 1. "
            "Jump buttons (X, Y, C-up, etc.) still work."));
    AddWidget(path, "Disable Tap Jump (P2)", WIDGET_CVAR_CHECKBOX)
        .CVar(enhancements::TapJumpCVarName(1))
        .RaceDisable(false)
        .Options(CheckboxOptions().Tooltip("Same as P1, applied to player 2."));
    AddWidget(path, "Disable Tap Jump (P3)", WIDGET_CVAR_CHECKBOX)
        .CVar(enhancements::TapJumpCVarName(2))
        .RaceDisable(false)
        .Options(CheckboxOptions().Tooltip("Same as P1, applied to player 3."));
    AddWidget(path, "Disable Tap Jump (P4)", WIDGET_CVAR_CHECKBOX)
        .CVar(enhancements::TapJumpCVarName(3))
        .RaceDisable(false)
        .Options(CheckboxOptions().Tooltip("Same as P1, applied to player 4."));
    */

    AddWidget(path, "1P Stage Clear: Frozen Frame Background", WIDGET_CVAR_CHECKBOX)
        .CVar(enhancements::StageClearFrozenWallpaperCVarName())
        .RaceDisable(false)
        .Callback([](WidgetInfo&) {
            // When this flips on we need LUS to start rendering off-screen so
            // mGameFb is populated by the time the next stage-clear scene
            // transition fires. When it flips off we drop the per-frame blit.
            port_capture_set_force_render_to_fb(
                port_enhancement_stage_clear_frozen_wallpaper_enabled());
        })
        .Options(CheckboxOptions().Tooltip(
            "On real hardware the 1P stage-clear bonus screen freezes the last gameplay frame "
            "as the background. The port reproduces this via a GPU readback when the scene "
            "loads. While enabled, the renderer draws each frame to an off-screen buffer "
            "(sub-millisecond cost) so the prior gameplay frame is preserved across the "
            "scene transition. Disable to revert to a solid black background.")
                     .DefaultValue(true));
    AddWidget(path, "Debug", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "Hitbox View", WIDGET_CVAR_COMBOBOX)
        .CVar(enhancements::HitboxViewCVarName())
        .RaceDisable(false)
        .Options(ComboboxOptions()
                     .Tooltip("Replaces fighter/item/weapon rendering with the debug "
                              "hitbox visualization the decomp ships. Filled mode "
                              "draws red cubes for active hitboxes; Outline mode "
                              "draws hitbox edges and shows hurtboxes as solid "
                              "yellow/green/blue boxes (yellow=normal, green="
                              "invincible, blue=intangible).")
                     .ComboMap(kHitboxViewMap)
                     .DefaultIndex(0));

    AddWidget(path, "Per-Port Enhancements", WIDGET_SEPARATOR_TEXT);

    // --- Player 1 ---
    AddWidget(path, "Player 1", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "Disable Tap Jump (P1)", WIDGET_CVAR_CHECKBOX)
        .CVar(enhancements::TapJumpCVarName(0))
        .RaceDisable(false)
        .Options(CheckboxOptions().Tooltip("Disables jumping by pushing up on the analog stick."));
    AddWidget(path, "C-Stick Smash (P1)", WIDGET_CVAR_CHECKBOX)
        .CVar(enhancements::CStickSmashCVarName(0))
        .RaceDisable(false)
        .Options(CheckboxOptions().Tooltip("Replaces C-Button inputs with instant smash attacks."));
    AddWidget(path, "D-Pad to Jump (P1)", WIDGET_CVAR_CHECKBOX)
        .CVar(enhancements::DPadJumpCVarName(0))
        .RaceDisable(false)
        .Options(CheckboxOptions().Tooltip("Translates N64 D-Pad inputs into C-Up (Jump)."));

    // --- Player 2 ---
    AddWidget(path, "Player 2", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "Disable Tap Jump (P2)", WIDGET_CVAR_CHECKBOX)
        .CVar(enhancements::TapJumpCVarName(1))
        .RaceDisable(false)
        .Options(CheckboxOptions().Tooltip("Same as P1, applied to player 2."));
    AddWidget(path, "C-Stick Smash (P2)", WIDGET_CVAR_CHECKBOX)
        .CVar(enhancements::CStickSmashCVarName(1))
        .RaceDisable(false)
        .Options(CheckboxOptions().Tooltip("Same as P1, applied to player 2."));
    AddWidget(path, "D-Pad to Jump (P2)", WIDGET_CVAR_CHECKBOX)
        .CVar(enhancements::DPadJumpCVarName(1))
        .RaceDisable(false)
        .Options(CheckboxOptions().Tooltip("Same as P1, applied to player 2."));

    // --- Player 3 ---
    AddWidget(path, "Player 3", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "Disable Tap Jump (P3)", WIDGET_CVAR_CHECKBOX)
        .CVar(enhancements::TapJumpCVarName(2))
        .RaceDisable(false)
        .Options(CheckboxOptions().Tooltip("Same as P1, applied to player 3."));
    AddWidget(path, "C-Stick Smash (P3)", WIDGET_CVAR_CHECKBOX)
        .CVar(enhancements::CStickSmashCVarName(2))
        .RaceDisable(false)
        .Options(CheckboxOptions().Tooltip("Same as P1, applied to player 3."));
    AddWidget(path, "D-Pad to Jump (P3)", WIDGET_CVAR_CHECKBOX)
        .CVar(enhancements::DPadJumpCVarName(2))
        .RaceDisable(false)
        .Options(CheckboxOptions().Tooltip("Same as P1, applied to player 3."));

    // --- Player 4 ---
    AddWidget(path, "Player 4", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "Disable Tap Jump (P4)", WIDGET_CVAR_CHECKBOX)
        .CVar(enhancements::TapJumpCVarName(3))
        .RaceDisable(false)
        .Options(CheckboxOptions().Tooltip("Same as P1, applied to player 4."));
    AddWidget(path, "C-Stick Smash (P4)", WIDGET_CVAR_CHECKBOX)
        .CVar(enhancements::CStickSmashCVarName(3))
        .RaceDisable(false)
        .Options(CheckboxOptions().Tooltip("Same as P1, applied to player 4."));
    AddWidget(path, "D-Pad to Jump (P4)", WIDGET_CVAR_CHECKBOX)
        .CVar(enhancements::DPadJumpCVarName(3))
        .RaceDisable(false)
        .Options(CheckboxOptions().Tooltip("Same as P1, applied to player 4."));
}

void PortMenu::AddMenuWindows() {
    AddMenuEntry("Windows", CVAR_SETTING("Menu.WindowsSidebarSection"));

    WidgetPath path = { "Windows", "Tools", SECTION_COLUMN_1 };
    AddSidebarEntry("Windows", "Tools", 1);
    AddWidget(path, "Debug Windows", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "Stats", WIDGET_WINDOW_BUTTON)
        .CVar(CVAR_STATS_WINDOW_OPEN)
        .RaceDisable(false)
        .WindowName("Stats")
        .HideInSearch(true)
        .Options(WindowButtonOptions().Tooltip("Toggles the Stats window."));
    AddWidget(path, "Console", WIDGET_WINDOW_BUTTON)
        .CVar(CVAR_CONSOLE_WINDOW_OPEN)
        .RaceDisable(false)
        .WindowName("Console")
        .HideInSearch(true)
        .Options(WindowButtonOptions().Tooltip("Toggles the Console window."));
    AddWidget(path, "Gfx Debugger", WIDGET_WINDOW_BUTTON)
        .CVar(CVAR_GFX_DEBUGGER_WINDOW_OPEN)
        .RaceDisable(false)
        .WindowName("GfxDebuggerWindow")
        .HideInSearch(true)
        .Options(WindowButtonOptions().Tooltip("Toggles the graphics debugger window."));

    path.sidebarName = "Input";
    AddSidebarEntry("Windows", "Input", 1);
    AddWidget(path, "Controller", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "Controller Configuration", WIDGET_WINDOW_BUTTON)
        .CVar(CVAR_CONTROLLER_CONFIGURATION_WINDOW_OPEN)
        .RaceDisable(false)
        .WindowName("Input Editor")
        .HideInSearch(true)
        .Options(WindowButtonOptions().Tooltip("Toggles the controller configuration window."));
}

void PortMenu::AddMenuAssets() {
    AddMenuEntry("Assets", CVAR_SETTING("Menu.AssetsSidebarSection"));

    WidgetPath path = { "Assets", "Extraction", SECTION_COLUMN_1 };
    AddSidebarEntry("Assets", "Extraction", 1);
    AddWidget(path, "Extraction", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "BattleShip.o2r is generated from your ROM and loaded on launch.", WIDGET_TEXT);
    AddWidget(path, "Schedule Re-extract", WIDGET_BUTTON)
        .RaceDisable(false)
        .Callback([this](WidgetInfo&) {
            const auto path = Ship::Context::GetPathRelativeToAppDirectory("BattleShip.o2r");
            std::error_code ec;
            fs::remove(path, ec);
            mShowReextractMessage = true;
        })
        .Options(ButtonOptions().Tooltip("Deletes BattleShip.o2r so it is regenerated from the ROM on next launch.")
                     .Size(Sizes::Inline));

    path.sidebarName = "Paths";
    AddSidebarEntry("Assets", "Paths", 1);
    AddWidget(path, "Runtime Paths", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, fmt::format("App directory: {}", Ship::Context::GetPathRelativeToAppDirectory("")), WIDGET_TEXT);
    AddWidget(path, fmt::format("Main archive: {}", Ship::Context::GetPathRelativeToAppDirectory("BattleShip.o2r")),
              WIDGET_TEXT);
}

void PortMenu::AddMenuAbout() {
    AddMenuEntry("About", CVAR_SETTING("Menu.AboutSidebarSection"));

    WidgetPath path = { "About", "Build", SECTION_COLUMN_1 };
    AddSidebarEntry("About", "Build", 1);
    AddWidget(path, "Build", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "BattleShip", WIDGET_TEXT);
    AddWidget(path, "Built from the ssb-decomp-re decompilation", WIDGET_TEXT);
    AddWidget(path, "Powered by libultraship", WIDGET_TEXT);
}

void PortMenu::AddMenuElements() {
    AddMenuSettings();
    AddMenuWindows();
    AddMenuAssets();
    AddMenuAbout();

    if (CVarGetInteger(CVAR_SETTING("Menu.SidebarSearch"), 0)) {
        InsertSidebarSearch();
    }

    for (auto& initFunc : MenuInit::GetInitFuncs()) {
        initFunc();
    }

    mMenuElementsInitialized = true;
}

void PortMenu::InitElement() {
    if (!mMenuElementsInitialized) {
        AddMenuElements();
    }

    Ship::Menu::InitElement();
    disabledMap = {
        { DISABLE_FOR_NO_VSYNC,
          { [](disabledInfo&) -> bool { return !Ship::Context::GetInstance()->GetWindow()->CanDisableVerticalSync(); },
            "Disabling VSync not supported" } },
        { DISABLE_FOR_NO_WINDOWED_FULLSCREEN,
          { [](disabledInfo&) -> bool {
               return !Ship::Context::GetInstance()->GetWindow()->SupportsWindowedFullscreen();
           },
            "Windowed Fullscreen not supported" } },
        { DISABLE_FOR_NO_MULTI_VIEWPORT,
          { [](disabledInfo&) -> bool {
               return !Ship::Context::GetInstance()->GetWindow()->GetGui()->SupportsViewports();
           },
            "Multi-viewports not supported" } },
        { DISABLE_FOR_NOT_DIRECTX,
          { [](disabledInfo&) -> bool {
               return Ship::Context::GetInstance()->GetWindow()->GetWindowBackend() !=
                      Ship::WindowBackend::FAST3D_DXGI_DX11;
           },
            "Available only on DirectX" } },
        { DISABLE_FOR_DIRECTX,
          { [](disabledInfo&) -> bool {
               return Ship::Context::GetInstance()->GetWindow()->GetWindowBackend() ==
                      Ship::WindowBackend::FAST3D_DXGI_DX11;
           },
            "Not available on DirectX" } },
        { DISABLE_FOR_MATCH_REFRESH_RATE_ON,
          { [](disabledInfo&) -> bool { return CVarGetInteger(CVAR_SETTING("MatchRefreshRate"), 0); },
            "Match Refresh Rate is enabled" } },
        { DISABLE_FOR_ADVANCED_RESOLUTION_ON,
          { [](disabledInfo&) -> bool { return CVarGetInteger(CVAR_PREFIX_ADVANCED_RESOLUTION ".Enabled", 0); },
            "Advanced Resolution enabled" } },
        { DISABLE_FOR_VERTICAL_RES_TOGGLE_ON,
          { [](disabledInfo&) -> bool {
               return CVarGetInteger(CVAR_PREFIX_ADVANCED_RESOLUTION ".VerticalResolutionToggle", 0);
           },
            "Vertical Resolution Toggle enabled" } },
        { DISABLE_FOR_LOW_RES_MODE_ON,
          { [](disabledInfo&) -> bool { return CVarGetInteger(CVAR_LOW_RES_MODE, 0); }, "N64 mode enabled" } },
        { DISABLE_FOR_ADVANCED_RESOLUTION_OFF,
          { [](disabledInfo&) -> bool { return !CVarGetInteger(CVAR_PREFIX_ADVANCED_RESOLUTION ".Enabled", 0); },
            "Advanced Resolution is disabled" } },
        { DISABLE_FOR_VERTICAL_RESOLUTION_OFF,
          { [](disabledInfo&) -> bool {
               return !CVarGetInteger(CVAR_PREFIX_ADVANCED_RESOLUTION ".VerticalResolutionToggle", 0);
           },
            "Vertical Resolution Toggle is off" } },
    };
}

void PortMenu::UpdateElement() {
    Ship::Menu::UpdateElement();
}

void PortMenu::Draw() {
    Ship::Menu::Draw();
}

void PortMenu::DrawElement() {
    if (mMenuElementsInitialized) {
        Ship::Menu::DrawElement();
    }

    // Re-extract confirmation: render as an in-context ImGui modal rather
    // than calling SDL_ShowSimpleMessageBox, which would nest a blocking
    // native modal (NSAlert / Win32 MessageBox) inside the in-progress
    // ImGui frame and the GPU command encode for that frame. ImGui's own
    // popup runs in the same context with no thread/native-modal nesting.
    if (mShowReextractMessage) {
        ImGui::OpenPopup("Re-extract scheduled");
        mShowReextractMessage = false;
    }
    const ImVec2 viewportCenter = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(viewportCenter, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal("Re-extract scheduled", nullptr,
                               ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings)) {
        ImGui::TextWrapped(
            "BattleShip.o2r will be regenerated from your ROM the next time the game launches.");
        ImGui::Spacing();
        if (ImGui::Button("OK", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

} // namespace ssb64
