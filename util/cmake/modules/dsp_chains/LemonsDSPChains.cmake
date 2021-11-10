include_guard (GLOBAL)

find_package (Python3 COMPONENTS Interpreter)

if (NOT Python3_Interpreter_FOUND)
    message (FATAL_ERROR "Python3 is required for code generation, but could not be found!")
    return()
endif()

include (LemonsAudioModules)


set (lemons_dsp_config_script "${CMAKE_CURRENT_LIST_DIR}/scripts/configure_signal_chain.py" CACHE INTERNAL "")


function (lemons_configure_dsp_signal_chain)

	set (options "")
    set (oneValueArgs JSON TARGET HEADER_NAME)
    set (multiValueArgs "")

    cmake_parse_arguments (LEMONS_GEN "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (NOT LEMONS_GEN_JSON)
    	message (FATAL_ERROR "JSON not specified in call to ${CMAKE_CURRENT_FUNCTION}!")
    endif()

    cmake_path (IS_ABSOLUTE LEMONS_GEN_JSON json_path_is_absolute)

    if (json_path_is_absolute)
        set (json_file "${LEMONS_GEN_JSON}")
    else()
        set (json_file "${PROJECT_SOURCE_DIR}/${LEMONS_GEN_JSON}")
    endif()

    if (NOT EXISTS "${json_file}")
    	message (FATAL_ERROR "${CMAKE_CURRENT_FUNCTION}: Nonexistant input JSON file!")
    endif()

    if (NOT LEMONS_GEN_TARGET)
    	message (FATAL_ERROR "TARGET not specified in call to ${CMAKE_CURRENT_FUNCTION}!")
    endif()

    if (NOT TARGET ${LEMONS_GEN_TARGET})
    	message (FATAL_ERROR "${CMAKE_CURRENT_FUNCTION}: Target ${LEMONS_GEN_TARGET} does not exist!")
    endif()

    if (NOT LEMONS_GEN_HEADER_NAME)
        set (LEMONS_GEN_HEADER_NAME "DSPChain.h")
    endif()

    set (generated_header "${CMAKE_CURRENT_BINARY_DIR}/${LEMONS_GEN_HEADER_NAME}")

    execute_process (COMMAND "${Python3_EXECUTABLE}" "${lemons_dsp_config_script}" "${generated_header}" "${json_file}")

    target_sources (${LEMONS_GEN_TARGET} PUBLIC "${generated_header}")

    target_link_libraries (${LEMONS_GEN_TARGET} PUBLIC lemons_dsp)

endfunction()
