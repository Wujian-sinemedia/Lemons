
include (get_cpm.cmake)

#

include (juce_config.cmake)

include (mts_esp_config.cmake)

#

macro (_configure_vecops)

    if (DEFINED BV_IGNORE_VDSP)
        target_compile_definitions (bv_core INTERFACE JUCE_USE_VDSP_FRAMEWORK=0 BV_USE_VDSP=0)
    elseif (APPLE)
        message (STATUS "Configuring vDSP for vecops...")
        target_compile_definitions (bv_core INTERFACE JUCE_USE_VDSP_FRAMEWORK=1 BV_USE_VDSP=1)
    endif()

endmacro()

#

macro (_configure_third_party_libs)

    _configure_mts_esp()
    _configure_vecops()

endmacro()

