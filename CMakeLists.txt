include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.19)

project (Lemons VERSION 0.1.0 LANGUAGES CXX C)

set (LEMONS_CXX_VERSION 20 CACHE INTERNAL "C++ standard version")

add_subdirectory (util/cmake)
add_subdirectory (modules)
