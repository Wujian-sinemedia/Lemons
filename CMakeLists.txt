include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

project (Lemons VERSION 0.1.0 LANGUAGES CXX C)

include (CMakeDependentOption)

#

set (LEMONS_CXX_VERSION 20 CACHE STRING "C++ standard version")

set (LEMONS_SANITIZERS_TO_ENABLE "" CACHE STRING "List of sanitizers to enable")

option (LEMONS_IGNORE_ALL_EXTRAS "Ignore everything except what's needed to make compiling your project work" OFF)

cmake_dependent_option (LEMONS_GENERATE_TRANSLATION_FILES "Generate translation files" ON "NOT LEMONS_IGNORE_ALL_EXTRAS" OFF)
cmake_dependent_option (LEMONS_BUILD_TESTS "Build the Lemons unit tests" OFF "NOT LEMONS_IGNORE_ALL_EXTRAS" OFF)
cmake_dependent_option (LEMONS_BUILD_DOCS "Build the Lemons documentation" OFF "NOT LEMONS_IGNORE_ALL_EXTRAS" OFF)
cmake_dependent_option (LEMONS_BUILD_TEMPLATES "Build the Lemons project templates" OFF "NOT LEMONS_IGNORE_ALL_EXTRAS" OFF)

set (LEMONS_AAX_SDK_PATH "" CACHE STRING "Path to the AAX SDK")
set (LEMONS_VST2_SDK_PATH "" CACHE STRING "Path to the VST2 SDK")

if (LEMONS_BUILD_TESTS)
	set (LEMONS_PLUGINVAL_LEVEL "10" CACHE STRING "Pluginval testing level")
endif()

#

add_subdirectory (util/cmake)
add_subdirectory (modules)

#

if (LEMONS_BUILD_TESTS)
	enable_testing()
endif()

if (LEMONS_BUILD_DOCS)
	add_subdirectory (util/doxygen)
endif()

if (LEMONS_BUILD_TEMPLATES)
	add_subdirectory (templates)
endif()
