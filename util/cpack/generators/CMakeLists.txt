if (NOT CPACK_GENERATOR)
	include (set_default_generator.cmake)
endif()

message (STATUS "Using CPack generator: ${CPACK_GENERATOR}")

if (NOT APPLE AND NOT WIN32)
	add_subdirectory (linux)
else()
	include (gen_settings.cmake)
endif()
