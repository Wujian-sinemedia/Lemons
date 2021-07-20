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

macro (_bv_juce_target_internal_config)

    set (options BROWSER MTS-ESP ALWAYS_VDSP NEVER_VDSP)
    set (oneValueArgs TARGET)
    set (multiValueArgs "")

    cmake_parse_arguments (BV_TARGETCONFIG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (NOT DEFINED BV_TARGETCONFIG_TARGET)
        bv_print_warning ("Target name not specified in call to _bv_juce_target_internal_config")
        return()
    endif()

    _bv_configure_juce_target (${BV_TARGETCONFIG_TARGET} 
                               ${BV_TARGETCONFIG_BROWSER}
                               ${BV_TARGETCONFIG_MTS-ESP}
                               ${BV_TARGETCONFIG_ALWAYS_VDSP}
                               ${BV_TARGETCONFIG_NEVER_VDSP})
endmacro()


function (bv_configure_juce_plugin)
    _bv_juce_target_internal_config (${ARGN})

    target_link_libraries (${BV_TARGETCONFIG_TARGET} PUBLIC ${BV_PLUGIN_ONLY_MODULES})

    target_compile_definitions (${BV_TARGETCONFIG_TARGET} PUBLIC
            JUCE_MICROPHONE_PERMISSION_ENABLED=1
            JUCE_USE_CUSTOM_PLUGIN_STANDALONE_APP=0)
endfunction()


function (bv_configure_juce_app)
    _bv_juce_target_internal_config (${ARGN})

    target_link_libraries (${BV_TARGETCONFIG_TARGET} PUBLIC ${BV_APP_ONLY_MODULES})
endfunction()


function (bv_add_juce_modules dir)
    _bv_add_juce_modules_internal (${dir} dummylist)
endfunction()
