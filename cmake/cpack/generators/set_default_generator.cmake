if (MSVC OR WIN32)
    set (CPACK_GENERATOR "NSIS")
    set (CPACK_NSIS_MODIFY_PATH ON)
    return()
endif()

if (APPLE)
    set (CPACK_GENERATOR "PackageMaker")
    #set (CPACK_OSX_PACKAGE_VERSION "${${UPPER_PROJECT_NAME}_OSX_VERSION}")
    return()
endif()


# Linux

include (linux/LSBInfo.cmake)

if (LSB_DISTRIBUTOR_ID MATCHES "Ubuntu")
    set (CPACK_GENERATOR "DEB")
    return()
endif()

if (LSB_DISTRIBUTOR_ID MATCHES "RedHatEnterpriseServer")
    set (CPACK_GENERATOR "RPM")
    return()
endif()

find_program (DEB_EXE debuild)

if (DEB_EXE)
    set (CPACK_GENERATOR "DEB")
    return()
endif()

find_program (RPM_EXE rpmbuild)

if (RPM_EXE)
    set (CPACK_GENERATOR "RPM")
else()
    set (CPACK_GENERATOR "TGZ")
endif()