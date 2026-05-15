// Libretro shader-pack downloader for the post-process picker.
//
// Two-phase flow, both run on background threads:
//   1. FetchShaderPackCatalogAsync() — download zip, extract to a
//      tempdir, walk every candidate `.glsl`, run the normalize +
//      transpile pipeline against each, and publish the validated
//      subset to s_candidates. The tempdir is kept alive for the
//      install phase to copy from.
//   2. InstallSelectedShaderPackAsync(stems) — copy just the user-
//      picked entries from the tempdir into the user-data shaders
//      dir, write per-shader sidecars, then clean up the tempdir.
//
// CancelShaderPackFlow() returns to Idle from any phase and tears
// the tempdir down. The phase enum is what the UI keys off — the
// boolean queries kept for back-compat are derived from it.
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

#include <algorithm>
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
#include <utility>
#include <vector>

#include <spdlog/spdlog.h>

#include <nlohmann/json.hpp>
#include <ship/Context.h>

#include <fast/postprocess/PostProcessGlslNormalizer.h>
#include <fast/postprocess/PostProcessTranspiler.h>
#include <fast/postprocess/PostProcessTypes.h>

namespace ssb64 {
namespace enhancements {

namespace {

// The phase the flow is currently in. UI keys off this; status string
// is supplementary detail for the modal's loading line.
std::atomic<ShaderPackPhase> s_phase{ShaderPackPhase::Idle};
std::atomic<int>  s_installedCount{0};
std::atomic<int>  s_skippedUnsupported{0};

std::mutex  s_statusMutex;
std::string s_status;

// One internal candidate that survived the cheap filters + transpile
// gate. Kept under s_candidatesMutex; UI sees a sanitized snapshot
// (ShaderPackCandidate) without the source-side path / category.
struct PendingCandidate {
    std::filesystem::path sourcePath;
    std::string           finalStem;     // collision-resolved, used as install filename
    std::string           displayLabel;  // human-readable label shown in modal
};

std::mutex                     s_candidatesMutex;
std::vector<PendingCandidate>  s_candidates;

// Tempdir holding the extracted libretro tree between phases — the
// install thread copies from here. Set by the catalog phase, cleared
// by the install / cancel phases. Read under s_candidatesMutex
// (catalog + install are serialized via s_phase, but cancel can
// arrive concurrently).
std::filesystem::path          s_tempDir;

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

// Reject shaders that derive their output from libretro multipass-
// history bindings (`PassPrev<N>Texture`, numbered `Pass<N>Texture`).
// Our auto-bind preprocessor lets these compile by stubbing the
// sampler at a fresh slot, but the Phase 1 runtime never populates
// that slot, so the texture reads back zero — and shaders that key
// their final color matrix off the history binding (e.g.
// crt-easymode-halation samples PassPrev4Texture five times to
// build its Lanczos kernel) render essentially black. Filtering
// here keeps such entries out of the picker entirely instead of
// silently letting the user pick a shader that can't render.
//
// Plain `Texture` is the legacy alias for our `Source` slot and
// stays valid; only the digit-suffixed forms are unsupported.
bool ReferencesUnsupportedHistoryBinding(const std::string& source) {
    auto isDigit = [](char c) { return c >= '0' && c <= '9'; };
    auto isIdChar = [&](char c) {
        return isDigit(c) || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
    };
    auto findUnboundedSemantic = [&](const std::string& prefix) -> bool {
        // Scan for `<prefix><digits>Texture` as a whole identifier.
        size_t pos = 0;
        while ((pos = source.find(prefix, pos)) != std::string::npos) {
            const bool leftOk = (pos == 0) || !isIdChar(source[pos - 1]);
            size_t end = pos + prefix.size();
            // Must have at least one digit after the prefix.
            const size_t digitsBegin = end;
            while (end < source.size() && isDigit(source[end])) {
                ++end;
            }
            if (end > digitsBegin) {
                static const std::string kSuffix = "Texture";
                if (end + kSuffix.size() <= source.size() &&
                    source.compare(end, kSuffix.size(), kSuffix) == 0) {
                    const size_t after = end + kSuffix.size();
                    const bool rightOk = (after >= source.size()) || !isIdChar(source[after]);
                    if (leftOk && rightOk) {
                        return true;
                    }
                }
            }
            pos += prefix.size();
        }
        return false;
    };
    return findUnboundedSemantic("PassPrev") || findUnboundedSemantic("Pass");
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

// Walk the extracted libretro tree and build the list of installable
// candidates (those that pass the cheap pre-filters AND the transpile
// gate AND don't reference unsupported history bindings). Each
// candidate carries the absolute source path so the install phase can
// copy from it without re-walking the tree. The collision-resolution
// algorithm matches the previous one-shot installer so the visible
// stems stay stable across the refactor.
//
// `repoRoot` points at the libretro repo root (after the
// `glsl-shaders-master/` zip wrapper has been peeled off).
void EnumerateCandidates(const std::filesystem::path& repoRoot,
                         std::vector<PendingCandidate>& out) {
    out.clear();
    std::set<std::string> usedStems;

    // Categories whose `shaders/` subdir holds effects useful as a
    // post-process on an N64 game. Other libretro categories
    // (`bezel/`, `xbr/`, …) are scaler chains or multi-pass-only and
    // bring no value at this phase.
    const char* kCategories[] = {
        "crt", "scanlines", "ntsc", "handheld", "dithering",
    };

    auto considerOne = [&](const std::filesystem::path& path,
                           const std::string& categoryHint) {
        std::string stem = path.stem().string();
        if (stem.empty() || LooksLikeMultiPassComponent(stem)) {
            return;
        }
        const std::string source = ReadCandidateShader(path);
        if (source.empty() || !LooksLikeSingleFileShader(source)) {
            return;
        }
        if (!TranspilesUnderCurrentNormalizer(source) ||
            ReferencesUnsupportedHistoryBinding(source)) {
            s_skippedUnsupported.fetch_add(1);
            return;
        }
        // Resolve stem collisions deterministically — first the parent
        // dir name, then the category, then a numeric suffix. Mirrors
        // the previous installer so on-disk filenames don't churn for
        // users between releases.
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

        PendingCandidate cand;
        cand.sourcePath  = path;
        cand.finalStem   = stem;
        cand.displayLabel = DeriveLabel(stem);
        out.push_back(std::move(cand));
    };

    for (const char* category : kCategories) {
        const std::filesystem::path catShaders = repoRoot / category / "shaders";
        std::error_code catEc;
        if (!std::filesystem::is_directory(catShaders, catEc)) {
            continue;
        }
        for (const auto& entry : std::filesystem::directory_iterator(catShaders, catEc)) {
            if (catEc) break;
            std::error_code entEc;
            if (entry.is_regular_file(entEc) && entry.path().extension() == ".glsl") {
                considerOne(entry.path(), category);
            }
        }
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
                    considerOne(sub.path(), category);
                }
            }
        }
    }
    // Stable, alphabetical order so the modal's checklist is
    // predictable across rebuilds.
    std::sort(out.begin(), out.end(),
              [](const PendingCandidate& a, const PendingCandidate& b) {
                  return a.displayLabel < b.displayLabel;
              });
}

// Move the flow back to Idle and best-effort delete the tempdir.
// Safe to call from any phase; serialized by s_candidatesMutex so a
// running install thread sees `s_tempDir` empty before deleting.
void ResetFlowState() {
    std::filesystem::path victim;
    {
        std::lock_guard<std::mutex> lock(s_candidatesMutex);
        s_candidates.clear();
        victim = std::move(s_tempDir);
        s_tempDir.clear();
    }
    if (!victim.empty()) {
        std::error_code ec;
        std::filesystem::remove_all(victim, ec);
    }
    s_phase.store(ShaderPackPhase::Idle);
    s_installedCount.store(0);
    s_skippedUnsupported.store(0);
    SetStatus(std::string());
}

} // namespace

