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

include (LemonsConnectivityModules)

_lemons_add_module_subcategory (LemonsCommonModules)

#

option (LEMONS_BUILD_TESTS "Build tests" OFF)

if (LEMONS_BUILD_TESTS)
    target_compile_definitions (lemons_core INTERFACE LEMONS_UNIT_TESTS=1)
endif()
