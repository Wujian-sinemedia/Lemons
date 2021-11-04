include (LemonsJuceUtilities)

#

# Ableton Link

CPMAddPackage (
        NAME AbletonLink
        GITHUB_REPOSITORY Ableton/link
        GIT_TAG origin/master
        DOWNLOAD_ONLY YES)

include (${AbletonLink_SOURCE_DIR}/AbletonLinkConfig.cmake)

add_library (AbletonLink INTERFACE)
add_dependencies (AbletonLink Ableton::Link)

target_include_directories (AbletonLink INTERFACE "${AbletonLink_SOURCE_DIR}/include/ableton")

#

# MTS-ESP

CPMAddPackage (
        NAME MTS-ESP
        GITHUB_REPOSITORY ODDSound/MTS-ESP
        GIT_TAG origin/main)

add_library (MTS-ESP INTERFACE)

target_include_directories (MTS-ESP INTERFACE "${MTS-ESP_SOURCE_DIR}/Client")

#
