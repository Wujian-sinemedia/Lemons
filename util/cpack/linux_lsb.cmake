# Linux - detect LSB info

find_program (LSB_RELEASE_EXECUTABLE lsb_release)

if (LSB_RELEASE_EXECUTABLE)

    execute_process (COMMAND ${LSB_RELEASE_EXECUTABLE} -sc
                     OUTPUT_VARIABLE LSB_CODENAME OUTPUT_STRIP_TRAILING_WHITESPACE)

    execute_process (COMMAND ${LSB_RELEASE_EXECUTABLE} -sr
                     OUTPUT_VARIABLE LSB_RELEASE OUTPUT_STRIP_TRAILING_WHITESPACE)

    execute_process (COMMAND ${LSB_RELEASE_EXECUTABLE} -si
                     OUTPUT_VARIABLE LSB_DISTRIBUTOR_ID OUTPUT_STRIP_TRAILING_WHITESPACE)
else()
    message (WARNING "Unable to detect LSB info for your Linux distro")

    set (LSB_DISTRIBUTOR_ID "unknown")
    set (LSB_RELEASE "unknown")
    set (LSB_CODENAME "unknown")
endif() 
