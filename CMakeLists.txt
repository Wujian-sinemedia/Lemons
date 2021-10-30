include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.19)

project (Lemons VERSION 0.1.0 LANGUAGES CXX C)

#

option (LEMONS_WORKSPACE_BUILD "" OFF)

set (LEMONS_CXX_VERSION 20 CACHE STRING "C++ standard version")

set (LEMONS_SANITIZERS_TO_ENABLE "" CACHE STRING "List of sanitizers to enable")

set (LEMONS_CCACHE_OPTIONS "" CACHE STRING "Opions passed to ccache as environment variables")

option (LEMONS_GENERATE_TRANSLATION_FILES "Generate translation files" ON)

option (LEMONS_BUILD_TESTS "Build the Lemons unit tests" OFF)

if (LEMONS_BUILD_TESTS)
	set (LEMONS_PLUGINVAL_LEVEL "10" CACHE STRING "Pluginval testing level")
endif()

option (LEMONS_BUILD_DOCS "Build the Lemons documentation" OFF)

set (LEMONS_AAX_SDK_PATH "" CACHE STRING "Path to the AAX SDK")
set (LEMONS_VST2_SDK_PATH "" CACHE STRING "Path to the VST2 SDK")

#

file (REAL_PATH "${CMAKE_CURRENT_LIST_DIR}" lemons_repo_path)
set (LEMONS_REPO_ROOT "${lemons_repo_path}" CACHE INTERNAL "" FORCE)

add_subdirectory (util/cmake)
add_subdirectory (modules)

#

if (LEMONS_BUILD_TESTS)
	enable_testing()
endif()

if (LEMONS_BUILD_DOCS)
	add_subdirectory (util/doxygen)
endif()
