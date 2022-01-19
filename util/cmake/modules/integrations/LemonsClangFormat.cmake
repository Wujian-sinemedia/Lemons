#[[
Configures a target to run clang-format over all your code.

## Targets:
- ClangFormat: Runs clang-format recursively over all source files in the source tree.

This module does nothing if clang-format cannot be found at configure-time.

## Note

If the `LEMONS_ENABLE_INTEGRATIONS` option is ON, then this module will be included by Lemons, when Lemons is added as a subdirectory.

]]


include_guard (GLOBAL)

include (LemonsFileUtils)
include (LemonsCmakeDevTools)


find_program (CLANG_FORMAT clang-format)


function (lemons_configure_clang_format_integration)

    set (multiValueArgs DIRS EXCLUDE_DIRS)

    cmake_parse_arguments (LEMONS_CF "" "TARGET" "${multiValueArgs}" ${ARGN})

    lemons_require_function_arguments (LEMONS_CF TARGET DIRS)

    list (APPEND LEMONS_CF_EXCLUDE_DIRS ${CMAKE_CURRENT_BINARY_DIR})

    set (script_name "run_clang_format.cmake")

    configure_file ("${CMAKE_CURRENT_LIST_DIR}/scripts/${script_name}" "${script_name}" @ONLY)

    add_custom_target (${LEMONS_CF_TARGET}
                       COMMAND "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/${script_name}"
                       COMMENT "Running clang-format...")
endfunction()


function (lemons_run_clang_format)

	if (NOT CLANG_FORMAT)
		message (WARNING "clang-format cannot be found!")
		return()
	endif()

	set (multiValueArgs DIRS EXCLUDE)

	cmake_parse_arguments (LEMONS_CF "" "" "${multiValueArgs}" ${ARGN})

	lemons_require_function_arguments (LEMONS_CF DIRS)
	lemons_check_for_unparsed_args (LEMONS_CF)

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

	list (APPEND LEMONS_CF_EXCLUDE ${CMAKE_CURRENT_BINARY_DIR})

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

		lemons_make_path_absolute (VAR directory BASE_DIR ${CMAKE_CURRENT_LIST_DIR})

		_lemons_clang_format_process_subdir (${directory})

	endforeach()

endfunction()
