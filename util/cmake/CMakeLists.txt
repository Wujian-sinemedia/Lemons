option (LEMONS_WORKSPACE_BUILD "" OFF)

set_property (GLOBAL PROPERTY USE_FOLDERS YES)
set_property (GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "Targets")

set (CMAKE_SUPPRESS_REGENERATION        TRUE)
set (CMAKE_EXPORT_COMPILE_COMMANDS      TRUE)
set (CMAKE_CXX_STANDARD                 ${LEMONS_CXX_VERSION})
set (CMAKE_CXX_STANDARD_REQUIRED        ON)
set (CMAKE_INCLUDE_CURRENT_DIR          ON)

set (LEMONS_CMAKE_ROOT "${CMAKE_CURRENT_LIST_DIR}" CACHE INTERNAL "" FORCE)

# The top level directory of the current build
if (NOT LEMONS_TOPLEVEL_DIR)
    set (LEMONS_TOPLEVEL_DIR "${LEMONS_REPO_ROOT}/.." CACHE INTERNAL "" FORCE)
endif()

# set up CPM source cache in a git-ignored location (makes successive build configs MUCH faster!)
# if you wish to refresh the cache, I suggest manually deleting the cached files from the Cache dir. They will be freshly re-downloaded when the next CMake configuration is run.
if (NOT ENV{CPM_SOURCE_CACHE})
    set (ENV{CPM_SOURCE_CACHE} "${LEMONS_TOPLEVEL_DIR}/Cache")
endif()

#

include (FileUtilities.cmake)

add_subdirectory (native)
add_subdirectory (third_party)

include (plugin_formats.cmake)

add_subdirectory (config)
add_subdirectory (integrations)

#

set (BV_DEFAULT_BRAND_FLAGS
        BUNDLE_ID                   com.BenViningMusicSoftware.${PROJECT_NAME}
        COMPANY_NAME                Ben Vining Music Software
        COMPANY_WEBSITE             www.benvining.com
        COMPANY_EMAIL               ben.the.vining@gmail.com
        COMPANY_COPYRIGHT           "Copyright 2021 Ben Vining"
        PLUGIN_MANUFACTURER_CODE    Benv
        CACHE INTERNAL "Ben Vining Music Software brand flags")
