
### Ableton Link ###

if (NOT DEFINED BV_USE_ABLETON_LINK)
	set (BV_USE_ABLETON_LINK FALSE)
endif()

if (${BV_USE_ABLETON_LINK})

    message (STATUS "Configuring Ableton Link...")

    include (../third_party/ableton-link/AbletonLinkConfig.cmake)

    target_compile_definitions (Ableton::Link INTERFACE BV_USE_ABLETON_LINK=1)

    target_include_directories (Ableton::Link INTERFACE 
        "${CMAKE_CURRENT_LIST_DIR}/ableton-link/include/ableton"
        "${CMAKE_CURRENT_LIST_DIR}/ableton-link/include/ableton/platforms")

    target_link_libraries (bv_plugin INTERFACE Ableton::Link)

endif()


### MTS-ESP ###

if (NOT DEFINED BV_USE_MTS_ESP)
    set (BV_USE_MTS_ESP FALSE)
endif()

if (${BV_USE_MTS_ESP})

	message (STATUS "Configuring MTS-ESP...")

	add_library (MTS-ESP INTERFACE)

    set (MTS_ESP_DIR "${CMAKE_CURRENT_LIST_DIR}/../third_party/MTS-ESP/Client")
    set (MTS_ESP_SOURCES "${MTS_ESP_DIR}/libMTSClient.cpp")

    target_sources             (MTS-ESP INTERFACE ${MTS_ESP_SOURCES})
    target_include_directories (MTS-ESP INTERFACE ${MTS_ESP_DIR})
    target_compile_definitions (MTS-ESP INTERFACE BV_USE_MTS_ESP=1)

    set_source_files_properties (${MTS_ESP_SOURCES} TARGET_DIRECTORY bv_midi PROPERTIES COMPILE_FLAGS "-w")

    target_link_libraries (bv_midi INTERFACE MTS-ESP)

endif()


### vDSP ###

if (DEFINED BV_IGNORE_VDSP)
    target_compile_definitions (bv_core INTERFACE JUCE_USE_VDSP_FRAMEWORK=0 BV_USE_VDSP=0)
elseif (APPLE)
    message (STATUS "Configuring vDSP for vecops...")
    target_compile_definitions (bv_core INTERFACE JUCE_USE_VDSP_FRAMEWORK=1 BV_USE_VDSP=1)
endif()


