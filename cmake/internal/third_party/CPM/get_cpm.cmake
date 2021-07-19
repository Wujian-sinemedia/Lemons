if (DEFINED CACHE{BV_CPM_PATH})
    if (EXISTS $CACHE{BV_CPM_PATH})
        include ($CACHE{BV_CPM_PATH})
        return()
    endif()
endif()

set (BV_CPM_PATH ${SHAREDCODE_LOCAL_CPM_PATH} CACHE INTERNAL "Path to the CPM.cmake script")

include ($CACHE{BV_CPM_PATH})