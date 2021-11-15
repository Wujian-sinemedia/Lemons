include_guard (GLOBAL)

find_program (APT "apt-get")

if (NOT APT)
	find_program (APT apt)
endif()

#

execute_process (COMMAND "${APT}" update
			     COMMAND_ECHO STDOUT)

execute_process (COMMAND "${APT}" upgrade
				 COMMAND_ECHO STDOUT)

#

function (os_install_func deps)

	execute_process (COMMAND "${APT}" install -y --no-install-recommends ${deps}
		             COMMAND_ECHO STDOUT)

endfunction()
