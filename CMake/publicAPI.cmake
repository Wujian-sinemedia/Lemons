
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
    _bv_make_plugin_format_list (${outvar})
endmacro()