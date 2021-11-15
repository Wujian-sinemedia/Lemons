include_guard (GLOBAL)

find_program (SUDO sudo)

#

execute_process (COMMAND "${SUDO}" apt-get update
			     COMMAND_ECHO STDOUT)

execute_process (COMMAND "${SUDO}" apt-get upgrade
				 COMMAND_ECHO STDOUT)

#

function (os_install_func deps)

	execute_process (COMMAND "${SUDO}" apt-get install -y --no-install-recommends ${deps}
		             COMMAND_ECHO STDOUT
		             COMMAND_ERROR_IS_FATAL ANY)
	
endfunction()
