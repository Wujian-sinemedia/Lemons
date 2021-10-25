find_program (PYTHON_EXEC python3)


function (lemons_generate_translation_files outputFolder)

	if (NOT PYTHON_EXEC)
		return()
	endif()

	message (STATUS "Writing translation files to ${outputFolder}...")

	set (translation_scripts_dir "${LEMONS_REPO_ROOT}/scripts")

	execute_process (COMMAND "${PYTHON_EXEC}" "${translation_scripts_dir}/generate_translation_file_template.py" "Source" "${outputFolder}/needed_translations.txt"
					 WORKING_DIRECTORY "${outputFolder}/..")

	execute_process (COMMAND "${PYTHON_EXEC}" "${translation_scripts_dir}/generate_translation_files.py" "${outputFolder}/needed_translations.txt" "${outputFolder}"
					 WORKING_DIRECTORY "${outputFolder}/..")

endfunction()