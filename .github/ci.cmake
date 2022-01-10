set (CTEST_SOURCE_DIRECTORY ${CTEST_SCRIPT_DIRECTORY}/..)

set (CTEST_BINARY_DIRECTORY ${CTEST_SCRIPT_DIRECTORY}/../Builds/tests)

set (CTEST_CONFIGURATION_TYPE Debug)

set (CTEST_GIT_INIT_SUBMODULES ON)

set (ENV{CXXFLAGS} "--coverage")

set (CTEST_COVERAGE_COMMAND "gcov")

set (CTEST_MEMORYCHECK_COMMAND "valgrind")

set (CTEST_MEMORYCHECK_TYPE "ThreadSanitizer")

if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
	set (CTEST_CMAKE_GENERATOR "Xcode")
elseif (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
	set (CTEST_CMAKE_GENERATOR "\"Visual Studio 16 2019\"")
else()
	set (CTEST_CMAKE_GENERATOR "Ninja")
endif()


include (${CTEST_SOURCE_DIRECTORY}/scripts/clean.cmake)


ctest_start ("CI build")

#ctest_update (SOURCE ${CTEST_SOURCE_DIRECTORY})

ctest_configure (OPTIONS LEMONS_BUILD_TESTS=ON)

ctest_build (CONFIGURATION Debug
			 TARGET LemonsTests)

ctest_test (INCLUDE "Lemons.")

ctest_coverage()

ctest_memcheck()

#ctest_submit()
