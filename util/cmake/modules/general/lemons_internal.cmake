include_guard (GLOBAL)


option (LEMONS_CREATE_AGGREGATE_TARGETS "Generate ALL_APPS and ALL_PLUGINS targets, populated accordingly" ON)
mark_as_advanced (FORCE LEMONS_CREATE_AGGREGATE_TARGETS)

#

function (_lemons_add_to_all_apps_target target)

	if (NOT LEMONS_CREATE_AGGREGATE_TARGETS)
		return()
	endif()

	if (NOT TARGET LEMONS_ALL_APPS)
        add_custom_target (ALL_APPS COMMENT "Building all apps...")
    endif()
    
    add_dependencies (LEMONS_ALL_APPS ${target})

    if (NOT TARGET Lemons::ALL_APPS)
    	add_library (Lemons::ALL_APPS ALIAS LEMONS_ALL_APPS)
    endif()
endfunction()

#

function (_lemons_add_to_all_plugins_target target)

	if (NOT LEMONS_CREATE_AGGREGATE_TARGETS)
		return()
	endif()

	if (NOT TARGET LEMONS_ALL_PLUGINS)
        add_custom_target (ALL_PLUGINS COMMENT "Building all plugins...")
    endif()

    add_dependencies (LEMONS_ALL_PLUGINS "${target}_All")

    if (NOT TARGET Lemons::ALL_PLUGINS)
    	add_library (Lemons::ALL_PLUGINS ALIAS LEMONS_ALL_PLUGINS)
    endif()
endfunction()
