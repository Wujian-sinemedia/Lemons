set (BV_GITHOOKS_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE INTERNAL "Path to githook scripts")

if (NOT GIT_FOUND)
    find_package (Git QUIET)
endif()


function (bv_configure_precommit_git_hook projectDir)
    if (NOT UNIX)
        return()
    endif()

    if (NOT GIT_FOUND OR NOT EXISTS ${projectDir}/.git)
        return()
    endif()

    if (NOT CLANG_FORMAT)
        find_program (CLANG_FORMAT clang-format)
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

    file (CHMOD ${DEST_DIR} 
          PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_WRITE WORLD_EXECUTE SETUID SETGID)

endfunction()