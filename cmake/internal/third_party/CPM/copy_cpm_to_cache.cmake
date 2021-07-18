if (NOT DEFINED ENV{CPM_SOURCE_CACHE})
    return()
endif()

set (cpm_cache_path $ENV{CPM_SOURCE_CACHE}/CPM.cmake)

if (EXISTS ${cpm_cache_path})
    return()
endif()

bv_copy_file (${BV_CPM_PATH} ${cpm_cache_path})
