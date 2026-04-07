cmake_minimum_required(VERSION 3.16)
project(m64p-trace-plugins C)

# === RSP Trace Plugin (primary — for emulator-side DL + audio capture) ===
add_library(mupen64plus-rsp-trace SHARED
    rsp_trace_plugin.c
    ../gbi_trace/gbi_decoder.c
    ../acmd_trace/acmd_decoder.c
)

target_include_directories(mupen64plus-rsp-trace PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

set_target_properties(mupen64plus-rsp-trace PROPERTIES PREFIX "")

# === GFX Trace Plugin (stub — needed alongside RSP plugin) ===
add_library(mupen64plus-video-trace SHARED
    gfx_trace_plugin.c
    ../gbi_trace/gbi_decoder.c
)

target_include_directories(mupen64plus-video-trace PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

set_target_properties(mupen64plus-video-trace PROPERTIES PREFIX "")

if(NOT WIN32)
    set_target_properties(mupen64plus-rsp-trace PROPERTIES C_VISIBILITY_PRESET hidden)
    set_target_properties(mupen64plus-video-trace PROPERTIES C_VISIBILITY_PRESET hidden)
endif()
