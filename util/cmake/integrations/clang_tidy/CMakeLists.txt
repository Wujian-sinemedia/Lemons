find_program (CLANG_TIDY clang-tidy)

if (NOT CLANG_TIDY)
    return()
endif()

set (CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY};-checks=cppcoreguidelines-*")

if (WIN32) # for MSVC
    set (CMAKE_CXX_CLANG_TIDY "${CMAKE_CXX_CLANG_TIDY};--extra-arg-before=-fms-compatibility-version=19.10")
endif()

if (NOT LEMONS_TOPLEVEL_DIR)
    message (WARNING "LEMONS_TOPLEVEL_DIR must be specified in order to create the ClangTidy target!")
    return()
endif()

set (scriptName run_clang_tidy.cmake)

configure_file (${CMAKE_CURRENT_LIST_DIR}/${scriptName} ${scriptName} @ONLY)

add_custom_target (ClangTidy
                   COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_BINARY_DIR}/${scriptName}"
                   COMMENT "Running clang-tidy...")

message (STATUS " -- using clang-tidy! -- ")