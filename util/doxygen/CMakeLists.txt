include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

project (LemonsDocs VERSION 0.0.1)

#

find_package (Doxygen REQUIRED dot)
find_package (Python3 COMPONENTS Interpreter)

if (NOT DOXYGEN_FOUND OR NOT Python3_Interpreter_FOUND)
	message (FATAL_ERROR "Doxygen dependencies missing!")
endif()

if (NOT DEFINED Lemons_SOURCE_DIR)
	set (Lemons_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/../.." CACHE INTERNAL "")
endif()

set (out_folder "${CMAKE_CURRENT_BINARY_DIR}/build")

set (DoxygenDepends "")

#

function (_lemons_configure_doxygen_file_copy_target input output)

	cmake_path (GET input FILENAME filename)

	add_custom_command (OUTPUT "${output}" MAIN_DEPENDENCY "${input}" VERBATIM
				    	COMMAND "${CMAKE_COMMAND}" ARGS -E copy "${input}" "${output}"
				    	COMMENT "Copying ${filename} to Doxygen build tree...")

	list (APPEND DoxygenDepends "${output}")
	set (DoxygenDepends ${DoxygenDepends} PARENT_SCOPE)
	
endfunction()

_lemons_configure_doxygen_file_copy_target ("${CMAKE_CURRENT_LIST_DIR}/Doxyfile" 
											"${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")

_lemons_configure_doxygen_file_copy_target ("${CMAKE_CURRENT_LIST_DIR}/DoxygenLayout.xml" 
											"${CMAKE_CURRENT_BINARY_DIR}/DoxygenLayout.xml")

_lemons_configure_doxygen_file_copy_target ("${Lemons_SOURCE_DIR}/util/assets/logo.png" 
											"${out_folder}/logo.png")

#

set (main_page_output "${out_folder}/main_page.md")

add_custom_command (OUTPUT "${main_page_output}" MAIN_DEPENDENCY "${Lemons_SOURCE_DIR}/README.md" VERBATIM
					DEPENDS "${CMAKE_CURRENT_LIST_DIR}/main_page.md"
					COMMAND Python3::Interpreter ARGS "${CMAKE_CURRENT_LIST_DIR}/scripts/main_page.py" "${Lemons_SOURCE_DIR}" "${CMAKE_CURRENT_LIST_DIR}/main_page.md" "${main_page_output}"
					COMMENT "Copying from main readme into Doxygen main page...")

list (APPEND DoxygenDepends "${main_page_output}")

#

set (modules_output "${out_folder}/lemons_modules.dox")

add_custom_command (OUTPUT "${modules_output}" VERBATIM
				 	COMMAND Python3::Interpreter ARGS "${CMAKE_CURRENT_LIST_DIR}/scripts/process_juce_modules.py" "${Lemons_SOURCE_DIR}" "${out_folder}" 
				 	COMMENT "Preparing Doxygen build tree...")

list (APPEND DoxygenDepends "${modules_output}")

#

set (cmake_modules_output "${out_folder}/CMake")

add_custom_command (OUTPUT "${cmake_modules_output}" VERBATIM
					COMMAND Python3::Interpreter ARGS "${CMAKE_CURRENT_LIST_DIR}/scripts/process_cmake_modules.py" "${Lemons_SOURCE_DIR}/util/cmake/modules" "${Lemons_SOURCE_DIR}/modules" "${cmake_modules_output}" "${CMAKE_CURRENT_LIST_DIR}/cmake_api.md"
					COMMENT "Generating documentation for CMake modules...")

list (APPEND DoxygenDepends "${cmake_modules_output}")

#

add_custom_target (Doxygen COMMAND Doxygen::doxygen
				   DEPENDS ${DoxygenDepends}
				   WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
				   COMMENT "Running Doxygen...")

add_custom_command (TARGET Doxygen POST_BUILD
					COMMAND "${CMAKE_COMMAND}" ARGS -E copy_directory "${CMAKE_CURRENT_BINARY_DIR}/doc" "${Lemons_SOURCE_DIR}/docs"
					COMMENT "Copying to output...")

