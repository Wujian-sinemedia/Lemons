function (bv_print_warning text)
	message (STATUS "")
	message (WARNING "${text}")
	message (STATUS "")
endfunction()


function (bv_list_all_cmake_variables)
	message (STATUS "")
  	get_cmake_property (vs VARIABLES)

  	foreach (v ${vs})
    	message (STATUS "${v} = '${${v}}'")
  	endforeach()
  	
  	message (STATUS "")
endfunction()


function (bv_apply_git_patch)

	set (options "")
	set (oneValueArgs GIT_DIR PATCH_PATH)
	set (multiValueArgs "")

	cmake_parse_arguments (BV_GIT_PATCH "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	if (NOT DEFINED BV_GIT_PATCH_GIT_DIR)
		bv_print_warning ("Git repo directory not specified in call to bv_apply_git_patch")
		return()
	endif()

	if (NOT DEFINED BV_GIT_PATCH_PATCH_PATH)
		bv_print_warning ("Git patch path not specified in call to bv_apply_git_patch")
		return()
	endif()

	if (NOT GIT_FOUND)
    	find_package (Git QUIET)
	endif()

	if (NOT GIT_FOUND)
		bv_print_warning ("Warning from call to bv_apply_git_patch -- git not found!")
		return()
	endif()

	execute_process (COMMAND ${GIT_EXECUTABLE} reset --hard HEAD
            		 WORKING_DIRECTORY ${BV_GIT_PATCH_GIT_DIR})

    execute_process (COMMAND ${GIT_EXECUTABLE} apply ${BV_GIT_PATCH_PATCH_PATH}
            		 WORKING_DIRECTORY ${BV_GIT_PATCH_GIT_DIR})
endfunction()
