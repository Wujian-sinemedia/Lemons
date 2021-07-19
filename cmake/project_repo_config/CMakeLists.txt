include (git_hooks/GitHooks.cmake)


function (bv_update_repo_gitignore repodir)
	bv_copy_file (${CMAKE_CURRENT_LIST_DIR}/default_gitignore.txt ${repodir}/.gitignore)
endfunction()
