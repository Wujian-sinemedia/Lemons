function (bv_subdir_list)

	set (options "")
	set (oneValueArgs RESULT DIR)
	set (multiValueArgs "")

	cmake_parse_arguments (BV_SUBDIR "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	if (NOT DEFINED BV_SUBDIR_RESULT)
		bv_print_warning ("Result variable not defined in call to bv_subdir_list")
		return()
	endif()

	if (NOT DEFINED BV_SUBDIR_DIR)
		bv_print_warning ("Directory to search not defined in call to bv_subdir_list")
		return()
	endif()

  	file (GLOB children RELATIVE ${BV_SUBDIR_DIR} ${BV_SUBDIR_DIR}/*)

  	set (dirlist "")

  	foreach (child ${children})
    	if (IS_DIRECTORY ${BV_SUBDIR_DIR}/${child})
      		list (APPEND dirlist ${child})
    	endif()
  	endforeach()

  	set (${BV_SUBDIR_RESULT} ${dirlist} PARENT_SCOPE)
endfunction()


function (bv_copy_file)

	set (options "")
	set (oneValueArgs SOURCE DEST)
	set (multiValueArgs "")

	cmake_parse_arguments (BV_FILECOPY "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	if (NOT DEFINED BV_FILECOPY_SOURCE)
		bv_print_warning ("Source path not defined in call to bv_copy_file")
		return()
	endif()

	if (NOT DEFINED BV_FILECOPY_DEST)
		bv_print_warning ("Destination path not defined in call to bv_copy_file")
		return()
	endif()

	file (READ ${BV_FILECOPY_SOURCE} filetext)
	file (WRITE ${BV_FILECOPY_DEST} ${filetext})
endfunction()


function (bv_enable_all_file_permissions filepath)
	file (CHMOD ${filepath}
          PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_WRITE WORLD_EXECUTE SETUID SETGID)
endfunction()
