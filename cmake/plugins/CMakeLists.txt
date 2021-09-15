if (NOT WIN32 AND EXISTS ${JUCE_SOURCE_DIR}/modules/juce_audio_plugin_client/juce_audio_plugin_client_LV2.cpp)
    set (BV_LV2_AVAILABLE TRUE CACHE INTERNAL "Availability of LV2 plugin format" FORCE)
else()
    set (BV_LV2_AVAILABLE FALSE CACHE INTERNAL "Availability of LV2 plugin format" FORCE)
endif()

#

if (NOT DEFINED BV_AAX_SDK_PATH OR NOT IS_DIRECTORY ${BV_AAX_SDK_PATH})
    set (BV_AAX_AVAILABLE FALSE CACHE INTERNAL "Availability of the AAX plugin SDK" FORCE)
else()
    set (BV_AAX_AVAILABLE TRUE CACHE INTERNAL "Availability of the AAX plugin SDK" FORCE)
endif()

if (${BV_AAX_AVAILABLE})
    juce_set_aax_sdk_path (${BV_AAX_SDK_PATH})
endif()

#

set (BV_VST2_AVAILABLE FALSE CACHE INTERNAL "Availability of the VST2 plugin SDK")

#

set (available_formats Standalone)

if (NOT ("${CMAKE_SYSTEM_NAME}" MATCHES "iOS"))
    list (APPEND available_formats Unity VST3)

    if (APPLE)
        list (APPEND available_formats AU AUv3)
    endif()

    if (${BV_LV2_AVAILABLE})
        list (APPEND available_formats LV2)
    endif()

    if (${BV_AAX_AVAILABLE})
        list (APPEND available_formats AAX)
    endif()

    if (${BV_VST2_AVAILABLE})
        list (APPEND available_formats VST)
    endif()
endif()

set (BV_PLUGIN_FORMATS ${available_formats} CACHE INTERNAL "Available plugin formats" FORCE)

message (STATUS "Available plugin formats: ${BV_PLUGIN_FORMATS}")

#

include (plugin_config.cmake)
include (plugin_hosting_config.cmake)
