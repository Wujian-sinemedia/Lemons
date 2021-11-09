include_guard (GLOBAL)


function (lemons_configure_dsp_signal_chain)

	set (options "")
    set (oneValueArgs JSON TARGET)
    set (multiValueArgs "")

    cmake_parse_arguments (LEMONS_GEN "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (NOT LEMONS_GEN_JSON)
    	message (FATAL_ERROR "JSON not specified in call to ${CMAKE_CURRENT_FUNCTION}!")
    endif()

    if (NOT EXISTS "${LEMONS_GEN_JSON}")
    	message (AUTHOR_WARNING "${CMAKE_CURRENT_FUNCTION}: Nonexistant JSON file!")
    	return()
    endif()

    if (NOT LEMONS_GEN_TARGET)
    	message (FATAL_ERROR "TARGET not specified in call to ${CMAKE_CURRENT_FUNCTION}!")
    endif()

    if (TARGET ${LEMONS_GEN_TARGET})
    	message (AUTHOR_WARNING "${CMAKE_CURRENT_FUNCTION}: A target with the requested name ${LEMONS_GEN_TARGET} already exists!")
    	return()
    endif()



    add_library (INTERFACE ${LEMONS_GEN_TARGET})

endfunction()
