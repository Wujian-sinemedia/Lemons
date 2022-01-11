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

set (configLog ${thisRunLogsDir}/config.log)
set (configErrorLog ${thisRunLogsDir}/config_Errors.log)

file (TOUCH ${configLog})
file (TOUCH ${configErrorLog})

if (APPLE)
	set (lemons_cmake_generator Xcode)
elseif (WIN32)
	set (lemons_cmake_generator "\"Visual Studio 16 2019\"")
else()
	set (lemons_cmake_generator Ninja)
endif()

execute_process (COMMAND ${CMAKE_COMMAND} -B Builds -G ${lemons_cmake_generator}
				 WORKING_DIRECTORY ${run_dir}
				 OUTPUT_FILE ${configLog}
				 ERROR_FILE ${configErrorLog}
				 COMMAND_ECHO STDOUT
				 COMMAND_ERROR_IS_FATAL ANY
				 ECHO_OUTPUT_VARIABLE ECHO_ERROR_VARIABLE)

#

set (buildLog ${thisRunLogsDir}/build.log)
set (buildErrorLog ${thisRunLogsDir}/build_Errors.log)

file (TOUCH ${buildLog})
file (TOUCH ${buildErrorLog})

cmake_host_system_information (RESULT num_of_cores QUERY NUMBER_OF_LOGICAL_CORES)

execute_process (COMMAND ${CMAKE_COMMAND} --build Builds -j ${num_of_cores}
				 WORKING_DIRECTORY ${run_dir}
				 OUTPUT_FILE ${buildLog}
				 ERROR_FILE ${buildErrorLog}
				 COMMAND_ECHO STDOUT
				 COMMAND_ERROR_IS_FATAL ANY
				 ECHO_OUTPUT_VARIABLE ECHO_ERROR_VARIABLE)
