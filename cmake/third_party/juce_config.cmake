option (JUCE_ENABLE_MODULE_SOURCE_GROUPS "Use JUCE source groups" ON)
option (JUCE_BUILD_EXAMPLES "Build JUCE Examples" OFF)
option (JUCE_BUILD_EXTRAS "Build JUCE Extras" OFF)

if (NOT APPLE AND NOT WIN32)
        message (STATUS "Using the LV2 Porting Project fork of JUCE.")
        set (bv_juce_repo https://github.com/lv2-porting-project/JUCE)
        set (bv_juce_tag origin/lv2)
else()
        set (bv_juce_repo https://github.com/juce-framework/JUCE)
        set (bv_juce_tag  origin/develop)
endif()

CPMAddPackage (
        NAME JUCE
        GIT_REPOSITORY ${bv_juce_repo}.git
        GIT_TAG ${bv_juce_tag})