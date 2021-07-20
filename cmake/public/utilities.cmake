function (bv_list_all_cmake_variables)
	message (STATUS "")
  	get_cmake_property (vs VARIABLES)
  	foreach (v ${vs})
    	message (STATUS "${v} = '${${v}}'")
  	endforeach()
  	message (STATUS "")
endfunction()


function (bv_apply_git_patch gitdir patch_path)
	if (NOT GIT_FOUND)
    	find_package (Git QUIET)
	endif()

	if (NOT GIT_FOUND)
		message (WARNING "Git not found!")
		return()
	endif()

	execute_process (COMMAND ${GIT_EXECUTABLE} reset --hard HEAD
            		 WORKING_DIRECTORY ${gitdir})

    execute_process (COMMAND ${GIT_EXECUTABLE} apply ${patch_path}
            		 WORKING_DIRECTORY ${gitdir})
endfunction()
