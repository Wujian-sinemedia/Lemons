
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
