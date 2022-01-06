#[[
Utilities for adding custom JUCE modules to projects.

## Includes:
- LemonsFileUtils
- LemonsJuceUtilities


## Function:

### lemons_add_juce_modules
```
lemons_add_juce_modules (<directory>)
```
Adds any/all JUCE modules that are nested subdirectories within the specified directory.

]]


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

        list (APPEND lemons_all_modules "${folder}")
        set (lemons_all_modules ${lemons_all_modules} CACHE INTERNAL "")
    endforeach()
endmacro()
