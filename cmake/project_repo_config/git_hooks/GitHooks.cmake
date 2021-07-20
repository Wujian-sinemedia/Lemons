set (BV_GITHOOKS_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE INTERNAL "Path to githook scripts")

if (NOT GIT_FOUND)
    find_package (Git QUIET)
endif()

if (NOT GIT_FOUND OR NOT UNIX)
    function (bv_configure_precommit_git_hook projectDir)
    endfunction()

    return()
endif()


function (bv_configure_precommit_git_hook projectDir)
    if (NOT EXISTS ${projectDir}/.git)
        return()
    endif()

    if (NOT CLANG_FORMAT)
        return()
    endif()

    set (DEST_DIR ${projectDir}/.git/hooks/pre-commit)
    
    if (EXISTS ${DEST_DIR})
        file (REMOVE ${DEST_DIR})
    endif()

    configure_file (${BV_GITHOOKS_DIR}/pre_commit_script.in
                    ${DEST_DIR})

    bv_enable_all_file_permissions (${DEST_DIR})
endfunction()