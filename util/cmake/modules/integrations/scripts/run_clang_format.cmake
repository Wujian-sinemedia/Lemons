#!/usr/bin/env cmake -P

find_package (LemonsCmakeModules REQUIRED CONFIG PATHS @Lemons_SOURCE_DIR@/util/cmake/modules)

list (APPEND CMAKE_MODULE_PATH "${LEMONS_CMAKE_MODULE_PATH}")

include (LemonsClangFormat)

lemons_run_clang_format (DIRS @LEMONS_CF_DIRS@ EXCLUDE @LEMONS_CF_EXCLUDE_DIRS@)
