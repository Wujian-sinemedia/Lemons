include_guard (GLOBAL)

find_program (CHOCO choco)

if (NOT CHOCO)
	# install Chocolatey...
endif()

#

function (os_install_func deps)

	execute_process (COMMAND "${CHOCO}" install ${deps}
		             COMMAND_ECHO STDOUT)

endfunction()
