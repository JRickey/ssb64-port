#include "enhancements.h"

#include "port_log.h"

#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <mutex>
#include <string>
#include <thread>

#include <nlohmann/json.hpp>

#if defined(__linux__)
#include <sys/stat.h>
#endif

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

} // namespace enhancements
} // namespace ssb64
