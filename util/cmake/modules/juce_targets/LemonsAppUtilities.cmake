include_guard (GLOBAL)

include (LemonsJuceUtilities)



macro (_lemons_configure_app_internal)

    lemons_configure_juce_target (${ARGN})

    set (options "")
    set (oneValueArgs TARGET)
    set (multiValueArgs "")

    cmake_parse_arguments (LEMONS_APP "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (NOT LEMONS_APP_TARGET)
        message(FATAL_ERROR "boo :(")
    endif()

    if (NOT TARGET ALL_APPS)
        add_custom_target (ALL_APPS COMMENT "Building all apps...")
    endif()
    
    add_dependencies (ALL_APPS ${LEMONS_APP_TARGET})
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

    target_link_libraries (${LEMONS_APP_TARGET} PUBLIC LemonsAppModules)

endfunction()
