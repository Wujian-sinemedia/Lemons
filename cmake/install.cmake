include (CPack)


function (_bv_configure_target_install target)

	if (NOT TARGET ${target})
		return()
	endif()

	install (TARGETS ${target}
             DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/deploy
             COMPONENT ${target})

endfunction()

#

function (_bv_configure_target_workspace_deploy target productName)

    if (NOT TARGET ${target})
        return()
    endif()

    if (NOT DEFINED ENV{BV_WORKSPACE_BUILD})
        return()
    endif()

    if (NOT $ENV{BV_WORKSPACE_BUILD})
        return()
    endif()

    install (TARGETS ${target}
             DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../deploy/${productName}
             COMPONENT ${target})

endfunction()

#

function (_bv_add_target_install_command target)

    if (NOT TARGET ${target})
        return()
    endif()

    add_custom_command (TARGET ${target} POST_BUILD
            COMMAND "${CMAKE_COMMAND}"
            "-DCMAKE_INSTALL_CONFIG_NAME=$<CONFIG>"
            "-DCMAKE_INSTALL_COMPONENT=${target}"
            "-P" "${CMAKE_CURRENT_BINARY_DIR}/cmake_install.cmake")

endfunction()
