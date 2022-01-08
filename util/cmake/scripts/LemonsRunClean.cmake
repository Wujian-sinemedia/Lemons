include_guard (GLOBAL)

include (LemonsParseConfigFile)


function (lemons_run_clean)

	set (oneValueArgs FILE DIR)

	cmake_parse_arguments (LEMONS_CLEAN "WIPE" "${oneValueArgs}" "" ${ARGN})

	lemons_require_function_arguments (LEMONS_CLEAN FILE)

	if (NOT LEMONS_CLEAN_DIR)
		set (LEMONS_CLEAN_DIR ${CMAKE_CURRENT_SOURCE_DIR})
	endif()

	cmake_path (IS_ABSOLUTE LEMONS_CLEAN_FILE absPath)

	if (NOT absPath)
		set (LEMONS_CLEAN_FILE "${LEMONS_CLEAN_DIR}/${LEMONS_CLEAN_FILE}")
	endif()

	lemons_parse_config_file (OUT_PREFIX configFile FILE "${LEMONS_CLEAN_FILE}")

	if (NOT configFile_Clean)
		message (AUTHOR_WARNING "No clean items specified in config file!")
		return()
	endif()

	foreach (cleanItem ${configFile_Clean})
		file (REMOVE_RECURSE "${LEMONS_CLEAN_DIR}/${cleanItem}")
	endforeach()

	if (LEMONS_CLEAN_WIPE)
		if (NOT configFile_Wipe)
			message (AUTHOR_WARNING "Wipe requested, but no wipe items found in config file!")
			return()
		endif()

		foreach (wipeItem ${configFile_Wipe})
			file (REMOVE_RECURSE "${LEMONS_CLEAN_DIR}/${wipeItem}")
		endforeach()
	endif()
endfunction()
