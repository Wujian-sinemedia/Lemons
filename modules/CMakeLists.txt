include (LemonsFileUtils)

function (_lemons_add_module_subcategory dir target)
    if (NOT TARGET ${target})
        add_library (${target} INTERFACE)
    endif()

    set (dir "${CMAKE_CURRENT_LIST_DIR}/${dir}")

    lemons_subdir_list (RESULT moduleFolders DIR "${dir}")

    foreach (folder ${moduleFolders})
        juce_add_module ("${dir}/${folder}")
        target_link_libraries (${target} INTERFACE ${folder})
    endforeach()
endfunction()


_lemons_add_module_subcategory (Plugins      LemonsPluginModules)
_lemons_add_module_subcategory (Audio        LemonsPluginModules)
_lemons_add_module_subcategory (Common       LemonsCommonModules)
_lemons_add_module_subcategory (Connectivity LemonsCommonModules)
_lemons_add_module_subcategory (GUI          LemonsCommonModules)
_lemons_add_module_subcategory (Apps         LemonsAppModules)

#

CPMAddPackage (
        NAME MTS-ESP
        GITHUB_REPOSITORY ODDSound/MTS-ESP
        GIT_TAG origin/main)

add_library (MTS-ESP INTERFACE)

target_include_directories (MTS-ESP INTERFACE "${MTS-ESP_SOURCE_DIR}/Client")

target_link_libraries (mts_esp_cpp   INTERFACE MTS-ESP)

#

if (LEMONS_BUILD_TESTS)
    target_compile_definitions (lemons_core INTERFACE LEMONS_UNIT_TESTS=1)
endif()


add_library (LemonsModules INTERFACE)
target_link_libraries (LemonsModules INTERFACE LemonsPluginModules LemonsCommonModules LemonsAppModules)
