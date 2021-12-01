#[[
Helpers for writing unit tests

## Includes:
- LemonsJuceModuleUtilities

## Targets:
- LemonsTestingModules

]]


include_guard (GLOBAL)

include (LemonsJuceModuleUtilities)

_lemons_add_module_subcategory (LemonsTestingModules)

target_link_libraries (LemonsTestingModules INTERFACE
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

