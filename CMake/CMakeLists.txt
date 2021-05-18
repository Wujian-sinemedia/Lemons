list (APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/Scripts)

include (Scripts/CMakeUtils.cmake)
include (Scripts/DirectoryHelpers.cmake)

if (APPLE)
	include (native/Apple.cmake)
	setDefaultAppleSettings()
elseif (WIN32)
	include (native/Windows.cmake)
	setDefaultWindowsSettings()
endif()
