#[[
Configures a target to run clang-format over all your code.

## Targets:
- ClangFormat: Runs clang-format recursively over all source files in the source tree.

This module does nothing if clang-format cannot be found at configure-time.

## Note

If the `LEMONS_ENABLE_INTEGRATIONS` option is ON, then this module will be included by Lemons, when Lemons is added as a subdirectory.

]]


include_guard (GLOBAL)

include (LemonsCmakeDevTools)


function (lemons_configure_clang_format_integration)

    set (multiValueArgs DIRS EXCLUDE_DIRS)

    cmake_parse_arguments (LEMONS_CF "" "TARGET" "${multiValueArgs}" ${ARGN})

    lemons_require_function_arguments (LEMONS_CF TARGET DIRS)

    list (APPEND LEMONS_CF_EXCLUDE_DIRS ${CMAKE_CURRENT_BINARY_DIR})

    set (script_name "run_clang_format.cmake")

    configure_file ("${CMAKE_CURRENT_LIST_DIR}/scripts/${script_name}" "${script_name}" @ONLY)

    add_custom_target (${LEMONS_CF_TARGET}
                       COMMAND "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/${script_name}"
                       COMMENT "Running clang-format...")
endfunction()
