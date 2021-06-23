
function (add_binary_data_folder target folder)
    
    _add_resources_folder (${target} ${CMAKE_CURRENT_LIST_DIR}/${folder})
endfunction()


function (link_binary_data_to_target_create_if_needed target folder)

    _add_resources_folder_dont_regenerate (${target} ${CMAKE_CURRENT_LIST_DIR}/${folder})
endfunction()


###########


function (configure_default_juce_plugin target)

	_configure_juce_app (${target} TRUE)
endfunction()


function (configure_default_juce_app target)

    _configure_juce_app (${target} TRUE)
endfunction()

