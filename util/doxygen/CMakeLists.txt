find_package (Doxygen REQUIRED dot)
find_package (Python3 COMPONENTS Interpreter)

if (NOT DOXYGEN_FOUND OR NOT Python3_Interpreter_FOUND)
	message (WARNING "Doxygen dependencies missing!")
	return()
endif()

set (out_folder "${CMAKE_CURRENT_LIST_DIR}/build")

#

set (cmake_api_input "${CMAKE_CURRENT_LIST_DIR}/cmake_api.md")
set (cmake_api_output "${out_folder}/cmake_api.md")

add_custom_command (OUTPUT "${cmake_api_output}" MAIN_DEPENDENCY "${cmake_api_input}" VERBATIM
					COMMAND "${CMAKE_COMMAND}" ARGS -E copy "${cmake_api_input}" "${cmake_api_output}"
					WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
					COMMENT "Copying CMake API to Doxygen build tree...")

#

set (logo_input "${Lemons_SOURCE_DIR}/util/assets/logo.png")
set (logo_output "${out_folder}/logo.png")

add_custom_command (OUTPUT "${logo_output}" MAIN_DEPENDENCY "${logo_input}" VERBATIM
					COMMAND "${CMAKE_COMMAND}" ARGS -E copy "${logo_input}" "${logo_output}" 
					WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
					COMMENT "Copying logo to Doxygen build tree...")

#

set (lemons_repo_readme "${Lemons_SOURCE_DIR}/README.md")
set (main_page_output "${out_folder}/main_page.md")

add_custom_command (OUTPUT "${main_page_output}" MAIN_DEPENDENCY "${lemons_repo_readme}" VERBATIM
					DEPENDS "${CMAKE_CURRENT_LIST_DIR}/main_page.md"
					COMMAND "${CMAKE_COMMAND}" ARGS -D "MAIN_PAGE_OUTPUT=${main_page_output}" -D "LEMONS_README=${lemons_repo_readme}" -P "${CMAKE_CURRENT_LIST_DIR}/copy_main_page.cmake"
					WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
					COMMENT "Copying from main readme into Doxygen main page...")

#

set (module_process_script "${CMAKE_CURRENT_LIST_DIR}/process_modules.py")
set (modules_output "${out_folder}/lemons_modules.dox")

add_custom_command (OUTPUT "${modules_output}" MAIN_DEPENDENCY "${module_process_script}" VERBATIM
				 	COMMAND Python3::Interpreter ARGS "${module_process_script}" "${Lemons_SOURCE_DIR}" "${out_folder}" 
				 	WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
				 	COMMENT "Preparing Doxygen build tree...")

#

add_custom_target (Doxygen COMMAND Doxygen::doxygen
				   DEPENDS "${modules_output}" "${cmake_api_output}" "${logo_output}" "${main_page_output}"
				   WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
				   COMMENT "Running Doxygen...")
