include (LemonsDefaultPlatformSettings)

option (LEMONS_ENABLE_INTEGRATIONS "Enable all available integrations by default" ON)

if (LEMONS_ENABLE_INTEGRATIONS)
	include (LemonsAllIntegrations)
endif()

option (LEMONS_CONFIGURE_CPACK "Configure default CPack settings" OFF)

if (LEMONS_CONFIGURE_CPACK)
	include (LemonsDefaultCpackSettings)
endif()

if (LEMONS_BUILD_DOCS)
	add_subdirectory (doxygen)
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

if (LEMONS_BUILD_TESTS)
	enable_testing()
    add_subdirectory (tests)
endif()
