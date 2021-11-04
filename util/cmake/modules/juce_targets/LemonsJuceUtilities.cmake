include_guard (GLOBAL)

include (LemonsGetCPM)
include (LemonsFileUtils)

#

CPMAddPackage (
        NAME JUCE
        GITHUB_REPOSITORY juce-framework/JUCE
        GIT_TAG origin/develop
        OPTIONS "JUCE_ENABLE_MODULE_SOURCE_GROUPS ON" "JUCE_BUILD_EXAMPLES OFF" "JUCE_BUILD_EXTRAS OFF")


########################################################################


function (lemons_add_juce_modules dir)
    lemons_subdir_list (RESULT moduleFolders DIR "${dir}")

    foreach (folder ${moduleFolders})
        juce_add_module ("${dir}/${folder}")
    endforeach()
endfunction()

#

function (lemons_enable_plugin_hosting target)
    if (IOS)
        return()
    endif()

    target_compile_definitions ("${target}" PRIVATE JUCE_PLUGINHOST_VST3=1 JUCE_PLUGINHOST_LADSPA=1)

    if (LEMONS_VST2_SDK_PATH)
        target_compile_definitions ("${target}" PRIVATE  JUCE_PLUGINHOST_VST=1)
    endif()

    if (APPLE)
        target_compile_definitions ("${target}" PRIVATE JUCE_PLUGINHOST_AU=1)
    endif()
endfunction()

#

function (lemons_configure_juce_target)

    set (options BROWSER PLUGIN_HOST CAMERA MICROPHONE)
    set (oneValueArgs TARGET ASSET_FOLDER)
    set (multiValueArgs "")

    cmake_parse_arguments (LEMONS_TARGETCONFIG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (NOT LEMONS_TARGETCONFIG_TARGET)
        message (FATAL_ERROR "Target name not specified in call to ${CMAKE_CURRENT_FUNCTION}!")
        return()
    endif()

    target_compile_definitions (${LEMONS_TARGETCONFIG_TARGET} PUBLIC
            JUCE_VST3_CAN_REPLACE_VST2=0
            JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:${LEMONS_TARGETCONFIG_TARGET},JUCE_PRODUCT_NAME>"
            JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:${LEMONS_TARGETCONFIG_TARGET},JUCE_VERSION>"
            JUCE_COREGRAPHICS_DRAW_ASYNC=1
            JUCE_STRICT_REFCOUNTEDPTR=1
            JUCE_MODAL_LOOPS_PERMITTED=0
            JUCE_JACK=1
            JUCE_DISABLE_AUDIO_MIXING_WITH_OTHER_APPS=1
            JUCE_EXECUTE_APP_SUSPEND_ON_BACKGROUND_TASK=1
            JUCE_DISPLAY_SPLASH_SCREEN=0
            _CRT_SECURE_NO_WARNINGS=1)

    target_link_libraries (${LEMONS_TARGETCONFIG_TARGET} PUBLIC
        LemonsCommonModules
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

    target_compile_features (${LEMONS_TARGETCONFIG_TARGET} PUBLIC cxx_std_${LEMONS_CXX_VERSION})

    if (LEMONS_TARGETCONFIG_BROWSER)
        target_compile_definitions ("${LEMONS_TARGETCONFIG_TARGET}" PUBLIC JUCE_WEB_BROWSER=1 JUCE_USE_CURL=1 JUCE_LOAD_CURL_SYMBOLS_LAZILY=1)

        # Linux
        if (NOT (APPLE OR WIN32))
            target_link_libraries ("${LEMONS_TARGETCONFIG_TARGET}" PRIVATE juce::pkgconfig_JUCE_CURL_LINUX_DEPS)
        endif()
    else()
        target_compile_definitions ("${LEMONS_TARGETCONFIG_TARGET}" PUBLIC JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0)
    endif()

    if (APPLE)
        _lemons_set_default_macos_options (${LEMONS_TARGETCONFIG_TARGET})
    endif()

    if (LEMONS_TARGETCONFIG_ASSET_FOLDER)
        lemons_add_resources_folder (TARGET ${LEMONS_TARGETCONFIG_TARGET} FOLDER ${LEMONS_TARGETCONFIG_ASSET_FOLDER})
    endif()

    if (LEMONS_TARGETCONFIG_PLUGIN_HOST)
        lemons_enable_plugin_hosting ("${LEMONS_TARGETCONFIG_TARGET}")
    endif()

    if (LEMONS_TARGETCONFIG_CAMERA)
        target_compile_definitions (${LEMONS_TARGETCONFIG_TARGET} PUBLIC JUCE_USE_CAMERA=1)
        target_link_libraries (${LEMONS_TARGETCONFIG_TARGET} PUBLIC juce_video)
    endif()

    if (LEMONS_TARGETCONFIG_MICROPHONE)
        target_compile_definitions (${LEMONS_TARGETCONFIG_TARGET} PUBLIC JUCE_MICROPHONE_PERMISSION_ENABLED=1)
    endif()
endfunction()
