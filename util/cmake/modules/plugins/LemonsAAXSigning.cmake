include_guard (GLOBAL)


function (lemons_set_aax_signing_settings)

    set (oneValueArgs ACCOUNT SIGNID KEYFILE KEYPASSWORD)

    cmake_parse_arguments (LEMONS_AAX "FORCE" "${oneValueArgs}" "" ${ARGN})

    lemons_check_for_unparsed_args (LEMONS_AAX)

    if (LEMONS_AAX_ACCOUNT)
        if (LEMONS_AAX_FORCE)
            set (LEMONS_AAX_ACCOUNT "${LEMONS_AAX_ACCOUNT}" CACHE STRING "Account ID for AAX plugin signing" FORCE)
        else()
            set (LEMONS_AAX_ACCOUNT "${LEMONS_AAX_ACCOUNT}" CACHE STRING "Account ID for AAX plugin signing")
        endif()
    endif()

    if (LEMONS_AAX_SIGNID)
        if (LEMONS_AAX_FORCE)
            set (LEMONS_AAX_SIGNID "${LEMONS_AAX_SIGNID}" CACHE STRING "SignID for AAX signing on Mac" FORCE)
        else()
            set (LEMONS_AAX_SIGNID "${LEMONS_AAX_SIGNID}" CACHE STRING "SignID for AAX signing on Mac")
        endif()
    endif()

    if (LEMONS_AAX_KEYFILE)
        if (LEMONS_AAX_FORCE)
            set (LEMONS_AAX_KEYFILE "${LEMONS_AAX_KEYFILE}" CACHE PATH "Keyfile for AAX signing on Windows" FORCE)
        else()
            set (LEMONS_AAX_KEYFILE "${LEMONS_AAX_KEYFILE}" CACHE PATH "Keyfile for AAX signing on Windows")
        endif()
    endif()

    if (LEMONS_AAX_KEYPASSWORD)
        if (LEMONS_AAX_FORCE)
            set (LEMONS_AAX_KEYPASSWORD "${LEMONS_AAX_KEYPASSWORD}" CACHE STRING "Keypassword for AAX signing on Windows" FORCE)
        else()
            set (LEMONS_AAX_KEYPASSWORD "${LEMONS_AAX_KEYPASSWORD}" CACHE STRING "Keypassword for AAX signing on Windows")
        endif()
    endif()
endfunction()

#

function (lemons_configure_aax_plugin_signing)

    set (oneValueArgs TARGET GUID ACCOUNT SIGNID KEYFILE KEYPASSWORD)

    cmake_parse_arguments (LEMONS_AAX "" "${oneValueArgs}" "" ${ARGN})

    lemons_require_function_arguments (LEMONS_AAX TARGET GUID ACCOUNT)
    lemons_check_for_unparsed_args (LEMONS_AAX)

    find_program (WRAPTOOL_PROGRAM wraptool)

    if (NOT WRAPTOOL_PROGRAM)
        message (WARNING "wraptool cannot be found, AAX signing disabled!")
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

    message (DEBUG "Configured AAX plugin signing!")
endfunction()
