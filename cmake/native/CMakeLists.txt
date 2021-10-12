if (APPLE)
    include (Apple.cmake)
elseif (WIN32)
    include (Windows.cmake)
else()
    include (Linux.cmake)
    return()
endif()


include (CheckIPOSupported)

check_ipo_supported (RESULT result OUTPUT output)

if (result)
    set (CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
else()
    set (CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF)
endif()