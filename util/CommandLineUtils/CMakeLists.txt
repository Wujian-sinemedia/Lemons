include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

project (CommandLineUtils VERSION 0.1.0 LANGUAGES CXX)

#

if (NOT DEFINED CPM_Lemons_SOURCE)
    set (CPM_Lemons_SOURCE "${CMAKE_CURRENT_LIST_DIR}/../.." CACHE INTERNAL "")
endif()

if (NOT DEFINED ENV{CPM_SOURCE_CACHE})
    set (ENV{CPM_SOURCE_CACHE} "${CPM_Lemons_SOURCE}/Cache")
endif()

#

add_subdirectory (KeyFileGenerator)
add_subdirectory (RSAKeyGenerator)
add_subdirectory (FontSerializer)
add_subdirectory (TranslationFileGenerator)

add_custom_target (CommandLineUtils COMMENT "Building command line utils...")
add_dependencies (CommandLineUtils KeyFileGenerator RSAKeyGenerator FontSerializer TranslationFileGenerator)
