include_guard (GLOBAL)

include (LemonsJuceUtilities)


_lemons_add_module_subcategory (Common       LemonsCommonModules)
_lemons_add_module_subcategory (Connectivity LemonsCommonModules)
_lemons_add_module_subcategory (GUI          LemonsCommonModules)

#

CPMAddPackage (
        NAME MTS-ESP
        GITHUB_REPOSITORY ODDSound/MTS-ESP
        GIT_TAG origin/main)

add_library (MTS-ESP INTERFACE)

target_include_directories (MTS-ESP INTERFACE "${MTS-ESP_SOURCE_DIR}/Client")

target_link_libraries (mts_esp_cpp INTERFACE MTS-ESP)

#

option (LEMONS_BUILD_TESTS "Build tests" OFF)

if (LEMONS_BUILD_TESTS)
    target_compile_definitions (lemons_core INTERFACE LEMONS_UNIT_TESTS=1)
endif()
