include (CPack)


function (_bv_configure_target_install target isApp)

	install (TARGETS ${target}
			LIBRARY DESTINATION "lib"
			BUNDLE DESTINATION "bin"
			RESOURCE DESTINATION "res")

endfunction()