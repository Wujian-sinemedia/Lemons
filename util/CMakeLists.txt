add_subdirectory (cmake/modules)

list (APPEND CMAKE_MODULE_PATH "${LEMONS_CMAKE_MODULE_PATH}")

include (LemonsParseProjectConfigFile)

lemons_parse_project_configuration_file (
	FORCE FILE "${Lemons_SOURCE_DIR}/scripts/lemons_config.json")
