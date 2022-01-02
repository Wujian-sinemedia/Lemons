#[[
Building blocks for JUCE applications

## Includes:
- LemonsCommonModules
- LemonsAppUtilities

## Targets:
- LemonsAppModules

]]


include_guard (GLOBAL)

include (LemonsGUIModules)
include (LemonsAppUtilities)

add_library (LemonsAppModules INTERFACE)
target_link_libraries (LemonsAppModules INTERFACE LemonsGUIModules)
