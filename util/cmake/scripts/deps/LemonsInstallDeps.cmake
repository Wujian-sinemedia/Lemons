include_guard (GLOBAL)

include (LemonsCmakeDevTools)
include (LemonsParseDepsfile)

if (APPLE)
	include (os/install_mac.cmake)
elseif (WIN32)
	include (os/install_win.cmake)
else()
	include (os/install_linux.cmake)
endif()


function (lemons_install_deps)

	function (_lemons_install_deps_list list)
		if (list)
			list (JOIN list " " deps)

			separate_arguments (command UNIX_COMMAND "${deps}")

			_lemons_deps_os_install_func ("${command}")
		endif()
	endfunction()

	cmake_parse_arguments (LEMONS_DEPS "OMIT_DEFAULT" "FILE" "CATEGORIES" ${ARGN})

	lemons_require_function_arguments (LEMONS_DEPS FILE)

	lemons_parse_depsfile (${LEMONS_DEPS_FILE})

	if (NOT LEMONS_DEPS_OMIT_DEFAULT)
		if (APPLE)
			if (CATEGORY_Mac)
				_lemons_install_deps_list ("${CATEGORY_Mac}")
			endif()
		elseif (WIN32)
			if (CATEGORY_Windows)
				_lemons_install_deps_list ("${CATEGORY_Windows}")
			endif()
		else()
			if (CATEGORY_Linux)
				_lemons_install_deps_list ("${CATEGORY_Linux}")
			endif()
		endif()

		if (CATEGORY_Default)
			_lemons_install_deps_list ("${CATEGORY_Default}")
		endif()
	endif()

	foreach (category ${LEMONS_DEPS_CATEGORIES})
		if (CATEGORY_${category})
			_lemons_install_deps_list ("${CATEGORY_${category}}")
		endif()
	endforeach()

endfunction()
