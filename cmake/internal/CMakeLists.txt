add_subdirectory (third_party)

include (concepts.cmake)

include (configure_helpers.cmake)
include (plugin_format_helpers.cmake)

if (APPLE)
	include (native/Apple.cmake)
elseif (WIN32)
	include (native/Windows.cmake)
endif()
