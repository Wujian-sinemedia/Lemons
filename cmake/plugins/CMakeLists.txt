#
#  LV2
if (NOT WIN32 AND NOT APPLE AND EXISTS ${JUCE_SOURCE_DIR}/modules/juce_audio_plugin_client/juce_audio_plugin_client_LV2.cpp)
    set (LEMONS_LV2_AVAILABLE TRUE)
else()
    set (LEMONS_LV2_AVAILABLE FALSE)
endif()


#
#  AAX
if (NOT LEMONS_AAX_SDK_PATH)
    set (LEMONS_AAX_AVAILABLE FALSE)
else()
    if (NOT IS_DIRECTORY ${LEMONS_AAX_SDK_PATH})
        set (LEMONS_AAX_AVAILABLE FALSE)
    else()
        set (LEMONS_AAX_AVAILABLE TRUE)
    endif()
endif()


#
#  VST2
set (LEMONS_VST2_AVAILABLE FALSE)


#

set (available_formats Standalone)

if (APPLE)
    list (APPEND available_formats AUv3)
endif()

if (NOT ("${CMAKE_SYSTEM_NAME}" MATCHES "iOS"))
    list (APPEND available_formats Unity VST3)

    if (APPLE)
        list (APPEND available_formats AU)
    endif()

    if (LEMONS_LV2_AVAILABLE)
        list (APPEND available_formats LV2)
    endif()

    if (LEMONS_AAX_AVAILABLE)
        juce_set_aax_sdk_path (${LEMONS_AAX_SDK_PATH})
        list (APPEND available_formats AAX)
    endif()

    if (LEMONS_VST2_AVAILABLE)
        list (APPEND available_formats VST)
    endif()
endif()

set (LEMONS_PLUGIN_FORMATS ${available_formats} CACHE INTERNAL "Available plugin formats" FORCE)

message (STATUS "  -- Available plugin formats: ${LEMONS_PLUGIN_FORMATS}")

#

include (plugin_config.cmake)
include (plugin_hosting_config.cmake)
