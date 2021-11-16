#[[
Configures a target to run clang-format over all your code.

## Targets:
- ClangFormat: Runs clang-format recursively over all source files in the source tree.

This module does nothing if clang-format cannot be found at configure-time.

## Note

If the `LEMONS_ENABLE_INTEGRATIONS` option is ON, then this module will be included by Lemons, when Lemons is added as a subdirectory.

]]


include_guard (GLOBAL)

find_program (CLANG_FORMAT clang-format)

if (NOT CLANG_FORMAT)
	return()
endif()

set (script_name "run_clang_format.cmake")

set (LEMONS_FILE_UTILS "${CMAKE_CURRENT_LIST_DIR}/../general/LemonsFileUtils.cmake")

configure_file ("${CMAKE_CURRENT_LIST_DIR}/scripts/${script_name}" "${script_name}" @ONLY)

add_custom_target (ClangFormat
                   COMMAND "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/${script_name}"
                   WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                   COMMENT "Running clang-format...")

message (VERBOSE " -- using clang-format! -- ")
