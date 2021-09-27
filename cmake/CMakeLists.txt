option (LEMONS_WORKSPACE_BUILD "" OFF)

set_property (GLOBAL PROPERTY USE_FOLDERS YES)
set_property (GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "Targets")

set (CMAKE_SUPPRESS_REGENERATION        TRUE)
set (CMAKE_EXPORT_COMPILE_COMMANDS      TRUE)
set (CMAKE_CXX_STANDARD                 ${LEMONS_CXX_VERSION})
set (CMAKE_CXX_STANDARD_REQUIRED        ON)
set (CMAKE_INCLUDE_CURRENT_DIR          ON)

if (NOT Lemons_SOURCE_DIR)
    set (Lemons_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/.. CACHE INTERNAL "Path to the Lemons repo")
endif()


include (env.cmake)
include (FileUtilities.cmake)
include (plugin_formats.cmake)

add_subdirectory (native)
add_subdirectory (third_party)
add_subdirectory (config)
add_subdirectory (integrations)
add_subdirectory (cpack)

#

set (BV_DEFAULT_BRAND_FLAGS
        BUNDLE_ID                   com.BenViningMusicSoftware.${PROJECT_NAME}
        COMPANY_NAME                Ben Vining Music Software
        COMPANY_WEBSITE             www.benvining.com
        COMPANY_EMAIL               ben.the.vining@gmail.com
        COMPANY_COPYRIGHT           "Copyright 2021 Ben Vining"
        PLUGIN_MANUFACTURER_CODE    Benv
        CACHE INTERNAL "Ben Vining Music Software brand flags")
