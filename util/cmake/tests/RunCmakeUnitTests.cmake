set (CPM_Lemons_SOURCE @CPM_Lemons_SOURCE@)

include ("${CPM_Lemons_SOURCE}/AddCMakeModules.cmake")

set (CMAKE_MODULE_PATH ${LEMONS_CMAKE_MODULE_PATH} CACHE INTERNAL "")

message (STATUS "Running CMake unit tests...")

foreach (filepath @TEST_FILE_PATHS@)

	include (${filepath})
	
endforeach()
