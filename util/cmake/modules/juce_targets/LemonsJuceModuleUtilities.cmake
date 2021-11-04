include_guard (GLOBAL)

include (LemonsFileUtils)

#

function (lemons_add_juce_modules dir)
    lemons_subdir_list (RESULT moduleFolders DIR "${dir}")

    foreach (folder ${moduleFolders})
        juce_add_module ("${dir}/${folder}")
    endforeach()
endfunction()

#

function (_lemons_add_module_subcategory dir target)
    if (NOT TARGET ${target})
        add_library (${target} INTERFACE)
    endif()

    set (path "${Lemons_SOURCE_DIR}/modules/${dir}")

    lemons_subdir_list (RESULT moduleFolders DIR "${path}")

    foreach (folder ${moduleFolders})
        juce_add_module ("${path}/${folder}")
        target_link_libraries (${target} INTERFACE ${folder})
    endforeach()
endfunction()
