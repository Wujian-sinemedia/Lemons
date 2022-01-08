include_guard (GLOBAL)

include (LemonsFileUtils)


function (lemons_parse_config_file)

	set (oneValueArgs FILE SECTION OUT_PREFIX)

	cmake_parse_arguments (LEMONS_CONFIG "" "${oneValueArgs}" "" ${ARGN})

	lemons_require_function_arguments (LEMONS_CONFIG FILE SECTION)

	if (NOT LEMONS_CONFIG_OUT_PREFIX)
		set (LEMONS_CONFIG_OUT_PREFIX "CATEGORY")
	endif()

	# parse just the lines in this section of the config file

	lemons_make_path_absolute (VAR LEMONS_CONFIG_FILE BASE_DIR ${CMAKE_CURRENT_LIST_DIR})

	file (STRINGS "${LEMONS_CONFIG_FILE}" file_lines)

	set (SectionLines "")

	foreach (line ${file_lines})

		string (STRIP ${line} stripped_line)

		if (NOT stripped_line)
			continue()
		endif()

		string (SUBSTRING ${stripped_line} 0 1 first_char)

		if (${first_char} STREQUAL "#") # comment
			continue()
		endif()

		if (${first_char} STREQUAL "{") # beginning of a section
			set (LastSectionName "${lastLine}")
			continue()
		endif()

		if (${first_char} STREQUAL "}") # end of a section
			if ("${LastSectionName}" STREQUAL "${LEMONS_CONFIG_SECTION}")
				break()
			else()
				continue()
			endif()
		endif()

		if ("${LastSectionName}" STREQUAL "${LEMONS_CONFIG_SECTION}")
			list (APPEND SectionLines "${line}")
			continue()
		endif()

		if (NOT ${first_char} STREQUAL "-")
			set (lastLine "${stripped_line}")
		endif()
	endforeach()

	# parse these lines into categories

	set (AllCategoryNames "")

	foreach (line ${SectionLines})

		string (STRIP ${line} stripped_line)

		if (NOT stripped_line)
			continue()
		endif()

		string (SUBSTRING ${stripped_line} 0 1 first_char)

		if (${first_char} STREQUAL "-") # category name

			string (SUBSTRING ${stripped_line} 1 -1 category_name)	
			string (STRIP ${category_name} category)

			set (CurrentCategoryName "${category}")
			set ("CATEGORY_${category}" "")

			list (APPEND AllCategoryNames ${category})

			continue()
		endif()

		# category member

		if (NOT CurrentCategoryName)
			message (AUTHOR_WARNING "Category item declared outside of category in Depsfile!")
			continue()
		endif()

		if (NOT ${first_char} STREQUAL "<")
			list (APPEND "CATEGORY_${CurrentCategoryName}" "${stripped_line}")
			continue()
		endif()

		# referencing another category

		string (FIND ${stripped_line} ">" end_idx REVERSE)
		string (COMPARE GREATER ${end_idx} "-1" found)

		if (NOT found)
			message (AUTHOR_WARNING "Unterminated '<' character in Depsfile!")
			continue()
		endif()

		math (EXPR end_idx "${end_idx} - 1" OUTPUT_FORMAT DECIMAL)

		string (SUBSTRING ${stripped_line} 1 ${end_idx} subcat_name)

		if (subcat_name)
			list (APPEND CATEGORY_${CurrentCategoryName} "SUBCAT_${subcat_name}")
		else()
			message (AUTHOR_WARNING "Invalid subcategory name: ${subcat_name}")
		endif()

	endforeach()


	if (NOT AllCategoryNames)
		message (AUTHOR_WARNING "No categories found in ${LEMONS_CONFIG_SECTION} section of file ${LEMONS_CONFIG_FILE}")
		return()
	endif()	

	# resolve all categories that have dependencies on other categories

	while (TRUE)

		foreach (category ${AllCategoryNames})

			if (NOT CATEGORY_${category})
				message (AUTHOR_WARNING "Category ${category} is empty!")
				continue()
			endif()

			foreach (item ${CATEGORY_${category}})

				string (FIND ${item} "SUBCAT_" subcat_substring)
				string (COMPARE GREATER ${subcat_substring} "-1" is_subcategory)

				if (NOT is_subcategory) # item isn't referencing another category
					continue()
				endif()

				list (REMOVE_ITEM CATEGORY_${category} ${item})

				string (SUBSTRING ${item} 7 -1 subcat_name)

				if (NOT subcat_name)
					message (AUTHOR_WARNING "Error parsing name of subcategory dependancy ${item}!")
					continue()
				endif()

				if (NOT CATEGORY_${subcat_name})
					message (AUTHOR_WARNING "Dependant subcategory ${subcat_name} is empty!")
					continue()
				endif()

				foreach (subcat_item ${CATEGORY_${subcat_name}})
					list (APPEND CATEGORY_${category} ${subcat_item})
				endforeach()
			endforeach()

			list (REMOVE_DUPLICATES CATEGORY_${subcat_name})

		endforeach()


		set (shouldBreak TRUE)

		foreach (category ${AllCategoryNames})
			foreach (item ${CATEGORY_${category}})
				string (FIND ${item} "SUBCAT_" subcat_substring)
				string (COMPARE GREATER ${subcat_substring} "-1" is_subcategory)

				if (is_subcategory)
					set (shouldBreak FALSE)
					break()
				endif()
			endforeach()

			if (${shouldBreak} STREQUAL "FALSE")
				break()
			endif()
		endforeach()

		if (${shouldBreak} STREQUAL "TRUE")
			break()
		endif()

	endwhile()


	foreach (category ${AllCategoryNames})
		list (REMOVE_DUPLICATES CATEGORY_${category})
		set (${LEMONS_CONFIG_OUT_PREFIX}_${category} "${CATEGORY_${category}}" PARENT_SCOPE)
	endforeach()

endfunction()

