set (BV_GITHOOKS_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE INTERNAL "Path to githook scripts")

if (NOT GIT_FOUND)
    find_package (Git QUIET)
endif()

function (bv_configure_precommit_git_hook projectDir)
    if (NOT GIT_FOUND OR NOT EXISTS ${projectDir}/.git)
        return()
    endif()

    if (NOT CLANG_FORMAT)
        find_program (CLANG_FORMAT clang-format)
    endif()

    if (NOT CLANG_FORMAT)
        return()
    endif()

    configure_file (${BV_GITHOOKS_DIR}/pre_commit_script.in
                    ${projectDir}/.git/hooks/pre-commit)
endfunction()