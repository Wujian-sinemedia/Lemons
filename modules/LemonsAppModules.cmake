#[[
Building blocks for JUCE applications

## Targets:
- LemonsAppModules

]]

include_guard (GLOBAL)

include (LemonsGUIModules)
include (LemonsAppUtilities)
include (LemonsJuceModuleUtilities)

add_library (LemonsAppModules ALIAS LemonsGUIModules)
add_library (Lemons::LemonsAppModules ALIAS LemonsGUIModules)
