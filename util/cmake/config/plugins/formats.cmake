#
#  LV2

set (LEMONS_LV2_AVAILABLE FALSE)

if (LEMONS_USE_LV2_JUCE)
    if (NOT WIN32 AND NOT APPLE AND EXISTS ${JUCE_SOURCE_DIR}/modules/juce_audio_plugin_client/juce_audio_plugin_client_LV2.cpp)
        set (LEMONS_LV2_AVAILABLE TRUE)
    endif()
endif()


#
#  VST2
set (LEMONS_VST2_AVAILABLE FALSE)

if (LEMONS_VST2_SDK_PATH)
    if (IS_DIRECTORY "${LEMONS_VST2_SDK_PATH}")
        set (LEMONS_VST2_AVAILABLE TRUE)
    endif()
endif()


###

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
        juce_set_vst2_sdk_path ("${LEMONS_VST2_SDK_PATH}")
        list (APPEND available_formats VST)
    endif()
endif()

set (LEMONS_PLUGIN_FORMATS ${available_formats} CACHE INTERNAL "Available plugin formats" FORCE)

list (JOIN available_formats " " formats_output)
message (STATUS "  -- Available plugin formats: ${formats_output}")

