#[[
# LemonsCommonModules

Includes the common Lemons JUCE modules.

## Includes:
- [LemonsConnectivityModules](LemonsConnectivityModules.md)
- [LemonsGUIModules](LemonsGUIModules.md)

## Targets:
- LemonsCommonModules

## Options:
- LEMONS_BUILD_TESTS, defaults to OFF

]]


include_guard (GLOBAL)

include (LemonsConnectivityModules)
include (LemonsGUIModules)

_lemons_add_module_subcategory (LemonsCommonModules)

target_link_libraries (LemonsCommonModules INTERFACE LemonsConnectivityModules LemonsGUIModules)

#

option (LEMONS_BUILD_TESTS "Build tests" OFF)

if (LEMONS_BUILD_TESTS)
    target_compile_definitions (lemons_core INTERFACE LEMONS_UNIT_TESTS=1)
endif()
