juce_add_modules (bv_core
                  bv_dsp
                  bv_midi
                  bv_mobile_utils
                  bv_plugin
                  bv_networking
                  )

add_subdirectory (GUI)

#

if (${BV_USE_ABLETON_LINK})

    message (STATUS "Configuring Ableton Link...")

    include (../third_party/ableton-link/AbletonLinkConfig.cmake)

    target_link_libraries (bv_plugin INTERFACE Ableton::Link)

    target_compile_definitions (bv_plugin INTERFACE BV_USE_ABLETON_LINK=1)

    target_include_directories (bv_plugin INTERFACE 
        "${CMAKE_CURRENT_LIST_DIR}/../third_party/ableton-link/include/ableton"
        "${CMAKE_CURRENT_LIST_DIR}/../third_party/ableton-link/include/ableton/platforms")

endif()

#

if (${BV_USE_MTS_ESP})

    message (STATUS "Configuring MTS-ESP...")

    target_link_libraries (bv_midi INTERFACE MTS-ESP)

endif()

#

if (NOT DEFINED BV_IGNORE_VDSP)
    set (BV_IGNORE_VDSP FALSE)
endif()

if (APPLE AND NOT ${BV_IGNORE_VDSP})
    message (STATUS "Configuring vDSP for vecops...")
    target_compile_definitions (bv_core INTERFACE JUCE_USE_VDSP_FRAMEWORK=1 BV_USE_VDSP=1)
elseif (NOT ${BV_IGNORE_MIPP})
    message (STATUS "Configuring MIPP for vecops...")
    target_link_libraries (bv_core INTERFACE MIPP)
else()
    message (STATUS "Using fallback vecops implementations...")
endif()
