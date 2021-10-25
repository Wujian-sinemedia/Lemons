include (translations.cmake)

#
# lemons_add_resources_folder
# configures a binary resources target for the specified product target. If you're making a JUCE plugin, you should call this function with your plugin's shared code target.
#
# INPUTS:
# TARGET : the name of the product target to link the generated resources target to. The resources target will be named "$TARGET-Assets". If you're making a JUCE plugin, this should be your plugin's shared code target (usually named the same as your product)
# FOLDER : the absolute path of the assets folder to use to populate the resources target. This folder can have any nested directory structure -- glob-resurs'ing is used to generate the list of files.
# 
function (lemons_add_resources_folder)

    set (options "")
    set (oneValueArgs TARGET FOLDER)
    set (multiValueArgs "")

    cmake_parse_arguments (LEMONS_RSRC_FLDR "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (NOT LEMONS_RSRC_FLDR_TARGET)
        message (WARNING "Target name not specified in call to lemons_add_resources_folder!")
        return()
    endif()

    if (NOT LEMONS_RSRC_FLDR_FOLDER)
        message (WARNING "Folder name not specified in call to lemons_add_resources_folder!")
        return()
    endif()

    set (resourcesTarget "${LEMONS_RSRC_FLDR_TARGET}-Assets")

    # create resources target, if needed 
    if (NOT TARGET ${resourcesTarget})

        # lemons_generate_translation_files ("${LEMONS_RSRC_FLDR_FOLDER}")

        file (GLOB_RECURSE files "${LEMONS_RSRC_FLDR_FOLDER}/*.*")

        if (files)
            juce_add_binary_data (${resourcesTarget} SOURCES ${files})
            set_target_properties (${resourcesTarget} PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
            target_compile_definitions (${resourcesTarget} INTERFACE LEMONS_HAS_BINARY_DATA=1)
        else()
            message (WARNING "No files found for inclusion in resources target!")
            return()
        endif()

        if (NOT TARGET ${resourcesTarget})
            message (WARNING "Error creating resources target.")
            return()
        endif()

    endif()

    juce_add_bundle_resources_directory (${LEMONS_RSRC_FLDR_TARGET} ${LEMONS_RSRC_FLDR_FOLDER})

    target_link_libraries (${LEMONS_RSRC_FLDR_TARGET} PRIVATE ${resourcesTarget})
    
endfunction()
