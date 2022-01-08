cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

project (LemonsTests VERSION 0.1.0 LANGUAGES CXX C)

enable_testing()

if (NOT DEFINED CPM_Lemons_SOURCE)
    set (CPM_Lemons_SOURCE "${CMAKE_CURRENT_LIST_DIR}/../.." CACHE INTERNAL "")
endif()

if (NOT DEFINED ENV{CPM_SOURCE_CACHE})
    set (ENV{CPM_SOURCE_CACHE} "${CPM_Lemons_SOURCE}/Cache")
endif()

set (LEMONS_BUILD_TESTS ON CACHE INTERNAL "")

include ("${CPM_Lemons_SOURCE}/AddLemons.cmake")


add_custom_target (${PROJECT_NAME} COMMENT "Building all Lemons tests...")


add_subdirectory (UnitTestRunner)
add_subdirectory (CompileModulesIndividually)
