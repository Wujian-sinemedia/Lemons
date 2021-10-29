include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.19)

project (Lemons VERSION 0.1.0 LANGUAGES CXX C)

set (LEMONS_CXX_VERSION 20 CACHE INTERNAL "C++ standard version")


file (REAL_PATH "${CMAKE_CURRENT_LIST_DIR}" lemons_repo_path)
set (LEMONS_REPO_ROOT "${lemons_repo_path}" CACHE INTERNAL "" FORCE)

add_subdirectory (util/cmake)
add_subdirectory (modules)

#

option (LEMONS_BUILD_TESTS "Build the Lemons unit tests" OFF)

if (LEMONS_BUILD_TESTS)
	enable_testing()
endif()
