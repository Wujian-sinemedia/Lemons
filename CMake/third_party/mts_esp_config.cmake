CPMAddPackage (
        NAME MTS-ESP
        GIT_REPOSITORY https://github.com/ODDSound/MTS-ESP.git
        GIT_TAG origin/main)

set (bv_mts_esp_dir ${MTS-ESP_SOURCE_DIR}/Client CACHE INTERNAL "Path to MTS-ESP sources")

if (NOT DEFINED BV_USE_MTS_ESP)
    set (BV_USE_MTS_ESP FALSE)
endif()

#

function (_configure_mts_esp)

    if (NOT ${BV_USE_MTS_ESP})
	return()
    endif()

    message (STATUS "Configuring MTS-ESP...")

    add_library (MTS-ESP INTERFACE)

    set (MTS_ESP_SOURCES "${bv_mts_esp_dir}/libMTSClient.cpp")

    target_sources             (MTS-ESP INTERFACE ${MTS_ESP_SOURCES})
    target_include_directories (MTS-ESP INTERFACE ${bv_mts_esp_dir})
    target_compile_definitions (MTS-ESP INTERFACE BV_USE_MTS_ESP=1)

    set_source_files_properties (${MTS_ESP_SOURCES} TARGET_DIRECTORY bv_midi PROPERTIES COMPILE_FLAGS "-w")

    target_link_libraries (bv_midi INTERFACE MTS-ESP)

endfunction()