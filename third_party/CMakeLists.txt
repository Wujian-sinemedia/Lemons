
### JUCE ###

add_subdirectory (JUCE)


### Ableton Link ###

if (NOT DEFINED BV_USE_ABLETON_LINK)
	set (BV_USE_ABLETON_LINK FALSE)
endif()


### MTS-ESP ###

if (NOT DEFINED BV_USE_MTS_ESP)
    set (BV_USE_MTS_ESP FALSE)
endif()

if (${BV_USE_MTS_ESP})

	add_library (MTS-ESP INTERFACE)

    target_sources (MTS-ESP INTERFACE 
        ../third_party/MTS-ESP/Client/libMTSClient.h
        ../third_party/MTS-ESP/Client/libMTSClient.cpp)

    target_include_directories (MTS-ESP INTERFACE "${CMAKE_CURRENT_LIST_DIR}/../third_party/MTS-ESP/Client")
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
