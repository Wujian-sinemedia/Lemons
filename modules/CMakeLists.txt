macro (_bv_add_modules)

	_subdir_list (subdirs ${CMAKE_CURRENT_LIST_DIR})

	juce_add_modules (${subdirs})

	list (APPEND modules ${subdirs})

	set (modules "${modules}" PARENT_SCOPE)

endmacro()

#

_add_all_subdirs()