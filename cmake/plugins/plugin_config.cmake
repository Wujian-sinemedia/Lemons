function (_lemons_configure_juce_aax origTarget pagetableFile)

    set (aaxTarget ${origTarget}_AAX)

    if (NOT TARGET ${aaxTarget})
        return()
    endif()

    set_target_properties (${aaxTarget} PROPERTIES OSX_ARCHITECTURES x86_64)

    if (pagetableFile)
        target_compile_definitions (${aaxTarget} PUBLIC JucePlugin_AAXPageTableFile=${pagetableFile})
    endif()
endfunction()

###

function (_lemons_configure_juce_lv2 origTarget)

    set (LV2target ${origTarget}_LV2)

    if (NOT TARGET ${LV2target})
        return()
    endif()

    get_target_property (BundleID ${origTarget} JUCE_BUNDLE_ID)

    if (NOT BundleID)
        message (WARNING "Unspecified bundle ID for ${origTarget}!")
        return()
    endif()

    set_target_properties (${LV2target} PROPERTIES JUCE_LV2_URI ${BundleID})

endfunction()

###

function (lemons_configure_juce_plugin)

    _lemons_configure_juce_target (${ARGN})

    target_link_libraries (${lemons_targetname} PUBLIC ${LEMONS_PLUGIN_ONLY_MODULES})

    target_compile_definitions (${lemons_targetname} PUBLIC
            JUCE_MICROPHONE_PERMISSION_ENABLED=1
            JUCE_USE_CUSTOM_PLUGIN_STANDALONE_APP=0)

    _lemons_configure_product_deploy (${lemons_targetname} TRUE)

    set (plugins_target "ALL_PLUGINS")

    if (NOT TARGET ${plugins_target})
        add_custom_target (${plugins_target})
    endif()

    add_dependencies (${plugins_target} "${lemons_targetname}_All")

    set (stdaln_target "${lemons_targetname}_Standalone")

    if (TARGET ${stdaln_target})
        set (app_target "ALL_APPS")

        if (NOT TARGET ${app_target})
            add_custom_target (${app_target})
        endif()

        add_dependencies (${app_target} ${stdaln_target})
    endif()

endfunction()
