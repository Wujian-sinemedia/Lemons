include (git_hooks/GitHooks.cmake)
include (GitHub_scripts.cmake)
include (github_scripts/GitHub_scripts.cmake)

set (BV_REPO_CONFIG_FILES_DIR "${CMAKE_CURRENT_LIST_DIR}" CACHE INTERNAL "Path to the project repo config files")


function (bv_update_repo_gitignore repodir)
	bv_copy_file (${BV_REPO_CONFIG_FILES_DIR}/default_gitignore.txt ${repodir}/.gitignore)
endfunction()


function (bv_update_repo_clangformat repodir)
	bv_copy_file (${Shared-code_SOURCE_DIR}/_clang-format ${repodir}/_clang-format)
endfunction()


function (bv_update_git_repo repodir)
	bv_update_repo_clangformat      (${repodir})
	bv_update_repo_gitignore        (${repodir})
	bv_configure_precommit_git_hook (${repodir})
	bv_update_github_scripts        (${repodir} TRUE)
endfunction()
