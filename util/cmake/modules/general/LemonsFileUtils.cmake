include_guard (GLOBAL)


#
# lemons_subdir_list
# returns a list of subdirectories found in the specified parent directory.
# 
# INPUTS:
# DIR : the parent directory to search 
# RESULT : the name of the variable to which the output list will be written in the calling scope

function (lemons_subdir_list)

	set (options "")
	set (oneValueArgs RESULT DIR)
	set (multiValueArgs "")

	cmake_parse_arguments (LEMONS_SUBDIR "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	if (NOT LEMONS_SUBDIR_RESULT)
		message (FATAL_ERROR "Result variable not defined in call to ${CMAKE_CURRENT_FUNCTION}!")
		return()
	endif()

	if (NOT LEMONS_SUBDIR_DIR)
		message (FATAL_ERROR "Directory to search not defined in call to ${CMAKE_CURRENT_FUNCTION}!")
		return()
	endif()

  	file (GLOB children RELATIVE ${LEMONS_SUBDIR_DIR} ${LEMONS_SUBDIR_DIR}/*)

  	set (dirlist "")

  	foreach (child ${children})
    	if (IS_DIRECTORY "${LEMONS_SUBDIR_DIR}/${child}")
      		list (APPEND dirlist "${child}")
    	endif()
  	endforeach()

  	set (${LEMONS_SUBDIR_RESULT} ${dirlist} PARENT_SCOPE)

endfunction()
