if (NOT WIN32 AND EXISTS ${JUCE_SOURCE_DIR}/modules/juce_audio_plugin_client/juce_audio_plugin_client_LV2.cpp)
    set (BV_LV2_AVAILABLE TRUE)
else()
    set (BV_LV2_AVAILABLE FALSE)
endif()

#

if (NOT DEFINED BV_AAX_SDK_PATH OR NOT IS_DIRECTORY ${BV_AAX_SDK_PATH})
    set (BV_AAX_AVAILABLE FALSE)
else()
    set (BV_AAX_AVAILABLE TRUE)
endif()

#

set (BV_VST2_AVAILABLE FALSE)

#

set (available_formats Standalone)

if (NOT ("${CMAKE_SYSTEM_NAME}" MATCHES "iOS"))
    list (APPEND available_formats Unity VST3)

    if (APPLE)
        list (APPEND available_formats AU AUv3)
    endif()

    if (BV_LV2_AVAILABLE)
        list (APPEND available_formats LV2)
    endif()

    if (BV_AAX_AVAILABLE)
        juce_set_aax_sdk_path (${BV_AAX_SDK_PATH})
        list (APPEND available_formats AAX)
    endif()

    if (BV_VST2_AVAILABLE)
        list (APPEND available_formats VST)
    endif()
endif()

set (BV_PLUGIN_FORMATS ${available_formats} CACHE INTERNAL "Available plugin formats" FORCE)

message (STATUS "  -- Available plugin formats: ${BV_PLUGIN_FORMATS}")

#

include (plugin_config.cmake)
include (plugin_hosting_config.cmake)
