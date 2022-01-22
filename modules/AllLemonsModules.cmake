#[[
Includes all available Lemons JUCE modules in your build, and provides a target to allow linking to all of them at once.

## Targets:
- AllLemonsModules

]]

include_guard (GLOBAL)

include (LemonsAppModules)
include (LemonsPluginModules)
include (LemonsConnectivityModules)


add_library (Lemons::AllLemonsModules ALIAS AllLemonsModules)
