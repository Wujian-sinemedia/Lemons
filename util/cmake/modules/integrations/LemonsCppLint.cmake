include_guard (GLOBAL)

find_program (cppLintProgram NAMES cpplint)

if (NOT cppLintProgram)
	return ()
endif ()

set (
	CMAKE_CXX_CPPLINT
	"${cppLintProgram}"
	CACHE INTERNAL "")
set (
	CMAKE_C_CPPLINT
	"${cppLintProgram}"
	CACHE INTERNAL "")

message (STATUS "Using cpplint")
