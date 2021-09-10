function (bv_configure_juce_app)

    _bv_configure_juce_target (${ARGN})

    target_link_libraries (${bv_targetname} PUBLIC ${BV_APP_ONLY_MODULES})

    _bv_configure_product_install (${bv_targetname} FALSE)

    set (app_target "ALL_APPS")

    if (NOT TARGET ${app_target})
        add_custom_target (${app_target})
    endif()

    add_dependencies (${app_target} ${bv_targetname})

endfunction()
