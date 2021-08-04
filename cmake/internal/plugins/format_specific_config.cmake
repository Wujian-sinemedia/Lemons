function (_bv_configure_juce_aax origTarget pagetableFile)

    set (aaxTarget ${origTarget}_AAX)

    if (NOT TARGET ${aaxTarget})
        return()
    endif()

    set_target_properties (${aaxTarget} PROPERTIES OSX_ARCHITECTURES x86_64)

    if (NOT "${pagetableFile}" STREQUAL "")
        target_compile_definitions (${aaxTarget} PUBLIC JucePlugin_AAXPageTableFile="${pagetableFile}")
    endif()
endfunction()

###

function (_bv_configure_juce_lv2 origTarget)

    set (LV2target ${origTarget}_LV2)

    if (NOT TARGET ${LV2target})
        return()
    endif()

    set_target_properties (${LV2target} PROPERTIES JUCE_LV2_URI https://github.com/benthevining/${CMAKE_PROJECT_NAME})

endfunction()