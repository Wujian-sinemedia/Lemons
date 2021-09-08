if (NOT WIN32 AND EXISTS ${JUCE_SOURCE_DIR}/modules/juce_audio_plugin_client/juce_audio_plugin_client_LV2.cpp)
    set (BV_LV2_AVAILABLE TRUE CACHE INTERNAL "Availability of LV2 plugin format")
else()
    set (BV_LV2_AVAILABLE FALSE CACHE INTERNAL "Availability of LV2 plugin format")
endif()

set (BV_AAX_AVAILABLE FALSE CACHE INTERNAL "Availability of the AAX plugin SDK")
set (BV_VST2_AVAILABLE FALSE CACHE INTERNAL "Availability of the VST2 plugin SDK")

#

include (plugin_format_helpers.cmake)
include (format_specific_config.cmake)
include (plugin_hosting_config.cmake)

#

function (_bv_plugin_target_postconfig targetName)

	target_link_libraries (${targetName} PUBLIC ${BV_PLUGIN_ONLY_MODULES})

    target_compile_definitions (${targetName} PUBLIC
            JUCE_MICROPHONE_PERMISSION_ENABLED=1
            JUCE_USE_CUSTOM_PLUGIN_STANDALONE_APP=0)

    get_target_property (formats ${targetName} JUCE_FORMATS)

    if (NOT formats)
        return()
    endif()

    foreach (format ${formats})
        _bv_configure_target_install ("${targetName}_${format}")
    endforeach()

    _bv_configure_target_workspace_deploy (${targetName})

endfunction()