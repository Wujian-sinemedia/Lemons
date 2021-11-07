include_guard (GLOBAL)

include (LemonsFileUtils)
include (LemonsJuceUtilities)

#

function (lemons_add_juce_modules dir)
    lemons_subdir_list (RESULT moduleFolders DIR "${dir}")

    foreach (folder ${moduleFolders})
        juce_add_module ("${dir}/${folder}")
    endforeach()
endfunction()

#

macro (_lemons_add_module_subcategory target)
    if (NOT TARGET ${target})
        add_library (${target} INTERFACE)
    endif()

    lemons_subdir_list (RESULT moduleFolders DIR "${CMAKE_CURRENT_LIST_DIR}")

    foreach (folder ${moduleFolders})
        juce_add_module ("${CMAKE_CURRENT_LIST_DIR}/${folder}")
        target_link_libraries (${target} INTERFACE ${folder})
    endforeach()
endmacro()
