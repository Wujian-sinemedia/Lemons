#[[
Utilities for adding custom JUCE modules to projects.

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
include (LemonsCmakeDevTools)

#

function (lemons_add_juce_modules dir)
    lemons_subdir_list (RESULT moduleFolders DIR "${dir}")

    foreach (folder ${moduleFolders})
        juce_add_module ("${dir}/${folder}")
    endforeach()
endfunction()

#

define_property (TARGET 
                 PROPERTY OriginalModuleNames INHERITED
                 BRIEF_DOCS "Original juce module names"
                 FULL_DOCS "The original names of the lemons juce module targets, without the Lemons:: prefix.")

define_property (TARGET
                 PROPERTY ModuleCategoryNames INHERITED
                 BRIEF_DOCS "Juce module category names"
                 FULL_DOCS "The original names of the lemons juce module category targets, without the Lemons:: prefix.")

#

function (_lemons_add_module_subcategory)

    cmake_parse_arguments (LEMONS_SUBMOD "" "TARGET" "CATEGORY_DEPS" ${ARGN})

    lemons_require_function_arguments (LEMONS_SUBMOD TARGET)

    if (NOT TARGET ${LEMONS_SUBMOD_TARGET})
        add_library (${LEMONS_SUBMOD_TARGET} INTERFACE)
    endif()

    if (NOT TARGET AllLemonsModules)
        add_library (AllLemonsModules INTERFACE)
    endif()

    lemons_subdir_list (RESULT moduleFolders DIR "${CMAKE_CURRENT_LIST_DIR}")

    foreach (folder ${moduleFolders})
        if (TARGET ${folder})
            message (AUTHOR_WARNING "Duplicate juce module found: ${folder}")
            continue()
        endif()

        juce_add_module ("${CMAKE_CURRENT_LIST_DIR}/${folder}" ALIAS_NAMESPACE Lemons)

        target_link_libraries (${LEMONS_SUBMOD_TARGET} INTERFACE Lemons::${folder})
        target_link_libraries (AllLemonsModules INTERFACE Lemons::${folder})

        lemons_append_to_target_property_list (TARGET AllLemonsModules PROPERTY OriginalModuleNames ADD ${folder})
    endforeach()

    foreach (categoryDependancy ${LEMONS_SUBMOD_CATEGORY_DEPS})
        include (${categoryDependancy})

        target_link_libraries (${LEMONS_SUBMOD_TARGET} INTERFACE Lemons::${categoryDependancy})
        target_link_libraries (AllLemonsModules INTERFACE Lemons::${categoryDependancy})

        lemons_append_to_target_property_list (TARGET AllLemonsModules PROPERTY ModuleCategoryNames ADD ${categoryDependancy})
    endforeach()

    add_library (Lemons::${LEMONS_SUBMOD_TARGET} ALIAS ${LEMONS_SUBMOD_TARGET})
endfunction()
