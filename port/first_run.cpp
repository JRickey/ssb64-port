#include "first_run.h"
#include "native_dialog.h"
#include "port_log.h"

#include <libultraship/libultraship.h>
#include <fast/Fast3dWindow.h>
#include <ship/window/FileDropMgr.h>
#include <ship/window/gui/Gui.h>

#include <SDL2/SDL.h>
#include <imgui.h>

#include <array>
#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <functional>
#include <future>
#include <string>
#include <thread>
#include <vector>

namespace fs = std::filesystem;

namespace ssb64 {

namespace {

// Quote a path for the shell. Wraps in double quotes and escapes embedded
// double quotes. Sufficient for our internal-only use; not a general-purpose
// shell-escape.
std::string ShellQuote(const std::string& s) {
    std::string out;
    out.reserve(s.size() + 2);
    out += '"';
    for (char c : s) {
        if (c == '"' || c == '\\' || c == '$' || c == '`') {
            out += '\\';
        }
        out += c;
    }
    out += '"';
    return out;
}

// Search candidates in order; return the first that exists.
std::string FindExisting(const std::vector<std::string>& candidates) {
    for (const auto& c : candidates) {
        if (!c.empty() && fs::exists(c)) {
            return c;
        }
    }
    return {};
}

// Resolve the directory containing this process's executable.
//
// Ship::Context::GetAppBundlePath() on Linux + NON_PORTABLE returns
// the literal CMAKE_INSTALL_PREFIX (default /usr/local), which is
// wrong for AppImages (binary actually lives at
// /tmp/.mount_XYZ/usr/bin/) and for any non-prefix install. Use
// /proc/self/exe to get the real directory and fall back to the
// upstream API only if the readlink fails.
std::string RealAppBundlePath() {
#if defined(__linux__)
    std::error_code ec;
    fs::path exe = fs::read_symlink("/proc/self/exe", ec);
    if (!ec && !exe.empty()) {
        return exe.parent_path().string();
    }
#endif
    return Ship::Context::GetAppBundlePath();
}

// Locate the torch binary. Tries (in priority order):
//   1. Next to ssb64 binary (shipped layout — Win/Linux: same dir; macOS:
//      Contents/MacOS/torch).
//   2. macOS Resources dir (Contents/Resources/torch).
//   3. Dev build location (build/TorchExternal/src/TorchExternal-build/torch).
//   4. PATH lookup via the bare name (last-resort).
std::string FindTorchBinary() {
    const std::string app = RealAppBundlePath();
    std::vector<std::string> candidates = {
        app + "/torch",
        app + "/torch.exe",
#ifdef __APPLE__
        // macOS bundle: GetAppBundlePath returns .app/Contents/Resources;
        // the sidecar torch binary lives in .app/Contents/MacOS alongside
        // the main executable per Apple convention.
        app + "/../MacOS/torch",
#endif
        // dev: when ssb64 binary lives in <build>/, sibling dir holds torch
        app + "/TorchExternal/src/TorchExternal-build/torch",
        app + "/TorchExternal/src/TorchExternal-build/Release/torch.exe",
        app + "/TorchExternal/src/TorchExternal-build/Debug/torch.exe",
    };
    auto hit = FindExisting(candidates);
    if (!hit.empty()) return hit;

    // Last resort: PATH lookup. Returning the bare name lets std::system rely
    // on the caller's PATH; if torch isn't installed globally this just fails
    // cleanly when we exec it.
    return "torch";
}

// Locate the directory that contains config.yml (the torch project config —
// enumerates all yamls/us/*.yml extraction recipes). Torch must be invoked
// with this directory as cwd.
std::string FindTorchConfigDir() {
    const std::string app = RealAppBundlePath();
    std::vector<std::string> candidates = {
        app,                                  // macOS Resources, Windows exe-dir
        app + "/../share/BattleShip",         // Linux AppImage usr/bin → usr/share/BattleShip;
                                              // /usr install bin → share/BattleShip
        app + "/..",                          // dev: build/ssb64 → project root
        ".",                                  // last-ditch cwd
    };
    for (const auto& dir : candidates) {
        if (fs::exists(dir + "/config.yml") && fs::exists(dir + "/yamls/us")) {
            return dir;
        }
    }
    return {};
}

// Locate a ROM file. Searches in priority order:
//   1. App data dir   — where the user is told to drop their ROM.
//   2. Bundle dir     — sometimes shipped together for testing.
//   3. cwd / project  — dev workflow.
std::string FindBaseRom() {
    const std::string appData = Ship::Context::GetAppDirectoryPath();
    const std::string bundle = RealAppBundlePath();
    std::vector<std::string> candidates;
    for (const auto& base : {appData, bundle, bundle + "/..", std::string(".")}) {
        for (const char* ext : {"z64", "n64", "v64"}) {
            candidates.push_back(base + "/baserom.us." + ext);
        }
    }
    return FindExisting(candidates);
}

} // namespace

bool ExtractAssetsIfNeeded(const std::string& target_o2r_path, bool silent) {
    if (fs::exists(target_o2r_path)) {
        return true;
    }

    port_log("first_run: %s missing — running asset extraction\n",
             target_o2r_path.c_str());

    const std::string rom = FindBaseRom();
    if (rom.empty()) {
        const std::string appData = Ship::Context::GetAppDirectoryPath();
        port_log("first_run: ERROR no ROM found.\n"
                 "  Drop a baserom.us.{z64,n64,v64} into %s\n",
                 appData.c_str());
        const std::string msg =
            "Asset extraction needs your Super Smash Bros. NTSC-U v1.0 ROM.\n\n"
            "Place a baserom.us.z64 (or .n64 / .v64) into:\n  " + appData +
            "\n\nThen launch the game again.";
        if (!silent) {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                                     "ROM not found", msg.c_str(), nullptr);
        }
        return false;
    }
    port_log("first_run: using ROM %s\n", rom.c_str());

