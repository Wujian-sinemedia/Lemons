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

set (BV_CPM_PATH ${CMAKE_CURRENT_LIST_DIR}/CPM.cmake CACHE INTERNAL "Path to the CPM.cmake script")

if (DEFINED ENV{CPM_SOURCE_CACHE})
    set (cpm_cache_path $ENV{CPM_SOURCE_CACHE}/CPM.cmake)

    if (EXISTS ${cpm_cache_path})
        set (BV_CPM_PATH ${cpm_cache_path} CACHE INTERNAL "Path to the CPM.cmake script")
        _bv_include_cpm()
        return()
    else()
        file (READ ${BV_CPM_PATH} CPMscriptText)
        file (WRITE ${cpm_cache_path} ${CPMscriptText})
    endif()
endif()

_bv_include_cpm()