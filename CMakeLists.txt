include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

project (Lemons 
		 VERSION 0.1.0 
		 DESCRIPTION "Building blocks and utilities for Juce-based apps and plugins"
		 HOMEPAGE_URL "www.github.com/benthevining/Lemons"
		 LANGUAGES CXX C)

#

set_property (GLOBAL PROPERTY USE_FOLDERS YES)
set_property (GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "Targets")

set (CMAKE_SUPPRESS_REGENERATION   TRUE CACHE INTERNAL "")
set (CMAKE_EXPORT_COMPILE_COMMANDS TRUE CACHE INTERNAL "")
set (CMAKE_INCLUDE_CURRENT_DIR     ON CACHE INTERNAL "")

set (CPM_Lemons_SOURCE "${CMAKE_CURRENT_LIST_DIR}" CACHE INTERNAL "")

#

add_subdirectory (util)
add_subdirectory (modules)
