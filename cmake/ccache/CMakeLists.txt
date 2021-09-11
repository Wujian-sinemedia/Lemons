option (USE_CCACHE "" OFF)
set (CCACHE_OPTIONS "" CACHE STRING "options for ccache")

if (NOT ${USE_CCACHE})
	return()
endif()

#

file (TO_CMAKE_PATH "$ENV{GITHUB_WORKSPACE}" ccache_basedir)

set (ENV{CCACHE_BASEDIR} "${ccache_basedir}")
set (ENV{CCACHE_DIR} "${ccache_basedir}/.ccache")

set (ENV{CCACHE_COMPRESS} "true")
set (ENV{CCACHE_COMPRESSLEVEL} "6")

set (ENV{CCACHE_MAXSIZE} "400M")

#

find_program (CCACHE_PROGRAM ccache)

if (NOT CCACHE_PROGRAM)
	message (WARNING "Could not enable Ccache: could not find program")
	return()
endif()

# Set up wrapper scripts
set (C_LAUNCHER   "${CCACHE_PROGRAM}")
set (CXX_LAUNCHER "${CCACHE_PROGRAM}")
set (CCCACHE_EXPORTS "")

foreach (option ${CCACHE_OPTIONS})
    set (CCCACHE_EXPORTS "${CCCACHE_EXPORTS}\nexport ${option}")
endforeach()

configure_file (${CMAKE_CURRENT_LIST_DIR}/launch-c.in   launch-c)
configure_file (${CMAKE_CURRENT_LIST_DIR}/launch-cxx.in launch-cxx)

execute_process (COMMAND chmod a+rx
                "${CMAKE_CURRENT_BINARY_DIR}/launch-c"
                "${CMAKE_CURRENT_BINARY_DIR}/launch-cxx")

if (CMAKE_GENERATOR STREQUAL "Xcode")
    # Set Xcode project attributes to route compilation and linking through our scripts
    set(CMAKE_XCODE_ATTRIBUTE_CC         "${CMAKE_CURRENT_BINARY_DIR}/launch-c" CACHE INTERNAL "")
    set(CMAKE_XCODE_ATTRIBUTE_CXX        "${CMAKE_CURRENT_BINARY_DIR}/launch-cxx" CACHE INTERNAL "")
    set(CMAKE_XCODE_ATTRIBUTE_LD         "${CMAKE_CURRENT_BINARY_DIR}/launch-c" CACHE INTERNAL "")
    set(CMAKE_XCODE_ATTRIBUTE_LDPLUSPLUS "${CMAKE_CURRENT_BINARY_DIR}/launch-cxx" CACHE INTERNAL "")
else()
    # Support Unix Makefiles and Ninja
    set(CMAKE_C_COMPILER_LAUNCHER   "${CMAKE_CURRENT_BINARY_DIR}/launch-c" CACHE INTERNAL "")
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CMAKE_CURRENT_BINARY_DIR}/launch-cxx" CACHE INTERNAL "")
endif()

message (STATUS "Using ccache")
