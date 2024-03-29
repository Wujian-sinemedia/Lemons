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
# ======================================================================================

include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.22 FATAL_ERROR)

include (LemonsDefaultPlatformSettings)

option (LEMONS_ENABLE_INTEGRATIONS "Enable all available integrations by default" ON)

if(LEMONS_ENABLE_INTEGRATIONS)
	include (LemonsAllIntegrations)
endif()

set_property (GLOBAL PROPERTY REPORT_UNDEFINED_PROPERTIES
							  "${PROJECT_SOURCE_DIR}/logs/undefined_properties.log")

set (CMAKE_CXX_STANDARD 20 CACHE STRING "C++ standard version")
set (CMAKE_CXX_STANDARD_REQUIRED ON CACHE INTERNAL "")

set (CMAKE_CXX_VISIBILITY_PRESET hidden CACHE INTERNAL "")
set (CMAKE_VISIBILITY_INLINES_HIDDEN YES CACHE INTERNAL "")

set_property (GLOBAL PROPERTY USE_FOLDERS YES)
set_property (GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "Targets")

set (CMAKE_SUPPRESS_REGENERATION TRUE CACHE INTERNAL "")

set (ENV{CMAKE_EXPORT_COMPILE_COMMANDS} TRUE)
set (CMAKE_EXPORT_COMPILE_COMMANDS TRUE CACHE INTERNAL "")

if(NOT DEFINED ENV{CMAKE_INSTALL_MODE})
	set (ENV{CMAKE_INSTALL_MODE} ABS_SYMLINK_OR_COPY)
endif()

#

include (CheckIPOSupported)

check_ipo_supported (RESULT result OUTPUT output)

if(result)
	set (ENV{CMAKE_INTERPROCEDURAL_OPTIMIZATION} ON)
	message (VERBOSE "Enabling IPO")
endif()

#

function(lemons_enable_coverage_flags target)

	if(NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
		message (
			WARNING
				"Coverage flags are not supported with your current compiler: ${CMAKE_CXX_COMPILER_ID}"
			)
		return ()
	endif()

	target_compile_options (
		${target}
		PUBLIC -O0 # no optimization
			   -g # generate debug info
			   --coverage # sets all required flags
		)

	target_link_options (${target} PUBLIC --coverage)

endfunction()
