include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

project (LemonsCmakeModules
		 VERSION 0.1.0
		 DESCRIPTION "All the Lemons CMake modules"
		 HOMEPAGE_URL "www.github.com/benthevining/Lemons"
		 LANGUAGES NONE)


include (${CMAKE_CURRENT_LIST_DIR}/LemonsCmakeModulesConfig.cmake)


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

