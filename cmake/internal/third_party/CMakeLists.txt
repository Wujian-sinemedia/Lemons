add_subdirectory (CPM)

include (juce_config.cmake)
include (mts_esp_config.cmake)
#include (ipp_config.cmake)

include (vecops_config.cmake)

if (DEFINED IgnoreVDSP)
	if (${IgnoreVDSP})
		set (BV_IGNORE_VDSP TRUE CACHE INTERNAL "Ignore Apple vDSP for this build?")
	else()
		set (BV_IGNORE_VDSP FALSE CACHE INTERNAL "Ignore Apple vDSP for this build?")
	endif()
else()
	set (BV_IGNORE_VDSP FALSE CACHE INTERNAL "Ignore Apple vDSP for this build?")
endif()
