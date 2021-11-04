include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

project (Lemons VERSION 0.1.0 LANGUAGES CXX C)


set (LEMONS_CXX_VERSION 20 CACHE STRING "C++ standard version")
mark_as_advanced (FORCE LEMONS_CXX_VERSION)

set (LEMONS_SANITIZERS_TO_ENABLE "" CACHE STRING "List of sanitizers to enable")

set (LEMONS_AAX_SDK_PATH ""  CACHE PATH "Path to the AAX SDK")
set (LEMONS_VST2_SDK_PATH "" CACHE PATH "Path to the VST2 SDK")

option (LEMONS_GENERATE_TRANSLATION_FILES "Generate translation files" ON)

option (LEMONS_IGNORE_ALL_EXTRAS "Ignore everything except what's needed to make compiling your project work" OFF)

include (CMakeDependentOption)

cmake_dependent_option (LEMONS_BUILD_TESTS "Build the Lemons unit tests" OFF "NOT LEMONS_IGNORE_ALL_EXTRAS" OFF)
cmake_dependent_option (LEMONS_BUILD_DOCS "Build the Lemons documentation" OFF "NOT LEMONS_IGNORE_ALL_EXTRAS" OFF)
cmake_dependent_option (LEMONS_BUILD_TEMPLATES "Build the Lemons project templates" OFF "NOT LEMONS_IGNORE_ALL_EXTRAS" OFF)
cmake_dependent_option (LEMONS_BUILD_TEST_LAB "Build the TestLab application" OFF "NOT LEMONS_IGNORE_ALL_EXTRAS" OFF)

if (LEMONS_BUILD_TESTS)
	set (LEMONS_PLUGINVAL_LEVEL "10" CACHE STRING "Pluginval testing level")
	mark_as_advanced (FORCE LEMONS_PLUGINVAL_LEVEL)
	enable_testing()
endif()

#

set (modules_dir "${CMAKE_CURRENT_LIST_DIR}/util/cmake/modules")

file (GLOB children RELATIVE ${modules_dir} ${modules_dir}/*)

foreach (child ${children})
	set (full_path "${modules_dir}/${child}")

	if (IS_DIRECTORY "${full_path}")
		list (APPEND CMAKE_MODULE_PATH "${full_path}")
	endif()
endforeach()

list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/modules")

#

add_subdirectory (util)

if (LEMONS_BUILD_TEMPLATES)
	add_subdirectory (templates)
endif()

if (LEMONS_BUILD_TEST_LAB)
	add_subdirectory (TestLab)
endif()
