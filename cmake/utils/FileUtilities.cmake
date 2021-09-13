function (bv_subdir_list)

	set (options "")
	set (oneValueArgs RESULT DIR)
	set (multiValueArgs "")

	cmake_parse_arguments (BV_SUBDIR "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	if (NOT DEFINED BV_SUBDIR_RESULT)
		message (WARNING "Result variable not defined in call to bv_subdir_list")
		return()
	endif()

	if (NOT DEFINED BV_SUBDIR_DIR)
		message (WARNING "Directory to search not defined in call to bv_subdir_list")
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
