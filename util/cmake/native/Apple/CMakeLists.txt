if (IOS)
    include (iOS.cmake)
else()
    include (macOS.cmake)
endif()


#####


function (_lemons_set_default_macos_options target)
    
    set_target_properties (${target} PROPERTIES XCODE_ATTRIBUTE_ENABLE_HARDENED_RUNTIME YES)

    if (IOS)
        set_target_properties (${target} PROPERTIES
                ARCHIVE_OUTPUT_DIRECTORY "./"
                XCODE_ATTRIBUTE_INSTALL_PATH "$(LOCAL_APPS_DIR)"
                XCODE_ATTRIBUTE_SKIP_INSTALL "NO")
    endif()

    set_target_properties  (${target} PROPERTIES XCODE_ATTRIBUTE_MACOSX_DEPLOYMENT_TARGET ${CMAKE_OSX_DEPLOYMENT_TARGET})
    target_compile_options (${target} PUBLIC "-mmacosx-version-min=${CMAKE_OSX_DEPLOYMENT_TARGET}")
    target_link_options    (${target} PUBLIC "-mmacosx-version-min=${CMAKE_OSX_DEPLOYMENT_TARGET}")

endfunction()
