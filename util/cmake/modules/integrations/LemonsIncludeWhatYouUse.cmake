include_guard (GLOBAL)

find_program (iwyu_path NAMES include-what-you-use iwyu)

if (NOT iwyu_path)
	return()
endif()

set (CMAKE_CXX_INCLUDE_WHAT_YOU_USE "${iwyu_path}" CACHE INTERNAL "")
set (CMAKE_C_INCLUDE_WHAT_YOU_USE "${iwyu_path}" CACHE INTERNAL "")

message (VERBOSE "Enabled include-what-you-use")
