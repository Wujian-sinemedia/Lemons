CPMAddPackage (
        NAME MTS-ESP
        GIT_REPOSITORY https://github.com/ODDSound/MTS-ESP.git
        GIT_TAG origin/main)

set (mts_esp_dir ${MTS-ESP_SOURCE_DIR}/Client)

add_library (MTS-ESP INTERFACE)

target_sources             (MTS-ESP INTERFACE "${mts_esp_dir}/libMTSClient.cpp")
target_include_directories (MTS-ESP INTERFACE ${mts_esp_dir})
target_compile_definitions (MTS-ESP INTERFACE BV_USE_MTS_ESP=1)
