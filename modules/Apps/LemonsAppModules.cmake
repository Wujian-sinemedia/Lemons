#[[
Building blocks for JUCE applications

## Includes:
- LemonsCommonModules
- LemonsAppUtilities

## Targets:
- LemonsAppModules

]]


include_guard (GLOBAL)

include (LemonsCommonModules)
include (LemonsGUIModules)
include (LemonsAppUtilities)


_lemons_add_module_subcategory (LemonsAppModules)
