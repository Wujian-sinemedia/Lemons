if (LEMONS_BUILD_DOCS)
	add_subdirectory (doxygen)
endif()

if (LEMONS_BUILD_TESTS)
	enable_testing()
    add_subdirectory (UnitTestRunner)
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