ShaderPackPhase GetShaderPackPhase() { return s_phase.load(); }
int  GetShaderPackInstalledCount()   { return s_installedCount.load(); }

std::string GetShaderPackStatus() {
    std::lock_guard<std::mutex> lock(s_statusMutex);
    return s_status;
}

std::vector<ShaderPackCandidate> GetShaderPackCandidates() {
    std::vector<ShaderPackCandidate> out;
    std::lock_guard<std::mutex> lock(s_candidatesMutex);
    out.reserve(s_candidates.size());
    for (const PendingCandidate& c : s_candidates) {
        out.push_back({c.finalStem, c.displayLabel});
    }
    return out;
}

// Back-compat boolean queries — the old caller and any future
// telemetry can keep using these without knowing about the phase
// enum. "InProgress" is true for every non-terminal phase; "Complete"
// is the post-install Done state.
bool IsShaderPackDownloadInProgress() {
    const ShaderPackPhase p = s_phase.load();
    return p == ShaderPackPhase::DownloadingCatalog ||
           p == ShaderPackPhase::ExtractingCatalog ||
           p == ShaderPackPhase::EnumeratingCatalog ||
           p == ShaderPackPhase::InstallingSelected;
}
bool IsShaderPackDownloadComplete() {
    return s_phase.load() == ShaderPackPhase::Done;
}