    const std::string torch = FindTorchBinary();
    port_log("first_run: torch binary -> %s\n", torch.c_str());

    const std::string cfgDir = FindTorchConfigDir();
    if (cfgDir.empty()) {
        port_log("first_run: ERROR could not locate torch config.yml + yamls/us/\n");
        return false;
    }
    port_log("first_run: torch config dir -> %s\n", cfgDir.c_str());

    // Stage a writable copy of config.yml + yamls/ in the app-data dir
    // and run torch from there. The bundled config dir is read-only in
    // every shipped layout (AppImage squashfs, macOS .app/Contents/...,
    // /usr/local install). Torch reads config from cwd and writes
    // BattleShip.o2r + a version file + side-artifact dirs (src/assets,
    // include/assets, modding) into cwd. On a read-only cwd those
    // writes fail mid-pipeline, but torch reports a successful exit
    // anyway — surfaces as "torch reported success but BattleShip.o2r
    // is missing." Staging into the app-data dir gives torch a
    // writable scratch space and lands BattleShip.o2r right next to
    // the target path so we can rename within the same filesystem.
    fs::create_directories(fs::path(target_o2r_path).parent_path());
    const fs::path workDir =
        fs::path(target_o2r_path).parent_path() / "torch-work";
    std::error_code ec;
    fs::remove_all(workDir, ec);
    ec.clear();
    fs::create_directories(workDir, ec);
    if (ec) {
        port_log("first_run: ERROR could not create %s: %s\n",
                 workDir.string().c_str(), ec.message().c_str());
        return false;
    }
    fs::copy_file(fs::path(cfgDir) / "config.yml",
                  workDir / "config.yml",
                  fs::copy_options::overwrite_existing, ec);
    if (ec) {
        port_log("first_run: ERROR could not stage config.yml: %s\n",
                 ec.message().c_str());
        return false;
    }
    fs::copy(fs::path(cfgDir) / "yamls", workDir / "yamls",
             fs::copy_options::recursive |
                 fs::copy_options::overwrite_existing,
             ec);
    if (ec) {
        port_log("first_run: ERROR could not stage yamls/: %s\n",
                 ec.message().c_str());
        return false;
    }
    const std::string runDir = workDir.string();
    port_log("first_run: torch work dir -> %s\n", runDir.c_str());

