find_program (PYTHON_EXEC python3)


set (_lemons_translation_scripts_dir "${LEMONS_PYTHON_SCRIPTS_DIR}/translations" CACHE INTERNAL "")

file (REAL_PATH "$ENV{CPM_SOURCE_CACHE}/translations" translation_files)
set (_lemons_translation_files_storage "${translation_files}" CACHE INTERNAL "")


if (PYTHON_EXEC AND LEMONS_GENERATE_TRANSLATION_FILES)
	message (STATUS "Generating translation files for Lemons and JUCE...")

	file (REAL_PATH "${JUCE_SOURCE_DIR}" juce_path)
	
	execute_process (COMMAND "${PYTHON_EXEC}" "${_lemons_translation_scripts_dir}/config_shared_translations.py" "${juce_path}" "${LEMONS_REPO_ROOT}" "${_lemons_translation_files_storage}")
endif()

#

function (lemons_generate_translation_files)

	if (NOT PYTHON_EXEC OR NOT LEMONS_GENERATE_TRANSLATION_FILES)
		return()
	endif()

	set (options "")
    set (oneValueArgs TARGET FOLDER)
    set (multiValueArgs "")

    cmake_parse_arguments (LEMONS_TRANS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    message (STATUS "Generating translation files for target: ${LEMONS_TRANS_TARGET}")

    file (REAL_PATH "${LEMONS_PROJECT_REPO_DIR}/${LEMONS_TRANS_FOLDER}/translations" translation_output)

    execute_process (COMMAND "${PYTHON_EXEC}" "${_lemons_translation_scripts_dir}/config_project_translations.py" "${_lemons_translation_files_storage}" "${LEMONS_PROJECT_REPO_DIR}" "${translation_output}")
endfunction()
