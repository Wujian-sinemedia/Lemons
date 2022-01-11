include_guard (GLOBAL)

include (LemonsJsonUtils)
include (LemonsFileUtils)

if (APPLE)
	include (${CMAKE_CURRENT_LIST_DIR}/os/install_mac.cmake)
elseif (WIN32)
	include (${CMAKE_CURRENT_LIST_DIR}/os/install_win.cmake)
else()
	include (${CMAKE_CURRENT_LIST_DIR}/os/install_linux.cmake)
endif()


option (LEMONS_DEPS_UPDATE_ALL_FIRST "Whether to update all installed system packages before installing additional dependencies" OFF)

if (LEMONS_DEPS_UPDATE_ALL_FIRST)
	_lemons_deps_os_update_func()
endif()


#####


function (lemons_get_list_of_deps_to_install)

	function (_lemons_deps_get_category_with_name rootJsonObj catName outVar)

		string (JSON numCategories LENGTH ${rootJsonObj} "Dependencies")

		math (EXPR numCategories "${numCategories} - 1" OUTPUT_FORMAT DECIMAL)

		foreach (idx RANGE ${numCategories})
			string (JSON cat_name GET ${rootJsonObj} "Dependencies" ${idx} "name")

			if ("${cat_name}" STREQUAL "${catName}")
				string (JSON cat_obj GET ${rootJsonObj} "Dependencies" ${idx})
				set (${outVar} "${cat_obj}" PARENT_SCOPE)
				return()
			endif()
		endforeach()

	endfunction()

	#
	
	function (_lemons_deps_add_from_category rootJsonObj catJsonObj outVar)

		lemons_json_array_to_list (TEXT "${catJsonObj}" ARRAY "packages" OUT outList)

		# check for dependencies on other categories

		lemons_json_array_to_list (TEXT "${catJsonObj}" ARRAY "categories" OUT subCats)

		if (subCats)
			foreach (subcategory ${subCats})

				_lemons_deps_get_category_with_name ("${rootJsonObj}" ${subcategory} subcatObj)

				if (subcatObj)
					_lemons_deps_add_from_category ("${rootJsonObj}" "${subcatObj}" subCatPackages)
					list (APPEND outList ${subCatPackages})
				else()
					message (AUTHOR_WARNING "Category dependancy ${subcategory} not found!")
				endif()
			endforeach()
		endif()

		# check for OS-specific packages

		if (APPLE)
			set (osCatName "MacPackages")
		elseif (WIN32)
			set (osCatName "WindowsPackages")
		else()
			set (osCatName "LinuxPackages")
		endif()

		lemons_json_array_to_list (TEXT "${catJsonObj}" ARRAY ${osCatName} OUT osPackages)

		if (osPackages)
			list (APPEND outList ${osPackages})
		endif()

		set (${outVar} "${outList}" PARENT_SCOPE)

	endfunction()

	#

	set (oneValueArgs FILE OUTPUT)

	cmake_parse_arguments (LEMONS_DEPS "OMIT_DEFAULT" "${oneValueArgs}" "CATEGORIES" ${ARGN})

	if (NOT LEMONS_DEPS_FILE)
		# from global config func...
		if (${PROJECT_NAME}_CONFIG_FILE)
			set (LEMONS_DEPS_FILE ${${PROJECT_NAME}_CONFIG_FILE})
		else()
			message (AUTHOR_WARNING "FILE not specified in call to ${CMAKE_CURRENT_FUNCTION}, either provide it in this call or call lemons_parse_project_configuration_file in this project first.")
		endif()
	endif()

	lemons_require_function_arguments (LEMONS_DEPS OUTPUT)
	lemons_check_for_unparsed_args (LEMONS_DEPS)

	lemons_make_path_absolute (VAR LEMONS_DEPS_FILE BASE_DIR ${CMAKE_CURRENT_LIST_DIR})

	file (READ ${LEMONS_DEPS_FILE} fileContents)

	string (JSON depsJsonObj GET ${fileContents} "Dependencies")

	set (outList "")

	if (NOT LEMONS_DEPS_OMIT_DEFAULT)
		_lemons_deps_get_category_with_name ("${fileContents}" Default defaultCatObj)

		if (defaultCatObj)
			_lemons_deps_add_from_category ("${fileContents}" "${defaultCatObj}" defaultPackages)
			list (APPEND outList ${defaultPackages})
		endif()
	endif()

	foreach (category ${LEMONS_DEPS_CATEGORIES})

		_lemons_deps_get_category_with_name ("${fileContents}" ${category} categoryObj)

		if (NOT categoryObj)
			message (AUTHOR_WARNING "Requested category ${category} not found in JSON file!")
			continue()
		endif()

		_lemons_deps_add_from_category ("${fileContents}" "${categoryObj}" categoryPackages)
		list (APPEND outList ${categoryPackages})
	endforeach()

	list (REMOVE_DUPLICATES outList)

	set (${LEMONS_DEPS_OUTPUT} ${outList} PARENT_SCOPE)

endfunction()



#####


function (lemons_install_deps)

	lemons_get_list_of_deps_to_install (OUTPUT deps_list ${ARGN})

	if (deps_list)
		_lemons_deps_os_install_func ("${deps_list}")
	else()
		message (AUTHOR_WARNING "No dependencies found to install!")
	endif()

endfunction()
