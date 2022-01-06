#[[
Utilities for configuring JUCE GUI apps.

## Functions:

### lemons_configure_headless_app
```
lemons_configure_headless_app (TARGET <>)
```

Forwards `${ARGN}` to [lemons_configure_juce_target](@ref lemons_configure_juce_target).


### lemons_configure_juce_app
```
lemons_configure_juce_app (TARGET <>)
```

Forwards `${ARGN`} to [lemons_configure_juce_target](@ref lemons_configure_juce_target).

]]


include_guard (GLOBAL)

include (LemonsJuceUtilities)
include (lemons_internal)
include (LemonsCmakeDevTools)

lemons_warn_if_not_processing_project()

#

macro (_lemons_configure_app_internal)
    lemons_configure_juce_target (${ARGN})

    set (oneValueArgs TARGET)
    cmake_parse_arguments (LEMONS_APP "" "${oneValueArgs}" "" ${ARGN})

    _lemons_add_to_all_apps_target (${LEMONS_APP_TARGET})
endmacro()

#

function (lemons_configure_headless_app)
    _lemons_configure_app_internal (${ARGN})
    target_link_libraries (${LEMONS_APP_TARGET} PUBLIC Lemons::LemonsCommonModules)
endfunction()

#

function (lemons_configure_juce_app)
    _lemons_configure_app_internal (${ARGN})
    target_link_libraries (${LEMONS_APP_TARGET} PUBLIC Lemons::LemonsAppModules)
endfunction()
