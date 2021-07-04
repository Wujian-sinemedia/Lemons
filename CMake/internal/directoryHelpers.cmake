macro (_subdir_list result curdir)

  file (GLOB children RELATIVE ${curdir} ${curdir}/*)

  set (dirlist "")

  foreach (child ${children})
    if (IS_DIRECTORY ${curdir}/${child})
      list (APPEND dirlist ${child})
    endif()
  endforeach()

  set (${result} ${dirlist})

endmacro()

#

macro (_add_all_subdirs)

  set (subdirs "")
  _subdir_list (subdirs ${CMAKE_CURRENT_LIST_DIR})

  foreach (subdir ${subdirs})
    add_subdirectory (${subdir})
  endforeach()

endmacro()