    // Compose: cd "<runDir>" && "<torch>" o2r "<rom>"
    //
    // Append the ssb64.log path to the child's stdio. Our parent process
    // has spdlog and assorted file descriptors open; if the child writes
    // to a closed/redirected stdout it can be killed by SIGPIPE (status
    // 141). Routing both streams to ssb64.log keeps the extraction trace
    // diagnosable and isolates us from stdio inheritance quirks.
    const std::string logPath = Ship::Context::GetPathRelativeToAppDirectory(
        "logs/torch-extract.log");
    fs::create_directories(fs::path(logPath).parent_path());
    std::string cmd;
#ifdef _WIN32
    cmd = "cmd /C \"cd /D " + ShellQuote(runDir) + " && " +
          ShellQuote(torch) + " o2r " + ShellQuote(rom) +
          " > " + ShellQuote(logPath) + " 2>&1\"";
#else
    cmd = "cd " + ShellQuote(runDir) + " && " +
          ShellQuote(torch) + " o2r " + ShellQuote(rom) +
          " > " + ShellQuote(logPath) + " 2>&1";
#endif
    port_log("first_run: > %s\n", cmd.c_str());

    int rc = std::system(cmd.c_str());
    if (rc != 0) {
        port_log("first_run: ERROR torch exited with %d\n", rc);
        const std::string msg =
            "Torch failed to extract assets from your ROM.\n\n"
            "Check the extraction log for details:\n  " + logPath +
            "\n\nThe most common cause is a non-NTSC-U-v1.0 ROM. Verify your "
            "dump's SHA-1 matches a supported hash printed in that log.";
        if (!silent) {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                                     "Asset extraction failed", msg.c_str(),
                                     nullptr);
        }
        return false;
    }

    const std::string emitted = (workDir / "BattleShip.o2r").string();
    if (!fs::exists(emitted)) {
        port_log("first_run: ERROR torch reported success but %s is missing\n",
                 emitted.c_str());
        return false;
    }

    // Move into the target app-data path. The work dir lives under the
    // same parent as target_o2r_path so this is an intra-fs rename.
    fs::rename(emitted, target_o2r_path, ec);
    if (ec) {
        ec.clear();
        fs::copy_file(emitted, target_o2r_path,
                      fs::copy_options::overwrite_existing, ec);
        if (ec) {
            port_log("first_run: ERROR failed to install %s -> %s: %s\n",
                     emitted.c_str(), target_o2r_path.c_str(),
                     ec.message().c_str());
            return false;
        }
    }
    // Drop the staging dir (config.yml + yamls/ + any torch
    // side-artifacts). Best-effort — leaving it behind isn't fatal.
    fs::remove_all(workDir, ec);

    port_log("first_run: extracted BattleShip.o2r -> %s\n",
             target_o2r_path.c_str());
    return true;
}

namespace {

// Render one pre-gameloop GUI frame (no game commands). Mirrors the
// gui->StartDraw() / interpreter / gui->EndDraw() sequence used by
// Fast3dWindow::DrawAndRunGraphicsCommands but substitutes RunGuiOnly()
// for the game-render step.
//
// window->EndFrame() at the end is critical: RunGuiOnly's StartFrame
// acquires a drawable from the swap chain, but only EndFrame's
// SwapBuffers actually presents it. Skipping EndFrame leaks drawables
// into CAImageQueue (Metal) / DXGI's swap chain; after enough wizard
// frames the queue corrupts and the very next nextDrawable call (the
// game's first real frame) segfaults inside QuartzCore.
void DrawWizardFrame(const std::function<void()>& drawContents) {
    auto context = Ship::Context::GetInstance();
    auto window = context->GetWindow();
    auto gui = window->GetGui();

    window->HandleEvents();

    gui->StartDraw();
    // Mirror DrawAndRunGraphicsCommands: StartFrame populates
    // mGfxCurrentWindowDimensions from the actual window before
    // RunGuiOnly hands them to UpdateFramebufferParameters. Skipping it
    // worked on Metal (it tolerates 0×0 framebuffer params) but on
    // D3D11/OpenGL the backbuffer is created with zeroed dimensions and
    // the wizard renders as a black screen.
    window->StartFrame();
    drawContents();
    window->RunGuiOnly();
    gui->EndDraw();
    window->EndFrame();
}

} // namespace

