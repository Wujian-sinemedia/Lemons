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

    get_target_property (Company ${LV2target} JUCE_COMPANY_NAME)

    if (NOT Company)
        message (WARNING "Unspecified company for LV2 target!")
        set (Company "YourCompany")
    endif()

    get_target_property (Product ${LV2target} JUCE_PRODUCT_NAME)

    if (NOT Product)
        message (WARNING "Unspecified product name for LV2 target!")
        set (Product "YourProduct")
    endif()

    set_target_properties (${LV2target} PROPERTIES JUCE_LV2_URI "${Company}.${Product}")

endfunction()