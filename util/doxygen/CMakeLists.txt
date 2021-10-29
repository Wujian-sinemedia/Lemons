find_program (PYTHON python3)
find_program (DOXYGEN doxygen)

if (NOT PYTHON OR NOT DOXYGEN)
	return()
endif()

set (out_folder "${CMAKE_CURRENT_LIST_DIR}/build")

#

set (cmake_api "cmake_api.md")
set (cmake_api_input "${CMAKE_CURRENT_LIST_DIR}/${cmake_api}")
set (cmake_api_output "${out_folder}/${cmake_api}")

add_custom_command (OUTPUT "${cmake_api_output}"
					MAIN_DEPENDENCY "${cmake_api_input}" 
					COMMAND "${CMAKE_COMMAND}" ARGS -E copy "${cmake_api_input}" "${cmake_api_output}"
					VERBATIM
					WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
					COMMENT "Copying CMake API to Doxygen build tree...")

#

set (logo_file "logo.png")
set (logo_input "${LEMONS_REPO_ROOT}/util/assets/${logo_file}")
set (logo_output "${out_folder}/${logo_file}")

add_custom_command (OUTPUT "${logo_output}"
					MAIN_DEPENDENCY "${logo_input}"
					COMMAND "${CMAKE_COMMAND}" ARGS -E copy "${logo_input}" "${logo_output}"
					VERBATIM
					WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
					COMMENT "Copying logo to Doxygen build tree...")

#

set (main_page "main_page.md")
set (lemons_repo_readme "${LEMONS_REPO_ROOT}/README.md")
set (main_page_output "${out_folder}/${main_page}")

add_custom_command (OUTPUT "${main_page_output}"
					MAIN_DEPENDENCY "${lemons_repo_readme}"
					DEPENDS "${CMAKE_CURRENT_LIST_DIR}/${main_page}"
					COMMAND "${CMAKE_COMMAND}" ARGS -D "MAIN_PAGE_OUTPUT=${main_page_output}" -D "LEMONS_README=${lemons_repo_readme}" -P "${CMAKE_CURRENT_LIST_DIR}/copy_main_page.cmake"
					VERBATIM
					WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
					COMMENT "Copying from main readme into Doxygen main page...")

#

set (module_process_script "${CMAKE_CURRENT_LIST_DIR}/process_modules.py")
set (modules_output "${out_folder}/lemons_modules.dox")

add_custom_command (OUTPUT "${modules_output}"
					MAIN_DEPENDENCY "${module_process_script}"
				 	COMMAND "${PYTHON}" ARGS "${module_process_script}" "${LEMONS_REPO_ROOT}" "${out_folder}"
				 	VERBATIM
				 	WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
				 	COMMENT "Preparing Doxygen build tree...")

#

add_custom_target (Doxygen 
				   COMMAND "${DOXYGEN}"
				   DEPENDS "${modules_output}" "${cmake_api_output}" "${logo_output}" "${main_page_output}"
				   WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
				   COMMENT "Running Doxygen...")
