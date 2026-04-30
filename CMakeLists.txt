cmake_minimum_required(VERSION 3.24.0 FATAL_ERROR)

project(ssb64 LANGUAGES C CXX)

include(ExternalProject)

# Pulled-in BEFORE the libultraship subdirectory so its
# find_package(tinyxml2) sees our pkg-config-fallback shim. Required
# for Ubuntu 22.04 jammy, where libtinyxml2-dev ships no CMake config.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin" OR CMAKE_SYSTEM_NAME STREQUAL "iOS")
    enable_language(OBJCXX)
    set(CMAKE_OBJCXX_FLAGS "${CMAKE_OBJCXX_FLAGS} -fobjc-arc")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE)
endif()

################################################################################
# Options
################################################################################

option(NON_MATCHING "Build without matching decomp constraints" ON)
option(NON_EQUIVALENT "Build without equivalent decomp constraints" ON)
option(USE_AUTO_VCPKG "Use automated vcpkg for dependency management" ON)
option(SSB64_STAGE_CYCLE_DEMO "Boot directly into attract-demo stage cycle for rendering regression testing" OFF)

find_package(Python3 COMPONENTS Interpreter REQUIRED)

################################################################################
# libultraship configuration (set before add_subdirectory)
################################################################################

# O2R (ZIP) only, no MPQ/StormLib
set(INCLUDE_MPQ_SUPPORT OFF CACHE BOOL "" FORCE)
set(GBI_UCODE "F3DEX_GBI_2" CACHE STRING "" FORCE)

# Build Torch as the original standalone sidecar executable.
set(USE_STANDALONE ON CACHE BOOL "" FORCE)
set(BUILD_STORMLIB OFF CACHE BOOL "" FORCE)
set(BUILD_SM64 OFF CACHE BOOL "" FORCE)
set(BUILD_MK64 OFF CACHE BOOL "" FORCE)
set(BUILD_SF64 OFF CACHE BOOL "" FORCE)
set(BUILD_PM64 OFF CACHE BOOL "" FORCE)
set(BUILD_FZERO OFF CACHE BOOL "" FORCE)
set(BUILD_MARIO_ARTIST OFF CACHE BOOL "" FORCE)
set(BUILD_NAUDIO ON CACHE BOOL "" FORCE)
set(BUILD_SSB64 ON CACHE BOOL "" FORCE)

################################################################################
# Subdirectories: libultraship
################################################################################

add_subdirectory(libultraship ${CMAKE_BINARY_DIR}/libultraship)

ExternalProject_Add(TorchExternal
    PREFIX TorchExternal
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/torch
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/torch-install
        -DUSE_STANDALONE=ON
        -DBUILD_STORMLIB=OFF
        -DBUILD_SM64=OFF
        -DBUILD_MK64=OFF
        -DBUILD_SF64=OFF
        -DBUILD_PM64=OFF
        -DBUILD_FZERO=OFF
        -DBUILD_MARIO_ARTIST=OFF
        -DBUILD_NAUDIO=ON
        -DBUILD_SSB64=ON
)

ExternalProject_Get_Property(TorchExternal install_dir)
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(TORCH_EXECUTABLE ${install_dir}/src/TorchExternal-build/$<CONFIGURATION>/torch.exe)
    set(TORCH_SIDECAR_NAME torch.exe)
else()
    set(TORCH_EXECUTABLE ${install_dir}/src/TorchExternal-build/torch)
    set(TORCH_SIDECAR_NAME torch)
endif()

set(SSB64_RELOC_YAML_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/yamls/us/reloc_animations.yml
    ${CMAKE_CURRENT_SOURCE_DIR}/yamls/us/reloc_bonus.yml
    ${CMAKE_CURRENT_SOURCE_DIR}/yamls/us/reloc_effects.yml
    ${CMAKE_CURRENT_SOURCE_DIR}/yamls/us/reloc_extern_data.yml
    ${CMAKE_CURRENT_SOURCE_DIR}/yamls/us/reloc_fighters_common.yml
    ${CMAKE_CURRENT_SOURCE_DIR}/yamls/us/reloc_fighters_main.yml
    ${CMAKE_CURRENT_SOURCE_DIR}/yamls/us/reloc_interface.yml
    ${CMAKE_CURRENT_SOURCE_DIR}/yamls/us/reloc_items.yml
    ${CMAKE_CURRENT_SOURCE_DIR}/yamls/us/reloc_menus.yml
    ${CMAKE_CURRENT_SOURCE_DIR}/yamls/us/reloc_misc_named.yml
    ${CMAKE_CURRENT_SOURCE_DIR}/yamls/us/reloc_movies.yml
    ${CMAKE_CURRENT_SOURCE_DIR}/yamls/us/reloc_scene.yml
    ${CMAKE_CURRENT_SOURCE_DIR}/yamls/us/reloc_stages.yml
    ${CMAKE_CURRENT_SOURCE_DIR}/yamls/us/reloc_submotions.yml
    ${CMAKE_CURRENT_SOURCE_DIR}/yamls/us/reloc_transitions.yml
)

