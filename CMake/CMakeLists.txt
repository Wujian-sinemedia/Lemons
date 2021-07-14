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

macro (add_binary_data_folder target folder)
    _add_resources_folder (${target} ${CMAKE_CURRENT_LIST_DIR}/${folder})
endmacro()


macro (configure_default_juce_plugin target)
	_configure_juce_app (${target} TRUE)
endmacro()


macro (configure_default_juce_app target)
    _configure_juce_app (${target} TRUE)
endmacro()


macro (bv_detect_plugin_formats outvar)
    _bv_make_plugin_format_list (${outvar})
endmacro()