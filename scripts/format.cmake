#!/usr/bin/env cmake -P


set (CMAKE_FIND_PACKAGE_PREFER_CONFIG TRUE)

find_package (LemonsCmakeModules REQUIRED
			  PATHS ${CMAKE_CURRENT_LIST_DIR}/../util/cmake/modules)

list (APPEND CMAKE_MODULE_PATH ${LEMONS_CMAKE_MODULE_PATH})


include (LemonsRunClangFormat)

lemons_run_clang_format (DIRS ${CMAKE_CURRENT_LIST_DIR}/../modules ${CMAKE_CURRENT_LIST_DIR}/../util)
