add_subdirectory (UnitTestRunner)

include (pluginval.cmake)
include (auval.cmake)
include (steinberg_val.cmake)


function (_lemons_configure_plugin_tests target)

	if (NOT TARGET "${target}")
		message (FATAL_ERROR "Cannot configure tests for nonexistant plugin target ${target}!")
		return()
	endif()

	_lemons_configure_pluginval_tests (TARGET "${target}")
	#_lemons_configure_auval_tests (TARGET "${target}")

endfunction()