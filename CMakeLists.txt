
cmake_minimum_required (VERSION 3.17)

project (bv_SharedCode)

set (CMAKE_CXX_STANDARD 17)

option (JUCE_ENABLE_MODULE_SOURCE_GROUPS "Use JUCE source groups" ON)
option (JUCE_BUILD_EXAMPLES "Build JUCE Examples" OFF)
option (JUCE_BUILD_EXTRAS "Build JUCE Extras" OFF)
set_property (GLOBAL PROPERTY USE_FOLDERS YES)

set (bv_sharedcode_dir ${CMAKE_CURRENT_LIST_DIR} CACHE INTERNAL "shared code path")

# No ZERO_CHECK target on XCode (it helps bust cache for cmake)
if ("${CMAKE_GENERATOR}" STREQUAL "Xcode")
    set (CMAKE_SUPPRESS_REGENERATION TRUE)
endif()

# clang-tidy needs this to be happy
set (CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

#########

add_subdirectory (JUCE)

add_subdirectory (CMake)

add_subdirectory (modules)