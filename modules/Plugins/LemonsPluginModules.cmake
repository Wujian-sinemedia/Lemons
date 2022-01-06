#[[
Building blocks for audio plugins

## Targets:
- LemonsPluginModules

]]


include_guard (GLOBAL)

include (LemonsPluginUtilities)
include (LemonsJuceModuleUtilities)

_lemons_add_module_subcategory (TARGET LemonsPluginModules CATEGORY_DEPS LemonsCommonModules LemonsAudioModules)
