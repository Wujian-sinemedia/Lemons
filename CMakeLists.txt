cmake_minimum_required (VERSION 3.17)

project (bv_SharedCode VERSION 0.0.1 LANGUAGES CXX C)

set (BV_CXX_VERSION 20 CACHE INTERNAL "C++ standard version")
set (BV_SHAREDCODE_DIR "${CMAKE_CURRENT_LIST_DIR}" CACHE INTERNAL "Path to the Shared-code source")

add_subdirectory (cmake)
add_subdirectory (modules)

include (cmake/public/public_api.cmake)

add_subdirectory (cmake/internal/project_config)
