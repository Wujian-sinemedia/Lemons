cmake_minimum_required (VERSION 3.17)

project (bv_SharedCode)

set (CMAKE_CXX_STANDARD 17)

add_subdirectory (CMake)

option (JUCE_ENABLE_MODULE_SOURCE_GROUPS "Use JUCE source groups" ON)
set_property (GLOBAL PROPERTY USE_FOLDERS YES)

juce_add_module (bv_SharedCode)
