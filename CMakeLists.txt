
cmake_minimum_required (VERSION 3.17)

project (bv_SharedCode VERSION 0.0.1 LANGUAGES CXX)

set (BV_CXX_VERSION 20 CACHE INTERNAL "C++ standard version")

#

set (CMAKE_CXX_STANDARD ${BV_CXX_VERSION})

option (JUCE_ENABLE_MODULE_SOURCE_GROUPS "Use JUCE source groups" ON)
option (JUCE_BUILD_EXAMPLES "Build JUCE Examples" OFF)
option (JUCE_BUILD_EXTRAS "Build JUCE Extras" OFF)
set_property (GLOBAL PROPERTY USE_FOLDERS YES)

set (CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

#

add_subdirectory (CMake)

add_subdirectory (third_party)

add_subdirectory (modules)

#

_configure_ableton_link()
_configure_mts_esp()
_configure_vecops()
