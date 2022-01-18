#!/usr/bin/env cmake -P

find_program (cppCheckProgram NAMES cppcheck)

if (NOT cppCheckProgram)
	message (FATAL_ERROR "cppcheck not found!")
endif()

#

file (REAL_PATH "${CMAKE_CURRENT_LIST_DIR}/.." lemons_root EXPAND_TILDE)

set (tests_dir "${lemons_root}/util/tests")
set (build_dir "${tests_dir}/Builds")

#

message (STATUS "Configuring CMake...")

if (IS_DIRECTORY "${build_dir}")
	file (REMOVE_RECURSE "${build_dir}")
endif()

execute_process (COMMAND "${CMAKE_COMMAND}" -B Builds
				 WORKING_DIRECTORY "${tests_dir}"
				 COMMAND_ECHO STDOUT
				 COMMAND_ERROR_IS_FATAL ANY)


message (STATUS "Running cppcheck...")

set (logs_dir "${lemons_root}/logs/cppcheck")

if (NOT IS_DIRECTORY "${logs_dir}")
	file (MAKE_DIRECTORY "${logs_dir}")
endif()

set (logFile "${logs_dir}/output.log")
set (errorFile "${logs_dir}/errors.log")

file (TOUCH ${logFile})
file (TOUCH ${errorFile})

set (cppcheck_cache_dir "${lemons_root}/Cache/cppcheck")

if (NOT IS_DIRECTORY "${cppcheck_cache_dir}")
	file (MAKE_DIRECTORY "${cppcheck_cache_dir}")
endif()

execute_process (COMMAND "${cppCheckProgram}" "--addon=threadsafety.py" "--project=${build_dir}/compile_commands.json" "--cppcheck-build-dir=${cppcheck_cache_dir}" "--bug-hunting"
				 WORKING_DIRECTORY "${tests_dir}"
				 COMMAND_ECHO STDOUT
				 OUTPUT_FILE "${logFile}"
				 ERROR_FILE "${errorFile}"
				 OUTPUT_VARIABLE std_out OUTPUT_STRIP_TRAILING_WHITESPACE ECHO_OUTPUT_VARIABLE
				 ERROR_VARIABLE std_err ERROR_STRIP_TRAILING_WHITESPACE ECHO_ERROR_VARIABLE)


if (std_out)
	message (STATUS ${std_out})
else()
	if (EXISTS ${logFile})
		file (READ "${logFile}" logFileContents)

		if (logFileContents)
			message (STATUS ${logFileContents})
		endif()
	endif()
endif()


function (_lemons_run_cppcheck_print_std_err errOutput)
	message (WARNING "cppcheck errors:")
	message (STATUS ${errOutput})
endfunction()

if (std_err)
	_lemons_run_cppcheck_print_std_err (${std_err})
else()
	if (EXISTS ${errorFile})
		file (READ "${errorFile}" errorFileContents)

		if (errorFileContents)
			_lemons_run_cppcheck_print_std_err (${errorFileContents})
		endif()
	endif()
endif()