set(SSB64_CREDITS_GENERATED_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/credits/staff.credits.encoded
    ${CMAKE_CURRENT_SOURCE_DIR}/src/credits/staff.credits.metadata
    ${CMAKE_CURRENT_SOURCE_DIR}/src/credits/titles.credits.encoded
    ${CMAKE_CURRENT_SOURCE_DIR}/src/credits/titles.credits.metadata
    ${CMAKE_CURRENT_SOURCE_DIR}/src/credits/info.credits.encoded
    ${CMAKE_CURRENT_SOURCE_DIR}/src/credits/info.credits.metadata
    ${CMAKE_CURRENT_SOURCE_DIR}/src/credits/companies.credits.encoded
    ${CMAKE_CURRENT_SOURCE_DIR}/src/credits/companies.credits.metadata
)

add_custom_target(RegenerateRelocYamls
    BYPRODUCTS ${SSB64_RELOC_YAML_FILES}
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tools/generate_yamls.py
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Regenerating reloc YAML configs"
)

add_custom_target(GenerateRelocArtifacts
    BYPRODUCTS
        ${CMAKE_CURRENT_SOURCE_DIR}/include/reloc_data.h
        ${CMAKE_CURRENT_SOURCE_DIR}/port/resource/RelocFileTable.cpp
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tools/generate_reloc_stubs.py
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tools/generate_reloc_table.py
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating reloc headers and RelocFileTable.cpp"
)

add_custom_target(GenerateCreditsAssets
    BYPRODUCTS ${SSB64_CREDITS_GENERATED_FILES}
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tools/creditsTextConverter.py staff.credits.us.txt
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tools/creditsTextConverter.py titles.credits.us.txt
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tools/creditsTextConverter.py -paragraphFont info.credits.us.txt
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tools/creditsTextConverter.py -paragraphFont companies.credits.us.txt
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/credits
    COMMENT "Encoding credits text assets"
)

add_custom_target(PrepareBuildInputs
    DEPENDS GenerateRelocArtifacts GenerateCreditsAssets
)

add_custom_target(ExtractAssetHeaders
    DEPENDS PrepareBuildInputs
    COMMENT "BattleShip asset/header generation is driven by PrepareBuildInputs"
)

################################################################################
# Collect game sources
################################################################################

# Decomp game code (exclude debug overlay and debug battle/menu)
file(GLOB_RECURSE SSB64_SRC_CREDITS "src/credits/*.c" "src/credits/*.h")
file(GLOB_RECURSE SSB64_SRC_EF      "src/ef/*.c"      "src/ef/*.h")
file(GLOB_RECURSE SSB64_SRC_FT      "src/ft/*.c"      "src/ft/*.h")
file(GLOB_RECURSE SSB64_SRC_GM      "src/gm/*.c"      "src/gm/*.h")
file(GLOB_RECURSE SSB64_SRC_GR      "src/gr/*.c"      "src/gr/*.h")
file(GLOB_RECURSE SSB64_SRC_IF      "src/if/*.c"      "src/if/*.h")
file(GLOB_RECURSE SSB64_SRC_IT      "src/it/*.c"      "src/it/*.h")
file(GLOB_RECURSE SSB64_SRC_LB      "src/lb/*.c"      "src/lb/*.h")
file(GLOB_RECURSE SSB64_SRC_MN      "src/mn/*.c"      "src/mn/*.h")
file(GLOB_RECURSE SSB64_SRC_MP      "src/mp/*.c"      "src/mp/*.h")
file(GLOB_RECURSE SSB64_SRC_MV      "src/mv/*.c"      "src/mv/*.h")
file(GLOB_RECURSE SSB64_SRC_SC      "src/sc/*.c"      "src/sc/*.h")
file(GLOB_RECURSE SSB64_SRC_SYS     "src/sys/*.c"     "src/sys/*.h")
file(GLOB_RECURSE SSB64_SRC_WP      "src/wp/*.c"      "src/wp/*.h")

