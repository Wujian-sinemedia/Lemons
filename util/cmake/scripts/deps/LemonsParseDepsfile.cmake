include_guard (GLOBAL)


function (lemons_parse_depsfile filepath)

	file (STRINGS "${filepath}" file_lines)

	foreach (line ${file_lines})

		string (STRIP ${line} stripped_line)

		if (NOT stripped_line)
			continue()
		endif()

		string (SUBSTRING ${stripped_line} 0 1 first_char)

		if (${first_char} STREQUAL "#") # comment
			continue()
		endif()

		if (${first_char} STREQUAL "-") # category name

			string (SUBSTRING ${stripped_line} 1 -1 category_name)	
			string (STRIP ${category_name} category)

			set (CurrentCategoryName "${category}")
			set ("CATEGORY_${CurrentCategoryName}" "")

			continue()
		endif()

		# category member

		if (NOT CurrentCategoryName)
			message (AUTHOR_WARNING "Category item declared outside of category in Depsfile!")
			continue()
		endif()

		if (NOT ${first_char} STREQUAL "<") # simple category member

			list (APPEND "CATEGORY_${CurrentCategoryName}" "${stripped_line}")
			set (CATEGORY_${CurrentCategoryName} "${CATEGORY_${CurrentCategoryName}}" PARENT_SCOPE)

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
			message (STATUS "subcategory name: ${subcat_name}")

			if (CATEGORY_${subcat_name})
				list (APPEND CATEGORY_${CurrentCategoryName} ${CATEGORY_${subcat_name}})
				set (CATEGORY_${CurrentCategoryName} "${CATEGORY_${CurrentCategoryName}}" PARENT_SCOPE)
			else()
				message (AUTHOR_WARNING "Dependant subcategory ${CATEGORY_${subcat_name}} does not exist!")
			endif()
		endif()

	endforeach()

endfunction()
