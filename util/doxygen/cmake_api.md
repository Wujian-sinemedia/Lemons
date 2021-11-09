# Lemons CMake API		{#CMake_API}


## CMake options

#### LEMONS_ENABLE_INTEGRATIONS
Set this to ON to enable all available integrations by default (ccache, clang-format, etc). Defaults to ON.

#### LEMONS_BUILD_TESTS
Set this to ON to enable testing. Defaults to OFF. When enabled, using the plugin configuration functions will also automatically configure plugin QC tests.

#### LEMONS_BUILD_DOCS
Set this to ON to enable building the Lemons documentation (requires Doxygen, Graphviz, and Python3). Defaults to OFF.

#### LEMONS_BUILD_TEMPLATES
Set this to ON to enable building the Lemons template projects. Defaults to OFF.

#### LEMONS_IGNORE_ALL_EXTRAS
Set this to ON to disable tests, docs, and templates with one option. Defaults to OFF.

---

## CMake modules	{#CMake_modules}

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
