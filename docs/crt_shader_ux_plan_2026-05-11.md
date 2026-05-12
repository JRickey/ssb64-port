# CRT Shader UX Plan — 2026-05-11

Follow-up to `docs/crt_shader_plan_2026-05-11.md`. Phase 1 (runtime,
transpiler, normalizer) shipped; this plan tracks the user-facing
polish layered on top so end users never have to touch the in-game
console to pick a shader.

This is a planning doc — implementation lands on follow-up branches.
Listed in recommended order; each item is independently shippable.

---

## 1. Move the shader directory to LUS user-data

**Today**: `PostProcessSourceLoader::TryLoadLanguage` reads
`shaders/<name>.<ext>` relative to the process CWD. Launching via
`./build/BattleShip` from the project root puts CWD at the project
root, not `build/`; launching via Finder / `.app` puts CWD at `/`.
Both produce silent "shader not found" failures.

**Fix**: resolve `<userdata>/shaders/<name>.<ext>` instead, where
`<userdata>` is the per-platform location LUS already uses for cvars
and saves:

| Platform | Path                                                     |
|----------|----------------------------------------------------------|
| macOS    | `~/Library/Application Support/BattleShip/shaders/`      |
| Windows  | `%APPDATA%/BattleShip/shaders/`                          |
| Linux    | `$XDG_DATA_HOME/BattleShip/shaders/` (fallback `~/.local/share/BattleShip/shaders/`) |

LUS exposes the base path via `Ship::Context::GetInstance()
->GetAppBundlePath()` / `GetUserDataPath()` (check
`AppleFolderManager.mm` / similar — already in tree). The directory
is created on first launch if absent. Existing CWD-relative behaviour
can stay as a development fallback (project-root `shaders/` wins if
present) so contributors don't have to copy files into Application
Support every time they iterate.

---

## 2. Replace the 3-item ComboBox with a tree picker

**Today**: `port/gui/PortMenu.cpp` hard-codes
```cpp
{ 0, "Off" }, { 1, "Scanlines" }, { 2, "CRT — Lottes" }
```
plus a docs hint that the user can set `gPostProcessShader` to any
filename. Discovering custom shaders requires console use.

**Fix**: ImGui-tree picker keyed on the on-disk layout under
`<userdata>/shaders/`. Group by parent folder name (mirrors
libretro's `crt/` / `ntsc/` / `presets/` taxonomy), filter by a name
textbox at the top. The two bundled CC0 shaders show as a "Bundled"
group at the top of the tree, ahead of any user folders. Selection
writes `gPostProcessShader` and bumps `gPostProcessEnabled` exactly
the same way the existing combo does, so the runtime path stays
unchanged.

`PostProcessSourceLoader::ListBuiltinPostProcessShaders` currently
hardcodes the two-name list; for the picker, add a sibling
`ListUserPostProcessShaders` that walks the user-data dir and
returns a tree node structure (folder → file list).

---

## 3. Resolution-compat metadata + warning

**Why**: bundled CC0 shaders (`scanlines`, `crt-lottes`) work at any
output resolution because they don't ratio `TextureSize / InputSize`.
Libretro shaders (Hyllian, GTU, easymode, etc.) reference InputSize
to compute scanline period in game-pixel space; running them with
LUS's high-res FBO produces a "shader runs but renders into a small
corner / background black" failure mode unless the user has the new
Low Resolution Mode enabled (which forces the game FBO to match
native 320×240).

**Fix**: per-shader sidecar `<name>.lus.json` next to the `.glsl`:
```json
{
  "compat": "any" | "native",
  "label": "CRT — Hyllian Curvature + Glow",
  "license": "MIT"
}
```
Defaults if the sidecar is missing:
- Bundled shaders ship with `compat=any` sidecars in `f3d.o2r`.
- Downloaded libretro shaders get `compat=native` written by the
  downloader (item 4) at extract time.
- User-dropped shaders without a sidecar default to `compat=native`
  (safer; the "small corner" failure is more confusing than seeing
  the warning).

UI: every entry in the picker shows a compatibility badge. If the
user picks a `native`-only shader while Low Resolution Mode is off,
pop an inline ImGui modal: "This shader needs Low Resolution Mode.
Enable and restart? [Yes / No]". The new boot-latched setting from
`d297cf8` means we can't toggle live; the modal writes the cvar and
prompts the user to restart.

---

## 4. "Download libretro shader pack" button

**Why**: libretro/glsl-shaders is the de-facto single-pass shader
library and Phase 1 can run a large slice of it once normalized. We
want one button, same flow as RetroArch's online updater.

**Implementation**: piggyback on `port/enhancements/Updater.cpp`
(landed via `00d6e76` / `2387e1f`). It already has HTTP fetch + zip
extract glue.

Flow:
1. Fetch `https://github.com/libretro/glsl-shaders/archive/refs/heads/master.zip`
   to a temp file (≈ a few MB).
2. Stream-extract into `<userdata>/shaders/libretro/`, skipping
   anything that isn't a single-pass `.glsl` (filter out `.glslp`,
   `.slangp`, multi-pass folders that contain a `resolve.glsl`,
   readme files, etc.).
3. For each extracted `.glsl`, write a sibling `<name>.lus.json`
   with `compat=native` and a human-readable label derived from the
   filename.
4. Tell the user how many shaders were installed; refresh the
   picker tree.
5. Surface an "Update shader pack" button that re-runs the same
   flow; it should diff-detect (md5 the existing tree) to avoid
   re-downloading on every click.

**Licensing**: no change to our binary distribution. The shaders are
data the user installs at runtime — same legal posture as RetroArch's
shader-downloader feature. We never ship GPL bits inside `f3d.o2r`
or the binary. Plan §5 / §8 already cleared this approach.

**Filter rationale (Phase 1 only)**: dropping `.glslp` multi-pass
folders avoids confusing the user with shaders that look broken
because Phase 2 hasn't shipped. When `.glslp` support lands, change
the extract filter to include them and the picker will pick them up
automatically.

---

## 5. Open questions

1. **Custom-shader-folder cvar override?** Power-users might want to
   point at an external folder (e.g. a shared shader collection on
   another drive). Adding `gPostProcessShaderDir` as an override
   cvar is cheap and matches RetroArch's `video_shader_dir`. Worth
   doing alongside item 1.
2. **Per-shader parameter sliders?** Libretro shaders carry
   `#pragma parameter` declarations the normalizer currently strips.
   Surfacing those as ImGui sliders is a significant UX win for the
   serious-tuning crowd but a separate plan-doc-sized effort.
3. **Preset save/load?** RetroArch lets you save the current shader
   + parameter state as a named preset. Out of scope until item 2 is
   done.
4. **Auto-disable post-process when game pauses / shows menus?**
   Some shaders flicker badly when the source FB is static. Worth a
   single CVar gate; not part of this plan.

---

## 6. Estimated effort

| Item | Adds                                                     | Effort |
|------|----------------------------------------------------------|--------|
| 1    | user-data path resolution + dev CWD fallback             | ½ day  |
| 2    | tree picker + dir walk                                   | 1 day  |
| 3    | sidecar parser + bundled-shader sidecars + warn modal    | 1 day  |
| 4    | downloader (re-use Updater) + extract filter + sidecars  | 1–2 days |

Total roughly a week of focused work, but each item ships on its own
branch so the user-visible improvement is incremental.
