
function (add_binary_data_folder target folder)
    
    _add_resources_folder (${target} ${CMAKE_CURRENT_LIST_DIR}/${folder})
endfunction()


###########


function (configure_default_juce_plugin target)

	_configure_juce_app (${target} TRUE)
endfunction()


function (configure_default_juce_app target)

    _configure_juce_app (${target} TRUE)
endfunction()


###########


macro (bv_detect_default_plugin_formats outvar)
    if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS")
        set (${outvar} Standalone)
    elseif (APPLE)
        set (${outvar} Standalone Unity VST3 AU)
    elseif (WIN32)
        set (${outvar} Standalone Unity VST3)
    else()

        set (${outvar} Standalone Unity VST3)

    endif()
endmacro()