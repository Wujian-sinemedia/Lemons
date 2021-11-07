# Lemons CMake API		{#CMake_API}


## CMake options

### Options for controlling the configuration of your build

#### LEMONS_AAX_SDK_PATH
If you are an Avid developer, you can set this variable to the absolute path of the AAX SDK root to enable using the AAX plugin format. If this is set, the Lemons CMake scripts will attempt to build the AAX SDK when you add the Lemons package -- note that this requires xcodebuild on Mac or msbuild on Windows.

#### LEMONS_VST2_SDK_PATH
If you have access to the VST2 SDK, you can define this variable to the absolute path of the VST2 SDK root to enable compiling VST2 plugins.

#### LEMONS_ENABLE_INTEGRATIONS
Set this to ON to enable all available integrations by default (ccache, clang-format, etc). Defaults to ON.

#### LEMONS_SANITIZERS_TO_ENABLE
An optional list of sanitizers to enable. Currently only supported with GNU, Clang, or MSVC compilers.
The options are:
- address
- memory
- thread
- leak
- undefined
- fuzzer

The sanitizers "address", "memory" and "thread" are mutually exclusive.  You cannot enable two of them in the same build.

"leak" requires the  "address" sanitizer.

#### LEMONS_PLUGINVAL_LEVEL
The "level" of strictness of the configured pluginval tests; 1 through 10. Defaults to 10.

#### LEMONS_LANGUAGE_LIST
An optional path to a text file, containing a line-separated list of target languages for generated translation files. If not provided, a default list of languages will be used. An example language list file might look like this:
```
French
German
Italian
Spanish
```


### Options for conditionally enabling building additional stuff

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
