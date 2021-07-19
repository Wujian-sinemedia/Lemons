set (CMAKE_CXX_STANDARD ${BV_CXX_VERSION})
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set_property (GLOBAL PROPERTY USE_FOLDERS YES)

set (CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

if (NOT DEFINED ENV{CPM_SOURCE_CACHE})
    set (ENV{CPM_SOURCE_CACHE} ${CMAKE_CURRENT_LIST_DIR}/../Cache)
endif()

include (public/utilities.cmake)

add_subdirectory (internal)

add_subdirectory (project_repo_config)

include (public/public_api.cmake)
