set (BV_GITHUB_SCRIPTS_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE INTERNAL "Path to the GitHub scripts")

#

function (_bv_update_github_script repodir script)

	macro (_bv_script_path_from_repo_dir _scriptpath_out _scriptname _repodir)
		if ("${_scriptname}" STREQUAL "FUNDING")
			set (${_scriptpath_out} ${_repodir}/.github/FUNDING.yml)
		else()
			set (${_scriptpath_out} ${_repodir}/.github/workflows/${_scriptname}.yml)
		endif()
	endmacro()

	_bv_script_path_from_repo_dir (dest_dir ${script} ${repodir})

	bv_copy_file (${BV_GITHUB_SCRIPTS_DIR}/${script}.yml ${dest_dir})

endfunction()

#

function (bv_update_github_scripts repodir includeBuildScript)

	if (NOT EXISTS ${repodir}/.github)
		return()
	endif()

	_bv_update_github_script (${repodir} "FUNDING")
	_bv_update_github_script (${repodir} "RepoMaintenance")
	_bv_update_github_script (${repodir} "UpdateGitSubmodules")

	if (${includeBuildScript})
		_bv_update_github_script (${repodir} "Build")
	endif()
endfunction()