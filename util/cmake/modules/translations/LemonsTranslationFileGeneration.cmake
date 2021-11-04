include_guard (GLOBAL)


find_package (Python3 COMPONENTS Interpreter)

if (NOT Python3_Interpreter_FOUND)
    message (FATAL_ERROR "Python3 is required for translation file generation, but could not be found!")
	return()
endif()


if (NOT JUCE_SOURCE_DIR OR NOT Lemons_SOURCE_DIR)
    message (FATAL_ERROR "Missing required variable definitions...")
    return()
endif()

#

set (LEMONS_LANGUAGE_LIST "${CMAKE_CURRENT_LIST_DIR}/languages.txt" 
     CACHE PATH "Path to a file containing a list of target languages for translation file generation")

if (DEFINED ENV{CPM_SOURCE_CACHE})
    set (LEMONS_SHARED_TRANSLATION_FILE_STORAGE "$ENV{CPM_SOURCE_CACHE}/translations" 
            CACHE PATH "Path to directory where cached translation files for JUCE and Lemons will be stored")
else()
    set (LEMONS_SHARED_TRANSLATION_FILE_STORAGE "${CMAKE_SOURCE_DIR}/Cache/translations" 
            CACHE PATH "Path to directory where cached translation files for JUCE and Lemons will be stored")
endif()

mark_as_advanced (FORCE LEMONS_SHARED_TRANSLATION_FILE_STORAGE LEMONS_LANGUAGE_LIST)

#

message (STATUS "Generating translation files for Lemons and JUCE...")

execute_process (COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_LIST_DIR}/scripts/config_shared_translations.py" "${JUCE_SOURCE_DIR}" "${Lemons_SOURCE_DIR}" "${LEMONS_SHARED_TRANSLATION_FILE_STORAGE}" "${LEMONS_LANGUAGE_LIST}"
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

    file (REAL_PATH "${PROJECT_SOURCE_DIR}/${LEMONS_TRANS_FOLDER}" translation_output)

    execute_process (COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_LIST_DIR}/scripts/config_project_translations.py" "${LEMONS_SHARED_TRANSLATION_FILE_STORAGE}" "${PROJECT_SOURCE_DIR}" "${translation_output}"
                     OUTPUT_QUIET)
endfunction()
