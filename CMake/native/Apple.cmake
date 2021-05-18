macro (setDefaultAppleSettings)

    if (${CMAKE_SYSTEM_NAME} STREQUAL "iOS")

        set (CMAKE_OSX_DEPLOYMENT_TARGET 9.3 CACHE STRING "Minimum OS X deployment version" FORCE)
        set (CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer")

    else()

        set (CMAKE_OSX_DEPLOYMENT_TARGET "10.9" CACHE STRING "Minimum OS X deployment version" FORCE)
        set (XCODE_DIR "/Applications/Xcode.app" CACHE STRING "")
        set (MAC_SDK_VERSION "10.13" CACHE STRING "")

        set (MAC_SDK_DIR "${XCODE_DIR}/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs")
        set (MAC_CURRENT_SDK "${MAC_SDK_DIR}/MacOSX${MAC_SDK_VERSION}.sdk")

        if (IS_DIRECTORY ${MAC_CURRENT_SDK})
            message ("Using Mac OSX SDK ${MAC_SDK_VERSION}")
            set (CMAKE_OSX_SYSROOT ${MAC_CURRENT_SDK})
        else()
            message ("Can't find Mac SDK ${MAC_SDK_VERSION}, switching to default instead")
        endif()

        set (CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE INTERNAL "")
    endif()

endmacro()

#

function (set_min_macos_version target version)
    if (APPLE)
        set_target_properties (${target} PROPERTIES XCODE_ATTRIBUTE_MACOSX_DEPLOYMENT_TARGET ${version})
        target_compile_options (${target} PUBLIC "-mmacosx-version-min=${version}")
        target_link_options (${target} PUBLIC "-mmacosx-version-min=${version}")
    endif ()
endfunction()
