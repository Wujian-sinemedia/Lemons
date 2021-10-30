include ("@LEMONS_CMAKE_ROOT@/FileUtilities.cmake")

#

function (_lemons_cf_process_dir dirpath)

	lemons_subdir_list (DIR "${dirpath}" RESULT subdirs)

	foreach (subdir ${subdirs})
		_lemons_cf_process_dir ("${dirpath}/${subdir}")
	endforeach()

	execute_process (COMMAND "@CLANG_FORMAT@" -i *.h *.hpp *.c *.cpp
					 WORKING_DIRECTORY "${dirpath}"
					 TIMEOUT 120)

endfunction()

#

_lemons_cf_process_dir ("@CMAKE_SOURCE_DIR@")
