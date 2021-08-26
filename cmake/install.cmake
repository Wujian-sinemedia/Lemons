include (CPack)


function (_bv_configure_target_install target isApp)

	if (NOT TARGET ${target})
		return()
	endif()

	set (targetDir ${CMAKE_CURRENT_BINARY_DIR}/deploy)

	install (TARGETS ${target}
             DESTINATION ${targetDir}
             COMPONENT ${target})

    add_custom_command (TARGET ${target} POST_BUILD
            COMMAND "${CMAKE_COMMAND}"
            "-DCMAKE_INSTALL_CONFIG_NAME=$<CONFIG>"
            "-DCMAKE_INSTALL_COMPONENT=${target}"
            "-P" "${CMAKE_CURRENT_BINARY_DIR}/cmake_install.cmake")

endfunction()