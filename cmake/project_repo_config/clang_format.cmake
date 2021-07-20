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

function (bv_update_repo_clangformat repodir)
    _bv_update_repo_clangformat_config_file (${repodir})

	# configure a shell script in the project repo to run clang-format recursively over its source tree #

	if (NOT UNIX)
        return()
    endif()

    set (BV_PROJECT_SOURCE_DIR ${repodir}/Source)

    if (NOT EXISTS ${BV_PROJECT_SOURCE_DIR})
    	return()
    endif()

	set (DEST_DIR ${repodir}/scripts/shell/run_clang_format.sh)

	if (EXISTS ${DEST_DIR})
        file (REMOVE ${DEST_DIR})
    endif()

	configure_file (${BV_REPO_CONFIG_FILES_DIR}/run_clang_format.sh ${DEST_DIR})

    bv_enable_all_file_permissions (${DEST_DIR})
endfunction()
