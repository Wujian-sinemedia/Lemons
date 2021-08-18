set (BV_REPO_CONFIG_FILES_DIR "${CMAKE_CURRENT_LIST_DIR}/other" CACHE INTERNAL "Path to the project repo config files")

include (git.cmake)
include (clang_format.cmake)

include (git_hooks/GitHooks.cmake)
include (github_scripts/GitHub_scripts.cmake)

#

function (bv_update_git_repo repodir)
	bv_update_repo_clangformat      (${repodir})
	bv_update_repo_gitignore        (${repodir})
	# bv_configure_precommit_git_hook (${repodir})
	bv_update_github_scripts        (${repodir} TRUE)
endfunction()

#

# Update the SharedCode repo itself....
#bv_configure_precommit_git_hook         (${Shared-code_SOURCE_DIR})
bv_update_github_scripts                (${Shared-code_SOURCE_DIR} FALSE)
_bv_update_clangformat_recursive_script (${Shared-code_SOURCE_DIR}/modules ${Shared-code_SOURCE_DIR})