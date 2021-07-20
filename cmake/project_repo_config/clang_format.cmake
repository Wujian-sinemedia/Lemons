macro (_bv_update_repo_clangformat_config_file _repo_dir)
    bv_copy_file (${Shared-code_SOURCE_DIR}/_clang-format ${_repo_dir}/_clang-format)
endmacro()

if (NOT CLANG_FORMAT)
    find_program (CLANG_FORMAT clang-format)
endif()

if (NOT CLANG_FORMAT)
    function (bv_update_repo_clangformat repodir)
        _bv_update_repo_clangformat_config_file (${repodir})
    endfunction()

    return()
endif()

#

function (_bv_update_clangformat_recursive_script sourcedir repodir)
    if (NOT UNIX)
        return()
    endif()

    set (BV_PROJECT_SOURCE_DIR ${sourcedir})

    if (NOT EXISTS ${BV_PROJECT_SOURCE_DIR})
        return()
    endif()

    if ("${repodir}" STREQUAL "${Shared-code_SOURCE_DIR}")
        set (BV_RUN_CLANG_FORMAT_IN_SOURCE_DIR 0)
    else()
        set (BV_RUN_CLANG_FORMAT_IN_SOURCE_DIR 1)
    endif()

    set (DEST_DIR ${repodir}/run_clang_format.sh)

    if (EXISTS ${DEST_DIR})
        file (REMOVE ${DEST_DIR})
    endif()

    configure_file (${BV_REPO_CONFIG_FILES_DIR}/run_clang_format.sh ${DEST_DIR})

    bv_enable_all_file_permissions (${DEST_DIR})
endfunction()


function (bv_update_repo_clangformat repodir)
    _bv_update_repo_clangformat_config_file (${repodir})
	_bv_update_clangformat_recursive_script (${repodir}/Source ${repodir})
endfunction()
