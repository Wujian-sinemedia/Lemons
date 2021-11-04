include_guard (GLOBAL)

if (COMMAND CPMAddPackage)
    return()
endif()

if (NOT DEFINED ENV{CPM_SOURCE_CACHE})
	set (ENV{CPM_SOURCE_CACHE} "${CMAKE_SOURCE_DIR}/Cache")
endif()

set (lemons_cpm_path "$ENV{CPM_SOURCE_CACHE}/CPM.cmake" CACHE PATH "Path to the CPM.cmake script")
mark_as_advanced (FORCE lemons_cpm_path)

if (NOT EXISTS ${lemons_cpm_path})
    message (VERBOSE "Downloading CPM.cmake to ${lemons_cpm_path}")

    file (DOWNLOAD
      https://raw.githubusercontent.com/cpm-cmake/CPM.cmake/master/cmake/CPM.cmake
      ${lemons_cpm_path})
endif()

include (${lemons_cpm_path})
