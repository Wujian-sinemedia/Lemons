include (UsefulScripts/get_cpm.cmake)

add_subdirectory (third_party)
add_subdirectory (plugins)

set (CMAKE_CXX_STANDARD ${BV_CXX_VERSION})
set (CMAKE_CXX_STANDARD_REQUIRED ON)

if (APPLE)
	include (native/Apple.cmake)
elseif (WIN32)
	include (native/Windows.cmake)
endif()

include (configure_helpers.cmake)