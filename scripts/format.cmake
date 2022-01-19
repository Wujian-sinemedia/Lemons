#!/usr/bin/env cmake -P


find_package (LemonsCmakeModules REQUIRED CONFIG
			  PATHS ${CMAKE_CURRENT_LIST_DIR}/../util/cmake/modules)

list (APPEND CMAKE_MODULE_PATH ${LEMONS_CMAKE_MODULE_PATH})


include (LemonsClangFormat)

lemons_run_clang_format (DIRS ${CMAKE_CURRENT_LIST_DIR}/../modules ${CMAKE_CURRENT_LIST_DIR}/../util)
