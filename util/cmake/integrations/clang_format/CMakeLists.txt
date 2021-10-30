find_program (CLANG_FORMAT clang-format)

if (NOT CLANG_FORMAT)
	return()
endif()

set (script_name "run_clang_format.cmake")

configure_file ("${CMAKE_CURRENT_LIST_DIR}/${script_name}" "${script_name}" @ONLY)

add_custom_target (ClangFormat
                   COMMAND "${CMAKE_COMMAND}" -P "${script_name}"
                   WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                   COMMENT "Running clang-format...")

message (STATUS " -- using clang-format! -- ")
