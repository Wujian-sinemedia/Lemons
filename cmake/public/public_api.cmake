include_guard (GLOBAL)

###  CMAKE PUBLIC API  ###

function (bv_add_resources_folder target folder)
    set (resourcesTarget ${target}-Assets)

    if (NOT TARGET ${resourcesTarget})
        file (GLOB_RECURSE files "${folder}/*.*")
        juce_add_binary_data (${resourcesTarget} SOURCES ${files})
        set_target_properties (${resourcesTarget} PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
        target_compile_definitions (${resourcesTarget} INTERFACE BV_HAS_BINARY_DATA=1)
    endif()

    target_link_libraries (${target} PRIVATE ${resourcesTarget})
endfunction()


function (bv_configure_juce_plugin target)
	_bv_configure_juce_target (${target} TRUE)

    target_link_libraries (${target} PUBLIC ${BV_PLUGIN_ONLY_MODULES})

    target_compile_definitions (${target} PUBLIC
            JUCE_MICROPHONE_PERMISSION_ENABLED=1
            JUCE_USE_CUSTOM_PLUGIN_STANDALONE_APP=0)
endfunction()


function (bv_configure_juce_app target)
    _bv_configure_juce_target (${target} TRUE)
    target_link_libraries (${target} PUBLIC ${BV_APP_ONLY_MODULES})
endfunction()


macro (bv_detect_plugin_formats outvar)
    _bv_make_plugin_format_list (${outvar})
endmacro()


macro (bv_add_juce_modules dir)
    _bv_add_juce_modules_internal (${dir} bvdummyjucemodulelist)
endmacro()