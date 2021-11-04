if (NOT LEMONS_AAX_SDK_PATH)
	return()
endif()

if (NOT IS_DIRECTORY "${LEMONS_AAX_SDK_PATH}")
	return()
endif()

if (IOS OR NOT (APPLE OR WIN32))
	return()
endif()


if (APPLE)
    if (NOT "x86_64" IN_LIST CMAKE_OSX_ARCHITECTURES)
    	message (WARNING "You're not building for x86_64, which will cause linker errors with AAX targets! Enable universal binaries to build for AAX.")
    	return()
    endif()

    find_program (XCODE_BUILD xcodebuild)

    if (XCODE_BUILD)
        add_custom_target (AAXSDK
                           COMMAND "${XCODE_BUILD}" "-scheme" "AAXLibrary_libcpp" "ONLY_ACTIVE_ARCH=NO" "ARCHS=x86_64" "-configuration" "$<COMMAND_CONFIG:$<CONFIG>>" "build"
                           COMMAND_EXPAND_LISTS VERBATIM
                           WORKING_DIRECTORY "${LEMONS_AAX_SDK_PATH}/Libs/AAXLibrary/MacBuild"
                           COMMENT "Building AAX SDK..."
                           COMMAND_ECHO STDOUT)

        set_target_properties (AAXSDK PROPERTIES OSX_ARCHITECTURES x86_64)
    endif()

elseif (WIN32)

    find_program (MS_BUILD msbuild)

    if (MS_BUILD)
        add_custom_target (AAXSDK
                           COMMAND "${MS_BUILD}" "AAX_SDK.sln" "-p:Configuration=$<COMMAND_CONFIG:$<CONFIG>>"
                           COMMAND_EXPAND_LISTS VERBATIM
                           WORKING_DIRECTORY "${LEMONS_AAX_SDK_PATH}/msvc"
                           COMMENT "Building AAX SDK..."
                           COMMAND_ECHO STDOUT)
    endif()
endif()


if (TARGET AAXSDK)
    option (LEMONS_SIGN_AAX "Digitally sign the AAX binary after the build" OFF)
endif()
