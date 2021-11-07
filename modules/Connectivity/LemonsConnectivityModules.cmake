#[[
# Module: LemonsConnectivityModules

Includes Lemons JUCE modules for connectivity and networking APIs.

## Includes:
- LemonsJuceModuleUtilities

## Include-time actions:
Adds the MTS-ESP library via CPM.cmake.

## Targets:
- LemonsConnectivityModules

]]


include_guard (GLOBAL)

include (LemonsJuceModuleUtilities)


_lemons_add_module_subcategory (LemonsConnectivityModules)


CPMAddPackage (
        NAME MTS-ESP
        GITHUB_REPOSITORY ODDSound/MTS-ESP
        GIT_TAG origin/main)

target_include_directories (mts_esp_cpp INTERFACE "${MTS-ESP_SOURCE_DIR}/Client")
