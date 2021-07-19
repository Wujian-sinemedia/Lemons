if (DEFINED CACHE{BV_CPM_PATH})
    if (EXISTS $CACHE{BV_CPM_PATH})
        include ($CACHE{BV_CPM_PATH})
        return()
    endif()
endif()

# Using the local version - update it first!
message (STATUS "Updating committed version of CPM.cmake...")
    
file (REMOVE ${SHAREDCODE_LOCAL_CPM_PATH})

file (DOWNLOAD
    https://raw.githubusercontent.com/cpm-cmake/CPM.cmake/master/cmake/CPM.cmake
    ${SHAREDCODE_LOCAL_CPM_PATH})

set (BV_CPM_PATH ${SHAREDCODE_LOCAL_CPM_PATH} CACHE INTERNAL "Path to the CPM.cmake script")

include ($CACHE{BV_CPM_PATH})