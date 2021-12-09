#[[
Common building blocks and utilities

## Includes:
- LemonsJuceModuleUtilities

## Targets:
- LemonsCommonModules

## Options:
- LEMONS_BUILD_TESTS, defaults to OFF

]]


include_guard (GLOBAL)

#

option (LEMONS_BUILD_TESTS "Build tests" OFF)

include (LemonsJuceModuleUtilities)

_lemons_add_module_subcategory (LemonsCommonModules)

target_compile_definitions (lemons_core INTERFACE 
        LEMONS_VERSION_MAJOR=${Lemons_VERSION_MAJOR} 
        LEMONS_VERSION_MINOR=${Lemons_VERSION_MINOR} 
        LEMONS_VERSION_PATCH=${Lemons_VERSION_PATCH}
        LEMONS_PROJECT_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
        LEMONS_PROJECT_VERSION_MINOR=${PROJECT_VERSION_MINOR}
        LEMONS_PROJECT_VERSION_PATCH=${PROJECT_VERSION_PATCH})

if (LEMONS_BUILD_TESTS)
    target_compile_definitions (lemons_core INTERFACE LEMONS_UNIT_TESTS=1)
endif()
