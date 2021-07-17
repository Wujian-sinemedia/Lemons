if (DEFINED BV_CPM_INCLUDED)
    if (${BV_CPM_INCLUDED})
        return()
    endif()
endif()

macro (_bv_include_cpm)
    include (${BV_CPM_PATH})
    set (BV_CPM_INCLUDED TRUE PARENT_SCOPE)
endmacro()

if (DEFINED BV_CPM_PATH)
    if (EXISTS ${BV_CPM_PATH})
        _bv_include_cpm()
        return()
    endif()
endif()

set (BV_CPM_PATH ${SHAREDCODE_LOCAL_CPM_PATH} CACHE INTERNAL "Path to the CPM.cmake script")

_bv_include_cpm()