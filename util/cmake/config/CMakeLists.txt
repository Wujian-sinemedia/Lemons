include (assets.cmake)

#

function (_lemons_enable_browser target)
    target_compile_definitions ("${target}" PUBLIC 
            JUCE_WEB_BROWSER=1
            JUCE_USE_CURL=1
            JUCE_LOAD_CURL_SYMBOLS_LAZILY=1)

    if (CMAKE_SYSTEM_NAME MATCHES "Linux")
        target_link_libraries ("${target}" PRIVATE juce::pkgconfig_JUCE_CURL_LINUX_DEPS)
    endif()
endfunction()

function (_lemons_enable_plugin_hosting target)
    target_compile_definitions ("${target}" PRIVATE 
            JUCE_PLUGINHOST_VST=0
            JUCE_PLUGINHOST_VST3=1
            JUCE_PLUGINHOST_LADSPA=1)

    if (APPLE)
        target_compile_definitions ("${target}" PRIVATE JUCE_PLUGINHOST_AU=1)
    endif()
endfunction()

#

function (_lemons_configure_juce_target)

    set (options BROWSER PLUGIN_HOST CAMERA MICROPHONE)
    set (oneValueArgs TARGET ASSET_FOLDER AAX_PAGETABLE_FILE)
    set (multiValueArgs "")

    cmake_parse_arguments (LEMONS_TARGETCONFIG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (NOT LEMONS_TARGETCONFIG_TARGET)
        message (FATAL_ERROR "Target name not specified in call to _lemons_configure_juce_target!")
        return()
    endif()

    # The directory of the target being configured
    if (NOT LEMONS_PROJECT_REPO_DIR)
        set (LEMONS_PROJECT_REPO_DIR "${LEMONS_TOPLEVEL_DIR}" CACHE INTERNAL "")
    endif()

    target_compile_definitions (${LEMONS_TARGETCONFIG_TARGET} PUBLIC
            JUCE_VST3_CAN_REPLACE_VST2=0
            JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:${LEMONS_TARGETCONFIG_TARGET},JUCE_PRODUCT_NAME>"
            JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:${LEMONS_TARGETCONFIG_TARGET},JUCE_VERSION>"
            JUCE_COREGRAPHICS_DRAW_ASYNC=1
            _CRT_SECURE_NO_WARNINGS=1
            JUCE_STRICT_REFCOUNTEDPTR=1
            JUCE_MODAL_LOOPS_PERMITTED=0
            JUCE_JACK=1
            JUCE_DISABLE_AUDIO_MIXING_WITH_OTHER_APPS=1
            JUCE_EXECUTE_APP_SUSPEND_ON_BACKGROUND_TASK=1)

    target_link_libraries (${LEMONS_TARGETCONFIG_TARGET} PUBLIC
        ${LEMONS_JUCE_MODULES}
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

    target_compile_features (${LEMONS_TARGETCONFIG_TARGET} PUBLIC cxx_std_${LEMONS_CXX_VERSION})
    
    if (LEMONS_TARGETCONFIG_BROWSER)
        _lemons_enable_browser ("${LEMONS_TARGETCONFIG_TARGET}")
    else()
        target_compile_definitions ("${LEMONS_TARGETCONFIG_TARGET}" PUBLIC JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0)
    endif()

    if (APPLE)
        _lemons_set_default_macos_options (${LEMONS_TARGETCONFIG_TARGET})
    endif()

    if (LEMONS_TARGETCONFIG_ASSET_FOLDER)
        lemons_add_resources_folder (TARGET ${LEMONS_TARGETCONFIG_TARGET} FOLDER ${LEMONS_TARGETCONFIG_ASSET_FOLDER} TRANSLATIONS)
    endif()

    if (LEMONS_TARGETCONFIG_PLUGIN_HOST)
        _lemons_enable_plugin_hosting ("${LEMONS_TARGETCONFIG_TARGET}")
    endif()

    if (LEMONS_TARGETCONFIG_CAMERA)
        target_compile_definitions (${LEMONS_TARGETCONFIG_TARGET} PUBLIC JUCE_USE_CAMERA=1)
        target_link_libraries (${LEMONS_TARGETCONFIG_TARGET} PUBLIC juce_video)
    endif()

    if (LEMONS_TARGETCONFIG_MICROPHONE)
        target_compile_definitions (${LEMONS_TARGETCONFIG_TARGET} PUBLIC JUCE_MICROPHONE_PERMISSION_ENABLED=1)
    endif()

    set (lemons_targetname "${LEMONS_TARGETCONFIG_TARGET}" PARENT_SCOPE)
    set (lemons_aax_pagetable_file "${LEMONS_TARGETCONFIG_AAX_PAGETABLE_FILE}" PARENT_SCOPE)

endfunction()

#

include (deploy.cmake)
include (apps.cmake)
include (plugins.cmake)
