#!/usr/bin/env cmake -P

set (CMAKE_FIND_PACKAGE_PREFER_CONFIG TRUE)

find_package (LemonsCmakeModules REQUIRED PATHS ${CMAKE_CURRENT_LIST_DIR}/../util/cmake/modules)

list (APPEND CMAKE_MODULE_PATH ${LEMONS_CMAKE_MODULE_PATH})

include (LemonsRunClean)

option (LEMONS_WIPE_CLEAN "Wipe the cache as well as executing clean" OFF)

if(LEMONS_WIPE_CLEAN)
	lemons_run_clean (WIPE FILE ${CMAKE_CURRENT_LIST_DIR}/lemons_config.json DIR
					  ${CMAKE_CURRENT_LIST_DIR}/..)
else()
	lemons_run_clean (FILE ${CMAKE_CURRENT_LIST_DIR}/lemons_config.json DIR
					  ${CMAKE_CURRENT_LIST_DIR}/..)
endif()
