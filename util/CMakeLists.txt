add_subdirectory (cmake/modules)

list (APPEND CMAKE_MODULE_PATH "${LEMONS_CMAKE_MODULE_PATH}")

include (LemonsDefaultPlatformSettings)

option (LEMONS_ENABLE_INTEGRATIONS "Enable all available integrations by default" ON)

if (LEMONS_ENABLE_INTEGRATIONS)
	include (LemonsAllIntegrations)
elseif (LEMONS_SANITIZERS_TO_ENABLE)
	include (LemonsSanitizers)
endif()

include (LemonsParseProjectConfigFile)

lemons_parse_project_configuration_file (FORCE FILE "${Lemons_SOURCE_DIR}/scripts/lemons_config.json")
