#[[
# Module: LemonsClangFormatIntegration

## Targets:
- ClangFormat: Runs clang-format recursively over all source files in the source tree.

This module does nothing if clang-format cannot be found at configure-time.

]]


include_guard (GLOBAL)

find_program (CLANG_FORMAT clang-format)

if (NOT CLANG_FORMAT)
	return()
endif()

set (script_name "run_clang_format.cmake")

configure_file ("${CMAKE_CURRENT_LIST_DIR}/scripts/${script_name}" "${script_name}" @ONLY)

add_custom_target (ClangFormat
                   COMMAND "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/${script_name}"
                   WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                   COMMENT "Running clang-format...")

message (VERBOSE " -- using clang-format! -- ")
