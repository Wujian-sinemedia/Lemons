#!/usr/bin/env cmake -P


file (REAL_PATH "${CMAKE_CURRENT_LIST_DIR}/.." lemons_root EXPAND_TILDE)


set (run_dir ${lemons_root}/${LEMONS_DIR})

if (NOT IS_DIRECTORY ${run_dir})
	message (FATAL_ERROR "Directory '${run_dir}' deduced from argument '${LEMONS_DIR}' does not exist!")
endif()


set (logsDir ${lemons_root}/logs)

if (NOT IS_DIRECTORY ${logsDir})
	file (MAKE_DIRECTORY ${logsDir})
endif()

set (thisRunLogsDir ${logsDir}/${LEMONS_DIR})

if (NOT IS_DIRECTORY ${thisRunLogsDir})
	file (MAKE_DIRECTORY ${thisRunLogsDir})
endif()

#

if (APPLE)
	set (lemons_cmake_generator Xcode)
elseif (WIN32)
	set (lemons_cmake_generator "Visual Studio 16 2019")
else()
	set (lemons_cmake_generator Ninja)
endif()

cmake_host_system_information (RESULT num_of_cores QUERY NUMBER_OF_LOGICAL_CORES)


foreach (configuration Debug Release)

	message (STATUS "Running configuration: ${configuration}...")

	set (thisConfigsLogsDir ${thisRunLogsDir}/${configuration})

	if (NOT IS_DIRECTORY ${thisConfigsLogsDir})
		file (MAKE_DIRECTORY ${thisConfigsLogsDir})
	endif()

	set (configLog ${thisConfigsLogsDir}/config.log)
	set (configErrorLog ${thisConfigsLogsDir}/config_Errors.log)

	file (TOUCH ${configLog})
	file (TOUCH ${configErrorLog})

	set (thisConfigsBuildsDir Builds/${configuration})

	set (configureCommand "-B ${thisConfigsBuildsDir} -G '${lemons_cmake_generator}'")

	if (LEMONS_PRIVATE_SDKS)
		message (DEBUG "Enabling PrivateSDKs...")
		set (configureCommand "-D CPM_PrivateSDKs_SOURCE=${LEMONS_PRIVATE_SDKS} ${configureCommand}")
	endif()

	separate_arguments (configureCommand UNIX_COMMAND "${configureCommand}")

	execute_process (COMMAND "${CMAKE_COMMAND}" ${configureCommand}
					 WORKING_DIRECTORY "${run_dir}"
					 OUTPUT_FILE "${configLog}"
					 ERROR_FILE "${configErrorLog}"
					 ERROR_VARIABLE configErrors
					 RESULT_VARIABLE res
					 COMMAND_ECHO STDOUT
					 COMMAND_ERROR_IS_FATAL ANY
					 ECHO_OUTPUT_VARIABLE ECHO_ERROR_VARIABLE)

	if (configErrors)
		message (WARNING "Errors in configuration!")
		message (STATUS ${configErrors})
	endif()

	if (res)
		message (FATAL_ERROR "Configuration failed with exit code ${res}")
	else()
		message (STATUS "Configuration succeeded with exit code ${res}")
	endif()

	#

	set (buildLog ${thisConfigsLogsDir}/build.log)
	set (buildErrorLog ${thisConfigsLogsDir}/build_Errors.log)

	file (TOUCH ${buildLog})
	file (TOUCH ${buildErrorLog})

	execute_process (COMMAND "${CMAKE_COMMAND}" --build ${thisConfigsBuildsDir} -j ${num_of_cores} --config ${configuration}
					 WORKING_DIRECTORY "${run_dir}"
					 OUTPUT_FILE "${buildLog}"
					 ERROR_FILE "${buildErrorLog}"
					 ERROR_VARIABLE buildErrors
					 RESULT_VARIABLE errno
					 COMMAND_ECHO STDOUT
					 COMMAND_ERROR_IS_FATAL ANY
					 ECHO_OUTPUT_VARIABLE ECHO_ERROR_VARIABLE)

	if (buildErrors)
		message (WARNING "Errors in build!")
		message (STATUS ${buildErrors})
	endif()

	if (errno)
		message (FATAL_ERROR "Build failed with exit code ${errno}")
	else()
		message (STATUS "Build succeeded with exit code ${errno}")
	endif()
	
endforeach()

