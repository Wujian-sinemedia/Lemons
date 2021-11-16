#[[

## Include-time actions:
Sets a default CPACK_GENERATOR for your system, unless one has already been explicitly specified. 

Also configures some default settings for the selected generator.

## Includes:
- LinuxLSBInfo

]]

include_guard (GLOBAL)

if (NOT APPLE AND NOT WIN32)
	include (LinuxLSBInfo)
endif()

if (NOT CPACK_GENERATOR)
	include ("${CMAKE_CURRENT_LIST_DIR}/scripts/set_default_generator.cmake")
endif()

message (STATUS "Using CPack generator: ${CPACK_GENERATOR}")

if (APPLE OR WIN32)
	if (NOT CPACK_RESOURCE_FILE_LICENSE)
    	set (CPACK_RESOURCE_FILE_LICENSE ${PROJECT_SOURCE_DIR}/LICENSE.txt CACHE INTERNAL "")
	endif()

	if (NOT EXISTS ${CPACK_RESOURCE_FILE_LICENSE})
    	message (AUTHOR_WARNING "${CPACK_RESOURCE_FILE_LICENSE} file not found, provide one or set CPACK_RESOURCE_FILE_LICENSE to point to an existing one.")
	endif()
else()
	include ("${CMAKE_CURRENT_LIST_DIR}/scripts/linux_generator_settings.cmake")
endif()
