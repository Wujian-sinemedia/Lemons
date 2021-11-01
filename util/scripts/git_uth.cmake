if (NOT DIR)
	return()
endif()

find_package (Git)

if (NOT Git_FOUND)
	return()
endif()


set (git_branch_name main)

set (submodule_update_command submodule update --init --recursive --merge)

execute_process (COMMAND "${GIT_EXECUTABLE}" fetch
				 COMMAND "${GIT_EXECUTABLE}" pull
				 COMMAND "${GIT_EXECUTABLE}" ${submodule_update_command}
				 COMMAND "${GIT_EXECUTABLE}" submodule foreach --recursive 'git checkout ${git_branch_name} && git fetch && git pull && git ${submodule_update_command}'
				 COMMAND_ECHO STDOUT
				 WORKING_DIRECTORY "${DIR}"
				 OUTPUT_QUIET)
