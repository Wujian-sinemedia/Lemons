#[[
Includes Lemons JUCE modules for building GUIs.

## Includes:
- LemonsJuceModuleUtilities

## Targets:
- LemonsGUIModules

]]


include_guard (GLOBAL)

include (LemonsCommonModules)


_lemons_add_module_subcategory (LemonsGUIModules)
target_link_libraries (LemonsGUIModules INTERFACE LemonsCommonModules)
