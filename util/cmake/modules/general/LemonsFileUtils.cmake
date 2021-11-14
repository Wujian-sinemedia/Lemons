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

	cmake_parse_arguments (LEMONS_SUBDIR "${options}" "${oneValueArgs}" "" ${ARGN})

	if (NOT LEMONS_SUBDIR_RESULT)
		message (FATAL_ERROR "Result variable not defined in call to ${CMAKE_CURRENT_FUNCTION}!")
	endif()

	if (NOT LEMONS_SUBDIR_DIR)
		message (FATAL_ERROR "Directory to search not defined in call to ${CMAKE_CURRENT_FUNCTION}!")
	endif()

	cmake_path (IS_ABSOLUTE LEMONS_SUBDIR_DIR dir_path_is_absolute)

	if (dir_path_is_absolute)
		set (dir "${LEMONS_SUBDIR_DIR}")
	else()
		set (dir "${CMAKE_CURRENT_LIST_DIR}/${LEMONS_SUBDIR_DIR}")
	endif()

	if (LEMONS_SUBDIR_RECURSE)
		file (GLOB_RECURSE children RELATIVE ${dir} ${dir}/*)
	else()
		file (GLOB children RELATIVE ${dir} ${dir}/*)
	endif()

  	set (dirlist "")

  	foreach (child ${children})
    	if (IS_DIRECTORY "${dir}/${child}")
      		list (APPEND dirlist "${child}")
    	endif()
  	endforeach()

  	set (${LEMONS_SUBDIR_RESULT} ${dirlist} PARENT_SCOPE)

endfunction()
