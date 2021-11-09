#[[
General filesystem utilities.

## Function:

### lemons_subdir_list
```
lemons_subdir_list (DIR <directory> RESULT <out_var> [RECURSE])
```
Returns a list of subdirectories within the specified directory.

`DIR` is required and must be the absolute path to the directory to be searched.
`RESULT` is required and is the name of the variable to which the resulting list will be set in the parent scope.

`RECURSE` is optional, and when present, the search will recurse into all levels of subdirectories.

]]


include_guard (GLOBAL)


function (lemons_subdir_list)

	set (options RECURSE)
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

	if (LEMONS_SUBDIR_RECURSE)
		file (GLOB_RECURSE children RELATIVE ${LEMONS_SUBDIR_DIR} ${LEMONS_SUBDIR_DIR}/*)
	else()
		file (GLOB children RELATIVE ${LEMONS_SUBDIR_DIR} ${LEMONS_SUBDIR_DIR}/*)
	endif()

  	set (dirlist "")

  	foreach (child ${children})
    	if (IS_DIRECTORY "${LEMONS_SUBDIR_DIR}/${child}")
      		list (APPEND dirlist "${child}")
    	endif()
  	endforeach()

  	set (${LEMONS_SUBDIR_RESULT} ${dirlist} PARENT_SCOPE)

endfunction()
