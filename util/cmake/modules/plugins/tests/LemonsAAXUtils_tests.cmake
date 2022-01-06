include_guard (GLOBAL)

if (NOT LEMONS_AAX_SDK_PATH)
    return()
endif()

include ("${CMAKE_CURRENT_LIST_DIR}/../LemonsAAXUtils.cmake")

#[[

## Targets:
- AAXSDK

## Functions:

### lemons_configure_aax_plugin {#lemons_configure_aax_plugin}

```
lemons_configure_aax_plugin (TARGET <target>
                            [PAGETABLE_FILE <file>]
                            [GUID <guid>])
```

### lemons_configure_aax_plugin_signing

```
lemons_configure_aax_plugin_signing (TARGET <target> GUID <guid> 
                                     ACCOUNT <account> SIGNID <signid> KEYFILE <keyfile> KEYPASSWORD <password>)
```

### lemons_set_aax_signing_settings
```
lemons_set_aax_signing_settings ([ACCOUNT <accountID>] 
                                 [SIGNID <signID>] 
                                 [KEYFILE <keyfilePath>] 
                                 [KEYPASSWORD <keyPassword>])
```

]]
