find_program (PYTHON python3)
find_program (DOXYGEN doxygen)

if (NOT PYTHON OR NOT DOXYGEN)
	return()
endif()

set (modules_output "${CMAKE_CURRENT_LIST_DIR}/build/lemons_modules.dox")

add_custom_command (OUTPUT "${modules_output}"
				 	COMMAND "${PYTHON}" ARGS "${CMAKE_CURRENT_LIST_DIR}/scripts/prepare_doxygen_build_tree.py"
				 	VERBATIM
				 	WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
				 	COMMENT "Preparing Doxygen build tree...")

add_custom_target (Doxygen 
				   COMMAND "${DOXYGEN}"
				   DEPENDS "${modules_output}"
				   WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
				   COMMENT "Running Doxygen...")
