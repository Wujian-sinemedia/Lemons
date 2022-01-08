include_guard (GLOBAL)

include (LemonsFileUtils)

find_program (CLANG_FORMAT clang-format)


function (lemons_run_clang_format)

	if (NOT CLANG_FORMAT)
		message (FATAL_ERROR "clang-format cannot be found!")
	endif()

	set (multiValueArgs DIRS EXCLUDE)

	cmake_parse_arguments (LEMONS_CF "" "" "${multiValueArgs}" ${ARGN})

	lemons_require_function_arguments (LEMONS_CF DIRS)

	if (LEMONS_CF_EXCLUDE)
		foreach (excludeDir ${LEMONS_CF_EXCLUDE})
			cmake_path (IS_ABSOLUTE excludeDir is_abs_path)

			if (NOT is_abs_path)
				list (REMOVE_ITEM LEMONS_CF_EXCLUDE ${excludeDir})
				list (APPEND LEMONS_CF_EXCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/${excludeDir}")
			endif()
		endforeach()
	else()
		set (LEMONS_CF_EXCLUDE "")
	endif()

	#

	function (_lemons_clang_format_process_subdir directory)

		if (NOT IS_DIRECTORY ${directory})
			return()
		endif()

		separate_arguments (clang_format_command UNIX_COMMAND "-i *.h *.hpp *.c *.cpp")

		execute_process (COMMAND ${CLANG_FORMAT} ${clang_format_command}
						 WORKING_DIRECTORY ${directory}
						 OUTPUT_QUIET ERROR_QUIET)

		lemons_subdir_list (RESULT subdirs DIR ${directory} FULL_PATHS)

		foreach (subdir ${subdirs})

			if (LEMONS_CF_EXCLUDE)
				if (${subdir} IN_LIST ${LEMONS_CF_EXCLUDE})
					continue()
				endif()
			endif()

			_lemons_clang_format_process_subdir (${subdir})
			
		endforeach()

	endfunction()

	#

	foreach (directory ${LEMONS_CF_DIRS})

		cmake_path (IS_ABSOLUTE directory is_abs_path)

		if (NOT is_abs_path)
			set (directory "${CMAKE_CURRENT_SOURCE_DIR}/${directory}")
		endif()

		_lemons_clang_format_process_subdir (${directory})

	endforeach()

endfunction()
