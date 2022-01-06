include_guard (GLOBAL)

if (TARGET UnitTestRunner)
    return()
endif()

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

project (UnitTestRunner VERSION 0.1.0 LANGUAGES CXX)

#

if (NOT DEFINED CPM_Lemons_SOURCE)
    set (CPM_Lemons_SOURCE "${CMAKE_CURRENT_LIST_DIR}/../.." CACHE INTERNAL "")
endif()

if (NOT DEFINED ENV{CPM_SOURCE_CACHE})
    set (ENV{CPM_SOURCE_CACHE} "${CPM_Lemons_SOURCE}/Cache")
endif()

set (LEMONS_BUILD_TESTS ON CACHE INTERNAL "")

include ("${CPM_Lemons_SOURCE}/AddLemons.cmake")

include (AllLemonsModules)
include (LemonsAssetsHelpers)

#

if (NOT LEMONS_BUILD_TESTS)
    message (AUTHOR_WARNING "Cmake configuration is incorrect!")
endif()

juce_add_console_app (${PROJECT_NAME})

target_sources (${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/Main.cpp")

target_compile_definitions (${PROJECT_NAME} PUBLIC
    JUCE_USE_CURL=0
    JUCE_WEB_BROWSER=0
    JUCE_DISPLAY_SPLASH_SCREEN=0
    LEMONS_UNIT_TESTS=1)

target_link_libraries (${PROJECT_NAME} PUBLIC Lemons::AllLemonsModules)

lemons_add_resources_folder (TARGET ${PROJECT_NAME} ASSET_FOLDER "${CPM_Lemons_SOURCE}/util/assets")

#

enable_testing()

add_test (NAME Lemons.UnitTests 
          COMMAND ${PROJECT_NAME} -f "logs/unit_tests.log" -i High
          WORKING_DIRECTORY "${Lemons_SOURCE_DIR}")
