### Apple vDSP ###
function (_bv_configure_vdsp force_use force_ignore outvar)

	macro (_bv_set_vdsp_state_for_target _use_vdsp_)
		set (${outvar} ${_use_vdsp_} PARENT_SCOPE)
		return()
	endmacro()

	if (${force_use} AND ${force_ignore})
		bv_print_warning ("Warning: force use and force ignore can't both be true - _bv_configure_vdsp")
		_bv_set_vdsp_state_for_target (0)
	endif()

	if (NOT APPLE)
		_bv_set_vdsp_state_for_target (0)
	endif()

	if (${force_use})
		_bv_set_vdsp_state_for_target (1)
	endif()

	if ($CACHE{BV_IGNORE_VDSP} OR ${force_ignore})
		_bv_set_vdsp_state_for_target (0)
	endif()

	_bv_set_vdsp_state_for_target (1)
endfunction()

#

function (_bv_configure_vecops target forceUseVDSP forceIgnoreVDSP)
	_bv_configure_vdsp (${forceUseVDSP} ${forceIgnoreVDSP} useVDSP)
	target_compile_definitions (${target} PUBLIC JUCE_USE_VDSP_FRAMEWORK=${useVDSP} BV_USE_VDSP=${useVDSP})

	if (${useVDSP})
		return()
	endif()

endfunction()
