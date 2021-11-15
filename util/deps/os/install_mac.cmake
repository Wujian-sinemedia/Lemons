include_guard (GLOBAL)

find_program (HOMEBREW brew)

if (NOT HOMEBREW)
	# install Homebrew...
endif()

#

function (os_install_func deps)

	execute_process (COMMAND "${HOMEBREW}" install ${deps}
					 COMMAND_ECHO STDOUT)

endfunction()
