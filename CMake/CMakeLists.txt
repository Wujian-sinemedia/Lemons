set (CMAKE_CXX_STANDARD ${BV_CXX_VERSION})

set_property (GLOBAL PROPERTY USE_FOLDERS YES)

set (CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

#

add_subdirectory (third_party)

add_subdirectory (internal)

include (publicAPI.cmake)