// app/build.gradle.kts — the BattleShip APK module.
//
// Wires AGP's externalNativeBuild to the existing repo-root CMakeLists.txt,
// which produces libmain.so + libSDL2.so for arm64-v8a. AGP scans the CMake
// output dir for SHARED libs and bundles them under jniLibs/<ABI>/ in the
// APK automatically.

plugins {
    id("com.android.application")
}

// Repo root = the outer git checkout, two levels up from the agent
// worktree's android/app/build.gradle.kts:
//   <repo>/.claude/worktrees/<slug>/android/app/build.gradle.kts
// Resolve relative to projectDir so it works from any worktree.
val repoRoot = rootProject.projectDir.resolve("..").canonicalFile

/** Resolve a generated asset by checking the desktop build dir first
 *  (where new-worktree.sh symlinks main-tree outputs into build/), then
 *  the repo root, then fail with a clear message. */
fun findGameArtifact(name: String): java.io.File {
    val candidates = listOf(
        repoRoot.resolve("build").resolve(name),
        repoRoot.resolve(name),
    )
    return candidates.firstOrNull { it.exists() }
        ?: error("Missing $name — looked in:\n" +
                 candidates.joinToString("\n") { "  $it" } +
                 "\nRun the desktop cmake build (targets: GenerateF3DO2R, " +
                 "GeneratePortO2R, ExtractAssets) before assembleDebug.")
}

// Stage f3d.o2r + ssb64.o2r + Torch's runtime config (config.yml + yamls/)
// into a generated dir that AGP merges into the APK's assets/.
//
// BattleShip.o2r is intentionally NOT bundled — it derives from Nintendo's
// ROM and the user supplies that themselves on first launch (Phase 4.4
// SAF picker → libtorch_runner.so → produces BattleShip.o2r in
// externalFilesDir).
val stagedAssetsDir = layout.buildDirectory.dir("generated/staged_assets")

val stageGameAssets = tasks.register<Copy>("stageGameAssets") {
    description = "Stage f3d.o2r / ssb64.o2r / config.yml / yamls into APK assets/"
    group = "android"
    into(stagedAssetsDir)

    // Resolved at task-config time so we fail before AGP even starts the
    // merge — avoids a half-built APK with missing assets.
    from(findGameArtifact("f3d.o2r"))
    from(findGameArtifact("ssb64.o2r"))
    from(repoRoot.resolve("config.yml"))
    from(repoRoot.resolve("yamls")) {
        into("yamls")
    }

    doFirst {
        require(repoRoot.resolve("config.yml").exists()) {
            "config.yml not found at $repoRoot — agent worktree should " +
            "always have this file from git"
        }
    }
}

// Run the staging task before AGP's merge step.
afterEvaluate {
    tasks.matching { it.name.startsWith("merge") && it.name.endsWith("Assets") }
         .configureEach { dependsOn(stageGameAssets) }
}

android {
    namespace = "com.jrickey.battleship"
    compileSdk = 34

    // BuildConfig.VERSION_CODE is referenced from AssetExtractor.java for
    // re-extraction sentinel comparison. AGP 8.x disables this by default.
    buildFeatures {
        buildConfig = true
    }
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

    // Stage host-built / source files into a generated assets dir, then
    // tell AGP to merge that dir into the APK assets/. The dir lives under
    // app/build/ so it gets cleaned with `./gradlew clean`.
    sourceSets {
        getByName("main") {
            assets.srcDirs("$buildDir/generated/staged_assets")
        }
    }

    packaging {
        // (sourceSets is configured above)
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

// Force a single Kotlin stdlib version. AndroidX 1.9.3's transitive
// kotlin-stdlib-jdk8 1.6.21 collides with AGP's bundled kotlin-stdlib
// 1.8.22 (the older split stdlib was merged in 1.8). Pin both halves
// to 1.8.22 so dexer doesn't see duplicate classes.
configurations.all {
    resolutionStrategy.eachDependency {
        if (requested.group == "org.jetbrains.kotlin"
            && requested.name.startsWith("kotlin-stdlib")) {
            useVersion("1.9.25")
        }
    }
}

dependencies {
    // BootActivity uses the AndroidX result-API (registerForActivityResult)
    // for the SAF ROM picker. ComponentActivity is the base class that
    // wires the result-callback machinery into the Activity lifecycle.
    implementation("androidx.activity:activity:1.9.3")
}
