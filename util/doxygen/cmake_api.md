## CMake modules {#CMake_modules}

Lemons is designed to be as modular as possible; its CMake features, integrations, and the JUCE modules themselves (and even JUCE itself) are all included by projects via CMake module includes. For example, a basic plugin's CMakeLists.txt might look something like this:

```
cmake_minimum_required (VERSION 3.19)

project (NewPlugin VERSION 0.0.1 LANGUAGES CXX)

add_subdirectory (GetLemons)

include (LemonsPluginModules)

juce_add_plugin (...)

lemons_configure_juce_plugin (TARGET ${PROJECT_NAME} ASSET_FOLDER assets TRANSLATIONS)
```

It is possible to use Lemons only for its default CMake set-up, without including any of the JUCE modules in the build tree at all. Categories of JUCE modules can be included by the relevant module in the 'Juce module inclusion' category below:
