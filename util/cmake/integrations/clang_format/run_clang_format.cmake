include (@CF_FILEUTILS_SCRIPT@)

#

function (run_clangformat_on_subdir dir)

	lemons_subdir_list (DIR "${dir}" RESULT subdirs)

	foreach (subdir ${subdirs})
		run_clangformat_on_subdir ("${subdir}")
	endforeach()

	execute_process (COMMAND "@CLANG_FORMAT@" -i *.c *.h *.cpp *.hpp
		             WORKING_DIRECTORY "${dir}")
endfunction()

#

run_clangformat_on_subdir ("@LEMONS_TOPLEVEL_DIR@")
run_clangformat_on_subdir ("@LEMONS_REPO_ROOT@/modules")