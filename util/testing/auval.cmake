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

    set (testName "${au_target}-Auval")

    add_test (NAME "${testName}" 
              COMMAND "${AUVAL_PROGRAM}")

    set_tests_properties ("${testName}" PROPERTIES 
                          REQUIRED_FILES "$<TARGET_FILE:${au_target}>"
                          LABELS Auval)

    message (STATUS "Configured auval tests for target: ${LEMONS_AUV_TARGET}!")

endfunction()
