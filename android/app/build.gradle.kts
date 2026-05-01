// app/build.gradle.kts — the BattleShip APK module.
//
// Wires AGP's externalNativeBuild to the existing repo-root CMakeLists.txt,
// which produces libmain.so + libSDL2.so for arm64-v8a. AGP scans the CMake
// output dir for SHARED libs and bundles them under jniLibs/<ABI>/ in the
// APK automatically.

plugins {
    id("com.android.application")
}

android {
    namespace = "com.jrickey.battleship"
    compileSdk = 34
    // NDK install is at /opt/homebrew/share/android-ndk via brew; AGP needs
    // ndkVersion to match the actual revision (read from source.properties)
    // or it errors out. r29 = 29.0.14206865.
    ndkVersion = "29.0.14206865"

    defaultConfig {
        applicationId = "com.jrickey.battleship"
        minSdk = 26          // matches CMake -DANDROID_PLATFORM=android-26
        targetSdk = 34       // Play Store requires >= 34 (since Aug 2024)
        versionCode = 1
        versionName = "0.1.0-spike"

        // Single ABI for now. arm64-v8a covers ~99% of in-use Android devices
        // and runs natively on the M-series emulator. Add x86_64 if you need
        // to test on Intel-host emulators.
        ndk {
            abiFilters += listOf("arm64-v8a")
        }

        externalNativeBuild {
            cmake {
                // Forwarded to CMake configure. -DUSE_AUTO_VCPKG=OFF stops
                // libultraship from trying to bootstrap vcpkg on Android.
                arguments += listOf(
                    "-DUSE_AUTO_VCPKG=OFF",
                    "-DANDROID_STL=c++_shared",
                )
                // Targets we explicitly want AGP to drive a build for.
                //   ssb64         — libmain.so (the Activity loads this)
                //   torch_runner  — libtorch_runner.so (first-run ROM
                //                   extractor, loaded standalone by Java
                //                   before SDLActivity is up)
                // Transitive deps (libultraship.a, libSDL2.so, libtinyxml2,
                // libtorch.a, etc.) get pulled in automatically — and only
                // SHARED libs end up in the APK.
                targets += listOf("ssb64", "torch_runner")
            }
        }
    }

    buildTypes {
        getByName("debug") {
            isJniDebuggable = true
            // Pure-native crashes need the unstripped .so to symbolicate;
            // packagingOptions below keeps debug symbols around in debug APKs.
            isMinifyEnabled = false
            applicationIdSuffix = ".debug"
            versionNameSuffix = "-debug"
        }
        getByName("release") {
            isMinifyEnabled = false   // No Java code worth shrinking yet
            isShrinkResources = false
            // R8 / shrinking can come in Phase 8 polish along with signing.
        }
    }

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_17
        targetCompatibility = JavaVersion.VERSION_17
    }

    externalNativeBuild {
        cmake {
            // Path is resolved relative to this build.gradle.kts:
            //   android/app/ → ../.. → repo root
            path = file("../../CMakeLists.txt")
            // Match the cmake_minimum_required in the root.
            version = "3.24.0+"
        }
    }

    packaging {
        // Keep our prebuilt SDL2's debug info in debug APKs so ndk-stack can
        // symbolicate native crashes. release APK strips by default.
        jniLibs {
            keepDebugSymbols += "**/*.so"
        }
        resources {
            // libultraship and SDL2 each ship a META-INF — collide harmlessly,
            // pick one.
            pickFirsts += listOf(
                "META-INF/LICENSE*",
                "META-INF/NOTICE*",
                "META-INF/AL2.0",
                "META-INF/LGPL2.1",
            )
        }
    }
}
