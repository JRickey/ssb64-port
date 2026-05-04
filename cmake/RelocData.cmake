# RelocData.cmake — compile-from-source pipeline for decomp/src/relocData/*.c
#
# Drives the optional path that produces BattleShip.fromsource.o2r — a
# RelocFile-resource archive containing source-compiled equivalents of the
# reloc data Torch normally extracts from the ROM. The archive is loaded
# ahead of BattleShip.o2r at runtime when the file is present, shadowing
# Torch entries for matching paths via LUS's FIFO ArchiveManager lookup.
#
# Cross-platform constraint: relocData .c files cast (u32)&symbol_addr in
# static initialisers. That's only a constant expression on a 32-bit-pointer
# target, so we always cross-compile to i686 (ELF via clang or COFF via
# MSVC's x86 cl.exe). Selection is driven by the RELOCDATA_COMPILER cache
# option:
#
#   auto  (default) — prefer clang, fall back to MSVC x86, skip if neither
#   clang           — require clang, fail loudly if missing
#   msvc            — require MSVC x86 (vcvars32), fail loudly if missing
#
# When neither toolchain is available (or RELOCDATA_COMPILER=auto with
# nothing found) the BuildBattleShipFromSource target is omitted and the
# runtime falls back to Torch-extracted reloc data exactly as today.
#
# ─────────────────────────── Required inputs ───────────────────────────
#
#   RELOCDATA_DECOMP_DIR      Path to the decomp tree; we read .c sources
#                             from <decomp>/src/relocData/, headers from
#                             <decomp>/include/, and the file-descriptions
#                             table from <decomp>/tools/. No default.
#   RELOCDATA_RELOC_TABLE     Path to RelocFileTable.cpp — the codegen
#                             output mapping file_id ↔ archive resource
#                             path. No default.
#
# ─────────────────────────── Optional inputs ───────────────────────────
#
#   RELOCDATA_TOOLS_DIR       Path to the toolkit Python tools. Defaults
#                             to <RelocData.cmake>/../tools so when this
#                             module ships in a standalone modkit repo,
#                             tools/ next to cmake/ is found automatically.
#   RELOCDATA_OUTPUT_DIR      Build output directory. Defaults to
#                             ${CMAKE_BINARY_DIR}/relocdata.
#   RELOCDATA_BATTLESHIP_O2R  Path to a Torch-extracted BattleShip.o2r
#                             (used only by extract_inc_c / passthrough
#                             paths). Probed at the parent's CMAKE_SOURCE_DIR
#                             and CMAKE_BINARY_DIR if unset.
#   RELOCDATA_COMPILER        auto|clang|msvc — see comment above.

if(NOT DEFINED RELOCDATA_DECOMP_DIR)
    message(FATAL_ERROR
        "RelocData: RELOCDATA_DECOMP_DIR must be set before including "
        "RelocData.cmake — point it at the decomp tree (e.g. "
        "set(RELOCDATA_DECOMP_DIR \"\${CMAKE_SOURCE_DIR}/decomp\")).")
endif()
if(NOT DEFINED RELOCDATA_RELOC_TABLE)
    message(FATAL_ERROR
        "RelocData: RELOCDATA_RELOC_TABLE must be set before including "
        "RelocData.cmake — point it at the consumer's RelocFileTable.cpp.")
endif()
if(NOT DEFINED RELOCDATA_TOOLS_DIR)
    get_filename_component(RELOCDATA_TOOLS_DIR
        "${CMAKE_CURRENT_LIST_DIR}/../tools" ABSOLUTE)
endif()
if(NOT DEFINED RELOCDATA_OUTPUT_DIR)
    set(RELOCDATA_OUTPUT_DIR "${CMAKE_BINARY_DIR}/relocdata")
endif()

set(RELOCDATA_COMPILER "auto" CACHE STRING
    "Backend for the from-source relocData pipeline (auto|clang|msvc)")
set_property(CACHE RELOCDATA_COMPILER PROPERTY STRINGS auto clang msvc)

# ── Backend discovery ──
# Both backends produce the same .relo bytes from the same source — verified
# byte-identical on representative files. See tools/build_reloc_resource.py
# for the format-agnostic downstream pipeline.

