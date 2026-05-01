// android_torch_bridge.cpp — non-CLI entry points for Torch on Android.
//
// Torch's normal main() lives behind STANDALONE && !__EMSCRIPTEN__ in
// torch/src/main.cpp and uses CLI11 to drive the Companion class. On
// Android we ship Torch as a separate shared library (libtorch_runner.so)
// that the first-run Activity loads to extract a ROM the user picked
// via the Storage Access Framework.
//
// This file is only built on Android (gated in the root CMakeLists.txt).
// It deliberately avoids any SDL2 / libultraship deps — libtorch_runner.so
// must be loadable BEFORE SDLActivity is set up, otherwise SDL2's
// JNI_OnLoad would trip over the missing SDLActivity Java class.

#if defined(__ANDROID__)

#include "Companion.h"

#include <android/log.h>
#include <jni.h>

#include <cstdio>
#include <cstring>
#include <exception>
#include <filesystem>
#include <string>

#define LOG_TAG "ssb64.torch"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,  LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

extern "C" {

/**
 * torch_extract_o2r — produce BattleShip.o2r from a user-supplied ROM.
 *
 * @param rom_path    Absolute path to the user's baserom.us.{z64,n64,v64}.
 *                    Must be readable by the calling process. The first-run
 *                    Activity is expected to copy the SAF-picked content
 *                    into the app's filesDir before calling us, since
 *                    Torch's Cartridge::open() does plain fopen(2).
 * @param src_dir     Absolute path to the directory containing config.yml,
 *                    yamls/us/*.yml, and any other torch-config files.
 *                    On Android this is filesDir + "/torch_data/", which
 *                    the first-run flow extracts from APK assets.
 * @param dst_dir     Absolute path where Torch will write BattleShip.o2r.
 *                    Typically the app's externalFilesDir.
 *
 * @return  0 on success, non-zero on failure (see android logcat tag
 *          "ssb64.torch" for the failure reason).
 *
 * Thread-safety: Companion uses a singleton (Companion::Instance), so
 * concurrent calls are NOT supported. The first-run flow is single-shot
 * anyway; the ROM picker doesn't allow re-entry while extraction is
 * running.
 */
int torch_extract_o2r(const char *rom_path, const char *src_dir, const char *dst_dir) {
    if (rom_path == nullptr || src_dir == nullptr || dst_dir == nullptr) {
        LOGE("torch_extract_o2r: null argument (rom=%p src=%p dst=%p)",
             rom_path, src_dir, dst_dir);
        return -1;
    }

    LOGI("torch_extract_o2r: rom=%s src=%s dst=%s", rom_path, src_dir, dst_dir);

    try {
        // Defensive: confirm the ROM exists before Torch's exception path
        // takes over with a less-clear message.
        std::error_code ec;
        if (!std::filesystem::exists(rom_path, ec) || ec) {
            LOGE("torch_extract_o2r: ROM does not exist: %s (%s)",
                 rom_path, ec ? ec.message().c_str() : "missing");
            return -2;
        }

        Companion *instance = new Companion(
            std::filesystem::path(rom_path),
            ArchiveType::O2R,
            /*debug=*/false,
            std::string(src_dir),
            std::string(dst_dir)
        );
        Companion::Instance = instance;

        instance->Init(ExportType::Binary);

        LOGI("torch_extract_o2r: extraction completed");
        // Companion owns its lifetime via the singleton; matching the
        // pattern in main.cpp's o2r subcommand. Deleting here would
        // dangle Companion::Instance for any later use.
        return 0;
    } catch (const std::exception &e) {
        LOGE("torch_extract_o2r: std::exception: %s", e.what());
        return 1;
    } catch (...) {
        LOGE("torch_extract_o2r: non-std exception");
        return 2;
    }
}

/* ========================================================================= */
/*  JNI bridge — called from Java RomImporter on a background thread        */
/* ========================================================================= */

/**
 * Java-callable wrapper around torch_extract_o2r. All three jstrings are
 * Java-owned UTF-16 internally; we acquire UTF-8 views with
 * GetStringUTFChars and release them after the C call returns.
 *
 * Java signature:
 *   public static native int extractO2R(String rom, String src, String dst);
 */
JNIEXPORT jint JNICALL
Java_com_jrickey_battleship_RomImporter_extractO2R(
    JNIEnv *env, jclass /*clazz*/,
    jstring rom_jstr, jstring src_jstr, jstring dst_jstr) {

    if (rom_jstr == nullptr || src_jstr == nullptr || dst_jstr == nullptr) {
        LOGE("RomImporter.extractO2R: null jstring (rom=%p src=%p dst=%p)",
             rom_jstr, src_jstr, dst_jstr);
        return -10;
    }

    const char *rom = env->GetStringUTFChars(rom_jstr, nullptr);
    const char *src = env->GetStringUTFChars(src_jstr, nullptr);
    const char *dst = env->GetStringUTFChars(dst_jstr, nullptr);

    jint result;
    if (rom == nullptr || src == nullptr || dst == nullptr) {
        LOGE("RomImporter.extractO2R: GetStringUTFChars failed");
        result = -11;
    } else {
        result = (jint) torch_extract_o2r(rom, src, dst);
    }

    if (rom) env->ReleaseStringUTFChars(rom_jstr, rom);
    if (src) env->ReleaseStringUTFChars(src_jstr, src);
    if (dst) env->ReleaseStringUTFChars(dst_jstr, dst);

    return result;
}

} // extern "C"

#endif // __ANDROID__
