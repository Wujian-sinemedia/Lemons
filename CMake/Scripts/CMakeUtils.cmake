
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

function (_create_resources_target targetName folder)
    file (GLOB_RECURSE files "${folder}/*.*")
    juce_add_binary_data (${targetName} SOURCES ${files})
    set_target_properties (${targetName} PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
    target_compile_definitions (${targetName} PUBLIC BV_HAS_BINARY_DATA=1)
endfunction()

#

function (_link_resources_target target resourcesTarget)
    _adjustDefaultMacTarget (${resourcesTarget} ${target})
    target_link_libraries (${target} PRIVATE ${resourcesTarget})
endfunction()

#

function (_add_resources_folder target folder)
    set (resourcesTarget ${target}-Assets)
    _create_resources_target (${resourcesTarget} ${folder})
    _link_resources_target (${target} ${resourcesTarget})
endfunction()

#

function (add_binary_data_folder target folder)
    _add_resources_folder (${target} ${CMAKE_CURRENT_LIST_DIR}/${folder})
endfunction()

#

function (set_default_juce_options target)
    option (JUCE_ENABLE_MODULE_SOURCE_GROUPS "Enable Module Source Groups" ON)
    option (JUCE_BUILD_EXAMPLES "Build JUCE Examples" OFF)
    option (JUCE_BUILD_EXTRAS "Build JUCE Extras" OFF)

    if (TARGET ${target}_AAX)
        set_target_properties (${target}_AAX PROPERTIES OSX_ARCHITECTURES x86_64)
    endif()

    if (TARGET ${target}_LV2)
        set_target_properties (${target}_LV2 PROPERTIES JUCE_LV2_URI https://github.com/benthevining/${target})
    endif()

    target_compile_definitions (${target} PUBLIC
            JUCE_WEB_BROWSER=1
            JUCE_USE_CURL=1
            JUCE_VST3_CAN_REPLACE_VST2=0
            JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:${target},JUCE_PRODUCT_NAME>"
            JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:${target},JUCE_VERSION>"
            JUCE_COREGRAPHICS_DRAW_ASYNC=1
            _CRT_SECURE_NO_WARNINGS=1
            JUCE_MICROPHONE_PERMISSION_ENABLED=1
            JUCE_STRICT_REFCOUNTEDPTR=1
            JUCE_MODAL_LOOPS_PERMITTED=0
            JUCE_LOAD_CURL_SYMBOLS_LAZILY=1)

    if (APPLE)
    	message (STATUS "Configuring vDSP for vecops...")
        target_compile_definitions (${target} PUBLIC JUCE_USE_VDSP_FRAMEWORK=1 BV_USE_VDSP=1)
    else()
    	message (STATUS "Configuring MIPP for vecops...")
    	target_compile_definitions (${target} PUBLIC MIPP_ENABLE_BACKTRACE BV_USE_MIPP=1)
        target_include_directories (${target} PUBLIC "${bv_sharedcode_dir}/third_party/MIPP/src" "MIPP")
    endif()

    _adjustDefaultMacTarget (${target} ${target})

    target_link_libraries (${target} PUBLIC
        bv_SharedCode
        bv_dsp
        bv_SynthBase
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

    target_compile_features (${target} PUBLIC cxx_std_17)
endfunction()

#
