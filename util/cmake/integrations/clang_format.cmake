find_program (CLANG_FORMAT clang-format)

if (NOT CLANG_FORMAT)
	return()
endif()

if (NOT LEMONS_TOPLEVEL_DIR)
    return()
endif()

find_program (PYTHON_EXEC python3)

if (NOT PYTHON_EXEC)
    return()
endif()

add_custom_target (ClangFormat
                   COMMAND ${PYTHON_EXEC} "${LEMONS_PYTHON_SCRIPTS_DIR}/run_clang_format.py" "${LEMONS_TOPLEVEL_DIR}"
                   WORKING_DIRECTORY "${LEMONS_TOPLEVEL_DIR}"
                   COMMENT "Running clang-format...")

message (STATUS " -- using clang-format! -- ")
