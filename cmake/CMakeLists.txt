include (ConfigVars.cmake)

set_property (GLOBAL PROPERTY USE_FOLDERS YES)

set (CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set (CMAKE_CXX_STANDARD            ${BV_CXX_VERSION})
set (CMAKE_CXX_STANDARD_REQUIRED   ON)

if (NOT DEFINED ENV{CPM_SOURCE_CACHE})
    set (ENV{CPM_SOURCE_CACHE} ${CMAKE_CURRENT_LIST_DIR}/../Cache)
endif()

if (DEFINED Shared-code_SOURCE_DIR)
    set (Shared-code_SOURCE_DIR ${Shared-code_SOURCE_DIR} CACHE INTERNAL "Path to the Shared-code repo")
else()
    set (Shared-code_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/.. CACHE INTERNAL "Path to the Shared-code repo")
endif()

if (DEFINED IgnoreVDSP)
    set (BV_IGNORE_VDSP ${IgnoreVDSP} CACHE INTERNAL "Ignore Apple vDSP for this build?")
else()
    set (BV_IGNORE_VDSP FALSE CACHE INTERNAL "Ignore Apple vDSP for this build?")
endif()

add_subdirectory (native)

include (UsefulScripts/get_cpm.cmake)
add_subdirectory (third_party)

add_subdirectory (plugins)

include (FileUtilities.cmake)
include (assets.cmake)
include (configure_helpers.cmake)

include (public_api.cmake)

set (BV_BRAND_FILE "${CMAKE_CURRENT_LIST_DIR}/BrandSpecific.cmake" CACHE INTERNAL "Path to the BenViningMusicSoftware brand-specific config file.")
