#[[
Includes all available Lemons JUCE modules in your build, and provides a target to allow linking to all of them at once.

## Includes:
- LemonsAppModules
- LemonsPluginModules

## Targets:
- AllLemonsModules

]]

include_guard (GLOBAL)

include (LemonsAppModules)
include (LemonsPluginModules)


add_library (AllLemonsModules INTERFACE)
target_link_libraries (AllLemonsModules INTERFACE LemonsPluginModules LemonsAppModules)


option (LEMONS_BUILD_TESTS "Build the Lemons unit tests" OFF)

if (LEMONS_BUILD_TESTS)
	target_link_libraries (AllLemonsModules INTERFACE LemonsTestingModules)
endif()
