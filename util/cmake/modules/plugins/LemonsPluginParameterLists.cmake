include_guard (GLOBAL)

find_package (Python3 COMPONENTS Interpreter)

if (NOT Python3_Interpreter_FOUND)
    message (FATAL_ERROR "Python3 is required for code generation, but could not be found!")
    return()
endif()

set (lemons_param_list_config_script "${CMAKE_CURRENT_LIST_DIR}/scripts/configure_parameter_list.py" CACHE INTERNAL "")


function (lemons_configure_plugin_parameter_list)

    set (options "")
    set (oneValueArgs JSON TARGET)
    set (multiValueArgs "")

    cmake_parse_arguments (LEMONS_PARAM "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (NOT LEMONS_PARAM_JSON)
        message (FATAL_ERROR "JSON not specified in call to ${CMAKE_CURRENT_FUNCTION}!")
    endif()

    cmake_path (IS_ABSOLUTE LEMONS_PARAM_JSON json_path_is_absolute)

    if (json_path_is_absolute)
        set (json_file "${LEMONS_PARAM_JSON}")
    else()
        set (json_file "${PROJECT_SOURCE_DIR}/${LEMONS_PARAM_JSON}")
    endif()

    if (NOT EXISTS "${json_file}")
        message (FATAL_ERROR "${CMAKE_CURRENT_FUNCTION}: Nonexistant input JSON file!")
    endif()

    if (NOT LEMONS_PARAM_TARGET)
        message (FATAL_ERROR "TARGET not specified in call to ${CMAKE_CURRENT_FUNCTION}!")
    endif() 

    if (NOT TARGET ${LEMONS_PARAM_TARGET})
        message (FATAL_ERROR "${CMAKE_CURRENT_FUNCTION}: Target ${LEMONS_GEN_TARGET} does not exist!")
    endif()

    execute_process (COMMAND "${Python3_EXECUTABLE}" "${lemons_param_list_config_script}" "${json_file}")

endfunction()
