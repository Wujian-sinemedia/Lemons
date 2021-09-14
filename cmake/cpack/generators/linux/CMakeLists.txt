include (LSBInfo.cmake)

if (CPACK_GENERATOR STREQUAL "DEB")
	include (deb/deb_settings.cmake)
elseif (CPACK_GENERATOR STREQUAL "RPM")
	include (rpm/rpm_settings.cmake)
endif()
