if (NOT GIT_FOUND)
    find_package (Git QUIET)
endif()

if (NOT GIT_FOUND)
    message (WARNING "Git not found! Some repository configuration features may not work as expected.")
endif()

#

function (bv_update_repo_gitignore repodir)
    bv_copy_file (SOURCE ${BV_REPO_CONFIG_FILES_DIR}/default_gitignore.txt DEST ${repodir}/.gitignore)
endfunction()

#

function (bv_update_repo_git_submodules repodir)

    message (STATUS "Updating git submodules in ${repodir}")

    set (git_command ${GIT_EXECUTABLE})

    set (submodule_command "git fetch && git checkout origin/HEAD && git submodule update && git pull")

    set (git_args "submodule foreach ${submodule_command}")

    execute_process (COMMAND ${git_command} ${git_args}
                     WORKING_DIRECTORY ${repodir})

endfunction()