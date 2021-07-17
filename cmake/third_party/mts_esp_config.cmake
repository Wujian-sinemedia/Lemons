CPMAddPackage (
        NAME MTS-ESP
        GIT_REPOSITORY https://github.com/ODDSound/MTS-ESP.git
        GIT_TAG origin/main)

set (bv_mts_esp_dir ${MTS-ESP_SOURCE_DIR}/Client CACHE INTERNAL "Path to MTS-ESP sources")

add_library (MTS-ESP INTERFACE)

target_sources             (MTS-ESP INTERFACE "${bv_mts_esp_dir}/libMTSClient.cpp")
target_include_directories (MTS-ESP INTERFACE ${bv_mts_esp_dir})
target_compile_definitions (MTS-ESP INTERFACE BV_USE_MTS_ESP=1)

option (BV_USE_MTS_ESP "Use MTS-ESP" OFF)

#

macro (_configure_mts_esp)
    if (${BV_USE_MTS_ESP})
	   target_link_libraries (bv_midi INTERFACE MTS-ESP)
    endif()
endmacro()