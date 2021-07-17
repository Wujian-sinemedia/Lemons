set (cpm_local_path ${CMAKE_CURRENT_LIST_DIR}/CPM.cmake)

if (DEFINED ENV{CPM_SOURCE_CACHE})
    set (cpm_cache_path $ENV{CPM_SOURCE_CACHE}/CPM.cmake)

    if (NOT EXISTS ${cpm_cache_path})
        file (READ ${cpm_local_path} CPMscriptText)
        file (WRITE ${cpm_cache_path} ${CPMscriptText})
    endif()
endif()

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

set (BV_CPM_PATH ${cpm_local_path} CACHE INTERNAL "Path to the CPM.cmake script")

_bv_include_cpm()