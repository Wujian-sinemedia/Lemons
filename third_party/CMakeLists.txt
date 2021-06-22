
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

    target_sources (MTS-ESP INTERFACE 
        ../third_party/MTS-ESP/Client/libMTSClient.h
        ../third_party/MTS-ESP/Client/libMTSClient.cpp)

    target_include_directories (MTS-ESP INTERFACE "${CMAKE_CURRENT_LIST_DIR}/../third_party/MTS-ESP/Client")
    target_compile_definitions (MTS-ESP INTERFACE BV_USE_MTS_ESP=1)

    target_link_libraries (bv_midi INTERFACE MTS-ESP)

endif()


##################
##### VECOPS #####
##################

if (NOT DEFINED BV_IGNORE_VDSP)
    set (BV_IGNORE_VDSP FALSE)
endif()

if (NOT DEFINED BV_IGNORE_MIPP)
    set (BV_IGNORE_MIPP FALSE)
endif()

if (APPLE AND NOT ${BV_IGNORE_VDSP})

	### vDSP ###

    message (STATUS "Configuring vDSP for vecops...")

    target_compile_definitions (bv_core INTERFACE JUCE_USE_VDSP_FRAMEWORK=1 BV_USE_VDSP=1)

elseif (NOT ${BV_IGNORE_MIPP})

	### MIPP ###

    message (STATUS "Configuring MIPP for vecops...")

	add_library (MIPP INTERFACE)

	target_sources (MIPP INTERFACE MIPP/src/mipp.h)

	target_include_directories (MIPP INTERFACE "MIPP/src")

	target_compile_definitions (MIPP INTERFACE BV_USE_MIPP=1 MIPP_ENABLE_BACKTRACE=1)

    target_link_libraries (bv_core INTERFACE MIPP)
    
else()
    message (STATUS "Using fallback vecops implementations...")
endif()


