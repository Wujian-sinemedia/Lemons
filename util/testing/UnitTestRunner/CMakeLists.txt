cmake_minimum_required (VERSION 3.19)

project (UnitTestRunner VERSION 0.1.0 LANGUAGES CXX)

#

juce_add_console_app (${PROJECT_NAME})

target_sources (${PROJECT_NAME} PRIVATE Main.cpp)

target_compile_definitions (${PROJECT_NAME} PRIVATE
    JUCE_USE_CURL=0
    JUCE_WEB_BROWSER=0)

option (LEMONS_INCLUDE_JUCE_UNIT_TESTS "Include the JUCE unit tests in the Lemons test runner" OFF)

if (LEMONS_INCLUDE_JUCE_UNIT_TESTS)
    target_compile_definitions (${PROJECT_NAME} PRIVATE JUCE_UNIT_TESTS=1)
endif()

target_link_libraries (${PROJECT_NAME} PRIVATE
    juce_core
    ${LEMONS_JUCE_MODULES}
    ${LEMONS_PLUGIN_ONLY_MODULES}
    ${LEMONS_APP_ONLY_MODULES})
