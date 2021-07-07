
option (JUCE_ENABLE_MODULE_SOURCE_GROUPS "Use JUCE source groups" ON)
option (JUCE_BUILD_EXAMPLES "Build JUCE Examples" OFF)
option (JUCE_BUILD_EXTRAS "Build JUCE Extras" OFF)

add_subdirectory (JUCE)

set (BV_THIRD_PARTY_DIR "${CMAKE_CURRENT_LIST_DIR}" CACHE INTERNAL "Directory of third-party submodules")

add_subdirectory (cmake)
