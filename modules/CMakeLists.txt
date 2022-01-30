# ======================================================================================
#
#  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
#  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
#  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
#  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
#  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
#  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
#
#  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
#
#  ======================================================================================

include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

project (
	LemonsJuceModules
	VERSION 0.1.0
	DESCRIPTION "All the Lemons JUCE modules"
	HOMEPAGE_URL "www.github.com/benthevining/Lemons"
	LANGUAGES CXX C)

include ("${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}Config.cmake")

# install files

include (GNUInstallDirs)
include (CMakePackageConfigHelpers)

set (installDest ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

foreach(module_path ${lemonsModulePaths})
	install (DIRECTORY ${module_path} DESTINATION ${installDest} COMPONENT ${PROJECT_NAME})
endforeach()

write_basic_package_version_file ("${PROJECT_NAME}ConfigVersion.cmake" VERSION ${PROJECT_VERSION}
								  COMPATIBILITY SameMajorVersion)

install (FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
			   "${PROJECT_NAME}Config.cmake" DESTINATION ${installDest} COMPONENT ${PROJECT_NAME})
