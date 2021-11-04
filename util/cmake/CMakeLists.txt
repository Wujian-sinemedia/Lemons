set_property (GLOBAL PROPERTY USE_FOLDERS YES)
set_property (GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "Targets")

set (CMAKE_SUPPRESS_REGENERATION   TRUE CACHE INTERNAL "")
set (CMAKE_EXPORT_COMPILE_COMMANDS TRUE CACHE INTERNAL "")
set (CMAKE_CXX_STANDARD            ${LEMONS_CXX_VERSION} CACHE INTERNAL "")
set (CMAKE_CXX_STANDARD_REQUIRED   ON CACHE INTERNAL "")
set (CMAKE_INCLUDE_CURRENT_DIR     ON CACHE INTERNAL "")

# set up CPM source cache in a git-ignored location (makes successive build configs MUCH faster!)
# if you wish to refresh the cache, I suggest manually deleting the cached files from the Cache dir. They will be freshly re-downloaded when the next CMake configuration is run.
set (ENV{CPM_SOURCE_CACHE} "${CMAKE_SOURCE_DIR}/Cache")

set (CPM_Lemons_SOURCE "${PROJECT_SOURCE_DIR}" CACHE INTERNAL "")

#

include (FileUtilities.cmake)

add_subdirectory (native)
add_subdirectory (integrations)
add_subdirectory (third_party)
add_subdirectory (config)

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
