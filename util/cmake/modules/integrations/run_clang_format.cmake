include ("@LEMONS_CMAKE_ROOT@/FileUtilities.cmake")

#

function (_lemons_cf_process_dir dirpath)
	if (NOT IS_DIRECTORY "${dirpath}")
		return()
	endif()

	execute_process (COMMAND "@CLANG_FORMAT@" -i *.h *.hpp *.c *.cpp
				 	 WORKING_DIRECTORY "${dirpath}"
				 	 OUTPUT_QUIET ERROR_QUIET)

	lemons_subdir_list (DIR "${dirpath}" RESULT subdirs)

	foreach (subdir ${subdirs})
		_lemons_cf_process_dir ("${dirpath}/${subdir}")
	endforeach()
endfunction()

#

_lemons_cf_process_dir ("@CMAKE_SOURCE_DIR@")
