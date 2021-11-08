project (UnitTestRunner VERSION 0.1.0 LANGUAGES CXX)

include (AllLemonsModules)

#

juce_add_console_app (${PROJECT_NAME})

target_sources (${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/Main.cpp")

target_compile_definitions (${PROJECT_NAME} PRIVATE
    JUCE_USE_CURL=0
    JUCE_WEB_BROWSER=0)

target_link_libraries (${PROJECT_NAME} PRIVATE juce_core AllLemonsModules)

add_test (NAME LemonsUnitTests 
          COMMAND ${PROJECT_NAME} 
          WORKING_DIRECTORY "${Lemons_SOURCE_DIR}")
