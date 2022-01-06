include_guard (GLOBAL)

include ("${CMAKE_CURRENT_LIST_DIR}/../LemonsPluginUtilities.cmake")

#[[

## Output variables:
- LEMONS_PLUGIN_FORMATS: list of all available plugin formats

## Options:
- LEMONS_VST2_SDK_PATH: if you have access to the VST2 SDK, you can define this variable to the absolute path of the VST2 SDK root to enable compiling VST2 plugins.
- LEMONS_BUILD_TESTS: if ON, this module will include LemonsPluginvalUtils, and calling `lemons_configure_juce_plugin` will also call `lemons_configure_pluginval_tests`. Defaults to OFF.


## Function:

### lemons_configure_juce_plugin
```
lemons_configure_juce_plugin (TARGET <target> 
                             [AAX_PAGETABLE_FILE <file>] [AAX_GUID <guid>])
```

]]

