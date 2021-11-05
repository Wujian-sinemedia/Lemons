include_guard (GLOBAL)

#

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
    	message (AUTHOR_WARNING "You're not building for x86_64, which will cause linker errors with AAX targets! Enable universal binaries to build for AAX.")
    	return()
    endif()

    find_program (XCODE_BUILD xcodebuild)

    if (NOT XCODE_BUILD)
    	return()
    endif()

    add_custom_target (AAXSDK
                       COMMAND "${XCODE_BUILD}" "-scheme" "AAXLibrary_libcpp" "ONLY_ACTIVE_ARCH=NO" "ARCHS=x86_64" "-configuration" "$<COMMAND_CONFIG:$<CONFIG>>" "build"
                       COMMAND_EXPAND_LISTS VERBATIM
                       WORKING_DIRECTORY "${LEMONS_AAX_SDK_PATH}/Libs/AAXLibrary/MacBuild"
                       COMMENT "Building AAX SDK..."
                       COMMAND_ECHO STDOUT)

    set_target_properties (AAXSDK PROPERTIES OSX_ARCHITECTURES x86_64)

elseif (WIN32)

    find_program (MS_BUILD msbuild)

    if (NOT MS_BUILD)
    	return()
    endif()

    add_custom_target (AAXSDK
                       COMMAND "${MS_BUILD}" "AAX_SDK.sln" "-p:Configuration=$<COMMAND_CONFIG:$<CONFIG>>"
                       COMMAND_EXPAND_LISTS VERBATIM
                       WORKING_DIRECTORY "${LEMONS_AAX_SDK_PATH}/msvc"
                       COMMENT "Building AAX SDK..."
                       COMMAND_ECHO STDOUT)
endif()


if (NOT TARGET AAXSDK)
	return()
endif()


include (LemonsJuceUtilities)
juce_set_aax_sdk_path ("${LEMONS_AAX_SDK_PATH}")



########################################################################


function (lemons_configure_aax_plugin_signing)

    set (options "")
    set (oneValueArgs TARGET ACCOUNT GUID SIGNID KEYFILE KEYPASSWORD)
    set (multiValueArgs "")

    cmake_parse_arguments (LEMONS_AAX "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (NOT LEMONS_AAX_TARGET)
        message (FATAL_ERROR "Target name not specified in call to ${CMAKE_CURRENT_FUNCTION}!")
        return()
    endif()

    if (NOT LEMONS_AAX_ACCOUNT)
        message (WARNING "ACCOUNT must be specified in order to enable AAX signing!")
        return()
    endif()

    if (NOT LEMONS_AAX_GUID)
        message (WARNING "GUID must be specified in order to enable AAX signing!")
        return()
    endif()

    find_program (WRAPTOOL_PROGRAM wraptool)

    if (NOT WRAPTOOL_PROGRAM)
        message (WARNING "wraptool cannot be found, AAX signing disabled!")
        return()
    endif()

    if (APPLE)
        if (NOT LEMONS_AAX_SIGNID)
            message (WARNING "SIGNID is required to enable AAX signing on Mac!")
            return()
        endif()

        add_custom_command (TARGET ${LEMONS_AAX_TARGET} POST_BUILD VERBATIM
                            COMMAND "${WRAPTOOL_PROGRAM}" 
                            ARGS sign --verbose --dsig1-compat off --account ${LEMONS_AAX_ACCOUNT} --wcguid ${LEMONS_AAX_GUID} --signid ${LEMONS_AAX_SIGNID} --in $<TARGET_PROPERTY:${aaxTarget},JUCE_PLUGIN_ARTEFACT_FILE> --out $<TARGET_PROPERTY:${aaxTarget},JUCE_PLUGIN_ARTEFACT_FILE>
                            COMMENT "Signing AAX...")
    elseif (WIN32)

        if (NOT LEMONS_AAX_KEYFILE)
            message (WARNING "KEYFILE is required to enable AAX signing on Windows!")
            return()
        endif()

        if (NOT LEMONS_AAX_KEYPASSWORD)
            message (WARNING "KEYPASSWORD is required to enable AAX signing on Windows!")
            return()
        endif()

        add_custom_command (TARGET ${LEMONS_AAX_TARGET} POST_BUILD VERBATIM
                            COMMAND "${WRAPTOOL_PROGRAM}" 
                            ARGS sign --verbose --dsig1-compat off --account ${LEMONS_AAX_ACCOUNT} --keyfile "${LEMONS_AAX_KEYFILE}" --keypassword ${LEMONS_AAX_KEYPASSWORD} --wcguid ${LEMONS_AAX_GUID} --in $<TARGET_PROPERTY:${aaxTarget},JUCE_PLUGIN_ARTEFACT_FILE> --out $<TARGET_PROPERTY:${aaxTarget},JUCE_PLUGIN_ARTEFACT_FILE>
                            COMMENT "Signing AAX...")
    endif()

endfunction()

#

function (lemons_configure_aax_plugin)

    set (options SIGN)
    set (oneValueArgs TARGET PAGETABLE_FILE)
    set (multiValueArgs "")

    cmake_parse_arguments (LEMONS_AAX "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (NOT LEMONS_AAX_TARGET)
        message (FATAL_ERROR "Target name not specified in call to ${CMAKE_CURRENT_FUNCTION}!")
        return()
    endif()

    if (NOT TARGET ${LEMONS_AAX_TARGET})
        message (WARNING "AAX target does not exist!")
        return()
    endif()

    if (NOT TARGET AAXSDK)
        message (FATAL_ERROR "AAX plugin target created, but AAXSDK target doesn't exist!")
        return()
    endif()

    set_target_properties (${LEMONS_AAX_TARGET} PROPERTIES OSX_ARCHITECTURES x86_64)

    add_dependencies (${LEMONS_AAX_TARGET} AAXSDK)

    if (LEMONS_AAX_PAGETABLE_FILE)
        target_compile_definitions (${LEMONS_AAX_TARGET} PUBLIC "JucePlugin_AAXPageTableFile=\"${LEMONS_AAX_PAGETABLE_FILE}\"")

        if (WIN32)
            # On Windows, pagetable files need a special post-build copy step to be included in the binary correctly
            add_custom_command (TARGET ${LEMONS_AAX_TARGET} POST_BUILD VERBATIM
                                COMMAND "${CMAKE_COMMAND}" ARGS -E copy "${LEMONS_AAX_PAGETABLE_FILE}" "$<TARGET_PROPERTY:${LEMONS_AAX_TARGET},JUCE_PLUGIN_ARTEFACT_FILE>/Contents/Resources"
                                COMMENT "Copying AAX pagetable into AAX binary...")
        endif()
    endif()

    if (LEMONS_AAX_SIGN)
        lemons_configure_aax_plugin_signing (${ARGN})
    endif()
endfunction()
