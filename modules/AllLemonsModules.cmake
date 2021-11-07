#[[
Module: AllLemonsModules

Includes:
- LemonsAppModules
- LemonsPluginModules

Targets:
- AllLemonsModules

]]

include_guard (GLOBAL)

include (LemonsAppModules)
include (LemonsPluginModules)


add_library (AllLemonsModules INTERFACE)
target_link_libraries (AllLemonsModules INTERFACE LemonsPluginModules LemonsAppModules)
