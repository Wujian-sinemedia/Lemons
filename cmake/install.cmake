include (CPack)


function (_bv_configure_target_install target)

	if (NOT TARGET ${target})
		return()
	endif()

	install (TARGETS ${target}
             DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/deploy
             COMPONENT ${target})

    add_custom_command (TARGET ${target} POST_BUILD
            COMMAND "${CMAKE_COMMAND}"
            "-DCMAKE_INSTALL_CONFIG_NAME=$<CONFIG>"
            "-DCMAKE_INSTALL_COMPONENT=${target}"
            "-P" "${CMAKE_CURRENT_BINARY_DIR}/cmake_install.cmake")

endfunction()

#

function (_bv_configure_target_workspace_deploy target)

    if (NOT TARGET ${target})
        return()
    endif()

    if (NOT DEFINED ENV{BV_WORKSPACE_BUILD})
        return()
    endif()

    if (NOT $ENV{BV_WORKSPACE_BUILD})
        return()
    endif()

    add_custom_command (TARGET ${target} POST_BUILD
            COMMAND "${CMAKE_COMMAND}"
            "-E" "copy_directory" "${CMAKE_CURRENT_BINARY_DIR}/deploy" "${CMAKE_CURRENT_BINARY_DIR}/../deploy/${target}")

endfunction()