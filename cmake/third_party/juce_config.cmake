option (JUCE_ENABLE_MODULE_SOURCE_GROUPS "Use JUCE source groups" ON)
option (JUCE_BUILD_EXAMPLES "Build JUCE Examples" OFF)
option (JUCE_BUILD_EXTRAS "Build JUCE Extras" OFF)

CPMAddPackage (
        NAME JUCE
        GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
        GIT_TAG origin/develop)