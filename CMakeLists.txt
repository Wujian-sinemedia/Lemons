cmake_minimum_required (VERSION 3.17)

project (bv_SharedCode)

set (CMAKE_CXX_STANDARD 17)

add_subdirectory (CMake)

juce_add_module (bv_SharedCode)
