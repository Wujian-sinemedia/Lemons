
function (_bv_configure_product_deploy target isPlugin)

    function (_bv_configure_target_deploy target productName)

        if (NOT TARGET ${target})
            return()
        endif()

        if (${BV_WORKSPACE_BUILD})
            set (dest_dir ${CMAKE_CURRENT_BINARY_DIR}/../deploy/${productName})
        else()
            set (dest_dir ${CMAKE_CURRENT_BINARY_DIR}/deploy)
        endif()

        install (TARGETS ${target}
                 DESTINATION ${dest_dir}
                 COMPONENT ${target})

        add_custom_command (TARGET ${target} POST_BUILD
                COMMAND "${CMAKE_COMMAND}"
                "-DCMAKE_INSTALL_CONFIG_NAME=$<CONFIG>"
                "-DCMAKE_INSTALL_COMPONENT=${target}"
                "-P" "${CMAKE_CURRENT_BINARY_DIR}/cmake_install.cmake")
    endfunction()

    #

    if (NOT TARGET ${target})
        return()
    endif()

    if (${isPlugin})
        get_target_property (formats ${target} JUCE_FORMATS)

        if (formats)
            foreach (format ${formats})
                _bv_configure_target_deploy ("${target}_${format}" ${target})
            endforeach()
        endif()
    endif()

    _bv_configure_target_deploy (${target} ${target})

endfunction()
