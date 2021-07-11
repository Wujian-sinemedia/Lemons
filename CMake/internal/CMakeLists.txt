include (resourcesHelpers.cmake)
include (configureHelpers.cmake)
include (directoryHelpers.cmake)

if (APPLE)
	include (native/Apple.cmake)
elseif (WIN32)
	include (native/Windows.cmake)
endif()