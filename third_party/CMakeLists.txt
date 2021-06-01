add_subdirectory (JUCE)

#

if (NOT DEFINED BV_USE_ABLETON_LINK)
	set (BV_USE_ABLETON_LINK FALSE)
endif()

if (${BV_USE_ABLETON_LINK})

include (ableton-link/AbletonLinkConfig.cmake)

add_library (AbletonLink INTERFACE)

target_link_libraries (AbletonLink INTERFACE Ableton::Link)

target_include_directories (AbletonLink INTERFACE "ableton-link/include/ableton" "ableton-link/src/ableton" "ableton-link")

target_compile_definitions (AbletonLink INTERFACE BV_USE_ABLETON_LINK=1)

endif()

#

if (NOT DEFINED BV_USE_MTS_ESP)
    set (BV_USE_MTS_ESP FALSE)
endif()

if (${BV_USE_MTS_ESP})

add_library (MTS-ESP INTERFACE)

target_sources (MTS-ESP INTERFACE 
	MTS-ESP/Client/libMTSClient.h
	MTS-ESP/Client/libMTSClient.cpp)

target_include_directories (MTS-ESP INTERFACE "MTS-ESP/Client" "MTS-ESP")

target_compile_definitions (MTS-ESP INTERFACE BV_USE_MTS_ESP=1)

endif()

#

if (NOT DEFINED BV_IGNORE_MIPP)
    set (BV_IGNORE_MIPP FALSE)
endif()

if (NOT ${BV_IGNORE_MIPP})

add_library (MIPP INTERFACE)

target_sources (MIPP INTERFACE 
	MIPP/src/mipp.h)

target_include_directories (MIPP INTERFACE "MIPP/src" "MIPP")

target_compile_definitions (MIPP INTERFACE MIPP_ENABLE_BACKTRACE BV_USE_MIPP=1)
    
endif()
