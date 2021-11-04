include_guard (GLOBAL)

#

function (_lemons_configure_aax_sdk_target)
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
endfunction()

_lemons_configure_aax_sdk_target()

if (TARGET AAXSDK)
    option (LEMONS_SIGN_AAX "Digitally sign the AAX binary after the build" OFF)

    include (LemonsJuceUtilities)
    juce_set_aax_sdk_path ("${LEMONS_AAX_SDK_PATH}")
endif()


########################################################################


function (_lemons_configure_juce_aax origTarget pagetableFile)

    set (aaxTarget ${origTarget}_AAX)

    if (NOT TARGET ${aaxTarget})
        return()
    endif()

    if (NOT TARGET AAXSDK)
        message (FATAL_ERROR "AAX plugin target created, but AAXSDK target doesn't exist!")
        return()
    endif()

    set_target_properties (${aaxTarget} PROPERTIES OSX_ARCHITECTURES x86_64)

    add_dependencies (${aaxTarget} AAXSDK)

    if (pagetableFile)
        target_compile_definitions (${aaxTarget} PUBLIC "JucePlugin_AAXPageTableFile=\"${pagetableFile}\"")

        if (WIN32)
            # On Windows, pagetable files need a special post-build copy step to be included in the binary correctly
            add_custom_command (TARGET ${aaxTarget} POST_BUILD VERBATIM
                                COMMAND "${CMAKE_COMMAND}" ARGS -E copy "${pagetableFile}" "$<TARGET_PROPERTY:${aaxTarget},JUCE_PLUGIN_ARTEFACT_FILE>/Contents/Resources"
                                COMMENT "Copying AAX pagetable into AAX binary...")
        endif()
    endif()

    if (LEMONS_SIGN_AAX)
        find_program (WRAPTOOL_PROGRAM wraptool)

        if (NOT WRAPTOOL_PROGRAM)
            message (WARNING "wraptool cannot be found, AAX signing disabled!")
            return()
        endif()

        if (APPLE)

            add_custom_command (TARGET ${aaxTarget} POST_BUILD 
                                COMMAND "${WRAPTOOL_PROGRAM}" ARGS sign --verbose --dsig1-compat off --account $AAX_DSIG_ACCOUNT --wcguid ${!AAX_WCGUID_NM} --signid "$AAX_DSIG_ID" --in $<TARGET_PROPERTY:${aaxTarget},JUCE_PLUGIN_ARTEFACT_FILE> --out $<TARGET_PROPERTY:${aaxTarget},JUCE_PLUGIN_ARTEFACT_FILE>
                                VERBATIM COMMENT "Signing AAX...")
        elseif (WIN32)

            add_custom_command (TARGET ${aaxTarget} POST_BUILD
                                COMMAND "${WRAPTOOL_PROGRAM}" ARGS sign --verbose --dsig1-compat off --account %AAX_DSIG_ACCOUNT% --keyfile "%AAX_DSIG_KEYFILE%" --keypassword %AAX_DSIG_KEYFILE_PASS% --wcguid %WCGUID% --in $<TARGET_PROPERTY:${aaxTarget},JUCE_PLUGIN_ARTEFACT_FILE> --out $<TARGET_PROPERTY:${aaxTarget},JUCE_PLUGIN_ARTEFACT_FILE>
                                VERBATIM COMMENT "Signing AAX...")
        endif()
    endif()

endfunction()
