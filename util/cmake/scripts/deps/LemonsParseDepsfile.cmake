include_guard (GLOBAL)

include (LemonsParseConfigFile)


macro (lemons_parse_depsfile filepath)
	lemons_parse_config_file (FILE ${filepath} SUBCATEGORIES_ALLOWED)
endmacro()


function (lemons_get_list_of_deps_to_install)

	set (oneValueArgs FILE OUTPUT)

	cmake_parse_arguments (LEMONS_DEPS "OMIT_DEFAULT" "${oneValueArgs}" "CATEGORIES" ${ARGN})

	lemons_require_function_arguments (LEMONS_DEPS FILE OUTPUT)

	lemons_parse_depsfile (${LEMONS_DEPS_FILE})

	set (lemons_deps_list "")

	macro (_lemons_deps_add_list depsList)
		foreach (dep_name ${depsList})
			list (APPEND lemons_deps_list ${dep_name})
		endforeach()
	endmacro()

	if (NOT LEMONS_DEPS_OMIT_DEFAULT)
		if (CATEGORY_Default)
			_lemons_deps_add_list ("${CATEGORY_Default}")
		endif()

		if (APPLE)
			if (CATEGORY_Mac)
				_lemons_deps_add_list ("${CATEGORY_Mac}")
			endif()
		elseif (WIN32)
			if (CATEGORY_Windows)
				_lemons_deps_add_list ("${CATEGORY_Windows}")
			endif()
		else()
			if (CATEGORY_Linux)
				_lemons_deps_add_list ("${CATEGORY_Linux}")
			endif()
		endif()

		if (IOS)
			if (CATEGORY_iOS)
				_lemons_deps_add_list ("${CATEGORY_iOS}")
			endif()
		elseif (ANDROID)
			if (CATEGORY_Android)
				_lemons_deps_add_list ("${CATEGORY_Android}")
			endif()
		endif()
	endif()

	foreach (category ${LEMONS_DEPS_CATEGORIES})
		if (CATEGORY_${category})
			_lemons_deps_add_list ("${CATEGORY_${category}}")
		else()
			message (AUTHOR_WARNING "Depsfile category ${category} not found!")
		endif()
	endforeach()

	if (NOT lemons_deps_list)
		message (AUTHOR_WARNING "Parsed dependency list is empty!")
		return()
	endif()

	list (REMOVE_DUPLICATES lemons_deps_list)

	set (${LEMONS_DEPS_OUTPUT} ${lemons_deps_list} PARENT_SCOPE)

endfunction()

