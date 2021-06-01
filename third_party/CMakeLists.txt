
### JUCE ###

add_subdirectory (JUCE)


### Ableton Link ###

if (NOT DEFINED BV_USE_ABLETON_LINK)
	set (BV_USE_ABLETON_LINK FALSE)
endif()

if (${BV_USE_ABLETON_LINK})
	include (ableton-link/AbletonLinkConfig.cmake)
endif()


### MTS-ESP ###

if (NOT DEFINED BV_USE_MTS_ESP)
    set (BV_USE_MTS_ESP FALSE)
endif()

if (${BV_USE_MTS_ESP})

	add_library (MTS-ESP STATIC)

	target_sources (MTS-ESP PRIVATE 
		MTS-ESP/Client/libMTSClient.h
		MTS-ESP/Client/libMTSClient.cpp)

	target_include_directories (MTS-ESP INTERFACE "MTS-ESP/Client")
	target_compile_definitions (MTS-ESP INTERFACE BV_USE_MTS_ESP=1)

endif()


### MIPP ###

if (NOT DEFINED BV_IGNORE_MIPP)
    set (BV_IGNORE_MIPP FALSE)
endif()

if (NOT ${BV_IGNORE_MIPP})

	add_library (MIPP INTERFACE)

	target_sources (MIPP INTERFACE MIPP/src/mipp.h)

	target_include_directories (MIPP INTERFACE "MIPP/src")
	target_compile_definitions (MIPP INTERFACE BV_USE_MIPP=1 MIPP_ENABLE_BACKTRACE=1)
    
endif()
