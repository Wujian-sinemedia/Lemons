macro (_configure_ableton_link)

	if (NOT DEFINED BV_USE_ABLETON_LINK)
		set (BV_USE_ABLETON_LINK FALSE)
	endif()

	if (${BV_USE_ABLETON_LINK})

	    message (STATUS "Configuring Ableton Link...")

	    include (${bv_ableton_link_dir}/AbletonLinkConfig.cmake)

	    target_compile_definitions (Ableton::Link INTERFACE BV_USE_ABLETON_LINK=1)

	    target_include_directories (Ableton::Link INTERFACE 
	        "${bv_ableton_link_dir}/include/ableton"
	        "${bv_ableton_link_dir}/include/ableton/platforms")

	    target_link_libraries (bv_plugin INTERFACE Ableton::Link)

	endif()

endmacro()