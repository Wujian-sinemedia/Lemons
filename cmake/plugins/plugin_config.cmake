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

###

function (bv_configure_juce_plugin)

    _bv_configure_juce_target (${ARGN})

    target_link_libraries (${bv_targetname} PUBLIC ${BV_PLUGIN_ONLY_MODULES})

    target_compile_definitions (${bv_targetname} PUBLIC
            JUCE_MICROPHONE_PERMISSION_ENABLED=1
            JUCE_USE_CUSTOM_PLUGIN_STANDALONE_APP=0)

    _bv_configure_product_install (${bv_targetname} TRUE)

    set (plugins_target "ALL_PLUGINS")

    if (NOT TARGET ${plugins_target})
        add_custom_target (${plugins_target})
    endif()

    add_dependencies (${plugins_target} "${bv_targetname}_All")

endfunction()
