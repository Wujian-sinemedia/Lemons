
macro (bv_add_modules dir)

	_subdir_list (moduleFolders ${dir})

	foreach (folder ${moduleFolders})
		juce_add_module (${dir}/${folder})
	endforeach()

	list (APPEND modules ${moduleFolders})

endmacro()

#

_subdir_list (categories ${CMAKE_CURRENT_LIST_DIR})

foreach (category ${categories})

	bv_add_modules ("${CMAKE_CURRENT_LIST_DIR}/${category}")

endforeach()

set (modules "${modules}" PARENT_SCOPE)