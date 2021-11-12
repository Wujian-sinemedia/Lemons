include_guard (GLOBAL)

#

function (_lemons_add_to_all_apps_target target)

	if (NOT TARGET ALL_APPS)
        add_custom_target (ALL_APPS COMMENT "Building all apps...")
    endif()
    
    add_dependencies (ALL_APPS ${target})

endfunction()

#

function (_lemons_add_to_all_plugins_target target)

	if (NOT TARGET ALL_PLUGINS)
        add_custom_target (ALL_PLUGINS COMMENT "Building all plugins...")
    endif()

    add_dependencies (ALL_PLUGINS "${target}_All")

endfunction()
