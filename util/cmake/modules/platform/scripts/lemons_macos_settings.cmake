include_guard (GLOBAL)

set (CMAKE_OSX_DEPLOYMENT_TARGET "10.11" CACHE INTERNAL "")

set (LEMONS_MAC_SDK_VERSION "10.13" CACHE STRING "")
mark_as_advanced (FORCE LEMONS_MAC_SDK_VERSION)

set (MAC_SDK_DIR "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX${LEMONS_MAC_SDK_VERSION}.sdk")

if (IS_DIRECTORY ${MAC_SDK_DIR})
    set (CMAKE_OSX_SYSROOT ${MAC_SDK_DIR} CACHE INTERNAL "")
endif()

option (LEMONS_MAC_UNIVERSAL_BINARY "Builds for x86_64 and arm64" ON)

if (LEMONS_MAC_UNIVERSAL_BINARY)
    set (CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE INTERNAL "")
endif()
