#[[
    The code in this file is based on:
        - this blog post: https://cristianadam.eu/20200113/speeding-up-c-plus-plus-github-actions-using-ccache/
        - and this repository: https://github.com/TheLartians/Ccache.cmake

    Configures ccache support for all builds in this source tree. 
    (Specifically, works by configuring a shell script that invokes ccache with the correct arguments for each build, then sets that shell script as the C and C++ launcher for these builds.)
    
    Ccache can drastically improve build time, and there's not really a good reason NOT to use it if it's installed on your system.
]]


find_program (CCACHE_PROGRAM ccache
              PATHS $ENV{CPM_SOURCE_CACHE}/ccache)

if (NOT CCACHE_PROGRAM)
    message (STATUS "Could not enable ccache: could not find program")
    return()
endif()

set (C_LAUNCHER   "${CCACHE_PROGRAM}")
set (CXX_LAUNCHER "${CCACHE_PROGRAM}")

if (NOT DEFINED LEMONS_CCACHE_OPTIONS)
    set (LEMONS_CCACHE_OPTIONS "")
endif()

list (APPEND LEMONS_CCACHE_OPTIONS "CCACHE_COMPRESS=true")
list (APPEND LEMONS_CCACHE_OPTIONS "CCACHE_COMPRESSLEVEL=6")
list (APPEND LEMONS_CCACHE_OPTIONS "CCACHE_MAXSIZE=800M")

set (CCCACHE_EXPORTS "")

foreach (option ${LEMONS_CCACHE_OPTIONS})
    set (CCCACHE_EXPORTS "${CCCACHE_EXPORTS}\nexport ${option}")
endforeach()

configure_file (${CMAKE_CURRENT_LIST_DIR}/launch-c.in   launch-c)
configure_file (${CMAKE_CURRENT_LIST_DIR}/launch-cxx.in launch-cxx)

execute_process (COMMAND chmod a+rx
                "${CMAKE_CURRENT_BINARY_DIR}/launch-c"
                "${CMAKE_CURRENT_BINARY_DIR}/launch-cxx")

if (CMAKE_GENERATOR MATCHES "Xcode")
    set (CMAKE_XCODE_ATTRIBUTE_CC         "${CMAKE_CURRENT_BINARY_DIR}/launch-c" CACHE INTERNAL "" FORCE)
    set (CMAKE_XCODE_ATTRIBUTE_CXX        "${CMAKE_CURRENT_BINARY_DIR}/launch-cxx" CACHE INTERNAL "" FORCE)
    set (CMAKE_XCODE_ATTRIBUTE_LD         "${CMAKE_CURRENT_BINARY_DIR}/launch-c" CACHE INTERNAL "" FORCE)
    set (CMAKE_XCODE_ATTRIBUTE_LDPLUSPLUS "${CMAKE_CURRENT_BINARY_DIR}/launch-cxx" CACHE INTERNAL "" FORCE)
else()
    set (CMAKE_C_COMPILER_LAUNCHER   "${CMAKE_CURRENT_BINARY_DIR}/launch-c" CACHE INTERNAL "" FORCE)
    set (CMAKE_CXX_COMPILER_LAUNCHER "${CMAKE_CURRENT_BINARY_DIR}/launch-cxx" CACHE INTERNAL "" FORCE)
endif()

message (STATUS " -- Using ccache! -- ")
