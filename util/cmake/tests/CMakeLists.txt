include_guard (GLOBAL)

if (NOT LEMONS_BUILD_TESTS)
	message (AUTHOR_WARNING "Cmake configuration is incorrect!")
endif()

enable_testing()

#include ("${CMAKE_CURRENT_LIST_DIR}/RunAllBuilds.cmake")
include ("${CMAKE_CURRENT_LIST_DIR}/ConfigCmakeUnitTests.cmake")
