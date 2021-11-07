#[[
# LemonsTranslationFileGeneration   {#LemonsTranslationFileGeneration}


## Functions:
```
lemons_generate_translation_files (TARGET <target> FOLDER <output_folder>)
```
Generates JUCE-style translation files for the given target (by scanning the source tree recursively, starting at ${PROJECT_SOURCE_DIR}), and merges them with the shared Lemons and JUCE translation output.
Places the output files in <folder>, which is relative to the project's root directory.

You can control the list of output languages by setting LEMONS_LANGUAGE_LIST to the path to a text file containing a list of target languages, separated by lines. Otherwise a default list of target languages will be used.

The first time you call this function, shared translation files for Lemons and JUCE will be generated, that are cached and reused for each project's configuration.

Note that this module's functionality requires Python3.

]]


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

if (DEFINED ENV{CPM_SOURCE_CACHE})
    set (LEMONS_SHARED_TRANSLATION_FILE_STORAGE "$ENV{CPM_SOURCE_CACHE}/translations" 
            CACHE PATH "Path to directory where cached translation files for JUCE and Lemons will be stored")
else()
    set (LEMONS_SHARED_TRANSLATION_FILE_STORAGE "${CMAKE_SOURCE_DIR}/Cache/translations" 
            CACHE PATH "Path to directory where cached translation files for JUCE and Lemons will be stored")
endif()

set (LEMONS_TRANSLATION_SCRIPTS_DIR "${CMAKE_CURRENT_LIST_DIR}/scripts" CACHE INTERNAL "")

set (LEMONS_LANGUAGE_LIST "${CMAKE_CURRENT_LIST_DIR}/languages.txt" 
     CACHE PATH "Path to a file containing a list of target languages for translation file generation")

mark_as_advanced (FORCE LEMONS_SHARED_TRANSLATION_FILE_STORAGE LEMONS_LANGUAGE_LIST LEMONS_TRANSLATION_SCRIPTS_DIR)

#


set (shared_translations_generated FALSE CACHE INTERNAL "")


function (_lemons_generate_shared_translations)

    message (STATUS "Generating translation files for Lemons and JUCE...")

    execute_process (COMMAND "${Python3_EXECUTABLE}" "${LEMONS_TRANSLATION_SCRIPTS_DIR}/config_shared_translations.py" "${JUCE_SOURCE_DIR}" "${Lemons_SOURCE_DIR}" "${LEMONS_SHARED_TRANSLATION_FILE_STORAGE}" "${LEMONS_LANGUAGE_LIST}"
                     OUTPUT_QUIET)

    set (shared_translations_generated TRUE CACHE INTERNAL "")
endfunction()

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

    if (NOT shared_translations_generated)
        _lemons_generate_shared_translations()
    endif()

    message (VERBOSE "Generating translation files for target: ${LEMONS_TRANS_TARGET}")

    file (REAL_PATH "${PROJECT_SOURCE_DIR}/${LEMONS_TRANS_FOLDER}" translation_output)

    execute_process (COMMAND "${Python3_EXECUTABLE}" "${LEMONS_TRANSLATION_SCRIPTS_DIR}/config_project_translations.py" "${LEMONS_SHARED_TRANSLATION_FILE_STORAGE}" "${PROJECT_SOURCE_DIR}" "${translation_output}"
                     OUTPUT_QUIET)
endfunction()
