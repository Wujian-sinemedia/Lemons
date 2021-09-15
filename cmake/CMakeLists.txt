option (BV_WORKSPACE_BUILD "" OFF)

set_property (GLOBAL PROPERTY USE_FOLDERS YES)
set_property (GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "Targets")

set (CMAKE_SUPPRESS_REGENERATION        TRUE)
set (CMAKE_EXPORT_COMPILE_COMMANDS      TRUE)
set (CMAKE_CXX_STANDARD                 ${BV_CXX_VERSION})
set (CMAKE_CXX_STANDARD_REQUIRED        ON)
set (CMAKE_INCLUDE_CURRENT_DIR          ON)

if (DEFINED Shared-code_SOURCE_DIR)
    set (Shared-code_SOURCE_DIR ${Shared-code_SOURCE_DIR} CACHE INTERNAL "Path to the Shared-code repo" FORCE)
else()
    set (Shared-code_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/.. CACHE INTERNAL "Path to the Shared-code repo")
endif()


include (env.cmake)

add_subdirectory (native)

add_subdirectory (ccache)

include (UsefulScripts/get_cpm.cmake)
add_subdirectory (third_party)

include (FileUtilities.cmake)

include (assets.cmake)
include (configure_helpers.cmake)

add_subdirectory (plugins)
include (apps.cmake)

add_subdirectory (cpack)
include (deploy.cmake)

add_subdirectory (tooling)

#

set (BV_DEFAULT_BRAND_FLAGS
        BUNDLE_ID                   com.BenViningMusicSoftware.${PROJECT_NAME}
        COMPANY_NAME                BenViningMusicSoftware
        COMPANY_WEBSITE             www.benvining.com
        COMPANY_EMAIL               ben.the.vining@gmail.com
        COMPANY_COPYRIGHT           "Copyright 2021 Ben Vining"
        PLUGIN_MANUFACTURER_CODE    Benv
        CACHE INTERNAL "BenViningMusicSoftware brand flags")
