if (DEFINED BV_CPM_PATH)
    if (EXISTS ${BV_CPM_PATH})
        include (${BV_CPM_PATH})
        return()
    endif()
endif()

set (BV_CPM_PATH ${SHAREDCODE_LOCAL_CPM_PATH} CACHE INTERNAL "Path to the CPM.cmake script")

include (${BV_CPM_PATH})