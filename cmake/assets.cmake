function (bv_add_resources_folder)

    set (options "")
    set (oneValueArgs TARGET FOLDER)
    set (multiValueArgs "")

    cmake_parse_arguments (BV_RSRC_FLDR "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (NOT DEFINED BV_RSRC_FLDR_TARGET)
        message (WARNING "Target name not specified in call to bv_add_resources_folder")
        return()
    endif()

    if (NOT DEFINED BV_RSRC_FLDR_FOLDER)
        message (WARNING "Folder name not specified in call to bv_add_resources_folder")
        return()
    endif()

    set (resourcesTarget ${BV_RSRC_FLDR_TARGET}-Assets)

    if (NOT TARGET ${resourcesTarget})
        file (GLOB_RECURSE files "${BV_RSRC_FLDR_FOLDER}/*.*")

        if (files)
            juce_add_binary_data (${resourcesTarget} SOURCES ${files})
            set_target_properties (${resourcesTarget} PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
            target_compile_definitions (${resourcesTarget} INTERFACE BV_HAS_BINARY_DATA=1)
        endif()
    endif()

    juce_add_bundle_resources_directory (${BV_RSRC_FLDR_TARGET} ${BV_RSRC_FLDR_FOLDER})

    if (NOT TARGET ${resourcesTarget})
        message (WARNING "Error -- resources target not found, or could not be created for ${BV_RSRC_FLDR_TARGET}")
        return()
    endif()

    target_link_libraries (${BV_RSRC_FLDR_TARGET} PRIVATE ${resourcesTarget})
    
endfunction()