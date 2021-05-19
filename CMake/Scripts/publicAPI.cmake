
function (add_binary_data_folder target folder)
    _add_resources_folder (${target} ${CMAKE_CURRENT_LIST_DIR}/${folder})
endfunction()


function (link_binary_data_to_target_create_if_needed target folder)
    _add_resources_folder_dont_regenerate (${target} ${CMAKE_CURRENT_LIST_DIR}/${folder})
endfunction()

###########

function (set_default_juce_options target useBrowser)
	message (STATUS "Configuring ${target}...")

    if (TARGET ${target}_AAX)
        set_target_properties (${target}_AAX PROPERTIES OSX_ARCHITECTURES x86_64)
    endif()

    if (TARGET ${target}_LV2)
        set_target_properties (${target}_LV2 PROPERTIES JUCE_LV2_URI https://github.com/benthevining/${CMAKE_PROJECT_NAME})
    endif()

    set_target_properties (${target} PROPERTIES FOLDER "")

    target_compile_definitions (${target} PUBLIC
            JUCE_VST3_CAN_REPLACE_VST2=0
            JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:${target},JUCE_PRODUCT_NAME>"
            JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:${target},JUCE_VERSION>"
            JUCE_COREGRAPHICS_DRAW_ASYNC=1
            _CRT_SECURE_NO_WARNINGS=1
            JUCE_MICROPHONE_PERMISSION_ENABLED=1
            JUCE_STRICT_REFCOUNTEDPTR=1
            JUCE_MODAL_LOOPS_PERMITTED=0)
    
    if (${useBrowser})
        target_compile_definitions (${target} PUBLIC 
            JUCE_WEB_BROWSER=1
            JUCE_USE_CURL=1
            JUCE_LOAD_CURL_SYMBOLS_LAZILY=1)
    else()
        target_compile_definitions (${target} PUBLIC 
            JUCE_WEB_BROWSER=0
            JUCE_USE_CURL=0)
    endif()

    if (APPLE)
    	message (STATUS "Configuring vDSP for vecops...")
        target_compile_definitions (${target} PUBLIC JUCE_USE_VDSP_FRAMEWORK=1 BV_USE_VDSP=1)
    else()
    	message (STATUS "Configuring MIPP for vecops...")
    	target_compile_definitions (${target} PUBLIC MIPP_ENABLE_BACKTRACE BV_USE_MIPP=1)
        target_include_directories (${target} PUBLIC "${bv_sharedcode_dir}/third_party/MIPP/src" "MIPP")
    endif()

    configure_synth_module (${target})

    _adjustDefaultMacTarget (${target} ${target})

    target_link_libraries (${target} PUBLIC
        bv_data_model
        bv_dsp
        bv_gui
        bv_midi
        bv_motion
        bv_core
        bv_SynthBase
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

    target_compile_features (${target} PUBLIC cxx_std_17)
endfunction()


###########


function (add_subdirectories)
    if (NOT DEFINED BuildAll)
        set (BuildAll TRUE)
    endif()

    if (BuildAll)
        _add_all_subdirs()
    else()
        _add_all_flagged_subdirs()
    endif()
endfunction()

