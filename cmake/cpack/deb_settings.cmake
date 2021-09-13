# Follow Debian package naming conventions:
# https://www.debian.org/doc/manuals/debian-faq/ch-pkg_basics.en.html
# Build version, e.g. name_1.3.2~xenial_amd64 or name_1.3.2-1~xenial_amd64
# when re-releasing.
# Note: the ~codename is not part of any standard and could be omitted.
if (NOT CPACK_DEBIAN_PACKAGE_VERSION)
    include (LSBInfo)

    if (${LSB_CODENAME} STREQUAL "bionic")
        set (CPACK_DEBIAN_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION}")
    else()
        set (CPACK_DEBIAN_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION}~${LSB_CODENAME}")
    endif()
endif()

if (NOT CPACK_PACKAGE_FILE_NAME)
    execute_process (COMMAND dpkg --print-architecture OUTPUT_VARIABLE deb_arch OUTPUT_STRIP_TRAILING_WHITESPACE)
    set (CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}_${CPACK_DEBIAN_PACKAGE_VERSION}_${deb_arch}")
endif()

if (NOT CPACK_DEBIAN_BUILD_DEPENDS)
    set (CPACK_DEBIAN_BUILD_DEPENDS cmake git pkg-config)
endif()

#if (NOT CPACK_DEBIAN_PACKAGE_CONFLICTS)
#    set (CPACK_DEBIAN_PACKAGE_CONFLICTS ${_package_conflicts})
#endif()

if (NOT DEFINED CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA)
    # script name must be 'postinst' to avoid lintian W: "unknown-control-file"
    set (_ldconfig_script "${CMAKE_CURRENT_LIST_DIR}/postinst")
    set (CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${_ldconfig_script};")
endif()

#if (NOT CPACK_DEBIAN_PACKAGE_DEPENDS)
#    set (CPACK_DEBIAN_PACKAGE_DEPENDS ${${UPPER_PROJECT_NAME}_PACKAGE_DEB_DEPENDS})
#    list_to_string (CPACK_DEBIAN_PACKAGE_DEPENDS)
#endif()

#if (NOT CPACK_DEBIAN_PACKAGE_HOMEPAGE)
#    set (CPACK_DEBIAN_PACKAGE_HOMEPAGE ${${UPPER_PROJECT_NAME}_URL})
#endif()

#if (NOT CPACK_DEBIAN_PACKAGE_REPLACES)
#    set (CPACK_DEBIAN_PACKAGE_REPLACES ${_package_replaces})
#endif()
