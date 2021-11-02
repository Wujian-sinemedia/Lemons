set (CMAKE_OSX_DEPLOYMENT_TARGET "9.3" CACHE INTERNAL "")

set (CMAKE_XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH NO CACHE INTERNAL "")

enable_language (OBJC)
enable_language (OBJCXX)


option (LEMONS_IOS_SIMULATOR "Build for an iOS simulator, rather than a real device" ON)

if (LEMONS_IOS_SIMULATOR)
    # Options for building for a simulator

    set (CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "\"iPhone Developer\"" CACHE INTERNAL "")

    set (IOS_PLATFORM_LOCATION "iPhoneSimulator.platform" CACHE INTERNAL "")
    set (CMAKE_XCODE_EFFECTIVE_PLATFORMS "-iphonesimulator" CACHE INTERNAL "")

    set (CMAKE_OSX_ARCHITECTURES "i386;x86_64" CACHE INTERNAL "")

else()
    # Options for building for a real device

    set (CMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "" CACHE STRING "")

    set (IOS_PLATFORM_LOCATION "iPhoneOS.platform" CACHE INTERNAL "")
    set (CMAKE_XCODE_EFFECTIVE_PLATFORMS "-iphoneos" CACHE INTERNAL "")

    set (CMAKE_OSX_ARCHITECTURES "armv7;armv7s;arm64;i386;x86_64" CACHE INTERNAL "")

endif()
