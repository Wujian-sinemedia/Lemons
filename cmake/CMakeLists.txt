option (BV_WORKSPACE_BUILD "" OFF)

set_property (GLOBAL PROPERTY USE_FOLDERS YES)
set_property (GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "Targets")

set (CMAKE_SUPPRESS_REGENERATION        TRUE)
set (CMAKE_EXPORT_COMPILE_COMMANDS      TRUE)
set (CMAKE_CXX_STANDARD                 ${BV_CXX_VERSION})
set (CMAKE_CXX_STANDARD_REQUIRED        ON)
set (CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set (CMAKE_INCLUDE_CURRENT_DIR          ON)
set (CMAKE_LINK_DEPENDS_NO_SHARED       ON)

if (DEFINED Shared-code_SOURCE_DIR)
    set (Shared-code_SOURCE_DIR ${Shared-code_SOURCE_DIR} CACHE INTERNAL "Path to the Shared-code repo" FORCE)
else()
    set (Shared-code_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/.. CACHE INTERNAL "Path to the Shared-code repo")
endif()

if (NOT DEFINED ENV{CPM_SOURCE_CACHE})
    set (ENV{CPM_SOURCE_CACHE} ${CMAKE_CURRENT_LIST_DIR}/../Cache)
endif()

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

include (BrandSpecific.cmake)
