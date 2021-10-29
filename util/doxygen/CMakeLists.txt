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
					COMMAND "${CMAKE_COMMAND}" -E copy "${cmake_api_input}" "${cmake_api_output}"
					VERBATIM
					WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
					COMMENT "Copying CMake API to Doxygen build tree...")

#

set (logo_file "logo.png")
set (logo_input "${LEMONS_REPO_ROOT}/util/assets/${logo_file}")
set (logo_output "${out_folder}/${logo_file}")

add_custom_command (OUTPUT "${logo_output}"
					COMMAND "${CMAKE_COMMAND}" -E copy "${logo_input}" "${logo_output}"
					VERBATIM
					WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
					COMMENT "Copying logo to Doxygen build tree...")

#

set (modules_output "${out_folder}/lemons_modules.dox")

add_custom_command (OUTPUT "${modules_output}"
				 	COMMAND "${PYTHON}" ARGS "${CMAKE_CURRENT_LIST_DIR}/scripts/prepare_doxygen_build_tree.py"
				 	VERBATIM
				 	WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
				 	COMMENT "Preparing Doxygen build tree...")

#

add_custom_target (Doxygen 
				   COMMAND "${DOXYGEN}"
				   DEPENDS "${modules_output}" "${cmake_api_output}" "${logo_output}"
				   WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
				   COMMENT "Running Doxygen...")
