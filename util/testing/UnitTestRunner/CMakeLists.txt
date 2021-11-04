project (UnitTestRunner VERSION 0.1.0 LANGUAGES CXX)

#

juce_add_console_app (${PROJECT_NAME})

target_sources (${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/Main.cpp")

target_compile_definitions (${PROJECT_NAME} PRIVATE
    JUCE_USE_CURL=0
    JUCE_WEB_BROWSER=0)

target_link_libraries (${PROJECT_NAME} PRIVATE
    juce_core
    ${LEMONS_JUCE_MODULES}
    ${LEMONS_PLUGIN_ONLY_MODULES}
    ${LEMONS_APP_ONLY_MODULES})

add_test (NAME LemonsUnitTests 
          COMMAND ${PROJECT_NAME} 
          WORKING_DIRECTORY "${LEMONS_REPO_ROOT}")
