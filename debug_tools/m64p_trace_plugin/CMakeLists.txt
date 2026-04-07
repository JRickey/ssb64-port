cmake_minimum_required(VERSION 3.16)
project(mupen64plus-video-trace C)

# Build as a shared library (Mupen64Plus dynamically loads plugins)
add_library(mupen64plus-video-trace SHARED
    gfx_trace_plugin.c
    ../gbi_trace/gbi_decoder.c
)

target_include_directories(mupen64plus-video-trace PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# On Linux/macOS, hide internal symbols
if(NOT WIN32)
    set_target_properties(mupen64plus-video-trace PROPERTIES
        C_VISIBILITY_PRESET hidden
    )
endif()

# Output name without lib prefix on Linux
set_target_properties(mupen64plus-video-trace PROPERTIES
    PREFIX ""
)
