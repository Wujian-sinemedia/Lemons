include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

project (LemonsTests VERSION 0.1.0 LANGUAGES CXX C)

enable_testing()

if (NOT DEFINED CPM_Lemons_SOURCE)
    set (CPM_Lemons_SOURCE "${CMAKE_CURRENT_LIST_DIR}/../.." CACHE INTERNAL "")
endif()

if (NOT DEFINED ENV{CPM_SOURCE_CACHE})
    set (ENV{CPM_SOURCE_CACHE} "${CPM_Lemons_SOURCE}/Cache")
endif()

include ("${CPM_Lemons_SOURCE}/scripts/AddLemons.cmake")


add_subdirectory (UnitTestRunner)
add_subdirectory (CompileModulesIndividually)
