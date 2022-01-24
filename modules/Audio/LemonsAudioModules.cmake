#[[
JUCE modules for audio and DSP

## Targets:
- LemonsAudioModules

]]

include_guard (GLOBAL)

include (LemonsJuceModuleUtilities)

_lemons_add_module_subcategory (TARGET LemonsAudioModules CATEGORY_DEPS LemonsCommonModules)

cpmaddpackage (NAME MTS-ESP GITHUB_REPOSITORY ODDSound/MTS-ESP GIT_TAG origin/main)

target_include_directories (lemons_midi INTERFACE "${MTS-ESP_SOURCE_DIR}/Client")
