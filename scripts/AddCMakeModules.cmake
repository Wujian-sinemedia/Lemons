include_guard (GLOBAL)

include ("${CMAKE_CURRENT_LIST_DIR}/../util/cmake/modules/LemonsCmakeModulesConfig.cmake")

list (APPEND CMAKE_MODULE_PATH ${LEMONS_CMAKE_MODULE_PATH})

set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" CACHE INTERNAL "")
