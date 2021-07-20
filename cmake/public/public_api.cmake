function (bv_add_resources_folder)

    set (options "")
    set (oneValueArgs TARGET FOLDER)
    set (multiValueArgs "")

    cmake_parse_arguments (BV_RSRC_FLDR "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (NOT DEFINED BV_RSRC_FLDR_TARGET)
        bv_print_warning ("Target name not specified in call to bv_add_resources_folder")
        return()
    endif()

    if (NOT DEFINED BV_RSRC_FLDR_FOLDER)
        bv_print_warning ("Folder name not specified in call to bv_add_resources_folder")
        return()
    endif()

    set (resourcesTarget ${BV_RSRC_FLDR_TARGET}-Assets)

    if (NOT TARGET ${resourcesTarget})
        file (GLOB_RECURSE files "${BV_RSRC_FLDR_FOLDER}/*.*")
        juce_add_binary_data (${resourcesTarget} SOURCES ${files})
        set_target_properties (${resourcesTarget} PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
        target_compile_definitions (${resourcesTarget} INTERFACE BV_HAS_BINARY_DATA=1)
    endif()

    target_link_libraries (${BV_RSRC_FLDR_TARGET} PRIVATE ${resourcesTarget})
endfunction()

#

function (bv_configure_juce_plugin)

    set (options BROWSER)
    set (oneValueArgs TARGET)
    set (multiValueArgs "")

    cmake_parse_arguments (BV_PLUGINTARGETCONFIG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (NOT DEFINED BV_PLUGINTARGETCONFIG_TARGET)
        bv_print_warning ("Target name not specified in call to bv_configure_juce_plugin")
        return()
    endif()

	_bv_configure_juce_target (${BV_PLUGINTARGETCONFIG_TARGET} ${BV_PLUGINTARGETCONFIG_BROWSER})

    target_link_libraries (${BV_PLUGINTARGETCONFIG_TARGET} PUBLIC ${BV_PLUGIN_ONLY_MODULES})

    target_compile_definitions (${BV_PLUGINTARGETCONFIG_TARGET} PUBLIC
            JUCE_MICROPHONE_PERMISSION_ENABLED=1
            JUCE_USE_CUSTOM_PLUGIN_STANDALONE_APP=0)
endfunction()

#

function (bv_configure_juce_app)

    set (options BROWSER)
    set (oneValueArgs TARGET)
    set (multiValueArgs "")

    cmake_parse_arguments (BV_APPTARGETCONFIG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (NOT DEFINED BV_APPTARGETCONFIG_TARGET)
        bv_print_warning ("Target name not specified in call to bv_configure_juce_app")
        return()
    endif()

    _bv_configure_juce_target (${BV_APPTARGETCONFIG_TARGET} ${BV_APPTARGETCONFIG_BROWSER})
    target_link_libraries (${BV_APPTARGETCONFIG_TARGET} PUBLIC ${BV_APP_ONLY_MODULES})
endfunction()


function (bv_add_juce_modules dir)
    _bv_add_juce_modules_internal (${dir} dummylist)
endfunction()
