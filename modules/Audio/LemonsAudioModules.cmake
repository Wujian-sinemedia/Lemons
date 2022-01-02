#[[
JUCE modules for audio and DSP

## Includes:
- LemonsCommonModules

## Targets:
- LemonsAudioModules

]]


include_guard (GLOBAL)

include (LemonsCommonModules)

_lemons_add_module_subcategory (LemonsAudioModules)

CPMAddPackage (
        NAME MTS-ESP
        GITHUB_REPOSITORY ODDSound/MTS-ESP
        GIT_TAG origin/main)

target_include_directories (lemons_midi INTERFACE "${MTS-ESP_SOURCE_DIR}/Client")

target_link_libraries (LemonsAudioModules INTERFACE LemonsCommonModules)
