include_guard (GLOBAL)

if (COMMAND CPMAddPackage)
    return()
endif()

if (NOT DEFINED ENV{CPM_SOURCE_CACHE})
	set (ENV{CPM_SOURCE_CACHE} "${CMAKE_SOURCE_DIR}/Cache")
endif()

set (LEMONS_CPM_PATH "$ENV{CPM_SOURCE_CACHE}/CPM.cmake" CACHE PATH "Path to the CPM.cmake script")
mark_as_advanced (FORCE LEMONS_CPM_PATH)

if (NOT EXISTS ${LEMONS_CPM_PATH})
    message (VERBOSE "Downloading CPM.cmake to ${LEMONS_CPM_PATH}")

    file (DOWNLOAD
      https://raw.githubusercontent.com/cpm-cmake/CPM.cmake/master/cmake/CPM.cmake
      ${LEMONS_CPM_PATH})
endif()

include (${LEMONS_CPM_PATH})
