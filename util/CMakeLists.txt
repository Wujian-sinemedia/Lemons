add_subdirectory (cmake/native)

if (LEMONS_BUILD_DOCS)
	add_subdirectory (doxygen)
endif()

if (LEMONS_BUILD_TESTS)
    add_subdirectory (testing)
endif()
