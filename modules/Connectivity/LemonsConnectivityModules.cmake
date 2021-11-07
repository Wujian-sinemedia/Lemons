include_guard (GLOBAL)

include (LemonsJuceUtilities)
include (LemonsJuceModuleUtilities)

_lemons_add_module_subcategory (LemonsConnectivityModules)

CPMAddPackage (
        NAME MTS-ESP
        GITHUB_REPOSITORY ODDSound/MTS-ESP
        GIT_TAG origin/main)

add_library (MTS-ESP INTERFACE)

target_include_directories (MTS-ESP INTERFACE "${MTS-ESP_SOURCE_DIR}/Client")

target_link_libraries (mts_esp_cpp INTERFACE MTS-ESP)
