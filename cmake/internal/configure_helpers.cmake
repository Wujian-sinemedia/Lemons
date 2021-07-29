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

function (_bv_set_default_macos_options target)
    if (NOT APPLE)
        return()
    endif()

    macro (_bv_configure_macos_version target version)
        set_target_properties  (${target} PROPERTIES XCODE_ATTRIBUTE_MACOSX_DEPLOYMENT_TARGET ${version})
        target_compile_options (${target} PUBLIC "-mmacosx-version-min=${version}")
        target_link_options    (${target} PUBLIC "-mmacosx-version-min=${version}")
    endmacro()

    set_target_properties (${target} PROPERTIES JUCE_BUNDLE_ID "com.bv.${target}")

    if (${CMAKE_SYSTEM_NAME} STREQUAL "iOS")
        set_target_properties (${target} PROPERTIES
                ARCHIVE_OUTPUT_DIRECTORY "./"
                XCODE_ATTRIBUTE_INSTALL_PATH "$(LOCAL_APPS_DIR)"
                XCODE_ATTRIBUTE_SKIP_INSTALL "NO")

        _bv_configure_macos_version (${target} "9.3")
    elseif (TARGET ${target}_AUv3)
        _bv_configure_macos_version (${target} "10.11")
    else()
        _bv_configure_macos_version (${target} "10.9")
    endif()
endfunction()

#

function (_bv_configure_juce_target)

    set (options BROWSER MTS-ESP ABLETON_LINK ALWAYS_VDSP NEVER_VDSP)
    set (oneValueArgs TARGET ASSET_FOLDER AAX_PAGETABLE_FILE)
    set (multiValueArgs "")

    cmake_parse_arguments (BV_TARGETCONFIG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (NOT DEFINED BV_TARGETCONFIG_TARGET)
        message (FATAL_ERROR "Target name not specified in call to _bv_configure_juce_target")
    endif()

    # AAX settings
    if (TARGET ${BV_TARGETCONFIG_TARGET}_AAX)
        set_target_properties (${BV_TARGETCONFIG_TARGET}_AAX PROPERTIES OSX_ARCHITECTURES x86_64)

        if (DEFINED BV_TARGETCONFIG_AAX_PAGETABLE_FILE)
            target_compile_definitions (${BV_TARGETCONFIG_TARGET}_AAX PUBLIC 
                JucePlugin_AAXPageTableFile="${BV_TARGETCONFIG_AAX_PAGETABLE_FILE}")

            if (NOT DEFINED BV_TARGETCONFIG_ASSET_FOLDER)
                message (WARNING "Warning - AAX pagetable file must be embedded in plugin's binary data!")
            endif()
        endif()
    endif()

    # LV2 settings
    if (TARGET ${BV_TARGETCONFIG_TARGET}_LV2)
        set_target_properties (${BV_TARGETCONFIG_TARGET}_LV2 PROPERTIES JUCE_LV2_URI https://github.com/benthevining/${CMAKE_PROJECT_NAME})
    endif()

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
            JUCE_EXECUTE_APP_SUSPEND_ON_BACKGROUND_TASK=1)
    
    _bv_configure_juce_browser (${BV_TARGETCONFIG_TARGET} ${BV_TARGETCONFIG_BROWSER})

    _bv_set_default_macos_options (${BV_TARGETCONFIG_TARGET})

    target_link_libraries (${BV_TARGETCONFIG_TARGET} PUBLIC
        ${BV_JUCE_MODULES}
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

    target_compile_features (${BV_TARGETCONFIG_TARGET} PUBLIC cxx_std_${BV_CXX_VERSION})

    _bv_configure_third_party_libs (${BV_TARGETCONFIG_TARGET} 
                                    ${BV_TARGETCONFIG_ALWAYS_VDSP} 
                                    ${BV_TARGETCONFIG_NEVER_VDSP} 
                                    ${BV_TARGETCONFIG_MTS-ESP} 
                                    ${BV_TARGETCONFIG_ABLETON_LINK})

    if (DEFINED BV_TARGETCONFIG_ASSET_FOLDER)
        bv_add_resources_folder (TARGET ${BV_TARGETCONFIG_TARGET} FOLDER ${BV_TARGETCONFIG_ASSET_FOLDER})
    endif()

    set (bv_targetname ${BV_TARGETCONFIG_TARGET} PARENT_SCOPE)
endfunction()

