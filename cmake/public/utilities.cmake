function (bv_subdir_list result curdir)
  	file (GLOB children RELATIVE ${curdir} ${curdir}/*)

  	set (dirlist "")

  	foreach (child ${children})
    	if (IS_DIRECTORY ${curdir}/${child})
      		list (APPEND dirlist ${child})
    	endif()
  	endforeach()

  	set (${result} ${dirlist} PARENT_SCOPE)
endfunction()


function (bv_copy_file in_path out_path)
	file (READ ${in_path} filetext)
	file (WRITE ${out_path} ${filetext})
endfunction()


function (bv_list_all_cmake_variables)
	message (STATUS "")
  	get_cmake_property (vs VARIABLES)
  	foreach (v ${vs})
    	message (STATUS "${v} = '${${v}}'")
  	endforeach()
  	message (STATUS "")
endfunction()