bool RunFirstRunWizard(const std::string& target_o2r_path) {
    port_log("first_run: launching ImGui wizard\n");

    auto context = Ship::Context::GetInstance();
    auto window = context->GetWindow();
    if (!window || !window->GetGui()) {
        port_log("first_run: ERROR wizard requires Window+Gui already up\n");
        return false;
    }

    const std::string appData = Ship::Context::GetAppDirectoryPath();
    const std::string targetParent = fs::path(target_o2r_path).parent_path().string();

    // 256 char ImGui input buffer. Pre-fill with the conventional path the
    // user is most likely to try first; they can edit/replace freely.
    char romPath[1024] = {0};
    std::snprintf(romPath, sizeof(romPath), "%s/baserom.us.z64",
                  appData.c_str());

    enum class State { WaitingForRom, Extracting, Done, Cancelled };
    State state = State::WaitingForRom;
    std::string statusMsg;

    // Test hook: SSB64_WIZARD_AUTOCANCEL=N cancels after the Nth frame.
    // Lets CI / manual smoke tests exercise the cancel exit path without
    // having to drive the ImGui modal interactively.
    int autoCancelFrame = -1;
    if (const char* env = std::getenv("SSB64_WIZARD_AUTOCANCEL")) {
        autoCancelFrame = std::atoi(env);
    }
    int frameCount = 0;

    auto fdm = context->GetFileDropMgr();

    /* Background extraction is driven by a std::async future so the wizard
     * keeps rendering while torch runs (~2-3 seconds). The future is set
     * when the user clicks Extract; each frame we poll wait_for(0) to see
     * if it's done. While the future is pending, the modal renders an
     * indeterminate progress bar animated on frameCount. */
    std::future<bool> extractFuture;
    bool extractRunning = false;

    /* Register a "consume everything" drop handler for the duration of
     * the wizard.  FileDropMgr::CallHandlers fires synchronously from
     * the SDL_DROPFILE event in the SDL2 backend; if no handler claims
     * the drop (returns true), it falls through to
     *   gui->GetGameOverlay()->TextDrawNotification("Unsupported file
     *                                                dropped, ignoring")
     * which queues an overlay notification.  GameOverlay's font hasn't
     * been initialized yet (Gui::Init / GameOverlay::Init normally runs
     * during the first game frame), so the notification's later
     * ImGui::PushFont(nullptr) deref crashes once the user clicks
     * Extract and another frame draws.
     *
     * We register and unregister around the wizard so this only changes
     * behavior here — outside the wizard, drops still surface the
     * standard "unsupported" toast. */
    using DropFunc = bool (*)(char*);
    DropFunc consumer = [](char*) -> bool { return true; };
    if (fdm) fdm->RegisterDropHandler(consumer);

    while (state != State::Done && state != State::Cancelled) {
        if (!window->IsRunning()) {
            state = State::Cancelled;
            break;
        }
        if (autoCancelFrame >= 0 && frameCount++ >= autoCancelFrame) {
            port_log("first_run: SSB64_WIZARD_AUTOCANCEL fired at frame %d\n",
                     frameCount);
            state = State::Cancelled;
            break;
        }

        // Drag-drop integration: if the user dropped a file on the window
        // since last frame, paste its path into the input. SDL already
        // routes SDL_DROPFILE events to FileDropMgr via Fast3dWindow's
        // event handler; we just consume them here.
        if (fdm && fdm->FileDropped()) {
            if (const char* p = fdm->GetDroppedFile()) {
                std::snprintf(romPath, sizeof(romPath), "%s", p);
                statusMsg.clear();
                port_log("first_run: drag-drop -> %s\n", p);
            }
            fdm->ClearDroppedFile();
        }

        DrawWizardFrame([&] {
            ImGui::OpenPopup("First-run setup");

            const ImVec2 viewportCenter =
                ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(viewportCenter, ImGuiCond_Always,
                                    ImVec2(0.5f, 0.5f));
            ImGui::SetNextWindowSize(ImVec2(620, 0));

            if (ImGui::BeginPopupModal("First-run setup", nullptr,
                                       ImGuiWindowFlags_AlwaysAutoResize |
                                       ImGuiWindowFlags_NoMove |
                                       ImGuiWindowFlags_NoSavedSettings)) {
                ImGui::TextWrapped(
                    "BattleShip needs to extract assets from your "
                    "Nintendo 64 ROM before it can launch.");
                ImGui::Spacing();
                ImGui::TextWrapped(
                    "Required: a Super Smash Bros. (NTSC-U v1.0) dump in "
                    ".z64, .n64, or .v64 format.");
                ImGui::Separator();

                ImGui::Text("ROM path:");
                // InputText takes most of the row, Browse takes the right
                // edge. -100 reserves room for a 90 px button + padding.
                ImGui::SetNextItemWidth(-100);
                ImGui::InputText("##rompath", romPath, sizeof(romPath));
                ImGui::SameLine();
                if (ImGui::Button("Browse...", ImVec2(90, 0))) {
                    std::string picked = ssb64::OpenFileDialog(
                        "Select your Super Smash Bros. ROM",
                        {"z64", "n64", "v64"});
                    if (!picked.empty()) {
                        std::snprintf(romPath, sizeof(romPath), "%s",
                                      picked.c_str());
                        statusMsg.clear();
                        port_log("first_run: native picker -> %s\n",
                                 picked.c_str());
                    }
                }

                ImGui::TextDisabled(
                    "Tip: drag a .z64/.n64/.v64 onto the window, or click");
                ImGui::TextDisabled(
                    "     Browse... and pick your ROM dump.");
                ImGui::Spacing();

                if (state == State::Extracting) {
                    ImGui::TextColored(ImVec4(1.0f, 0.85f, 0.2f, 1.0f),
                                       "Extracting assets — please wait...");
                    // Indeterminate progress bar. Torch doesn't emit
                    // progress callbacks, so we animate a marquee-style
                    // fraction that loops every ~2 s. ImGui doesn't
                    // ship a marquee helper; pass a fraction in [0,1]
                    // and an empty overlay string with a sliding
                    // SetNextItemWidth to fake it.
                    const float t = (frameCount % 120) / 120.0f;
                    ImGui::ProgressBar(t, ImVec2(-1, 0), "Running torch...");
                } else if (!statusMsg.empty()) {
                    ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f),
                                       "%s", statusMsg.c_str());
                }

                ImGui::Spacing();

                const bool busy = (state == State::Extracting);
                ImGui::BeginDisabled(busy);

                if (ImGui::Button("Extract", ImVec2(120, 0))) {
                    if (!fs::exists(romPath)) {
                        statusMsg = "ROM not found at that path.";
                    } else {
                        // Stage the ROM into appData so FindBaseRom picks
                        // it up, then run extraction on a background
                        // thread so the wizard can keep rendering frames
                        // (and the progress bar animates).
                        fs::create_directories(appData);
                        const std::string staged =
                            appData + "/baserom.us." +
                            fs::path(romPath).extension().string().substr(1);
                        std::error_code ec;
                        if (fs::path(romPath) != fs::path(staged)) {
                            fs::copy_file(romPath, staged,
                                          fs::copy_options::overwrite_existing,
                                          ec);
                        }
                        if (ec) {
                            statusMsg = "Could not stage ROM: " + ec.message();
                        } else {
                            state = State::Extracting;
                            statusMsg.clear();
                            // Capture target_o2r_path by value — the
                            // future may outlive any reference into
                            // RunFirstRunWizard's scope on shutdown.
                            std::string target = target_o2r_path;
                            extractFuture = std::async(
                                std::launch::async,
                                [target]() {
                                    return ExtractAssetsIfNeeded(
                                        target, /*silent=*/true);
                                });
                            extractRunning = true;
                        }
                    }
                }

                ImGui::SameLine();
                if (ImGui::Button("Quit", ImVec2(120, 0))) {
                    state = State::Cancelled;
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndDisabled();
                ImGui::EndPopup();
            }
        });

        // Poll the background extraction. wait_for(0) returns ready
        // immediately if the thread finished since last frame.
        if (extractRunning && extractFuture.valid()) {
            auto status =
                extractFuture.wait_for(std::chrono::milliseconds(0));
            if (status == std::future_status::ready) {
                bool ok = extractFuture.get();
                extractRunning = false;
                if (ok) {
                    state = State::Done;
                } else {
                    state = State::WaitingForRom;
                    statusMsg =
                        "Extraction failed — see logs/torch-extract.log.";
                }
            }
        }
    }
    // If we're exiting with a future still in flight (cancel during
    // extraction), block on it so the thread doesn't outlive the
    // future's stored state.
    if (extractFuture.valid()) {
        extractFuture.wait();
    }

    if (fdm) fdm->UnregisterDropHandler(consumer);

    if (state == State::Cancelled) {
        port_log("first_run: wizard cancelled by user\n");
        return false;
    }
    port_log("first_run: wizard completed successfully\n");
    return true;
}

} // namespace ssb64
