include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.19)

project (bv_SharedCode VERSION 0.0.1 LANGUAGES CXX C)

set (BV_CXX_VERSION 20 CACHE INTERNAL "C++ standard version")

set (BV_SHAREDCODE_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE INTERNAL "Path to the Shared-code repo")

add_subdirectory (cmake)
add_subdirectory (modules)

bv_configure_precommit_git_hook (${CMAKE_CURRENT_LIST_DIR})
bv_update_github_scripts (${CMAKE_CURRENT_LIST_DIR} FALSE)