function (configure_synth_module target)

	if (NOT DEFINED bvsb_USE_MTS_ESP)
        set (bvsb_USE_MTS_ESP FALSE)
    endif()

    if (${bvsb_USE_MTS_ESP})
    	message (STATUS "Configuring MTS-ESP...")
        target_include_directories (${target} PUBLIC "${bv_sharedcode_dir}/third_party/MTS-ESP/MTS-ESP/Client")
        target_compile_definitions (${target} PUBLIC bvsb_USE_MTS_ESP=1)
        target_link_libraries (${target} PUBLIC bv_synth)
    else()
        target_compile_definitions (${target} PUBLIC bvsb_USE_MTS_ESP=0)
    endif()

endfunction()