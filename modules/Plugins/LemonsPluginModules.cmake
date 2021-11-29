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