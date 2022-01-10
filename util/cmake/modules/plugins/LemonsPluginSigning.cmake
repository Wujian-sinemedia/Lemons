include_guard (GLOBAL)


function (lemons_set_aax_signing_settings)

    set (oneValueArgs ACCOUNT SIGNID KEYFILE KEYPASSWORD)

    cmake_parse_arguments (LEMONS_AAX "" "${oneValueArgs}" "" ${ARGN})

    lemons_check_for_unparsed_args (LEMONS_AAX)

    if (LEMONS_AAX_ACCOUNT)
        set (LEMONS_AAX_ACCOUNT "${LEMONS_AAX_ACCOUNT}" CACHE STRING "Account ID for AAX plugin signing")
    endif()

    if (LEMONS_AAX_SIGNID)
        set (LEMONS_AAX_SIGNID "${LEMONS_AAX_SIGNID}" CACHE STRING "SignID for AAX signing on Mac")
    endif()

    if (LEMONS_AAX_KEYFILE)
        set (LEMONS_AAX_KEYFILE "${LEMONS_AAX_KEYFILE}" CACHE PATH "Keyfile for AAX signing on Windows")
    endif()

    if (LEMONS_AAX_KEYPASSWORD)
        set (LEMONS_AAX_KEYPASSWORD "${LEMONS_AAX_KEYPASSWORD}" CACHE STRING "Keypassword for AAX signing on Windows")
    endif()
endfunction()


function (lemons_configure_aax_plugin_signing)

    set (oneValueArgs TARGET GUID ACCOUNT SIGNID KEYFILE KEYPASSWORD)

    cmake_parse_arguments (LEMONS_AAX "" "${oneValueArgs}" "" ${ARGN})

    lemons_require_function_arguments (LEMONS_AAX TARGET GUID ACCOUNT)
    lemons_check_for_unparsed_args (LEMONS_AAX)

    find_program (WRAPTOOL_PROGRAM wraptool)

    if (NOT WRAPTOOL_PROGRAM)
        message (AUTHOR_WARNING "wraptool cannot be found, AAX signing disabled!")
        return()
    endif()

    if (APPLE)
        lemons_require_function_arguments (LEMONS_AAX SIGNID)
        
        add_custom_command (TARGET ${LEMONS_AAX_TARGET} POST_BUILD VERBATIM COMMAND_EXPAND_LISTS
                            COMMAND "${WRAPTOOL_PROGRAM}" 
                            ARGS sign --verbose --dsig1-compat off --account "${LEMONS_AAX_ACCOUNT}" --wcguid "${LEMONS_AAX_GUID}" --signid "${LEMONS_AAX_SIGNID}" --in "$<TARGET_PROPERTY:${aaxTarget},JUCE_PLUGIN_ARTEFACT_FILE>" --out "$<TARGET_PROPERTY:${aaxTarget},JUCE_PLUGIN_ARTEFACT_FILE>"
                            COMMENT "Signing ${LEMONS_AAX_TARGET}...")
    elseif (WIN32)
        lemons_require_function_arguments (LEMONS_AAX KEYFILE KEYPASSWORD)

        add_custom_command (TARGET ${LEMONS_AAX_TARGET} POST_BUILD VERBATIM COMMAND_EXPAND_LISTS
                            COMMAND "${WRAPTOOL_PROGRAM}" 
                            ARGS sign --verbose --dsig1-compat off --account "${LEMONS_AAX_ACCOUNT}" --keyfile "${LEMONS_AAX_KEYFILE}" --keypassword "${LEMONS_AAX_KEYPASSWORD}" --wcguid "${LEMONS_AAX_GUID}" --in "$<TARGET_PROPERTY:${aaxTarget},JUCE_PLUGIN_ARTEFACT_FILE>" --out "$<TARGET_PROPERTY:${aaxTarget},JUCE_PLUGIN_ARTEFACT_FILE>"
                            COMMENT "Signing ${LEMONS_AAX_TARGET}...")
    endif()
endfunction()

#

function (lemons_configure_plugin_signing)

	set (oneValueArgs TARGET GUID ACCOUNT SIGNID KEYFILE KEYPASSWORD)

    cmake_parse_arguments (LEMONS_SIGN "" "${oneValueArgs}" "" ${ARGN})

    lemons_require_function_arguments (LEMONS_SIGN TARGET)
    lemons_check_for_unparsed_args (LEMONS_SIGN)


    set (aaxTarget "${LEMONS_SIGN_TARGET}_AAX")

    if (TARGET ${aaxTarget} AND LEMONS_SIGN_GUID AND LEMONS_SIGN_ACCOUNT)
    	lemons_configure_aax_plugin_signing (TARGET ${aaxTarget} GUID ${LEMONS_SIGN_GUID} ACCOUNT ${LEMONS_SIGN_ACCOUNT} 
    										 SIGNID ${LEMONS_SIGN_SIGNID} 
    										 KEYFILE ${LEMONS_SIGN_KEYFILE} KEYPASSWORD ${LEMONS_SIGN_KEYPASSWORD})
    endif()


    get_target_property (pluginFormats ${LEMONS_SIGN_TARGET} JUCE_FORMATS)

    if (NOT pluginFormats)
    	message (AUTHOR_WARNING "Error retrieving plugin formats of target ${LEMONS_SIGN_TARGET}!")
    	return()
    endif()


    function (_lemons_config_plugin_format_signing formatTarget)
    	if (APPLE)

    		find_program (CODESIGN codesign)

	        if (NOT CODESIGN)
	            message (AUTHOR_WARNING "Codesign cannot be found, plugin signing cannot be configured!")
	            return()
	        endif()

        	add_custom_command (TARGET ${formatTarget} POST_BUILD VERBATIM COMMAND_EXPAND_LISTS
                            	COMMAND ${CODESIGN} -s - --force
                            	"$<TARGET_BUNDLE_DIR:${formatTarget}>"
                            	COMMENT "Signing ${formatTarget}...")

	        add_custom_command (TARGET ${formatTarget} POST_BUILD VERBATIM COMMAND_EXPAND_LISTS
	                            COMMAND ${CODESIGN} -verify
	                            "$<TARGET_BUNDLE_DIR:${formatTarget}>"
	                            COMMENT "Verifying signing of ${formatTarget}...")
        	
    	elseif (WIN32)

    	else()

    	endif()
    endfunction()


    foreach (pluginFormat ${pluginFormats})

    	set (formatTarget "${LEMONS_SIGN_TARGET}_${pluginFormat}")

    	if (NOT TARGET ${formatTarget})
    		message (AUTHOR_WARNING "Format ${pluginFormat} is in property list for target ${LEMONS_SIGN_TARGET}, but target ${formatTarget} does not exist!")
    		continue()
    	endif()

    	_lemons_config_plugin_format_signing (${formatTarget})
    	
    endforeach()

endfunction()
