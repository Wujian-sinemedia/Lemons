include_guard (GLOBAL)

include (LemonsParseConfigFile)


function (lemons_run_clean)

	set (oneValueArgs FILE DIR)

	cmake_parse_arguments (LEMONS_CLEAN "WIPE" "${oneValueArgs}" "" ${ARGN})

	lemons_require_function_arguments (LEMONS_CLEAN FILE)

	if (NOT LEMONS_CLEAN_DIR)
		set (LEMONS_CLEAN_DIR ${CMAKE_CURRENT_SOURCE_DIR})
	endif()

	lemons_make_path_absolute (VAR LEMONS_CLEAN_FILE BASE_DIR ${LEMONS_CLEAN_DIR})

	lemons_parse_config_file (FILE "${LEMONS_CLEAN_FILE}" SECTION Cleaning OUT_PREFIX CATEGORY)

	if (NOT CATEGORY_Clean)
		message (AUTHOR_WARNING "No clean items specified in config file!")
		return()
	endif()

	foreach (cleanItem ${CATEGORY_Clean})
		file (REMOVE_RECURSE "${LEMONS_CLEAN_DIR}/${cleanItem}")
	endforeach()

	if (LEMONS_CLEAN_WIPE)
		if (NOT CATEGORY_Wipe)
			message (AUTHOR_WARNING "Wipe requested, but no wipe items found in config file!")
			return()
		endif()

		foreach (wipeItem ${CATEGORY_Wipe})
			file (REMOVE_RECURSE "${LEMONS_CLEAN_DIR}/${wipeItem}")
		endforeach()
	endif()
	
endfunction()
