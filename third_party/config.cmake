function (_configure_mipp target)
	if (NOT DEFINED BV_IGNORE_MIPP)
        set (BV_IGNORE_MIPP FALSE)
    endif()

    if (NOT ${BV_IGNORE_MIPP})
        message (STATUS "Configuring MIPP for vecops...")
        target_compile_definitions (${target} PUBLIC MIPP_ENABLE_BACKTRACE BV_USE_MIPP=1)
        target_include_directories (${target} PUBLIC "${bv_sharedcode_dir}/third_party/MIPP/src" "MIPP")
    endif()
endfunction()

#

function (_configure_mts_esp target)
    if (NOT DEFINED BV_USE_MTS_ESP)
        set (BV_USE_MTS_ESP FALSE)
    endif()

    if (${BV_USE_MTS_ESP})
        message (STATUS "Configuring MTS-ESP...")
        target_include_directories (${target} PUBLIC "${bv_sharedcode_dir}/third_party/MTS-ESP/Client" "MTS-ESP")
        target_compile_definitions (${target} PUBLIC BV_USE_MTS_ESP=1)
    endif()
endfunction()