void FetchShaderPackCatalogAsync() {
    // Idempotent: only Idle / Done / Error are entry points. From
    // AwaitingSelection the user should pick or cancel; from any
    // running phase the call is a no-op.
    const ShaderPackPhase cur = s_phase.load();
    SPDLOG_INFO("Shader pack: FetchShaderPackCatalogAsync entered, phase={}",
                static_cast<int>(cur));
    if (cur != ShaderPackPhase::Idle &&
        cur != ShaderPackPhase::Done &&
        cur != ShaderPackPhase::Error) {
        SPDLOG_INFO("Shader pack: ignoring fetch request (already running)");
        return;
    }
    ResetFlowState();
    s_phase.store(ShaderPackPhase::DownloadingCatalog);
    SetStatus("Starting download...");

    std::thread([]() {
        SPDLOG_INFO("Shader pack: worker thread started");
        const std::filesystem::path tempDir = std::filesystem::temp_directory_path() /
                                              "battleship-shader-pack";
        const std::filesystem::path zipPath = tempDir / "glsl-shaders.zip";
        const std::filesystem::path extractDir = tempDir / "extract";

        std::error_code ec;
        std::filesystem::remove_all(tempDir, ec);
        std::filesystem::create_directories(extractDir, ec);
        if (ec) {
            SetStatus("Error: could not create temp dir");
            s_phase.store(ShaderPackPhase::Error);
            return;
        }
        {
            std::lock_guard<std::mutex> lock(s_candidatesMutex);
            s_tempDir = tempDir;
        }

        SetStatus("Downloading libretro/glsl-shaders...");
        const std::string url =
            "https://github.com/libretro/glsl-shaders/archive/refs/heads/master.zip";
        SPDLOG_INFO("Shader pack: downloading {} -> {}", url, zipPath.string());
        if (!RunCurlDownload(url, zipPath)) {
            SPDLOG_ERROR("Shader pack: curl download failed");
            SetStatus("Error: download failed");
            s_phase.store(ShaderPackPhase::Error);
            return;
        }

        s_phase.store(ShaderPackPhase::ExtractingCatalog);
        SetStatus("Extracting...");
        SPDLOG_INFO("Shader pack: extracting {} -> {}", zipPath.string(), extractDir.string());
        if (!RunExtract(zipPath, extractDir)) {
            SPDLOG_ERROR("Shader pack: extract failed");
            SetStatus("Error: extract failed");
            s_phase.store(ShaderPackPhase::Error);
            return;
        }

        s_phase.store(ShaderPackPhase::EnumeratingCatalog);
        SetStatus("Validating shaders...");
        SPDLOG_INFO("Shader pack: enumerating candidates");
        // The libretro zip wraps everything in `glsl-shaders-master/`;
        // fall back to the extract root if that ever changes.
        std::filesystem::path repoRoot = extractDir / "glsl-shaders-master";
        std::error_code rrEc;
        if (!std::filesystem::is_directory(repoRoot, rrEc)) {
            repoRoot = extractDir;
        }

        std::vector<PendingCandidate> staged;
        EnumerateCandidates(repoRoot, staged);
        {
            std::lock_guard<std::mutex> lock(s_candidatesMutex);
            s_candidates = std::move(staged);
        }
        const size_t total = [] {
            std::lock_guard<std::mutex> lock(s_candidatesMutex);
            return s_candidates.size();
        }();
        if (total == 0) {
            SPDLOG_WARN("Shader pack: enumeration produced 0 candidates");
            SetStatus("No supported shaders in this build");
            s_phase.store(ShaderPackPhase::Error);
            return;
        }
        SPDLOG_INFO("Shader pack: catalog ready with {} candidates", total);
        SetStatus(std::to_string(total) + " shaders available");
        s_phase.store(ShaderPackPhase::AwaitingSelection);
    }).detach();
}

