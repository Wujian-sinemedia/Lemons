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

add_custom_target (${PROJECT_NAME} ALL
                   COMMENT "Building command line utils...")

macro (_lemons_add_cmd_line_util subprojectName)
    add_subdirectory (${subprojectName})
    add_dependencies (${PROJECT_NAME} ${subprojectName})
endmacro()

#

_lemons_add_cmd_line_util (KeyFileGenerator)
_lemons_add_cmd_line_util (RSAKeyGenerator)
_lemons_add_cmd_line_util (FontSerializer)
_lemons_add_cmd_line_util (TranslationFileGenerator)
