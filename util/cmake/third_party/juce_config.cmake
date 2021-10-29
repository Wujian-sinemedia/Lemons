option (LEMONS_USE_LV2_JUCE "Use the fork of JUCE that enables compiling to LV2 on Linux" OFF)

if (NOT APPLE AND NOT WIN32 AND LEMONS_USE_LV2_JUCE)
        set (juce_repo https://github.com/lv2-porting-project/JUCE)
        set (juce_tag origin/lv2)
else()
        set (juce_repo https://github.com/juce-framework/JUCE)
        set (juce_tag  origin/develop)
endif()

CPMAddPackage (
        NAME JUCE
        GIT_REPOSITORY ${juce_repo}.git
        GIT_TAG ${juce_tag}
        OPTIONS "JUCE_ENABLE_MODULE_SOURCE_GROUPS ON" "JUCE_BUILD_EXAMPLES OFF" "JUCE_BUILD_EXTRAS OFF")