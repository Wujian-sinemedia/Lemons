find_program (PYTHON_EXEC python3)

#

function (lemons_generate_translation_files)

	if (NOT PYTHON_EXEC)
		return()
	endif()

	set (options "")
    set (oneValueArgs TARGET FOLDER)
    set (multiValueArgs "")

    cmake_parse_arguments (LEMONS_TRANS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    #

	set (translation_scripts_dir "${LEMONS_REPO_ROOT}/scripts/translations")
	set (generate_translation_template "${translation_scripts_dir}/generate_translation_file_template.py")

	######################################

	function (_lemons_generate_template_translation_file workingDir name)

		set (translations_file "${name}_translations.txt")
		set (translations_file_abs_path "${workingDir}/${translations_file}")

		if (NOT EXISTS "${translations_file_abs_path}")
			message (STATUS "Generating translation file for ${name}...")

			execute_process (COMMAND "${PYTHON_EXEC}" "${generate_translation_template}" "modules" "${translations_file}"
						 	 WORKING_DIRECTORY "${workingDir}")
		endif()

		set ("${name}_translations_file" "${translations_file_abs_path}" PARENT_SCOPE)

	endfunction()

	######################################

	# Generate Lemons translations, if needed
	_lemons_generate_template_translation_file ("${LEMONS_REPO_ROOT}" "Lemons")

	# Generate JUCE translations, if needed
	_lemons_generate_template_translation_file ("${JUCE_SOURCE_DIR}" "JUCE")


	message (STATUS "Generating translation files for target: ${LEMONS_TRANS_TARGET}")

	set (translations_folder "${LEMONS_TRANS_FOLDER}/translations")
	set (template_file "${translations_folder}/needed_translations.txt")

	set (template_file_abs_path "${LEMONS_PROJECT_REPO_DIR}/${template_file}")

	file (MAKE_DIRECTORY "${LEMONS_PROJECT_REPO_DIR}/${translations_folder}")

	# Generate the product's template translations file
	execute_process (COMMAND "${PYTHON_EXEC}" "${generate_translation_template}" "Source" "${template_file}"
					 WORKING_DIRECTORY "${LEMONS_PROJECT_REPO_DIR}")

	# Merge the product's template translation file with the generated Lemons and JUCE translation files
	execute_process (COMMAND "${PYTHON_EXEC}" "${translation_scripts_dir}/merge_translation_files.py" "${template_file_abs_path}" "-l" "${Lemons_translations_file}" "${JUCE_translations_file}"
					 WORKING_DIRECTORY "${LEMONS_PROJECT_REPO_DIR}")

	# Translate the template file into each target language
	execute_process (COMMAND "${PYTHON_EXEC}" "${translation_scripts_dir}/generate_translation_files.py" "${template_file}" "${translations_folder}"
					 WORKING_DIRECTORY "${LEMONS_PROJECT_REPO_DIR}")

	file (REMOVE "${template_file_abs_path}")

endfunction()
