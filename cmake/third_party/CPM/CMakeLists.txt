set (SHAREDCODE_LOCAL_CPM_PATH ${CMAKE_CURRENT_LIST_DIR}/CPM.cmake CACHE INTERNAL "Path to the locally committed copy of the CPM.cmake script")

include (get_cpm.cmake)
include (copy_cpm_to_cache.cmake)