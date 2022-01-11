include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

project (LemonsCmakeModules
		 VERSION 0.1.0
		 DESCRIPTION "All the Lemons CMake modules"
		 HOMEPAGE_URL "www.github.com/benthevining/Lemons"
		 LANGUAGES CXX)


include (${CMAKE_CURRENT_LIST_DIR}/LemonsCmakeModulesConfig.cmake)


# install files

include (LemonsFileUtils)
include (GNUInstallDirs)

foreach (module_path ${lemonsModulePaths})

	install (DIRECTORY ${module_path}
		 	 DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
		 	 COMPONENT ${PROJECT_NAME})
endforeach()

