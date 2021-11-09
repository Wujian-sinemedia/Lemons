#[[
Utilities for audio plugins.

## Includes:
- LemonsJuceUtilities
- LemonsAAXUtils
- LemonsPluginvalUtils

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
                             [AAX_PAGETABLE_FILE <file>])
```
Forwards `${ARGN}` to [lemons_configure_juce_target](@ref lemons_configure_juce_target).

If an AAX-format target exists for this plugin, then [lemons_configure_aax_plugin()](@ref lemons_configure_aax_plugin) will be called for you. The `AAX_PAGETABLE_FILE` option will be forwarded.

If the `LEMONS_BUILD_TESTS` option is set to ON, then [lemons_configure_pluginval_tests()](@ref lemons_configure_pluginval_tests) will be called for you.

]]


include_guard (GLOBAL)

include (LemonsJuceUtilities)

if (LEMONS_AAX_SDK_PATH AND NOT IOS AND (APPLE OR WIN32))
    include (LemonsAAXUtils)
endif()

option (LEMONS_BUILD_TESTS "Build tests" OFF)

if (LEMONS_BUILD_TESTS)
    include (LemonsPluginvalUtils)
endif()


########################################################################


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
        endif()
    endif()
endif()

set (LEMONS_PLUGIN_FORMATS ${available_formats} CACHE INTERNAL "Available plugin formats")

list (JOIN available_formats " " formats_output)
message (VERBOSE "  -- Available plugin formats: ${formats_output}")


########################################################################


function (lemons_configure_juce_plugin)

    lemons_configure_juce_target (${ARGN})

    set (options "")
    set (oneValueArgs TARGET AAX_PAGETABLE_FILE)
    set (multiValueArgs "")

    cmake_parse_arguments (LEMONS_PLUGIN "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set (aax_target ${LEMONS_PLUGIN_TARGET}_AAX)
    if (TARGET ${aax_target})
        lemons_configure_aax_plugin (TARGET "${aax_target}" PAGETABLE_FILE "${LEMONS_PLUGIN_AAX_PAGETABLE_FILE}")
    endif()

    target_link_libraries (${LEMONS_PLUGIN_TARGET} PUBLIC LemonsPluginModules)

    target_compile_definitions (${LEMONS_PLUGIN_TARGET} PUBLIC JUCE_USE_CUSTOM_PLUGIN_STANDALONE_APP=0)

    # add this plugin to the `ALL_PLUGINS` target...
    if (NOT TARGET ALL_PLUGINS)
        add_custom_target (ALL_PLUGINS COMMENT "Building all plugins...")
    endif()

    add_dependencies (ALL_PLUGINS "${LEMONS_PLUGIN_TARGET}_All")

    # add the standalone plugin to the `ALL_APPS` target...
    set (stdaln_target "${LEMONS_PLUGIN_TARGET}_Standalone")
    if (TARGET ${stdaln_target})
        if (NOT TARGET ALL_APPS)
            add_custom_target (ALL_APPS COMMENT "Building all apps...")
        endif()
        
        add_dependencies (ALL_APPS ${stdaln_target})
    endif()

    if (LEMONS_BUILD_TESTS)
        lemons_configure_pluginval_tests (TARGET "${LEMONS_PLUGIN_TARGET}")
    endif()

endfunction()

