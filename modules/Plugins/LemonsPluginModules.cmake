#[[
# LemonsPluginModules	{#LemonsPluginModules}

Includes audio plugin-related JUCE modules and CMake utilities into your build.

## Includes:
- [LemonsCommonModules](@ref LemonsCommonModules)
- [LemonsAudioModules](@ref LemonsAudioModules)
- [LemonsPluginUtilities](@ref LemonsPluginUtilities)

## Targets:
- LemonsPluginModules

]]


include_guard (GLOBAL)

include (LemonsCommonModules)
include (LemonsAudioModules)
include (LemonsPluginUtilities)


_lemons_add_module_subcategory (LemonsPluginModules)

target_link_libraries (LemonsPluginModules INTERFACE LemonsAudioModules LemonsCommonModules)
