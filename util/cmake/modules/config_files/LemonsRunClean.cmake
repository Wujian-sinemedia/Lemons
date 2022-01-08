include_guard (GLOBAL)

include (LemonsJsonUtils)
include (LemonsFileUtils)


function (lemons_run_clean)

	set (oneValueArgs FILE DIR)

	cmake_parse_arguments (LEMONS_CLEAN "WIPE" "${oneValueArgs}" "" ${ARGN})

	lemons_require_function_arguments (LEMONS_CLEAN FILE)

	if (NOT LEMONS_CLEAN_DIR)
		set (LEMONS_CLEAN_DIR ${CMAKE_CURRENT_SOURCE_DIR})
	endif()

	lemons_make_path_absolute (VAR LEMONS_CLEAN_FILE BASE_DIR ${LEMONS_CLEAN_DIR})

	file (READ ${LEMONS_CLEAN_FILE} file_contents)

	string (JSON cleaningObj GET "${file_contents}" "Cleaning")

	lemons_json_array_to_list (TEXT "${cleaningObj}" ARRAY "clean" OUT cleanItems)

	if (cleanItems)
		foreach (cleanItem ${CATEGORY_Clean})
			file (REMOVE_RECURSE "${LEMONS_CLEAN_DIR}/${cleanItem}")
		endforeach()
	endif()

	if (LEMONS_CLEAN_WIPE)
		lemons_json_array_to_list (TEXT "${cleaningObj}" ARRAY "wipe" OUT wipeItems)

		if (wipeItems)
			foreach (wipeItem ${CATEGORY_Wipe})
				file (REMOVE_RECURSE "${LEMONS_CLEAN_DIR}/${wipeItem}")
			endforeach()
		endif()
	endif()

endfunction()
