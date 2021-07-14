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
