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
# ======================================================================================

include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

project (
	LemonsDocs
	VERSION 0.1.0
	DESCRIPTION "Lemons documentation"
	HOMEPAGE_URL "www.github.com/benthevining/Lemons"
	LANGUAGES NONE)

find_package (Doxygen REQUIRED dot)
find_package (Python3 COMPONENTS Interpreter)

if(NOT TARGET Python3::Interpreter OR NOT TARGET Doxygen::doxygen)
	message (FATAL_ERROR "Doxygen dependencies missing!")
endif()

if(NOT DEFINED Lemons_SOURCE_DIR)
	set (Lemons_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/../.." CACHE INTERNAL "")
endif()

set (out_folder "${CMAKE_CURRENT_LIST_DIR}/build")

#

set (main_page_output "${out_folder}/main_page.md")

add_custom_command (
	OUTPUT "${main_page_output}"
	VERBATIM
	MAIN_DEPENDENCY "${Lemons_SOURCE_DIR}/README.md"
	DEPENDS "${CMAKE_CURRENT_LIST_DIR}/main_page.md"
			"${CMAKE_CURRENT_LIST_DIR}/scripts/main_page.py"
	COMMAND Python3::Interpreter ARGS "${CMAKE_CURRENT_LIST_DIR}/scripts/main_page.py"
			"${Lemons_SOURCE_DIR}" "${CMAKE_CURRENT_LIST_DIR}/main_page.md" "${main_page_output}"
	COMMENT "Copying from main readme into Doxygen main page...")

#

set (modules_output "${out_folder}/lemons_modules.dox")

add_custom_command (
	OUTPUT "${modules_output}"
	VERBATIM
	MAIN_DEPENDENCY "${CMAKE_CURRENT_LIST_DIR}/scripts/process_juce_modules.py"
	COMMAND Python3::Interpreter ARGS "${CMAKE_CURRENT_LIST_DIR}/scripts/process_juce_modules.py"
			"${Lemons_SOURCE_DIR}" "${out_folder}"
	COMMENT "Preparing Doxygen build tree...")

#

set (cmake_modules_output "${out_folder}/CMake")

add_custom_command (
	OUTPUT "${cmake_modules_output}"
	VERBATIM
	MAIN_DEPENDENCY "${CMAKE_CURRENT_LIST_DIR}/scripts/cmake/process_cmake_modules.py"
	COMMAND
		Python3::Interpreter ARGS "${CMAKE_CURRENT_LIST_DIR}/scripts/cmake/process_cmake_modules.py"
		"${Lemons_SOURCE_DIR}/util/cmake/modules" "${Lemons_SOURCE_DIR}/modules"
		"${Lemons_SOURCE_DIR}" "${cmake_modules_output}" "${CMAKE_CURRENT_LIST_DIR}/cmake_api.md"
	COMMENT "Generating documentation for CMake modules...")

#

add_custom_target (
	Doxygen ALL
	COMMAND Doxygen::doxygen
	VERBATIM
	DEPENDS ${main_page_output} ${modules_output} ${cmake_modules_output}
	BYPRODUCTS "doc/index.html"
	WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
	COMMENT "Running Doxygen...")

add_custom_command (TARGET Doxygen PRE_BUILD COMMAND Doxygen::doxygen --version
					COMMENT "Doxygen version:")

set_target_properties (Doxygen PROPERTIES ADDITIONAL_CLEAN_FILES
										  "${out_folder};${CMAKE_CURRENT_LIST_DIR}/doc")