void InstallSelectedShaderPackAsync(const std::vector<std::string>& selectedStems) {
    if (s_phase.load() != ShaderPackPhase::AwaitingSelection) {
        return;
    }
    s_phase.store(ShaderPackPhase::InstallingSelected);
    s_installedCount.store(0);
    SetStatus("Installing selected shaders...");

    // Snapshot the selection set on the calling thread — it's a tiny
    // string set and we don't want to touch the caller's vector from
    // the worker.
    std::set<std::string> picked(selectedStems.begin(), selectedStems.end());

    std::thread([picked = std::move(picked)]() {
        // Resolve <userdata>/shaders/libretro/ once; matches the
        // single-shot installer's path so existing sidecar files stay
        // valid across the refactor.
        std::filesystem::path outRoot;
        try {
            outRoot = std::filesystem::path(
                Ship::Context::GetPathRelativeToAppDirectory("shaders")) / "libretro";
        } catch (...) {
            SetStatus("Error: LUS context unavailable");
            s_phase.store(ShaderPackPhase::Error);
            return;
        }

        std::error_code ec;
        // Wipe the libretro subdir before re-installing — the
        // downloader fully owns it, and removed entries shouldn't
        // linger in the picker just because the user installed a
        // smaller subset this round.
        std::filesystem::remove_all(outRoot, ec);
        std::filesystem::create_directories(outRoot, ec);

        // Snapshot candidate list under lock so the install loop
        // doesn't hold the mutex during file I/O.
        std::vector<PendingCandidate> snapshot;
        {
            std::lock_guard<std::mutex> lock(s_candidatesMutex);
            snapshot = s_candidates;
        }

        int installed = 0;
        for (const PendingCandidate& cand : snapshot) {
            if (picked.count(cand.finalStem) == 0) {
                continue;
            }
            const std::filesystem::path destPath = outRoot / (cand.finalStem + ".glsl");
            std::error_code copyEc;
            std::filesystem::copy_file(cand.sourcePath, destPath,
                                       std::filesystem::copy_options::overwrite_existing,
                                       copyEc);
            if (copyEc) {
                SPDLOG_WARN("Shader pack: failed to copy {} -> {}: {}",
                            cand.sourcePath.string(), destPath.string(),
                            copyEc.message());
                continue;
            }
            WriteSidecar(destPath, cand.displayLabel);
            ++installed;
        }
        s_installedCount.store(installed);

        // Cleanup: the install phase owns tempdir teardown.
        std::filesystem::path victim;
        {
            std::lock_guard<std::mutex> lock(s_candidatesMutex);
            victim = std::move(s_tempDir);
            s_tempDir.clear();
            s_candidates.clear();
        }
        if (!victim.empty()) {
            std::filesystem::remove_all(victim, ec);
        }

        if (installed > 0) {
            SetStatus("Installed " + std::to_string(installed) + " shaders");
            s_phase.store(ShaderPackPhase::Done);
        } else {
            SetStatus("No shaders installed");
            s_phase.store(ShaderPackPhase::Idle);
        }
    }).detach();
}

void CancelShaderPackFlow() { ResetFlowState(); }

} // namespace enhancements
} // namespace ssb64
