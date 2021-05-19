
macro (_subdir_list result curdir)
    file (GLOB children RELATIVE ${curdir} ${curdir}/*)
    set (dirlist "")

    foreach (child ${children})
        if (IS_DIRECTORY ${curdir}/${child})
            list (APPEND dirlist ${child})
        endif ()
    endforeach ()

    set (${result} ${dirlist})
endmacro()

#

function (_add_subdirectory_if_valid subdir)
    if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/${subdir}/CMakeLists.txt)
        add_subdirectory (${subdir})
    endif ()
endfunction()

#

function (_add_all_subdirs)
    _subdir_list (subDirs ${CMAKE_CURRENT_LIST_DIR})

    foreach (subdir ${subDirs})
        _add_subdirectory_if_valid (${subdir})
    endforeach()
endfunction()

#

function (_add_all_flagged_subdirs)
    _subdir_list (subDirs ${CMAKE_CURRENT_LIST_DIR})

    foreach (subdir ${subDirs})
        set (buildflag "Build${subdir}")

        if (${buildflag})
            _add_subdirectory_if_valid (${subdir})
        endif ()
    endforeach ()
endfunction()