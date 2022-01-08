#!/usr/bin/env cmake -P

include (${CMAKE_CURRENT_LIST_DIR}/AddCMakeModules.cmake)

list (APPEND CMAKE_MODULE_PATH ${LEMONS_CMAKE_MODULE_PATH})

include (LemonsRunClean)


lemons_run_clean (FILE lemons_config.json DIR ${CMAKE_CURRENT_LIST_DIR})
