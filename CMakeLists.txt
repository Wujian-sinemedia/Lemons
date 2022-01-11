include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

project (Lemons 
		 VERSION 0.1.0 
		 DESCRIPTION "Building blocks and utilities for Juce-based apps and plugins"
		 HOMEPAGE_URL "www.github.com/benthevining/Lemons"
		 LANGUAGES CXX C)

#

set (CMAKE_CXX_STANDARD 20 CACHE STRING "C++ standard version")

set (LEMONS_SANITIZERS_TO_ENABLE "" CACHE STRING "List of sanitizers to enable")

set (LEMONS_AAX_SDK_PATH ""  CACHE PATH "Path to the AAX SDK")
set (LEMONS_VST2_SDK_PATH "" CACHE PATH "Path to the VST2 SDK")

set (LEMONS_JUCE_BRANCH "develop" CACHE STRING "The branch of the JUCE GitHub repository to use - should be develop or master")

mark_as_advanced (FORCE CMAKE_CXX_STANDARD LEMONS_SANITIZERS_TO_ENABLE LEMONS_JUCE_BRANCH)


if (IOS)
	option (LEMONS_IOS_SIMULATOR "Build for an iOS simulator, rather than a real device" ON)

	if (NOT LEMONS_IOS_SIMULATOR)
		set (CMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "" CACHE STRING "10-character ID for your Apple developer account")
	endif()
elseif (APPLE)
	set (LEMONS_MAC_SDK_VERSION "10.13" CACHE STRING "Version of the MacOS SDK to use")
	mark_as_advanced (FORCE LEMONS_MAC_SDK_VERSION)

	option (LEMONS_MAC_UNIVERSAL_BINARY "Builds for x86_64 and arm64" ON)
endif()

# 

set (CMAKE_CXX_VISIBILITY_PRESET hidden CACHE INTERNAL "")
set (CMAKE_VISIBILITY_INLINES_HIDDEN YES CACHE INTERNAL "")

set_property (GLOBAL PROPERTY USE_FOLDERS YES)
set_property (GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "Targets")

set (CMAKE_SUPPRESS_REGENERATION   TRUE CACHE INTERNAL "")
set (CMAKE_EXPORT_COMPILE_COMMANDS TRUE CACHE INTERNAL "")
set (CMAKE_CXX_STANDARD_REQUIRED   ON CACHE INTERNAL "")
set (CMAKE_INCLUDE_CURRENT_DIR     ON CACHE INTERNAL "")

set (CPM_Lemons_SOURCE "${CMAKE_CURRENT_LIST_DIR}" CACHE INTERNAL "")

#

add_subdirectory (modules)
add_subdirectory (util)

