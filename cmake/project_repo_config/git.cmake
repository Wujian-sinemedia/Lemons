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
