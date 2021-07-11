macro (_configure_mts_esp)

	if (NOT DEFINED BV_USE_MTS_ESP)
	    set (BV_USE_MTS_ESP FALSE)
	endif()

	if (${BV_USE_MTS_ESP})

		message (STATUS "Configuring MTS-ESP...")

		add_library (MTS-ESP INTERFACE)

	    set (MTS_ESP_DIR "bv_mts_esp_dir/Client")
	    set (MTS_ESP_SOURCES "${MTS_ESP_DIR}/libMTSClient.cpp")

	    target_sources             (MTS-ESP INTERFACE ${MTS_ESP_SOURCES})
	    target_include_directories (MTS-ESP INTERFACE ${MTS_ESP_DIR})
	    target_compile_definitions (MTS-ESP INTERFACE BV_USE_MTS_ESP=1)

	    set_source_files_properties (${MTS_ESP_SOURCES} TARGET_DIRECTORY bv_midi PROPERTIES COMPILE_FLAGS "-w")

	    target_link_libraries (bv_midi INTERFACE MTS-ESP)

	endif()

endmacro()