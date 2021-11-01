add_library (Vecops INTERFACE)

#

if (APPLE)
	target_compile_definitions (Vecops INTERFACE JUCE_USE_VDSP_FRAMEWORK=1 LEMONS_USE_VDSP=1)
	message (VERBOSE " -- using vDSP for vecops -- ")
	return()
endif()

#

include (ipp_config.cmake)

if (TARGET IntelIPP)
	target_link_libraries (Vecops INTERFACE IntelIPP)
	message (VERBOSE " -- using IPP for vecops -- ")
	return()
endif()

#

message (VERBOSE " -- using fallback for vecops -- ")