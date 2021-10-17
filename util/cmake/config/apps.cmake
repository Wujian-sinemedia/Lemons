#
# lemons_configure_juce_app
# configures a default juce app
#
# OPTIONS (binary toggles, all optional):
# BROWSER : when mentioned in the call, enables the juce web browser functionality for this app target
# PLUGIN_HOST : when mentioned in the call, enables juce audio plugin hosting capabilities for this app target
# CAMERA : when mentioned in the call, enables juce support for using the camera with this app target
# 
# INPUTS : 
# TARGET : the name of the app target (usually the name of your product)
# ASSET_FOLDER (optional): the name of the assets folder to use for generating the resources target. The folder can be relative to your project's root; for example, if your folder structure is "YourProject/assets", you can pass `lemons_configure_juce_app (TARGET YourProject ASSET_FOLDER assets)`. 
#
function (lemons_configure_juce_app)

    _lemons_configure_juce_target (${ARGN})

    target_link_libraries (${lemons_targetname} PUBLIC ${LEMONS_APP_ONLY_MODULES})

    _lemons_configure_product_deploy (${lemons_targetname} FALSE)


    # add this app to the `ALL_APPS` target...

    set (app_target "ALL_APPS")

    if (NOT TARGET ${app_target})
        add_custom_target (${app_target})
    endif()

    add_dependencies (${app_target} ${lemons_targetname})

endfunction()
