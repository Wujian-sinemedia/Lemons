include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

project (LemonsTests VERSION 0.1.0 LANGUAGES CXX C)

enable_testing()

if (NOT DEFINED CPM_Lemons_SOURCE)
    set (CPM_Lemons_SOURCE "${CMAKE_CURRENT_LIST_DIR}/../.." CACHE INTERNAL "")
endif()

if (NOT DEFINED ENV{CPM_SOURCE_CACHE})
    set (ENV{CPM_SOURCE_CACHE} "${CPM_Lemons_SOURCE}/Cache")
endif()

include ("${CPM_Lemons_SOURCE}/scripts/AddLemons.cmake")


add_custom_target (${PROJECT_NAME} ALL
                   COMMENT "Building all Lemons tests...")


add_subdirectory (UnitTestRunner)
add_dependencies (LemonsTests UnitTestRunner)

#

configure_file (${CMAKE_CURRENT_LIST_DIR}/CompileModulesIndividually/RunModulesBuild.cmake RunModulesBuild.cmake 
                @ONLY NEWLINE_STYLE UNIX)

add_test (NAME Lemons.CompileModulesIndividually 
          COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/RunModulesBuild.cmake)

set_tests_properties (Lemons.CompileModulesIndividually PROPERTIES
                        ATTACHED_FILES "${CMAKE_CURRENT_BINARY_DIR}/logs/moduleBuild_Config.log;${CMAKE_CURRENT_BINARY_DIR}/logs/moduleBuild_ConfigError.log;${CMAKE_CURRENT_BINARY_DIR}/logs/moduleBuild_Build.log;${CMAKE_CURRENT_BINARY_DIR}/logs/moduleBuild_BuildError.log"
                        LABELS "Lemons")
