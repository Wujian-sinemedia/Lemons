# install JUCE linux deps

execute_process (COMMAND chmod a+rx "${CMAKE_CURRENT_LIST_DIR}/install_juce_linux_deps.sh")

execute_process (COMMAND bash "${CMAKE_CURRENT_LIST_DIR}/install_juce_linux_deps.sh")


# fixes a bug with LTO on Ubuntu with Clang
set (CMAKE_AR ${CMAKE_CXX_COMPILER_AR} CACHE PATH "AR" FORCE)
set (CMAKE_RANLIB ${CMAKE_CXX_COMPILER_RANLIB} CACHE PATH "RANLIB" FORCE)


# detect LSB info

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