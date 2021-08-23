function (_bv_configure_juce_browser target shouldUseBrowser)
    if (${shouldUseBrowser})
        target_compile_definitions (${target} PUBLIC 
            JUCE_WEB_BROWSER=1
            JUCE_USE_CURL=1
            JUCE_LOAD_CURL_SYMBOLS_LAZILY=1)

        if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
            target_link_libraries (${target} PRIVATE juce::pkgconfig_JUCE_CURL_LINUX_DEPS)
        endif()
    else()
        target_compile_definitions (${target} PUBLIC 
            JUCE_WEB_BROWSER=0
            JUCE_USE_CURL=0)
    endif()
endfunction()

#

function (_bv_configure_juce_target)

    set (options BROWSER PLUGIN_HOST CAMERA)
    set (oneValueArgs TARGET ASSET_FOLDER AAX_PAGETABLE_FILE)
    set (multiValueArgs "")

    cmake_parse_arguments (BV_TARGETCONFIG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (NOT DEFINED BV_TARGETCONFIG_TARGET)
        message (FATAL_ERROR "Target name not specified in call to _bv_configure_juce_target")
    endif()

    _bv_configure_juce_aax (${BV_TARGETCONFIG_TARGET} "${BV_TARGETCONFIG_AAX_PAGETABLE_FILE}")

    _bv_configure_juce_lv2 (${BV_TARGETCONFIG_TARGET})

    target_compile_definitions (${BV_TARGETCONFIG_TARGET} PUBLIC
            JUCE_VST3_CAN_REPLACE_VST2=0
            JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:${BV_TARGETCONFIG_TARGET},JUCE_PRODUCT_NAME>"
            JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:${BV_TARGETCONFIG_TARGET},JUCE_VERSION>"
            JUCE_COREGRAPHICS_DRAW_ASYNC=1
            _CRT_SECURE_NO_WARNINGS=1
            JUCE_STRICT_REFCOUNTEDPTR=1
            JUCE_MODAL_LOOPS_PERMITTED=0
            JUCE_JACK=1
            JUCE_DISABLE_AUDIO_MIXING_WITH_OTHER_APPS=1
            JUCE_EXECUTE_APP_SUSPEND_ON_BACKGROUND_TASK=1
            )
    
    _bv_configure_juce_browser (${BV_TARGETCONFIG_TARGET} ${BV_TARGETCONFIG_BROWSER})

    if (APPLE)
        _bv_set_default_macos_options (${BV_TARGETCONFIG_TARGET})
    endif()

    target_link_libraries (${BV_TARGETCONFIG_TARGET} PUBLIC
        ${BV_JUCE_MODULES}
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

    target_compile_features (${BV_TARGETCONFIG_TARGET} PUBLIC cxx_std_${BV_CXX_VERSION})

    _bv_configure_vecops (${BV_TARGETCONFIG_TARGET})

    if (DEFINED BV_TARGETCONFIG_ASSET_FOLDER)
        bv_add_resources_folder (TARGET ${BV_TARGETCONFIG_TARGET} FOLDER ${BV_TARGETCONFIG_ASSET_FOLDER})
    endif()

    if (${BV_TARGETCONFIG_PLUGIN_HOST})
        _bv_configure_plugin_hosting (${BV_TARGETCONFIG_TARGET})
    endif()

    if (${BV_TARGETCONFIG_CAMERA})
        target_compile_definitions (${BV_TARGETCONFIG_TARGET} PUBLIC JUCE_USE_CAMERA=1)
        target_link_libraries (${BV_TARGETCONFIG_TARGET} PUBLIC juce_video)
    endif()

    set (bv_targetname ${BV_TARGETCONFIG_TARGET} PARENT_SCOPE)
endfunction()

#

function (bv_configure_juce_plugin)

    _bv_configure_juce_target (${ARGN})

    _bv_plugin_target_postconfig (${bv_targetname})
endfunction()

#

function (bv_configure_juce_app)

    _bv_configure_juce_target (${ARGN})

    target_link_libraries (${bv_targetname} PUBLIC ${BV_APP_ONLY_MODULES})

    _bv_configure_target_install (${bv_targetname} true)
endfunction()
