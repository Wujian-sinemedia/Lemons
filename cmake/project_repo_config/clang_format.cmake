set (BV_CLANG_FORMAT_FILETYPES "*.h *.hpp *.c *.cpp" CACHE INTERNAL "Filetypes clang-format will be used on")

if (NOT CLANG_FORMAT)
    find_program (CLANG_FORMAT clang-format)
endif()


macro (_bv_update_repo_clangformat_config_file _repo_dir)
    bv_copy_file (${Shared-code_SOURCE_DIR}/_clang-format ${_repo_dir}/_clang-format)
endmacro()


function (_bv_update_clangformat_recursive_script sourcedir repodir)
    set (DEST_DIR ${repodir}/run_clang_format.sh)

    if (EXISTS ${DEST_DIR})
        file (REMOVE ${DEST_DIR})
    endif()

    if (NOT UNIX OR NOT CLANG_FORMAT OR NOT EXISTS ${sourcedir})
        return()
    endif()

    set (BV_PROJECT_SOURCE_DIR ${sourcedir})

    configure_file (${BV_REPO_CONFIG_FILES_DIR}/run_clang_format.sh ${DEST_DIR})

    bv_enable_all_file_permissions (${DEST_DIR})
endfunction()


function (bv_update_repo_clangformat repodir)
    _bv_update_repo_clangformat_config_file (${repodir})
	_bv_update_clangformat_recursive_script (${repodir}/Source ${repodir})
endfunction()
