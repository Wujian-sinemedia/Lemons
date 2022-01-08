include_guard (GLOBAL)

find_program (SUDO sudo)

if (NOT SUDO)
	message (FATAL_ERROR "sudo is required on Linux!")
endif()

# use apt-get if available, else apt

find_program (APT_GET apt-get)

if (APT_GET)
	set (apt_program "apt-get" CACHE INTERNAL "")
else()
	find_program (APT apt)

	if (APT)
		set (apt_program "apt" CACHE INTERNAL "")
	else()
		message (FATAL_ERROR "Neither apt or apt-get can be found!")
	endif()
endif()

#

# update installed packages

execute_process (COMMAND "${SUDO}" ${apt_program} update
			     COMMAND_ECHO STDOUT)

execute_process (COMMAND "${SUDO}" ${apt_program} upgrade
				 COMMAND_ECHO STDOUT)

#

function (_lemons_deps_os_install_func deps)

	execute_process (COMMAND "${SUDO}" ${apt_program} install -y --no-install-recommends ${deps}
		             COMMAND_ECHO STDOUT
		             COMMAND_ERROR_IS_FATAL ANY)
	
endfunction()
