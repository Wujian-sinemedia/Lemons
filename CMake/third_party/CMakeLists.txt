
include (cmake/get_cpm.cmake)

option (JUCE_ENABLE_MODULE_SOURCE_GROUPS "Use JUCE source groups" ON)
option (JUCE_BUILD_EXAMPLES "Build JUCE Examples" OFF)
option (JUCE_BUILD_EXTRAS "Build JUCE Extras" OFF)

CPMAddPackage (
        NAME JUCE
        GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
        GIT_TAG origin/develop)

CPMAddPackage (
        NAME MTS-ESP
        GIT_REPOSITORY https://github.com/ODDSound/MTS-ESP.git
        GIT_TAG origin/main)

set (bv_mts_esp_dir ${MTS-ESP_SOURCE_DIR} CACHE INTERNAL "Path to MTS-ESP sources")

CPMAddPackage (
        NAME AbletonLink
        GIT_REPOSITORY https://github.com/Ableton/link.git
        GIT_TAG origin/master)

set (bv_ableton_link_dir ${AbletonLink_SOURCE_DIR} CACHE INTERNAL "Path to Ableton Link sources")

add_subdirectory (cmake)
