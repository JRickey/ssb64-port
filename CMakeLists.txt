cmake_minimum_required(VERSION 3.24.0 FATAL_ERROR)

project(ssb64 LANGUAGES C CXX)

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin" OR CMAKE_SYSTEM_NAME STREQUAL "iOS")
    enable_language(OBJCXX)
    set(CMAKE_OBJCXX_FLAGS "${CMAKE_OBJCXX_FLAGS} -fobjc-arc")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

################################################################################
# Options
################################################################################

option(NON_MATCHING "Build without matching decomp constraints" ON)
option(NON_EQUIVALENT "Build without equivalent decomp constraints" ON)
option(USE_AUTO_VCPKG "Use automated vcpkg for dependency management" ON)

################################################################################
# libultraship configuration (set before add_subdirectory)
################################################################################

# O2R (ZIP) only, no MPQ/StormLib
set(INCLUDE_MPQ_SUPPORT OFF CACHE BOOL "" FORCE)
set(GBI_UCODE "F3DEX_GBI_2" CACHE STRING "" FORCE)

################################################################################
# Subdirectories: libultraship and Torch
################################################################################

add_subdirectory(libultraship ${CMAKE_BINARY_DIR}/libultraship)
add_subdirectory(torch ${CMAKE_BINARY_DIR}/torch)

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

# Port layer (C++ glue)
file(GLOB_RECURSE SSB64_SRC_PORT "port/*.cpp" "port/*.c" "port/*.h")

set(SSB64_ALL_SOURCES
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
    ${SSB64_SRC_PORT}
)

# Exclude .inc.c files (inline data included by other .c files)
list(FILTER SSB64_ALL_SOURCES EXCLUDE REGEX "\\.inc\\.c$")

################################################################################
# Excluded directories (not compiled):
#   src/ovl8/   - Debug overlay
#   src/db/     - Debug battle/menu testing
#   src/libultra/ - Replaced by libultraship
################################################################################

################################################################################
# Executable target
################################################################################

add_executable(${PROJECT_NAME} ${SSB64_ALL_SOURCES})

################################################################################
# Compile definitions
################################################################################

target_compile_definitions(${PROJECT_NAME} PRIVATE
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

    # Platform compat
    _USE_MATH_DEFINES

    # ImGui
    CIMGUI_DEFINE_ENUMS_AND_STRUCTS
)

################################################################################
# Include paths
################################################################################

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/port
)

################################################################################
# Link libultraship and Torch
################################################################################

add_dependencies(${PROJECT_NAME} libultraship)
target_link_libraries(${PROJECT_NAME} PRIVATE libultraship torch)

################################################################################
# Platform-specific settings
################################################################################

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_options(${PROJECT_NAME} PRIVATE /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup)
    target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_options(${PROJECT_NAME} PRIVATE -Wl,-export-dynamic)
endif()

################################################################################
# Torch asset extraction target
################################################################################

# Build Torch as a standalone tool for asset extraction
include(ExternalProject)
ExternalProject_Add(TorchExternal
    PREFIX TorchExternal
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/torch
    CMAKE_ARGS
        -DCMAKE_BUILD_TYPE=Release
        -DUSE_STANDALONE=ON
        -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/torch-install
)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(TORCH_EXECUTABLE ${CMAKE_BINARY_DIR}/torch-install/bin/torch.exe)
else()
    set(TORCH_EXECUTABLE ${CMAKE_BINARY_DIR}/torch-install/bin/torch)
endif()

add_custom_target(ExtractAssets
    DEPENDS TorchExternal
    COMMAND ${TORCH_EXECUTABLE} o2r ${CMAKE_SOURCE_DIR}/baserom.us.z64
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Extracting assets from baserom.us.z64 into O2R archive"
)

add_custom_target(GeneratePortO2R
    DEPENDS TorchExternal
    COMMAND ${TORCH_EXECUTABLE} pack assets ssb64.o2r o2r
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Packing port-specific assets into ssb64.o2r"
)
