if (MSVC OR WIN32)
    set (CPACK_GENERATOR "NSIS" CACHE INTERNAL "")
    set (CPACK_NSIS_MODIFY_PATH ON CACHE INTERNAL "")
    return()
endif()

if (APPLE)
    set (CPACK_GENERATOR "PackageMaker" CACHE INTERNAL "")
    #set (CPACK_OSX_PACKAGE_VERSION "${${UPPER_PROJECT_NAME}_OSX_VERSION}")
    return()
endif()


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


if (LSB_DISTRIBUTOR_ID MATCHES "Ubuntu")
    set (CPACK_GENERATOR "DEB" CACHE INTERNAL "")
    return()
endif()


if (LSB_DISTRIBUTOR_ID MATCHES "RedHatEnterpriseServer")
    set (CPACK_GENERATOR "RPM" CACHE INTERNAL "")
    return()
endif()


find_program (DEB_EXE debuild)

if (DEB_EXE)
    set (CPACK_GENERATOR "DEB" CACHE INTERNAL "")
    return()
endif()


find_program (RPM_EXE rpmbuild)

if (RPM_EXE)
    set (CPACK_GENERATOR "RPM" CACHE INTERNAL "")
else()
    set (CPACK_GENERATOR "TGZ" CACHE INTERNAL "")
endif()
