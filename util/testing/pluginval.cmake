

function (_lemons_configure_pluginval_tests)

	set (options "")
    set (oneValueArgs TARGET LEVEL)
    set (multiValueArgs "")

    cmake_parse_arguments (LEMONS_PIV "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    ###

    message (STATUS "Configured pluginval tests for target: ${LEMONS_PIV_TARGET}!")

endfunction()
