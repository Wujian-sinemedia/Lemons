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

add_subdirectory (third_party)

include (LemonsTranslationFileGeneration)
include (LemonsPluginUtilities)
include (LemonsAppUtilities)
include (LemonsAssetsHelpers)

#

set (BV_DEFAULT_BRAND_FLAGS
        BUNDLE_ID                   com.BenViningMusicSoftware.${PROJECT_NAME}
        COMPANY_NAME                Ben Vining Music Software
        COMPANY_WEBSITE             www.benvining.com
        COMPANY_EMAIL               ben.the.vining@gmail.com
        COMPANY_COPYRIGHT           "Copyright 2021 Ben Vining"
        PLUGIN_MANUFACTURER_CODE    Benv
        CACHE INTERNAL "Ben Vining Music Software brand flags")

#
