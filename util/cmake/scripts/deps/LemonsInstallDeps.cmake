include_guard (GLOBAL)

include (LemonsParseDepsfile)

if (APPLE)
	include (${CMAKE_CURRENT_LIST_DIR}/os/install_mac.cmake)
elseif (WIN32)
	include (${CMAKE_CURRENT_LIST_DIR}/os/install_win.cmake)
else()
	include (${CMAKE_CURRENT_LIST_DIR}/os/install_linux.cmake)
endif()


option (LEMONS_DEPS_UPDATE_ALL_FIRST "Whether to update all installed system packages before installing additional dependencies" OFF)

if (LEMONS_DEPS_UPDATE_ALL_FIRST)
	_lemons_deps_os_update_func()
endif()


function (lemons_install_deps)

	lemons_get_list_of_deps_to_install (OUTPUT deps_list ${ARGN})

	if (deps_list)
		_lemons_deps_os_install_func ("${deps_list}")
	else()
		message (AUTHOR_WARNING "No dependencies found to install!")
	endif()

endfunction()
