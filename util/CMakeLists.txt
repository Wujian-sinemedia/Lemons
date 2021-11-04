set_property (GLOBAL PROPERTY USE_FOLDERS YES)
set_property (GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "Targets")

set (CMAKE_SUPPRESS_REGENERATION   TRUE CACHE INTERNAL "")
set (CMAKE_EXPORT_COMPILE_COMMANDS TRUE CACHE INTERNAL "")
set (CMAKE_CXX_STANDARD            ${LEMONS_CXX_VERSION} CACHE INTERNAL "")
set (CMAKE_CXX_STANDARD_REQUIRED   ON CACHE INTERNAL "")
set (CMAKE_INCLUDE_CURRENT_DIR     ON CACHE INTERNAL "")

set (CPM_Lemons_SOURCE "${PROJECT_SOURCE_DIR}" CACHE INTERNAL "")

#

add_subdirectory (cmake/native)

include (LemonsCcacheIntegration)
include (LemonsClangFormatIntegration)

#

if (LEMONS_BUILD_DOCS)
	add_subdirectory (doxygen)
endif()

if (LEMONS_BUILD_TESTS)
    add_subdirectory (testing)
endif()
