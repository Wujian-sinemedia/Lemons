#[[
JUCE modules for audio and DSP

## Includes:
- LemonsJuceModuleUtilities

## Targets:
- LemonsAudioModules

]]


include_guard (GLOBAL)

include (LemonsCommonModules)


_lemons_add_module_subcategory (LemonsAudioModules)
target_link_libraries (LemonsAudioModules INTERFACE LemonsCommonModules)
