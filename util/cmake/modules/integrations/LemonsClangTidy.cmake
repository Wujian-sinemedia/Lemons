include_guard (GLOBAL)

find_program (clangTidyProgram NAMES clang-tidy)

if (NOT clangTidyProgram)
	return()
endif()

set (CMAKE_CXX_CLANG_TIDY "${clangTidyProgram}" CACHE INTERNAL "")
set (CMAKE_C_CLANG_TIDY "${clangTidyProgram}" CACHE INTERNAL "")

message (VERBOSE "Using clang-tidy")
