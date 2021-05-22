
function (add_binary_data_folder target folder)
    
    _add_resources_folder (${target} ${CMAKE_CURRENT_LIST_DIR}/${folder})
endfunction()


function (link_binary_data_to_target_create_if_needed target folder)

    _add_resources_folder_dont_regenerate (${target} ${CMAKE_CURRENT_LIST_DIR}/${folder})
endfunction()

###########

function (set_default_juce_options target)

	_configure_juce_app (${target} TRUE TRUE)
endfunction()


###########


function (add_subdirectories)

    if (NOT DEFINED BuildAll)
        set (BuildAll TRUE)
    endif()

    if (BuildAll)
        _add_all_subdirs()
    else()
        _add_all_flagged_subdirs()
    endif()
endfunction()

