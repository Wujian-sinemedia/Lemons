function (bv_configure_juce_plugin)

    _bv_configure_juce_target (${ARGN})

    target_link_libraries (${bv_targetname} PUBLIC ${BV_PLUGIN_ONLY_MODULES})

    target_compile_definitions (${bv_targetname} PUBLIC
            JUCE_MICROPHONE_PERMISSION_ENABLED=1
            JUCE_USE_CUSTOM_PLUGIN_STANDALONE_APP=0)
endfunction()

#

function (bv_configure_juce_app)

    _bv_configure_juce_target (${ARGN})

    target_link_libraries (${bv_targetname} PUBLIC ${BV_APP_ONLY_MODULES})
endfunction()

#

function (bv_add_juce_modules dir)
    _bv_add_juce_modules_internal (${dir} dummylist)
endfunction()
