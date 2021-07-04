list (APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})

include (resourcesHelpers.cmake)
include (configureHelpers.cmake)
include (directoryHelpers.cmake)

if (APPLE)
	include (native/Apple.cmake)
	setDefaultAppleSettings()
elseif (WIN32)
	include (native/Windows.cmake)
	setDefaultWindowsSettings()
endif()