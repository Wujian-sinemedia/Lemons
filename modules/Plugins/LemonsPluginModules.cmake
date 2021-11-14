#[[
Building blocks for audio plugins

## Includes:
- LemonsCommonModules
- LemonsAudioModules
- LemonsPluginUtilities

## Targets:
- LemonsPluginModules

]]


include_guard (GLOBAL)

include (LemonsCommonModules)
include (LemonsGUIModules)
include (LemonsAudioModules)
include (LemonsPluginUtilities)


_lemons_add_module_subcategory (LemonsPluginModules)
target_link_libraries (LemonsPluginModules INTERFACE LemonsAudioModules LemonsCommonModules LemonsGUIModules)
