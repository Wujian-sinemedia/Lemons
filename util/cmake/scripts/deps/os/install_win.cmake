include_guard (GLOBAL)

find_program (CHOCO choco)

if (NOT CHOCO)
	unset (CACHE{CHOCO})

	find_program (POWERSHELL powershell)

	if (NOT POWERSHELL)
		message (FATAL_ERROR "powershell is required for installing Homebrew, and cannot be found!")
	endif()

	message (STATUS "Installing Chocolatey...")

	execute_process (COMMAND "${POWERSHELL}" "${CMAKE_CURRENT_LIST_DIR}/install_chocolatey.ps1"
					 COMMAND_ECHO STDOUT
					 COMMAND_ERROR_IS_FATAL ANY)

	find_program (CHOCO choco)

	if (NOT CHOCO)
		message (FATAL_ERROR "Error installing Chocolatey!")
	endif()
endif()

#

function (_lemons_deps_os_install_func deps)

	execute_process (COMMAND "${CHOCO}" install ${deps}
		             COMMAND_ECHO STDOUT
		             COMMAND_ERROR_IS_FATAL ANY)

endfunction()
