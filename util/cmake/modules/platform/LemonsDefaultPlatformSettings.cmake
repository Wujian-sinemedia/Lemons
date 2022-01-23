#[[
Default settings and configuration for a CMake build on the OS you're currently running.

## Include-time actions:
Sets appropriate platform-specific settings for the current build.

## Function:

### lemons_set_default_macos_options
```
lemons_set_default_macos_options (<target>)
```
Sets default Apple-only options for the given target.
Does nothing on non-Apple platforms.


## Note

This module is included by Lemons by default, when Lemons is added as a subdirectory.

]]


include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

if (APPLE)
    if (IOS)
        include ("${CMAKE_CURRENT_LIST_DIR}/scripts/lemons_ios_settings.cmake")
    else()
        include ("${CMAKE_CURRENT_LIST_DIR}/scripts/lemons_macos_settings.cmake")
    endif()
else()
    set (CMAKE_INSTALL_RPATH $ORIGIN CACHE INTERNAL "")

    if (WIN32)
        include ("${CMAKE_CURRENT_LIST_DIR}/scripts/lemons_windows_settings.cmake")
    else()
        include ("${CMAKE_CURRENT_LIST_DIR}/scripts/lemons_linux_settings.cmake")
        include (LinuxLSBInfo)
    endif()
endif()

#

function (lemons_set_default_macos_options target)

    if (NOT APPLE)
        return()
    endif()

    target_compile_definitions (${target} PRIVATE JUCE_USE_VDSP_FRAMEWORK=1)

    set_target_properties (${target} PROPERTIES XCODE_ATTRIBUTE_ENABLE_HARDENED_RUNTIME YES)

    if (IOS)
        set_target_properties (${target} PROPERTIES
                ARCHIVE_OUTPUT_DIRECTORY "./"
                XCODE_ATTRIBUTE_INSTALL_PATH "$(LOCAL_APPS_DIR)"
                XCODE_ATTRIBUTE_SKIP_INSTALL "NO")
    endif()

    set_target_properties  (${target} PROPERTIES XCODE_ATTRIBUTE_MACOSX_DEPLOYMENT_TARGET ${CMAKE_OSX_DEPLOYMENT_TARGET})
    target_compile_options (${target} PRIVATE "-mmacosx-version-min=${CMAKE_OSX_DEPLOYMENT_TARGET}")
    target_link_options    (${target} PRIVATE "-mmacosx-version-min=${CMAKE_OSX_DEPLOYMENT_TARGET}")

endfunction()
