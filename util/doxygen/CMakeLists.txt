find_package (Doxygen REQUIRED dot)
find_package (Python3 COMPONENTS Interpreter)

if (NOT DOXYGEN_FOUND OR NOT Python3_Interpreter_FOUND)
	message (FATAL_ERROR "Doxygen dependencies missing!")
	return()
endif()

set (out_folder "${CMAKE_CURRENT_BINARY_DIR}/build")

#

set (lemons_repo_readme "${Lemons_SOURCE_DIR}/README.md")
set (main_page_output "${out_folder}/main_page.md")

add_custom_command (OUTPUT "${main_page_output}" MAIN_DEPENDENCY "${lemons_repo_readme}" VERBATIM
					DEPENDS "${CMAKE_CURRENT_LIST_DIR}/main_page.md"
					COMMAND "${CMAKE_COMMAND}" ARGS -D "MAIN_PAGE_OUTPUT=${main_page_output}" -D "LEMONS_README=${lemons_repo_readme}" -P "${CMAKE_CURRENT_LIST_DIR}/copy_main_page.cmake"
					WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
					COMMENT "Copying from main readme into Doxygen main page...")

#

set (doxyfile_input "${CMAKE_CURRENT_LIST_DIR}/Doxyfile")
set (doxyfile_output "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")

add_custom_command (OUTPUT "${doxyfile_output}" MAIN_DEPENDENCY "${doxyfile_input}" VERBATIM
				    COMMAND "${CMAKE_COMMAND}" ARGS -E copy "${doxyfile_input}" "${doxyfile_output}"
				    WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
				    COMMENT "Copying Doxyfile to Doxygen build tree...")

set (logo_input "${Lemons_SOURCE_DIR}/util/assets/logo.png")
set (logo_output "${out_folder}/logo.png")

add_custom_command (OUTPUT "${logo_output}" MAIN_DEPENDENCY "${logo_input}" VERBATIM
					COMMAND "${CMAKE_COMMAND}" ARGS -E copy "${logo_input}" "${logo_output}" 
					WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
					COMMENT "Copying logo to Doxygen build tree...")

#

set (modules_output "${out_folder}/lemons_modules.dox")

add_custom_command (OUTPUT "${modules_output}" VERBATIM
				 	COMMAND Python3::Interpreter ARGS "${CMAKE_CURRENT_LIST_DIR}/scripts/process_juce_modules.py" "${Lemons_SOURCE_DIR}" "${out_folder}" 
				 	WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
				 	COMMENT "Preparing Doxygen build tree...")

set (DoxyLayoutOutput "${CMAKE_CURRENT_BINARY_DIR}/DoxygenLayout.xml")

add_custom_command (OUTPUT "${DoxyLayoutOutput}" VERBATIM
					COMMAND Python3::Interpreter ARGS "${CMAKE_CURRENT_LIST_DIR}/scripts/process_cmake_modules.py" "${Lemons_SOURCE_DIR}/util/cmake/modules" "${Lemons_SOURCE_DIR}/modules" "${out_folder}" "${CMAKE_CURRENT_LIST_DIR}/DoxygenLayout.xml" "${DoxyLayoutOutput}"
					WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
					COMMENT "Generating documentation for CMake modules...")

#

add_custom_target (Doxygen COMMAND Doxygen::doxygen
				   DEPENDS "${DoxyLayoutOutput}" "${modules_output}" "${doxyfile_output}" "${logo_output}" "${main_page_output}"
				   WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
				   COMMENT "Running Doxygen...")

add_custom_command (TARGET Doxygen POST_BUILD
					COMMAND "${CMAKE_COMMAND}" ARGS -E copy_directory "${CMAKE_CURRENT_BINARY_DIR}/doc" "${Lemons_SOURCE_DIR}/docs"
					COMMENT "Copying to output...")

