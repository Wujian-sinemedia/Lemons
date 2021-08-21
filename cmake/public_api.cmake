function (bv_configure_juce_plugin)

    _bv_configure_juce_target (${ARGN})

    _bv_plugin_target_postconfig (${bv_targetname})
endfunction()

#

function (bv_configure_juce_app)

    _bv_configure_juce_target (${ARGN})

    target_link_libraries (${bv_targetname} PUBLIC ${BV_APP_ONLY_MODULES})

    _bv_configure_target_install (${bv_targetname} true)
endfunction()

#

function (bv_add_juce_modules dir)
    _bv_add_juce_modules_internal (${dir} dummylist)
endfunction()
