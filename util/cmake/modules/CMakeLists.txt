include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

project (LemonsCmakeModules
		 VERSION 0.1.0
		 DESCRIPTION "All the Lemons CMake modules"
		 HOMEPAGE_URL "www.github.com/benthevining/Lemons"
		 LANGUAGES NONE)


# add subfolders to module path

file (GLOB children RELATIVE ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/*)

foreach (child ${children})
	set (full_path "${CMAKE_CURRENT_LIST_DIR}/${child}")

	if (IS_DIRECTORY "${full_path}")
		file (REAL_PATH "${full_path}" _abs_path EXPAND_TILDE)
		list (APPEND lemonsModulePaths "${_abs_path}")
	endif()
endforeach()

list (APPEND CMAKE_MODULE_PATH "${lemonsModulePaths}")
list (APPEND LEMONS_CMAKE_MODULE_PATH "${lemonsModulePaths}")

set (LEMONS_CMAKE_MODULE_PATH "${LEMONS_CMAKE_MODULE_PATH}" CACHE INTERNAL "")


# install files

include (LemonsFileUtils)

foreach (module_path ${lemonsModulePaths})

	lemons_subdir_list (DIR ${module_path}
						RESULT dir_files
						FILES FULL_PATHS RECURSE)

	install (FILES ${dir_files}
		 	 DESTINATION share/cmake/${PROJECT_NAME}
		 	 COMPONENT ${PROJECT_NAME})
endforeach()

