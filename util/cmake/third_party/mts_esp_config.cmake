CPMAddPackage (
        NAME MTS-ESP
        GIT_REPOSITORY https://github.com/ODDSound/MTS-ESP.git
        GIT_TAG origin/main)

add_library (MTS-ESP INTERFACE)

target_include_directories (MTS-ESP INTERFACE ${MTS-ESP_SOURCE_DIR}/Client)
