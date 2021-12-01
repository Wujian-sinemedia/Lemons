#[[
Common building blocks and utilities

## Includes:
- LemonsConnectivityModules
- LemonsGUIModules

## Targets:
- LemonsCommonModules

## Options:
- LEMONS_BUILD_TESTS, defaults to OFF

]]


include_guard (GLOBAL)

#

option (LEMONS_BUILD_TESTS "Build tests" OFF)

include (LemonsJuceModuleUtilities)

set (target LemonsCommonModules)

if (NOT TARGET ${target})
    add_library (${target} INTERFACE)
endif()

juce_add_module ("${CMAKE_CURRENT_LIST_DIR}/lemons_core")
target_link_libraries (${target} INTERFACE lemons_core)

if (LEMONS_BUILD_TESTS)
    juce_add_module ("${CMAKE_CURRENT_LIST_DIR}/lemons_unit_testing")
    target_link_libraries (${target} INTERFACE lemons_unit_testing)

    target_link_libraries (lemons_unit_testing INTERFACE
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

    target_compile_definitions (lemons_core INTERFACE LEMONS_UNIT_TESTS=1)
    target_compile_definitions (lemons_unit_testing INTERFACE LEMONS_UNIT_TESTS=1)
endif()
