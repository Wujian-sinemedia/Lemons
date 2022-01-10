set (logsDir @CMAKE_CURRENT_BINARY_DIR@/logs)

set (config_output_file ${logsDir}/moduleBuild_Config.log)
set (config_error_output ${logsDir}/moduleBuild_ConfigError.log)
set (build_output_file ${logsDir}/moduleBuild_Build.log)
set (build_error_output ${logsDir}/moduleBuild_BuildError.log)

if (NOT IS_DIRECTORY ${logsDir})
	file (MAKE_DIRECTORY ${logsDir})
endif()

file (TOUCH ${config_output_file} ${config_error_output} ${build_output_file} ${build_error_output})


message (STATUS " --- configuring individual module builds... ---")

execute_process (COMMAND @CMAKE_COMMAND@ -B Builds -G @CMAKE_GENERATOR@
				 WORKING_DIRECTORY @CMAKE_CURRENT_LIST_DIR@/CompileModulesIndividually
				 COMMAND_ECHO STDOUT
				 COMMAND_ERROR_IS_FATAL ANY
				 OUTPUT_FILE ${config_output_file}
				 ERROR_FILE ${config_error_output})


message (STATUS " --- building each module individually... ---")

execute_process (COMMAND @CMAKE_COMMAND@ --build Builds
				 WORKING_DIRECTORY @CMAKE_CURRENT_LIST_DIR@/CompileModulesIndividually
				 COMMAND_ECHO STDOUT
				 COMMAND_ERROR_IS_FATAL ANY
				 OUTPUT_FILE ${build_output_file}
				 ERROR_FILE ${build_error_output})