set(RELOCDATA_CC      "")
set(RELOCDATA_CC_KIND "")
set(RELOCDATA_MSVC_ENV "")     # set when CC_KIND==msvc — list of VAR=val for cmake -E env

# 1. Try clang (works on every host with LLVM installed).
if(RELOCDATA_COMPILER STREQUAL "auto" OR RELOCDATA_COMPILER STREQUAL "clang")
    # NAMES list excludes clang-cl: it's a clang in MSVC compatibility mode
    # and won't accept `-target i686-pc-linux-gnu`. Use real clang only.
    find_program(CLANG_EXECUTABLE NAMES clang)
    if(CLANG_EXECUTABLE)
        set(RELOCDATA_CC      "${CLANG_EXECUTABLE}")
        set(RELOCDATA_CC_KIND "clang")
    elseif(RELOCDATA_COMPILER STREQUAL "clang")
        message(FATAL_ERROR
            "RelocData: RELOCDATA_COMPILER=clang but no clang on PATH. "
            "Install LLVM (Windows: 'winget install LLVM.LLVM'; "
            "macOS: ships with Xcode CLI tools; Linux: 'apt install clang') "
            "or set RELOCDATA_COMPILER=auto/msvc.")
    endif()
endif()

# 2. Try MSVC x86 (Windows hosts with Visual Studio C++ Build Tools).
#    Skipped silently in auto mode if clang already found above.
#
# Discovery has three robustness concerns we resolve up-front rather than
# papering over with cmd /c shell-quoting tricks:
#   (a) vswhere lives in a fixed Installer directory but the *VS install
#       itself* is wherever the user told VS to put it. Use vswhere to
#       resolve the install path; don't hardcode it.
#   (b) vcvars32.bat with embedded paths containing spaces is fragile to
#       quote through `cmd /c "..."`. We write a tiny capture wrapper batch
#       that calls vcvars32 by absolute path with no shell-string escaping.
#   (c) INCLUDE/LIB/PATH all contain semicolons. Round-tripping them through
#       CMake variables, then through `cmake -E env`, then through ninja's
#       command quoting is a minefield (CMake list expansion silently shreds
#       the values). We sidestep it: the captured env is baked LITERALLY
#       into a per-build-tree shim batch file. add_custom_command then just
#       invokes the shim — no env round-trip through CMake variables.
if(WIN32
   AND NOT RELOCDATA_CC
   AND (RELOCDATA_COMPILER STREQUAL "auto" OR RELOCDATA_COMPILER STREQUAL "msvc"))

    # ── (a) Locate vswhere ─────────────────────────────────────────────
    # vswhere ships with the VS Installer (2017+). Try the fixed install-time
    # paths first, then fall back to PATH lookup for users who installed VS
    # in a non-default location and added the Installer to PATH manually.
    find_program(VSWHERE_EXECUTABLE
        NAMES vswhere vswhere.exe
        PATHS "$ENV{ProgramFiles\(x86\)}/Microsoft Visual Studio/Installer"
              "$ENV{ProgramFiles}/Microsoft Visual Studio/Installer"
        DOC "Microsoft VS installation locator")

    set(_msvc_x86_ok FALSE)
    set(_msvc_x86_diag "")          # accumulates failure reasons for the FATAL_ERROR

    if(NOT VSWHERE_EXECUTABLE)
        string(CONCAT _msvc_x86_diag
            "vswhere.exe not found in the Visual Studio Installer "
            "directory or on PATH")
    else()
        # ── Resolve VS install path ────────────────────────────────────
        # -products * to include BuildTools-only installs. -requires the
        # x86 toolset component so vswhere returns nothing if VS exists
        # but the C++ x86 workload was never installed (avoids a confusing
        # cl.exe-not-found error later).
        execute_process(
            COMMAND "${VSWHERE_EXECUTABLE}"
                -latest
                -products *
                -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64
                -property installationPath
            OUTPUT_VARIABLE _vs_install_path
            OUTPUT_STRIP_TRAILING_WHITESPACE
            RESULT_VARIABLE _vswhere_rc
            ERROR_QUIET)

        if(NOT _vswhere_rc EQUAL 0 OR NOT _vs_install_path)
            string(CONCAT _msvc_x86_diag
                "vswhere returned no VS install with the "
                "'VC.Tools.x86.x64' component (rc=${_vswhere_rc}). Install "
                "the 'Desktop development with C++' workload via Visual "
                "Studio Installer.")
        else()
            file(TO_CMAKE_PATH "${_vs_install_path}" _vs_install_path)
            set(_vcvars32 "${_vs_install_path}/VC/Auxiliary/Build/vcvars32.bat")

            if(NOT EXISTS "${_vcvars32}")
                string(CONCAT _msvc_x86_diag
                    "vcvars32.bat missing under VS install: ${_vcvars32}")
            else()
                # ── (b) Capture env via wrapper batch ───────────────────
                # The wrapper writes INCLUDE/LIB/PATH each to its own file
                # rather than dumping the full env via `set` and parsing it.
                # Reason: env values contain semicolons (PATH), CMake's
                # newline-split-then-foreach pattern would re-split each
                # value on its embedded `;`, silently capturing only the
                # first path. Per-var files sidestep all CMake list
                # semantics — each value round-trips as an opaque string.
                file(TO_NATIVE_PATH "${_vcvars32}" _vcvars32_native)
                set(_inc_capture  "${CMAKE_BINARY_DIR}/_relocdata_vc_include.txt")
                set(_lib_capture  "${CMAKE_BINARY_DIR}/_relocdata_vc_lib.txt")
                set(_path_capture "${CMAKE_BINARY_DIR}/_relocdata_vc_path.txt")
                file(TO_NATIVE_PATH "${_inc_capture}"  _inc_capture_native)
                file(TO_NATIVE_PATH "${_lib_capture}"  _lib_capture_native)
                file(TO_NATIVE_PATH "${_path_capture}" _path_capture_native)

                # Clear any stale captures from a prior reconfigure so we
                # detect a partial failure (e.g. vcvars32 sets INCLUDE but
                # not LIB).
                file(REMOVE "${_inc_capture}" "${_lib_capture}" "${_path_capture}")

                set(_capture_bat "${CMAKE_BINARY_DIR}/_relocdata_capture_vcvars32.bat")
                file(WRITE "${_capture_bat}"
"@echo off
call \"${_vcvars32_native}\" >NUL 2>&1
if errorlevel 1 (
  exit /b 1
)
rem `>file echo %VAR%` writes the literal expansion. Don't quote %VAR% —
rem cmd would echo the quotes verbatim. Trailing CR/LF is stripped on read.
> \"${_inc_capture_native}\"  echo %INCLUDE%
> \"${_lib_capture_native}\"  echo %LIB%
> \"${_path_capture_native}\" echo %PATH%
")
                execute_process(
                    COMMAND "${_capture_bat}"
                    RESULT_VARIABLE _capture_rc
                    OUTPUT_QUIET ERROR_QUIET)

                if(NOT _capture_rc EQUAL 0
                   OR NOT EXISTS "${_inc_capture}"
                   OR NOT EXISTS "${_lib_capture}"
                   OR NOT EXISTS "${_path_capture}")
                    string(CONCAT _msvc_x86_diag
                        "vcvars32.bat failed to set up the x86 "
                        "environment (rc=${_capture_rc}). The VS C++ x86 "
                        "toolset may not be installed.")
                else()
                    # Read each captured value as one opaque string.
                    file(READ "${_inc_capture}"  _msvc_include)
                    file(READ "${_lib_capture}"  _msvc_lib)
                    file(READ "${_path_capture}" _msvc_path)
                    # Strip the trailing CRLF/LF added by `echo`.
                    string(STRIP "${_msvc_include}" _msvc_include)
                    string(STRIP "${_msvc_lib}"     _msvc_lib)
                    string(STRIP "${_msvc_path}"    _msvc_path)

                    # ── Sanity-check the captured env ───────────────────
                    # vcvars32 (x86) always sources the x86 host toolset
                    # path which contains "Hostx86" — verifying its
                    # presence catches "vcvars32 ran but didn't actually
                    # set up x86" failure modes.
                    if(NOT _msvc_include OR NOT _msvc_lib OR NOT _msvc_path)
                        string(CONCAT _msvc_x86_diag
                            "vcvars32 ran but INCLUDE/LIB/PATH were not all "
                            "populated — installation appears damaged.")
                    elseif(NOT _msvc_path MATCHES "[Hh]ost[Xx]86")
                        # vcvars32 (x86) always sources a HostX86 toolset
                        # path; its absence means the x86 toolset wasn't
                        # actually selected even though the script ran.
                        string(CONCAT _msvc_x86_diag
                            "vcvars32's PATH does not contain a HostX86 "
                            "toolset (got: ${_msvc_path}). Reinstall the "
                            "x86 component.")
                    else()
                        # Locate cl.exe by searching the captured PATH.
                        # NO_DEFAULT_PATH so we don't pick up a stray cl.exe
                        # from the user's regular PATH (e.g. a cygwin port
                        # or an x64-only one outside vcvars).
                        unset(MSVC_X86_CL_EXECUTABLE CACHE)
                        find_program(MSVC_X86_CL_EXECUTABLE
                            NAMES cl.exe
                            PATHS ${_msvc_path}
                            NO_DEFAULT_PATH NO_CMAKE_PATH
                            NO_CMAKE_ENVIRONMENT_PATH
                            NO_SYSTEM_ENVIRONMENT_PATH)

                        if(NOT MSVC_X86_CL_EXECUTABLE)
                            string(CONCAT _msvc_x86_diag
                                "cl.exe not found in the captured vcvars32 "
                                "PATH despite HostX86 being present "
                                "(PATH=${_msvc_path}).")
                        else()
                            # ── (c) Bake env into a build-time wrapper ──
                            # Generated once per configure; takes its own
                            # args via %* so add_custom_command can call it
                            # like a normal program. The literal SET lines
                            # avoid every list/quoting hazard that would
                            # come from re-substituting the captured env
                            # through CMake variables and `cmake -E env`.
                            #
                            # Escape % and ^ in case the captured env ever
                            # contains them (unusual but legal in a path).
                            set(_inc_esc "${_msvc_include}")
                            set(_lib_esc "${_msvc_lib}")
                            set(_path_esc "${_msvc_path}")
                            string(REPLACE "%" "%%" _inc_esc  "${_inc_esc}")
                            string(REPLACE "%" "%%" _lib_esc  "${_lib_esc}")
                            string(REPLACE "%" "%%" _path_esc "${_path_esc}")

                            set(RELOCDATA_CC_WRAPPER
                                "${CMAKE_BINARY_DIR}/_relocdata_msvc_env.bat")
                            file(WRITE "${RELOCDATA_CC_WRAPPER}"
"@echo off
rem Auto-generated by cmake/RelocData.cmake — do not edit.
rem Bakes the configure-time vcvars32 (x86) environment so per-file
rem build commands in add_reloc_resource() invoke cl.exe with the
rem correct INCLUDE/LIB/PATH and no env round-trip through CMake.
set \"INCLUDE=${_inc_esc}\"
set \"LIB=${_lib_esc}\"
set \"PATH=${_path_esc}\"
%*
exit /b %ERRORLEVEL%
")

                            set(_msvc_x86_ok TRUE)
                            set(RELOCDATA_CC      "${MSVC_X86_CL_EXECUTABLE}")
                            set(RELOCDATA_CC_KIND "msvc")
                            # Used by add_reloc_resource to wrap commands.
                            # Empty when CC_KIND==clang.
                            file(TO_CMAKE_PATH "${RELOCDATA_CC_WRAPPER}"
                                RELOCDATA_CC_WRAPPER)
                        endif()
                    endif()
                endif()
            endif()
        endif()
    endif()

    if(NOT _msvc_x86_ok AND RELOCDATA_COMPILER STREQUAL "msvc")
        message(FATAL_ERROR
            "RelocData: RELOCDATA_COMPILER=msvc requested but the x86 "
            "toolchain isn't usable.\n"
            "  Reason: ${_msvc_x86_diag}\n"
            "  Fix: install Visual Studio 2017+ (or Build Tools) with the "
            "'Desktop development with C++' workload, ensuring "
            "MSVC v143 - VS 2022 C++ x64/x86 build tools is selected.\n"
            "  Or: set RELOCDATA_COMPILER=auto and install LLVM "
            "('winget install LLVM.LLVM') as the alternative backend.")
    endif()
endif()

# 3. No backend found — degrade silently in auto mode (consistent with
#    pre-MSVC behaviour: build runs Torch-only). Already errored loudly
#    above for explicit clang/msvc requests.
if(NOT RELOCDATA_CC)
    message(STATUS
        "RelocData: no backend available (set RELOCDATA_COMPILER explicitly "
        "to debug). Skipping BuildBattleShipFromSource — runtime uses Torch-"
        "extracted reloc data only. To enable: install LLVM "
        "('winget install LLVM.LLVM' on Windows, 'apt install clang' on "
        "Linux, ships with Xcode CLI tools on macOS) or VS Build Tools "
        "with the C++ workload.")
    return()
endif()

# Re-export under the legacy name so anything reading CLANG_EXECUTABLE
# downstream keeps working. CMakeLists.txt's `if(CLANG_EXECUTABLE)` gate
# stays valid — we set it whenever any backend is wired up.
set(CLANG_EXECUTABLE "${RELOCDATA_CC}" CACHE INTERNAL "")

message(STATUS "RelocData: ${RELOCDATA_CC_KIND} backend at ${RELOCDATA_CC} — "
               "BuildBattleShipFromSource enabled")

set(RELOC_FROMSOURCE_DIR ${RELOCDATA_OUTPUT_DIR}/reloc_resources)
set(RELOC_OBJECTS_DIR    ${RELOCDATA_OUTPUT_DIR}/reloc_objects)
set(RELOC_INC_C_DIR      ${RELOCDATA_OUTPUT_DIR}/inc_c_extracts)
file(MAKE_DIRECTORY ${RELOC_FROMSOURCE_DIR})
file(MAKE_DIRECTORY ${RELOC_OBJECTS_DIR})
file(MAKE_DIRECTORY ${RELOC_INC_C_DIR})

# Run the .inc.c extractor at configure time. Outputs land at
# <RELOCDATA_OUTPUT_DIR>/inc_c_extracts/<FileName>/<sym>.<type>.inc.c,
# matching the include paths (`#include <FileName/sym.type.inc.c>`)
# used by typed relocData .c files. Depends on the Torch-extracted
# BattleShip.o2r; if that doesn't exist yet (no ExtractAssets run), the
# extractor is skipped — relocData files needing .inc.c will fail compile
# and the user will see the failure with a clear path forward.
if(NOT DEFINED RELOCDATA_BATTLESHIP_O2R)
    if(EXISTS ${CMAKE_SOURCE_DIR}/BattleShip.o2r)
        set(RELOCDATA_BATTLESHIP_O2R ${CMAKE_SOURCE_DIR}/BattleShip.o2r)
    elseif(EXISTS ${CMAKE_BINARY_DIR}/BattleShip.o2r)
        set(RELOCDATA_BATTLESHIP_O2R ${CMAKE_BINARY_DIR}/BattleShip.o2r)
    else()
        set(RELOCDATA_BATTLESHIP_O2R "")
    endif()
endif()

if(RELOCDATA_BATTLESHIP_O2R)
    message(STATUS "RelocData: extracting .inc.c blocks from ${RELOCDATA_BATTLESHIP_O2R}")
    execute_process(
        COMMAND ${Python3_EXECUTABLE} ${RELOCDATA_TOOLS_DIR}/extract_inc_c.py
            --battleship-o2r ${RELOCDATA_BATTLESHIP_O2R}
            --reloc-table ${RELOCDATA_RELOC_TABLE}
            --reloc-dir ${RELOCDATA_DECOMP_DIR}/src/relocData
            --descriptions ${RELOCDATA_DECOMP_DIR}/tools/relocFileDescriptions.us.txt
            --output-dir ${RELOC_INC_C_DIR}
        RESULT_VARIABLE _extract_result
        OUTPUT_QUIET
    )
    if(NOT _extract_result EQUAL 0)
        message(WARNING "RelocData: extract_inc_c.py failed (exit ${_extract_result})")
    endif()
else()
    message(STATUS "RelocData: BattleShip.o2r not present — skipping .inc.c extract; "
                   "files needing inc.c will fall back to Torch")
endif()

# Per-file: file_id N + source path → custom command emitting <build>/reloc_resources/<N>.relo
# resource_path is the LUS archive path (from RelocFileTable.cpp), e.g.
# "reloc_animations/FTSamusAnim047". Stored on the target so the pack step
# knows which on-disk path each resource ends up at.
set(SSB64_RELOC_FROMSOURCE_OUTPUTS "" CACHE INTERNAL "")
set(SSB64_RELOC_FROMSOURCE_PATHS "" CACHE INTERNAL "")

function(add_reloc_resource file_id source_path resource_path)
    set(reloc_path ${source_path})
    string(REGEX REPLACE "\\.c$" ".reloc" reloc_path ${reloc_path})

    set(out ${RELOC_FROMSOURCE_DIR}/${file_id}.relo)
    # Object suffix differs by backend (.o for ELF, .obj for COFF) but
    # otherwise files share the directory.
    if(RELOCDATA_CC_KIND STREQUAL "msvc")
        set(obj ${RELOC_OBJECTS_DIR}/${file_id}.obj)
    else()
        set(obj ${RELOC_OBJECTS_DIR}/${file_id}.o)
    endif()

    # MSVC backend invokes the per-build-tree wrapper batch
    # (RELOCDATA_CC_WRAPPER) which has the captured vcvars32 env baked in
    # literally. We deliberately avoid `cmake -E env VAR=value` here:
    # INCLUDE/LIB/PATH all contain semicolons, and routing them through
    # CMake variables → ninja command quoting would silently shred the
    # values via list expansion. The wrapper sidesteps it entirely.
    if(RELOCDATA_CC_KIND STREQUAL "msvc")
        set(_cmd_prefix "${RELOCDATA_CC_WRAPPER}")
    else()
        set(_cmd_prefix "")
    endif()

    add_custom_command(
        OUTPUT ${out}
        COMMAND ${_cmd_prefix} ${Python3_EXECUTABLE}
            ${RELOCDATA_TOOLS_DIR}/build_reloc_resource.py
            --src ${source_path}
            --reloc ${reloc_path}
            --file-id ${file_id}
            --cc ${RELOCDATA_CC}
            --cc-kind ${RELOCDATA_CC_KIND}
            --include-dir ${RELOCDATA_DECOMP_DIR}/include
            --include-dir ${RELOCDATA_DECOMP_DIR}/src
            --include-dir ${RELOCDATA_DECOMP_DIR}/src/relocData
            --include-dir ${RELOC_INC_C_DIR}
            --obj-out ${obj}
            --output ${out}
        DEPENDS ${source_path} ${reloc_path}
                ${RELOCDATA_TOOLS_DIR}/build_reloc_resource.py
        COMMENT "Building from-source reloc resource ${file_id} (${resource_path})"
        VERBATIM
    )

    list(APPEND SSB64_RELOC_FROMSOURCE_OUTPUTS ${out})
    list(APPEND SSB64_RELOC_FROMSOURCE_PATHS "${file_id}|${resource_path}")
    set(SSB64_RELOC_FROMSOURCE_OUTPUTS "${SSB64_RELOC_FROMSOURCE_OUTPUTS}" CACHE INTERNAL "")
    set(SSB64_RELOC_FROMSOURCE_PATHS "${SSB64_RELOC_FROMSOURCE_PATHS}" CACHE INTERNAL "")
endfunction()

# Pack step: gathers all .relo outputs from add_reloc_resource calls into a
# single .o2r archive at the resource paths the runtime expects. The pack
# manifest is a Python-readable text file the pack tool consumes.
function(add_passthrough_resource file_id resource_path)
    set(out ${RELOC_FROMSOURCE_DIR}/${file_id}.relo)
    add_custom_command(
        OUTPUT ${out}
        COMMAND ${Python3_EXECUTABLE} ${RELOCDATA_TOOLS_DIR}/passthrough_reloc.py
            --battleship-o2r ${RELOCDATA_BATTLESHIP_O2R}
            --reloc-table ${RELOCDATA_RELOC_TABLE}
            --file-id ${file_id}
            --output ${out}
        DEPENDS ${RELOCDATA_BATTLESHIP_O2R}
                ${RELOCDATA_TOOLS_DIR}/passthrough_reloc.py
        COMMENT "Passthrough reloc resource ${file_id} (${resource_path})"
        VERBATIM
    )

    list(APPEND SSB64_RELOC_FROMSOURCE_OUTPUTS ${out})
    list(APPEND SSB64_RELOC_FROMSOURCE_PATHS "${file_id}|${resource_path}")
    set(SSB64_RELOC_FROMSOURCE_OUTPUTS "${SSB64_RELOC_FROMSOURCE_OUTPUTS}" CACHE INTERNAL "")
    set(SSB64_RELOC_FROMSOURCE_PATHS "${SSB64_RELOC_FROMSOURCE_PATHS}" CACHE INTERNAL "")
endfunction()


function(finalize_battleship_from_source archive_output)
    set(manifest ${RELOCDATA_OUTPUT_DIR}/reloc_fromsource_manifest.txt)
    set(manifest_lines "")
    foreach(entry ${SSB64_RELOC_FROMSOURCE_PATHS})
        list(APPEND manifest_lines ${entry})
    endforeach()
    string(REPLACE ";" "\n" manifest_content "${manifest_lines}")
    file(WRITE ${manifest} "${manifest_content}\n")

    add_custom_command(
        OUTPUT ${archive_output}
        COMMAND ${Python3_EXECUTABLE} ${RELOCDATA_TOOLS_DIR}/pack_reloc_archive.py
            --manifest ${manifest}
            --reloc-dir ${RELOC_FROMSOURCE_DIR}
            --output ${archive_output}
        DEPENDS ${SSB64_RELOC_FROMSOURCE_OUTPUTS}
                ${RELOCDATA_TOOLS_DIR}/pack_reloc_archive.py
        COMMENT "Packing BattleShip.fromsource.o2r (${archive_output})"
        VERBATIM
    )

    add_custom_target(BuildBattleShipFromSource ALL
        DEPENDS ${archive_output}
    )

    # Copy archive next to the executable so the runtime PortLocateFile
    # finds it without extra -E setup — only when the consuming project
    # actually has a target to point at. Standalone modkit users pack
    # the archive directly to wherever they need it via archive_output.
    if(TARGET ${PROJECT_NAME})
        add_custom_command(TARGET BuildBattleShipFromSource POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${archive_output}
                $<TARGET_FILE_DIR:${PROJECT_NAME}>/BattleShip.fromsource.o2r
            VERBATIM
        )
    endif()
endfunction()


# ─────────────────────────── Eligible-set generation ───────────────────────
#
# Run gen_reloc_cmake.py at configure time to enumerate which relocData files
# the source-compile path can handle. Output is a CMake fragment of
# add_reloc_resource() / add_passthrough_resource() calls, included
# immediately. Lives at the bottom of this module so add_reloc_resource and
# add_passthrough_resource are defined before the include() pulls them in.

execute_process(
    COMMAND ${Python3_EXECUTABLE} ${RELOCDATA_TOOLS_DIR}/gen_reloc_cmake.py
        --reloc-dir ${RELOCDATA_DECOMP_DIR}/src/relocData
        --reloc-table ${RELOCDATA_RELOC_TABLE}
        --inc-c-dir ${RELOC_INC_C_DIR}
        --cmake-source-prefix \${RELOCDATA_DECOMP_DIR}/src/relocData
        --output ${RELOCDATA_OUTPUT_DIR}/reloc_data_targets.cmake
    RESULT_VARIABLE _gen_result
)
if(NOT _gen_result EQUAL 0)
    message(FATAL_ERROR
        "RelocData: gen_reloc_cmake.py failed (exit ${_gen_result})")
endif()
include(${RELOCDATA_OUTPUT_DIR}/reloc_data_targets.cmake)
