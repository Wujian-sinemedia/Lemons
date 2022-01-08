#!/usr/bin/env cmake -P

include (${CMAKE_CURRENT_LIST_DIR}/AddCMakeModules.cmake)

list (APPEND CMAKE_MODULE_PATH ${LEMONS_CMAKE_MODULE_PATH})


include (LemonsRunClangFormat)

lemons_run_clang_format (DIRS ${CMAKE_CURRENT_LIST_DIR}/modules ${CMAKE_CURRENT_LIST_DIR}/util)
