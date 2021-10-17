function (dirlist dir outvar)

	file (GLOB children RELATIVE ${dir} ${dir}/*)

  	set (dirlist "")

  	foreach (child ${children})
    	if (IS_DIRECTORY ${dir}/${child})
      		list (APPEND dirlist ${child})
    	endif()
  	endforeach()

  	set (${outvar} ${dirlist} PARENT_SCOPE)

endfunction()

#

function (run_clangformat_on_subdir dir)

	dirlist ("${dir}" subdirs)

	foreach (subdir ${subdirs})
		run_clangformat_on_subdir ("${subdir}")
	endforeach()

	execute_process (COMMAND "${CLANG_FORMAT}" -i *.c *.h *.cpp *.hpp
		             WORKING_DIRECTORY "${dir}")
endfunction()

#

set (START_DIR "")

run_clangformat_on_subdir ("${START_DIR}")
