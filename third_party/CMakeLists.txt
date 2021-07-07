
set (BV_THIRD_PARTY_DIR "${CMAKE_CURRENT_LIST_DIR}" CACHE INTERNAL "Directory of third-party submodules")

add_subdirectory (JUCE)
add_subdirectory (cmake)

#

macro (_configure_third_party_libs)

	_configure_ableton_link()
	_configure_mts_esp()
	_configure_vecops()

endmacro()