include (plugin_format_helpers.cmake)
include (format_specific_config.cmake)
include (plugin_hosting_config.cmake)


function (_bv_plugin_target_postconfig targetName)

	target_link_libraries (${targetName} PUBLIC ${BV_PLUGIN_ONLY_MODULES})

    target_compile_definitions (${targetName} PUBLIC
            JUCE_MICROPHONE_PERMISSION_ENABLED=1
            JUCE_USE_CUSTOM_PLUGIN_STANDALONE_APP=0)

    get_target_property (formats ${targetName} JUCE_FORMATS)

    if (NOT formats)
        return()
    endif()

    foreach (format ${formats})
    	set (target_name "${targetName}_${format}")

    	if ("${format}" STREQUAL "Standalone")
    		_bv_configure_target_install (${target_name} true)
    	else()
    		_bv_configure_target_install (${target_name} false)
    	endif()
    endforeach()

endfunction()