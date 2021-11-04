function (_lemons_create_all_apps_target_if_not_exists)
    if (NOT TARGET ALL_APPS)
        add_custom_target (ALL_APPS COMMENT "Building all apps...")
    endif()
endfunction()

macro (_lemons_configure_app_internal)

    _lemons_configure_juce_target (${ARGN})

    _lemons_create_all_apps_target_if_not_exists()
    add_dependencies (ALL_APPS ${lemons_targetname})
endmacro()

#

#
# lemons_configure_headless_app
# configures a default headless console app
#
# INPUTS : 
# TARGET : the name of the app target (usually the name of your product)
# ASSET_FOLDER (optional): the name of the assets folder to use for generating the resources target. The folder can be relative to your project's root; for example, if your folder structure is "YourProject/assets", you can pass `lemons_configure_juce_app (TARGET YourProject ASSET_FOLDER assets)`. 
#
function (lemons_configure_headless_app)
    _lemons_configure_app_internal (${ARGN})
endfunction()



#
# lemons_configure_juce_app
# configures a default juce app
#
# OPTIONS (binary toggles, all optional):
# BROWSER : when mentioned in the call, enables the juce web browser functionality for this app target
# PLUGIN_HOST : when mentioned in the call, enables juce audio plugin hosting capabilities for this app target
# CAMERA : when mentioned in the call, enables juce support for using the camera with this app target
# PLUGIN_MODULES : when mentioned in the call, links all lemons plugin modules to this app target
# 
# INPUTS : 
# TARGET : the name of the app target (usually the name of your product)
# ASSET_FOLDER (optional): the name of the assets folder to use for generating the resources target. The folder can be relative to your project's root; for example, if your folder structure is "YourProject/assets", you can pass `lemons_configure_juce_app (TARGET YourProject ASSET_FOLDER assets)`. 
#
function (lemons_configure_juce_app)

    _lemons_configure_app_internal (${ARGN})

    target_link_libraries (${lemons_targetname} PUBLIC LemonsAppModules)

    if (_lemons_link_plugin_modules)
        target_link_libraries (${lemons_targetname} PUBLIC LemonsPluginModules)
    endif()
endfunction()
