include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

project (UnitTestRunner VERSION 0.1.0 LANGUAGES CXX)

include (AllLemonsModules)
include (LemonsAssetsHelpers)

#

juce_add_console_app (${PROJECT_NAME})

target_sources (${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/Main.cpp")

target_compile_definitions (${PROJECT_NAME} PRIVATE
    JUCE_USE_CURL=0
    JUCE_WEB_BROWSER=0
    JUCE_DISPLAY_SPLASH_SCREEN=0
    LEMONS_UNIT_TESTS=1)

target_link_libraries (${PROJECT_NAME} PRIVATE Lemons::AllLemonsModules)

lemons_add_resources_folder (TARGET ${PROJECT_NAME} ASSET_FOLDER "${CPM_Lemons_SOURCE}/util/assets")

#

enable_testing()

add_test (NAME Lemons.UnitTests 
          COMMAND ${PROJECT_NAME} -f "${CPM_Lemons_SOURCE}/logs/unit_tests_$<CONFIG>.log" -i High --rerun-failed
          COMMAND_EXPAND_LISTS)

set_tests_properties (Lemons.UnitTests PROPERTIES 
                        ATTACHED_FILES "${CMAKE_CURRENT_BINARY_DIR}/Testing/Temporary/LastTest.log;${CPM_Lemons_SOURCE}/logs/unit_tests_$<CONFIG>.log"
                        LABELS "Lemons;UnitTests")
