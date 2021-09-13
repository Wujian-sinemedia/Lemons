include (FindPackageHandleStandardArgs)

add_subdirectory (ccache)

include (juce_config.cmake)

include (mts_esp_config.cmake)
include (ableton_link_config.cmake)

include (fftw_config.cmake)

add_subdirectory (vecops)

include (static_analysis.cmake)