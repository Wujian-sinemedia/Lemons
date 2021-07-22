CPMAddPackage (
        NAME AbletonLink
        GIT_REPOSITORY https://github.com/Ableton/link.git
        GIT_TAG origin/master)

include (${AbletonLink_SOURCE_DIR}/AbletonLinkConfig.cmake)

target_include_directories (Ableton::Link INTERFACE ${AbletonLink_SOURCE_DIR}/include/ableton)
target_compile_definitions (Ableton::Link INTERFACE BV_USE_ABLETON_LINK=1)

#

function (_bv_configure_ableton_link target)
    target_link_libraries (${target} PUBLIC Ableton::Link)
    target_compile_definitions (${target} PUBLIC BV_USE_ABLETON_LINK=1)
endfunction()