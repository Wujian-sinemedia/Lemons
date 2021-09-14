if ("${CMAKE_SYSTEM_NAME}" MATCHES "iOS")

    set (CMAKE_OSX_DEPLOYMENT_TARGET "9.3" CACHE STRING "Minimum OS X deployment version" FORCE)
    set (CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer")

else()

    set (CMAKE_OSX_DEPLOYMENT_TARGET "10.9" CACHE STRING "Minimum OS X deployment version" FORCE)

    set (MAC_SDK_VERSION "10.13" CACHE STRING "")
    set (MAC_SDK_DIR "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX${MAC_SDK_VERSION}.sdk")

    if (IS_DIRECTORY ${MAC_SDK_DIR})
        set (CMAKE_OSX_SYSROOT ${MAC_SDK_DIR})
    endif()

    set (CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE INTERNAL "")

endif()


#####


function (_bv_set_default_macos_options target)
    
    set_target_properties (${target} PROPERTIES XCODE_ATTRIBUTE_ENABLE_HARDENED_RUNTIME YES)

    if (${CMAKE_SYSTEM_NAME} MATCHES "iOS")
        set_target_properties (${target} PROPERTIES
                ARCHIVE_OUTPUT_DIRECTORY "./"
                XCODE_ATTRIBUTE_INSTALL_PATH "$(LOCAL_APPS_DIR)"
                XCODE_ATTRIBUTE_SKIP_INSTALL "NO")

        set (min_macos_version "9.3")
    elseif (TARGET ${target}_AUv3)
        set (min_macos_version "10.11")
    else()
        set (min_macos_version "10.9")
    endif()

    set_target_properties  (${target} PROPERTIES XCODE_ATTRIBUTE_MACOSX_DEPLOYMENT_TARGET ${min_macos_version})
    target_compile_options (${target} PUBLIC "-mmacosx-version-min=${min_macos_version}")
    target_link_options    (${target} PUBLIC "-mmacosx-version-min=${min_macos_version}")

endfunction()
