#!/usr/bin/env cmake -P

#[[

inputs:
- LEMONS_DIR
- LEMONS_CONFIGS (can be read from env var)

]]


file (REAL_PATH "${CMAKE_CURRENT_LIST_DIR}/.." lemons_root EXPAND_TILDE)


set (run_dir ${lemons_root}/${LEMONS_DIR})

if (NOT IS_DIRECTORY ${run_dir})
	message (FATAL_ERROR "Directory '${run_dir}' deduced from argument '${LEMONS_DIR}' does not exist!")
endif()

message (STATUS "Run directory: ${run_dir}")


set (logsDir ${lemons_root}/logs)

if (NOT IS_DIRECTORY ${logsDir})
	file (MAKE_DIRECTORY ${logsDir})
endif()


set (thisRunLogsDir ${logsDir}/${LEMONS_DIR})

if (NOT IS_DIRECTORY ${thisRunLogsDir})
	file (MAKE_DIRECTORY ${thisRunLogsDir})
endif()

message (DEBUG "Log directory for this run: ${thisRunLogsDir}")


#


if (APPLE)
	set (lemons_cmake_generator Xcode)
elseif (WIN32)
	set (lemons_cmake_generator "Visual Studio 16 2019")
else()
	set (lemons_cmake_generator Ninja)
endif()


cmake_host_system_information (RESULT num_of_cores QUERY NUMBER_OF_LOGICAL_CORES)

string (COMPARE LESS ${num_of_cores} "2" lessThanTwo)

if (lessThanTwo)
	set (num_of_cores 2)
endif()


#

if (DEFINED ENV{LEMONS_CONFIGS})
	set (LEMONS_CONFIGS $ENV{LEMONS_CONFIGS})
endif()

if (NOT LEMONS_CONFIGS)
	set (LEMONS_CONFIGS Debug Release)
endif()

list (JOIN LEMONS_CONFIGS " " configList)

message (STATUS "Configuration list: ${configList}")


foreach (configuration ${LEMONS_CONFIGS})

	message (STATUS "Running configuration: ${configuration}...")

	set (thisConfigsLogsDir ${thisRunLogsDir}/${configuration})

	if (NOT IS_DIRECTORY ${thisConfigsLogsDir})
		file (MAKE_DIRECTORY ${thisConfigsLogsDir})
	endif()

	message (DEBUG "Log directory for configuration - ${configuration}: ${thisConfigsLogsDir}")

	#

	function (_lemons_run_cmake_config_in_dir stepName)

		set (logFile ${thisConfigsLogsDir}/${stepName}.log)
		set (errorFile ${thisConfigsLogsDir}/${stepName}_Errors.log)

		file (TOUCH ${logFile})
		file (TOUCH ${errorFile})

		message (DEBUG "Step name: ${stepName}")
		message (TRACE "Configuration: ${configuration}")
		message (DEBUG "Command: ${ARGN}")

		execute_process (COMMAND "${CMAKE_COMMAND}" ${ARGN}
						 WORKING_DIRECTORY "${run_dir}"
						 OUTPUT_FILE "${logFile}"
						 ERROR_FILE "${errorFile}"
						 OUTPUT_VARIABLE std_out OUTPUT_STRIP_TRAILING_WHITESPACE ECHO_OUTPUT_VARIABLE
						 ERROR_VARIABLE std_err ERROR_STRIP_TRAILING_WHITESPACE ECHO_ERROR_VARIABLE
						 RESULT_VARIABLE res_var
						 COMMAND_ECHO STDOUT
						 COMMAND_ERROR_IS_FATAL ANY)

		if (std_out)
			message (STATUS "${stepName} output - ${configuration}:")
			message (STATUS ${std_out})
		else()
			if (EXISTS ${logFile})
				file (READ "${logFile}" logFileContents)

				if (logFileContents)
					message (STATUS "${stepName} output - ${configuration}:")
					message (STATUS ${logFileContents})
				endif()
			endif()
		endif()

		if (std_err)
			message (WARNING "Errors in ${stepName} - ${configuration}!")
			message (STATUS ${std_err})
		else()
			if (EXISTS ${errorFile})
				file (READ "${errorFile}" errorFileContents)

				if (errorFileContents)
					message (WARNING "Errors in ${stepName} - ${configuration}!")
					message (STATUS ${errorFileContents})
				endif()
			endif()
		endif()

		if (res_var)
			message (FATAL_ERROR "${stepName} - ${configuration} failed with exit code ${res_var}")
		else()
			message (STATUS "${stepName} - ${configuration} succeeded with exit code ${res_var}")
		endif()

	endfunction()

	#

	set (thisConfigsBuildsDir Builds/${configuration})
	set (thisConfigBuildDirAbsPath "${run_dir}/${thisConfigsBuildsDir}")

	message (DEBUG "Build dir for configuration - ${configuration}: ${thisConfigBuildDirAbsPath}")

	if (EXISTS "${thisConfigBuildDirAbsPath}")
		message (DEBUG "Removing old build directory...")
		file (REMOVE_RECURSE "${thisConfigBuildDirAbsPath}")
	endif()

	_lemons_run_cmake_config_in_dir (Configuration -B "${thisConfigsBuildsDir}" -G "${lemons_cmake_generator}" --log-level=DEBUG)

	_lemons_run_cmake_config_in_dir (Build --build "${thisConfigsBuildsDir}" -j "${num_of_cores}" --config "${configuration}")
	
endforeach()

