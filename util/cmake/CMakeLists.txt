set_property (GLOBAL PROPERTY USE_FOLDERS YES)
set_property (GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "Targets")

set (CMAKE_SUPPRESS_REGENERATION   TRUE CACHE INTERNAL "")
set (CMAKE_EXPORT_COMPILE_COMMANDS TRUE CACHE INTERNAL "")
set (CMAKE_CXX_STANDARD            ${LEMONS_CXX_VERSION} CACHE INTERNAL "")
set (CMAKE_CXX_STANDARD_REQUIRED   ON CACHE INTERNAL "")
set (CMAKE_INCLUDE_CURRENT_DIR     ON CACHE INTERNAL "")

set (CPM_Lemons_SOURCE "${PROJECT_SOURCE_DIR}" CACHE INTERNAL "")

#

include (LemonsFileUtils)

add_subdirectory (native)

include (LemonsClangFormatIntegration)
include (LemonsCcacheIntegration)

include (LemonsJuceUtilities)

CPMAddPackage (
        NAME MTS-ESP
        GITHUB_REPOSITORY ODDSound/MTS-ESP
        GIT_TAG origin/main)

add_library (MTS-ESP INTERFACE)

target_include_directories (MTS-ESP INTERFACE "${MTS-ESP_SOURCE_DIR}/Client")

include (LemonsTranslationFileGeneration)
include (LemonsPluginUtilities)
include (LemonsAppUtilities)
include (LemonsAssetsHelpers)
