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
        endif()
    endif()

    # configure post-build signing...
    if (FALSE)
        find_program (WRAPTOOL_PROGRAM wraptool)

        if (NOT WRAPTOOL_PROGRAM)
            message (WARNING "wraptool cannot be found, AAX signing disabled!")
            return()
        endif()

        if (APPLE)

            add_custom_command (TARGET ${aaxTarget} POST_BUILD 
                                COMMAND "${WRAPTOOL_PROGRAM}" ARGS sign --verbose --dsig1-compat off --account $AAX_DSIG_ACCOUNT --wcguid ${!AAX_WCGUID_NM} --signid "$AAX_DSIG_ID" --in $<TARGET_FILE:${aaxTarget}> --out $<TARGET_FILE:${aaxTarget}>
                                VERBATIM COMMENT "Signing AAX...")

        elseif (WIN32)

            add_custom_command (TARGET ${aaxTarget} POST_BUILD
                                COMMAND "${WRAPTOOL_PROGRAM}" ARGS sign --verbose --dsig1-compat off --account %AAX_DSIG_ACCOUNT% --keyfile "%AAX_DSIG_KEYFILE%" --keypassword %AAX_DSIG_KEYFILE_PASS% --wcguid %WCGUID% --in $<TARGET_FILE:${aaxTarget}> --out $<TARGET_FILE:${aaxTarget}>
                                VERBATIM COMMENT "Signing AAX...")
        endif()
    endif()

endfunction()
