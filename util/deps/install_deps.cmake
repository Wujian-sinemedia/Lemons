if (APPLE)
	include ("${CMAKE_CURRENT_LIST_DIR}/os/install_mac.cmake")
elseif (WIN32)
	include ("${CMAKE_CURRENT_LIST_DIR}/os/install_win.cmake")
else()
	include ("${CMAKE_CURRENT_LIST_DIR}/os/install_linux.cmake")
endif()

#

function (install_deps_from_file file_path)

	file (STRINGS "${file_path}" file_list)

	# set (deps_list "")

	# foreach (dep ${file_list})
	# 	if (NOT "${dep}" IN_LIST deps_list)
	# 		list (APPEND deps_list "${dep}")
	# 	endif()
	# endforeach()

	list (JOIN file_list " " deps)

	separate_arguments (command UNIX_COMMAND "${deps}")

	os_install_func ("${command}")

endfunction()

#

install_deps_from_file ("${CMAKE_CURRENT_LIST_DIR}/deps_list.txt")

if (NOT APPLE AND NOT WIN32)
	install_deps_from_file ("${CMAKE_CURRENT_LIST_DIR}/juce_linux_deps.txt")
endif()

if (DOCS)
	install_deps_from_file ("${CMAKE_CURRENT_LIST_DIR}/docs_deps.txt")
endif()
