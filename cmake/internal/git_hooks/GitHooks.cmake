set (BV_GITHOOKS_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE INTERNAL "Path to githook scripts")

function (bv_configure_precommit_git_hook projectDir)
    if (NOT EXISTS ${projectDir}/.git)
        return()
    endif()

    if (NOT GIT_FOUND)
        find_package (Git QUIET)
    endif()

    if (NOT CLANG_FORMAT)
        find_program (CLANG_FORMAT clang-format)
    endif()

    if (NOT GIT_FOUND OR NOT CLANG_FORMAT)
        return()
    endif()

    message (STATUS "Configuring pre-commit git hook in dir: ${projectDir}")

    configure_file (${BV_GITHOOKS_DIR}/pre_commit_script.in
                    ${projectDir}/.git/hooks/pre-commit)
endfunction()