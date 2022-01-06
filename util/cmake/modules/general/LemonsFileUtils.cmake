#[[
General filesystem utilities.

## Function:

### lemons_subdir_list
```
lemons_subdir_list (DIR <directory> RESULT <out_var> [RECURSE] [FILES])
```
Returns a list of subdirectories within the specified directory.

`DIR` is required and must be the absolute path to the directory to be searched.
`RESULT` is required and is the name of the variable to which the resulting list will be set in the parent scope.

`RECURSE` is optional, and when present, the search will recurse into all levels of subdirectories.

If the `FILES` flag is present, the function returns a list of files that are in the parent directory. If it is not present, the function returns a list of subdirectories that are in the parent directory.

]]


include_guard (GLOBAL)

include (LemonsCmakeDevTools)

function (lemons_subdir_list)

	set (options RECURSE FILES)
	set (oneValueArgs RESULT DIR)

	cmake_parse_arguments (LEMONS_SUBDIR "${options}" "${oneValueArgs}" "" ${ARGN})

	lemons_require_function_arguments (LEMONS_SUBDIR RESULT DIR)

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
  		if (LEMONS_SUBDIR_FILES)
			if (EXISTS "${dir}/${child}" AND NOT IS_DIRECTORY "${dir}/${child}" AND NOT "${child}" STREQUAL ".DS_Store")
				list (APPEND dirlist "${child}")
			endif()
  		else()
			if (IS_DIRECTORY "${dir}/${child}" AND EXISTS "${dir}/${child}")
      			list (APPEND dirlist "${child}")
    		endif()
  		endif()
  	endforeach()

  	set (${LEMONS_SUBDIR_RESULT} ${dirlist} PARENT_SCOPE)

endfunction()
