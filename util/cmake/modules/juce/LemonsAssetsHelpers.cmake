#[[
Utilities for adding binary data targets to JUCE projects.

## Function:

### lemons_add_resources_folder {#lemons_add_resources_folder}
```
lemons_add_resources_folder (TARGET <target> ASSET_FOLDER <folder>
                            [OUTPUT_TARGET <targetName>])
```

Adds a JUCE binary data folder for the specified `<target>`, and populates it with all the files found in `<folder>`. 
if `<folder>` is a relative path, it will be evaluated relative to your project's root directory (ie, the value of `${PROJECT_SOURCE_DIR}` when this function is called). You may also pass an absolute path.

If `[OUTPUT_TARGET]` is present, `targetName` will be the name of the generated resources target; otherwise, it will default to `${PROJECT_NAME}-Assets`.

]]


include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

include (LemonsJuceUtilities)
include (LemonsFileUtils)

lemons_warn_if_not_processing_project()


function (lemons_add_resources_folder)

    set (oneValueArgs TARGET ASSET_FOLDER OUTPUT_TARGET)

    cmake_parse_arguments (LEMONS_RSRC_FLDR "" "${oneValueArgs}" "" ${ARGN})

    lemons_require_function_arguments (LEMONS_RSRC_FLDR TARGET ASSET_FOLDER)

    cmake_path (IS_ABSOLUTE LEMONS_RSRC_FLDR_ASSET_FOLDER folder_path_is_absolute)

    if (folder_path_is_absolute)
        set (dest_folder "${LEMONS_RSRC_FLDR_ASSET_FOLDER}")
    else()
        set (dest_folder "${PROJECT_SOURCE_DIR}/${LEMONS_RSRC_FLDR_ASSET_FOLDER}")
    endif()

    if (LEMONS_RSRC_FLDR_OUTPUT_TARGET)
        set (resourcesTarget "${LEMONS_RSRC_FLDR_OUTPUT_TARGET}")
    else()
        set (resourcesTarget "${PROJECT_NAME}-Assets")
    endif()

    if (NOT TARGET ${resourcesTarget})
        if (TARGET ${LEMONS_RSRC_FLDR_TARGET}::${resourcesTarget})
            message (AUTHOR_WARNING "Target ${LEMONS_RSRC_FLDR_TARGET}::${resourcesTarget} exists, but target ${resourcesTarget} not found!")
        endif()

        lemons_subdir_list (DIR "${dest_folder}" FILES FULL_PATHS RECURSE RESULT files)

        if (NOT files)
            message (AUTHOR_WARNING "No files found for inclusion in resources target!")
            return()
        endif()

        juce_add_binary_data (${resourcesTarget} SOURCES ${files})

        set_target_properties (${resourcesTarget} PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
        target_compile_definitions (${resourcesTarget} INTERFACE LEMONS_HAS_BINARY_DATA=1)

        if (NOT TARGET ${resourcesTarget})
            message (AUTHOR_WARNING "Error creating resources target.")
            return()
        endif()

        add_library (${LEMONS_RSRC_FLDR_TARGET}::${resourcesTarget} ALIAS ${resourcesTarget})
    endif()

    if (NOT TARGET ${LEMONS_RSRC_FLDR_TARGET}::${resourcesTarget})
        message (AUTHOR_WARNING "Error creating resources target - target ${LEMONS_RSRC_FLDR_TARGET}::${resourcesTarget} not found.")
        return()
    endif()

    juce_add_bundle_resources_directory (${LEMONS_RSRC_FLDR_TARGET} ${dest_folder})

    target_link_libraries (${LEMONS_RSRC_FLDR_TARGET} PRIVATE ${LEMONS_RSRC_FLDR_TARGET}::${resourcesTarget})
    
endfunction()
