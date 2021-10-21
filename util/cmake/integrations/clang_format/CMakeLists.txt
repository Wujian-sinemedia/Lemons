find_program (CLANG_FORMAT clang-format)

if (NOT CLANG_FORMAT)
	return()
endif()

if (NOT LEMONS_TOPLEVEL_DIR)
    return()
endif()

set (CF_FILEUTILS_SCRIPT "${LEMONS_CMAKE_ROOT}/FileUtilities.cmake")

set (scriptName run_clang_format.cmake)

configure_file (${CMAKE_CURRENT_LIST_DIR}/${scriptName} ${scriptName} @ONLY)

add_custom_target (ClangFormat
                   COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_BINARY_DIR}/${scriptName}"
                   COMMENT "Running clang-format...")

message (STATUS " -- using clang-format! -- ")
