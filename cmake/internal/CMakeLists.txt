add_subdirectory (third_party)

include (concepts.cmake)

include (configure_helpers.cmake)
include (plugin_format_helpers.cmake)

if (APPLE)
	include (native/Apple.cmake)
elseif (WIN32)
	include (native/Windows.cmake)
endif()

set (BV_BRAND_FILE "${CMAKE_CURRENT_LIST_DIR}/BrandSpecific.cmake" CACHE INTERNAL "Path to the BenViningMusicSoftware brand-specific config file.")
