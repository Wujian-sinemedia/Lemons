set (CMAKE_CXX_STANDARD ${BV_CXX_VERSION})

set_property (GLOBAL PROPERTY USE_FOLDERS YES)

set (CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

if (NOT DEFINED ENV{CPM_SOURCE_CACHE})
    set (ENV{CPM_SOURCE_CACHE} ${CMAKE_CURRENT_LIST_DIR}/../Cache)
endif()

#

add_subdirectory (third_party)

add_subdirectory (internal)


### PUBLIC API ###

macro (bv_add_resources_folder target folder)
    _bv_add_resources_folder (${target} ${CMAKE_CURRENT_LIST_DIR}/${folder})
endmacro()


macro (bv_configure_juce_plugin target)
	_bv_configure_juce_target (${target} TRUE)
    target_link_libraries (${target} PUBLIC ${BV_PLUGIN_ONLY_MODULES})
endmacro()


macro (bv_configure_juce_app target)
    _bv_configure_juce_target (${target} TRUE)
    target_link_libraries (${target} PUBLIC ${BV_APP_ONLY_MODULES})
endmacro()


macro (bv_detect_plugin_formats outvar)
    _bv_make_plugin_format_list (${outvar})
endmacro()


macro (bv_add_juce_modules dir)
    _bv_add_juce_modules_internal (${dir} bvdummyjucemodulelist)
endmacro()