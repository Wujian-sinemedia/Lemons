find_program (AUVAL_PROGRAM auval)


function (_lemons_configure_auval_tests)

	if (NOT AUVAL_PROGRAM)
		return()
	endif()

	set (options "")
    set (oneValueArgs TARGET)
    set (multiValueArgs "")

    cmake_parse_arguments (LEMONS_AUV "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set (au_target "${LEMONS_AUV_TARGET}_AU")

    if (NOT TARGET "${au_target}")
    	return()
    endif()

    ###

    message (STATUS "Configured auval tests for target: ${LEMONS_AUV_TARGET}!")

endfunction()