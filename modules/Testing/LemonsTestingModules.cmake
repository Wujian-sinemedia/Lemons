#[[
Utilities for testing

## Includes:
- LemonsPluginModules

## Targets:
- LemonsTestingModules

## Options:
- LEMONS_BUILD_TESTS, defaults to ON

]]

include_guard (GLOBAL)

#

include (LemonsPluginModules)

option (LEMONS_BUILD_TESTS "Build tests" ON)

if (NOT LEMONS_BUILD_TESTS)
	message (WARNING "LemonsTestingModues was included, but the LEMONS_BUILD_TESTS option is off!")
endif()

set (target LemonsTestingModules)

if (NOT TARGET ${target})
    add_library (${target} INTERFACE)
endif()

juce_add_module ("${CMAKE_CURRENT_LIST_DIR}/lemons_unit_testing")
target_link_libraries (${target} INTERFACE lemons_unit_testing)

target_link_libraries (lemons_unit_testing INTERFACE
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

target_compile_definitions (lemons_unit_testing INTERFACE LEMONS_UNIT_TESTS=1)
