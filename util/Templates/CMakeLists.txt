include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

project (LemonsTemplates VERSION 0.0.1 LANGUAGES CXX)

#

if (NOT DEFINED CPM_Lemons_SOURCE)
    set (CPM_Lemons_SOURCE "${CMAKE_CURRENT_LIST_DIR}/../.." CACHE INTERNAL "")
endif()

if (NOT DEFINED ENV{CPM_SOURCE_CACHE})
    set (ENV{CPM_SOURCE_CACHE} "${CPM_Lemons_SOURCE}/Cache")
endif()

include ("${CPM_Lemons_SOURCE}/scripts/AddLemons.cmake")

include (LemonsAddPrivateSDKs)

#

add_subdirectory (app)
add_subdirectory (plugin)

#

add_custom_target (${PROJECT_NAME} ALL COMMENT "Building templates...")

add_dependencies (${PROJECT_NAME} NewApp NewPlugin)
