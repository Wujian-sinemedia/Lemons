#!/usr/bin/env cmake -P

find_package (LemonsCmakeModules REQUIRED CONFIG)

list (APPEND CMAKE_MODULE_PATH "${LEMONS_CMAKE_MODULE_PATH}")

include (LemonsRunClangFormat)

lemons_run_clang_format (DIRS @LEMONS_CF_DIRS@
						 EXCLUDE @LEMONS_CF_EXCLUDE_DIRS@)
