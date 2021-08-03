set (BV_GITHOOKS_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE INTERNAL "Path to githook scripts")


function (bv_configure_precommit_git_hook projectDir)
    set (DEST_DIR ${projectDir}/.git/hooks/pre-commit)
    
    if (EXISTS ${DEST_DIR})
        file (REMOVE ${DEST_DIR})
    endif()

    # the pre-commit hook is a bash script, so unix only
    if (NOT GIT_FOUND OR NOT CLANG_FORMAT OR NOT UNIX)
        return()
    endif()

    configure_file (${BV_GITHOOKS_DIR}/pre_commit_script.in
                    ${DEST_DIR})

    bv_enable_all_file_permissions (${DEST_DIR})
endfunction()