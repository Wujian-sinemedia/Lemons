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

_lemons_add_module_subcategory (LemonsCommonModules)

if (LEMONS_BUILD_TESTS)
    target_compile_definitions (lemons_core INTERFACE LEMONS_UNIT_TESTS=1)
endif()
