# RelocData.cmake — compile-from-source pipeline for decomp/src/relocData/*.c
#
# Drives the new optional path that produces BattleShip.fromsource.o2r — a
# RelocFile-resource archive containing source-compiled equivalents of the
# reloc data Torch normally extracts from the ROM. The archive is loaded
# ahead of BattleShip.o2r at runtime when the file is present, shadowing
# Torch entries for matching paths via LUS's FIFO ArchiveManager lookup.
#
# Cross-platform constraint: relocData .c files cast (u32)&symbol_addr in
# static initialisers. That's only a constant expression on a 32-bit-pointer
# target, so we always cross-compile to i686-pc-linux-gnu (ELF). MSVC can't
# do that, so the entire subsystem is gated on clang availability — without
# clang the target is omitted and the build runs exactly as today (Torch-
# extracted reloc data only).

find_program(CLANG_EXECUTABLE NAMES clang clang-cl)

if(NOT CLANG_EXECUTABLE)
    message(STATUS "RelocData: clang not found — skipping BuildBattleShipFromSource. "
                   "Install LLVM/clang to enable from-source reloc data "
                   "(Windows: 'winget install LLVM.LLVM' or VS 'C++ Clang tools'; "
                   "macOS: ships with Xcode CLI tools; Linux: 'apt install clang').")
    return()
endif()

message(STATUS "RelocData: clang found at ${CLANG_EXECUTABLE} — "
               "BuildBattleShipFromSource enabled")

set(RELOC_FROMSOURCE_DIR ${CMAKE_BINARY_DIR}/reloc_resources)
set(RELOC_OBJECTS_DIR ${CMAKE_BINARY_DIR}/reloc_objects)
file(MAKE_DIRECTORY ${RELOC_FROMSOURCE_DIR})
file(MAKE_DIRECTORY ${RELOC_OBJECTS_DIR})

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
    set(obj ${RELOC_OBJECTS_DIR}/${file_id}.o)

    add_custom_command(
        OUTPUT ${out}
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/tools/build_reloc_resource.py
            --src ${source_path}
            --reloc ${reloc_path}
            --file-id ${file_id}
            --clang ${CLANG_EXECUTABLE}
            --include-dir ${CMAKE_SOURCE_DIR}/decomp/include
            --include-dir ${CMAKE_SOURCE_DIR}/decomp/src
            --include-dir ${CMAKE_SOURCE_DIR}/decomp/src/relocData
            --obj-out ${obj}
            --output ${out}
        DEPENDS ${source_path} ${reloc_path}
                ${CMAKE_SOURCE_DIR}/tools/build_reloc_resource.py
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
function(finalize_battleship_from_source archive_output)
    set(manifest ${CMAKE_BINARY_DIR}/reloc_fromsource_manifest.txt)
    set(manifest_lines "")
    foreach(entry ${SSB64_RELOC_FROMSOURCE_PATHS})
        list(APPEND manifest_lines ${entry})
    endforeach()
    string(REPLACE ";" "\n" manifest_content "${manifest_lines}")
    file(WRITE ${manifest} "${manifest_content}\n")

    add_custom_command(
        OUTPUT ${archive_output}
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/tools/pack_reloc_archive.py
            --manifest ${manifest}
            --reloc-dir ${RELOC_FROMSOURCE_DIR}
            --output ${archive_output}
        DEPENDS ${SSB64_RELOC_FROMSOURCE_OUTPUTS}
                ${CMAKE_SOURCE_DIR}/tools/pack_reloc_archive.py
        COMMENT "Packing BattleShip.fromsource.o2r (${archive_output})"
        VERBATIM
    )

    add_custom_target(BuildBattleShipFromSource ALL
        DEPENDS ${archive_output}
    )

    # Copy archive next to the executable so the runtime PortLocateFile
    # finds it without extra -E setup. Same pattern ExtractAssets uses.
    add_custom_command(TARGET BuildBattleShipFromSource POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${archive_output}
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/BattleShip.fromsource.o2r
        VERBATIM
    )
endfunction()
