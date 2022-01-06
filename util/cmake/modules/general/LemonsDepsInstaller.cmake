include_guard (GLOBAL)

include (LemonsGetCPM)
include (LemonsCmakeDevTools)

function (lemons_install_dependencies)

	set (oneValueArgs DEPFILE CATEGORY)

	cmake_parse_arguments (LEMONS_DEPS "" "${oneValueArgs}" "")

	lemons_require_function_arguments (LEMONS_DEPS DEPFILE)

	CPMAddPackage (
        NAME DepsInstaller
        GITHUB_REPOSITORY benthevining/DepsInstaller
        GIT_TAG origin/main)

	set (installer_script "${CPM_DepsInstaller_SOURCE}/install_deps.cmake")

	if (LEMONS_DEPS_CATEGORY)
		separate_arguments (installer_command UNIX_COMMAND "-D DEPFILE=${LEMONS_DEPS_DEPFILE} -D CAT=${LEMONS_DEPS_CATEGORY} -P ${installer_script}")
	else()
		separate_arguments (installer_command UNIX_COMMAND "-D DEPFILE=${LEMONS_DEPS_DEPFILE} -P ${installer_script}")
	endif()

	execute_process (COMMAND "${CMAKE_COMMAND}" ${installer_command}
					 COMMAND_ERROR_IS_FATAL ANY)

endfunction()
