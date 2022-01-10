add_subdirectory (cmake/modules)

list (APPEND CMAKE_MODULE_PATH "${LEMONS_CMAKE_MODULE_PATH}")

include (LemonsDefaultPlatformSettings)

option (LEMONS_ENABLE_INTEGRATIONS "Enable all available integrations by default" ON)

if (LEMONS_ENABLE_INTEGRATIONS)
	include (LemonsAllIntegrations)
endif()
