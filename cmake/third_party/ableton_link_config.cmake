CPMAddPackage (
        NAME AbletonLink
        GIT_REPOSITORY https://github.com/Ableton/link.git
        GIT_TAG origin/master
        DOWNLOAD_ONLY YES)

include (${AbletonLink_SOURCE_DIR}/AbletonLinkConfig.cmake)

add_library (AbletonLink INTERFACE)
add_dependencies (AbletonLink Ableton::Link)

target_include_directories (AbletonLink INTERFACE ${AbletonLink_SOURCE_DIR}/include/ableton)
target_compile_definitions (AbletonLink INTERFACE BV_USE_ABLETON_LINK=1)