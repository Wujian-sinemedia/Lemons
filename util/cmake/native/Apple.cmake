execute_process (
      COMMAND uname -m
      RESULT_VARIABLE result
      OUTPUT_VARIABLE osx_native_architecture
      OUTPUT_STRIP_TRAILING_WHITESPACE)

if ("${CMAKE_GENERATOR}" MATCHES "Xcode" AND "${osx_native_architecture}" MATCHES "arm64" AND NOT "${CMAKE_SYSTEM_NAME}" MATCHES "iOS")
    set (CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "")
endif()


if ("${CMAKE_SYSTEM_NAME}" MATCHES "iOS")

    set (CMAKE_OSX_DEPLOYMENT_TARGET "9.3" CACHE STRING "" FORCE)
    set (CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer" CACHE INTERNAL "")

else()

    set (CMAKE_OSX_DEPLOYMENT_TARGET "10.11" CACHE STRING "" FORCE)
    set (MAC_SDK_VERSION "10.13" CACHE STRING "")

    set (MAC_SDK_DIR "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX${MAC_SDK_VERSION}.sdk")

    if (IS_DIRECTORY ${MAC_SDK_DIR})
        set (CMAKE_OSX_SYSROOT ${MAC_SDK_DIR})
    endif()

endif()


#####


function (_lemons_set_default_macos_options target)
    
    set_target_properties (${target} PROPERTIES XCODE_ATTRIBUTE_ENABLE_HARDENED_RUNTIME YES)

    if (${CMAKE_SYSTEM_NAME} MATCHES "iOS")
        set_target_properties (${target} PROPERTIES
                ARCHIVE_OUTPUT_DIRECTORY "./"
                XCODE_ATTRIBUTE_INSTALL_PATH "$(LOCAL_APPS_DIR)"
                XCODE_ATTRIBUTE_SKIP_INSTALL "NO")
    endif()

    set_target_properties  (${target} PROPERTIES XCODE_ATTRIBUTE_MACOSX_DEPLOYMENT_TARGET ${CMAKE_OSX_DEPLOYMENT_TARGET})
    target_compile_options (${target} PUBLIC "-mmacosx-version-min=${CMAKE_OSX_DEPLOYMENT_TARGET}")
    target_link_options    (${target} PUBLIC "-mmacosx-version-min=${CMAKE_OSX_DEPLOYMENT_TARGET}")

endfunction()
