if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS")

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

# No ZERO_CHECK target on XCode (it helps bust cache for cmake)
if ("${CMAKE_GENERATOR}" STREQUAL "Xcode")
     set (CMAKE_SUPPRESS_REGENERATION FALSE)
endif()


#####


function (_bv_set_default_macos_options target)
    
    macro (_bv_configure_macos_version target version)
        set_target_properties  (${target} PROPERTIES XCODE_ATTRIBUTE_MACOSX_DEPLOYMENT_TARGET ${version})
        target_compile_options (${target} PUBLIC "-mmacosx-version-min=${version}")
        target_link_options    (${target} PUBLIC "-mmacosx-version-min=${version}")
    endmacro()

    #

    set_target_properties (${target} PROPERTIES 
                            JUCE_BUNDLE_ID "com.bv.${target}" 
                            XCODE_ATTRIBUTE_ENABLE_HARDENED_RUNTIME YES)

    if (${CMAKE_SYSTEM_NAME} STREQUAL "iOS")
        set_target_properties (${target} PROPERTIES
                ARCHIVE_OUTPUT_DIRECTORY "./"
                XCODE_ATTRIBUTE_INSTALL_PATH "$(LOCAL_APPS_DIR)"
                XCODE_ATTRIBUTE_SKIP_INSTALL "NO")

        _bv_configure_macos_version (${target} "9.3")
    elseif (TARGET ${target}_AUv3)
        _bv_configure_macos_version (${target} "10.11")
    else()
        _bv_configure_macos_version (${target} "10.9")
    endif()

endfunction()
