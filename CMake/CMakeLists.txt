list (APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})

include (internal/resourcesHelpers.cmake)
include (internal/directoryHelpers.cmake)
include (internal/configureHelpers.cmake)

if (APPLE)
	include (internal/native/Apple.cmake)
	setDefaultAppleSettings()
elseif (WIN32)
	include (internal/native/Windows.cmake)
	setDefaultWindowsSettings()
endif()

include (publicAPI.cmake)