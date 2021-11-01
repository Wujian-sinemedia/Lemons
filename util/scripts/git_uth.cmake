if (NOT DIR)
	message (FATAL_ERROR "DIR must be specified!")
	return()
endif()

if (NOT IS_DIRECTORY "${DIR}")
	message (FATAL_ERROR "Requested directory ${DIR} does not exist!")
	return()
endif()

#

find_package (Git)

if (NOT Git_FOUND)
	message (FATAL_ERROR "Git could not be found!")
	return()
endif()

#

set (BRANCH_NAME "main" CACHE STRING "The git branch name to use")

#

cmake_host_system_information (RESULT numCores QUERY NUMBER_OF_LOGICAL_CORES)

set (fetch_command "fetch --all --recurse-submodules --jobs=${numCores}")

set (pull_command "pull --all --recurse-submodules --jobs=${numCores}")

set (submodule_update_command "submodule update --init --recursive")

#

separate_arguments (fetch_command_line UNIX_COMMAND "${fetch_command}")

separate_arguments (pull_command_line UNIX_COMMAND "${pull_command}")

separate_arguments (submodule_update_command_line UNIX_COMMAND "${submodule_update_command}")

separate_arguments (submodule_foreach_command UNIX_COMMAND "submodule foreach --recursive 'git checkout ${BRANCH_NAME} && git ${fetch_command} && git ${pull_command} && git ${submodule_update_command}'")

execute_process (COMMAND "${GIT_EXECUTABLE}" ${fetch_command_line}
				 COMMAND "${GIT_EXECUTABLE}" ${pull_command_line}
				 COMMAND "${GIT_EXECUTABLE}" ${submodule_update_command_line}
				 COMMAND "${GIT_EXECUTABLE}" ${submodule_foreach_command}
				 COMMAND_ECHO STDOUT
				 WORKING_DIRECTORY "${DIR}"
				 OUTPUT_QUIET)
