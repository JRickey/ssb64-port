# ssb64_add_mod_o2r_target(MOD_NAME)
#
# Adds an opt-in custom target `mod_<MOD_NAME>_o2r` that packs the mod's
# output/ directory into a single distributable .o2r archive using
# torch.exe's `pack` subcommand. NOT added to ALL — invoke explicitly:
#
#     cmake --build build --target mod_<MOD_NAME>_o2r
#
# Output: ${CMAKE_CURRENT_SOURCE_DIR}/output/<MOD_NAME>.o2r
#
# Distribute the .o2r by copying it into a player's mods/ folder. The
# runtime loader (port.cpp:MountModsDir) handles .o2r and folder
# layouts identically through LUS's ArchiveManager — keep the folder
# install for fast hot-reload during development, ship the .o2r.
function(ssb64_add_mod_o2r_target MOD_NAME)
    if(NOT DEFINED TORCH_EXECUTABLE)
        message(WARNING "ssb64_add_mod_o2r_target(${MOD_NAME}): TORCH_EXECUTABLE not defined; o2r packaging unavailable")
        return()
    endif()

    set(WS_OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/output)
    set(WS_O2R    ${WS_OUTPUT}/${MOD_NAME}.o2r)

    add_custom_target(mod_${MOD_NAME}_o2r
        COMMAND ${CMAKE_COMMAND} -E remove -f ${WS_O2R}
        COMMAND ${TORCH_EXECUTABLE} pack ${WS_OUTPUT} ${WS_O2R} o2r
        DEPENDS mod_${MOD_NAME} TorchExternal
        COMMENT "Packing ${MOD_NAME} into ${WS_O2R}"
        VERBATIM
    )
endfunction()
