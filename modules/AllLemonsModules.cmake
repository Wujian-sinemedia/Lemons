include_guard (GLOBAL)

include (LemonsAppModules)
include (LemonsPluginModules)

add_library (LemonsModules INTERFACE)
target_link_libraries (LemonsModules INTERFACE LemonsPluginModules LemonsAppModules)
