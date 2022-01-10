#[[
Utilities for AAX plugins.

## Include-time actions:
Attempts to configure a target to build the AAX SDK based on the path you've provided to LEMONS_AAX_SDK_PATH. 
This module may return early if the AAXSDK target cannot be configured correctly, in which case the AAXSDK target will not exist and the functions provided by this module may not exist.
If the AAXSDK target is configured successfully, this module will call juce_set_aax_sdk_path() for you.

## Targets:
- AAXSDK

## Options:
- LEMONS_AAX_SDK_PATH: the absolute path of the AAX SDK root.


## Functions:

### lemons_configure_aax_plugin {#lemons_configure_aax_plugin}

```
lemons_configure_aax_plugin (TARGET <target>
                            [PAGETABLE_FILE <file>]
                            [GUID <guid>])
```
Configures default settings for the specified AAX plugin target. Note that `<target>` is the *literal* name of this plugin target, not the shared plugin target name!

`PAGETABLE_FILE` is optional and specifies the name of an AAX pagetable file within your resources target to use.

If `GUID` is present, `${ARGN}` will be forwarded to lemons_configure_aax_plugin_signing.


### lemons_configure_aax_plugin_signing

```
lemons_configure_aax_plugin_signing (TARGET <target> GUID <guid> 
                                     ACCOUNT <account> SIGNID <signid> KEYFILE <keyfile> KEYPASSWORD <password>)
```
Configures a post-build digital signing step for the specified AAX plugin target. Note that `<target>` is the *literal* name of this plugin target, not the shared plugin target name!

Does nothing if wraptool cannot be located at configure-time.


### lemons_set_aax_signing_settings
```
lemons_set_aax_signing_settings ([ACCOUNT <accountID>] 
                                 [SIGNID <signID>] 
                                 [KEYFILE <keyfilePath>] 
                                 [KEYPASSWORD <keyPassword>])
```
Configures some default settings for the lemons_configure_aax_plugin_signing() function, so that these settings don't have to be provided for each target you configure.

I recommend you create a `YourBrand.cmake` module in some shared location that each project can include, and that module can call this function at include-time with your settings.

]]


include_guard (GLOBAL)

include (${CMAKE_CURRENT_LIST_DIR}/scripts/create_aaxsdk_target.cmake)

#

if (NOT TARGET AAXSDK)
    message (AUTHOR_WARNING "Error configuring the AAXSDK target!")
    return()
endif()

include (LemonsFileUtils)
include (LemonsJuceUtilities)

juce_set_aax_sdk_path ("${LEMONS_AAX_SDK_PATH}")

#

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

#

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
        
        add_custom_command (TARGET ${LEMONS_AAX_TARGET} POST_BUILD VERBATIM
                            COMMAND "${WRAPTOOL_PROGRAM}" 
                            ARGS sign --verbose --dsig1-compat off --account "${LEMONS_AAX_ACCOUNT}" --wcguid "${LEMONS_AAX_GUID}" --signid "${LEMONS_AAX_SIGNID}" --in "$<TARGET_PROPERTY:${aaxTarget},JUCE_PLUGIN_ARTEFACT_FILE>" --out "$<TARGET_PROPERTY:${aaxTarget},JUCE_PLUGIN_ARTEFACT_FILE>"
                            COMMENT "Signing AAX...")
    elseif (WIN32)
        lemons_require_function_arguments (LEMONS_AAX KEYFILE KEYPASSWORD)

        add_custom_command (TARGET ${LEMONS_AAX_TARGET} POST_BUILD VERBATIM
                            COMMAND "${WRAPTOOL_PROGRAM}" 
                            ARGS sign --verbose --dsig1-compat off --account "${LEMONS_AAX_ACCOUNT}" --keyfile "${LEMONS_AAX_KEYFILE}" --keypassword "${LEMONS_AAX_KEYPASSWORD}" --wcguid "${LEMONS_AAX_GUID}" --in "$<TARGET_PROPERTY:${aaxTarget},JUCE_PLUGIN_ARTEFACT_FILE>" --out "$<TARGET_PROPERTY:${aaxTarget},JUCE_PLUGIN_ARTEFACT_FILE>"
                            COMMENT "Signing AAX...")
    endif()
endfunction()


#


function (lemons_configure_aax_plugin)

    set (oneValueArgs TARGET PAGETABLE_FILE)

    cmake_parse_arguments (LEMONS_AAX "SIGN" "${oneValueArgs}" "" ${ARGN})

    lemons_require_function_arguments (LEMONS_AAX TARGET)
    lemons_check_for_unparsed_args (LEMONS_AAX)

    if (NOT TARGET ${LEMONS_AAX_TARGET})
        message (AUTHOR_WARNING "AAX target does not exist!")
        return()
    endif()

    if (NOT TARGET AAXSDK)
        message (FATAL_ERROR "AAX plugin target created, but AAXSDK target doesn't exist!")
    endif()

    set_target_properties (${LEMONS_AAX_TARGET} PROPERTIES OSX_ARCHITECTURES x86_64)

    add_dependencies (${LEMONS_AAX_TARGET} AAXSDK)

    if (LEMONS_AAX_PAGETABLE_FILE)

        lemons_make_path_absolute (VAR LEMONS_AAX_PAGETABLE_FILE 
                                   BASE_DIR ${PROJECT_SOURCE_DIR})

        cmake_path (IS_ABSOLUTE LEMONS_AAX_PAGETABLE_FILE pagetable_path_is_absolute)

        target_compile_definitions (${LEMONS_AAX_TARGET} PRIVATE "JucePlugin_AAXPageTableFile=\"${LEMONS_AAX_PAGETABLE_FILE}\"")

        if (WIN32)
            # On Windows, pagetable files need a special post-build copy step to be included in the binary correctly
            add_custom_command (TARGET ${LEMONS_AAX_TARGET} POST_BUILD VERBATIM
                                COMMAND "${CMAKE_COMMAND}" ARGS -E copy "${LEMONS_AAX_PAGETABLE_FILE}" "$<TARGET_PROPERTY:${LEMONS_AAX_TARGET},JUCE_PLUGIN_ARTEFACT_FILE>/Contents/Resources"
                                COMMENT "Copying AAX pagetable into AAX binary...")
        endif()
    endif()

    if (LEMONS_AAX_GUID)
        lemons_configure_aax_plugin_signing (${ARGN})
    endif()
endfunction()
