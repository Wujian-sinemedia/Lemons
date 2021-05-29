
function (_adjustDefaultMacTarget target bundleName)
    if (APPLE)
        set_target_properties (${target} PROPERTIES JUCE_BUNDLE_ID "com.bv.${bundleName}")

        if (${CMAKE_SYSTEM_NAME} STREQUAL "iOS")
            set_target_properties (${target} PROPERTIES
                    ARCHIVE_OUTPUT_DIRECTORY "./"
                    XCODE_ATTRIBUTE_INSTALL_PATH "$(LOCAL_APPS_DIR)"
                    XCODE_ATTRIBUTE_SKIP_INSTALL "NO")
        endif ()
    endif ()
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

    #set_target_properties (${target} PROPERTIES FOLDER "${CMAKE_PROJECT_NAME}")

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
            JUCE_LOAD_CURL_SYMBOLS_LAZILY=1
            JUCE_JACK=1
            JUCE_BELA=1
            JUCE_DISABLE_AUDIO_MIXING_WITH_OTHER_APPS=1
            JUCE_EXECUTE_APP_SUSPEND_ON_BACKGROUND_TASK=1)
    
    if (${useBrowser})
        message (STATUS "Configuring JUCE web browser...")

        target_compile_definitions (${target} PUBLIC 
            JUCE_WEB_BROWSER=1
            JUCE_USE_CURL=1)
    else()
        target_compile_definitions (${target} PUBLIC 
            JUCE_WEB_BROWSER=0
            JUCE_USE_CURL=0)
    endif()

    if (APPLE)
        message (STATUS "Configuring vDSP for vecops...")
        target_compile_definitions (${target} PUBLIC JUCE_USE_VDSP_FRAMEWORK=1 BV_USE_VDSP=1)
    else()
        if (NOT DEFINED BV_IGNORE_MIPP)
            set (BV_IGNORE_MIPP FALSE)
        endif()
    
        if (NOT ${BV_IGNORE_MIPP})
            message (STATUS "Configuring MIPP for vecops...")
            target_compile_definitions (${target} PUBLIC MIPP_ENABLE_BACKTRACE BV_USE_MIPP=1)
            target_include_directories (${target} PUBLIC "${bv_sharedcode_dir}/third_party/MIPP/src" "MIPP")
        endif()
    endif()

    if (NOT DEFINED BV_USE_MTS_ESP)
        set (BV_USE_MTS_ESP FALSE)
    endif()

    if (${BV_USE_MTS_ESP})
        message (STATUS "Configuring MTS-ESP...")
        target_include_directories (${target} PUBLIC "${bv_sharedcode_dir}/third_party/MTS-ESP/Client" "MTS-ESP")
        target_compile_definitions (${target} PUBLIC BV_USE_MTS_ESP=1)
    endif()

    _adjustDefaultMacTarget (${target} ${target})

    target_link_libraries (${target} PUBLIC
        bv_dsp
        bv_gui
        bv_midi
        bv_mobile_utils
        bv_core
        bv_plugin
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

    target_compile_features (${target} PUBLIC cxx_std_17)
endfunction()

