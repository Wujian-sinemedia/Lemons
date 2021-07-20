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


function (bv_enable_all_file_permissions filepath)
	file (CHMOD ${filepath}
          PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_WRITE WORLD_EXECUTE SETUID SETGID)
endfunction()