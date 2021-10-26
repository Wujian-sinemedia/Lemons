find_program (PYTHON_EXEC python3)

option (LEMONS_GENERATE_TRANSLATION_FILES "Always skip translation file generation" ON)

set (_lemons_translation_scripts_dir "${LEMONS_REPO_ROOT}/util/translations" CACHE INTERNAL "" FORCE)
set (_lemons_translation_files_storage "$ENV{CPM_SOURCE_CACHE}/translations" CACHE INTERNAL "" FORCE)


if (PYTHON_EXEC AND LEMONS_GENERATE_TRANSLATION_FILES)
	message (STATUS "Generating translation files for Lemons and JUCE...")
	
	execute_process (COMMAND "${PYTHON_EXEC}" "${_lemons_translation_scripts_dir}/config_shared_translations.py" "${JUCE_SOURCE_DIR}" "${LEMONS_REPO_ROOT}" "${_lemons_translation_files_storage}"
    				 COMMAND_ECHO STDOUT
    				 WORKING_DIRECTORY "${LEMONS_REPO_ROOT}")
endif()

#

function (lemons_generate_translation_files)

	if (NOT PYTHON_EXEC)
		return()
	endif()

	if (NOT LEMONS_GENERATE_TRANSLATION_FILES)
		return()
	endif()

	set (options "")
    set (oneValueArgs TARGET FOLDER)
    set (multiValueArgs "")

    cmake_parse_arguments (LEMONS_TRANS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    message (STATUS "Generating translation files for target: ${LEMONS_TRANS_TARGET}")

    execute_process (COMMAND "${PYTHON_EXEC}" "${_lemons_translation_scripts_dir}/config_project_translations.py" "${_lemons_translation_files_storage}" "${LEMONS_TRANS_FOLDER}/translations"
    				 COMMAND_ECHO STDOUT
    				 WORKING_DIRECTORY "${LEMONS_REPO_ROOT}")
endfunction()
