
_bv_subdir_list (categories ${CMAKE_CURRENT_LIST_DIR})

foreach (category ${categories})

	bv_add_juce_modules ("${CMAKE_CURRENT_LIST_DIR}/${category}")

endforeach()

set (modules "${modules}" PARENT_SCOPE)