function (_configure_juce_browser target shouldUseBrowser)
    if (${shouldUseBrowser})
        message (STATUS "Configuring JUCE web browser...")

        target_compile_definitions (${target} PUBLIC 
            JUCE_WEB_BROWSER=1
            JUCE_USE_CURL=1
            JUCE_LOAD_CURL_SYMBOLS_LAZILY=1)
    else()
        target_compile_definitions (${target} PUBLIC 
            JUCE_WEB_BROWSER=0
            JUCE_USE_CURL=0)
    endif()
endfunction()

#

function (_adjustDefaultMacTarget target)
    if (APPLE)
        set_target_properties (${target} PROPERTIES JUCE_BUNDLE_ID "com.bv.${target}")

        if (${CMAKE_SYSTEM_NAME} STREQUAL "iOS")
            set_target_properties (${target} PROPERTIES
                    ARCHIVE_OUTPUT_DIRECTORY "./"
                    XCODE_ATTRIBUTE_INSTALL_PATH "$(LOCAL_APPS_DIR)"
                    XCODE_ATTRIBUTE_SKIP_INSTALL "NO")
        endif ()
    endif ()
endfunction()

#

function (_set_min_macos_version target version)
    set_target_properties (${target} PROPERTIES XCODE_ATTRIBUTE_MACOSX_DEPLOYMENT_TARGET ${version})
    target_compile_options (${target} PUBLIC "-mmacosx-version-min=${version}")
    target_link_options (${target} PUBLIC "-mmacosx-version-min=${version}")
endfunction()

function (_configure_macos_version target)
    if (APPLE)
        if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS")
            _set_min_macos_version (${target} "9.3")
        else()
            _set_min_macos_version (${target} "10.9")
        endif()
    endif()
endfunction()

#

function (_configure_juce_app target useBrowser)
    message (STATUS "Configuring ${target}")

    if (TARGET ${target}_AAX)
        set_target_properties (${target}_AAX PROPERTIES OSX_ARCHITECTURES x86_64)
    endif()

    if (TARGET ${target}_LV2)
        set_target_properties (${target}_LV2 PROPERTIES JUCE_LV2_URI https://github.com/benthevining/${CMAKE_PROJECT_NAME})
    endif()

    target_compile_definitions (${target} PUBLIC
            JUCE_VST3_CAN_REPLACE_VST2=0
            JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:${target},JUCE_PRODUCT_NAME>"
            JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:${target},JUCE_VERSION>"
            JUCE_COREGRAPHICS_DRAW_ASYNC=1
            _CRT_SECURE_NO_WARNINGS=1
            JUCE_MICROPHONE_PERMISSION_ENABLED=1
            JUCE_STRICT_REFCOUNTEDPTR=1
            JUCE_MODAL_LOOPS_PERMITTED=0
            JUCE_USE_CUSTOM_PLUGIN_STANDALONE_APP=1
            JUCE_JACK=1
            JUCE_DISABLE_AUDIO_MIXING_WITH_OTHER_APPS=1
            JUCE_EXECUTE_APP_SUSPEND_ON_BACKGROUND_TASK=1)
    
    _configure_juce_browser (${target} ${useBrowser})

    _adjustDefaultMacTarget (${target})
    _configure_macos_version (${target})

    target_link_libraries (${target} PUBLIC
        ${modules}
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

    target_compile_features (${target} PUBLIC cxx_std_20)
endfunction()

