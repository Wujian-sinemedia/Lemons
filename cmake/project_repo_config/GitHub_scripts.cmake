function (_bv_update_github_script repodir script)

	macro (_bv_script_path_from_repo_dir _scriptpath_out _scriptname _repodir)
		if ("${_scriptname}" STREQUAL "FUNDING")
			set (${_scriptpath_out} ${_repodir}/.github/FUNDING.yml)
		else()
			set (${_scriptpath_out} ${_repodir}/.github/workflows/${_scriptname}.yml)
		endif()
	endmacro()

	_bv_script_path_from_repo_dir (SOURCE ${script} $CACHE{BV_SHAREDCODE_DIR})
	_bv_script_path_from_repo_dir (DEST   ${script} ${repodir})

	bv_copy_file (${SOURCE} ${DEST})

endfunction()

#

function (bv_update_all_github_scripts repodir)

	_bv_update_github_script (${repodir} "FUNDING")
	_bv_update_github_script (${repodir} "RepoMaintenance")
	_bv_update_github_script (${repodir} "UpdateGitSubmodules")
	
endfunction()