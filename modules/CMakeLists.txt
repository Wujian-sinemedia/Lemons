include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

project (LemonsJuceModules
		 VERSION 0.1.0
		 DESCRIPTION "All the Lemons JUCE modules"
		 HOMEPAGE_URL "www.github.com/benthevining/Lemons"
		 LANGUAGES CXX C)

#

list (APPEND CMAKE_PREFIX_PATH "${CPM_Lemons_SOURCE}/util/cmake/modules")

include ("${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}Config.cmake")

#

set (CMAKE_CXX_STANDARD 20 CACHE STRING "C++ standard version")
set (CMAKE_CXX_STANDARD_REQUIRED ON CACHE INTERNAL "")

set (CMAKE_CXX_VISIBILITY_PRESET hidden CACHE INTERNAL "")
set (CMAKE_VISIBILITY_INLINES_HIDDEN YES CACHE INTERNAL "")

set_property (GLOBAL PROPERTY USE_FOLDERS YES)
set_property (GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "Targets")

set (CMAKE_SUPPRESS_REGENERATION   TRUE CACHE INTERNAL "")
set (CMAKE_EXPORT_COMPILE_COMMANDS TRUE CACHE INTERNAL "")
set (CMAKE_INCLUDE_CURRENT_DIR     ON CACHE INTERNAL "")


# install files

include (GNUInstallDirs)

set (installDest ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

foreach (module_path ${lemonsModulePaths})
	install (DIRECTORY ${module_path}
		 	 DESTINATION ${installDest}
		 	 COMPONENT ${PROJECT_NAME})
endforeach()

install (FILES ${PROJECT_NAME}Config.cmake 
		 DESTINATION ${installDest}
		 COMPONENT ${PROJECT_NAME})

include (CMakePackageConfigHelpers)

write_basic_package_version_file ("${PROJECT_NAME}ConfigVersion.cmake"
								  VERSION ${PROJECT_VERSION}
								  COMPATIBILITY SameMajorVersion)

install (FILES "${PROJECT_NAME}ConfigVersion.cmake"
		 DESTINATION ${installDest}
		 COMPONENT ${PROJECT_NAME})
