add_subdirectory (CPM)

include (juce_config.cmake)

include (mts_esp_config.cmake)
include (ableton_link_config.cmake)

#include (ipp_config.cmake)
include (vecops_config.cmake)

#

function (_bv_configure_third_party_libs target alwaysVDSP neverVDSP useMtsEsP useAbletonLink)

	_bv_configure_vecops (${target} ${alwaysVDSP} ${neverVDSP})

	if (${useMtsEsP})
        _bv_configure_mts_esp (${BV_TARGETCONFIG_TARGET})
    endif()

    if (${useAbletonLink})
        _bv_configure_ableton_link (${BV_TARGETCONFIG_TARGET})
    endif()
endfunction()