# Port layer (C++ glue) — excludes port/stubs/ which is compiled as part of ssb64_game
file(GLOB_RECURSE SSB64_SRC_PORT "port/*.cpp" "port/*.c" "port/*.h")
list(FILTER SSB64_SRC_PORT EXCLUDE REGEX "port/stubs/")

# Debug tools (GBI trace + Acmd trace systems)
file(GLOB SSB64_DEBUG_TOOLS
    "debug_tools/gbi_trace/*.c" "debug_tools/gbi_trace/*.h"
    "debug_tools/acmd_trace/*.c" "debug_tools/acmd_trace/*.h"
)

set(SSB64_DECOMP_SOURCES
    ${SSB64_SRC_CREDITS}
    ${SSB64_SRC_EF}
    ${SSB64_SRC_FT}
    ${SSB64_SRC_GM}
    ${SSB64_SRC_GR}
    ${SSB64_SRC_IF}
    ${SSB64_SRC_IT}
    ${SSB64_SRC_LB}
    ${SSB64_SRC_MN}
    ${SSB64_SRC_MP}
    ${SSB64_SRC_MV}
    ${SSB64_SRC_SC}
    ${SSB64_SRC_SYS}
    ${SSB64_SRC_WP}
)

# Exclude .inc.c files (inline data included by other .c files)
list(FILTER SSB64_DECOMP_SOURCES EXCLUDE REGEX "\\.inc\\.c$")

################################################################################
# Excluded directories (not compiled):
#   src/ovl8/   - Debug overlay
#   src/db/     - Debug battle/menu testing
#   src/libultra/ - Replaced by libultraship
################################################################################

# Shared compile definitions used by both the decomp object library and the
# main executable.
set(SSB64_COMPILE_DEFS
    # Region
    REGION_US=1
    VERSION_US=1

    # Port mode
    NON_MATCHING=1
    NON_EQUIVALENT=1
    AVOID_UB=1
    PORT=1

    # GBI / graphics
    F3DEX_GBI_2=1
    _LANGUAGE_C

    # Audio — SSB64 uses the compact "n_audio" microcode (N_MICRO).
    # This controls buffer layout and command set in the synthesis pull chain.
    N_MICRO=1

    # Platform compat
    _USE_MATH_DEFINES

    # ImGui
    CIMGUI_DEFINE_ENUMS_AND_STRUCTS
)

if(SSB64_STAGE_CYCLE_DEMO)
    list(APPEND SSB64_COMPILE_DEFS PORT_STAGE_CYCLE_DEMO=1)
endif()

################################################################################
# Decomp C code — OBJECT library with custom include/ shadowing system headers
################################################################################

add_library(ssb64_game OBJECT ${SSB64_DECOMP_SOURCES})

# Port stub definitions (C code that satisfies N64 linker symbols)
file(GLOB SSB64_STUBS "port/stubs/*.c")
target_sources(ssb64_game PRIVATE ${SSB64_STUBS})

# Selectively include decomp libultra files that provide needed functionality.
# These are pure C implementations that work on any platform.
set(SSB64_LIBULTRA_PORT
    src/libultra/gu/mtxcatf.c       # guMtxCatF
    src/libultra/gu/mtxutil.c        # guMtxF2L, guMtxL2F, guMtxIdentF
    src/libultra/gu/mtxxfmf.c        # guMtxXFMF
    src/libultra/gu/normalize.c      # guNormalize
    # NOTE: sprite.c is NOT included — drawbitmap is assembly-only (MIPS).
    # Sprite functions are stubbed in port/stubs/n64_stubs.c instead.
)
target_sources(ssb64_game PRIVATE ${SSB64_LIBULTRA_PORT})

