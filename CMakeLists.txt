# ======================================================================================
#
#  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
#  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
#  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
#  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
#  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
#  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
#
#  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
#
#  ======================================================================================

include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.22 FATAL_ERROR)

project (
	Lemons
	VERSION 0.1.0
	DESCRIPTION "Building blocks and utilities for JUCE-based apps and plugins"
	HOMEPAGE_URL "www.github.com/benthevining/Lemons"
	LANGUAGES CXX C)

set (CPM_Lemons_SOURCE "${CMAKE_CURRENT_LIST_DIR}" CACHE INTERNAL "")

file (REAL_PATH "${CMAKE_CURRENT_LIST_DIR}/util/cmake/modules" LemonsCmakeModules_DIR EXPAND_TILDE)
file (REAL_PATH "${CMAKE_CURRENT_LIST_DIR}/modules" LemonsJuceModules_DIR EXPAND_TILDE)

add_subdirectory (util)
add_subdirectory (modules)
