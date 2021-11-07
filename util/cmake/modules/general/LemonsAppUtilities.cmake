#[[
Utilities for configuring JUCE GUI apps.

## Includes:
- LemonsJuceUtilities


## Functions:

```
lemons_configure_headless_app (TARGET <>)
```

Forwards ${ARGN} to lemons_configure_juce_target.

```
lemons_configure_juce_app (TARGET <>)
```

Forwards ${ARGN} to lemons_configure_juce_target.

]]


include_guard (GLOBAL)

include (LemonsJuceUtilities)



macro (_lemons_configure_app_internal)

    lemons_configure_juce_target (${ARGN})

    set (options "")
    set (oneValueArgs TARGET)
    set (multiValueArgs "")

    cmake_parse_arguments (LEMONS_APP "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (NOT TARGET ALL_APPS)
        add_custom_target (ALL_APPS COMMENT "Building all apps...")
    endif()
    
    add_dependencies (ALL_APPS ${LEMONS_APP_TARGET})
endmacro()

#

function (lemons_configure_headless_app)
    _lemons_configure_app_internal (${ARGN})
endfunction()

#

function (lemons_configure_juce_app)

    _lemons_configure_app_internal (${ARGN})

    target_link_libraries (${LEMONS_APP_TARGET} PUBLIC LemonsAppModules)

endfunction()
