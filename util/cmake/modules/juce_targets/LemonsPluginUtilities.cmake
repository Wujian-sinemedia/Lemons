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

#
# lemons_configure_juce_plugin
# configures a default juce plugin
#
# OPTIONS (binary toggles, all optional):
# BROWSER : when mentioned in the call, enables the juce web browser functionality for this plugin target
# PLUGIN_HOST : when mentioned in the call, enables juce audio plugin hosting capabilities for this plugin target
# CAMERA : when mentioned in the call, enables juce support for using the camera with this plugin target
# 
# INPUTS : 
# TARGET : the name of the plugin target (usually the name of your product)
# ASSET_FOLDER (optional): the name of the assets folder to use for generating the resources target. The folder can be relative to your project's root; for example, if your folder structure is "YourProject/assets", you can pass `lemons_configure_juce_app (TARGET YourProject ASSET_FOLDER assets)`. 
# AAX_PAGETABLE_FILE (optional) : the path to the AAX pagetable file for this plugin
#
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