# N64 audio library — synthesis driver, sequence player, voice management.
# These decomp files are compiled with PORT defines; the CPU Acmd interpreter
# (port/audio/mixer.h) replaces RSP microcode via macro substitution.
set(SSB64_NAUDIO_PORT
    src/libultra/n_audio/n_env.c             # Core: synth driver, audio frame, pull chain, CSP, FX, event queue
    src/libultra/n_audio/n_synaddplayer.c     # Add player to synth driver
    src/libultra/n_audio/n_synallocfx.c       # FX bus allocation
    src/libultra/n_audio/n_synallocvoice.c    # Voice allocation + stealing
    src/libultra/n_audio/n_synfreevoice.c     # Voice free
    src/libultra/n_audio/n_syndelete.c        # Synth delete
    src/libultra/n_audio/n_synsetfxmix.c      # Voice FX mix
    src/libultra/n_audio/n_synsetpan.c        # Voice pan
    src/libultra/n_audio/n_synsetpitch.c      # Voice pitch
    src/libultra/n_audio/n_synsetpriority.c   # Voice priority
    src/libultra/n_audio/n_synsetvol.c        # Voice volume
    src/libultra/n_audio/n_synstartvoiceparam.c # Voice start with params
    src/libultra/n_audio/n_synstopvoice.c     # Voice stop
    src/libultra/n_audio/n_cspplay.c          # CSP play
    src/libultra/n_audio/n_cspsetbank.c       # CSP set bank
    src/libultra/n_audio/n_cspsetfxmix.c      # CSP set FX mix
    src/libultra/n_audio/n_cspsetpriority.c   # CSP set priority
    src/libultra/n_audio/n_cspsetseq.c        # CSP set sequence
    src/libultra/n_audio/n_cspsetvol.c        # CSP set volume
    src/libultra/n_audio/n_cspstop.c          # CSP stop
    src/libultra/n_audio/n_seq.c              # Sequence data reader
    src/libultra/n_audio/n_seqplayer.c        # Sequence player voice handler
    src/libultra/n_audio/n_seqpgetchlvol.c    # Get channel volume
    src/libultra/n_audio/n_seqpgetvol.c       # Get master volume
    src/libultra/audio/cents2ratio.c          # alCents2Ratio — pitch conversion
)
target_sources(ssb64_game PRIVATE ${SSB64_NAUDIO_PORT})

target_compile_definitions(ssb64_game PRIVATE ${SSB64_COMPILE_DEFS})

target_include_directories(ssb64_game PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include    # Custom C headers (shadows system headers)
    ${CMAKE_CURRENT_SOURCE_DIR}/src        # Game headers
    ${CMAKE_CURRENT_SOURCE_DIR}/port       # Port headers (coroutine.h, etc.)
    ${CMAKE_CURRENT_SOURCE_DIR}/debug_tools # Acmd trace (mixer.h calls acmd_trace_log_cmd)
)

# Clang 16+ promoted several historically-warning diagnostics to errors by
# default, which breaks the decomp C code on macOS/Linux. The decomp was
# written against IDO 7.1 and relies on implicit function declarations, loose
# int/pointer conversions, and incompatible library redeclarations — all of
# which MSVC merely warns about. Downgrade those here so the port can build
# without modifying every decomp source file.
if (NOT MSVC)
    # Common warnings that both Clang and GCC accept.
    target_compile_options(ssb64_game PRIVATE
        -Werror=implicit-function-declaration
        -Wno-implicit-int
        -Werror=int-conversion
        -Werror=incompatible-pointer-types
        -Wno-shift-negative-value
        -Wno-parentheses-equality
        -Wno-pointer-sign
        -Wno-unused-value
        -Wno-unused-variable
        -Wno-unused-but-set-variable
        -Wno-unused-function
        -Werror=return-type
    )
    # Clang-only diagnostics. -Wincompatible-library-redeclaration is
    # Clang-specific; -Wreturn-mismatch / -Wconstant-conversion /
    # -Wtautological-constant-out-of-range-compare exist in modern Clang
    # but only landed in GCC 14+ (or never).
    if (CMAKE_C_COMPILER_ID MATCHES "Clang")
        target_compile_options(ssb64_game PRIVATE
            -Werror=incompatible-library-redeclaration
            -Werror=return-mismatch
            -Wno-constant-conversion
            -Wno-tautological-constant-out-of-range-compare
        )
    endif()
endif()

################################################################################
# Executable target — port C++ code + decomp object library
################################################################################

add_executable(${PROJECT_NAME} ${SSB64_SRC_PORT} ${SSB64_DEBUG_TOOLS} $<TARGET_OBJECTS:ssb64_game>)

# User-facing binary name. The CMake target keeps the historical
# `ssb64` identifier (matches the repo, build dirs, internal symbols),
# but the produced executable is `BattleShip` / `BattleShip.exe`.
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "BattleShip")

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set_target_properties(ssb64_game PROPERTIES
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
    )
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
    )
