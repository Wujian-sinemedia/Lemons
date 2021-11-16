include_guard (GLOBAL)

if (CPACK_GENERATOR MATCHES "DEB")
	include ("${CMAKE_CURRENT_LIST_DIR}/deb_settings.cmake")
elseif (CPACK_GENERATOR MATCHES "RPM")
	include ("${CMAKE_CURRENT_LIST_DIR}/rpm_settings.cmake")
endif()
