#[[
Utilities for adding binary data targets to JUCE projects.

## Includes:
- LemonsJuceUtilities
- LemonsTranslationFileGeneration


## Function:

### lemons_add_resources_folder {#lemons_add_resources_folder}
```
lemons_add_resources_folder (TARGET <target> ASSET_FOLDER <folder> 
                            [HEADER_NAME <header.h>] [NAMESPACE <namespace>] 
                            [OUTPUT_TARGET <targetName>]
                            [TRANSLATIONS])
```

Adds a JUCE binary data folder for the specified `<target>`, and populates it with all the files found in `<folder>`. 
`<folder>` is relative to your project's root directory (ie, the value of `${PROJECT_SOURCE_DIR}` when this function is called).

If `[OUTPUT_TARGET]` is present, `targetName` will be the name of the generated resources target; otherwise, it will default to `${PROJECT_NAME}-Assets`.

If the `TRANSLATIONS` option is present, then this will call [lemons_generate_translation_files()](@ref lemons_generate_translation_files), placing the output in `<folder>/translations` and also bundling these files in the generated resources target.

]]


include_guard (GLOBAL)

include (LemonsJuceUtilities)


function (lemons_add_resources_folder)

    set (options TRANSLATIONS)
    set (oneValueArgs TARGET ASSET_FOLDER HEADER_NAME NAMESPACE OUTPUT_TARGET)

    cmake_parse_arguments (LEMONS_RSRC_FLDR "${options}" "${oneValueArgs}" "" ${ARGN})

    if (NOT LEMONS_RSRC_FLDR_TARGET)
        message (FATAL_ERROR "Target name not specified in call to ${CMAKE_CURRENT_FUNCTION}!")
        return()
    endif()

    if (NOT LEMONS_RSRC_FLDR_ASSET_FOLDER)
        message (FATAL_ERROR "Folder name not specified in call to ${CMAKE_CURRENT_FUNCTION}!")
        return()
    endif()

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
        if (LEMONS_RSRC_FLDR_TRANSLATIONS)
            include (LemonsTranslationFileGeneration)
            lemons_generate_translation_files (TARGET "${LEMONS_RSRC_FLDR_TARGET}" FOLDER "${dest_folder}/translations")
        endif()

        file (GLOB_RECURSE files "${dest_folder}/*.*")

        if (NOT files)
            message (AUTHOR_WARNING "No files found for inclusion in resources target!")
            return()
        endif()

        if (NOT LEMONS_RSRC_FLDR_HEADER_NAME)
            set (LEMONS_RSRC_FLDR_HEADER_NAME "BinaryData.h")
        endif()

        if (NOT LEMONS_RSRC_FLDR_FOLDER_NAMESPACE)
            set (LEMONS_RSRC_FLDR_FOLDER_NAMESPACE "BinaryData")
        endif()

        juce_add_binary_data (${resourcesTarget} 
                              SOURCES ${files}
                              HEADER_NAME "${LEMONS_RSRC_FLDR_HEADER_NAME}"
                              NAMESPACE "${LEMONS_RSRC_FLDR_FOLDER_NAMESPACE}")

        set_target_properties (${resourcesTarget} PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
        target_compile_definitions (${resourcesTarget} INTERFACE LEMONS_HAS_BINARY_DATA=1)

        if (NOT TARGET ${resourcesTarget})
            message (AUTHOR_WARNING "Error creating resources target.")
            return()
        endif()
    endif()

    juce_add_bundle_resources_directory (${LEMONS_RSRC_FLDR_TARGET} ${dest_folder})

    target_link_libraries (${LEMONS_RSRC_FLDR_TARGET} PRIVATE ${resourcesTarget})
    
endfunction()
