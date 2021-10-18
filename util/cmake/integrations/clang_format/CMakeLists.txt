find_program (CLANG_FORMAT clang-format)

if (NOT CLANG_FORMAT)
	return()
endif()

if (NOT LEMONS_TOPLEVEL_DIR)
    message (WARNING "LEMONS_TOPLEVEL_DIR must be specified in order to create the ClangFormat target!")
    return()
endif()

set (CF_FILEUTILS_SCRIPT "${LEMONS_CMAKE_ROOT}/FileUtilities.cmake")

configure_file (${CMAKE_CURRENT_LIST_DIR}/run_clang_format.cmake run_clang_format.cmake
                @ONLY)

add_custom_target (ClangFormat
                   COMMAND cmake -P "${CMAKE_CURRENT_BINARY_DIR}/run_clang_format.cmake"
                   COMMENT "Running clang-format...")

message (STATUS " -- using clang-format! -- ")