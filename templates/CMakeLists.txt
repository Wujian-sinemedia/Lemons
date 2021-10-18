include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.19)

option (LEMONS_WORKSPACE_BUILD "" ON)

project (LemonsDefaultProjects VERSION 0.0.1 LANGUAGES CXX C)

set (LEMONS_TOPLEVEL_DIR "${CMAKE_CURRENT_LIST_DIR}" CACHE INTERNAL "" FORCE)

set (ENV{CPM_SOURCE_CACHE} ${CMAKE_CURRENT_LIST_DIR}/../Cache)
set (CPM_Lemons_SOURCE ${CMAKE_CURRENT_LIST_DIR}/..)

set (LEMONS_CCACHE_OPTIONS "CCACHE_BASEDIR=${CPM_Lemons_SOURCE}")
list (APPEND LEMONS_CCACHE_OPTIONS "CCACHE_DIR=$ENV{CPM_SOURCE_CACHE}/ccache/cache")

add_subdirectory (DefaultJuceApp)
add_subdirectory (DefaultJucePlugin)