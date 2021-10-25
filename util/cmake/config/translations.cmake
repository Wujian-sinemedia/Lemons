find_program (PYTHON_EXEC python3)

option (LEMONS_AUTOGENERATE_TRANSLATION_FILES "Automatically generate translation files during configuration" ON)

#

function (lemons_generate_translation_files target outputFolder)

	if (NOT PYTHON_EXEC)
		return()
	endif()

	if (NOT LEMONS_AUTOGENERATE_TRANSLATION_FILES)
		return()
	endif()

	set (translation_scripts_dir "${LEMONS_REPO_ROOT}/scripts")

	set (translations_folder "${outputFolder}/translations")
	set (template_file "${translations_folder}/needed_translations.txt")

	if (EXISTS "${translations_folder}")
		file (REMOVE "${template_file}")
		return()
	endif()

	message (STATUS "Generating translation files for target: ${target}")

	file (MAKE_DIRECTORY "${translations_folder}")

	execute_process (COMMAND "${PYTHON_EXEC}" "${translation_scripts_dir}/generate_translation_file_template.py" "Source" "${template_file}"
					 WORKING_DIRECTORY "${LEMONS_PROJECT_REPO_DIR}"
					 COMMAND_ECHO STDOUT)

	execute_process (COMMAND "${PYTHON_EXEC}" "${translation_scripts_dir}/generate_translation_files.py" "${template_file}" "${translations_folder}"
					 WORKING_DIRECTORY "${LEMONS_PROJECT_REPO_DIR}"
					 COMMAND_ECHO STDOUT)

	file (REMOVE "${template_file}")

endfunction()