endif()

################################################################################
# Compile definitions
################################################################################

target_compile_definitions(${PROJECT_NAME} PRIVATE ${SSB64_COMPILE_DEFS})

################################################################################
# Include paths (no include/ here — port code uses system headers)
################################################################################

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/port
    ${CMAKE_CURRENT_SOURCE_DIR}/debug_tools
)

################################################################################
# Link libultraship and Torch
################################################################################

add_dependencies(${PROJECT_NAME} libultraship TorchExternal)
target_link_libraries(${PROJECT_NAME} PRIVATE libultraship)

################################################################################
# Platform-specific settings
################################################################################

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_options(${PROJECT_NAME} PRIVATE /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup)
    # DMA functions (osEPiStartDma, osPiStartDma) are provided by LUS.
    # Game DMA code is no-opped via #ifdef PORT in dma.c, so no /FORCE needed.
    target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)
    # Embed the application icon (Explorer / taskbar / window).
    target_sources(${PROJECT_NAME} PRIVATE port/ssb64.rc)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_options(${PROJECT_NAME} PRIVATE -Wl,-export-dynamic)
endif()

################################################################################
# Copy runtime data files to build directory
################################################################################

configure_file(
    ${CMAKE_SOURCE_DIR}/gamecontrollerdb.txt
    ${CMAKE_BINARY_DIR}/gamecontrollerdb.txt
    COPYONLY
)

add_custom_target(GenerateF3DO2R
    BYPRODUCTS ${CMAKE_SOURCE_DIR}/f3d.o2r
    COMMAND ${CMAKE_COMMAND} -E rm -f ${CMAKE_SOURCE_DIR}/f3d.o2r
    COMMAND ${CMAKE_COMMAND} -E tar cf ${CMAKE_SOURCE_DIR}/f3d.o2r --format=zip shaders
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/libultraship/src/fast
    COMMENT "Packaging Fast3D shaders into f3d.o2r"
)

add_dependencies(${PROJECT_NAME} GenerateF3DO2R)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets/custom/fonts
    COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/yamls/us
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/f3d.o2r
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/f3d.o2r
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/gamecontrollerdb.txt
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/gamecontrollerdb.txt
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/assets/custom/fonts/Montserrat-Regular.ttf
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets/custom/fonts/Montserrat-Regular.ttf
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/assets/custom/fonts/Inconsolata-Regular.ttf
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets/custom/fonts/Inconsolata-Regular.ttf
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/config.yml
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/config.yml
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/yamls/us
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/yamls/us
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${TORCH_EXECUTABLE}
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/${TORCH_SIDECAR_NAME}
)

################################################################################
# Asset extraction targets
################################################################################

# Accept .z64 (big-endian), .n64 (little-endian), or .v64 (byteswapped).
# Torch's Cartridge::Normalize converts non-z64 byte orders in-memory.
set(SSB64_BASEROM "")
foreach(ext z64 n64 v64)
    if(EXISTS "${CMAKE_SOURCE_DIR}/baserom.us.${ext}")
        set(SSB64_BASEROM "${CMAKE_SOURCE_DIR}/baserom.us.${ext}")
        break()
    endif()
endforeach()
if(NOT SSB64_BASEROM)
    set(SSB64_BASEROM "${CMAKE_SOURCE_DIR}/baserom.us.z64")
endif()

add_custom_target(ExtractAssets
    DEPENDS TorchExternal PrepareBuildInputs GenerateF3DO2R
    COMMAND ${TORCH_EXECUTABLE} o2r ${SSB64_BASEROM} -s ${CMAKE_SOURCE_DIR} -d ${CMAKE_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/BattleShip.o2r
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/BattleShip.o2r
    COMMENT "Extracting assets from ${SSB64_BASEROM} into O2R archive"
)

add_custom_target(GenerateBattleShipO2R
    DEPENDS ExtractAssets
    COMMENT "Generating BattleShip.o2r"
)

add_custom_target(GeneratePortO2R
    DEPENDS TorchExternal PrepareBuildInputs
    COMMAND ${TORCH_EXECUTABLE} pack ${CMAKE_SOURCE_DIR}/assets ${CMAKE_SOURCE_DIR}/ssb64.o2r o2r
    COMMENT "Packing port-specific assets into ssb64.o2r"
)
