macro (_configure_ableton_link)

	if (NOT DEFINED BV_USE_ABLETON_LINK)
		set (BV_USE_ABLETON_LINK FALSE)
	endif()

	if (${BV_USE_ABLETON_LINK})

	    message (STATUS "Configuring Ableton Link...")

	    set (_ABLETON_LINK_DIR ${BV_THIRD_PARTY_DIR}/ableton-link)

	    include (${_ABLETON_LINK_DIR}/AbletonLinkConfig.cmake)

	    target_compile_definitions (Ableton::Link INTERFACE BV_USE_ABLETON_LINK=1)

	    target_include_directories (Ableton::Link INTERFACE 
	        "${_ABLETON_LINK_DIR}/include/ableton"
	        "${_ABLETON_LINK_DIR}/include/ableton/platforms")

	    target_link_libraries (bv_plugin INTERFACE Ableton::Link)

	endif()

endmacro()