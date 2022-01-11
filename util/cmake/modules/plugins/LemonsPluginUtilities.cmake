#[[
Utilities for audio plugins.

## Include-time actions:
Determines the list of all available plugin formats.

## Output variables:
- LEMONS_PLUGIN_FORMATS: list of all available plugin formats

## Options:
- LEMONS_VST2_SDK_PATH: if you have access to the VST2 SDK, you can define this variable to the absolute path of the VST2 SDK root to enable compiling VST2 plugins.
- LEMONS_BUILD_TESTS: if ON, this module will include LemonsPluginvalUtils, and calling `lemons_configure_juce_plugin` will also call `lemons_configure_pluginval_tests`. Defaults to OFF.


## Function:

### lemons_configure_juce_plugin
```
lemons_configure_juce_plugin (TARGET <target> 
                             [AAX_PAGETABLE_FILE <file>] [AAX_GUID <guid>])
```
Forwards `${ARGN}` to [lemons_configure_juce_target](@ref lemons_configure_juce_target).

If an AAX-format target exists for this plugin, then [lemons_configure_aax_plugin()](@ref lemons_configure_aax_plugin) will be called for you. The `AAX_PAGETABLE_FILE` and `AAX_GUID` options will be forwarded, if present.

If the `LEMONS_BUILD_TESTS` option is set to ON, then [lemons_configure_pluginval_tests()](@ref lemons_configure_pluginval_tests) will be called for you.

]]


include_guard (GLOBAL)

include (LemonsJuceUtilities)
include (LemonsCmakeDevTools)
include (lemons_AggregateTargets)

set (LEMONS_AAX_SDK_PATH ""  CACHE PATH "Path to the AAX SDK")
set (LEMONS_VST2_SDK_PATH "" CACHE PATH "Path to the VST2 SDK")

option (LEMONS_INCLUDE_PRIVATE_SDKS "Add the PrivateSDKs repo via CPM.cmake" OFF)

if (LEMONS_INCLUDE_PRIVATE_SDKS OR CPM_PrivateSDKs_SOURCE)
    include (LemonsAddPrivateSDKs)
    mark_as_advanced (FORCE LEMONS_AAX_SDK_PATH LEMONS_VST2_SDK_PATH)
endif()

if (LEMONS_AAX_SDK_PATH)
    include (LemonsAAXUtils)
endif()

lemons_warn_if_not_processing_project()

#

if (NOT LEMONS_PLUGIN_FORMATS)

    set (available_formats Standalone)

    if (APPLE)
        list (APPEND available_formats AUv3)
    endif()

    if (NOT IOS)
        list (APPEND available_formats Unity VST3)

        if (APPLE)
            list (APPEND available_formats AU)
        endif()

        if (TARGET AAXSDK)
            list (APPEND available_formats AAX)
        endif()

        if (LEMONS_VST2_SDK_PATH)
            if (IS_DIRECTORY "${LEMONS_VST2_SDK_PATH}")
                juce_set_vst2_sdk_path ("${LEMONS_VST2_SDK_PATH}")
                list (APPEND available_formats VST)
            else()
                message (AUTHOR_WARNING "LEMONS_VST2_SDK_PATH specified, but the directory does not exist!")
            endif()
        endif()
    endif()

    set (LEMONS_PLUGIN_FORMATS ${available_formats} CACHE STRING "Available plugin formats")

endif()


list (JOIN LEMONS_PLUGIN_FORMATS " " formats_output)
message (STATUS "  -- Available plugin formats: ${formats_output}")

#

function (lemons_configure_juce_plugin)

    lemons_configure_juce_target (${ARGN})

    set (oneValueArgs TARGET AAX_PAGETABLE_FILE AAX_GUID)
    cmake_parse_arguments (LEMONS_PLUGIN "" "${oneValueArgs}" "" ${ARGN})

    set (aax_target "${LEMONS_PLUGIN_TARGET}_AAX")
    if (TARGET ${aax_target})
        message (DEBUG "Configuring AAX plugin target...")

        lemons_configure_aax_plugin (TARGET ${aax_target} 
                                     PAGETABLE_FILE "${LEMONS_PLUGIN_AAX_PAGETABLE_FILE}"
                                     GUID "${LEMONS_PLUGIN_AAX_GUID}")
    endif()

    target_link_libraries (${LEMONS_PLUGIN_TARGET} PRIVATE Lemons::LemonsPluginModules)

    target_compile_definitions (${LEMONS_PLUGIN_TARGET} PRIVATE JUCE_USE_CUSTOM_PLUGIN_STANDALONE_APP=0)

    _lemons_add_to_all_plugins_target (${LEMONS_PLUGIN_TARGET})


    # This dependency is needed to build Standalone and AUv3 targets, but isn't needed directly by my lemons_plugin module, so add it to those targets here...
    function (_lemons_add_extra_pluginformat_modules formatTarget)
        if (TARGET ${formatTarget})
            target_link_libraries (${formatTarget} PRIVATE juce_audio_devices)
        endif()
    endfunction()

    _lemons_add_extra_pluginformat_modules (${LEMONS_PLUGIN_TARGET}_Standalone)
    _lemons_add_extra_pluginformat_modules (${LEMONS_PLUGIN_TARGET}_AUv3)

endfunction()

