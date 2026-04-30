# Provides the tinyxml2::tinyxml2 IMPORTED target on systems where
# libtinyxml2-dev only ships pkg-config metadata (tinyxml2.pc) but no
# CMake config (tinyxml2Config.cmake).
#
# Concretely this matters for Ubuntu 22.04 jammy, which ships
# libtinyxml2-dev 9.0.0 with pkg-config only. Newer distros (Ubuntu
# 24.04 noble libtinyxml2 10.x, Fedora 39+ tinyxml2 10.x+, Arch,
# Homebrew, vcpkg) all ship a CMake config and skip the fallback.
#
# Strategy: Module mode runs first by default, so this file is
# consulted before <package>Config.cmake. Try CONFIG mode explicitly
# (cheap re-entry, can't recurse since CONFIG bypasses Module mode).
# If that finds a config, defer to it. Otherwise fall back to
# pkg-config and assemble an IMPORTED target by hand.

find_package(tinyxml2 CONFIG QUIET)
if(tinyxml2_FOUND)
    return()
endif()

find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(PC_TINYXML2 QUIET tinyxml2)
endif()

find_path(tinyxml2_INCLUDE_DIR
    NAMES tinyxml2.h
    HINTS ${PC_TINYXML2_INCLUDE_DIRS}
)
find_library(tinyxml2_LIBRARY
    NAMES tinyxml2
    HINTS ${PC_TINYXML2_LIBRARY_DIRS}
)

set(tinyxml2_VERSION ${PC_TINYXML2_VERSION})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(tinyxml2
    REQUIRED_VARS tinyxml2_LIBRARY tinyxml2_INCLUDE_DIR
    VERSION_VAR tinyxml2_VERSION
)

if(tinyxml2_FOUND AND NOT TARGET tinyxml2::tinyxml2)
    add_library(tinyxml2::tinyxml2 UNKNOWN IMPORTED)
    set_target_properties(tinyxml2::tinyxml2 PROPERTIES
        IMPORTED_LOCATION "${tinyxml2_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${tinyxml2_INCLUDE_DIR}"
    )
endif()

mark_as_advanced(tinyxml2_INCLUDE_DIR tinyxml2_LIBRARY)
