include (juce_config.cmake)

include (mts_esp_config.cmake)
include (ableton_link_config.cmake)

if (NOT APPLE)
	# default to using vDSP on Apple platforms, elsewhere, check for Intel IPP...
	include (ipp_config.cmake)
endif()

include (vecops_config.cmake)
