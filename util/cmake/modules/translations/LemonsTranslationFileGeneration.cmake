include_guard (GLOBAL)

function (lemons_generate_translation_files)
endfunction()

option (LEMONS_GENERATE_TRANSLATION_FILES "Generate translation files" ON)

if (NOT LEMONS_GENERATE_TRANSLATION_FILES)
    return()
endif()

find_package (Python3 COMPONENTS Interpreter)

if (NOT Python3_Interpreter_FOUND)
    message (WARNING "Python3 is required for translation file generation, but could not be found!")
	return()
endif()


set (LEMONS_LANGUAGE_LIST "${CMAKE_CURRENT_LIST_DIR}/languages.txt" 
     CACHE PATH "Path to a file containing a list of target languages for translation file generation")

#

# TO DO: deal with JUCE_SOURCE_DIR and Lemons_SOURCE_DIR not being defined...

if (DEFINED ENV{CPM_SOURCE_CACHE})
    set (_lemons_translation_files_storage "$ENV{CPM_SOURCE_CACHE}/translations" CACHE PATH "")
else()
    set (_lemons_translation_files_storage "${CMAKE_SOURCE_DIR}/Cache/translations" CACHE PATH "")
endif()

mark_as_advanced (FORCE _lemons_translation_files_storage LEMONS_LANGUAGE_LIST)

message (STATUS "Generating translation files for Lemons and JUCE...")

set (scripts_dir "${CMAKE_CURRENT_LIST_DIR}/scripts")

execute_process (COMMAND "${Python3_EXECUTABLE}" "${scripts_dir}/config_shared_translations.py" "${JUCE_SOURCE_DIR}" "${Lemons_SOURCE_DIR}" "${_lemons_translation_files_storage}" "${LEMONS_LANGUAGE_LIST}"
                 OUTPUT_QUIET)

#

function (lemons_generate_translation_files)

	set (options "")
    set (oneValueArgs TARGET FOLDER)
    set (multiValueArgs "")

    cmake_parse_arguments (LEMONS_TRANS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (NOT LEMONS_TRANS_FOLDER)
        message (FATAL_ERROR "Folder not specified in call to ${CMAKE_CURRENT_FUNCTION}!")
        return()
    endif()

    message (VERBOSE "Generating translation files for target: ${LEMONS_TRANS_TARGET}")

    file (REAL_PATH "${PROJECT_SOURCE_DIR}/${LEMONS_TRANS_FOLDER}/translations" translation_output)

    execute_process (COMMAND "${Python3_EXECUTABLE}" "${scripts_dir}/config_project_translations.py" "${_lemons_translation_files_storage}" "${PROJECT_SOURCE_DIR}" "${translation_output}"
                     OUTPUT_QUIET)

endfunction()
