include_guard (GLOBAL)

find_program (cppCheckProgram NAMES cppcheck)

if (NOT cppCheckProgram)
	return ()
endif ()

set (CMAKE_CXX_CPPCHECK "${cppCheckProgram}" CACHE INTERNAL "")

set (CMAKE_EXPORT_COMPILE_COMMANDS TRUE CACHE INTERNAL "")

message (STATUS "Using cppcheck")
