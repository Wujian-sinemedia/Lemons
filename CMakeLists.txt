cmake_minimum_required (VERSION 3.17)

project (bv_SharedCode)

set (CMAKE_CXX_STANDARD 17)

option (JUCE_ENABLE_MODULE_SOURCE_GROUPS "Use JUCE source groups" ON)
set_property (GLOBAL PROPERTY USE_FOLDERS YES)

add_subdirectory (JUCE)

add_subdirectory (CMake)

add_subdirectory (modules)

juce_add_module (SynthBase/bv_SynthBase)

set (bv_sharedcode_dir ${CMAKE_CURRENT_LIST_DIR} CACHE INTERNAL "shared code path")