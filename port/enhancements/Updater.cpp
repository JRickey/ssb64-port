#include "enhancements.h"

#include "port_log.h"

#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <mutex>
#include <string>
#include <thread>

#include <nlohmann/json.hpp>

#if defined(__linux__)
#include <sys/stat.h>
#endif

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>
#endif

namespace ssb64 {
namespace enhancements {

namespace {

#if defined(_WIN32)
// Run a child process with stdout/stderr redirected and no console window.
// Required on Windows because _popen() / system() shell through cmd.exe;
// when called from a /SUBSYSTEM:WINDOWS binary (no parent console), the OS
// allocates a fresh visible console window for the child. The user sees that
// console flash up in the background — once at startup when the updater
// queries GitHub for tags, then again every time we shell out for anything.
//
// `onLine` receives each non-empty line of merged stdout/stderr as it
// arrives (split on \r or \n so curl's progress output also feeds through
// one update at a time). Returns the child's exit code, or -1 on launch
// failure.
int RunCaptureNoWindow(std::string cmd,
                       const std::function<void(const std::string&)>& onLine) {
    HANDLE readPipe = nullptr;
    HANDLE writePipe = nullptr;
    SECURITY_ATTRIBUTES sa{};
    sa.nLength = sizeof(sa);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = nullptr;
    if (!CreatePipe(&readPipe, &writePipe, &sa, 0)) return -1;
    // The read end stays in the parent; don't let the child inherit it.
    SetHandleInformation(readPipe, HANDLE_FLAG_INHERIT, 0);

    STARTUPINFOA si{};
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdOutput = writePipe;
    si.hStdError = writePipe;
    si.hStdInput = nullptr;

    PROCESS_INFORMATION pi{};
    BOOL ok = CreateProcessA(nullptr, cmd.data(), nullptr, nullptr, TRUE,
                             CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi);
    // Always close the parent's write handle; the child holds its own copy.
    // Without this, ReadFile blocks forever waiting for EOF.
    CloseHandle(writePipe);
    if (!ok) {
        CloseHandle(readPipe);
        return -1;
    }

    std::string line;
    char buf[512];
    DWORD got = 0;
    while (ReadFile(readPipe, buf, sizeof(buf), &got, nullptr) && got > 0) {
        for (DWORD i = 0; i < got; ++i) {
            char c = buf[i];
            if (c == '\r' || c == '\n') {
                if (!line.empty()) {
                    onLine(line);
                    line.clear();
                }
            } else {
                line += c;
            }
        }
    }
    if (!line.empty()) onLine(line);
    CloseHandle(readPipe);

    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD code = 0;
    GetExitCodeProcess(pi.hProcess, &code);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return static_cast<int>(code);
}
#endif

} // namespace

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
        const char* const kCurlTagsCmd =
            "curl -s -m 10 -H \"User-Agent: BattleShip-Updater\" "
            "https://api.github.com/repos/JRickey/BattleShip/tags";

        std::string response;
        #if defined(_WIN32)
        // CreateProcess path: keep the child fully headless. The response is
        // a single JSON blob, so just concatenate every captured line.
        RunCaptureNoWindow(kCurlTagsCmd, [&](const std::string& line) {
            response += line;
        });
        #else
        FILE* pipe = popen(kCurlTagsCmd, "r");
        if (!pipe) {
            s_isCheckingForUpdates.store(false);
            return;
        }
        char buffer[128];
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            response += buffer;
        }
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

        #elif defined(_WIN32)
        std::string tempZip = "update_temp.zip";
        std::string cmd = "curl -L -# -o \"" + tempZip + "\" \"" + url + "\" 2>&1";

        #elif defined(__APPLE__)
        std::string tempDmg = "/tmp/BattleShip_Update.dmg";
        std::string cmd = "curl -L -# -o \"" + tempDmg + "\" \"" + url + "\" 2>&1";
        #endif

        // Forward each line of curl's progress output to the UI status string.
        // curl -# emits "##... 12.5%" style progress; we strip everything but
        // digits and dots to land on the percentage.
        auto onProgressLine = [](const std::string& line) {
            if (line.find('%') == std::string::npos) return;
            std::string pctStr;
            for (char ch : line) {
                if ((ch >= '0' && ch <= '9') || ch == '.') {
                    pctStr += ch;
                }
            }
            if (!pctStr.empty()) {
                std::lock_guard<std::mutex> lock(s_stringMutex);
                s_downloadStatus = "Downloading... " + pctStr + "%";
            }
        };

        int exitCode;
        #if defined(_WIN32)
        // No console flash even though curl is a console subprocess.
        exitCode = RunCaptureNoWindow(cmd, onProgressLine);
        if (exitCode < 0) {
            {
                std::lock_guard<std::mutex> lock(s_stringMutex);
                s_downloadStatus = "Error: Failed to launch download.";
            }
            s_isDownloading.store(false);
            return;
        }
        #else
        FILE* pipe = popen(cmd.c_str(), "r");
        if (!pipe) {
            {
                std::lock_guard<std::mutex> lock(s_stringMutex);
                s_downloadStatus = "Error: Failed to launch download.";
            }
            s_isDownloading.store(false);
            return;
        }
        int c;
        std::string currentLine;
        while ((c = fgetc(pipe)) != EOF) {
            if (c == '\r' || c == '\n') {
                if (!currentLine.empty()) onProgressLine(currentLine);
                currentLine.clear();
            } else {
                currentLine += static_cast<char>(c);
            }
        }
        if (!currentLine.empty()) onProgressLine(currentLine);
        exitCode = pclose(pipe);
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
                // ShellExecuteA avoids the cmd.exe console flash that
                // system("start ...") would produce; the .bat is still
                // launched in its own console (the .bat associates with
                // cmd.exe through the shell, which is the desired visible
                // window for update-in-progress feedback).
                ShellExecuteA(nullptr, "open", "update_game.bat", nullptr,
                              nullptr, SW_SHOWNORMAL);
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
    // ShellExecute hands off to the default browser without first spawning
    // a cmd.exe console (which system("start ...") would do from a GUI app).
    ShellExecuteA(nullptr, "open", url, nullptr, nullptr, SW_SHOWNORMAL);
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

} // namespace enhancements
} // namespace ssb64
