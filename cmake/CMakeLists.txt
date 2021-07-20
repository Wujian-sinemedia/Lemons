set_property (GLOBAL PROPERTY USE_FOLDERS YES)

set (CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

if (NOT DEFINED ENV{CPM_SOURCE_CACHE})
    set (ENV{CPM_SOURCE_CACHE} ${CMAKE_CURRENT_LIST_DIR}/../Cache)
endif()

if (NOT DEFINED Shared-code_SOURCE_DIR)
    set (Shared-code_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/.. CACHE INTERNAL "Path to the Shared-code repo")
endif()

add_subdirectory (public)
add_subdirectory (internal)
add_subdirectory (project_repo_config)

set (BV_BRAND_FILE "${CMAKE_CURRENT_LIST_DIR}/BrandSpecific.cmake" CACHE INTERNAL "Path to the BenViningMusicSoftware brand-specific config file.")

