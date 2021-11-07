#[[
Includes app-related Lemons JUCE modules and CMake utilities.

## Includes:
- LemonsCommonModules
- LemonsAppUtilities

## Targets:
- LemonsAppModules

]]


include_guard (GLOBAL)

include (LemonsCommonModules)
include (LemonsAppUtilities)


_lemons_add_module_subcategory (LemonsAppModules)

target_link_libraries (LemonsAppModules INTERFACE LemonsCommonModules)
