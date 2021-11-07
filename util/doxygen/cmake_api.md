# Lemons CMake API		{#CMake_API}

## CMake options

### Options for controlling the configuration of your build

#### LEMONS_AAX_SDK_PATH
If you are an Avid developer, you can set this variable to the absolute path of the AAX SDK root to enable using the AAX plugin format. If this is set, the Lemons CMake scripts will attempt to build the AAX SDK when you add the Lemons package -- note that this requires xcodebuild on Mac or msbuild on Windows.

#### LEMONS_VST2_SDK_PATH
If you have access to the VST2 SDK, you can define this variable to the absolute path of the VST2 SDK root to enable compiling VST2 plugins.

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

#### LEMONS_LANGUAGE_LIST

An optional path to a text file, containing a line-separated list of target languages for generated translation files. If not provided, a default list of languages will be used. An example language list file might look like this:
```
French
German
Italian
Spanish
```

#### LEMONS_PLUGINVAL_LEVEL

The "level" of strictness of the configured pluginval tests; 1 through 10. Defaults to 10.

### Options for conditionally enabling building additional stuff

#### LEMONS_BUILD_TESTS

Set this to ON to enable testing. Defaults to OFF. When enabled, using the plugin configuration functions will also automatically configure plugin QC tests.

#### LEMONS_BUILD_DOCS

Set this to ON to enable building the Lemons documentation (requires Doxygen, Graphviz, and Python3). Defaults to OFF.

#### LEMONS_BUILD_TEMPLATES

Set this to ON to enable building the Lemons template projects. Defaults to OFF.

#### LEMONS_IGNORE_ALL_EXTRAS

Set this to ON to disable tests, docs, and templates with one option.

---
