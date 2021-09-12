if (NOT WIN32 AND EXISTS ${JUCE_SOURCE_DIR}/modules/juce_audio_plugin_client/juce_audio_plugin_client_LV2.cpp)
    set (BV_LV2_AVAILABLE TRUE CACHE INTERNAL "Availability of LV2 plugin format")
else()
    set (BV_LV2_AVAILABLE FALSE CACHE INTERNAL "Availability of LV2 plugin format")
endif()

set (BV_AAX_AVAILABLE FALSE CACHE INTERNAL "Availability of the AAX plugin SDK")
set (BV_VST2_AVAILABLE FALSE CACHE INTERNAL "Availability of the VST2 plugin SDK")

#

include (plugin_format_helpers.cmake)
include (plugin_config.cmake)
include (plugin_hosting_config.cmake)
