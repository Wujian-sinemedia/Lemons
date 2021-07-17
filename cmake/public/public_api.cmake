###  CMAKE PUBLIC API  ###

macro (bv_add_resources_folder target folder)
    _bv_add_resources_folder (${target} ${CMAKE_CURRENT_LIST_DIR}/${folder})
endmacro()


function (bv_configure_juce_plugin target)
	_bv_configure_juce_target (${target} TRUE)
    _bv_finish_plugin_config (${target})
endfunction()


function (bv_configure_juce_app target)
    _bv_configure_juce_target (${target} TRUE)
    _bv_finish_app_config (${target})
endfunction()


macro (bv_detect_plugin_formats outvar)
    _bv_make_plugin_format_list (${outvar})
endmacro()


macro (bv_add_juce_modules dir)
    _bv_add_juce_modules_internal (${dir} bvdummyjucemodulelist)
endmacro()