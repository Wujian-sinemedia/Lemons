find_program (CLANG_FORMAT clang-format)

if (NOT CLANG_FORMAT)
	return()
endif()

configure_file (${CMAKE_CURRENT_LIST_DIR}/run_clang_format.cmake run_clang_format)

execute_process (COMMAND chmod a+rx "${CMAKE_CURRENT_BINARY_DIR}/run_clang_format")

add_custom_target (ClangFormat
                   COMMAND cmake -P "${CMAKE_CURRENT_BINARY_DIR}/run_clang_format"
                   COMMENT "running clang format")

message (STATUS " -- using clang-format! -- ")