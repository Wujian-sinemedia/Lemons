#[[
# LemonsAppModules	{#LemonsAppModules}

Includes app-related Lemons JUCE modules and CMake utilities.

## Includes:
- [LemonsCommonModules](@ref LemonsCommonModules)
- [LemonsAppUtilities](@ref LemonsAppUtilities)

## Targets:
- LemonsAppModules

]]


include_guard (GLOBAL)

include (LemonsCommonModules)
include (LemonsAppUtilities)


_lemons_add_module_subcategory (LemonsAppModules)

target_link_libraries (LemonsAppModules INTERFACE LemonsCommonModules)
