
cmake_minimum_required (VERSION 3.17)

project (bv_SharedCode VERSION 0.0.1 LANGUAGES CXX)

set (BV_CXX_VERSION 20 CACHE INTERNAL "C++ standard version")

#

set (CMAKE_CXX_STANDARD ${BV_CXX_VERSION})

set_property (GLOBAL PROPERTY USE_FOLDERS YES)

set (CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

#

add_subdirectory (third_party)

add_subdirectory (CMake)

add_subdirectory (modules)

#

_configure_third_party_libs()
