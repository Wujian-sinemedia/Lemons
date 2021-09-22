option (LEMIONS_COPY_TO_DEPLOY_FOLDER "Copies each product's install components to /Builds/deploy/<ProductName>" ON)

#

function (_lemons_configure_product_deploy target isPlugin)

    if (NOT LEMONS_COPY_TO_DEPLOY_FOLDER)
        return()
    endif()

    #

    function (_lemons_configure_target_deploy target productName)

        if (NOT TARGET ${target})
            return()
        endif()

        if (LEMONS_WORKSPACE_BUILD)
            set (dest_dir ${CMAKE_CURRENT_BINARY_DIR}/../../deploy/${productName})
        else()
            set (dest_dir ${CMAKE_CURRENT_BINARY_DIR}/deploy)
        endif()

        add_custom_command (TARGET ${target} POST_BUILD
                            COMMAND "${CMAKE_COMMAND}"
                            "-E" "copy" "$<TARGET_FILE:${target}>" "${dest_dir}/$<TARGET_FILE_NAME:${target}>")
    endfunction()

    #

    if (NOT TARGET ${target})
        return()
    endif()

    if (isPlugin)
        get_target_property (formats ${target} JUCE_FORMATS)

        if (formats)
            foreach (format ${formats})
                _lemons_configure_target_deploy ("${target}_${format}" ${target})
            endforeach()
        endif()
    endif()

    _lemons_configure_target_deploy (${target} ${target})

endfunction()
