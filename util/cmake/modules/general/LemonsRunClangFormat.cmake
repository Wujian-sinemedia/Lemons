include_guard (GLOBAL)

include (LemonsFileUtils)

find_program (CLANG_FORMAT clang-format)


function (lemons_run_clang_format)

	if (NOT CLANG_FORMAT)
		message (FATAL_ERROR "clang-format cannot be found!")
	endif()

	cmake_parse_arguments (LEMONS_CF "" "DIR" "EXCLUDE" ${ARGN})

	lemons_require_function_arguments (LEMONS_CF DIR)

	if (LEMONS_CF_EXCLUDE)
		foreach (excludeDir ${LEMONS_CF_EXCLUDE})
			cmake_path (IS_ABSOLUTE excludeDir is_abs_path)

			if (NOT is_abs_path)
				list (REMOVE_ITEM LEMONS_CF_EXCLUDE ${excludeDir})
				list (APPEND LEMONS_CF_EXCLUDE "${LEMONS_CF_DIR}/${excludeDir}")
			endif()
		endforeach()
	else()
		set (LEMONS_CF_EXCLUDE "")
	endif()

	#

	execute_process (COMMAND ${CLANG_FORMAT} -i *.h *.hpp *.c *.cpp
					 WORKING_DIRECTORY ${LEMONS_CF_DIR})


	function (_lemons_run_clang_format_in_subdir directory)

		lemons_subdir_list (RESULT subdirs DIR ${directory} FULL_PATHS)

		foreach (subdir subdirs)

			cmake_path (IS_ABSOLUTE subdir is_abs_path)

			if (NOT is_abs_path)
				set (subdir "${LEMONS_CF_DIR}/${subdir}")
			endif()

			if (${subdir} IN_LIST ${LEMONS_CF_EXCLUDE})
				continue()
			endif()

			execute_process (COMMAND ${CLANG_FORMAT} -i *.h *.hpp *.c *.cpp
						 	 WORKING_DIRECTORY ${subdir})

			_lemons_run_clang_format_in_subdir (${subdir})
			
		endforeach()

	endfunction()

	_lemons_run_clang_format_in_subdir (${LEMONS_CF_DIR})

endfunction()
