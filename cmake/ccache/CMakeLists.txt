#[[
    The code in this file is based on:
        - this blog post: https://cristianadam.eu/20200113/speeding-up-c-plus-plus-github-actions-using-ccache/
        - and this repository: https://github.com/TheLartians/Ccache.cmake
]]


option (BV_USE_CCACHE "" OFF)

if (NOT ${BV_USE_CCACHE})
	return()
endif()

find_program (CCACHE_PROGRAM ccache
              PATHS $ENV{CPM_SOURCE_CACHE}/ccache)

if (NOT CCACHE_PROGRAM)
    message (WARNING "Could not enable ccache: could not find program")
    return()
endif()


# Set up wrapper scripts

set (C_LAUNCHER   "${CCACHE_PROGRAM}")
set (CXX_LAUNCHER "${CCACHE_PROGRAM}")

if (NOT DEFINED BV_CCACHE_OPTIONS)
    set (BV_CCACHE_OPTIONS "")
endif()

list (APPEND BV_CCACHE_OPTIONS "CCACHE_COMPRESS=true")
list (APPEND BV_CCACHE_OPTIONS "CCACHE_COMPRESSLEVEL=6")
list (APPEND BV_CCACHE_OPTIONS "CCACHE_MAXSIZE=800M")

set (CCCACHE_EXPORTS "")

foreach (option ${BV_CCACHE_OPTIONS})
    set (CCCACHE_EXPORTS "${CCCACHE_EXPORTS}\nexport ${option}")
endforeach()

# the configured scripts are placed in the current binary dir 
configure_file (${CMAKE_CURRENT_LIST_DIR}/launch-c.in   launch-c)
configure_file (${CMAKE_CURRENT_LIST_DIR}/launch-cxx.in launch-cxx)

execute_process (COMMAND chmod a+rx
                "${CMAKE_CURRENT_BINARY_DIR}/launch-c"
                "${CMAKE_CURRENT_BINARY_DIR}/launch-cxx")

if (CMAKE_GENERATOR STREQUAL "Xcode")
    set (CMAKE_XCODE_ATTRIBUTE_CC         "${CMAKE_CURRENT_BINARY_DIR}/launch-c" CACHE INTERNAL "")
    set (CMAKE_XCODE_ATTRIBUTE_CXX        "${CMAKE_CURRENT_BINARY_DIR}/launch-cxx" CACHE INTERNAL "")
    set (CMAKE_XCODE_ATTRIBUTE_LD         "${CMAKE_CURRENT_BINARY_DIR}/launch-c" CACHE INTERNAL "")
    set (CMAKE_XCODE_ATTRIBUTE_LDPLUSPLUS "${CMAKE_CURRENT_BINARY_DIR}/launch-cxx" CACHE INTERNAL "")
else()
    set (CMAKE_C_COMPILER_LAUNCHER   "${CMAKE_CURRENT_BINARY_DIR}/launch-c" CACHE INTERNAL "")
    set (CMAKE_CXX_COMPILER_LAUNCHER "${CMAKE_CURRENT_BINARY_DIR}/launch-cxx" CACHE INTERNAL "")
endif()

message (STATUS " -- Using ccache! -- ")
