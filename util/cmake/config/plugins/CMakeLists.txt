add_subdirectory (aax)
include (formats.cmake)


#

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

    _lemons_configure_juce_target (${ARGN})

    _lemons_configure_juce_aax (${lemons_targetname} "${lemons_aax_pagetable_file}")

    target_link_libraries (${lemons_targetname} PUBLIC LemonsPluginModules)

    target_compile_definitions (${lemons_targetname} PUBLIC JUCE_USE_CUSTOM_PLUGIN_STANDALONE_APP=0)

    # add this plugin to the `ALL_PLUGINS` target...
    if (NOT TARGET ALL_PLUGINS)
        add_custom_target (ALL_PLUGINS COMMENT "Building all plugins...")
    endif()

    add_dependencies (ALL_PLUGINS "${lemons_targetname}_All")

    # add the standalone plugin to the `ALL_APPS` target...
    set (stdaln_target "${lemons_targetname}_Standalone")

    if (TARGET ${stdaln_target})
        _lemons_create_all_apps_target_if_not_exists()
        add_dependencies (ALL_APPS ${stdaln_target})
    endif()

    # configure plugin QC tests
    if (LEMONS_BUILD_TESTS)
        _lemons_configure_plugin_tests ("${lemons_targetname}")
    endif()

endfunction()

