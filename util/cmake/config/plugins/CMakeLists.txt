include (formats.cmake)

#

function (_lemons_configure_juce_aax origTarget pagetableFile)

    set (aaxTarget ${origTarget}_AAX)

    if (NOT TARGET ${aaxTarget})
        return()
    endif()

    if (NOT TARGET AAXSDK)
        message (FATAL_ERROR "AAX plugin target created, but AAXSDK target doesn't exist!")
        return()
    endif()

    set_target_properties (${aaxTarget} PROPERTIES OSX_ARCHITECTURES x86_64)

    add_dependencies (${aaxTarget} AAXSDK)

    if (pagetableFile)
        target_compile_definitions (${aaxTarget} PUBLIC "JucePlugin_AAXPageTableFile=\"${pagetableFile}\"")

        # On Windows, pagetable files need a special post-build copy step to be included in the binary correctly
        if (WIN32)

            set (LEMONS_PAGETABLE_FILENAME "${pagetableFile}")
            set (LEMONS_PLUGIN_NAME "${origTarget}")
            set (LEMONS_PLUGIN_ORIG_DIR "${PROJECT_SOURCE_DIR}")

            configure_file ("${CMAKE_CURRENT_LIST_DIR}/aax_pagetable_post_copy.bat" pagetable_copy.bat)

            execute_process (COMMAND chmod a+rx "${CMAKE_CURRENT_BINARY_DIR}/pagetable_copy.bat")

            add_custom_command (TARGET ${aaxTarget} POST_BUILD 
                                COMMAND "${CMAKE_CURRENT_BINARY_DIR}/pagetable_copy.bat")

        endif()
    endif()

endfunction()


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

    target_link_libraries (${lemons_targetname} PUBLIC ${LEMONS_PLUGIN_ONLY_MODULES})

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

