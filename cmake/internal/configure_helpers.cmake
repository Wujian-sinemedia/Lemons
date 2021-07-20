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

function (_bv_configure_LV2_URI target)

    if (NOT TARGET ${target}_LV2)
        return()
    endif()

    set_target_properties (${target} PROPERTIES JUCE_LV2_URI https://github.com/benthevining/${CMAKE_PROJECT_NAME})

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

function (_bv_configure_juce_target target useBrowser use_MTS-ESP forceUseVDSP forceIgnoreVDSP)
    if (TARGET ${target}_AAX)
        set_target_properties (${target}_AAX PROPERTIES OSX_ARCHITECTURES x86_64)
    endif()

    _bv_configure_LV2_URI (${target})

    target_compile_definitions (${target} PUBLIC
            JUCE_VST3_CAN_REPLACE_VST2=0
            JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:${target},JUCE_PRODUCT_NAME>"
            JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:${target},JUCE_VERSION>"
            JUCE_COREGRAPHICS_DRAW_ASYNC=1
            _CRT_SECURE_NO_WARNINGS=1
            JUCE_STRICT_REFCOUNTEDPTR=1
            JUCE_MODAL_LOOPS_PERMITTED=0
            JUCE_JACK=1
            JUCE_DISABLE_AUDIO_MIXING_WITH_OTHER_APPS=1
            JUCE_EXECUTE_APP_SUSPEND_ON_BACKGROUND_TASK=1)
    
    _bv_configure_juce_browser (${target} ${useBrowser})

    _bv_set_default_macos_options (${target})

    target_link_libraries (${target} PUBLIC
        ${BV_JUCE_MODULES}
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

    target_compile_features (${target} PUBLIC cxx_std_${BV_CXX_VERSION})

    _bv_configure_vecops (${target} ${forceUseVDSP} ${forceIgnoreVDSP})

    if (${use_MTS-ESP})
        _bv_configure_mts_esp (${target})
    endif()
endfunction()

