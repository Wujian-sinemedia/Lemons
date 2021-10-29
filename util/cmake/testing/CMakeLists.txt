add_subdirectory (UnitTestRunner)


include (pluginval.cmake)
include (auval.cmake)


function (_lemons_configure_plugin_tests target)

	if (NOT TARGET "${target}")
		message (WARNING "Cannot configure tests for nonexistant plugin target ${target}!")
		return()
	endif()

	_lemons_configure_pluginval_tests (TARGET "${target}" LEVEL 10)
	_lemons_configure_auval_tests (TARGET "${target}")

endfunction()