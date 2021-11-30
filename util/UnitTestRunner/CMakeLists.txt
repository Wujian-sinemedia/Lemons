include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

project (UnitTestRunner VERSION 0.1.0 LANGUAGES CXX)

#

if (NOT DEFINED CPM_Lemons_SOURCE)
    set (CPM_Lemons_SOURCE "${CMAKE_CURRENT_LIST_DIR}/../.." CACHE INTERNAL "")
endif()

if (NOT DEFINED ENV{CPM_SOURCE_CACHE})
    set (ENV{CPM_SOURCE_CACHE} "${CPM_Lemons_SOURCE}/Cache")
endif()

include ("${CPM_Lemons_SOURCE}/util/cmake/GetLemons/LemonsGetCPM.cmake")

CPMAddPackage (
        NAME Lemons
        GITHUB_REPOSITORY benthevining/Lemons
        GIT_TAG origin/main
        OPTIONS "LEMONS_BUILD_TESTS ON")

include (LemonsTestingModules)

#

juce_add_console_app (${PROJECT_NAME})

target_sources (${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/Main.cpp")

target_compile_definitions (${PROJECT_NAME} PRIVATE
    JUCE_USE_CURL=0
    JUCE_WEB_BROWSER=0
    JUCE_DISPLAY_SPLASH_SCREEN=0)

target_link_libraries (${PROJECT_NAME} PUBLIC LemonsTestingModules)

#

enable_testing()

add_test (NAME LemonsUnitTests 
          COMMAND ${PROJECT_NAME} 
          WORKING_DIRECTORY "${Lemons_SOURCE_DIR}")
