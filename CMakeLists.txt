include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

project (Lemons VERSION 0.1.0 LANGUAGES CXX C)


# Options and settings

set (CMAKE_CXX_STANDARD 20 CACHE STRING "C++ standard version")
mark_as_advanced (FORCE CMAKE_CXX_STANDARD)

set (LEMONS_SANITIZERS_TO_ENABLE "" CACHE STRING "List of sanitizers to enable")

set (LEMONS_AAX_SDK_PATH ""  CACHE PATH "Path to the AAX SDK")
set (LEMONS_VST2_SDK_PATH "" CACHE PATH "Path to the VST2 SDK")

option (LEMONS_ENABLE_INTEGRATIONS "Enable all available integrations by default" ON)

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

if (IOS)
	option (LEMONS_IOS_SIMULATOR "Build for an iOS simulator, rather than a real device" ON)

	if (NOT LEMONS_IOS_SIMULATOR)
		set (CMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "" CACHE STRING "10-character ID for your Apple developer account")
	endif()
else()
	set (LEMONS_MAC_SDK_VERSION "10.13" CACHE STRING "Version of the MacOS SDK to use")
	mark_as_advanced (FORCE LEMONS_MAC_SDK_VERSION)
endif()

option (LEMONS_CREATE_AGGREGATE_TARGETS "Generate ALL_APPS and ALL_PLUGINS targets, populated accordingly" ON)
set (LEMONS_JUCE_BRANCH "develop" CACHE STRING "The branch of the JUCE GitHub repository to use - should be develop or master")

mark_as_advanced (FORCE LEMONS_CREATE_AGGREGATE_TARGETS LEMONS_JUCE_BRANCH)


# General configuration

set_property (GLOBAL PROPERTY USE_FOLDERS YES)
set_property (GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "Targets")

set (CMAKE_SUPPRESS_REGENERATION   TRUE CACHE INTERNAL "")
set (CMAKE_EXPORT_COMPILE_COMMANDS TRUE CACHE INTERNAL "")
set (CMAKE_CXX_STANDARD_REQUIRED   ON CACHE INTERNAL "")
set (CMAKE_INCLUDE_CURRENT_DIR     ON CACHE INTERNAL "")

# in case any subprojects we include as subdirs try to include Lemons again via CPM...
set (CPM_Lemons_SOURCE "${CMAKE_CURRENT_LIST_DIR}" CACHE INTERNAL "")


# Add CMake modules

list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/modules")
list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/util/cmake/GetLemons")

foreach (directory "util/cmake/modules" "modules")
	set (directory "${CMAKE_CURRENT_LIST_DIR}/${directory}")
	file (GLOB children RELATIVE ${directory} ${directory}/*)

	foreach (child ${children})
		set (full_path "${directory}/${child}")

		if (IS_DIRECTORY "${full_path}")
			list (APPEND CMAKE_MODULE_PATH "${full_path}")
		endif()
	endforeach()
endforeach()

# force CMake module paths to propogate outwards to any containing projects...
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} CACHE INTERNAL "")

#

include (LemonsDefaultPlatformSettings)

if (LEMONS_ENABLE_INTEGRATIONS)
	include (LemonsCcacheIntegration)
	include (LemonsClangFormatIntegration)

	if (LEMONS_SANITIZERS_TO_ENABLE)
		include (LemonsSanitizerIntegration)
	endif()
endif()

add_subdirectory (util)
