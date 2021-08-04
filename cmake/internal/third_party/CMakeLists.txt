add_subdirectory (CPM)

include (juce_config.cmake)

include (mts_esp_config.cmake)
include (ableton_link_config.cmake)

#include (ipp_config.cmake)
include (vecops_config.cmake)

#

function (_bv_configure_third_party_libs target alwaysVDSP neverVDSP useAbletonLink)

	_bv_configure_vecops (${target} ${alwaysVDSP} ${neverVDSP})

    if (${useAbletonLink})
        _bv_configure_ableton_link (${target})
    endif()
endfunction()