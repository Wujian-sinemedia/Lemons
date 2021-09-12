function (_bv_configure_vecops target)

	if (APPLE)
		target_compile_definitions (${target} PUBLIC JUCE_USE_VDSP_FRAMEWORK=1 BV_USE_VDSP=1)
		return()
	endif()

	if (TARGET IntelIPP)
		target_link_libraries (${target} PUBLIC IntelIPP)
		return()
	endif()

endfunction()
