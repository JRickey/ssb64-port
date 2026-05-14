// Libretro shader-pack downloader for the post-process picker.
//
// Implementation pattern mirrors port/enhancements/Updater.cpp:
//   * curl is shelled out to (every platform we ship on has a usable
//     curl in PATH; Windows 10+ ships one in System32);
//   * a single std::thread carries the long-running work;
//   * UI state is exposed through atomics + a mutex-guarded string,
//     so the menu can poll once a frame without locks.
//
// Source: github.com/libretro/glsl-shaders. We only consume the single
// authored GLSL files that the LUS PostProcessGlslNormalizer already
// knows how to load (one file containing both `#ifdef VERTEX` and
// `#ifdef FRAGMENT` blocks). Multi-pass `.glslp` presets are out of
// scope for this Phase 1 downloader — they reference sibling shader
// files we'd otherwise have to chase and re-flatten; revisit once the
// downloader earns a tree-preserving extraction layout.
#include "enhancements.h"
#include "port_log.h"

#include <atomic>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <set>
#include <sstream>
#include <string>
#include <thread>

#include <spdlog/spdlog.h>

#include <nlohmann/json.hpp>
#include <ship/Context.h>

#include <fast/postprocess/PostProcessGlslNormalizer.h>
#include <fast/postprocess/PostProcessTranspiler.h>
#include <fast/postprocess/PostProcessTypes.h>

