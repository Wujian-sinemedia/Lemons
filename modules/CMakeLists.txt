function (_lemons_add_juce_modules_internal dir list)

    lemons_subdir_list (RESULT moduleFolders DIR ${dir})

    foreach (folder ${moduleFolders})
        juce_add_module (${dir}/${folder})
    endforeach()

    list (APPEND ${list} ${moduleFolders} PARENT_SCOPE)
endfunction()

macro (_lemons_add_module_subcategory folder list)
    _lemons_add_juce_modules_internal ("${CMAKE_CURRENT_LIST_DIR}/${folder}" ${list})
    set (${list} ${${list}} PARENT_SCOPE)
endmacro()

_lemons_add_module_subcategory (Plugins      LEMONS_PLUGIN_ONLY_MODULES)
_lemons_add_module_subcategory (Audio        LEMONS_PLUGIN_ONLY_MODULES)
_lemons_add_module_subcategory (Common       LEMONS_JUCE_MODULES)
_lemons_add_module_subcategory (Connectivity LEMONS_JUCE_MODULES)
_lemons_add_module_subcategory (GUI          LEMONS_JUCE_MODULES)

set (LEMONS_APP_ONLY_MODULES lemons_app_utils PARENT_SCOPE)

#

target_link_libraries (lemons_core   INTERFACE Vecops)
target_link_libraries (mts_esp_cpp   INTERFACE MTS-ESP)
target_link_libraries (lemons_plugin INTERFACE AbletonLink)

if (LEMONS_BUILD_TESTS)
    target_compile_definitions (lemons_core INTERFACE LEMONS_UNIT_TESTS=1)
endif()

#

function (lemons_add_juce_modules dir)
    _lemons_add_juce_modules_internal (${dir} dummylist)
endfunction()