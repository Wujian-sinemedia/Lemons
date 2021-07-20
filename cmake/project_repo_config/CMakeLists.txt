set (BV_REPO_CONFIG_FILES_DIR "${CMAKE_CURRENT_LIST_DIR}/other" CACHE INTERNAL "Path to the project repo config files")

include (clang_format.cmake)

include (git_hooks/GitHooks.cmake)
include (github_scripts/GitHub_scripts.cmake)


function (bv_update_repo_gitignore repodir)
	bv_copy_file (${BV_REPO_CONFIG_FILES_DIR}/default_gitignore.txt ${repodir}/.gitignore)
endfunction()


function (bv_update_git_repo repodir)
	bv_update_repo_clangformat      (${repodir})
	bv_update_repo_gitignore        (${repodir})
	bv_configure_precommit_git_hook (${repodir})
	bv_update_github_scripts        (${repodir} TRUE)
endfunction()