namespace ssb64 {
namespace enhancements {

namespace {

std::atomic<bool> s_inProgress{false};
std::atomic<bool> s_complete{false};
std::atomic<int>  s_installedCount{0};
std::atomic<int>  s_skippedUnsupported{0};

std::mutex  s_statusMutex;
std::string s_status;

void SetStatus(std::string s) {
    std::lock_guard<std::mutex> lock(s_statusMutex);
    s_status = std::move(s);
}

// Stem-to-label heuristic. Libretro's filename convention is already
// reasonable ("crt-hyllian-curvature-glow" → "CRT — Hyllian Curvature
// Glow") so we just title-case the components.
std::string DeriveLabel(const std::string& stem) {
    std::string label;
    bool startWord = true;
    bool seenDashAfterCrt = false;
    for (size_t i = 0; i < stem.size(); ++i) {
        char c = stem[i];
        if (c == '-' || c == '_') {
            // The first "crt-" gets an em dash separator for readability:
            // "crt-hyllian" → "CRT — Hyllian". Subsequent dashes are spaces.
            if (!seenDashAfterCrt && (label == "Crt" || label == "CRT")) {
                label = "CRT — ";
                seenDashAfterCrt = true;
            } else {
                label += ' ';
            }
            startWord = true;
            continue;
        }
        if (startWord) {
            label += static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
            startWord = false;
        } else {
            label += c;
        }
    }
    return label;
}

// Read a candidate shader file into memory. Returns the empty string
// on read failure — caller treats that as "not installable" without
// having to distinguish open-failure from unreadable.
std::string ReadCandidateShader(const std::filesystem::path& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in.is_open()) {
        return std::string();
    }
    std::ostringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

// A libretro single-file shader is recognizable by the VS/FS-half
// preprocessor guards. The PostProcessGlslNormalizer relies on these
// to split the file into VS and FS sources. Used as a cheap pre-
// filter so the transpile-validate step (which spins up glslang +
// SPIRV-Cross) only runs on plausible candidates.
bool LooksLikeSingleFileShader(const std::string& text) {
    const bool hasVertex   = text.find("#ifdef VERTEX") != std::string::npos ||
                             text.find("defined(VERTEX)") != std::string::npos;
    const bool hasFragment = text.find("#ifdef FRAGMENT") != std::string::npos ||
                             text.find("defined(FRAGMENT)") != std::string::npos;
    return hasVertex && hasFragment;
}

// Run the same normalize + transpile pipeline the in-game picker
// would use, returning true iff the shader compiles end-to-end on
// this LUS build. Anything that returns false here would also fail
// at runtime — installing it just clutters the picker with broken
// entries — so the downloader uses this as the authoritative
// install gate. Self-maintaining: when the normalizer improves
// (e.g. handles a new VS-varying shape), the next download
// automatically installs more shaders without code changes here.
bool TranspilesUnderCurrentNormalizer(const std::string& source) {
    const std::string normalized = Fast::NormalizeUserGlsl(source);
    Fast::PostProcessSource src;
    src.glsl = normalized;
    std::string err;
    return Fast::PostProcessTranspiler::SynthesizeMissing(src, err);
}

// Run a subprocess and discard its stdout. Returns true on exit code
// 0. We discard stdout/stderr because we shell out with the silent
// flags (`-s` on curl, `-q` on unzip) — but the FILE* still needs to
// be drained so the child doesn't block on a full pipe.
bool RunSilent(const std::string& cmd) {
#if defined(_WIN32)
    FILE* pipe = _popen(cmd.c_str(), "r");
#else
    FILE* pipe = popen(cmd.c_str(), "r");
#endif
    if (!pipe) {
        return false;
    }
    char buf[256];
    while (fgets(buf, sizeof(buf), pipe) != nullptr) {
        // discard
    }
#if defined(_WIN32)
    int rc = _pclose(pipe);
#else
    int rc = pclose(pipe);
#endif
    return rc == 0;
}

// Format a filesystem path for inclusion in a shell command line.
// `generic_string()` collapses Windows backslashes to forward slashes,
// which every shell-out tool we care about (curl, tar, unzip) accepts
// for file arguments and which avoids the well-known cmd.exe pitfall
// where a path ending in a single backslash escapes the closing quote
// (`"C:\foo\"` parses as one literal-backslash-then-quote-end).
std::string ShellQuote(const std::filesystem::path& p) {
    return "\"" + p.generic_string() + "\"";
}

// Shell out — every platform we ship has curl on $PATH (macOS,
// modern Windows, every desktop Linux distro), and the Updater
// already depends on it.
bool RunCurlDownload(const std::string& url, const std::filesystem::path& destPath) {
    std::string cmd = "curl -fLs -o " + ShellQuote(destPath) + " \"" + url + "\"";
    return RunSilent(cmd);
}

// We unzip to a scratch directory rather than streaming with libzip
// to keep this downloader's dependency surface flat (libultraship
// links libzip privately and re-plumbing it through ssb64.cmake is
// disproportionate to the once-per-install runtime cost). Per-platform
// archive tool:
//   - macOS / Linux: `unzip` is universal. We fall back to `bsdtar
//     -xf` if unzip is missing — bsdtar is libarchive-backed and
//     handles zip natively (shipped by default on macOS, available
//     via `libarchive-tools` on Debian, in `bsdtar` on Fedora/Arch).
//   - Windows: `tar -xf` understands zip on Win10+; matches the
//     existing Updater extraction pattern.
bool RunExtract(const std::filesystem::path& zipPath, const std::filesystem::path& destDir) {
    const std::string q_zip = ShellQuote(zipPath);
    const std::string q_dest = ShellQuote(destDir);
#if defined(_WIN32)
    return RunSilent("tar -xf " + q_zip + " -C " + q_dest);
#else
    if (RunSilent("unzip -q -o " + q_zip + " -d " + q_dest)) {
        return true;
    }
    // Try bsdtar/tar as a fallback — `tar -xf foo.zip -C dir` works
    // when tar links against libarchive (default on macOS, common on
    // modern Linux).
    return RunSilent("tar -xf " + q_zip + " -C " + q_dest);
#endif
}

void WriteSidecar(const std::filesystem::path& glslPath, const std::string& label) {
    nlohmann::json json;
    json["compat"]  = "native";
    json["label"]   = label;
    // The libretro/glsl-shaders repo is GPL'd. Recording it here so the
    // sidecar tooltip can show provenance without us having to embed
    // per-file LICENSE files.
    json["license"] = "GPL-3.0-or-later (libretro/glsl-shaders)";

    std::filesystem::path sidecarPath = glslPath;
    sidecarPath.replace_extension(".lus.json");

    std::ofstream out(sidecarPath, std::ios::binary | std::ios::trunc);
    if (out.is_open()) {
        out << json.dump(2);
    }
}

// True if `stem` looks like a multi-pass component (`crt-foo-pass0`,
// `tvout-tweaks-pass-1`, `interlaced_pass2`). These are partial shaders
// driven by a .glslp wrapper — loading one as a standalone single-pass
// shader either produces garbage or silently no-ops because the chain
// is missing the sibling passes. Filename heuristic is cheap and
// reliable across the libretro repo conventions.
bool LooksLikeMultiPassComponent(const std::string& stem) {
    auto isDigit = [](char c) { return c >= '0' && c <= '9'; };
    const std::string lower = [&] {
        std::string s = stem;
        for (char& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        return s;
    }();
    // Match suffixes `-pass<N>`, `_pass<N>`, `-pass-<N>`, `_pass-<N>`.
    const std::string passMarkers[] = {"-pass", "_pass"};
    for (const std::string& marker : passMarkers) {
        size_t pos = lower.rfind(marker);
        while (pos != std::string::npos) {
            size_t end = pos + marker.size();
            if (end < lower.size() && lower[end] == '-') {
                ++end;
            }
            // Need at least one digit after the marker to consider this
            // a pass-component suffix; "rfind" without that check would
            // also fire on legitimate names like "crt-passthrough".
            if (end < lower.size() && isDigit(lower[end])) {
                return true;
            }
            if (pos == 0) break;
            pos = lower.rfind(marker, pos - 1);
        }
    }
    return false;
}

// Walk the extracted libretro tree and install the canonical single-
// file shaders for the categories most likely to be useful as a
// post-process on an N64 game: CRT, scanlines, NTSC, handheld. Each
// category has a `<top>/shaders/` directory of files; some authors
// stash variants in a one-level subfolder (`hyllian/`, `mame_hlsl/`).
// We recurse one level into those subfolders but no deeper — deeper
// nesting is usually multi-pass component territory that a Phase 1
// flat-file downloader can't safely install.
//
// Stems are flattened so each installed file lives directly under
// `<outRoot>/`. Collisions get the parent dir prefixed, which keeps
// the picker entries readable (the picker only descends one level
// from the user-shaders root, so we cannot preserve the libretro
// directory tree here).
int CollectShaders(const std::filesystem::path& extractedRoot,
                   const std::filesystem::path& outRoot) {
    int installed = 0;
    std::set<std::string> usedStems;

    std::error_code ec;
    std::filesystem::create_directories(outRoot, ec);

    // Resolve `<extractedRoot>/glsl-shaders-master/` — libretro's zip
    // wraps everything in that top dir. If the zip layout ever changes
    // we fall back to scanning the extractedRoot itself.
    std::filesystem::path repoRoot = extractedRoot / "glsl-shaders-master";
    std::error_code existsEc;
    if (!std::filesystem::is_directory(repoRoot, existsEc)) {
        repoRoot = extractedRoot;
    }

    // Categories whose `shaders/` subdir holds the kind of effects an
    // N64 game wants overlaid. `cgp` / `bezel` / `xbr` etc. exist but
    // are either multi-pass-only or scaler chains that don't fit a
    // single-file post-process; skipping them keeps the picker
    // manageable.
    const char* kCategories[] = {
        "crt", "scanlines", "ntsc", "handheld", "dithering",
    };

    auto installOne = [&](const std::filesystem::path& path,
                          const std::string& categoryHint) {
        std::string stem = path.stem().string();
        if (stem.empty()) {
            return;
        }
        if (LooksLikeMultiPassComponent(stem)) {
            return;
        }
        // Read once, then run cheap-then-expensive filters against the
        // same buffer so we never touch the file twice or invoke
        // glslang on obvious junk.
        const std::string source = ReadCandidateShader(path);
        if (source.empty() || !LooksLikeSingleFileShader(source)) {
            return;
        }
        // Authoritative gate: if the in-process transpiler can't lower
        // this shader to the backend languages, the picker can't
        // either. Drop the file so the user never sees an entry that
        // would just fail to load. A single counter is bumped so the
        // status line can attribute the gap.
        if (!TranspilesUnderCurrentNormalizer(source)) {
            s_skippedUnsupported.fetch_add(1);
            return;
        }
        if (usedStems.count(stem) != 0) {
            const std::string parent = path.parent_path().filename().string();
            if (!parent.empty() && parent != "shaders") {
                stem = parent + "-" + stem;
            } else if (!categoryHint.empty()) {
                stem = categoryHint + "-" + stem;
            }
        }
        if (usedStems.count(stem) != 0) {
            int n = 2;
            const std::string base = stem;
            while (usedStems.count(stem) != 0) {
                stem = base + "-" + std::to_string(n++);
            }
        }
        usedStems.insert(stem);

        const std::filesystem::path destPath = outRoot / (stem + ".glsl");
        std::error_code copyEc;
        std::filesystem::copy_file(path, destPath,
                                   std::filesystem::copy_options::overwrite_existing, copyEc);
        if (copyEc) {
            SPDLOG_WARN("Shader pack: failed to copy {} -> {}: {}", path.string(),
                        destPath.string(), copyEc.message());
            return;
        }
        WriteSidecar(destPath, DeriveLabel(stem));
        ++installed;
    };

    for (const char* category : kCategories) {
        const std::filesystem::path catShaders = repoRoot / category / "shaders";
        std::error_code catEc;
        if (!std::filesystem::is_directory(catShaders, catEc)) {
            continue;
        }
        // Top-level files in `<category>/shaders/`.
        for (const auto& entry : std::filesystem::directory_iterator(catShaders, catEc)) {
            if (catEc) break;
            std::error_code entEc;
            if (entry.is_regular_file(entEc) && entry.path().extension() == ".glsl") {
                installOne(entry.path(), category);
            }
        }
        // One-level subfolders (`hyllian/`, etc.). Deeper nesting is
        // skipped — those are usually multi-pass-only territory.
        for (const auto& entry : std::filesystem::directory_iterator(catShaders, catEc)) {
            if (catEc) break;
            std::error_code entEc;
            if (!entry.is_directory(entEc)) {
                continue;
            }
            const std::string subName = entry.path().filename().string();
            if (!subName.empty() && subName[0] == '_') {
                continue;
            }
            for (const auto& sub : std::filesystem::directory_iterator(entry.path(), catEc)) {
                if (catEc) break;
                std::error_code subEc;
                if (sub.is_regular_file(subEc) && sub.path().extension() == ".glsl") {
                    installOne(sub.path(), category);
                }
            }
        }
    }
    return installed;
}

} // namespace

bool IsShaderPackDownloadInProgress() { return s_inProgress.load(); }
bool IsShaderPackDownloadComplete()   { return s_complete.load(); }
int  GetShaderPackInstalledCount()    { return s_installedCount.load(); }

std::string GetShaderPackStatus() {
    std::lock_guard<std::mutex> lock(s_statusMutex);
    return s_status;
}

void DownloadLibretroShaderPackAsync() {
    if (s_inProgress.load()) {
        return;
    }
    s_inProgress.store(true);
    s_complete.store(false);
    s_installedCount.store(0);
    s_skippedUnsupported.store(0);
    SetStatus("Starting download...");

    std::thread([]() {
        // Resolve <userdata>/shaders/libretro/ via the LUS context so
        // we stay in lockstep with the rest of the post-process loader.
        std::filesystem::path outRoot;
        try {
            outRoot = std::filesystem::path(
                Ship::Context::GetPathRelativeToAppDirectory("shaders")) / "libretro";
        } catch (...) {
            SetStatus("Error: LUS context unavailable");
            s_inProgress.store(false);
            return;
        }

        const std::filesystem::path tempDir = std::filesystem::temp_directory_path() /
                                              "battleship-shader-pack";
        const std::filesystem::path zipPath = tempDir / "glsl-shaders.zip";
        const std::filesystem::path extractDir = tempDir / "extract";

        std::error_code ec;
        std::filesystem::remove_all(tempDir, ec);
        std::filesystem::create_directories(extractDir, ec);
        if (ec) {
            SetStatus("Error: could not create temp dir");
            s_inProgress.store(false);
            return;
        }

        SetStatus("Downloading libretro/glsl-shaders...");
        const std::string url =
            "https://github.com/libretro/glsl-shaders/archive/refs/heads/master.zip";
        if (!RunCurlDownload(url, zipPath)) {
            SetStatus("Error: download failed");
            s_inProgress.store(false);
            return;
        }

        SetStatus("Extracting...");
        if (!RunExtract(zipPath, extractDir)) {
            SetStatus("Error: extract failed");
            s_inProgress.store(false);
            return;
        }

        SetStatus("Installing shaders...");
        // The libretro subdir is fully owned by the downloader — wipe
        // it before re-installing so we don't keep stale files from a
        // previous (looser-filter) run, and so removed entries don't
        // linger in the picker.
        std::filesystem::remove_all(outRoot, ec);
        const int installed = CollectShaders(extractDir, outRoot);
        s_installedCount.store(installed);

        // Best-effort cleanup; failure here just leaves files in /tmp.
        std::filesystem::remove_all(tempDir, ec);

        const int skipped = s_skippedUnsupported.load();
        if (installed > 0) {
            std::string msg = "Installed " + std::to_string(installed) + " shaders";
            if (skipped > 0) {
                msg += " (skipped " + std::to_string(skipped) +
                       " unsupported by this build)";
            }
            SetStatus(std::move(msg));
            s_complete.store(true);
        } else {
            SetStatus(skipped > 0
                          ? "No supported shaders (" + std::to_string(skipped) +
                                " candidates failed to transpile)"
                          : std::string("No compatible shaders found"));
        }
        s_inProgress.store(false);
    }).detach();
}

} // namespace enhancements
} // namespace ssb64
