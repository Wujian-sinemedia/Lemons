#[[
Common building blocks and utilities

## Includes:
- LemonsJuceModuleUtilities

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
    target_compile_definitions (lemons_core INTERFACE LEMONS_UNIT_TESTS=1)
endif()
