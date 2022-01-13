include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)


project (Lemons 
		 VERSION 0.1.0 
		 DESCRIPTION "Building blocks and utilities for Juce-based apps and plugins"
		 HOMEPAGE_URL "www.github.com/benthevining/Lemons"
		 LANGUAGES CXX C)


set (CPM_Lemons_SOURCE "${CMAKE_CURRENT_LIST_DIR}" CACHE INTERNAL "")


add_subdirectory (util)
add_subdirectory (modules)
