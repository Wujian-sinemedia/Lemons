juce_add_modules (bv_core
                  bv_dsp
                  bv_midi
                  bv_mobile_utils
                  bv_plugin
                  bv_networking
                  )

add_subdirectory (GUI)


if (${BV_USE_ABLETON_LINK})

    message (STATUS "Configuring Ableton Link...")

    include (../third_party/ableton-link/AbletonLinkConfig.cmake)

    target_link_libraries (bv_plugin INTERFACE Ableton::Link)

    target_compile_definitions (bv_plugin INTERFACE BV_USE_ABLETON_LINK=1)

    target_include_directories (bv_plugin INTERFACE 
        "${CMAKE_CURRENT_LIST_DIR}/../third_party/ableton-link/include/ableton"
        "${CMAKE_CURRENT_LIST_DIR}/../third_party/ableton-link/include/ableton/platforms")

endif()


if (${BV_USE_MTS_ESP})

    message (STATUS "Configuring MTS-ESP...")

    target_link_libraries (bv_midi INTERFACE MTS-ESP)

endif()