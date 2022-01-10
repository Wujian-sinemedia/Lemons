add_subdirectory (cmake/modules)

list (APPEND CMAKE_MODULE_PATH "${LEMONS_CMAKE_MODULE_PATH}")

include (LemonsDefaultPlatformSettings)

option (LEMONS_ENABLE_INTEGRATIONS "Enable all available integrations by default" ON)

if (LEMONS_ENABLE_INTEGRATIONS)
	include (LemonsAllIntegrations)
endif()

# if (LEMONS_BUILD_TEST_LAB)
# 	add_subdirectory (TestLab)
# endif()

if (LEMONS_BUILD_TEMPLATES)
	add_subdirectory (Templates)
endif()

if (LEMONS_BUILD_UTILS)
	add_subdirectory (CommandLineUtils)
endif()

if (LEMONS_BUILD_METADATA_EDITOR)
	add_subdirectory (PluginMetadataEditor)
endif()
