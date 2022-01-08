include_guard (GLOBAL)

include (LemonsCmakeDevTools)


function (lemons_parse_config_file)

	set (oneValueArgs FILE OUT_PREFIX)

	cmake_parse_arguments (LEMONS_CONFIG "" "${oneValueArgs}" "" ${ARGN})

	lemons_require_function_arguments (LEMONS_CONFIG FILE OUT_PREFIX)

	if (NOT EXISTS "${LEMONS_CONFIG_FILE}")
		message (AUTHOR_WARNING "Configuration file ${LEMONS_CONFIG_FILE} not found!")
		return()
	endif()

	file (STRINGS "${LEMONS_CONFIG_FILE}" file_lines)

	foreach (line ${file_lines})

		string (STRIP ${line} stripped_line)

		if (NOT stripped_line)
			continue()
		endif()

		string (SUBSTRING ${stripped_line} 0 1 first_char)

		if (${first_char} STREQUAL "#") # comment
			continue()
		endif()

		if (${first_char} STREQUAL "-" OR ${first_char} STREQUAL "*") # category member
			if (NOT CurrentCategoryName)
				message (AUTHOR_WARNING "Category item ${stripped_line} declared outside of category in Depsfile!")
				continue()
			endif()

			list (APPEND "${LEMONS_CONFIG_OUT_PREFIX}_${CurrentCategoryName}" "${stripped_line}")
			set (${LEMONS_CONFIG_OUT_PREFIX}_${CurrentCategoryName} "${${LEMONS_CONFIG_OUT_PREFIX}_${CurrentCategoryName}}" PARENT_SCOPE)
			
			continue()
		endif()

		# category name

		# handle last char being ':'

		string (SUBSTRING ${stripped_line} 1 -1 category_name)	
		string (STRIP ${category_name} category)

		set (CurrentCategoryName "${category}")
		set (${LEMONS_CONFIG_OUT_PREFIX}_${CurrentCategoryName} "")
	endforeach()
endfunction()
