set_property (GLOBAL PROPERTY USE_FOLDERS YES)

set (CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set (CMAKE_CXX_STANDARD            ${BV_CXX_VERSION})
set (CMAKE_CXX_STANDARD_REQUIRED   ON)

if (DEFINED Shared-code_SOURCE_DIR)
    set (Shared-code_SOURCE_DIR ${Shared-code_SOURCE_DIR} CACHE INTERNAL "Path to the Shared-code repo")
else()
    set (Shared-code_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/.. CACHE INTERNAL "Path to the Shared-code repo")
endif()

if (NOT DEFINED ENV{CPM_SOURCE_CACHE})
    set (ENV{CPM_SOURCE_CACHE} ${CMAKE_CURRENT_LIST_DIR}/../Cache)
endif()

add_subdirectory (native)

include (UsefulScripts/get_cpm.cmake)
add_subdirectory (third_party)

add_subdirectory (plugins)

include (FileUtilities.cmake)
include (assets.cmake)
include (configure_helpers.cmake)
include (install.cmake)

include (BrandSpecific.cmake)
